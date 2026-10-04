#pragma once

#include <cstdint>
#include <string_view>

namespace steammock
{

// ---------------------------------------------------------------------------
//  The values the two ends have to agree on before either can talk.
// ---------------------------------------------------------------------------
//  The stub looks for the backend at kDefaultHost:kDefaultPort unless the
//  environment says otherwise, and the backend listens there unless its own
//  options say otherwise. Those were four spellings of the same literal - one per
//  front end - and the only thing keeping them in step was somebody noticing. The
//  port in particular is written into a comment in bridge/client.hpp and into
//  docs/development.md as prose, so a change here is a change there too.

inline constexpr const char* kDefaultHost = "127.0.0.1";
inline constexpr std::uint16_t kDefaultPort = 50990;

// A whole decimal number no larger than `ceiling`, or nothing: `out` is written
// only when the text is one. Nothing longer than the ceiling is ever accumulated,
// so the multiplication cannot wrap. This was copied into three places, which is
// how the timeout inherited the port's own ceiling.
inline bool parse_number(std::string_view text, unsigned ceiling, unsigned& out) noexcept
{
    if (text.empty() || ceiling == 0u)
    {
        return false;
    }
    unsigned value = 0;
    for (const char ch : text)
    {
        if (ch < '0' || ch > '9')
        {
            return false;
        }
        value = value * 10u + static_cast<unsigned>(ch - '0');
        if (value > ceiling)
        {
            return false;
        }
    }
    out = value;
    return true;
}

} // namespace steammock
