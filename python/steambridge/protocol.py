"""Framing and message helpers.

The exact mirror of ``src/transport_tcp.cpp``: a 4 byte little-endian length,
then a UTF-8 JSON object. Kept dependency free and in one place so the stub and
the backend can only disagree about it in one file each.
"""

from __future__ import annotations

import asyncio
import json
import struct
from typing import Any

PROTOCOL_VERSION = 1

# Matches kMaxFrameBytes in the C++ transport: a wrong or hostile length prefix
# must not make either side allocate wildly.
MAX_FRAME_BYTES = 4 * 1024 * 1024


class ProtocolError(Exception):
    """Raised for anything that is not a well-formed frame."""


def encode(message: dict) -> bytes:
    """Serialise one message, length prefix included."""
    payload = json.dumps(message, separators=(",", ":"), ensure_ascii=False).encode("utf-8")
    if not payload:
        raise ProtocolError("refusing to send an empty message")
    if len(payload) > MAX_FRAME_BYTES:
        raise ProtocolError(f"message of {len(payload)} bytes exceeds the frame limit")
    return struct.pack("<I", len(payload)) + payload


def decode_frames(buffer: bytes) -> tuple[list[dict], bytes]:
    """Decode every complete frame in ``buffer``.

    Returns the messages and whatever was left over, so this works both for
    tests and for a blocking reader that does not want asyncio.
    """
    messages: list[dict] = []
    offset = 0
    while len(buffer) - offset >= 4:
        (length,) = struct.unpack_from("<I", buffer, offset)
        if length == 0 or length > MAX_FRAME_BYTES:
            raise ProtocolError(f"impossible frame length {length}")
        if len(buffer) - offset - 4 < length:
            break
        payload = buffer[offset + 4 : offset + 4 + length]
        try:
            message = json.loads(payload.decode("utf-8"))
        except (UnicodeDecodeError, json.JSONDecodeError) as error:
            raise ProtocolError(f"unparsable frame payload: {error}") from error
        if not isinstance(message, dict):
            raise ProtocolError("a frame payload has to be a JSON object")
        messages.append(message)
        offset += 4 + length
    return messages, buffer[offset:]


async def read_message(reader: asyncio.StreamReader) -> dict:
    """Read exactly one message, or raise ``IncompleteReadError`` at EOF."""
    header = await reader.readexactly(4)
    (length,) = struct.unpack("<I", header)
    if length == 0 or length > MAX_FRAME_BYTES:
        raise ProtocolError(f"impossible frame length {length}")
    payload = await reader.readexactly(length)
    try:
        message = json.loads(payload.decode("utf-8"))
    except (UnicodeDecodeError, json.JSONDecodeError) as error:
        raise ProtocolError(f"unparsable frame payload: {error}") from error
    if not isinstance(message, dict):
        raise ProtocolError("a frame payload has to be a JSON object")
    return message


async def write_message(writer: asyncio.StreamWriter, message: dict) -> None:
    writer.write(encode(message))
    await writer.drain()


def reply(seq: int, answered: bool, ret: Any = None, out: dict | None = None, **extra: Any) -> dict:
    """Build the answer to one call.

    ``answered`` is the whole point of the protocol: false means "no opinion",
    and the stub then uses the value it would use with Steam not running.
    """
    message: dict[str, Any] = {
        "type": "reply",
        "v": PROTOCOL_VERSION,
        "seq": seq,
        "answer": "handled" if answered else "default",
    }
    if answered:
        message["ret"] = ret
        if out:
            message["out"] = out
    message.update(extra)
    return message
