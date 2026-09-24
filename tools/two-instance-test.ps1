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
#
# Everything below is Windows-only and needs a built tree: -RepoRoot\build\Release
# (the backend) and -RepoRoot\build-w32\Release (the 32-bit stub). See the walkthrough
# in docs/two-instance-test.md for what the run is meant to show and what it stops at.
param(
    [string] $RepoRoot = (Split-Path -Parent (Split-Path -Parent $PSCommandPath)),
    [string] $GamePath = 'D:\Games\Steam\steamapps\common\Spacewar',
    [string] $RigDir = (Join-Path $env:TEMP 'sw-two'),
    [int] $WaitAfterStart = 25,
    [switch] $Shoot
)

$ErrorActionPreference = 'Continue'

$game = Join-Path $RigDir 'game'
$server = Join-Path $RepoRoot 'build\Release\steammock.exe'
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

$backend = Start-Process -FilePath $server `
    -ArgumentList @('--scenario', $scenario, '--transcript', $transcript, '--log-level', 'debug') `
    -WorkingDirectory $RepoRoot -PassThru -WindowStyle Hidden
Say "backend: pid $($backend.Id), listening on its default port"

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
Start-Sleep -Seconds $WaitAfterStart

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

Say ''
Say ("rig: {0}" -f $RigDir)
Say 'done'
