# The Spacewar run, scripted: two instances by default, up to the four the game has slots
# for with -Clients.
#
# Two copies of Valve's own test app, one session, one lobby, then the owner
# starting the game: the thing the harness exists for, driven end to end on a real
# 32-bit game and reported from the backend's own transcript. It leaves the rig (a
# copy of the game carrying the 32-bit stub) in -RigDir and the evidence beside it:
# transcript.jsonl and one log per instance (a.log, b.log, c.log, d.log).
#
#   pwsh -File tools/two-instance-test.ps1
#   pwsh -File tools/two-instance-test.ps1 -Shoot      # also capture each window
#   pwsh -File tools/two-instance-test.ps1 -Record -WaitAfterStart 90
#                                                       # tile the two windows and film them for
#                                                       # that many seconds (needs ffmpeg)
#   pwsh -File tools\two-instance-test.ps1 -Gui -Record # the live view is the server, so this
#                                                       # films it between the two clients
#   pwsh -File tools\two-instance-test.ps1 -Clients 3 -Gui -Record -WaitAfterStart 90
#                                                       # three clients and the live view in a
#                                                       # grid, filmed from the first menu on
#
#   pwsh -File tools\two-instance-test.ps1 -Clients 4 -Gui -WaitAfterStart 100
#                                                       # all four the game has slots for
#                                                       # (MAX_PLAYERS_PER_SERVER), as the
#                                                       # stress form of the run of record
#
#   pwsh -File tools\two-instance-test.ps1 -Clients 11 -Overfill -Gui -Shoot -WaitAfterStart 60
#                                                       # a crowd: eleven clients in one lobby,
#                                                       # four of which the game seats and the
#                                                       # rest are members only. -Shoot captures
#                                                       # the tiled screen mid-match, the live
#                                                       # view, and one seated and one unseated
#                                                       # client
#
#   pwsh -File tools\two-instance-test.ps1 -Clients 3 -StopWhenDecided -WaitAfterStart 12
#                                                       # the shortest run that still answers
#                                                       # "did every client get in": about a
#                                                       # minute, no recording, no live view
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
    [switch] $Gui,
    [switch] $StopWhenDecided,
    [int] $Clients = 2,
    # More clients than the game has seats for. The game holds MAX_PLAYERS_PER_SERVER (4) in a
    # match whatever this says; the rest are lobby members and nothing else, so a run this way
    # is a test of the harness holding a crowd - sessions, rosters, notifications, the live
    # view - and not of a match. Without it, more than four is refused.
    [switch] $Overfill,
    # Every guest after the first arrives by invitation instead of by `+connect_lobby`: the guest
    # launches with no arguments at all, the host is driven to the lobby menu's Invite Friend item,
    # and the guest's own game joins because Steam told it the room. That is the whole flow the
    # `+connect_lobby` route skips, where the game parses a command line and Steam says nothing.
    # The command line stays the default, because every set of runs before this one was taken with
    # it, and the two are meant to be compared.
    [switch] $ByInvite
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

# Everything under -RigDir is the rig's to delete and recreate, which makes a
# mistyped one destructive: `-RigDir D:\` would put the drive's own `game`,
# `a.log` and `transcript.jsonl` in the way of that. A drive root is refused, and
# the game copy is deleted only when it is one this rig made - which is what it
# has the game's executable in it, having copied it there.
if ([System.IO.Path]::GetPathRoot($RigDir) -eq $RigDir) {
    Say "-RigDir '$RigDir' is a drive root - name a directory the rig may own"
    exit 2
}
if ((Test-Path $game) -and -not (Test-Path (Join-Path $game 'SteamworksExample.exe'))) {
    Say "refusing to delete '$game': it is not a copy of the game this rig made"
    exit 2
}

foreach ($needed in @($server, $stub, $scenario, (Join-Path $GamePath 'SteamworksExample.exe'))) {
    if (-not (Test-Path $needed)) {
        Say "missing $needed - build the tree, or point -GamePath at the game"
        exit 2
    }
}

# Four is the game's own ceiling - SpaceWar.h's MAX_PLAYERS_PER_SERVER is 4, and a fifth
# client would join the lobby (the world imposes no limit), be listed to everyone, and never
# be given a slot. That is a lobby test wearing a client test's clothes, so it is refused
# rather than run and misreported - unless -Overfill asks for exactly that, in which case the
# run says how many of its clients the game will seat and the rest are the point.
$seats = 4
if ($Clients -gt $seats -and -not $Overfill) {
    Say ("-Clients {0} is more than the game has slots for (MAX_PLAYERS_PER_SERVER = {1}) - " -f $Clients, $seats)
    Say '  use -Overfill for a crowd run, where the extra clients are lobby members only'
    exit 2
}
if ($Clients -gt 16) {
    Say ("-Clients {0}: sixteen games is already more than a desktop has room for" -f $Clients)
    exit 2
}
if ($Clients -lt 2) {
    Say ("-Clients {0}: this is a session with a lobby in it, which takes two" -f $Clients)
    exit 2
}

# One profile per client, in the order the clients are launched: the host is always the first
# and always 'default' - it is the one that drives the menu and starts the game - and every
# guest gets its own. They are in scenarios/spacewar.json, and a name the scenario does not
# have fails the run loudly at the handshake rather than serving a client somebody else's
# identity.
$profiles = @(
    'default', 'second_player', 'third_player', 'fourth_player', 'fifth_player', 'sixth_player',
    'seventh_player', 'eighth_player', 'ninth_player', 'tenth_player', 'eleventh_player',
    'twelfth_player', 'thirteenth_player', 'fourteenth_player', 'fifteenth_player',
    'sixteenth_player'
)

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

# The one way out of this run, whichever way it goes. The reader has to be in it: it drains
# the machine-wide debug buffer, so one left behind keeps writing whatever games are running
# next into this run's game-output.log - lines that read exactly like this run's own, and
# that would be very convincing evidence an hour later. Games and backend first, so the last
# lines they have to say are in the file before the reader goes.
function Stop-Rig {
    param([object[]] $Things)
    foreach ($thing in $Things) {
        if ($null -eq $thing) { continue }
        $thing.Refresh()
        if (-not $thing.HasExited) { Stop-Process -Id $thing.Id -Force -ErrorAction SilentlyContinue }
    }
    $debugReader.Refresh()
    if (-not $debugReader.HasExited) { Stop-Process -Id $debugReader.Id -Force -ErrorAction SilentlyContinue }
    Start-Sleep -Milliseconds 500
}

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
    [DllImport("user32.dll")] public static extern bool SetProcessDPIAware();
    [DllImport("kernel32.dll")] public static extern uint GetCurrentThreadId();
    public struct RECT { public int Left; public int Top; public int Right; public int Bottom; }
}
'@
Add-Type -AssemblyName System.Drawing

# Physical pixels, asked for once and before any window is touched. A PowerShell process is
# DPI-unaware by default, which makes every coordinate here virtual - GetSystemMetrics answers
# the *scaled* screen size while CopyFromScreen copies *physical* pixels - so on a display at
# 150% a capture of the "whole screen" is the top-left of it, and GetWindowRect for a tiled
# window points at a region another window is in. That is exactly what a crowd run's grid and
# its per-window shots came out as: a screenshot of somebody else's cell, cropped.
[void] [Win]::SetProcessDPIAware()

$VK_DOWN = 0x28
$VK_UP = 0x26
$VK_RETURN = 0x0D
$VK_ESCAPE = 0x1B
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

# One key on a window that is brought forward first.
#
# Key() on its own posts to whatever happens to have the front, and a window that does not
# have it drops what it is sent (see the note above it) - which is fine inside Drive, which
# focuses once for the whole batch, and wrong for a key sent on its own a long time after the
# last drive. The reader's Escape was the first case of that, and it cost a run to see.
function Press([IntPtr] $hwnd, [int] $vk) {
    Focus $hwnd
    Key $hwnd $vk
}

# Bring a window to the front, retrying: Windows refuses a background process's first attempt at
# taking the front often enough that one call is not enough, and a game that does not have the front
# stops asking Steam for what has arrived - so a payload queued for it waits until something focuses
# it again. Drive did this inline for the keys it posts; the invited guest needs it for the payload
# alone, which is why it is a function.
function Bring-ToFront([IntPtr] $hwnd) {
    for ($attempt = 0; $attempt -lt 3; $attempt++) {
        Focus $hwnd
        if ([Win]::GetForegroundWindow() -eq $hwnd) { return $true }
        Start-Sleep -Milliseconds 300
    }
    return ([Win]::GetForegroundWindow() -eq $hwnd)
}

function Drive([IntPtr] $hwnd, [int[]] $keys, [string] $what) {
    Bring-ToFront $hwnd | Out-Null
    $foreground = ([Win]::GetForegroundWindow() -eq $hwnd)
    if ($keys.Count -eq 2) { $shape = 'a down and a return' } else { $shape = "$($keys.Count - 1) downs and a return" }
    Say ("  {0}: posting {1}" -f $what, $shape)
    if (-not $foreground) { Say '  (warning: the window never came to the front, the keys will be dropped)' }
    # Re-asserted before every key rather than once for the batch: the game decides per
    # frame whether to keep a key (see Key), so a window that loses the front halfway
    # through keeps the keys before it and drops the ones after - which is how a drive
    # ends up one item off rather than not landing at all. The count of keys that went
    # out without the front is the evidence a run needs when it goes wrong.
    $lost = 0
    foreach ($key in $keys) {
        if ([Win]::GetForegroundWindow() -ne $hwnd) {
            [void] [Win]::SetForegroundWindow($hwnd)
            [void] [Win]::BringWindowToTop($hwnd)
            if ([Win]::GetForegroundWindow() -ne $hwnd) { $lost++ }
        }
        Key $hwnd $key
    }
    if ($lost -gt 0) { Say ("  warning: {0} of {1} keys went out without the front - the menu may be on the wrong item" -f $lost, $keys.Count) }
    Start-Sleep -Milliseconds 400
}

# What the game did with a batch, read out of the stub's own log. A failed drive has
# two shapes and they mean different things: the game started a server instead of a
# lobby (the batch landed on Start New Server - it registers the game server's own
# callbacks, GSPolicyResponse_t and ValidateAuthTicketResponse_t), or it never moved at
# all (nothing was taken out of its queue of payloads). Without this a run only says
# 'the drive did not land', which is a guess about which of the two happened. It is
# only read when no lobby appeared, which is what makes the first shape conclusive: a
# game that registered the game server's callbacks without a lobby was driven onto
# Start New Server, since nothing else offers it.
function Drive-Diagnosis([string] $logPath) {
    if (-not (Test-Path $logPath)) { return 'there is no stub log to read' }
    $lines = @(Get-Content $logPath -ErrorAction SilentlyContinue)
    $took = @($lines | Select-String -Pattern 'taking a payload out of the queue').Count
    $ranServer = @($lines | Select-String -Pattern 'callback id 115: first|callback id 143: first').Count -gt 0
    if ($ranServer) {
        return ("the game started a server, so the batch landed on Start New Server - {0} payload(s) were taken" -f $took)
    }
    if ($took -eq 0) {
        return 'the game took nothing out of its queue, so the whole batch was dropped'
    }
    return ("the game took {0} payload(s) but never made a lobby" -f $took)
}

# The two windows on one screen, half each. The rig's keys need the foreground and the
# recording needs both of them in one frame, and by the time this is called the last key
# has been sent - so nothing has to hold the focus any more.
function Tile([IntPtr[]] $windows) {
    $live = @($windows | Where-Object { $_ -and $_ -ne [IntPtr]::Zero })
    $width = [Win]::GetSystemMetrics(0)
    $height = [Win]::GetSystemMetrics(1)
    if ($live.Count -eq 0 -or $width -le 0 -or $height -le 0) { return }
    # Three still reads better as a row, two as a pair. Past that the grid is squared off, so
    # a crowd run's twelve windows come out four across and three down instead of six thin
    # rows - which is the difference between a screenshot that reads and one that does not.
    $columns = if ($live.Count -le 3) { $live.Count }
               else { [int] [Math]::Ceiling([Math]::Sqrt($live.Count)) }
    $rows = [int] [Math]::Ceiling($live.Count / $columns)
    for ($index = 0; $index -lt $live.Count; ++$index) {
        $column = $index % $columns
        $row = [int] [Math]::Floor($index / $columns)
        $left = [int] ($column * $width / $columns)
        $size = if ($column -eq $columns - 1) { $width - $left } else { [int] ($width / $columns) }
        $top = [int] ($row * $height / $rows)
        $tall = if ($row -eq $rows - 1) { $height - $top } else { [int] ($height / $rows) }
        # On top of whatever else is on the desktop: a window that keeps the foreground
        # and repaints over one of these would end up in the recording.
        [void] [Win]::ShowWindow($live[$index], 5)
        [void] [Win]::MoveWindow($live[$index], $left, $top, $size, $tall, $true)
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

# The whole screen rather than one window: with the grid tiled, this is the one image that
# shows every instance at once, which is the thing a crowd run is worth looking at for. Taken
# before any Shoot() call, because those focus a window and the grid is what is wanted here.
function Shoot-Desktop([string] $path) {
    $width = [Win]::GetSystemMetrics(0)
    $height = [Win]::GetSystemMetrics(1)
    if ($width -le 0 -or $height -le 0) { return }
    $bitmap = New-Object System.Drawing.Bitmap($width, $height)
    $graphics = [System.Drawing.Graphics]::FromImage($bitmap)
    $graphics.CopyFromScreen(0, 0, 0, 0, $bitmap.Size)
    $bitmap.Save($path, [System.Drawing.Imaging.ImageFormat]::Png)
    $graphics.Dispose()
    $bitmap.Dispose()
    Say ("  captured the whole screen: {0} ({1}x{2})" -f $path, $width, $height)
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

# The live view is the same server with a window on it, and it takes the scenario, the
# transcript to keep and --start: the recording and the summary both want the run written
# down, and a window that kept nothing would leave this script with nothing to read.
$backend = if ($Gui) {
    Start-Process -FilePath $server `
        -ArgumentList @('--scenario', $scenario, '--start', '--transcript', $transcript) `
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

# The clients, known one at a time - the host first, then a guest per client - and the live
# view, if there is one. Tile takes whatever is known, so the grid fills in as the run goes,
# and everything after the lobby is driven off this list rather than off named variables, so
# a crowd run is the same code as a pair.
$instances = @()
$recorder = $null
$video = Join-Path $RigDir 'two-instances.mp4'

Say 'instance A: launching as the default profile'
Start-Sleep -Seconds 2
$a = Start-Game 'default' @() 'a.log'
$hwndA = Wait-Window $a
Say "instance A: pid $($a.Id), window $hwndA, exited $($a.HasExited)"
if ($hwndA -eq [IntPtr]::Zero) {
    Say 'instance A never showed a window - stopping'
    Stop-Rig @($a, $backend)
    exit 1
}

# Recording starts here, with the first window up and before a single key is pressed: the
# menus are half of what this is for, and a video that begins at the match has thrown that
# half away. -t is a ceiling rather than the length - the stop at the end is what closes the
# file properly, which on Windows is the only way to get one that plays.
if ($Record) {
    # ffmpeg may be on the PATH, or not until the shell that installed it is restarted - so
    # the package it came in is looked in as well.
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
        Tile @($hwndA, $guiHwnd)
        Remove-Item -Force $video -ErrorAction SilentlyContinue
        Say ("recording the whole run to {0}" -f $video)
        $recorder = New-Object System.Diagnostics.Process
        $recorder.StartInfo.FileName = $ffmpegPath
        $recorder.StartInfo.Arguments = @(
            '-hide_banner', '-loglevel', 'error', '-y',
            '-f', 'gdigrab', '-framerate', '30', '-i', 'desktop',
            '-c:v', 'libx264', '-preset', 'veryfast', '-pix_fmt', 'yuv420p',
            '-t', '240', $video) -join ' '
        $recorder.StartInfo.UseShellExecute = $false
        $recorder.StartInfo.RedirectStandardInput = $true
        $recorder.StartInfo.CreateNoWindow = $true
        [void] $recorder.Start()
    }
}

# Main menu: Start New Server, Find LAN Servers, Find Internet Servers, Create Lobby.
# The keys only land if the game is pumping frames when they arrive - it throws a batch
# away on any frame where its window is not the front one (see Key) - and a client that
# is still starting up is the one shape where every key is dropped. Its frame loop is in
# the transcript as RunCallbacks, so the drive waits for that instead of assuming the
# window appearing means the game is ready for keys.
$pumping = $false
for ($i = 0; $i -lt 40 -and -not $pumping; $i++) {
    Start-Sleep -Milliseconds 250
    $pumping = (Read-Transcript).Contains('"call":"SteamAPI_RunCallbacks"')
}
if ($pumping) { Say 'instance A: its frame loop is running - driving the menu' }
else { Say 'instance A: no RunCallbacks in ten seconds - the game never started pumping' }
Start-Sleep -Milliseconds 700

# Three tries. A batch that lands nowhere or one item off costs a ten minute run, and a
# relaunch costs eight seconds, so a failed try is retried rather than written off -
# and what it did is read out of its own stub log first, which is why each try keeps
# its log (a.log for the first, a-try2.log and so on after it).
$attempts = 3
$lobby = $null
$why = 'nothing was tried'
for ($attempt = 1; $attempt -le $attempts -and -not $lobby; $attempt++) {
    if ($attempt -gt 1) {
        Say ("instance A: trying again for try {0} - {1}" -f $attempt, $why)
        Stop-Process -Id $a.Id -Force -ErrorAction SilentlyContinue
        Start-Sleep -Seconds 2
        $a = Start-Game 'default' @() ("a-try{0}.log" -f $attempt)
        $hwndA = Wait-Window $a
        if ($hwndA -eq [IntPtr]::Zero) {
            Say 'instance A never showed a window on this try - stopping'
            Stop-Rig @($a, $backend)
            exit 1
        }
        Say ("instance A: pid {0}, window {1}, exited {2}" -f $a.Id, $hwndA, $a.HasExited)
        if ($Record) { Tile @($hwndA, $guiHwnd) }
        Start-Sleep -Seconds 3
    }

    # The stats screen, which is where an inventory is drawn - and the one drive in this run
    # that *is* the read-out rather than a way to get somewhere. Spacewar lists one line per
    # item it holds, by the name the catalogue gave it: the list arrives as an array of
    # sixteen-byte items written into the game's own memory (`GetResultItems`, asked for with a
    # null array first to be told how many there are) and each name is read out of the catalogue
    # into a buffer the game owns (`GetItemDefinitionProperty`). Both of those are the calls
    # this run exists to exercise, so the screenshot and the two counts below are what say
    # whether they landed.
    #
    # Stats and Achievements is the seventh of the seventeen - Leaderboards is the eighth, which
    # is what the drive below counts from - so this is six downs and a return. Escape is the way
    # out (`k_EClientStatsAchievements` -> `k_EClientGameMenu` in the client's own state
    # machine), and six ups put the cursor back at the top where the leaderboard drive wants it.
    Drive $hwndA @($VK_DOWN, $VK_DOWN, $VK_DOWN, $VK_DOWN, $VK_DOWN, $VK_DOWN, $VK_RETURN) 'instance A: Stats and Achievements'
    Start-Sleep -Seconds 4
    if ($Shoot) { Shoot $hwndA (Join-Path $RigDir 'stats-inventory.png') }
    $itemCalls = [regex]::Matches((Read-Transcript), 'SteamAPI_ISteamInventory_(LoadItemDefinitions|GrantPromoItems|GetAllItems|GetResultItems|GetItemDefinitionProperty)').Count
    Say ("  the stats screen was reached: {0} inventory call(s) so far" -f $itemCalls)
    Press $hwndA $VK_ESCAPE
    Start-Sleep -Seconds 2
    Say '  instance A: six ups, back to the top of the menu'
    foreach ($unused in 1..6) { Press $hwndA $VK_UP }

    # The leaderboard menu, before the lobby, because this is the only moment in a run when
    # it is reachable: a client that has joined a game cannot walk back to the main menu.
    #
    # Spacewar's main menu is a list of seventeen and Leaderboards is the eighth, so this is
    # seven downs and a return. What comes up is a board with nothing on it - nobody has
    # finished a round yet, so nobody has posted - and the way out is countable because of
    # that: a header ("Leaderboard: Quickest Win, Top 10"), the line a board with no rows
    # draws, "Next leaderboard" and "Return to main menu". Four items, so three downs and a
    # return - and a fifth item would be a row, which is what the count of board calls below
    # says did or did not happen.
    #
    # It is also the pass that matters for the rest of the run, whatever it shows: every
    # client finds the two boards and keeps their handles, and a client that has them posts a
    # score when the round it was in ends - which Spacewar's own server ends as a draw the
    # moment a second player joins, so a run of four has posted scores before anybody plays a
    # shot.
    #
    # The way *out* is Escape rather than a count of downs, and that is not laziness: this
    # menu is a list whose item count changes with the board, while Escape is read by the
    # client's own state machine (`k_EClientLeaderboards` -> `k_EClientGameMenu`). A count
    # cannot be right for both an empty board and a full one.
    Drive $hwndA @($VK_DOWN, $VK_DOWN, $VK_DOWN, $VK_DOWN, $VK_DOWN, $VK_DOWN, $VK_DOWN, $VK_RETURN) 'instance A: Leaderboards'
    Start-Sleep -Seconds 4
    if ($Shoot) { Shoot $hwndA (Join-Path $RigDir 'leaderboard-empty.png') }
    $boardCalls = [regex]::Matches((Read-Transcript), 'SteamAPI_ISteamUserStats_(FindOrCreateLeaderboard|DownloadLeaderboardEntries|GetDownloadedLeaderboardEntry)').Count
    Say ("  the leaderboard menu was reached: {0} board call(s) so far" -f $boardCalls)
    Press $hwndA $VK_ESCAPE
    Start-Sleep -Seconds 2
    # Escape leaves the menu where it was, and the main menu keeps its own cursor: instance A
    # left it on Leaderboards, so the drive below has to start from the top. Seven ups rather
    # than four, because a menu that stops at the first item and one that wraps both end at the
    # top this way - the first absorbs the extra keys and the second comes around to it. A guest
    # never gets a key at the main menu (it joins on the command line), which is why the reader
    # further down can count from index zero without any of this.
    Say '  instance A: seven ups, back to the top of the menu'
    foreach ($unused in 1..7) { Press $hwndA $VK_UP }

    Drive $hwndA @($VK_DOWN, $VK_DOWN, $VK_DOWN, $VK_RETURN) 'instance A: Create Lobby'
    Say 'waiting for the lobby the world mints'
    for ($i = 0; $i -lt 12 -and -not $lobby; $i++) {
        Start-Sleep -Seconds 1
        $match = [regex]::Match((Read-Transcript), '"steamIDLobby":(\d{10,})')
        if ($match.Success) { $lobby = $match.Groups[1].Value }
    }
    if (-not $lobby) {
        $log = if ($attempt -eq 1) { 'a.log' } else { "a-try{0}.log" -f $attempt }
        $why = Drive-Diagnosis (Join-Path $RigDir $log)
        Say ("instance A: no lobby after try {0} - {1}" -f $attempt, $why)
    }
}
if (-not $lobby) {
    Say ("no lobby was created in {0} tries - {1}" -f $attempts, $why)
    Stop-Rig @($a, $backend)
    exit 1
}
Say "lobby: the world minted $lobby"

# The host first, so the table below is the whole run in order - and it is the list every
# later step walks: the grid, the ready drive, the report, the stop.
$instances += [pscustomobject] @{
    Number = 1; Name = 'instance A'; Letter = 'a'; Profile = 'default'
    Process = $a; Window = $hwndA
}

# One guest per client after the first: each joins the same lobby by id - a number on the
# command line, so it needs no keypresses - and a lobby menu lists one row per member, which
# is what the key counts below are relative to. The profile is per client, because a client
# with no profile of its own silently becomes the default identity - which is how three
# clients once ran as two players.
for ($guest = 2; $guest -le $Clients; $guest++) {
    $letter = [char] (96 + $guest)
    $name = 'instance ' + [char] (64 + $guest)
    $profile = $profiles[$guest - 1]
    if ($ByInvite) {
        # No arguments at all, because this game is going to be invited. Which is also why the
        # invite cannot be opened before it is running: a friend with no live session is told
        # nothing, on purpose, so the guest has to be up and talking first.
        Say ("{0}: launching as '{1}' with no arguments, to be invited into the lobby" -f $name, $profile)
        $process = Start-Game $profile @() ("{0}.log" -f $letter)
        $guest_window = Wait-Window $process
        Start-Sleep -Seconds 8
        # One row per member, then the ready toggle, then Start game, then Invite Friend, so the
        # item is as many downs in as the room has members plus two - and the room holds the host
        # and every guest before this one. The walk to the top first is the rig's own habit: the
        # menu keeps the item the last batch left the cursor on.
        foreach ($unused in 1..7) { Press $hwndA $VK_UP }
        Drive $hwndA (@($VK_DOWN) * ($guest + 1) + $VK_RETURN) ("{0}: the host invites it" -f $name)
        # The invited game only picks the join request up while it is the window in front: asking
        # Steam for what has arrived is what its frame loop does, and a game that has lost the
        # front stops asking. The drive above took the front for the host, so the guest gets it
        # back here - without this the invitation lands when the rig next drives the guest, a
        # minute later, which is longer than the wait below.
        Start-Sleep -Seconds 2
        if (-not (Bring-ToFront $guest_window)) {
            Say '  warning: the invited game never came to the front, so it may not pick the invite up'
        }
        # And it needs a key, not just the front: a guest that is merely in front sits on the
        # invitation, and the guest that is sent one joins within a second of it. An up on a list
        # it is standing at the top of is the key that changes nothing it is showing.
        Press $guest_window $VK_UP
    } else {
        Say ("{0}: launching as '{1}' with +connect_lobby, which walks in with no keypresses" -f $name, $profile)
        $process = Start-Game $profile @("+connect_lobby $lobby") ("{0}.log" -f $letter)
    }
    $joined = $false
    for ($i = 0; $i -lt 60 -and -not $joined; $i++) {
        Start-Sleep -Seconds 1
        $joins = [regex]::Matches((Read-Transcript), 'SteamAPI_ISteamMatchmaking_JoinLobby').Count
        $joined = $joins -ge ($guest - 1)
    }
    Say ("{0}: pid {1}, joined={2}" -f $name, $process.Id, $joined)
    Start-Sleep -Seconds 5
    $window = Wait-Window $process
    Say ("{0}: window {1}, exited {2}" -f $name, $window, $process.HasExited)
    $instances += [pscustomobject] @{
        Number = $guest; Name = $name; Letter = $letter; Profile = $profile
        Process = $process; Window = $window
    }
    if ($guest -eq $seats + 1) {
        Say ("  the game seats {0}: from here on the {1} client(s) after it are lobby members and nothing more" -f `
            $seats, ($Clients - $seats))
    }
}
Start-Sleep -Seconds 2

# Everyone is in, so the grid is what the rest of the run looks like.
$tiles = @($instances | ForEach-Object { $_.Window })
$tiles += $guiHwnd
Tile $tiles

# The lobby menu lists one row per member and then the ready toggle, so Ready is as many
# downs in as there are clients - which is why this counts instead of being written twice.
# The selection moves by index whether or not a crowd's rows fit on screen, so this holds
# past the point where the menu stops drawing all of them.
$ready = @($VK_DOWN) * $Clients + $VK_RETURN
foreach ($which in $instances) {
    if ($which.Window -ne [IntPtr]::Zero) {
        Drive $which.Window $ready ("{0}: Set myself as Ready" -f $which.Name)
    }
}
Say 'instance A: Start game = down and return from the ready toggle, and the owner alone has it'
if ($Shoot) {
    Shoot $hwndA (Join-Path $RigDir 'lobby-a.png')
    $second = @($instances | Where-Object { $_.Number -eq 2 })
    if ($second.Count -gt 0 -and $second[0].Window -ne [IntPtr]::Zero) {
        Shoot $second[0].Window (Join-Path $RigDir 'lobby-b.png')
    }
}
Drive $hwndA @($VK_DOWN, $VK_RETURN) 'instance A: Start game'

# The shot the crowd run is for: everyone attached and the match up, so the live view's own
# table has a row per session and the grid has a window per client. The desktop capture comes
# first because Shoot brings one window to the front, and the whole tiled screen is the point.
if ($Shoot) {
    Start-Sleep -Seconds 12
    Shoot-Desktop (Join-Path $RigDir 'mid-game-screen.png')
    if ($Gui) {
        # The live view is the one window a crowd run needs legibly: at its tiled size its
        # games table shows five rows of eleven. It is grown for this shot and left grown -
        # the grid has already been photographed, and nothing after this needs it small.
        [void] [Win]::MoveWindow($guiHwnd, 0, 0, 1180, 1040, $true)
        [void] [Win]::BringWindowToTop($guiHwnd)
        Start-Sleep -Milliseconds 900
        Shoot $guiHwnd (Join-Path $RigDir 'mid-game-live-view.png')
    }
    Shoot $hwndA (Join-Path $RigDir 'mid-game-a.png')
    if ($Overfill) {
        $extra = @($instances | Where-Object { $_.Number -eq ($seats + 1) })
        if ($extra.Count -gt 0 -and $extra[0].Window -ne [IntPtr]::Zero) {
            Shoot $extra[0].Window (Join-Path $RigDir 'mid-game-first-unseated.png')
        }
    }
}

# How long the match then runs for. With -StopWhenDecided it runs until the question this
# is about is answered - every client has been passed authentication - or until
# -WaitAfterStart has elapsed, whichever comes first, which is a second or two for a match
# that holds and the ceiling for one that does not. The answer is in the transcript the
# moment the game server has been told about each player, and that is the first second
# after this keypress; waiting 25 seconds for it is 25 seconds that a run of ten pays
# twenty times over, and it is also 25 seconds of the game playing that cannot change the
# answer. What it does change is what the run *shows*: with the early stop the loser's own
# 30-second ticket timeout is not in the run, so the symptom is missing and the outcome is
# not.
#
# The host's k_EMsgServerPassAuthentication is message 3, little endian, at the front of
# every packet it sends - the one call a game server makes per player it lets in.
if ($StopWhenDecided) {
    $deadline = (Get-Date).AddSeconds($WaitAfterStart)
    $passed = 0
    while ($true) {
        Start-Sleep -Milliseconds 250
        $passed = [regex]::Matches((Read-Transcript), '"pubData":"03000000').Count
        if ($passed -ge $Clients) { break }
        if ((Get-Date) -ge $deadline) { break }
    }
    if ($passed -ge $Clients) {
        Say ("instance A: all {0} client(s) were passed authentication - stopping here" -f $passed)
    }
    else {
        Say ("instance A: only {0} of {1} client(s) were passed within {2}s - stopping here" -f $passed, $Clients, $WaitAfterStart)
    }
}
else {
    Start-Sleep -Seconds $WaitAfterStart
}

# The board as a player sees it, which is the whole point of one.
#
# The reader is a guest that was in the round - not a new window - and that matters twice over.
# A client has to have *played* to be on the board: Spacewar's server ends a round as a draw the
# moment a second player joins, and the game posts what it earned to the "Feet Traveled" board
# when a round ends, so the client that was there when the draw happened has a score. And a
# guest's main menu cursor is still at the top, because joining on the command line takes no
# keys at all, which is what makes the seven downs below count from a known place.
#
# The way out of the match is the way a player leaves one: Escape opens the quit menu ("Resume
# Game", "Exit To Menu", "Exit To Desktop"), so one down and a return is "Exit To Menu" and the
# client keeps its score on the board.
#
# What it then shows is the two boards Spacewar keeps: the quickest win first, which is empty
# because a draw is nobody's win and so nobody uploaded to it, and then - through "Next
# leaderboard" - the feet travelled, which it reads *around itself*. That last part is why the
# reader had to be somebody who played: a real Steam returns nothing for a user with no entry
# on the board (Spacewar's own menu says so), and this world does the same.
$reader = @($instances | Where-Object { $_.Number -eq 2 })[0]
if ($null -eq $reader) { $reader = $instances[-1] }
if ($null -ne $reader -and $reader.Window -ne [IntPtr]::Zero) {
    Say ("the reader: instance {0}, leaving the match the way a player does" -f $reader.Name)
    Press $reader.Window $VK_ESCAPE
    Start-Sleep -Seconds 2
    Drive $reader.Window @($VK_DOWN, $VK_RETURN) 'the reader: Exit To Menu'
    Start-Sleep -Seconds 3
    Drive $reader.Window @($VK_DOWN, $VK_DOWN, $VK_DOWN, $VK_DOWN, $VK_DOWN, $VK_DOWN, $VK_DOWN, $VK_RETURN) 'the reader: Leaderboards'
    Start-Sleep -Seconds 5
    if ($Shoot) { Shoot $reader.Window (Join-Path $RigDir 'leaderboard-quickest-win.png') }
    # "Next leaderboard" is the third of the four items a board with no rows draws: the header,
    # the line a board with no scores draws, then it.
    Drive $reader.Window @($VK_DOWN, $VK_DOWN, $VK_RETURN) 'the reader: the next leaderboard'
    Start-Sleep -Seconds 5
    if ($Shoot) { Shoot $reader.Window (Join-Path $RigDir 'leaderboard-rows.png') }
    $rows = [regex]::Matches((Read-Transcript), 'SteamAPI_ISteamUserStats_GetDownloadedLeaderboardEntry').Count
    Say ("  it asked for {0} row(s) out of the downloads it was handed" -f $rows)
}
else {
    Say 'no window to read the board from - the board was not read'
}

if ($null -ne $recorder) {
    # q rather than a kill: ffmpeg closes the file on its own terms, and an mp4 whose index
    # never got written is a file nothing can play. The recording has been running since the
    # first window appeared, so this is the end of the whole session, menus included.
    $recorder.StandardInput.WriteLine('q')
    $recorder.StandardInput.Flush()
    $recorder.WaitForExit(15000) | Out-Null
    if (-not $recorder.HasExited) { Stop-Process -Id $recorder.Id -Force -ErrorAction SilentlyContinue }
    if (Test-Path $video) { Say ("video: {0} ({1:N1} MB)" -f $video, (1.0 * (Get-Item $video).Length / 1MB)) }
    else { Say 'the recorder wrote no file' }
}

Say ''
Say '--- the instances ---'
foreach ($which in $instances) {
    $which.Process.Refresh()
}
foreach ($which in $instances) {
    Say ("{0} pid {1} exited={2}" -f $which.Name, $which.Process.Id, $which.Process.HasExited)
}

Say 'stopping the instances and the backend, so the transcript can be read'
$things = @($instances | ForEach-Object { $_.Process })
$things += $backend
Stop-Rig $things
Start-Sleep -Seconds 1

Say ''
Say '--- what the backend saw ---'
$sessionRe = [regex] '"session":"([0-9a-f]+)"'
$callRe = [regex] '"call":"([A-Za-z0-9_]+)"'
$retRe = [regex] '"ret":(\d+)\}'
$byID = [ordered] @{}
if (Test-Path $transcript) {
    $lines = [System.IO.File]::ReadAllLines($transcript)
    Say ("transcript: {0} calls" -f $lines.Count)
    # What the boards did, which the per-session table below does not show: it counts the
    # lobby's own calls, and a leaderboard is not the lobby's. A run that reached the menu has
    # finds and downloads; a run whose rounds ended has uploads; a board with somebody on it
    # has rows read out.
    $board = [ordered] @{}
    foreach ($name in 'FindOrCreateLeaderboard', 'GetLeaderboardName', 'GetLeaderboardEntryCount',
        'DownloadLeaderboardEntries', 'GetDownloadedLeaderboardEntry', 'UploadLeaderboardScore') {
        $board[$name] = [regex]::Matches(($lines -join "`n"), "SteamAPI_ISteamUserStats_$name`"").Count
    }
    Say ("the boards: " + (($board.Keys | ForEach-Object { "{0}={1}" -f $_, $board[$_] }) -join ' '))
    # What the inventory did, for the same reason and in the same shape: an item name that
    # reached the game is a GetResultItems (the list) followed by a GetItemDefinitionProperty
    # (the name), and a run that only ever shows the first is a run whose names never arrived.
    # A player who holds nothing still has all of the first four, so the numbers that matter
    # are the last two.
    $item = [ordered] @{}
    foreach ($name in 'LoadItemDefinitions', 'GrantPromoItems', 'GetAllItems', 'GetResultItems',
        'GetItemDefinitionProperty', 'CheckResultSteamID', 'TriggerItemDrop', 'ExchangeItems') {
        $item[$name] = [regex]::Matches(($lines -join "`n"), "SteamAPI_ISteamInventory_$name`"").Count
    }
    Say ("the items: " + (($item.Keys | ForEach-Object { "{0}={1}" -f $_, $item[$_] }) -join ' '))
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
foreach ($which in $instances) {
    $path = Join-Path $RigDir ("{0}.log" -f $which.Letter)
    if (-not (Test-Path $path)) { continue }
    Say ("{0}.log ({1}, {2}):" -f $which.Letter, $which.Name, $which.Profile)
    $handed = Select-String -Path $path -Pattern 'hand over: (\w+)' -AllMatches |
        ForEach-Object { $_.Matches } | ForEach-Object { $_.Groups[1].Value }
    if ($handed.Count -eq 0) { Say '  nothing was handed over' }
    else { $handed | Group-Object | Sort-Object Name | ForEach-Object { Say ("  {0,-44} {1}" -f $_.Name, $_.Count) } }
}

# The games' own words, which is where a game says why it is unhappy. Every line is
# "<pid> <text>", and the pids are the instances this run started - one per -Clients.
Say ''
Say '--- what the games themselves said ---'
$gameOutput = @()
if (Test-Path $debugLog) { $gameOutput = [System.IO.File]::ReadAllLines($debugLog) }
Say ("debug output: {0} line(s) in {1}" -f $gameOutput.Count, $debugLog)
foreach ($which in $instances) {
    $mine = $gameOutput | Where-Object { $_ -match "^$($which.Process.Id) " } |
        ForEach-Object { $_ -replace "^$($which.Process.Id) ", '' }
    Say ("{0} ({1}, pid {2}): {3} line(s)" -f $which.Name, $which.Profile, $which.Process.Id, $mine.Count)
    if ($mine.Count -gt 0) { $mine | Select-Object -Last 10 | ForEach-Object { Say "  $_" } }
}

Say ''
Say ("rig: {0}" -f $RigDir)
Say 'done'
