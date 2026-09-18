#include "bridge/transport.hpp"

#include <cstdio>
#include <cstring>

#include "bridge/frame.hpp"
#include "bridge/log.hpp"

#if defined(_WIN32)
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <winsock2.h>
#  include <ws2tcpip.h>
#else
#  include <netdb.h>
#  include <netinet/in.h>
#  include <netinet/tcp.h>
#  include <sys/socket.h>
#  include <sys/time.h>
#  include <unistd.h>
#endif

namespace steambridge {
namespace {

#if defined(_WIN32)
using socket_t = SOCKET;
constexpr socket_t kInvalidSocket = INVALID_SOCKET;
#else
using socket_t = int;
constexpr socket_t kInvalidSocket = -1;
#endif

constexpr std::uintptr_t kClosed = static_cast<std::uintptr_t>(~0ull);

void ensure_winsock_started() noexcept {
#if defined(_WIN32)
    static bool started = false;
    if (!started) {
        WSADATA data{};
        (void)WSAStartup(MAKEWORD(2, 2), &data);
        started = true;
    }
#endif
}

socket_t as_socket(std::uintptr_t value) noexcept {
    return static_cast<socket_t>(value);
}

bool is_open(std::uintptr_t value) noexcept {
    return value != kClosed;
}

void close_socket(std::uintptr_t value) noexcept {
    if (!is_open(value)) {
        return;
    }
    const socket_t handle = as_socket(value);
#if defined(_WIN32)
    (void)closesocket(handle);
#else
    (void)::close(handle);
#endif
}

// A partially written or partially read frame would desync the stream for every
// later call on this connection, so both directions loop until they are done.
bool send_all(socket_t handle, const char* data, std::size_t size) noexcept {
    std::size_t sent = 0;
    while (sent < size) {
        const std::size_t remaining = size - sent;
        const int chunk = static_cast<int>(remaining > 0x7FFFFFFFu ? 0x7FFFFFFFu : remaining);
        const int written = ::send(handle, data + sent, chunk, 0);
        if (written <= 0) {
            return false;
        }
        sent += static_cast<std::size_t>(written);
    }
    return true;
}

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
#if defined(_WIN32)
    const DWORD milliseconds = timeout_ms;
    (void)setsockopt(handle, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&milliseconds),
                     sizeof(milliseconds));
    (void)setsockopt(handle, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&milliseconds),
                     sizeof(milliseconds));
#else
    timeval timeout{};
    timeout.tv_sec = static_cast<long>(timeout_ms / 1000u);
    timeout.tv_usec = static_cast<long>((timeout_ms % 1000u) * 1000u);
    (void)setsockopt(handle, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    (void)setsockopt(handle, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
#endif
}

std::uint32_t read_length(const char header[4]) noexcept {
    return read_frame_length(header);
}

void write_length(char header[4], std::uint32_t length) noexcept {
    write_frame_length(header, length);
}

}  // namespace

TcpTransport::TcpTransport() noexcept : _socket(kClosed), _timeout_ms(2000u) {
    ensure_winsock_started();
}

TcpTransport::~TcpTransport() {
    close();
}

void TcpTransport::close() noexcept {
    close_socket(_socket);
    _socket = kClosed;
}

bool TcpTransport::is_connected() const noexcept {
    return is_open(_socket);
}

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
        if (::connect(handle, candidate->ai_addr, static_cast<int>(candidate->ai_addrlen)) == 0) {
            break;
        }
        close_socket(static_cast<std::uintptr_t>(handle));
        handle = kInvalidSocket;
    }
    freeaddrinfo(results);

    if (handle == kInvalidSocket) {
        return false;
    }
    _socket = static_cast<std::uintptr_t>(handle);
    apply_timeout(_socket, _timeout_ms);
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

}  // namespace steambridge
