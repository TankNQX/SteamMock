#pragma once

#include <cstdint>

namespace steambridge {

// ---------------------------------------------------------------------------
//  The wire format: a 4 byte little-endian length, then the payload.
// ---------------------------------------------------------------------------
//  The stub and the backend both link this header, so the framing has one
//  definition rather than one per side, and the messages above it live in
//  bridge/protocol.hpp.

// A frame this large means someone is confused about the format; refusing it is
// better than allocating whatever a wrong length prefix asks for.
constexpr std::uint32_t kMaxFrameBytes = 4u * 1024u * 1024u;

inline void write_frame_length(char header[4], std::uint32_t length) noexcept {
    header[0] = static_cast<char>(length & 0xFFu);
    header[1] = static_cast<char>((length >> 8) & 0xFFu);
    header[2] = static_cast<char>((length >> 16) & 0xFFu);
    header[3] = static_cast<char>((length >> 24) & 0xFFu);
}

inline std::uint32_t read_frame_length(const char header[4]) noexcept {
    return static_cast<std::uint32_t>(static_cast<unsigned char>(header[0])) |
           (static_cast<std::uint32_t>(static_cast<unsigned char>(header[1])) << 8) |
           (static_cast<std::uint32_t>(static_cast<unsigned char>(header[2])) << 16) |
           (static_cast<std::uint32_t>(static_cast<unsigned char>(header[3])) << 24);
}

}  // namespace steambridge
