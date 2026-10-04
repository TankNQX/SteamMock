#include "bridge/transport.hpp"

#include <cstdio>

#include "bridge/frame.hpp"
#include "bridge/log.hpp"
#include "socket_io.hpp"

namespace steammock
{
namespace
{

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
bool recv_all(socket_t handle, char* data, std::size_t size) noexcept
{
    std::size_t received = 0;
    while (received < size)
    {
        const std::size_t remaining = size - received;
        const int chunk = static_cast<int>(remaining > 0x7FFFFFFFu ? 0x7FFFFFFFu : remaining);
        const int got = ::recv(handle, data + received, chunk, 0);
        if (got > 0)
        {
            received += static_cast<std::size_t>(got);
            continue;
        }
        if (got < 0 && WSAGetLastError() == WSAEINTR)
        {
            // Interrupted before a byte arrived - not a reason to throw away a
            // connection that is answering perfectly well. `got == 0` is the peer
            // closing, and every other error (a receive timeout included) is real.
            continue;
        }
        return false;
    }
    return true;
}

void apply_timeout(std::uintptr_t value, unsigned timeout_ms) noexcept
{
    const socket_t handle = as_socket(value);
    const DWORD milliseconds = timeout_ms;
    // Asked for, not enforced: the caller has no way to tell a socket that took
    // them from one that refused, and a refused timeout is an interface that can
    // hang. Nothing here can throw, so it says so and carries on.
    if (setsockopt(handle, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&milliseconds),
                   sizeof(milliseconds)) != 0)
    {
        log_write(LogLevel::warn, "could not put a receive timeout on a socket");
    }
    if (setsockopt(handle, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&milliseconds),
                   sizeof(milliseconds)) != 0)
    {
        log_write(LogLevel::warn, "could not put a send timeout on a socket");
    }
}

// `::connect` to a port nobody is listening on does not return until the TCP stack
// gives up on its SYN retransmissions, and on Windows SO_RCVTIMEO/SO_SNDTIMEO do not
// bound it at all - they are read and write timeouts, and a connect is neither. The
// socket's own timeouts used to be set here and trusted for it, so a game pointing at
// an unreachable host was parked for the stack's own ~21 seconds instead of the 2 the
// client asked for, and "never hangs forever" was a promise about the wrong call.
//
// So the connect is made on a non-blocking socket and waited for with the deadline
// the caller gave, and the socket is put back in blocking mode before it is returned:
// the timeouts above only mean anything on a blocking socket, and everything after
// this - the framing, the receive - relies on them. A deadline of zero is taken at its
// word - the attempt is made and not waited for - which is why the client reads a
// STEAMMOCK_TIMEOUT_MS of 0 as its default rather than as "no time at all".
bool connect_with_timeout(socket_t handle, const sockaddr* address, int address_length,
                          unsigned timeout_ms) noexcept
{
    u_long nonblocking = 1;
    if (ioctlsocket(handle, FIONBIO, &nonblocking) != 0)
    {
        // No way to make it non-blocking: the blocking call is still the right one to
        // make, and its own timeout is all this socket is going to get.
        return ::connect(handle, address, address_length) == 0;
    }

    bool connected = false;
    if (::connect(handle, address, address_length) == 0)
    {
        connected = true;
    }
    else if (WSAGetLastError() == WSAEWOULDBLOCK)
    {
        fd_set writable;
        FD_ZERO(&writable);
        FD_SET(handle, &writable);
        timeval wait{};
        wait.tv_sec = static_cast<long>(timeout_ms / 1000u);
        wait.tv_usec = static_cast<long>(timeout_ms % 1000u) * 1000L;
        const int ready = ::select(0, nullptr, &writable, nullptr, &wait);
        if (ready > 0)
        {
            // Writability says the attempt finished, not that it succeeded: the
            // failure a connect cannot report at the call is reported here.
            int error = 0;
            int error_size = static_cast<int>(sizeof(error));
            connected = getsockopt(handle, SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&error),
                                   &error_size) == 0 &&
                        error == 0;
        }
    }

    u_long blocking = 0;
    (void)ioctlsocket(handle, FIONBIO, &blocking);
    return connected;
}

std::uint32_t read_length(const char header[4]) noexcept { return read_frame_length(header); }

void write_length(char header[4], std::uint32_t length) noexcept
{
    write_frame_length(header, length);
}

} // namespace

TcpTransport::TcpTransport() noexcept : _socket(kClosed), _timeout_ms(2000u)
{
    (void)ensure_winsock_started();
}

TcpTransport::~TcpTransport() { close(); }

void TcpTransport::close_locked() noexcept
{
    if (is_open(_socket))
    {
        close_socket(as_socket(_socket));
        _socket = kClosed;
    }
}

void TcpTransport::close() noexcept
{
    try
    {
        const std::scoped_lock lock(_handle_mutex);
        close_locked();
    }
    catch (...) // NOLINT(bugprone-empty-catch) - a game must not see a mutex failure
    {
        // A mutex that cannot be taken is not a reason to end the process this DLL
        // is loaded into: the handle is left open, which the process exit collects.
    }
}

bool TcpTransport::is_connected() const noexcept
{
    try
    {
        const std::scoped_lock lock(_handle_mutex);
        return is_open(_socket);
    }
    catch (...)
    {
        return false;
    }
}

void TcpTransport::set_timeout_ms(unsigned timeout_ms) noexcept
{
    try
    {
        const std::scoped_lock lock(_handle_mutex);
        _timeout_ms = timeout_ms;
    }
    catch (...) // NOLINT(bugprone-empty-catch) - a game must not see a mutex failure
    {
        // Same decision as close(): a timeout that could not be recorded leaves the
        // previous one in force rather than terminating a game.
    }
}

bool TcpTransport::connect(std::string_view host, std::uint16_t port)
{
    const std::scoped_lock lock(_handle_mutex);
    close_locked();
    if (!ensure_winsock_started())
    {
        log_write(LogLevel::error, "cannot connect: the socket layer never started");
        return false;
    }

    const std::string hostname(host);
    char service[8] = {};
    std::snprintf(service, sizeof(service), "%u", static_cast<unsigned>(port));

    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo* results = nullptr;
    if (getaddrinfo(hostname.c_str(), service, &hints, &results) != 0 || results == nullptr)
    {
        log_write(LogLevel::debug, "cannot resolve the backend address " + hostname);
        return false;
    }

    socket_t handle = kInvalidSocket;
    for (addrinfo* candidate = results; candidate != nullptr; candidate = candidate->ai_next)
    {
        handle = ::socket(candidate->ai_family, candidate->ai_socktype, candidate->ai_protocol);
        if (handle == kInvalidSocket)
        {
            continue;
        }
        const int enable = 1;
        (void)setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&enable),
                         sizeof(enable));
        // Before the connect, not after: the receive and send timeouts are what say
        // how long the conversation may take, and the connect itself is bounded by
        // the same number rather than by the TCP stack - see connect_with_timeout.
        apply_timeout(static_cast<std::uintptr_t>(handle), _timeout_ms);
        if (connect_with_timeout(handle, candidate->ai_addr,
                                 static_cast<int>(candidate->ai_addrlen), _timeout_ms))
        {
            break;
        }
        // A connect that timed out leaves a socket still trying in the background:
        // closing it is what stops it.
        close_socket(handle);
        handle = kInvalidSocket;
    }
    freeaddrinfo(results);

    if (handle == kInvalidSocket)
    {
        return false;
    }
    _socket = static_cast<std::uintptr_t>(handle);
    return true;
}

bool TcpTransport::exchange(const std::string& request, std::string& response)
{
    // Held for the whole exchange, so the handle this is using cannot be closed and
    // reassigned under it by another thread - `close` waits here for as long as the
    // round trip takes, which is bounded by the timeout the socket was given.
    const std::scoped_lock lock(_handle_mutex);
    if (!is_open(_socket))
    {
        return false;
    }
    const socket_t handle = as_socket(_socket);

    if (request.size() > kMaxFrameBytes)
    {
        log_write(LogLevel::error, "refusing to send an oversized frame");
        return false;
    }
    char header[4] = {};
    write_length(header, static_cast<std::uint32_t>(request.size()));
    if (!send_all(handle, header, sizeof(header)) ||
        (!request.empty() && !send_all(handle, request.data(), request.size())))
    {
        // The connection is not usable any more, and saying so here is the point:
        // leaving it open meant the next call exchanged on a stream that was already
        // out of step, so one failure became every later call's failure.
        log_write(LogLevel::warn, "could not send a request to the backend; hanging up");
        close_locked();
        return false;
    }

    char reply_header[4] = {};
    if (!recv_all(handle, reply_header, sizeof(reply_header)))
    {
        close_locked();
        return false;
    }
    const std::uint32_t reply_length = read_length(reply_header);
    if (reply_length == 0u || reply_length > kMaxFrameBytes)
    {
        log_write(LogLevel::warn, "the backend sent an impossible frame length");
        close_locked();
        return false;
    }
    response.assign(reply_length, '\0');
    if (!recv_all(handle, response.data(), reply_length))
    {
        close_locked();
        return false;
    }
    return true;
}

} // namespace steammock
