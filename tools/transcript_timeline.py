#!/usr/bin/env python3
# ---------------------------------------------------------------------------
#  transcript_timeline.py - read a recorded run back as a timeline.
# ---------------------------------------------------------------------------
#  The rig records every call a game makes and every call the backend answers, one
#  JSON object per line, in the order they were handled and with the wall clock on
#  them - so a run that went wrong can be read back afterwards. What this prints is the
#  part of that record a person actually reads: the packet handshake, the ticket
#  exchange, the lobby leaving, and - with the stub logs beside the transcript - which
#  callback object each payload was handed to.
#
#      python tools/transcript_timeline.py --rig %TEMP%\sw-two
#      python tools/transcript_timeline.py --transcript run.jsonl --summary
#      python tools/transcript_timeline.py --rig %TEMP%\sw-two --callbacks
#
#  Everything is the standard library, deliberately: tools/steamworks_sdk_import.py
#  needs a virtualenv for its parser, and reading a run should not.
#
#  Three traps are handled here rather than left to the reader, because each has cost
#  somebody a day already:
#
#   * An out-parameter in a record carries what the *caller* passed in, in `args` - the
#     game's own uninitialised or stale variable - and what the backend answered in
#     `out`. A `psteamIDRemote` read out of `args` is the game's previous read, not the
#     peer a packet came from. This reads `out` for every out-parameter.
#   * game-output.log is the machine's debug buffer, not this run's: it carries lines
#     from whatever else the machine was doing, and a line about another process reads
#     exactly like one of ours. Only lines whose pid is one of this run's are printed.
#   * A message id and a callback id are different namespaces that collide: 504 is
#     ClientLeavingServer in one and LobbyEnter_t in the other, 505 is ClientPing in one
#     and LobbyDataUpdate_t in the other. A packet is decoded with MESSAGES, a log line
#     is named with CALLBACKS, and neither table is used for the other.
# ---------------------------------------------------------------------------

import argparse
import json
import os
import sys
from collections import OrderedDict, namedtuple

# ---------------------------------------------------------------------------
#  The game's own protocol
# ---------------------------------------------------------------------------
#  Spacewar writes its message id as the first four bytes of every packet, little
#  endian, and the ids are its Messages.h (sdk/steamworksexample/Messages.h in the SDKs
#  this tree was imported from):
#
#      k_EMsgServerBegin = 0        k_EMsgClientBegin = 500
#      k_EMsgP2PBegin   = 600       k_EMsgVoiceChatBegin = 700
MESSAGES = {
    0: "ServerBegin",
    1: "ServerSendInfo",
    2: "ServerFailAuthentication",
    3: "ServerPassAuthentication",
    4: "ServerUpdateWorld",
    5: "ServerExiting",
    6: "ServerPingResponse",
    500: "ClientBegin",
    501: "ClientInitiateConnection",
    502: "ClientBeginAuthentication",
    503: "ClientSendLocalUpdate",
    504: "ClientLeavingServer",
    505: "ClientPing",
    600: "P2PBegin",
    601: "P2PSendingTicket",
    700: "VoiceChatBegin",
    701: "VoiceChatPing",
    702: "VoiceChatData",
}

# The same ids in one word, for a column that has to stay narrow.
WORDS = {1: "info", 2: "FAILAUTH", 3: "PassAuth", 4: "world", 5: "exiting", 6: "pingresp",
         501: "init", 502: "beginauth", 503: "update", 504: "leaving", 505: "ping",
         601: "ticket", 701: "voice", 702: "voicedata"}

# The ones that arrive every frame and would bury everything else.
NOISE = {4, 503, 505, 701, 702}

# The callback ids this harness's own payloads carry, as its logs print them. Names for
# the rest are learned from the logs when they are there, so an id nobody has seen
# before stays a number rather than becoming a wrong name.
CALLBACKS = {
    101: "SteamServersConnected_t",
    102: "SteamServersDisconnected_t",
    103: "SteamServerConnectFailure_t",
    143: "ValidateAuthTicketResponse_t",
    505: "LobbyDataUpdate_t",
    506: "LobbyChatUpdate_t",
    1202: "P2PSessionRequest_t",
    1203: "P2PSessionConnectFail_t",
}

# The calls worth a line each. Everything else is counted rather than printed: a game
# makes tens of thousands of calls a run, and the ones that shape a session are a few
# dozen of them. Registration and delivery are the stub's own story and are read out of
# its logs instead, where they carry the object they are about.
HANDSHAKE_CALLS = {
    "SteamAPI_ISteamNetworking_SendP2PPacket",
    "SteamAPI_ISteamNetworking_ReadP2PPacket",
    "SteamAPI_ISteamNetworking_AcceptP2PSessionWithUser",
    "SteamAPI_ISteamNetworking_CloseP2PSessionWithUser",
    "SteamAPI_ISteamNetworking_GetP2PSessionState",
    "SteamAPI_ISteamGameServer_BeginAuthSession",
    "SteamAPI_ISteamGameServer_EndAuthSession",
    "SteamAPI_ISteamGameServer_SendUserDisconnect",
    "SteamAPI_ISteamUser_BeginAuthSession",
    "SteamAPI_ISteamUser_EndAuthSession",
    "SteamAPI_ISteamUser_GetAuthSessionTicket",
    "SteamAPI_ISteamUser_CancelAuthTicket",
    "SteamAPI_ISteamMatchmaking_CreateLobby",
    "SteamAPI_ISteamMatchmaking_JoinLobby",
    "SteamAPI_ISteamMatchmaking_LeaveLobby",
    "SteamAPI_ISteamMatchmaking_SetLobbyGameServer",
    "SteamAPI_ISteamMatchmaking_GetLobbyGameServer",
    "SteamAPI_ISteamMatchmaking_SetLobbyMemberData",
    "SteamAPI_ISteamMatchmaking_SetLobbyData",
}

POLLS = "SteamAPI_ISteamNetworking_IsP2PPacketAvailable"

# One line of a log file: which file, which line of it, whose session it is (when the
# file or the pid says so), and the text.
LogLine = namedtuple("LogLine", "source number session text")


def payload_type(hex_text):
    """The game's message id at the front of a packet, or None if this is not one."""
    if not hex_text or not isinstance(hex_text, str) or len(hex_text) < 8:
        return None
    try:
        return int.from_bytes(bytes.fromhex(hex_text[:8]), "little")
    except ValueError:
        return None


def name_of(kind):
    if kind is None:
        return ""
    return WORDS.get(kind, MESSAGES.get(kind, str(kind)))


def load_transcript(path):
    """Every record the timeline is built from, in file order.

    A transcript of a three-pad run is a few hundred megabytes, so the cheap test on the
    raw line comes first and json only parses the lines that could matter.
    """
    wanted = ("P2PPacket", "AuthSession", "AuthTicket", "P2PSession", "Matchmaking")
    records = []
    with open(path, "r", encoding="utf-8", errors="replace") as handle:
        for line in handle:
            if not any(part in line for part in wanted):
                continue
            try:
                records.append(json.loads(line))
            except ValueError:
                continue
    return records


def sessions_of(records):
    """Which session did what, and how busy each was.

    The transcript carries no pid: the handshake is answered before the first call is
    recorded, so the hello - the one message that names the pid and the profile - is not
    in it. What it does carry is the part each session played, and that is what names
    them the way the rig makes them: the session that creates the lobby is the host, and
    the ones that join it follow in the order they joined.
    """
    sessions = OrderedDict()
    for record in records:
        session = record.get("session")
        if session is None:
            continue
        if session not in sessions:
            sessions[session] = {"id": session, "created": False, "joined": False, "calls": 0,
                                 "lobby": 0, "polls": 0, "reads": 0, "sends": 0, "slots": None}
        entry = sessions[session]
        entry["calls"] += 1
        call = record.get("call") or ""
        if call == "SteamAPI_ISteamMatchmaking_CreateLobby":
            entry["created"] = True
        elif call == "SteamAPI_ISteamMatchmaking_JoinLobby":
            entry["joined"] = True
        if record.get("via") == "lobby":
            entry["lobby"] += 1
        if call == POLLS:
            entry["polls"] += 1
        elif call.endswith("_ReadP2PPacket"):
            entry["reads"] += 1
        elif call.endswith("_SendP2PPacket"):
            entry["sends"] += 1
        elif call == "SteamAPI_ISteamMatchmaking_GetNumLobbyMembers" and \
                isinstance(record.get("ret"), int):
            entry["slots"] = record["ret"]
    return sessions


def host_of(sessions):
    for session, entry in sessions.items():
        if entry["created"]:
            return session
    return next(iter(sessions), None)


def assign_sessions(sessions, logs):
    """Which log file belongs to which session.

    The stub writes a log per process and names nothing in it, so the only thing that
    ties one to a session is the part it played - the rig starts the host first, with the
    default profile, and each guest as it joins. main() prints this mapping, because one
    that is wrong should be visible rather than quietly rearranging the story.
    """
    order = []
    host = host_of(sessions)
    if host is not None:
        order.append(host)
    for session, entry in sessions.items():
        if session != host and entry["joined"]:
            order.append(session)
    return {path: order[index] for index, path in enumerate(logs) if index < len(order)}


def timeline(records, sessions):
    """The interesting calls, in order, with the time each was handled."""
    host = host_of(sessions)
    lines = []
    for record in records:
        call = record.get("call") or ""
        if call not in HANDSHAKE_CALLS:
            continue
        args = record.get("args") or {}
        out = record.get("out") or {}
        session = record.get("session") or "?"
        kind = None
        detail = ""
        if call.endswith("_SendP2PPacket"):
            kind = payload_type(args.get("pubData"))
            detail = "to %s ch %s" % (args.get("steamIDRemote"), args.get("nChannel"))
            if kind is None and isinstance(args.get("pubData"), int):
                detail += "  (the flat call sends a pointer, not the bytes)"
        elif call.endswith("_ReadP2PPacket"):
            kind = payload_type(out.get("pubDest"))
            # The peer is in `out`: an out-parameter's `args` is what the caller passed
            # in, which for a read is the game's own stale variable.
            detail = "from %s, %s bytes" % (out.get("psteamIDRemote"), out.get("pcubMsgSize"))
        elif call.endswith("_BeginAuthSession"):
            detail = "about %s, ticket %s bytes, ret %s" % (
                args.get("steamID"), args.get("cbAuthTicket"), record.get("ret"))
        elif call.endswith("_GetAuthSessionTicket"):
            detail = "ret %s, %s bytes" % (record.get("ret"), out.get("pcbTicket"))
        elif call.endswith("_SetLobbyGameServer"):
            detail = "ip %s port %s" % (args.get("unGameServerIP"), args.get("unGameServerPort"))
        elif call.endswith("_GetNumLobbyMembers"):
            detail = "answered %s" % record.get("ret")
        elif call.endswith("_CreateLobby") or call.endswith("_JoinLobby"):
            detail = "call handle %s" % record.get("ret")
        if isinstance(kind, int) and kind in NOISE:
            continue
        name = call.replace("SteamAPI_ISteam", "").replace("SteamAPI_", "")
        lines.append((record.get("at_unix_ms"), "HOST" if session == host else "", session[:6],
                      name, name_of(kind), detail))
    return lines


def stub_log_lines(path, session):
    """One stub log: its lines, and the pid each was written under."""
    lines = []
    pids = set()
    with open(path, "r", encoding="utf-8", errors="replace") as handle:
        for number, line in enumerate(handle, start=1):
            parts = line.rstrip("\r\n").split(": ", 2)
            if len(parts) < 3 or not parts[0].startswith("[steammock]"):
                continue
            pid = None
            for token in parts[1].split():
                if token.isdigit():
                    pid = int(token)
                    break
            if pid is not None:
                pids.add(pid)
            lines.append(LogLine(os.path.basename(path), number, session, parts[2]))
    return lines, pids


def game_output_lines(path, pid_to_session):
    """The games' own words, and the stub lines that landed in the same buffer.

    This buffer belongs to the machine, not to the run: a line about some other process
    reads exactly like one of ours, so only a line whose pid is one of this run's is
    kept - which is also how a delivery line reaches the session it is about.
    """
    lines = []
    with open(path, "r", encoding="utf-8", errors="replace") as handle:
        for number, line in enumerate(handle, start=1):
            line = line.rstrip("\r\n")
            pid = None
            text = None
            if line.startswith("[steammock]"):
                parts = line.split(": ", 2)
                if len(parts) < 3:
                    continue
                for token in parts[1].split():
                    if token.isdigit():
                        pid = int(token)
                        break
                text = parts[2]
            else:
                first, _, rest = line.partition(" ")
                if first.isdigit():
                    pid = int(first)
                    text = rest
            session = pid_to_session.get(pid)
            if session is not None and text is not None:
                lines.append(LogLine(os.path.basename(path), number, session, text))
    return lines


def learn_callback_names(lines):
    """The names the harness's logs use, so an id nobody hard-coded still reads."""
    for line in lines:
        if not line.text.startswith("delivering "):
            continue
        head, _, tail = line.text.partition(" to ")
        name = head[len("delivering "):].split(" - ")[0].split(" for steam id ")[0]
        marker = "registered for id "
        at = tail.find(marker)
        if not name or at < 0:
            continue
        digits = ""
        for ch in tail[at + len(marker):]:
            if not ch.isdigit():
                break
            digits += ch
        if digits:
            CALLBACKS.setdefault(int(digits), name)


def callback_story(lines, callback_id):
    """The registrations and deliveries for one callback id, per session.

    Per session, because the registry is per process: a hosting process's game server and
    client share one, and a guest's is its own. What matters for the defect this reads for
    is whether a *delivery* arrived after a second object had joined that session's id.
    """
    story = OrderedDict()
    for line in lines:
        text = line.text
        kind = None
        if text.startswith("callback id %d:" % callback_id):
            kind = "registered"
        elif text.startswith("delivering ") and "registered for id %d" % callback_id in text:
            kind = "delivered"
        elif text.startswith("an event nobody is waiting for") and \
                "callback %d" % callback_id in text:
            kind = "dropped"
        if kind is None:
            continue
        entry = story.setdefault(line.session,
                                 {"registered": 0, "shadowed": 0, "lines": []})
        if kind == "registered":
            entry["registered"] += 1
        elif kind == "delivered" and entry["registered"] > 1:
            # The registry hands this to the object that registered last.
            entry["shadowed"] += 1
        entry["lines"].append((line, kind, text.split(": ", 1)[-1].strip()
                               if kind == "registered" else text))
    return story


def match_outcome(records, host):
    """Who the host's game server let in, read out of the ticket exchange itself.

    The game server asks about a ticket per player it was given one for, and it passes
    the ones Steam says are good - so the two lists, and the ids in the first that never
    appear in the second, are the players this run did and did not let in. That is the
    outcome; the callback registry is only how it comes about.
    """
    asked = []
    passed = []
    for record in records:
        if record.get("session") != host:
            continue
        call = record.get("call") or ""
        args = record.get("args") or {}
        if call == "SteamAPI_ISteamGameServer_BeginAuthSession":
            asked.append(args.get("steamID"))
        elif call == "SteamAPI_ISteamNetworking_SendP2PPacket" and \
                payload_type(args.get("pubData")) == 3:
            passed.append(args.get("steamIDRemote"))
    return asked, passed


def callback_verdict(lines, story, records, host):
    """The outcome first, because it is what a run is judged by, and then the mechanism:
    what the game itself said, and how many payloads went to the wrong object."""
    asked, passed = match_outcome(records, host)
    print()
    print("  the host's game server asked to validate %d ticket(s)" % len(asked))
    print("  and sent k_EMsgServerPassAuthentication to %d of them:" % len(passed))
    for steam_id in asked:
        mark = "passed" if steam_id in passed else "NEVER PASSED"
        account = "account %d" % (steam_id & 0xFFFFFFFF) if isinstance(steam_id, int) else "?"
        print("    %-22s %-14s %s" % (steam_id, account, mark))
    left_out = [steam_id for steam_id in asked if steam_id not in passed]
    if not left_out:
        print("  every player it was handed a ticket for was let in.")
    else:
        for steam_id in left_out:
            print("  %s was never let in: it waits out the game's own 30-second ticket"
                  % steam_id)
            print("  timeout and then leaves the match it had already joined.")

    completed = 0
    said = []
    for line in lines:
        if line.text.startswith("Auth completed for a client"):
            completed += 1
        elif line.text.startswith("P2P::") and any(
                word in line.text for word in ("Ticket", "response", "Nothing received")):
            said.append((line.session, line.text))
    print()
    print("  the game itself said \"Auth completed for a client\" %d time(s)" % completed)
    for session, text in said[:8]:
        print("  %s said: %s" % (session[:6], text))

    host_story = story.get(host)
    if host_story is not None and host_story["shadowed"]:
        print()
        print("  %d payload(s) reached the host's id after a second object had registered,"
              % host_story["shadowed"])
        print("  and the registry hands those to the later object. The ones about a player")
        if left_out:
            print("  the game server asked about are the players above that were never passed.")
        else:
            print("  the game server asked about reached it anyway - so the ones handed over")
            print("  late were for the players this process asked about as a client.")


def main(argv):
    parser = argparse.ArgumentParser(
        prog="transcript_timeline",
        description=("Read a recorded run back as a timeline: the packet handshake, the "
                     "ticket exchange, the lobby leaving, and - with the stub logs beside "
                     "the transcript - which callback object each payload reached."),
    )
    parser.add_argument("--rig", metavar="DIR",
                        help="a rig directory: transcript.jsonl and a.log/b.log/c.log in it")
    parser.add_argument("--transcript", metavar="FILE", help="a transcript, instead of --rig")
    parser.add_argument("--logs", metavar="FILE", nargs="*", default=None,
                        help="stub logs, host first (default: a.log b.log c.log in the rig)")
    parser.add_argument("--game-output", metavar="FILE",
                        help="the games' own output (default: game-output.log in the rig)")
    parser.add_argument("--callbacks", action="store_true",
                        help="also print the registration/delivery story for one id")
    parser.add_argument("--callback-id", type=int, default=143,
                        help="the id --callbacks follows (default 143, "
                             "ValidateAuthTicketResponse_t)")
    parser.add_argument("--summary", action="store_true",
                        help="only the per-session counts, no timeline")
    args = parser.parse_args(argv)

    if args.rig:
        rig = os.path.abspath(args.rig)
        transcript = args.transcript or os.path.join(rig, "transcript.jsonl")
        logs = args.logs if args.logs is not None else \
            [os.path.join(rig, name) for name in ("a.log", "b.log", "c.log")]
        game_output = args.game_output or os.path.join(rig, "game-output.log")
    else:
        transcript = args.transcript
        logs = args.logs if args.logs is not None else []
        game_output = args.game_output
    if not transcript:
        sys.stderr.write("transcript_timeline: --rig or --transcript is needed\n")
        return 2
    if not os.path.exists(transcript):
        sys.stderr.write("transcript_timeline: no transcript at %s\n" % transcript)
        return 2

    records = load_transcript(transcript)
    if not records:
        sys.stderr.write("transcript_timeline: %s has nothing this reads\n" % transcript)
        return 2
    sessions = sessions_of(records)
    first = min(record["at_unix_ms"] for record in records)
    last = max(record["at_unix_ms"] for record in records)

    print("run: %s" % transcript)
    print("%d session(s), %.1f s of calls, %d record(s) read" %
          (len(sessions), (last - first) / 1000.0, len(records)))
    print()
    for session, entry in sessions.items():
        print("session %s  %-4s  %7d calls  %6d lobby  %7d polls  %5d read  %5d sent  "
              "lobby size %s" %
              (session[:6], "HOST" if session == host_of(sessions) else "",
               entry["calls"], entry["lobby"], entry["polls"], entry["reads"], entry["sends"],
               entry["slots"]))

    lines = []
    pid_to_session = {}
    assigned = assign_sessions(sessions, [path for path in logs if os.path.exists(path)])
    if assigned:
        print()
        for path, session in assigned.items():
            print("log %s -> session %s" % (os.path.basename(path), session[:6]))
        for path, session in assigned.items():
            found, pids = stub_log_lines(path, session)
            lines += found
            for pid in pids:
                pid_to_session[pid] = session
    if game_output and os.path.exists(game_output):
        lines += game_output_lines(game_output, pid_to_session)
    learn_callback_names(lines)

    if not args.summary:
        print()
        print("--- the timeline ---")
        print("%9s %-5s %-7s %-19s %-11s %s" %
              ("t+ms", "who", "session", "call", "message", "detail"))
        for at, who, session, call, message, detail in timeline(records, sessions):
            print("%9d %-5s %-7s %-19s %-11s %s" % (at - first, who, session, call, message, detail))

    if args.callbacks:
        print()
        print("--- callback id %d (%s) ---" %
              (args.callback_id, CALLBACKS.get(args.callback_id, "?")))
        story = callback_story(lines, args.callback_id)
        host = host_of(sessions)
        for session, entry in story.items():
            print("  session %s%s: %d object(s) registered" %
                  (session[:6], " (the host)" if session == host else "", entry["registered"]))
            for line, kind, text in entry["lines"]:
                print("    %-8s %5d  %-9s %s" % (line.source, line.number, kind, text))
        if not story:
            print("  nothing for this id in the logs that were read.")
        callback_verdict(lines, story, records, host)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
