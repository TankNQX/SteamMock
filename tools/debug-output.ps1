# The games' own words, from the wire the games use for them.
#
# A Windows process says things with OutputDebugString, and every line goes into one
# machine-wide buffer that a single reader at a time can drain: DBWIN_BUFFER, with
# DBWIN_BUFFER_READY meaning "the buffer is yours to fill" and DBWIN_DATA_READY
# meaning "there is a line in it". This is that reader. It exists because the harness
# records every call a game makes and none of the opinions a game has about them -
# and a game's opinion is where it says why it is unhappy, which is otherwise only
# visible under a debugger.
#
#   powershell -File tools/debug-output.ps1 -OutFile game.log
#   powershell -File tools/debug-output.ps1 -OutFile game.log -Seconds 90 -OnlyPids 1234,5678
#
# Each line is written as "<pid> <text>", so a run with two games in it can say whose
# line is whose. Two things to know: only one reader can hold the buffer, so a
# debugger, an IDE or DebugView running at the same time will take these lines, and
# the buffer is written by the process that calls OutputDebugString - a reader that
# stops draining it makes those calls block, which is why the handshake below is
# kept tight.
param(
    [Parameter(Mandatory = $true)][string] $OutFile,
    [int] $Seconds = 0,
    [int[]] $OnlyPids = @()
)

$ErrorActionPreference = 'Stop'

Add-Type -TypeDefinition @'
using System;
using System.Runtime.InteropServices;
public static class DbgWin {
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern IntPtr CreateFileMapping(IntPtr hFile, IntPtr attributes, uint protect,
                                                  uint maximumHigh, uint maximumLow, string name);
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern IntPtr MapViewOfFile(IntPtr mapping, uint access, uint offsetHigh,
                                              uint offsetLow, UIntPtr bytes);
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern IntPtr CreateEvent(IntPtr attributes, bool manualReset, bool initialState,
                                           string name);
    [DllImport("kernel32.dll")]
    public static extern bool SetEvent(IntPtr handle);
    [DllImport("kernel32.dll")]
    public static extern uint WaitForSingleObject(IntPtr handle, uint milliseconds);
    [DllImport("kernel32.dll")]
    public static extern bool CloseHandle(IntPtr handle);
}
'@

$PAGE_READWRITE = 0x04
$FILE_MAP_READ = 0x0004
$BUFFER_BYTES = 4096
$WAIT_TIMEOUT = 258

# The mapping is created with a size of the buffer plus the process id that precedes
# it, and it is read-only on this side: a reader never writes the text, it only says
# when the buffer is free again.
$mapping = [DbgWin]::CreateFileMapping([IntPtr](-1), [IntPtr]::Zero, $PAGE_READWRITE, 0,
                                       $BUFFER_BYTES + 4, 'DBWIN_BUFFER')
if ($mapping -eq [IntPtr]::Zero) { throw 'could not create the DBWIN_BUFFER mapping' }
$view = [DbgWin]::MapViewOfFile($mapping, $FILE_MAP_READ, 0, 0, [UIntPtr]::Zero)
if ($view -eq [IntPtr]::Zero) { throw 'could not map the DBWIN buffer' }
$ready = [DbgWin]::CreateEvent([IntPtr]::Zero, $false, $false, 'DBWIN_BUFFER_READY')
$dataReady = [DbgWin]::CreateEvent([IntPtr]::Zero, $false, $false, 'DBWIN_DATA_READY')

$out = [System.IO.StreamWriter]::new($OutFile, $true)
$out.AutoFlush = $true

$deadline = if ($Seconds -gt 0) { (Get-Date).AddSeconds($Seconds) } else { [datetime]::MaxValue }
$count = 0

while ((Get-Date) -lt $deadline) {
    # Hands the buffer to whoever wants to write into it, then waits for a line.
    [void] [DbgWin]::SetEvent($ready)
    $waited = [DbgWin]::WaitForSingleObject($dataReady, 1000)
    if ($waited -eq $WAIT_TIMEOUT) { continue }
    if ($waited -ne 0) { break }

    $writer = [System.Runtime.InteropServices.Marshal]::ReadInt32($view)
    $text = [System.Runtime.InteropServices.Marshal]::PtrToStringAnsi([IntPtr]::Add($view, 4))
    if ($OnlyPids.Count -gt 0 -and $OnlyPids -notcontains $writer) { continue }
    $out.WriteLine("$writer $($text.TrimEnd([char]13))")
    $count++
}

$out.Flush()
$out.Dispose()
[void] [DbgWin]::CloseHandle($dataReady)
[void] [DbgWin]::CloseHandle($ready)
[void] [DbgWin]::CloseHandle($mapping)
Write-Host "debug-output: $count line(s) to $OutFile"
