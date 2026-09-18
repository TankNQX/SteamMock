"""SteamApiBridge - a local debugging backend for the Steamworks flat API.

The C++ stub named like ``steam_api64.dll`` forwards every call it exports over
loopback TCP to this package, which decides the answer. That makes it possible to
develop and debug a game's Steam integration locally, with scripted responses,
and only run against real Steam once the logic is settled.

Typical use::

    python -m steambridge --scenario python/steambridge/scenarios/example.json

See README.md and docs/architecture.md in the repository root.
"""

from .handlers import Dispatcher
from .protocol import PROTOCOL_VERSION, ProtocolError, decode_frames, encode
from .server import BridgeServer, serve
from .session import Profile, Session

__version__ = "0.1.0"

__all__ = [
    "BridgeServer",
    "Dispatcher",
    "PROTOCOL_VERSION",
    "Profile",
    "ProtocolError",
    "Session",
    "__version__",
    "decode_frames",
    "encode",
    "serve",
]
