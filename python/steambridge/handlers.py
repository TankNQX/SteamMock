"""Turning a scenario file plus a session into an answer.

Resolution order for every call, first one that speaks wins:

1. a ``scripted`` entry for that call in the game's profile     (``via: scripted``)
2. a hook the caller passed in - ``on_call(session, name, args)``  (``via: hook``)
3. the session state machine (identity, stats, achievements)     (``via: state``)
4. nobody: the backend reports "no opinion" and the stub uses its own default
                                                                (``via: none``)

That last line is the important one. A call nobody answers behaves exactly as it
would with Steam not running, so a game cannot be handed a success it did not ask
a scenario for - and the transcript always says which of the four happened.
"""

from __future__ import annotations

import json
from pathlib import Path
from typing import Any, Callable

from .session import Profile, Session

Hook = Callable[[Session, str, dict], Any]


class Dispatcher:
    """The scenario, the hooks and the state machine behind one interface."""

    def __init__(self, scenario: dict | None = None, on_call: Hook | None = None) -> None:
        scenario = scenario or {}
        self._profiles: dict[str, Profile] = {}
        for name, data in dict(scenario.get("profiles", {})).items():
            self._profiles[name] = Profile.from_dict(str(name), dict(data))
        if "default" not in self._profiles:
            self._profiles["default"] = Profile()
        self._match: list[dict] = [dict(rule) for rule in scenario.get("match", [])]
        self._default_profile = str(scenario.get("default_profile", "default"))
        self.on_call = on_call

    @staticmethod
    def load(path: str | Path, on_call: Hook | None = None) -> "Dispatcher":
        with Path(path).open("r", encoding="utf-8") as handle:
            return Dispatcher(json.load(handle), on_call=on_call)

    def profiles(self) -> list[str]:
        return sorted(self._profiles)

    def profile_for(self, hello: dict) -> Profile:
        """Pick the profile a connecting process should get.

        Rules are tried in order and the first match wins; a rule can look at the
        executable name, the process id or an app id the caller already knows.
        """
        exe = str(hello.get("exe", "")).lower()
        pid = int(hello.get("pid", 0))
        for rule in self._match:
            if "exe_contains" in rule and str(rule["exe_contains"]).lower() not in exe:
                continue
            if "exe" in rule and str(rule["exe"]).lower() != exe:
                continue
            if "pid" in rule and int(rule["pid"]) != pid:
                continue
            profile_name = str(rule.get("profile", self._default_profile))
            if profile_name in self._profiles:
                return self._profiles[profile_name]
        return self._profiles.get(self._default_profile, Profile())

    def answer(self, session: Session, name: str, args: dict) -> tuple[bool, Any, dict | None, str]:
        """Answer one call: ``(answered, ret, out, source)``."""
        scripted = session.profile.scripted.get(name)
        if scripted is not None:
            if str(scripted.get("answer", "handled")) == "default":
                return (False, None, None, "scripted")
            return (True, scripted.get("ret"), scripted.get("out"), "scripted")

        if self.on_call is not None:
            answer = self.on_call(session, name, args)
            if answer is not None:
                ret, out = _unpack_hook_answer(answer)
                return (True, ret, out, "hook")

        answered, ret, out = session.handle(name, args)
        if answered:
            return (True, ret, out, "state")

        return (False, None, None, "none")


def _unpack_hook_answer(answer: Any) -> tuple[Any, dict | None]:
    """Accept either a bare return value or ``{"ret": ..., "out": {...}}``."""
    if isinstance(answer, dict) and ("ret" in answer or "out" in answer):
        return (answer.get("ret"), answer.get("out"))
    return (answer, None)
