#!/usr/bin/env python3
"""End-to-end test: a fake game, the real stub DLL, the real backend.

    python python/tests/test_e2e.py --stub build/Release/steam_api64.dll \
                                    --game build/Release/fake_game.exe \
                                    --scenario python/steambridge/scenarios/example.json

It starts the backend as a subprocess on a port of its own choosing, runs the
fake game with that port in its environment, then checks both sides of the
conversation: what the game was told, and what the backend recorded about it.

This is the only test that exercises the C++ encoder against the Python decoder
and the other way round, so it is also the framing contract test. Exits non-zero
if a check fails.
"""

from __future__ import annotations

import argparse
import json
import os
import subprocess
import sys
import tempfile
import threading
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
PYTHON_ROOT = Path(__file__).resolve().parent.parent

failures = 0


def check(what: str, ok: bool, detail: str = "") -> None:
    global failures
    if ok:
        print(f"  [ok  ] {what}")
    else:
        failures += 1
        print(f"  [FAIL] {what}{(' - ' + detail) if detail else ''}")


def parse_key_values(output: str) -> dict:
    values = {}
    for line in output.splitlines():
        if "=" in line:
            key, _, value = line.partition("=")
            values[key.strip()] = value.strip()
    return values


class OutputCollector:
    """Reads a subprocess's output on a thread, so nothing blocks on readline."""

    def __init__(self, stream) -> None:
        self.lines: list[str] = []
        self._stream = stream
        self._thread = threading.Thread(target=self._pump, daemon=True)
        self._thread.start()

    def _pump(self) -> None:
        for line in self._stream:
            self.lines.append(line.rstrip("\r\n"))

    def text(self) -> str:
        return "\n".join(self.lines)

    def wait_for(self, needle: str, timeout: float = 20.0) -> bool:
        deadline = time.time() + timeout
        while time.time() < deadline:
            if any(needle in line for line in self.lines):
                return True
            time.sleep(0.02)
        return False


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--stub", type=Path, required=True, help="the built stub DLL")
    parser.add_argument("--game", type=Path, required=True, help="the built fake_game executable")
    parser.add_argument("--scenario", type=Path, required=True)
    parser.add_argument("--timeout", type=float, default=30.0)
    args = parser.parse_args()

    for path in (args.stub, args.game, args.scenario):
        if not path.exists():
            print(f"missing input: {path}", file=sys.stderr)
            return 2

    print("[+] SteamApiBridge end-to-end test")

    with tempfile.TemporaryDirectory(prefix="steambridge-e2e-") as workspace:
        transcript = Path(workspace) / "transcript.jsonl"
        environment = dict(os.environ)
        # The package is importable from python/ without an install step.
        environment["PYTHONPATH"] = str(PYTHON_ROOT) + os.pathsep + environment.get("PYTHONPATH", "")

        server = subprocess.Popen(
            [sys.executable, "-m", "steambridge", "--port", "0",
             "--scenario", str(args.scenario), "--transcript", str(transcript),
             "--log-level", "debug"],
            cwd=str(PYTHON_ROOT), env=environment, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
            text=True, bufsize=1,
        )
        collector = OutputCollector(server.stdout)
        try:
            if not collector.wait_for("listening on", args.timeout):
                print("the backend never started:", file=sys.stderr)
                print(collector.text(), file=sys.stderr)
                return 2
        except Exception:
            server.kill()
            raise

        listening = next(line for line in collector.lines if "listening on" in line)
        host_port = listening.split("listening on", 1)[1].strip()
        host, _, port = host_port.rpartition(":")
        print(f"    backend {host}:{port}")

        game_environment = dict(environment)
        game_environment["STEAMBRIDGE_STUB"] = str(args.stub)
        game_environment["STEAMBRIDGE_HOST"] = host
        game_environment["STEAMBRIDGE_PORT"] = port
        game_environment["STEAMBRIDGE_LOG_LEVEL"] = "debug"

        started = time.time()
        game = subprocess.run([str(args.game)], env=game_environment, capture_output=True, text=True,
                              timeout=args.timeout)
        values = parse_key_values(game.stdout)
        print(f"    game exited {game.returncode} after {time.time() - started:.2f}s")
        if game.returncode != 0:
            print(game.stdout, file=sys.stderr)
            print(game.stderr, file=sys.stderr)

        # --- what the game was told ---------------------------------------
        print("\n[:] answers the game received")
        check("the stub loaded and every export resolved", game.returncode == 0,
              f"exit {game.returncode}")
        check("SteamAPI_Init was answered true (scripted)", values.get("init") == "true",
              values.get("init", "missing"))
        check("the game sees Steam as running", values.get("is_running") == "true")
        check("the session handshake completed", values.get("session_set") == "true")
        check("the install path came back as text", values.get("install_path_set") == "true")
        check("the interface getters returned tokens", values.get("interfaces") == "true")
        check("the steam id came from the profile",
              values.get("steam_id") == "76561198000000001", values.get("steam_id", "missing"))
        check("the persona name came from the profile", values.get("persona") == "DebugPlayer",
              values.get("persona", "missing"))
        check("the app id came from the profile", values.get("app_id") == "480",
              values.get("app_id", "missing"))
        check("the language came from the profile", values.get("language") == "english",
              values.get("language", "missing"))

        # --- out parameters -------------------------------------------------
        print("\n[:] out parameters")
        check("GetStatInt32 wrote through the pointer", values.get("stat.Deaths.value") == "0",
              values.get("stat.Deaths.value", "missing"))
        check("a known stat reports success", values.get("stat.Deaths.found") == "true")
        check("SetStatInt32 reported success", values.get("stat.Deaths.write_ok") == "true")
        check("reading back after the write sees the new value",
              values.get("stat.Deaths.after_write.value") == "4",
              values.get("stat.Deaths.after_write.value", "missing"))
        check("the read-back also reports success",
              values.get("stat.Deaths.after_write.found") == "true")
        check("an unknown stat reports failure", values.get("stat.NoSuchStat.found") == "false")
        check("an unknown stat leaves the caller's variable alone",
              values.get("stat.NoSuchStat.value_untouched") == "true")
        check("an achievement can be read", values.get("achievement.ACH_BOOTED.value") == "false")
        check("unlocking an achievement reported success",
              values.get("achievement.ACH_BOOTED.unlocked") == "true")
        check("it reads back as unlocked", values.get("achievement.ACH_BOOTED.after") == "true")
        check("an unknown achievement reports failure",
              values.get("achievement.ACH_MISSING.found") == "false")
        check("an unknown achievement leaves the caller's variable alone",
              values.get("achievement.ACH_MISSING.value_untouched") == "true")
        check("the achievement list is indexed", values.get("achievement.0") == "ACH_BOOTED")
        check("StoreStats reported success", values.get("store_stats") == "true")

        # --- what the backend recorded -------------------------------------
        print("\n[:] what the backend recorded")
        deadline = time.time() + 5.0
        records: list[dict] = []
        while time.time() < deadline:
            if transcript.exists():
                records = [json.loads(line) for line in
                           transcript.read_text(encoding="utf-8").splitlines() if line.strip()]
                # Every call the game made has to be there by the time it exits.
                if values.get("forwarded") and len(records) >= int(values["forwarded"]):
                    break
            time.sleep(0.05)

        forwarded = int(values.get("forwarded", 0))
        # The transcript can hold more than the count the game printed: the game's
        # last calls happen after it asked for the count.
        check("the backend saw at least every call the stub had counted",
              len(records) >= forwarded, f"{len(records)} recorded, {forwarded} forwarded")
        check("the stub reported the two calls nobody answered",
              values.get("unhandled") == "2", values.get("unhandled", "missing"))
        check("the last thing the game did was shut down",
              bool(records) and records[-1]["call"] == "SteamAPI_Shutdown",
              records[-1]["call"] if records else "no records")

        sources = {record["call"]: record["via"] for record in records}
        check("identity was answered from the session state",
              sources.get("SteamAPI_ISteamUser_GetSteamID") == "state")
        check("a policy call was answered from the scenario",
              sources.get("SteamAPI_Init") == "scripted")
        check("RunCallbacks was forwarded but left to the stub",
              sources.get("SteamAPI_RunCallbacks") == "none")
        check("out parameters appear in the transcript",
              any(record.get("out", {}).get("pnData") == 0 for record in records
                  if record["call"].endswith("GetStatInt32")))
        check("the stats write is in the transcript",
              any(record["call"].endswith("SetStatInt32") and record["args"].get("nData") == 4
                  for record in records))
        check("every record names its session",
              all(record.get("session") for record in records))

        # --- shutdown -------------------------------------------------------
        # The server is killed rather than asked to stop (on Windows terminate()
        # is a hard stop), so the check here is that it logged the session, not
        # that it got to print a summary.
        check("the backend logged the connected game", "game connected" in collector.text())
        server.terminate()
        try:
            server.wait(timeout=10)
        except subprocess.TimeoutExpired:
            server.kill()

    print()
    if failures == 0:
        print("[+] all checks passed")
    else:
        print(f"[-] {failures} check(s) FAILED")
    return 0 if failures == 0 else 1


if __name__ == "__main__":
    raise SystemExit(main())
