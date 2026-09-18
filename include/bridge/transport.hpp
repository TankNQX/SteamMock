#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace steambridge {

// ---------------------------------------------------------------------------
//  How a call reaches the backend.
// ---------------------------------------------------------------------------
//  One connection per game process: send one framed request, block for the
//  answer, hand it back. A transport never throws and never hangs forever: on
//  any failure `exchange` returns false and the stub falls back to its defaults,
//  because a game stuck in a WinAPI call is worse than a game running without
//  the harness.
//
//  TCP loopback is the first implementation. Named pipes or shared memory drop
//  in behind this interface without the client noticing.
class Transport {
public:
    virtual ~Transport() = default;

    virtual bool connect(std::string_view host, std::uint16_t port) = 0;
    virtual void close() noexcept = 0;
    virtual bool is_connected() const noexcept = 0;

    // Frames the request (4-byte little-endian length + UTF-8 payload) and waits
    // for the reply frame. `response` is only meaningful on a true return.
    virtual bool exchange(const std::string& request, std::string& response) = 0;
};

class TcpTransport final : public Transport {
public:
    TcpTransport() noexcept;
    ~TcpTransport() override;

    TcpTransport(const TcpTransport&) = delete;
    TcpTransport& operator=(const TcpTransport&) = delete;

    bool connect(std::string_view host, std::uint16_t port) override;
    void close() noexcept override;
    bool is_connected() const noexcept override;
    bool exchange(const std::string& request, std::string& response) override;

    // Milliseconds to wait for a reply before giving up on the round trip, and
    // whether the transport may call connect() itself when it is not connected.
    void set_timeout_ms(unsigned timeout_ms) noexcept { _timeout_ms = timeout_ms; }
    unsigned timeout_ms() const noexcept { return _timeout_ms; }

private:
    // Held as an integer so this header stays free of winsock includes.
    std::uintptr_t _socket;
    unsigned _timeout_ms;
};

}  // namespace steambridge
