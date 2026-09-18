"""Command line entry point: ``python -m steambridge``."""

from __future__ import annotations

import argparse
import asyncio
import json
import logging
import sys
from pathlib import Path

from . import __version__
from .protocol import PROTOCOL_VERSION
from .server import BridgeServer

DEFAULT_SCENARIO = Path(__file__).resolve().parent / "scenarios" / "example.json"
SURFACE = Path(__file__).resolve().parent / "surface.json"


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog="steambridge",
        description="Answer Steam API calls from a local game, so you can debug the integration "
                    "without running Steam.",
    )
    parser.add_argument("--host", default="127.0.0.1", help="interface to listen on (default 127.0.0.1)")
    parser.add_argument("--port", type=int, default=50990,
                        help="port to listen on; 0 picks a free one (default 50990)")
    parser.add_argument("--scenario", type=Path, default=DEFAULT_SCENARIO,
                        help=f"scenario file (default {DEFAULT_SCENARIO.name})")
    parser.add_argument("--transcript", type=Path, default=None,
                        help="append every call, in JSON lines, to this file")
    parser.add_argument("--log-level", default="info",
                        choices=["error", "warning", "info", "debug"],
                        help="how much the backend says about each call (default info)")
    parser.add_argument("--list-api", action="store_true",
                        help="print the calls the stub exports, then exit")
    parser.add_argument("--show-profiles", action="store_true",
                        help="print the scenario's games and match rules, then exit")
    parser.add_argument("--version", action="version", version=f"steambridge {__version__}")
    return parser


def print_api() -> int:
    if not SURFACE.exists():
        print("surface.json is missing - run: python gen/generate.py", file=sys.stderr)
        return 2
    surface = json.loads(SURFACE.read_text(encoding="utf-8"))
    print(f"# surface '{surface['surface']}' revision {surface['revision']}, "
          f"{len(surface['calls'])} calls")
    for call in surface["calls"]:
        params = ", ".join(
            f"{p['type']}{'*' if p['dir'] == 'out' else ''} {p['name']}" for p in call["params"]
        )
        print(f"  {call['returns']:>10} {call['name']}({params})")
    return 0


def show_profiles(scenario: Path) -> int:
    from .handlers import Dispatcher

    if not scenario.exists():
        print(f"no scenario at {scenario}", file=sys.stderr)
        return 2
    dispatcher = Dispatcher.load(scenario)
    print(f"scenario {scenario}")
    for name in dispatcher.profiles():
        print(f"  profile '{name}'")
    print("match rules, first one wins:")
    for rule in dispatcher._match:  # noqa: SLF001 - a debug view of the file we just loaded
        print(f"  {rule}")
    return 0


async def run(args: argparse.Namespace) -> int:
    from .handlers import Dispatcher

    if not args.scenario.exists():
        logging.error("no scenario at %s (try --scenario python/steambridge/scenarios/example.json)",
                      args.scenario)
        return 2

    logging.basicConfig(
        level=getattr(logging, args.log_level.upper()),
        format="%(asctime)s %(levelname)-7s %(message)s",
        datefmt="%H:%M:%S",
    )
    log = logging.getLogger("steambridge")

    dispatcher = Dispatcher.load(args.scenario)
    server = BridgeServer(host=args.host, port=args.port, dispatcher=dispatcher,
                          transcript=args.transcript)
    port = await server.start()
    log.info("protocol v%d, scenario %s, profiles %s",
             PROTOCOL_VERSION, args.scenario.name, ", ".join(dispatcher.profiles()))
    if server._transcript_path is not None:  # noqa: SLF001 - reporting the user's own argument
        log.info("transcript: %s", server._transcript_path)
    # Printed in a fixed shape so a test (or a script) can find the port when 0
    # was requested.
    print(f"listening on {args.host}:{port}", flush=True)

    try:
        await server.serve_forever()
    except (KeyboardInterrupt, asyncio.CancelledError):
        pass
    finally:
        await server.stop()
    log.info("done: %s", server.summary())
    return 0


def main(argv: list[str] | None = None) -> int:
    args = build_parser().parse_args(argv)
    if args.list_api:
        return print_api()
    if args.show_profiles:
        return show_profiles(args.scenario)
    try:
        return asyncio.run(run(args))
    except KeyboardInterrupt:
        return 0


if __name__ == "__main__":
    raise SystemExit(main())
