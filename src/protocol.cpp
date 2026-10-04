#include "bridge/protocol.hpp"

#include "bridge/log.hpp"

namespace steammock
{

bool carries_out(const Json& out) noexcept
{
    if (out.is_object())
    {
        return !out.empty();
    }
    // Null and an empty object are the documented "no out-parameters". Anything else - a
    // string, a number, an array - is not something either end writes, so it is an upstream
    // mistake, and dropping it without a word is what makes one hard to find. Said out loud
    // instead. The reply and the transcript each ask this question, so a broken `out` says
    // so once on each of those paths rather than once per call.
    if (!out.is_null())
    {
        log_write(LogLevel::warn, "an answer's 'out' is not an object; it is not carried");
    }
    return false;
}

Json make_reply(std::int64_t seq, bool answered, const Json& ret, const Json& out)
{
    Json message = Json::object();
    message["type"] = Json("reply");
    message["v"] = Json(kProtocolVersion);
    message["seq"] = Json(seq);
    message["answer"] = Json(answered ? "handled" : "default");
    if (answered)
    {
        // `ret` is always present on an answer, even when it is null: a scenario
        // that scripts a call without saying what it returns means "nothing",
        // not "no opinion", and the stub reads that as its type's zero.
        message["ret"] = ret;
        if (carries_out(out))
        {
            message["out"] = out;
        }
    }
    return message;
}

Json make_welcome(const std::string& session_id, const std::string& profile_name)
{
    Json message = Json::object();
    message["type"] = Json("welcome");
    message["v"] = Json(kProtocolVersion);
    message["session"] = Json(session_id);
    message["profile"] = Json(profile_name);
    return message;
}

} // namespace steammock
