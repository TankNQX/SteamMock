#include "bridge/protocol.hpp"

namespace steammock {

bool carries_out(const Json& out) noexcept { return out.is_object() && !out.members().empty(); }

Json make_reply(std::int64_t seq, bool answered, const Json& ret, const Json& out) {
    Json message = Json::object();
    message.set("type", Json::string("reply"));
    message.set("v", Json::integer(kProtocolVersion));
    message.set("seq", Json::integer(seq));
    message.set("answer", Json::string(answered ? "handled" : "default"));
    if (answered) {
        // `ret` is always present on an answer, even when it is null: a scenario
        // that scripts a call without saying what it returns means "nothing",
        // not "no opinion", and the stub reads that as its type's zero.
        message.set("ret", ret);
        if (carries_out(out)) {
            message.set("out", out);
        }
    }
    return message;
}

Json make_welcome(const std::string& session_id, const std::string& profile_name) {
    Json message = Json::object();
    message.set("type", Json::string("welcome"));
    message.set("v", Json::integer(kProtocolVersion));
    message.set("session", Json::string(session_id));
    message.set("profile", Json::string(profile_name));
    return message;
}

}  // namespace steammock
