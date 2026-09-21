# ---------------------------------------------------------------------------
#  check-format.ps1 - fail if a hand-written file is not clang-format clean.
# ---------------------------------------------------------------------------
#  The generated files are deliberately out of scope: a test byte-compares them
#  against what steambridge_codegen writes, so a formatter must never touch one.
#  The same goes for external/, which is other people's code.
#
#  Run it the way CI does:
#
#      pwsh -File tools/check-format.ps1
#
#  Exits 0 when every file in scope is clean, 1 when one is not, 2 when
#  clang-format cannot be found.
# ---------------------------------------------------------------------------

# Continue, not Stop: clang-format reports its violations on stderr, and a
# native command's stderr becomes an error record under Stop - which would end
# the run before the file that caused it could be named.
$ErrorActionPreference = 'Continue'

$root = Split-Path -Parent $PSScriptRoot
Push-Location $root
try {
    # clang-format comes from an LLVM install, or from the Visual Studio C++
    # Clang component. Look for both rather than assume one layout - and skip
    # the ARM64 toolset, which is present on an x64 install but cannot run here.
    $candidates = @()
    $onPath = Get-Command clang-format -ErrorAction SilentlyContinue
    if ($onPath) { $candidates += $onPath.Source }
    $candidates += 'C:\Program Files\LLVM\bin\clang-format.exe'
    $candidates += (Get-ChildItem `
        'C:\Program Files\Microsoft Visual Studio\*\*\VC\Tools\Llvm\*\bin\clang-format.exe', `
        'C:\Program Files (x86)\Microsoft Visual Studio\*\*\VC\Tools\Llvm\*\bin\clang-format.exe' `
        -ErrorAction SilentlyContinue |
        Where-Object { $_.FullName -notmatch '\\ARM64\\' } |
        Sort-Object { if ($_.FullName -match '\\x64\\') { 0 } else { 1 } } |
        Select-Object -ExpandProperty FullName)

    $clangFormat = $candidates | Where-Object { $_ -and (Test-Path $_) } | Select-Object -First 1
    if (-not $clangFormat) {
        Write-Host 'clang-format was not found.'
        Write-Host 'Install LLVM, or the Visual Studio "C++ Clang Compiler for Windows" component.'
        exit 2
    }

    $files = @(git ls-files '*.cpp' '*.hpp' '*.h') |
        Where-Object { $_ -notlike 'external/*' -and $_ -notlike 'src/generated/*' }

    $dirty = @()
    foreach ($file in $files) {
        $report = & $clangFormat --style=file --dry-run --Werror $file 2>&1
        if ($LASTEXITCODE -ne 0) {
            $dirty += $file
            if ($dirty.Count -le 10) {
                Write-Host ("  {0}: {1}" -f $file, (($report | Select-Object -First 1) -join ''))
            }
        }
    }

    if ($dirty.Count -gt 0) {
        Write-Host ''
        Write-Host ("{0} of {1} files are not clang-format clean." -f $dirty.Count, $files.Count)
        Write-Host 'Fix with: clang-format --style=file -i <file>'
        exit 1
    }

    Write-Host ("clang-format clean: {0} files" -f $files.Count)
    exit 0
}
finally {
    Pop-Location
}
