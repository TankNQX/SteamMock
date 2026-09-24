#include "bridge/protocol.hpp"

namespace steammock {

bool carries_out(const Json& out) noexcept { return out.is_object() && !out.empty(); }

Json make_reply(std::int64_t seq, bool answered, const Json& ret, const Json& out) {
    Json message = Json::object();
    message["type"] = Json("reply");
    message["v"] = Json(kProtocolVersion);
    message["seq"] = Json(seq);
    message["answer"] = Json(answered ? "handled" : "default");
    if (answered) {
        // `ret` is always present on an answer, even when it is null: a scenario
        // that scripts a call without saying what it returns means "nothing",
        // not "no opinion", and the stub reads that as its type's zero.
        message["ret"] = ret;
        if (carries_out(out)) {
            message["out"] = out;
        }
    }
    return message;
}

Json make_welcome(const std::string& session_id, const std::string& profile_name) {
    Json message = Json::object();
    message["type"] = Json("welcome");
    message["v"] = Json(kProtocolVersion);
    message["session"] = Json(session_id);
    message["profile"] = Json(profile_name);
    return message;
}

}  // namespace steammock
