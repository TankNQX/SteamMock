"""The loopback server the stub talks to.

One connection is one game process. The server answers calls, writes a
line-delimited transcript of everything it saw, and keeps a ``Session`` per game
so several games can be debugged side by side - each with its own profile, its
own stats and its own achievements.
"""

from __future__ import annotations

import asyncio
import json
import logging
import time
import uuid
from pathlib import Path
from typing import Any, Callable

from .handlers import Dispatcher
from .protocol import PROTOCOL_VERSION, ProtocolError, read_message, reply, write_message
from .session import Session

LOG = logging.getLogger("steambridge")


class BridgeServer:
    """Accepts games, answers their calls, records what happened."""

    def __init__(
        self,
        host: str = "127.0.0.1",
        port: int = 50990,
        dispatcher: Dispatcher | None = None,
        transcript: str | Path | None = None,
    ) -> None:
        self.host = host
        self.port = port
        self.dispatcher = dispatcher or Dispatcher()
        self._transcript_path = transcript
        self._transcript = None
        self._server: asyncio.AbstractServer | None = None
        self.sessions: dict[str, Session] = {}
        self.total_calls = 0
        self.unanswered_calls = 0
        self.on_session: Callable[[Session], None] | None = None

    # -- lifecycle ----------------------------------------------------------

    async def start(self) -> int:
        """Bind and start serving; returns the port actually bound (0 picks one)."""
        if self._transcript_path is not None:
            Path(self._transcript_path).parent.mkdir(parents=True, exist_ok=True)
            self._transcript = Path(self._transcript_path).open("a", encoding="utf-8")
        self._server = await asyncio.start_server(self._handle_client, self.host, self.port)
        for sock in self._server.sockets or []:
            bound = sock.getsockname()
            if bound:
                self.port = int(bound[1])
                break
        LOG.info("listening on %s:%d", self.host, self.port)
        return self.port

    async def serve_forever(self) -> None:
        if self._server is None:
            await self.start()
        assert self._server is not None
        async with self._server:
            await self._server.serve_forever()

    async def stop(self) -> None:
        if self._server is not None:
            self._server.close()
            await self._server.wait_closed()
            self._server = None
        if self._transcript is not None:
            self._transcript.flush()
            self._transcript.close()
            self._transcript = None

    def summary(self) -> str:
        answered = self.total_calls - self.unanswered_calls
        return (
            f"{len(self.sessions)} game session(s), {self.total_calls} call(s), "
            f"{answered} answered, {self.unanswered_calls} left to the stub's defaults"
        )

    # -- one game -----------------------------------------------------------

    async def _handle_client(
        self, reader: asyncio.StreamReader, writer: asyncio.StreamWriter
    ) -> None:
        peer = writer.get_extra_info("peername")
        session: Session | None = None
        try:
            hello = await read_message(reader)
            if hello.get("type") != "hello":
                LOG.warning("first message from %s was %r, not a hello", peer, hello.get("type"))
                return

            session_id = uuid.uuid4().hex[:12]
            session = Session(session_id, hello, self.dispatcher.profile_for(hello))
            self.sessions[session_id] = session
            LOG.info("game connected: %s", session.describe())
            if self.on_session is not None:
                self.on_session(session)

            await write_message(
                writer,
                {
                    "type": "welcome",
                    "v": PROTOCOL_VERSION,
                    "session": session_id,
                    "profile": session.profile.name,
                },
            )

            while True:
                message = await read_message(reader)
                kind = message.get("type")
                if kind == "bye":
                    break
                if kind != "call":
                    LOG.warning("ignoring a %r message from %s", kind, session.id)
                    continue
                await self._handle_call(session, message, writer)

        except (asyncio.IncompleteReadError, ConnectionResetError, BrokenPipeError):
            # The normal way a game leaves.
            pass
        except ProtocolError as error:
            LOG.warning("protocol error from %s: %s", peer, error)
        except asyncio.CancelledError:
            raise
        except Exception:  # pragma: no cover - a debug harness must not die on one game
            LOG.exception("unexpected failure while serving %s", peer)
        finally:
            if session is not None:
                LOG.info("game disconnected: %s (%.1fs, %d calls)",
                         session.describe(), time.time() - session.started, len(session.calls))
            writer.close()
            try:
                await writer.wait_closed()
            except (ConnectionResetError, BrokenPipeError):
                pass

    async def _handle_call(
        self, session: Session, message: dict, writer: asyncio.StreamWriter
    ) -> None:
        name = str(message.get("name", ""))
        args = message.get("args") or {}
        seq = int(message.get("seq", 0))
        if not isinstance(args, dict):
            LOG.warning("%s: arguments were %r, not an object", name, type(args).__name__)
            args = {}

        started = time.perf_counter()
        answered, ret, out, source = self.dispatcher.answer(session, name, args)
        elapsed_ms = (time.perf_counter() - started) * 1000.0

        self.total_calls += 1
        if not answered:
            self.unanswered_calls += 1

        record = {
            "session": session.id,
            "seq": seq,
            "call": name,
            "args": args,
            "answered": answered,
            "via": source,
            "ms": round(elapsed_ms, 3),
        }
        if answered:
            record["ret"] = ret
            if out:
                record["out"] = out
        session.calls.append(record)
        self._write_transcript(record)

        if answered:
            LOG.debug("-> %s = %r%s", name, ret, f" out={out}" if out else "")
        else:
            LOG.debug("-- %s: no opinion, the stub uses its default", name)

        await write_message(writer, reply(seq, answered, ret, out))

    def _write_transcript(self, record: dict) -> None:
        if self._transcript is None:
            return
        self._transcript.write(json.dumps(record, ensure_ascii=False) + "\n")
        self._transcript.flush()


async def serve(
    host: str = "127.0.0.1",
    port: int = 50990,
    scenario: str | Path | None = None,
    transcript: str | Path | None = None,
    on_call: Callable | None = None,
    ready: Callable[[int], None] | None = None,
) -> BridgeServer:
    """Run until cancelled. Calls ``ready(port)`` once the socket is bound."""
    dispatcher = Dispatcher.load(scenario, on_call=on_call) if scenario else Dispatcher(on_call=on_call)
    server = BridgeServer(host=host, port=port, dispatcher=dispatcher, transcript=transcript)
    await server.start()
    if ready is not None:
        ready(server.port)
    try:
        await server.serve_forever()
    finally:
        await server.stop()
    return server
