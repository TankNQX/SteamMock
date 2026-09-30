# SteamMock

[![CI](https://github.com/TankNQX/SteamMock/actions/workflows/ci.yml/badge.svg)](https://github.com/TankNQX/SteamMock/actions/workflows/ci.yml) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE) ![Platform: Windows x64 and Win32](https://img.shields.io/badge/platform-Windows%20x64%20%2B%20Win32-0078d4) ![C++17](https://img.shields.io/badge/C%2B%2B-17-00599c) ![Steam client: not required](https://img.shields.io/badge/Steam%20client-not%20required-brightgreen) ![Steamworks SDK: required](https://img.shields.io/badge/Steamworks%20SDK-required-blue)

Born out of the constraints of multiplayer development and the need for a second Steam account and a
second PC to test the UI, invites most of all, this is a small Steam backend mock that speeds up
development.

<img alt="The live view" src="docs/images/live-view.png" width="800">

*Two copies of one game running at once. The view lists each as its own session, with the calls it
makes, what answered each one, how long it took, and the state that game is being told.*

Point a game at one DLL and every Steam call it makes lands in a window: the call, what answered it,
how long it took. No Steam client, no account, no Valve service, and no game code to change. Valve's
own test app runs against it through a lobby, a match, a leaderboard and its inventory screen.

## What works today

🟢 works. 🟡 in part. 🔴 not yet, so the game gets the value it would see with Steam absent.

| Status | Steam area | What a game gets |
| --- | --- | --- |
| 🟢 | Lobbies and matchmaking | rooms, their members, ready-up, lobby chat |
| 🟢 | Peer to peer networking | connections, packets, and a session per end of a process |
| 🟢 | Login and identity | who the player is, names after they leave, a peer let in on its ticket |
| 🟢 | App id, language, build id | answered from the profile the game was matched to |
| 🟢 | Leaderboards | a real board, filled by the players in this run |
| 🟢 | Stats and achievements | read and written, per profile |
| 🟢 | Inventory | a catalogue, and what the player holds |
| 🟡 | Controller, overlay, hosted logon | a scenario answers a few calls, the rest default |
| 🔴 | Music, video, screenshots, HTTP, UGC, Remote Storage, the HTML page, the server browser, parties, and the rest | nothing yet |

Valve's own test app exercises the green rows end to end. Two clients meet in a lobby, authenticate
each other, play a match, post a score to a leaderboard, and read their inventory off its own stats
screen. [development.md](docs/development.md#what-the-backend-answers) has the per-interface detail,
including the calls one run made and the 17 interfaces with nothing built in.

A red row is not a failure. The game gets the value it would see with Steam absent, so it keeps going
rather than being told something invented. Nothing here answers ownership, entitlement or licensing,
and a scenario can answer any call, red or green.

## Run it

You need Windows, Visual Studio 2022 or its Build Tools, CMake, a Steamworks SDK, and Valve's own test
app. Clone this repository with its submodules, then:

```bat
python -m venv .venv && .venv\Scripts\pip install -r tools\requirements.txt
.venv\Scripts\python tools\steamworks_sdk_import.py --sdk <sdk>\public\steam --out gen\steam_interfaces.json --surface gen\steam_api_surface.json
cmake -S . -B build -A Win32 -DSTEAMMOCK_BUILD_GUI=ON -DSTEAMMOCK_STUB_NAME=steam_api
cmake --build build --config Release
```

Copy Spacewar out of your library and put `build\Release\steam_api.dll` in the copy beside
`SteamworksExample.exe`. Start the window with `build\Release\steammock_gui.exe --scenario
scenarios\spacewar.json --start`, and run the game from the copy. Every call it makes lands in that
window instead of at Valve.

The stub exports the API an SDK describes, so an import is what makes it useful at all. Valve
publishes every generation in [their Proton repo](https://github.com/ValveSoftware/Proton), and
[development.md](docs/development.md#interface-layouts) has the clone command, the full walkthrough,
the tests, and what to check [when nothing appears](docs/development.md#when-nothing-appears).

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
