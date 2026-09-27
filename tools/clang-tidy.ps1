# ---------------------------------------------------------------------------
#  clang-tidy.ps1 - fail if a hand-written file has a finding .clang-tidy names.
# ---------------------------------------------------------------------------
#  The config next to this is a gate rather than a suggestion: this runs it over
#  every hand-written translation unit and exits non-zero when there is anything
#  to say, so a finding is a decision somebody makes - fix it, or write down in
#  .clang-tidy why it is not one.
#
#      pwsh -File tools/clang-tidy.ps1
#
#  Exits 0 when the tree is clean under .clang-tidy, 1 when it is not, and 2 when
#  the run cannot be trusted at all: no clang-tidy, no clang-cl or ninja to build
#  a compile database with, or nothing found to check.
#
#  Why a compile database: clang-tidy decides what a file means from the flags it
#  was compiled with, and only the Ninja and Makefile generators write
#  compile_commands.json - the Visual Studio generator, which the three build
#  trees here use, does not. So this configures its own Ninja tree in a temporary
#  directory, on the same compiler the stub is built with, and throws nothing but
#  the database away. It does not build: clang-tidy compiles each file itself.
#
#  The GUI is in scope when its submodules are there, because a source nobody
#  checks is a source that drifts - and a compile database that lacks its
#  include paths makes clang-tidy mis-report it rather than skip it.
# ---------------------------------------------------------------------------

# Continue, not Stop: clang-tidy reports its findings on stderr, and a native
# command's stderr becomes an error record under Stop - which would end the run
# before the file that caused it could be named.
$ErrorActionPreference = 'Continue'

$root = Split-Path -Parent $PSScriptRoot

# The LLVM tools come from an LLVM install or from the Visual Studio C++ Clang
# component. Look for both rather than assume one layout, and skip the ARM64
# toolset, which is present on an x64 install but cannot run here.
function Find-Tool([string] $name) {
    $onPath = Get-Command $name -ErrorAction SilentlyContinue
    $found = @()
    if ($onPath) { $found += $onPath.Source }
    $found += (Join-Path 'C:\Program Files\LLVM\bin' "$name.exe")
    $found += (Get-ChildItem `
        "C:\Program Files\Microsoft Visual Studio\*\*\VC\Tools\Llvm\*\bin\$name.exe", `
        "C:\Program Files (x86)\Microsoft Visual Studio\*\*\VC\Tools\Llvm\*\bin\$name.exe" `
        -ErrorAction SilentlyContinue |
        Where-Object { $_.FullName -notmatch '\\ARM64\\' } |
        Sort-Object { if ($_.FullName -match '\\x64\\') { 0 } else { 1 } } |
        Select-Object -ExpandProperty FullName)
    return $found | Where-Object { $_ -and (Test-Path $_) } | Select-Object -First 1
}

$clangTidy = Find-Tool 'clang-tidy'
$clangCl = Find-Tool 'clang-cl'

# Ninja is the one generator beside Makefile that writes a compile database. It
# ships with Visual Studio as well as on its own.
$ninja = $null
$onPath = Get-Command ninja -ErrorAction SilentlyContinue
if ($onPath) { $ninja = $onPath.Source }
if (-not $ninja) {
    $ninja = (Get-ChildItem `
        'C:\Program Files\Microsoft Visual Studio\*\*\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe', `
        'C:\Program Files (x86)\Microsoft Visual Studio\*\*\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe' `
        -ErrorAction SilentlyContinue |
        Select-Object -ExpandProperty FullName | Select-Object -First 1)
}

foreach ($tool in @($clangTidy, $clangCl, $ninja)) {
    if (-not $tool) {
        Write-Host 'clang-tidy, clang-cl or ninja was not found.'
        Write-Host 'Install LLVM (or the Visual Studio "C++ Clang Compiler for Windows" component) and ninja.'
        exit 2
    }
}

# clang-cl finds the C++ standard library by itself, but CMake's compiler check also
# *links*, and `lld-link` needs the SDK and CRT libraries on `LIB` and their headers on
# `INCLUDE`. Those come from the Visual Studio developer environment, which this takes
# rather than requiring the caller to be in a developer prompt.
function Use-MsvcEnvironment {
    $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
    $candidates = @()
    if (Test-Path $vswhere) {
        $install = & $vswhere -latest -products * `
            -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath 2>$null
        if ($install) { $candidates += (Join-Path $install 'VC\Auxiliary\Build\vcvars64.bat') }
    }
    $candidates += (Get-ChildItem `
        'C:\Program Files\Microsoft Visual Studio\*\*\VC\Auxiliary\Build\vcvars64.bat', `
        'C:\Program Files (x86)\Microsoft Visual Studio\*\*\VC\Auxiliary\Build\vcvars64.bat' `
        -ErrorAction SilentlyContinue | Select-Object -ExpandProperty FullName)
    $vcvars = $candidates | Where-Object { $_ -and (Test-Path $_) } | Select-Object -First 1
    if (-not $vcvars) { return $false }

    # `set` after vcvars prints the environment it made; take that into this process so
    # cmake runs in it. Only children ever see this, so nothing leaks into the caller.
    foreach ($line in (& cmd /c "`"$vcvars`" >nul && set" 2>$null)) {
        if ($line -match '^([^=]+)=(.*)$') {
            Set-Item -Path ('env:' + $matches[1]) -Value $matches[2] -ErrorAction SilentlyContinue
        }
    }
    return $true
}

if (-not (Use-MsvcEnvironment)) {
    Write-Host 'no vcvars64.bat was found, so cmake cannot link and no database can be written.'
    Write-Host 'Install the Visual Studio "Desktop development with C++" workload, or run this from a developer prompt.'
    exit 2
}

$database = Join-Path ([System.IO.Path]::GetTempPath()) 'steammock-clang-tidy'

Push-Location $root
try {
    # The header filter is left alone: .clang-tidy decides what is read, and a
    # finding in a hand-written header is a finding in the file that defines it.
    $configure = @(
        '-S', $root,
        '-B', $database,
        '-G', 'Ninja',
        "-DCMAKE_MAKE_PROGRAM=$ninja",
        "-DCMAKE_C_COMPILER=$clangCl",
        "-DCMAKE_CXX_COMPILER=$clangCl",
        '-DCMAKE_BUILD_TYPE=Release',
        '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON'
    )
    $gui = Test-Path (Join-Path $root 'external/imgui/imgui.cpp')
    $configure += "-DSTEAMMOCK_BUILD_GUI=$(if ($gui) { 'ON' } else { 'OFF' })"

    & cmake @configure 2>&1 | Out-Null
    $databaseFile = Join-Path $database 'compile_commands.json'
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path $databaseFile)) {
        Write-Host 'cmake could not write a compile database, so nothing was checked.'
        exit 2
    }

    # The hand-written sources, which is everything the gate is about. The
    # generated ones are outputs of a build and are compared byte for byte by a
    # test of their own; checking them here would report on the generator twice.
    $files = @(Get-ChildItem src/*.cpp, tests/*.cpp | Select-Object -ExpandProperty FullName)
    if (-not $gui) {
        $files = @($files | Where-Object { $_ -notlike '*gui_main.cpp' })
    }
    if ($files.Count -eq 0) {
        Write-Host 'no files were found to check, which is not the same as clean.'
        exit 2
    }

    # --warnings-as-errors makes a finding the exit code, which is what a gate
    # needs: clang-tidy exits 0 with warnings otherwise.
    $report = & $clangTidy -p $database "--warnings-as-errors=*" @files 2>&1
    $report | Where-Object { $_ -match ': (warning|error):' } | ForEach-Object { Write-Host "  $_" }

    if ($LASTEXITCODE -ne 0) {
        $findings = @($report | Where-Object { $_ -match ': (warning|error):' }).Count
        Write-Host ''
        Write-Host ("clang-tidy is not clean: {0} finding(s) across {1} files." -f $findings, $files.Count)
        Write-Host 'Fix them, or say why in .clang-tidy - see tools/clang-tidy.ps1.'
        exit 1
    }

    Write-Host ("clang-tidy clean: {0} files" -f $files.Count)
    exit 0
}
finally {
    Pop-Location
}
