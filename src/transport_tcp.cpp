#include "bridge/transport.hpp"

#include <cstdio>

#include "bridge/frame.hpp"
#include "bridge/log.hpp"
#include "socket_io.hpp"

namespace steammock {
namespace {

using socket_io::as_socket;
using socket_io::close_socket;
using socket_io::ensure_winsock_started;
using socket_io::kInvalidSocket;
using socket_io::send_all;
using socket_io::socket_t;

constexpr std::uintptr_t kClosed = static_cast<std::uintptr_t>(~0ull);

bool is_open(std::uintptr_t value) noexcept { return value != kClosed; }

// Blocks in recv() and lets the socket's own SO_RCVTIMEO end the wait, which is the
// other half of why this is not the same function the server uses: there it is a
// slice and a flag, here it is the timeout the caller asked for. See src/socket_io.hpp.
bool recv_all(socket_t handle, char* data, std::size_t size) noexcept {
    std::size_t received = 0;
    while (received < size) {
        const std::size_t remaining = size - received;
        const int chunk = static_cast<int>(remaining > 0x7FFFFFFFu ? 0x7FFFFFFFu : remaining);
        const int got = ::recv(handle, data + received, chunk, 0);
        if (got <= 0) {
            return false;
        }
        received += static_cast<std::size_t>(got);
    }
    return true;
}

void apply_timeout(std::uintptr_t value, unsigned timeout_ms) noexcept {
    const socket_t handle = as_socket(value);
    const DWORD milliseconds = timeout_ms;
    // Asked for, not enforced: the caller has no way to tell a socket that took
    // them from one that refused, and a refused timeout is an interface that can
    // hang. Nothing here can throw, so it says so and carries on.
    if (setsockopt(handle, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&milliseconds),
                   sizeof(milliseconds)) != 0) {
        log_write(LogLevel::warn, "could not put a receive timeout on a socket");
    }
    if (setsockopt(handle, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&milliseconds),
                   sizeof(milliseconds)) != 0) {
        log_write(LogLevel::warn, "could not put a send timeout on a socket");
    }
}

std::uint32_t read_length(const char header[4]) noexcept { return read_frame_length(header); }

void write_length(char header[4], std::uint32_t length) noexcept {
    write_frame_length(header, length);
}

}  // namespace

TcpTransport::TcpTransport() noexcept : _socket(kClosed), _timeout_ms(2000u) {
    ensure_winsock_started();
}

TcpTransport::~TcpTransport() { close(); }

void TcpTransport::close() noexcept {
    if (is_open(_socket)) {
        close_socket(as_socket(_socket));
        _socket = kClosed;
    }
}

bool TcpTransport::is_connected() const noexcept { return is_open(_socket); }

bool TcpTransport::connect(std::string_view host, std::uint16_t port) {
    close();
    ensure_winsock_started();

    const std::string hostname(host);
    char service[8] = {};
    std::snprintf(service, sizeof(service), "%u", static_cast<unsigned>(port));

    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo* results = nullptr;
    if (getaddrinfo(hostname.c_str(), service, &hints, &results) != 0 || results == nullptr) {
        log_write(LogLevel::debug, "cannot resolve the backend address " + hostname);
        return false;
    }

    socket_t handle = kInvalidSocket;
    for (addrinfo* candidate = results; candidate != nullptr; candidate = candidate->ai_next) {
        handle = ::socket(candidate->ai_family, candidate->ai_socktype, candidate->ai_protocol);
        if (handle == kInvalidSocket) {
            continue;
        }
        const int enable = 1;
        (void)setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&enable),
                         sizeof(enable));
        // Before the connect, not after: `::connect` blocks, and the timeouts are
        // what say for how long. Setting them once the call has returned is a
        // timeout on a conversation that may never have started - a peer that
        // accepts nothing left the whole interface hanging on it.
        apply_timeout(static_cast<std::uintptr_t>(handle), _timeout_ms);
        if (::connect(handle, candidate->ai_addr, static_cast<int>(candidate->ai_addrlen)) == 0) {
            break;
        }
        close_socket(handle);
        handle = kInvalidSocket;
    }
    freeaddrinfo(results);

    if (handle == kInvalidSocket) {
        return false;
    }
    _socket = static_cast<std::uintptr_t>(handle);
    return true;
}

bool TcpTransport::exchange(const std::string& request, std::string& response) {
    if (!is_connected()) {
        return false;
    }
    const socket_t handle = as_socket(_socket);

    if (request.size() > kMaxFrameBytes) {
        log_write(LogLevel::error, "refusing to send an oversized frame");
        return false;
    }
    char header[4] = {};
    write_length(header, static_cast<std::uint32_t>(request.size()));
    if (!send_all(handle, header, sizeof(header)) ||
        (!request.empty() && !send_all(handle, request.data(), request.size()))) {
        return false;
    }

    char reply_header[4] = {};
    if (!recv_all(handle, reply_header, sizeof(reply_header))) {
        return false;
    }
    const std::uint32_t reply_length = read_length(reply_header);
    if (reply_length == 0u || reply_length > kMaxFrameBytes) {
        log_write(LogLevel::warn, "the backend sent an impossible frame length");
        return false;
    }
    response.assign(reply_length, '\0');
    return recv_all(handle, response.data(), reply_length);
}

}  // namespace steammock
