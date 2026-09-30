# SteamMock

[![CI](https://github.com/TankNQX/SteamMock/actions/workflows/ci.yml/badge.svg)](https://github.com/TankNQX/SteamMock/actions/workflows/ci.yml) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE) ![Platform: Windows x64 and Win32](https://img.shields.io/badge/platform-Windows%20x64%20%2B%20Win32-0078d4) ![C++17](https://img.shields.io/badge/C%2B%2B-17-00599c) ![Steam client: not required](https://img.shields.io/badge/Steam%20client-not%20required-brightgreen) ![Steamworks SDK: optional](https://img.shields.io/badge/Steamworks%20SDK-optional-blue)

Watch a real game's Steam calls arrive, live, without Steam.

![The live view](docs/images/live-view.png)

*Two copies of one game running at once. The view lists each as its own session, with the calls it
makes, what answered each one, how long it took, and the state that game is being told.*

![Three copies of the game and the live view in a grid](docs/images/three-instances.gif)

*Three of them, and the view with them. One makes a lobby, the others join it, and all three are
authenticated against each other. [The full recording, 30 MB, mp4](https://github.com/TankNQX/SteamMock/releases/download/demo-two-instances/two-instances.mp4)
runs from the lobby menus through to the match, taken by the rig in `tools/`.*

Point a game at one DLL and every Steam call it makes lands in a window: the call, what answered it,
how long it took. No Steam client, no account, no Valve service, and no game code to change. Valve's
own test app runs against it through a lobby, a match, a leaderboard and its inventory screen.

## What works today

🟢 works. 🟡 in part. 🔴 not yet, so the game gets the value it would see with Steam absent.

| Status | Steam area | What a game gets |
| --- | --- | --- |
| 🟢 works | Lobbies and matchmaking | rooms, their members, ready-up, lobby chat |
| 🟢 works | Peer to peer networking | connections, packets, and a session per end of a process |
| 🟢 works | Login and identity | who the player is, names after they leave, a peer let in on its ticket |
| 🟢 works | App id, language, build id | answered from the profile the game was matched to |
| 🟢 works | Leaderboards | a real board, filled by the players in this run |
| 🟢 works | Stats and achievements | read and written, per profile |
| 🟢 works | Inventory | a catalogue, and what the player holds |
| 🟡 in part | Controller, overlay, hosted logon | a scenario answers a few calls, the rest default |
| 🔴 not yet | Music, video, screenshots, HTTP, UGC, Remote Storage, the HTML page, the server browser, parties, and the rest | nothing yet |

Valve's own test app exercises the green rows end to end. Two clients meet in a lobby, authenticate
each other, play a match, post a score to a leaderboard, and read their inventory off its own stats
screen. [development.md](docs/development.md#what-the-backend-answers) has the per-interface detail,
including the calls one run made and the 17 interfaces with nothing built in.

A red row is not a failure. The game gets the value it would see with Steam absent, so it keeps going
rather than being told something invented. Nothing here answers ownership, entitlement or licensing,
and a scenario can answer any call, red or green.

## Run it

You need Windows, Visual Studio 2022 or its Build Tools, CMake, and Valve's own test app, which Steam
installs as app 480 in your library. Clone this repository with its submodules, and run
`git submodule update --init --recursive` if you cloned without them.

**1. Build the stub and the window.** 32-bit, because Spacewar's own program is 32-bit.

```bat
cmake -S . -B build -A Win32 -DSTEAMMOCK_BUILD_GUI=ON -DSTEAMMOCK_STUB_NAME=steam_api
cmake --build build --config Release
```

**2. Copy the game out of your library**, for example to `C:\dev\spacewar`, and put
`build\Release\steam_api.dll` into the copy, next to `SteamworksExample.exe`. You work on the copy,
so your installed game never changes.

**3. Start the window.** From your checkout:

```bat
build\Release\steammock_gui.exe --scenario scenarios\spacewar.json --start
```

It opens already serving and says `listening on 127.0.0.1:50990`.

**4. Start the game** from your copy, not from Steam.

**5. Watch.** A row appears for the game and the calls start scrolling past. Click around in the
game's own window, Stats and Achievements or Friends, and those calls appear as you make them.

If nothing appears, check three things. The game has to be the one from your copy, with
`steam_api.dll` beside it. The DLL has to be the 32-bit build from step 1. And the window has to be
running first, though a game started before it finds it on the next call. To see the other side of the
conversation, set `STEAMMOCK_LOG` to a file before starting the game and read what the stub did.

## The live view

* **Games.** Each running game, the profile it was matched to, and whether it is still connected. Two
  copies of one game run at once, each with its own session and its own row.
* **Calls.** The filter box narrows both views. **by function** counts the calls, most-called first,
  which stays readable when a game polls one call every frame. **live** is the call-by-call list,
  where `via` says where each answer came from and `ms` says how long it took.
* **Game state.** Who the game thinks it is talking to: app id, Steam id, persona, language, and the
  stats and achievements it has been told about.

## Where to read more

* Build it, test it, and what CI checks: [development.md](docs/development.md).
* How the two halves talk: [protocol.md](docs/protocol.md). How the whole thing is put together:
  [architecture.md](docs/architecture.md).
* Drive two copies of a real game, with the traps that cost runs: [two-instance-test.md](docs/two-instance-test.md).

## What this is not

* **Not for games you do not own.** It answers no ownership or entitlement question.
* **Not shippable.** A substitute `steam_api.dll` is a development tool. Keep it out of anything you
  distribute.
* **Not a Steam emulator.** It never talks to Valve, so anything that needs the real service is
  scripted call by call.
* **Not Valve's code or data.** No SDK, header, library or interface layout is in this repository, and
  nothing generated from them is committed.

Licensed under MIT, see `LICENSE`.
