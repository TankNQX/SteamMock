# The two-instance Spacewar run, scripted.
#
# Two copies of Valve's own test app, one session, one lobby, then the owner
# starting the game: the thing the harness exists for, driven end to end on a real
# 32-bit game and reported from the backend's own transcript. It leaves the rig (a
# copy of the game carrying the 32-bit stub) in -RigDir and the evidence beside it:
# transcript.jsonl, a.log and b.log.
#
#   pwsh -File tools/two-instance-test.ps1
#   pwsh -File tools/two-instance-test.ps1 -Shoot      # also capture each window
#   pwsh -File tools/two-instance-test.ps1 -Record -WaitAfterStart 90
#                                                       # tile the two windows and film them for
#                                                       # that many seconds (needs ffmpeg)
#   pwsh -File tools\two-instance-test.ps1 -Gui -Record # the live view is the server, so this
#                                                       # films it between the two clients
#
# Everything below is Windows-only and needs a built tree: -RepoRoot\build\Release
# (the backend) and -RepoRoot\build-w32\Release (the 32-bit stub). See the walkthrough
# in docs/two-instance-test.md for what the run is meant to show and what it stops at.
param(
    [string] $RepoRoot = (Split-Path -Parent (Split-Path -Parent $PSCommandPath)),
    [string] $GamePath = 'D:\Games\Steam\steamapps\common\Spacewar',
    [string] $RigDir = (Join-Path $env:TEMP 'sw-two'),
    [int] $WaitAfterStart = 25,
    [switch] $Shoot,
    [switch] $Record,
    [switch] $Gui
)

$ErrorActionPreference = 'Continue'

$game = Join-Path $RigDir 'game'
# The live view is the same server with a window on it, so -Gui is one line down here: the
# games do not know or care which of the two is listening, they just connect to it.
$server = Join-Path $RepoRoot 'build\Release\steammock.exe'
if ($Gui) { $server = Join-Path $RepoRoot 'build\Release\steammock_gui.exe' }
$stub = Join-Path $RepoRoot 'build-w32\Release\steam_api64.dll'
$scenario = Join-Path $RepoRoot 'scenarios\spacewar.json'
$transcript = Join-Path $RigDir 'transcript.jsonl'

function Say($message) { '{0:HH:mm:ss} {1}' -f (Get-Date), $message }

foreach ($needed in @($server, $stub, $scenario, (Join-Path $GamePath 'SteamworksExample.exe'))) {
    if (-not (Test-Path $needed)) {
        Say "missing $needed - build the tree, or point -GamePath at the game"
        exit 2
    }
}

Say 'setup: copying the game out of the library, so the install is never touched'
Remove-Item -Recurse -Force $game -ErrorAction SilentlyContinue
Remove-Item -Force $transcript, (Join-Path $RigDir 'a.log'), (Join-Path $RigDir 'b.log') -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force -Path $RigDir | Out-Null
Copy-Item -Recurse -Force $GamePath $game
Copy-Item -Force $stub (Join-Path $game 'steam_api.dll')
Say ('setup: the copy holds the stub ({0} bytes) in place of Valve''s' -f (Get-Item (Join-Path $game 'steam_api.dll')).Length)

# The games' own OutputDebugString lines, which is where a game says why it is
# unhappy - the harness records the calls and none of the opinions. Started before
# the instances: the buffer is drained one line at a time, and a line nobody is
# holding is a line gone.
$debugLog = Join-Path $RigDir 'game-output.log'
Remove-Item -Force $debugLog -ErrorAction SilentlyContinue
$debugReader = Start-Process -FilePath 'powershell' -PassThru -WindowStyle Hidden -ArgumentList @(
    '-NoProfile', '-ExecutionPolicy', 'Bypass',
    '-File', (Join-Path $PSScriptRoot 'debug-output.ps1'),
    '-OutFile', $debugLog)
Say 'setup: the games'' own output is being read into game-output.log'

Add-Type -TypeDefinition @'
using System;
using System.Runtime.InteropServices;
public static class Win {
    [DllImport("user32.dll")] public static extern IntPtr GetForegroundWindow();
    [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr hWnd);
    [DllImport("user32.dll")] public static extern bool BringWindowToTop(IntPtr hWnd);
    [DllImport("user32.dll")] public static extern IntPtr SetActiveWindow(IntPtr hWnd);
    [DllImport("user32.dll")] public static extern uint GetWindowThreadProcessId(IntPtr hWnd, out uint pid);
    [DllImport("user32.dll")] public static extern bool AttachThreadInput(uint idAttach, uint idAttachTo, bool fAttach);
    [DllImport("user32.dll")] public static extern bool PostMessage(IntPtr hWnd, uint msg, IntPtr wParam, IntPtr lParam);
    [DllImport("user32.dll")] public static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);
    [DllImport("user32.dll")] public static extern void keybd_event(byte bVk, byte bScan, uint dwFlags, UIntPtr dwExtraInfo);
    [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr hWnd, out RECT rect);
    [DllImport("user32.dll")] public static extern bool MoveWindow(IntPtr hWnd, int x, int y, int width, int height, bool repaint);
    [DllImport("user32.dll")] public static extern int GetSystemMetrics(int index);
    [DllImport("kernel32.dll")] public static extern uint GetCurrentThreadId();
    public struct RECT { public int Left; public int Top; public int Right; public int Bottom; }
}
'@
Add-Type -AssemblyName System.Drawing

$VK_DOWN = 0x28
$VK_RETURN = 0x0D
$VK_MENU = 0x12
$KEYEVENTF_KEYUP = 2

function Focus([IntPtr] $hwnd) {
    $one = 0
    $two = 0
    $foreThread = [Win]::GetWindowThreadProcessId([Win]::GetForegroundWindow(), [ref] $one)
    $targetThread = [Win]::GetWindowThreadProcessId($hwnd, [ref] $two)
    $mine = [Win]::GetCurrentThreadId()
    [void] [Win]::ShowWindow($hwnd, 5)
    [void] [Win]::AttachThreadInput($mine, $foreThread, $true)
    [void] [Win]::AttachThreadInput($mine, $targetThread, $true)
    # A tap of Alt first: Windows lets the process that saw the last input event set
    # the foreground window outright, and this one has not seen any.
    [Win]::keybd_event($VK_MENU, 0, 0, [UIntPtr]::Zero)
    Start-Sleep -Milliseconds 40
    [Win]::keybd_event($VK_MENU, 0, $KEYEVENTF_KEYUP, [UIntPtr]::Zero)
    [void] [Win]::SetForegroundWindow($hwnd)
    [void] [Win]::BringWindowToTop($hwnd)
    [void] [Win]::SetActiveWindow($hwnd)
    [void] [Win]::AttachThreadInput($mine, $foreThread, $false)
    [void] [Win]::AttachThreadInput($mine, $targetThread, $false)
    Start-Sleep -Milliseconds 200
}

# The game throws away every recorded key on any frame where its window is not the
# foreground one (CGameEngineWin32::StartFrame clears m_SetKeysDown), so a batch of
# keys only lands if the window is brought forward immediately before it. The menu
# also takes a Return once per 220ms and a Down once per 140ms (BaseMenu::RunFrame),
# and it keeps its item across a rebuild (CBaseMenu::PopSelectedItem) - so the counts
# below are relative to where the last batch left the cursor.
function Key([IntPtr] $hwnd, [int] $vk) {
    [void] [Win]::PostMessage($hwnd, 0x0100, [IntPtr] $vk, [IntPtr] 0)
    Start-Sleep -Milliseconds 80
    [void] [Win]::PostMessage($hwnd, 0x0101, [IntPtr] $vk, [IntPtr] 0)
    Start-Sleep -Milliseconds 350
}

function Drive([IntPtr] $hwnd, [int[]] $keys, [string] $what) {
    for ($attempt = 0; $attempt -lt 3; $attempt++) {
        Focus $hwnd
        if ([Win]::GetForegroundWindow() -eq $hwnd) { break }
        Start-Sleep -Milliseconds 300
    }
    $foreground = ([Win]::GetForegroundWindow() -eq $hwnd)
    if ($keys.Count -eq 2) { $shape = 'a down and a return' } else { $shape = "$($keys.Count - 1) downs and a return" }
    Say ("  {0}: posting {1}" -f $what, $shape)
    if (-not $foreground) { Say '  (warning: the window never came to the front, the keys will be dropped)' }
    foreach ($key in $keys) { Key $hwnd $key }
    Start-Sleep -Milliseconds 400
}

# The two windows on one screen, half each. The rig's keys need the foreground and the
# recording needs both of them in one frame, and by the time this is called the last key
# has been sent - so nothing has to hold the focus any more.
function Tile([IntPtr[]] $windows) {
    $live = @($windows | Where-Object { $_ -ne [IntPtr]::Zero })
    $width = [Win]::GetSystemMetrics(0)
    $height = [Win]::GetSystemMetrics(1)
    if ($live.Count -eq 0 -or $width -le 0 -or $height -le 0) { return }
    $each = [int] ($width / $live.Count)
    for ($index = 0; $index -lt $live.Count; ++$index) {
        $left = $index * $each
        $size = if ($index -eq $live.Count - 1) { $width - $left } else { $each }
        # On top of whatever else is on the desktop: a window that keeps the foreground
        # and repaints over one of these would end up in the recording.
        [void] [Win]::ShowWindow($live[$index], 5)
        [void] [Win]::MoveWindow($live[$index], $left, 0, $size, $height, $true)
        [void] [Win]::BringWindowToTop($live[$index])
    }
    Start-Sleep -Milliseconds 400
}

function Shoot([IntPtr] $hwnd, [string] $path) {
    Focus $hwnd
    Start-Sleep -Milliseconds 600
    $rect = New-Object 'Win+RECT'
    [void] [Win]::GetWindowRect($hwnd, [ref] $rect)
    $width = $rect.Right - $rect.Left
    $height = $rect.Bottom - $rect.Top
    if ($width -le 0 -or $height -le 0) { return }
    $bitmap = New-Object System.Drawing.Bitmap($width, $height)
    $graphics = [System.Drawing.Graphics]::FromImage($bitmap)
    $graphics.CopyFromScreen($rect.Left, $rect.Top, 0, 0, $bitmap.Size)
    $bitmap.Save($path, [System.Drawing.Imaging.ImageFormat]::Png)
    $graphics.Dispose()
    $bitmap.Dispose()
    Say ("  captured {0}" -f $path)
}

# The backend holds the transcript open for append, so it has to be read with sharing.
function Read-Transcript {
    if (-not (Test-Path $transcript)) { return '' }
    try {
        $stream = [System.IO.File]::Open($transcript, [System.IO.FileMode]::Open, [System.IO.FileAccess]::Read, [System.IO.FileShare]::ReadWrite)
        $reader = New-Object System.IO.StreamReader($stream)
        $text = $reader.ReadToEnd()
        $reader.Close()
        $stream.Close()
        return $text
    }
    catch { return '' }
}

function Start-Game([string] $profile, [string[]] $extra, [string] $log) {
    $env:STEAMMOCK_PROFILE = $profile
    $env:STEAMMOCK_LOG = Join-Path $RigDir $log
    $env:STEAMMOCK_LOG_LEVEL = 'debug'
    if ($extra) {
        return Start-Process -FilePath (Join-Path $game 'SteamworksExample.exe') -ArgumentList $extra -WorkingDirectory $game -PassThru
    }
    return Start-Process -FilePath (Join-Path $game 'SteamworksExample.exe') -WorkingDirectory $game -PassThru
}

function Wait-Window($process) {
    $hwnd = [IntPtr]::Zero
    for ($i = 0; $i -lt 40 -and $hwnd -eq [IntPtr]::Zero; $i++) {
        Start-Sleep -Milliseconds 500
        $process.Refresh()
        if ($process.HasExited) { break }
        $hwnd = $process.MainWindowHandle
    }
    return $hwnd
}

# The live view takes the scenario and --start and nothing else, so a run with it on has no
# transcript for the summary below to read - it has a window, and that is the point of it.
$backend = if ($Gui) {
    Start-Process -FilePath $server -ArgumentList @('--scenario', $scenario, '--start') `
        -WorkingDirectory $RepoRoot -PassThru
}
else {
    Start-Process -FilePath $server `
        -ArgumentList @('--scenario', $scenario, '--transcript', $transcript, '--log-level', 'debug') `
        -WorkingDirectory $RepoRoot -PassThru -WindowStyle Hidden
}
Say "backend: pid $($backend.Id), listening on its default port"
$guiHwnd = [IntPtr]::Zero
if ($Gui) {
    $guiHwnd = Wait-Window $backend
    # It has to be drawn for the server to start: --start is honoured on the first frame the
    # window renders, and a minimized window never renders. So it is shown, put somewhere
    # out of the way and left drawing, which is what it does for the whole run until the
    # recording tiles it in between the two clients.
    [void] [Win]::ShowWindow($guiHwnd, 5)
    [void] [Win]::MoveWindow($guiHwnd, 0, 0, 640, 480, $true)
    [void] [Win]::BringWindowToTop($guiHwnd)

    # Serving is what the clients need, and it is worth waiting for rather than assuming:
    # a live view that never draws never listens, and then a game's first call goes
    # nowhere and its menu never moves - which is exactly what a first attempt at this
    # looked like from the outside.
    $listening = $false
    for ($attempt = 0; $attempt -lt 24 -and -not $listening; ++$attempt) {
        Start-Sleep -Milliseconds 250
        $listening = @(Get-NetTCPConnection -OwningProcess $backend.Id -State Listen -ErrorAction SilentlyContinue).Count -gt 0
    }
    if ($listening) { Say ("live view: window {0} for pid {1}, and listening" -f $guiHwnd, $backend.Id) }
    else { Say 'live view: the window never started listening - the games will not reach it' }
}

Say 'instance A: launching as the default profile'
Start-Sleep -Seconds 2
$a = Start-Game 'default' @() 'a.log'
$hwndA = Wait-Window $a
Say "instance A: pid $($a.Id), window $hwndA, exited $($a.HasExited)"
if ($hwndA -eq [IntPtr]::Zero) {
    Say 'instance A never showed a window - stopping'
    Stop-Process -Id $a.Id -Force -ErrorAction SilentlyContinue
    Stop-Process -Id $backend.Id -Force -ErrorAction SilentlyContinue
    exit 1
}

# Main menu: Start New Server, Find LAN Servers, Find Internet Servers, Create Lobby.
Drive $hwndA @($VK_DOWN, $VK_DOWN, $VK_DOWN, $VK_RETURN) 'instance A: Create Lobby'

Say 'waiting for the lobby the world mints'
$lobby = $null
for ($i = 0; $i -lt 40 -and -not $lobby; $i++) {
    Start-Sleep -Seconds 1
    $match = [regex]::Match((Read-Transcript), '"steamIDLobby":(\d{10,})')
    if ($match.Success) { $lobby = $match.Groups[1].Value }
}
if (-not $lobby) {
    Say 'no lobby was created - the menu drive did not land'
    Stop-Process -Id $a.Id -Force -ErrorAction SilentlyContinue
    Stop-Process -Id $backend.Id -Force -ErrorAction SilentlyContinue
    exit 1
}
Say "lobby: the world minted $lobby"

Say 'instance B: launching with +connect_lobby, which walks in with no keypresses'
$b = Start-Game 'second_player' @("+connect_lobby $lobby") 'b.log'
$joined = $false
for ($i = 0; $i -lt 40 -and -not $joined; $i++) {
    Start-Sleep -Seconds 1
    $joined = (Read-Transcript).Contains('JoinLobby')
}
Say "instance B: pid $($b.Id), joined=$joined"
# Both menus now list two members, which is what the key counts below assume.
Start-Sleep -Seconds 5

$hwndB = Wait-Window $b
Say "instance B: window $hwndB, exited $($b.HasExited)"

# The lobby menu, with two members listed: [member, member, ready toggle, ...]. The
# cursor starts at the top, so Ready is two downs in.
Drive $hwndA @($VK_DOWN, $VK_DOWN, $VK_RETURN) 'instance A: Set myself as Ready'
if ($hwndB -ne [IntPtr]::Zero) {
    Drive $hwndB @($VK_DOWN, $VK_DOWN, $VK_RETURN) 'instance B: Set myself as Ready'
}
Say 'instance A: Start game = down and return from the ready toggle, and the owner alone has it'
if ($Shoot) {
    Shoot $hwndA (Join-Path $RigDir 'lobby-a.png')
    Shoot $hwndB (Join-Path $RigDir 'lobby-b.png')
}
Drive $hwndA @($VK_DOWN, $VK_RETURN) 'instance A: Start game'

# Filming starts after the key that starts the match, so the video is the match and not
# the menu: the windows are put side by side first, and ffmpeg is given the same length as
# the wait, which is how it ends - a duration rather than a kill, because a killed
# recorder leaves a file nothing can play.
$recorder = $null
$video = Join-Path $RigDir 'two-instances.mp4'
if ($Record) {
    # ffmpeg may be on the PATH, or not until the shell that installed it is restarted -
    # so the package it came in is looked in as well.
    $ffmpegPath = $null
    $onPath = Get-Command ffmpeg -ErrorAction SilentlyContinue
    if ($null -ne $onPath) { $ffmpegPath = $onPath.Source }
    else {
        $found = Get-ChildItem (Join-Path $env:LOCALAPPDATA 'Microsoft\WinGet\Packages') -Recurse -Filter ffmpeg.exe -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($null -ne $found) { $ffmpegPath = $found.FullName }
    }
    if ($null -eq $ffmpegPath) {
        Say 'no ffmpeg on PATH or in the WinGet packages - recording nothing'
    }
    else {
        Tile @($hwndA, $guiHwnd, $hwndB)
        Remove-Item -Force $video -ErrorAction SilentlyContinue
        Say ("recording {0}s of the desktop to {1}" -f $WaitAfterStart, $video)
        $recorder = Start-Process -FilePath $ffmpegPath -PassThru -WindowStyle Hidden -ArgumentList @(
            '-hide_banner', '-loglevel', 'error', '-y',
            '-f', 'gdigrab', '-framerate', '30', '-i', 'desktop',
            '-c:v', 'libx264', '-preset', 'veryfast', '-pix_fmt', 'yuv420p',
            '-t', "$WaitAfterStart", $video)
    }
}

Start-Sleep -Seconds $WaitAfterStart

if ($null -ne $recorder) {
    $recorder.WaitForExit()
    if (Test-Path $video) { Say ("video: {0} ({1:N1} MB)" -f $video, (1.0 * (Get-Item $video).Length / 1MB)) }
    else { Say 'the recorder wrote no file' }
}

Say ''
Say '--- the two instances ---'
$a.Refresh()
$b.Refresh()
Say ("instance A pid {0} exited={1}; instance B pid {2} exited={3}" -f $a.Id, $a.HasExited, $b.Id, $b.HasExited)

Say 'stopping the instances and the backend, so the transcript can be read'
foreach ($process in @($a, $b)) {
    $process.Refresh()
    if (-not $process.HasExited) { Stop-Process -Id $process.Id -Force -ErrorAction SilentlyContinue }
}
$backend.Refresh()
if (-not $backend.HasExited) { Stop-Process -Id $backend.Id -Force -ErrorAction SilentlyContinue }
Start-Sleep -Seconds 1
# The reader last, so the games' final lines are in the file before it goes.
$debugReader.Refresh()
if (-not $debugReader.HasExited) { Stop-Process -Id $debugReader.Id -Force -ErrorAction SilentlyContinue }
Start-Sleep -Milliseconds 500

Say ''
Say '--- what the backend saw ---'
$sessionRe = [regex] '"session":"([0-9a-f]+)"'
$callRe = [regex] '"call":"([A-Za-z0-9_]+)"'
$retRe = [regex] '"ret":(\d+)\}'
$byID = [ordered] @{}
if (Test-Path $transcript) {
    $lines = [System.IO.File]::ReadAllLines($transcript)
    Say ("transcript: {0} calls" -f $lines.Count)
    foreach ($line in $lines) {
        $id = $sessionRe.Match($line)
        if (-not $id.Success) { continue }
        $key = $id.Groups[1].Value
        if (-not $byID.Contains($key)) {
            $byID[$key] = [pscustomobject] @{
                Calls = 0; Lobby = 0; Polls = 0; Members = '-'; MemberIndex = 0
                LobbyCalls = @{}; After = @{}; Left = $false
            }
        }
        $r = $byID[$key]
        $r.Calls++
        if ($line.Contains('"via":"lobby"')) { $r.Lobby++ }
        $call = $callRe.Match($line)
        if (-not $call.Success) { continue }
        $name = $call.Groups[1].Value
        if ($name -match 'Matchmaking') { $r.LobbyCalls[$name] = 1 + $r.LobbyCalls[$name] }
        if ($name -eq 'SteamAPI_ISteamNetworking_IsP2PPacketAvailable') { $r.Polls++ }
        if ($name -eq 'SteamAPI_ISteamMatchmaking_LeaveLobby') { $r.Left = $true }
        elseif ($r.Left -and $name -ne 'SteamAPI_ISteamNetworking_IsP2PPacketAvailable') {
            $r.After[$name] = 1 + $r.After[$name]
        }
        if ($name -eq 'SteamAPI_ISteamMatchmaking_GetNumLobbyMembers') {
            $ret = $retRe.Match($line.TrimEnd())
            if ($ret.Success) { $r.Members = $ret.Groups[1].Value }
        }
        if ($name -eq 'SteamAPI_ISteamMatchmaking_GetLobbyMemberByIndex') { $r.MemberIndex++ }
    }
    foreach ($key in $byID.Keys) {
        $r = $byID[$key]
        Say ''
        Say ("session {0}: {1} calls, {2} answered by the lobby" -f $key, $r.Calls, $r.Lobby)
        Say ("  how many it sees in the lobby: {0} (GetLobbyMemberByIndex x{1})" -f $r.Members, $r.MemberIndex)
        Say ("  IsP2PPacketAvailable polled {0} times" -f $r.Polls)
        foreach ($name in ($r.LobbyCalls.Keys | Sort-Object)) { Say ("    {0,-56} {1}" -f $name, $r.LobbyCalls[$name]) }
        if ($r.Left) {
            Say '  after it left the lobby (the start-game path):'
            if ($r.After.Count -eq 0) { Say '    nothing but the P2P poll' }
            else { foreach ($name in ($r.After.Keys | Sort-Object)) { Say ("    {0,-56} {1}" -f $name, $r.After[$name]) } }
        }
    }
}
else {
    Say 'no transcript was written'
}

Say ''
Say '--- what the stub handed to each game ---'
foreach ($log in 'a.log', 'b.log') {
    $path = Join-Path $RigDir $log
    if (-not (Test-Path $path)) { continue }
    Say ("{0}:" -f $log)
    $handed = Select-String -Path $path -Pattern 'hand over: (\w+)' -AllMatches |
        ForEach-Object { $_.Matches } | ForEach-Object { $_.Groups[1].Value }
    if ($handed.Count -eq 0) { Say '  nothing was handed over' }
    else { $handed | Group-Object | Sort-Object Name | ForEach-Object { Say ("  {0,-44} {1}" -f $_.Name, $_.Count) } }
}

# The games' own words, which is where a game says why it is unhappy. Every line is
# "<pid> <text>", and the two pids are the two instances this run started.
Say ''
Say '--- what the games themselves said ---'
$gameOutput = @()
if (Test-Path $debugLog) { $gameOutput = [System.IO.File]::ReadAllLines($debugLog) }
Say ("debug output: {0} line(s) in {1}" -f $gameOutput.Count, $debugLog)
foreach ($which in @(
        [pscustomobject] @{ Name = 'instance A'; Process = $a },
        [pscustomobject] @{ Name = 'instance B'; Process = $b })) {
    if ($null -eq $which.Process) { continue }
    $mine = $gameOutput | Where-Object { $_ -match "^$($which.Process.Id) " } |
        ForEach-Object { $_ -replace "^$($which.Process.Id) ", '' }
    Say ("{0} (pid {1}): {2} line(s)" -f $which.Name, $which.Process.Id, $mine.Count)
    if ($mine.Count -gt 0) { $mine | Select-Object -Last 10 | ForEach-Object { Say "  $_" } }
}

Say ''
Say ("rig: {0}" -f $RigDir)
Say 'done'
