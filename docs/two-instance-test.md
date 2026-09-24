# The two-instance test

Two copies of Valve's own test app, one session, one lobby, and then the owner
starting the game. Everything else in the suite runs against `fake_game`; this is
the run that says whether a **real 32-bit game**, twice over, agrees with the
harness - so it needs the game, a window it can be driven through, and a minute or
two, and it is therefore not a `ctest` case. The script is
[`tools/two-instance-test.ps1`](../tools/two-instance-test.ps1).

```bat
cmake --build build     --config Release      :: the backend
cmake --build build-w32 --config Release      :: the 32-bit stub
pwsh -File tools\two-instance-test.ps1
```

It copies the game out of the Steam library into `%TEMP%\sw-two\game`, puts the
32-bit stub in as `steam_api.dll`, starts the console backend with
`scenarios\spacewar.json`, and then drives:

1. **instance A** - the main menu's *Create Lobby* (three downs and a return), and
   the world mints the first lobby id, `109775240917155840`;
2. **instance B** - launched with `+connect_lobby <id>` (the game's own command
   line), which walks into the lobby with no keypresses at all;
3. both players *Set myself as Ready*, and the owner *Start game*.

It then stops everything - so the transcript can be read - and prints what the
backend saw, per session, plus what the stub handed each game.

## What a good run looks like

Measured on 24 Sep 2026, both instances on Steamworks SDK 1.46's Spacewar:

```
session 54b7d77cfc44: 114157 calls, 43 answered by the lobby
  how many it sees in the lobby: 2 (GetLobbyMemberByIndex x7)
  IsP2PPacketAvailable polled 84665 times
    CreateLobby 1, GetLobbyData 1, GetLobbyMemberByIndex 7, GetLobbyMemberData 9,
    GetLobbyOwner 3, GetNumLobbyMembers 4, LeaveLobby 1, SetLobbyData 2,
    SetLobbyGameServer 1, SetLobbyMemberData 1
  after it left the lobby (the start-game path):
    AcceptP2PSessionWithUser 1, ReadP2PPacket 1, SendP2PPacket 1

session 9c0aef48cc5b: 87676 calls, 49 answered by the lobby
  how many it sees in the lobby: 1 ... JoinLobby 1, SetLobbyMemberData 1

a.log: LobbyCreated_t 1, LobbyChatUpdate_t 1, LobbyDataUpdate_t 1, LobbyGameCreated_t 1, P2PSessionRequest_t 1
b.log: LobbyEnter_t 1, LobbyChatUpdate_t 2, LobbyDataUpdate_t 2, LobbyGameCreated_t 1, P2PSessionRequest_t 1
```

The one that says *1* member is B, read after A had already left the lobby to
connect - which is also why the last `GetNumLobbyMembers` in the run is a 1. The
calls the game-server half makes on the way (`SteamInternal_GameServer_Init`,
`SteamAPI_ISteamGameServer_LogOnAnonymous`, `SetLobbyGameServer`, and then
`SetServerName` / `SetMapName` / `SetMaxPlayerCount` on its own loop) all land
*before* `LeaveLobby`, so they are in the transcript rather than in the "after" list.

What that adds up to:

* **two sessions**, each with its own profile (A is `DebugPlayer` and owns the
  lobby, B is `DebugPlayer2`), and **both see two members** - the roster is real;
* every lobby call is answered by the world, labelled `via: "lobby"`;
* **payloads arrive at each game's own callback object**, on its own pump, with the
  ids the SDK declares (`LobbyCreated_t` 513, `LobbyChatUpdate_t` 506,
  `LobbyDataUpdate_t` 505, `LobbyGameCreated_t` 509, `LobbyEnter_t` 504) - the stub's
  log says which callback object each went to and that the game came back out of it;
* the owner's **Start game** brings up the game-server half of the same process
  (one `SteamInternal_GameServer_Init`, `LogOnAnonymous`, `SetLobbyGameServer`), the
  client leaves the lobby and sends the game's connect message, and the server side
  accepts the P2P session and reads it;
* **neither process exits**: the game survives every payload.

Nothing there needs a screenshot, but `-Shoot` captures each window, which is how
the lobby menu was read when the key counts were being worked out.

## Where it stops

The connect message the client sends to its own server is four bytes -
`f5 01 00 00` = 501 - and the game's server calls them *"unknown message on our
listen socket"*. No UDP socket is bound anywhere: this traffic is P2P, and
"listen socket" is the game's own name for its P2P receive. That is the one open
thing in the whole two-instance story, and it is a game-side parse, not a missing
answer from the harness - the bytes arrive, with the right length, on the right
channel, from the id the lobby published.

The cheapest next probe is the game's own `+connect` switch, whose argument is a
**number** rather than a dotted address (`"+connect %d:%d"` in the game's string
table): launch a second instance with `+connect 2130706433:27015` - 127.0.0.1 in
host order, which is what the harness publishes in
`SteamAPI_ISteamGameServer_GetPublicIP` - and see whether it gets further than the
lobby's own start-game path.

## Traps, so nobody works them out twice

* **Keys are dropped unless the window is in front.** `CGameEngineWin32::StartFrame`
  clears the whole key set on any frame where `GetForegroundWindow() != m_hWnd`.
  Posted `WM_KEYDOWN` still arrives at the window procedure; it is thrown away a
  frame later. Every batch of keys therefore needs an Alt-tap plus
  `AttachThreadInput` plus `SetForegroundWindow` immediately before it (the Alt tap
  is what lets a background process take the foreground at all), and the script
  checks `GetForegroundWindow` afterwards and says so if it failed.
* **The menu has a rate limit.** A return is taken at most once per 220 ms and a down
  once per 140 ms (`BaseMenu::RunFrame`), so keys closer together than that are
  swallowed. 350 ms apart is safe.
* **The menu keeps its item across a rebuild** (`CBaseMenu::PopSelectedItem`). Key
  counts are therefore relative to where the *previous* batch left the cursor, not
  from the top of the list - which is what made three attempts in a row open
  *Return to main menu* instead of *Start game*.
* **The transcript is open for append while the backend runs**, so it has to be read
  with `FileShare.ReadWrite` (or after the backend is stopped, which is what the
  script does).
* **The menus, for reference.** Main: Start New Server, Find LAN Servers, Find
  Internet Servers, **Create Lobby** (three downs), Find Lobby (four), ... With two
  members in a lobby: both members listed, then *Set myself as Ready* / *Not Ready*,
  then *Start game* (the owner only), *Invite Friend*, *Return to main menu*.
* **The second instance needs no keys at all**: `+connect_lobby <id>` is parsed by
  the game itself (`ParseCommandLine`), so B never touches a menu.
* **Launching the second instance steals the first one's keyboard.** That is the
  same trap as the first point, seen from the other side: whatever is driven next
  must be brought to the front first.

## What the run leaves behind

Under `%TEMP%\sw-two`: `game\` (the copy, carrying the stub), `transcript.jsonl`
(every call, with its session, its answer and where the answer came from), `a.log`
and `b.log` (the stub's own view of each instance, at `debug` level), and
`game-output.log` (both instances' `OutputDebugString`, the stub's lines and the
games' own, tagged by pid). The installed game is never written to - the copy is what
gets the stub, and `-GamePath` says where the original lives.

The games' own `OutputDebugString` lines *are* captured, by
[`tools/debug-output.ps1`](../tools/debug-output.ps1) - the rig starts it before the
instances and stops it after them, and it lands in `game-output.log` as `<pid> <text>`
so two games in one run stay apart. It exists because the harness records every call a
game makes and none of the opinions a game has about them, and a game's opinion is
where it says why it is unhappy: the guest sitting there doing nothing was only
readable as "the guest said nothing", and the host's *"unknown message on our listen
socket"* is the game, not the stub.

Two things about that wire: only one reader can hold the DBWIN buffer, so a debugger,
an IDE or DebugView running at the same time takes these lines; and the buffer is
filled by whoever calls `OutputDebugString`, so a reader that stops draining it makes
those calls block - which is why the rig starts the reader before the games and keeps
its handshake tight.

The stub's own debug lines go to that wire too (it writes them to `STEAMMOCK_LOG` and
to `OutputDebugString`), so `game-output.log` holds both; the games' own lines are the
ones without the `[steammock]` prefix.
