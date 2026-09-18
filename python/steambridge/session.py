"""The per-game state machine.

One ``Session`` per connected game process. It mirrors the parts of Steam the
harness can model locally - identity, language, app id, stats and achievements -
so a game can boot, read, write and store as it normally would, and so a
transcript shows what it asked and what it was told.

Two things it deliberately does not do:

* answer policy calls (``SteamAPI_Init``, ``SteamAPI_IsSteamRunning``, the
  interface getters). Those decide whether a game believes Steam is present, so
  they stay "no opinion" unless a scenario scripts them. See handlers.py.
* answer anything about ownership, entitlement or licensing.
"""

from __future__ import annotations

import time
from dataclasses import dataclass, field
from typing import Any


@dataclass
class Profile:
    """What one game is told about the world, from a scenario file."""

    name: str = "default"
    app_id: int = 0
    steam_id: int = 76561197960287930
    persona_name: str = "DebugPlayer"
    language: str = "english"
    ui_language: str = "english"
    install_path: str = r"C:\Program Files (x86)\Steam"
    build_id: int = 1
    stats: dict[str, int] = field(default_factory=dict)
    achievements: list[dict] = field(default_factory=list)
    scripted: dict[str, dict] = field(default_factory=dict)

    @staticmethod
    def from_dict(name: str, data: dict) -> "Profile":
        profile = Profile(name=name)
        profile.app_id = int(data.get("app_id", profile.app_id))
        profile.steam_id = int(data.get("steam_id", profile.steam_id))
        profile.persona_name = str(data.get("persona_name", profile.persona_name))
        profile.language = str(data.get("language", profile.language))
        profile.ui_language = str(data.get("ui_language", profile.ui_language))
        profile.install_path = str(data.get("install_path", profile.install_path))
        profile.build_id = int(data.get("build_id", profile.build_id))
        profile.stats = {str(k): int(v) for k, v in dict(data.get("stats", {})).items()}
        profile.achievements = [dict(entry) for entry in data.get("achievements", [])]
        profile.scripted = {str(k): dict(v) for k, v in dict(data.get("scripted", {})).items()}
        return profile

    def achievement_index(self, name: str) -> int | None:
        for index, entry in enumerate(self.achievements):
            if str(entry.get("name", "")) == name:
                return index
        return None


class Session:
    """The live state of one game process."""

    def __init__(self, session_id: str, hello: dict, profile: Profile) -> None:
        self.id = session_id
        self.pid = int(hello.get("pid", 0))
        self.exe = str(hello.get("exe", ""))
        self.arch = str(hello.get("arch", ""))
        self.profile = profile
        self.started = time.time()
        self.calls: list[dict] = []
        self.stats_written: dict[str, int] = {}
        self.achievements_set: list[str] = []

    # -- identity -----------------------------------------------------------

    def describe(self) -> str:
        return f"session {self.id} ({self.exe or 'unknown exe'}, pid {self.pid}, profile '{self.profile.name}')"

    # -- the calls the harness can answer from state ------------------------

    def handle(self, name: str, args: dict) -> tuple[bool, Any, dict | None]:
        """Answer one call, or report that this session has no opinion.

        Returns ``(answered, return_value, out_parameters)``.
        """
        handler = _STATE_HANDLERS.get(name)
        if handler is None:
            return (False, None, None)
        return handler(self, args)

    # -- individual behaviours ---------------------------------------------

    def _get_stat(self, args: dict) -> tuple[bool, Any, dict | None]:
        key = str(args.get("pchName", ""))
        if key not in self.profile.stats:
            # Steam reports failure for a name it does not know, and leaves the
            # caller's variable alone - which is exactly what we do here too.
            return (True, False, None)
        return (True, True, {"pnData": int(self.profile.stats[key])})

    def _set_stat(self, args: dict) -> tuple[bool, Any, dict | None]:
        key = str(args.get("pchName", ""))
        value = int(args.get("nData", 0))
        # Setting an unknown name is accepted and remembered, so a game can
        # invent a stat locally without the scenario having listed it first.
        self.profile.stats[key] = value
        self.stats_written[key] = value
        return (True, True, None)

    def _get_achievement(self, args: dict) -> tuple[bool, Any, dict | None]:
        name = str(args.get("pchName", ""))
        index = self.profile.achievement_index(name)
        if index is None:
            return (True, False, None)
        achieved = bool(self.profile.achievements[index].get("achieved", False))
        return (True, True, {"pbAchieved": achieved})

    def _set_achievement(self, args: dict) -> tuple[bool, Any, dict | None]:
        name = str(args.get("pchName", ""))
        index = self.profile.achievement_index(name)
        if index is None:
            return (True, False, None)
        self.profile.achievements[index]["achieved"] = True
        if name not in self.achievements_set:
            self.achievements_set.append(name)
        return (True, True, None)

    def _achievement_name(self, args: dict) -> tuple[bool, Any, dict | None]:
        index = int(args.get("iAchievement", 0))
        if index < 0 or index >= len(self.profile.achievements):
            return (True, "", None)
        return (True, str(self.profile.achievements[index].get("name", "")), None)


def _const(value: Any):
    def handler(session: Session, args: dict) -> tuple[bool, Any, dict | None]:
        del session, args
        return (True, value, None)

    return handler


def _seconds_since_app_active(session: Session, args: dict) -> tuple[bool, Any, dict | None]:
    del args
    return (True, int(time.time() - session.started), None)


def _server_real_time(session: Session, args: dict) -> tuple[bool, Any, dict | None]:
    del session, args
    # Steam hands out UTC seconds; a game only ever uses it for clock sanity.
    return (True, int(time.time()), None)


def _profile_app_id(session: Session, args: dict) -> tuple[bool, Any, dict | None]:
    del args
    return (True, session.profile.app_id, None)


def _profile_steam_id(session: Session, args: dict) -> tuple[bool, Any, dict | None]:
    del args
    return (True, session.profile.steam_id, None)


def _profile_persona(session: Session, args: dict) -> tuple[bool, Any, dict | None]:
    del args
    return (True, session.profile.persona_name, None)


def _profile_language(session: Session, args: dict) -> tuple[bool, Any, dict | None]:
    del args
    return (True, session.profile.language, None)


def _profile_ui_language(session: Session, args: dict) -> tuple[bool, Any, dict | None]:
    del args
    return (True, session.profile.ui_language, None)


def _profile_install_path(session: Session, args: dict) -> tuple[bool, Any, dict | None]:
    del args
    return (True, session.profile.install_path, None)


def _profile_build_id(session: Session, args: dict) -> tuple[bool, Any, dict | None]:
    del args
    return (True, session.profile.build_id, None)


def _request_current_stats(session: Session, args: dict) -> tuple[bool, Any, dict | None]:
    del args
    # In real Steam this only queues the request and the answer arrives as a
    # UserStatsReceived callback. Callback injection is the next feature; for now
    # the stats are simply readable, which is what a game does right afterwards.
    del session
    return (True, True, None)


def _store_stats(session: Session, args: dict) -> tuple[bool, Any, dict | None]:
    del args
    del session
    return (True, True, None)


def _num_achievements(session: Session, args: dict) -> tuple[bool, Any, dict | None]:
    del args
    return (True, len(session.profile.achievements), None)


# Calls answered from session state. Everything absent here is either scripted by
# the scenario or reported as "no opinion", which makes the stub fall back to the
# value it would use with Steam not running.
_STATE_HANDLERS = {
    "SteamAPI_GetHSteamUser": _const(1),
    "SteamAPI_GetHSteamPipe": _const(1),
    "SteamAPI_GetSteamInstallPath": _profile_install_path,
    "SteamAPI_ISteamUser_GetSteamID": _profile_steam_id,
    "SteamAPI_ISteamFriends_GetPersonaName": _profile_persona,
    "SteamAPI_ISteamUtils_GetAppID": _profile_app_id,
    "SteamAPI_ISteamUtils_GetCurrentGameLanguage": _profile_language,
    "SteamAPI_ISteamUtils_GetSteamUILanguage": _profile_ui_language,
    "SteamAPI_ISteamUtils_GetSecondsSinceAppActive": _seconds_since_app_active,
    "SteamAPI_ISteamUtils_GetServerRealTime": _server_real_time,
    "SteamAPI_ISteamApps_GetAppBuildId": _profile_build_id,
    "SteamAPI_ISteamUserStats_RequestCurrentStats": _request_current_stats,
    "SteamAPI_ISteamUserStats_StoreStats": _store_stats,
}

# These need the argument object, so they are registered separately rather than
# through the _const shortcut.
_ARG_HANDLERS = {
    "SteamAPI_ISteamUserStats_GetStatInt32": Session._get_stat,
    "SteamAPI_ISteamUserStats_SetStatInt32": Session._set_stat,
    "SteamAPI_ISteamUserStats_GetAchievement": Session._get_achievement,
    "SteamAPI_ISteamUserStats_SetAchievement": Session._set_achievement,
    "SteamAPI_ISteamUserStats_GetNumAchievements": _num_achievements,
    "SteamAPI_ISteamUserStats_GetAchievementName": Session._achievement_name,
}

_STATE_HANDLERS.update(_ARG_HANDLERS)

# Exposed so handlers.py can ask "does the session know this one?" without
# reaching into the table itself.
STATE_HANDLED_CALLS = frozenset(_STATE_HANDLERS)
