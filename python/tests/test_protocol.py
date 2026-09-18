"""Unit tests for framing, the protocol helpers and the session state machine.

Run from anywhere::

    python -m unittest discover -s python/tests -p "test_protocol.py"
"""

from __future__ import annotations

import json
import struct
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

from steambridge.handlers import Dispatcher  # noqa: E402
from steambridge.protocol import (  # noqa: E402
    MAX_FRAME_BYTES,
    PROTOCOL_VERSION,
    ProtocolError,
    decode_frames,
    encode,
    reply,
)
from steambridge.session import Profile, Session  # noqa: E402


class FramingTests(unittest.TestCase):
    def test_encode_is_a_little_endian_length_then_payload(self):
        frame = encode({"type": "bye"})
        (length,) = struct.unpack("<I", frame[:4])
        self.assertEqual(length, len(frame) - 4)
        self.assertEqual(json.loads(frame[4:].decode("utf-8")), {"type": "bye"})

    def test_the_cap_matches_the_cpp_side(self):
        self.assertEqual(MAX_FRAME_BYTES, 4 * 1024 * 1024)

    def test_two_messages_in_one_buffer(self):
        buffer = encode({"n": 1}) + encode({"n": 2})
        messages, leftover = decode_frames(buffer)
        self.assertEqual([m["n"] for m in messages], [1, 2])
        self.assertEqual(leftover, b"")

    def test_a_partial_frame_is_left_in_the_buffer(self):
        complete = encode({"n": 1})
        partial = encode({"n": 2})[:-3]
        messages, leftover = decode_frames(complete + partial)
        self.assertEqual([m["n"] for m in messages], [1])
        self.assertEqual(leftover, partial)

    def test_a_wrong_length_prefix_is_rejected(self):
        with self.assertRaises(ProtocolError):
            decode_frames(struct.pack("<I", MAX_FRAME_BYTES + 1))
        with self.assertRaises(ProtocolError):
            decode_frames(struct.pack("<I", 0))

    def test_an_unparsable_payload_is_rejected(self):
        bad = struct.pack("<I", 3) + b"{{{"
        with self.assertRaises(ProtocolError):
            decode_frames(bad)

    def test_an_oversized_message_is_refused_outright(self):
        with self.assertRaises(ProtocolError):
            encode({"blob": "x" * (MAX_FRAME_BYTES + 1)})

    def test_a_steam_id_stays_exact(self):
        steam_id = 76561198000000001
        frame = encode({"steam_id": steam_id})
        messages, _ = decode_frames(frame)
        self.assertEqual(messages[0]["steam_id"], steam_id)
        # Not 76561198000000000.0 - the point of keeping integers away from floats.
        self.assertIsInstance(messages[0]["steam_id"], int)


class ReplyTests(unittest.TestCase):
    def test_an_unanswered_reply_has_no_return_value(self):
        message = reply(7, False)
        self.assertEqual(message["answer"], "default")
        self.assertNotIn("ret", message)
        self.assertEqual(message["seq"], 7)
        self.assertEqual(message["v"], PROTOCOL_VERSION)

    def test_an_answered_reply_carries_the_value_and_out_parameters(self):
        message = reply(8, True, ret=True, out={"pnData": 42})
        self.assertEqual(message["answer"], "handled")
        self.assertTrue(message["ret"])
        self.assertEqual(message["out"], {"pnData": 42})

    def test_an_answered_reply_without_out_parameters_omits_out(self):
        self.assertNotIn("out", reply(9, True, ret=3))


def make_session(scripted: dict | None = None) -> Session:
    profile = Profile.from_dict(
        "test",
        {
            "app_id": 480,
            "steam_id": 76561198000000001,
            "persona_name": "Tester",
            "language": "english",
            "stats": {"Deaths": 3},
            "achievements": [{"name": "ACH_BOOTED", "achieved": False}],
            "scripted": scripted or {},
        },
    )
    return Session("session0", {"exe": "game.exe", "pid": 1234}, profile)


class SessionTests(unittest.TestCase):
    def test_identity_comes_from_the_profile(self):
        session = make_session()
        self.assertEqual(session.handle("SteamAPI_ISteamUser_GetSteamID", {})[1], 76561198000000001)
        self.assertEqual(session.handle("SteamAPI_ISteamFriends_GetPersonaName", {})[1], "Tester")
        self.assertEqual(session.handle("SteamAPI_ISteamUtils_GetAppID", {})[1], 480)
        self.assertEqual(session.handle("SteamAPI_ISteamUtils_GetCurrentGameLanguage", {})[1], "english")

    def test_a_call_nobody_knows_has_no_opinion(self):
        answered, ret, out = make_session().handle("SteamAPI_Init", {})
        self.assertFalse(answered)
        self.assertIsNone(ret)
        self.assertIsNone(out)

    def test_a_stat_is_read_as_named_out_parameters(self):
        session = make_session()
        answered, ret, out = session.handle("SteamAPI_ISteamUserStats_GetStatInt32",
                                           {"pchName": "Deaths"})
        self.assertTrue(answered)
        self.assertTrue(ret)
        self.assertEqual(out, {"pnData": 3})

    def test_writing_a_stat_then_reading_it_sees_the_new_value(self):
        session = make_session()
        session.handle("SteamAPI_ISteamUserStats_SetStatInt32", {"pchName": "Deaths", "nData": 9})
        _, _, out = session.handle("SteamAPI_ISteamUserStats_GetStatInt32", {"pchName": "Deaths"})
        self.assertEqual(out, {"pnData": 9})

    def test_an_unknown_stat_fails_like_steam_and_touches_nothing(self):
        answered, ret, out = make_session().handle("SteamAPI_ISteamUserStats_GetStatInt32",
                                                  {"pchName": "NoSuchStat"})
        self.assertTrue(answered)
        self.assertFalse(ret)
        self.assertIsNone(out)

    def test_an_achievement_can_be_read_and_unlocked(self):
        session = make_session()
        _, ret, out = session.handle("SteamAPI_ISteamUserStats_GetAchievement",
                                    {"pchName": "ACH_BOOTED"})
        self.assertTrue(ret)
        self.assertEqual(out, {"pbAchieved": False})

        session.handle("SteamAPI_ISteamUserStats_SetAchievement", {"pchName": "ACH_BOOTED"})
        answer = session.handle("SteamAPI_ISteamUserStats_GetAchievement", {"pchName": "ACH_BOOTED"})
        self.assertEqual(answer[2], {"pbAchieved": True})
        self.assertEqual(session.achievements_set, ["ACH_BOOTED"])

    def test_an_unknown_achievement_reports_failure(self):
        answered, ret, _ = make_session().handle("SteamAPI_ISteamUserStats_GetAchievement",
                                                 {"pchName": "ACH_MISSING"})
        self.assertTrue(answered)
        self.assertFalse(ret)

    def test_achievement_names_are_indexed(self):
        session = make_session()
        self.assertEqual(session.handle("SteamAPI_ISteamUserStats_GetNumAchievements", {})[1], 1)
        self.assertEqual(session.handle("SteamAPI_ISteamUserStats_GetAchievementName",
                                       {"iAchievement": 0})[1], "ACH_BOOTED")
        self.assertEqual(session.handle("SteamAPI_ISteamUserStats_GetAchievementName",
                                       {"iAchievement": 9})[1], "")

    def test_calls_are_recorded_for_the_transcript(self):
        session = make_session()
        session.handle("SteamAPI_ISteamUtils_GetAppID", {})
        self.assertTrue(session.id and session.exe == "game.exe" and "test" in session.describe())


class DispatcherTests(unittest.TestCase):
    def test_a_scripted_call_wins_over_the_state_machine(self):
        session = make_session(scripted={"SteamAPI_ISteamUtils_GetAppID": {"ret": 999}})
        dispatcher = Dispatcher({})
        answered, ret, _, source = dispatcher.answer(session, "SteamAPI_ISteamUtils_GetAppID", {})
        self.assertTrue(answered)
        self.assertEqual(ret, 999)
        self.assertEqual(source, "scripted")

    def test_a_scripted_call_can_decline_to_answer(self):
        session = make_session(scripted={"SteamAPI_Init": {"answer": "default"}})
        answered, _, _, source = Dispatcher({}).answer(session, "SteamAPI_Init", {})
        self.assertFalse(answered)
        self.assertEqual(source, "scripted")

    def test_state_answers_are_labelled(self):
        answered, ret, _, source = Dispatcher({}).answer(make_session(), "SteamAPI_GetHSteamUser", {})
        self.assertTrue(answered)
        self.assertEqual(ret, 1)
        self.assertEqual(source, "state")

    def test_an_unanswered_call_is_labelled_none(self):
        answered, _, _, source = Dispatcher({}).answer(make_session(), "SteamAPI_Shutdown", {})
        self.assertFalse(answered)
        self.assertEqual(source, "none")

    def test_a_hook_can_answer_anything(self):
        def hook(session, name, args):
            del session, args
            if name == "SteamAPI_Init":
                return True
            return None

        dispatcher = Dispatcher({}, on_call=hook)
        answered, ret, _, source = dispatcher.answer(make_session(), "SteamAPI_Init", {})
        self.assertTrue(answered)
        self.assertTrue(ret)
        self.assertEqual(source, "hook")

    def test_a_hook_can_return_out_parameters(self):
        def hook(session, name, args):
            del session, name, args
            return {"ret": True, "out": {"pnData": 7}}

        dispatcher = Dispatcher({}, on_call=hook)
        answered, ret, out, source = dispatcher.answer(make_session(), "Something", {})
        self.assertTrue(answered)
        self.assertTrue(ret)
        self.assertEqual(out, {"pnData": 7})
        self.assertEqual(source, "hook")

    def test_match_rules_pick_a_profile_by_executable(self):
        dispatcher = Dispatcher({
            "profiles": {
                "default": {"app_id": 1},
                "other": {"app_id": 2},
            },
            "match": [
                {"exe_contains": "special", "profile": "other"},
                {"exe_contains": "game", "profile": "default"},
            ],
            "default_profile": "default",
        })
        self.assertEqual(dispatcher.profile_for({"exe": "my_special_game.exe"}).app_id, 2)
        self.assertEqual(dispatcher.profile_for({"exe": "game.exe"}).app_id, 1)
        self.assertEqual(dispatcher.profile_for({"exe": "unknown.exe"}).app_id, 1)

    def test_an_unknown_profile_name_falls_back_to_the_default(self):
        dispatcher = Dispatcher({
            "profiles": {"default": {"app_id": 1}},
            "match": [{"exe_contains": "game", "profile": "typo"}],
        })
        self.assertEqual(dispatcher.profile_for({"exe": "game.exe"}).app_id, 1)

    def test_the_bundled_example_scenario_loads(self):
        scenario = Path(__file__).resolve().parent.parent / "steambridge" / "scenarios" / "example.json"
        dispatcher = Dispatcher.load(scenario)
        self.assertIn("default", dispatcher.profiles())
        profile = dispatcher.profile_for({"exe": "fake_game.exe"})
        self.assertEqual(profile.app_id, 480)
        self.assertTrue(profile.scripted["SteamAPI_Init"]["ret"])
        self.assertEqual(profile.stats["Deaths"], 0)

    def test_the_generated_surface_matches_the_idl(self):
        surface = json.loads(
            (Path(__file__).resolve().parent.parent / "steambridge" / "surface.json").read_text("utf-8")
        )
        names = [call["name"] for call in surface["calls"]]
        self.assertIn("SteamAPI_Init", names)
        self.assertIn("SteamAPI_ISteamUserStats_GetStatInt32", names)
        # Every call the state machine answers has to be in the surface, or the
        # stub and the backend have drifted apart.
        from steambridge.session import STATE_HANDLED_CALLS

        self.assertTrue(STATE_HANDLED_CALLS.issubset(set(names)),
                        sorted(STATE_HANDLED_CALLS - set(names)))


if __name__ == "__main__":
    unittest.main()
