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

## The short run, for work on one step of it

Most of what a run is read for is decided in the second after *Start game*: the game
server comes up, each client sends its ticket, and the game server passes the ones Steam
says are good. On the recordings that is 42 ms of traffic (`t+30390` to `t+30432`), and it
is the first thing a run can be judged by:

```bat
pwsh -File tools\two-instance-test.ps1 -Clients 3 -StopWhenDecided -WaitAfterStart 12
```

`-StopWhenDecided` stops the run as soon as every client has been passed (the host's
`k_EMsgServerPassAuthentication` is message 3 at the front of its packets), or when
`-WaitAfterStart` has elapsed if it never happens - so a run that holds is over in a few
seconds past the keypress and one that does not is over at the ceiling. Around **40
seconds** a run rather than two and a half minutes, with no recording and no live view,
and the rig says which it was as it stops (`all 3 client(s) were passed authentication`,
or `only 2 of 3 within 12s`).

What it gives up is the part of the run after that: the loser's own 30-second ticket
timeout, the ships, the round - the *symptom* rather than the outcome. Judge a run by
which players the host's game server passed, and read the mechanism behind it out of the
transcript with
[`tools/transcript_timeline.py`](../tools/transcript_timeline.py) `--rig <dir>
--summary --callbacks`; keep the long form for the runs that are evidence rather than
iteration.

## The full room

`-Clients 4` fills the game: `SpaceWar.h` declares `MAX_PLAYERS_PER_SERVER` as 4, so a
fourth client is the last one it has a slot for and a fifth would be listed in the lobby
and never given one - which the rig refuses rather than run and misreport. The fourth
instance is launched, tiled and stopped exactly as the third is, with its own profile in
`scenarios/spacewar.json` (a client without one of its own becomes the default identity,
which is how three clients once ran as two players).

The form is the same run of record with one more client:

```bat
pwsh -File tools\two-instance-test.ps1 -Clients 4 -Gui -WaitAfterStart 100
```

Ten of those, back to back, is what a set looks like -
`.reasonix\scratch\soak.ps1 -Clients 4 -Runs 10 -KeepTranscripts -Root %TEMP%\sw-set4`
records a row per run and writes `summary.txt`. The set of 29 Sep 2026 came out the same
in every one of the ten:

| | |
| :-- | :-- |
| the host's game, in its own words | `Auth completed for a client` **4** times (3 would be a player left out) |
| the room as the host sees it | **4** members |
| the host's game server asked Steam | `SteamAPI_ISteamGameServer_BeginAuthSession` **4** |
| answers to those, and to the client's own | `ValidateAuthTicketResponse_t` delivered **7 / 1 / 1 / 1** (host / B / C / D) |
| on the host's id 143, which has **4** objects on it | **4** answers to the first (each a game server's) and **3** to the last (the client's own peers) |
| lobby notifications delivered per client | `LobbyDataUpdate_t` **7-11**, `LobbyChatUpdate_t` 2-5 |
| crashes, callback faults, dropped keystrokes | **0 / 0 / 0** |
| the run | ~200 s, 1.5-1.8 M calls, ~450 MB of transcript |

So every player was let in, in all ten, at the largest room the game has - and the answer
to "did every client get in" needs no transcript at all: it is one line, repeated once per
player, in the host's own output (`lines.txt` folds it, `summary.txt` counts it as
`auths`).

What the four-client form does *not* add is a longer look at the match: the ships, the
round and the score are the same blind spot the two- and three-client forms have, and a
run is judged by who was let in rather than by who won. `-StopWhenDecided` and the deep
form both exist for that reason; the difference between them is how much of the
aftermath a reader wants to see.

### More clients than the game seats

`-Overfill` lifts that ceiling for the one run it makes sense in: the extra clients join the
lobby, are listed to everyone, and are never given a slot, because `MAX_PLAYERS_PER_SERVER`
is the game's number and the harness does not overrule it. It is a test of one process
holding a crowd - sessions, rosters, notification fan-out, the live view - and not of a
match. Eleven clients, one lobby, one run of 29 Sep 2026:

| | |
| :-- | :-- |
| the room as the host sees it | **11** members (`GetLobbyMemberByIndex` x329 on the host alone) |
| the host's game server asked Steam about | **11** players (`SteamAPI_ISteamGameServer_BeginAuthSession` 11) |
| the host's game, in its own words | `Auth completed for a client` **8** times, and 7 x `client leaving server msg, but couldn't find a matching client` |
| seated | **4** - `StartAuthPlayer slot=1,2,3` beside the host's own slot 0, four ships on the host's screen mid-round |
| the other seven | left the lobby after the game's own 30-second ticket timeout, each showing the game's `Connection failure / Multiplayer authentication failed` |
| held at once | 11 sessions, ~1.9 M calls, 672 MB of rig dir |
| crashes, callback faults, dropped keystrokes | 0 / 0 / 0 |

`-Shoot` is what makes a crowd run worth looking at: mid-match it writes the tiled screen as
one image (every window and the live view together), the live view grown big enough to read
its games table - which is where the eleven rows are - the host's round, and the first
unseated client. What the crowd does *not* change is the routing: the host's id 143 carries
four objects, the same four answers go to the first of them and the same three to the last,
exactly as in the four-client set.

## The board

Every run now walks the leaderboard menu, and it is the first thing in a run that reflects
what happened in the match rather than who was let into it.

Two walks, in fact. The first is instance A's, before it creates the lobby, because that is
the only moment a client can reach the main menu with the run still ahead of it:
`Leaderboards` is the eighth of seventeen items, so it is seven downs and a return. What
comes up is a board with nothing on it - nobody has finished a round yet - and that is what
`leaderboard-empty.png` shows: the header, `No scores for this leaderboard`, `Next
leaderboard`, `Return to main menu`. The way *out* is Escape, not a count of downs: this
menu's item count changes with the board, while Escape is read by the client's own state
machine (`k_EClientLeaderboards` → `k_EClientGameMenu`).

The second is a guest's, at the end of the run, and it is the one worth looking at. It has
to be a guest that **played**: Spacewar's server ends a round as a draw the moment a second
player joins - it does that so that one player cannot float around and keep the next one
out - and the game posts what it earned to the `Feet Traveled` board when a round ends, so
the clients that were there when the draw happened have real scores on a real board. It
also has to be a guest rather than the host, because a guest never gets a key at the main
menu (it joins on the command line), which is what lets the reader's seven downs count from
index zero.

Leaving the match is how a player leaves one: Escape opens the quit menu (`Resume Game`,
`Exit To Menu`, `Exit To Desktop`), so one down and a return is `Exit To Menu` and the
client keeps the score it posted. It then shows the quickest win first - empty, because a
draw is nobody's win and so nothing was uploaded to it - and, through `Next leaderboard`,
the feet travelled, read **around itself**. That last part is why the reader had to have
played: a real Steam returns nothing for a user with no entry on the board, Spacewar's own
menu says so in a comment, and this world does the same.

Here is the whole thing on a two-client run:

```
Leaderboard: Feet Traveled, Around User
(1) DebugPlayer2 - 277
(2) DebugPlayer - 43
{ Next leaderboard }
Return to main menu
```

Two real rows, in rank order, with the score each player earned in the round that drew and
the name of the profile it was launched as - and the names come from the run's roster rather
than from a lobby, because neither player is in a room by then. That is the leaderboard, the
async call results and the persona roster all visible in one screen.

The report has a line per board call, which is what a reader checks a run by:

```
the boards: FindOrCreateLeaderboard=4 GetLeaderboardName=10 GetLeaderboardEntryCount=0
            DownloadLeaderboardEntries=3 GetDownloadedLeaderboardEntry=2 UploadLeaderboardScore=11
```

The counts are the shape of the run: four finds (both boards, once per client), three
downloads (A's pass and the reader's two boards), **two row reads** - the thing that needs a
board with somebody on it - and eleven uploads, which is what a round ending does.

Three traps, all of which cost a run to find:

* A board is empty until a round ends, so a run that never seats a second player has nothing
  to download and nothing to read. The line above is the thing to read, not the screenshot.
* `Next leaderboard` is second from the bottom, so the downs it takes depend on how many rows
  are drawn above it. The reader's first board is the one board that is reliably empty, which
  is what makes its two downs a count rather than a guess.
* The main menu keeps its cursor. A client driven to `Leaderboards` and then back is still on
  `Leaderboards`, so a drive that "starts from the top" has to put it there: seven ups, which
  is the top whether the menu clamps at the first item or wraps around to it.

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

A process that hosts is two ends on one pipe - a customer and a game server - and
the world keeps its packet queue per session, so it hands a packet to whichever of
the two asks first. `LobbyWorld::peek_packet` says so in as many words: it merges
the customer's queue with the game server's, on the grounds that a packet for either
of them is for this process. It is - but not for *either* half of it.

So the host's own customer reads the connect message its own game server was sent:
`f5 01 00 00` = 501, `k_EMsgClientInitiateConnection`, answered with *"Received
unknown message on our listen socket"*. That is `SpaceWarClient.cpp:747`, the
customer's side of the mail; the game server's own complaint about a message it
cannot place would be *"Bad connection attempt msg"* (`SpaceWarServer.cpp:530`). No
UDP socket is bound anywhere: this traffic is P2P, and "listen socket" is the game's
own name for its P2P receive. Reading a packet does not put it back, so the game
server never sees the one its customer sent - which is why a guest that dialled
carefully goes unanswered.

The two halves cannot be told apart as things stand. `ReadP2PPacket` carries no
identity, and neither does an object: the stub hands out **one object per version
string**, and both halves reach `ISteamNetworking` through the same accessor with the
same arguments. The transcript shows it - `SteamAPI_ISteamClient_GetISteamNetworking`
twice per session, at seq 15 for the customer and at seq 33121 when the game server
starts, both `hSteamUser: 1, hSteamPipe: 1`, because the harness answers
`SteamGameServer_GetHSteamUser` exactly the way it answers the customer's. Real Steam
gives a process's game server its own user handle, and that is the way in: a handle
of its own, an object per handle, the calls that need it carrying the handle they were
made through, and the world reading from the queue of the end that asked.

That is what the harness does now, and the run above is from before it. Each version
string is handed out as one object per user handle, the handle rides along with every
`ISteamNetworking` call - see `docs/protocol.md` - and a read is served from the queue of
the end that asked, so the host's customer and its game server no longer read each
other's mail. The end that *sent* a packet is read off the same handle rather than
guessed from where the packet was addressed to, which costs a guest the host's P2P auth
ticket when it is guessed: the ticket goes out through the customer's object to another
player, and stamped with the game server's id instead it arrives looking like a server
message. That is `Unhandled message from server` on each guest, then `P2P:: No ticket
received for account=<the host>` thirty seconds later, and then the guests drop the
owner off his own server.

A shortcut worth knowing: the game's own `+connect` switch takes a **number** rather
than a dotted address (`"+connect %d:%d"` in its string table), so
`+connect 2130706433:27015` - 127.0.0.1 in host order, which is what the harness
publishes in `SteamAPI_ISteamGameServer_GetPublicIP` - walks straight to the dial
without the lobby dance.

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
* **A key on its own needs the window brought forward, not just posted.** `Key()` posts to
  whatever has the front, and only `Drive()` focuses first - so a `Key()` sent a while after
  the last drive goes to a window that is not in front and is dropped. `Press()` is the
  helper that focuses and posts one key; the reader's Escape is what found this, by not
  happening at all while its screenshot showed a match that was still running.
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

With `-Record`, also `two-instances.mp4`: every window the run has, tiled into a grid - two
across from four of them - and filmed from the first window appearing to the end of the
match, so the menus and the keypresses that drive them are in it too. It ends by asking
ffmpeg for `q` rather than killing it, because an mp4 whose index never got written is a
file nothing can play; a four minute ceiling is passed as well, in case the run dies.

Those files are read back with `python tools/transcript_timeline.py --rig <the rig
directory>`, which decodes the game's own message ids in the packet exchange and, with
`--callbacks`, says which of the players the host's game server was handed a ticket for it
never passed. It handles the two ways this evidence misleads on its own: an
out-parameter's `args` is the caller's value rather than the answer (the peer a packet
came from is in `out`), and `game-output.log` is the machine's debug buffer, so only the
lines whose pid belongs to this run are read.

`-Gui` puts the live view in the grid as the backend rather than beside it, and
`-Clients 3` adds a third client the same way the second is there - `-Clients 4` the
fourth, as [the full room](#the-full-room) describes. A two-client match of a minute is
about six megabytes of video; three clients and the window over a whole session came out
at thirty.

That paragraph used to end by saying what the recording showed - the host playing a round
while the guests were told *"Multiplayer authentication failed"* and sat at the menus. It
was true of the runs of 24 Sep 2026 and it is not true of the ones since the auth work
and the sender stamp: in the ten four-client runs of 29 Sep 2026 no run said
`authentication failed`, `No ticket` or `Nothing received` at all, and the host's game
said `Auth completed for a client` once per player. A recording is still worth watching
for the menus and the round; what it is not any more is the way to find out whether the
guests got in.

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
