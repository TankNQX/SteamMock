#pragma once

// ============================================================================
//  tests/child_process.hpp - start a program and read what it says.
// ----------------------------------------------------------------------------
//  The end-to-end test drives the real tools as real processes: the backend as a
//  person would run it, and the fake game as a game would be run. That needs a
//  bit of Win32, which is the only platform that matters here - the stub is a DLL
//  and the test loads it in a separate process, exactly like a game.
// ============================================================================

#include <windows.h>

#include <atomic>
#include <chrono>
#include <mutex>
#include <string>
#include <thread>

namespace steambridge_test {

// Quotes one argument for a Win32 command line. Without this a path under
// "C:\Program Files" splits into two arguments and the child never starts.
inline std::string quote(const std::string& argument) {
    std::string out = "\"";
    for (const char ch : argument) {
        if (ch == '"') {
            out += "\\\"";
        } else {
            out += ch;
        }
    }
    out += '"';
    return out;
}

class ChildProcess {
public:
    ChildProcess() = default;

    ~ChildProcess() {
        terminate();
        if (_reader.joinable()) {
            _reader.join();
        }
        if (_read != nullptr) {
            CloseHandle(_read);
        }
        if (_process != nullptr) {
            CloseHandle(_process);
        }
    }

    ChildProcess(const ChildProcess&) = delete;
    ChildProcess& operator=(const ChildProcess&) = delete;

    // Runs `command_line` with stdout and stderr captured. The child inherits
    // this process's environment, which is how the test hands over a port.
    bool start(const std::string& command_line, std::string& error) {
        SECURITY_ATTRIBUTES attributes{};
        attributes.nLength = sizeof(attributes);
        attributes.bInheritHandle = TRUE;

        HANDLE write_end = nullptr;
        if (!CreatePipe(&_read, &write_end, &attributes, 0)) {
            error = "cannot create a pipe";
            return false;
        }
        // The child must inherit the writing end only, or the pipe never closes
        // when it exits and the reader waits forever.
        SetHandleInformation(_read, HANDLE_FLAG_INHERIT, 0);

        STARTUPINFOA startup{};
        startup.cb = sizeof(startup);
        startup.dwFlags = STARTF_USESTDHANDLES;
        startup.hStdOutput = write_end;
        startup.hStdError = write_end;
        startup.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

        std::string mutable_command = command_line;
        PROCESS_INFORMATION process{};
        const BOOL started = CreateProcessA(nullptr, mutable_command.data(), nullptr, nullptr, TRUE,
                                            CREATE_NO_WINDOW, nullptr, nullptr, &startup, &process);
        CloseHandle(write_end);
        if (!started) {
            error = "cannot start " + command_line;
            return false;
        }

        _process = process.hProcess;
        CloseHandle(process.hThread);
        _reader = std::thread([this] { pump(); });
        return true;
    }

    // Everything the child has said so far.
    std::string output() const {
        std::lock_guard<std::mutex> lock(_mutex);
        return _text;
    }

    // True once some part of the output contains `needle`.
    bool wait_for(const std::string& needle, double timeout_seconds) const {
        const auto deadline =
            std::chrono::steady_clock::now() +
            std::chrono::milliseconds(static_cast<long long>(timeout_seconds * 1000.0));
        for (;;) {
            {
                std::lock_guard<std::mutex> lock(_mutex);
                if (_text.find(needle) != std::string::npos) {
                    return true;
                }
            }
            if (std::chrono::steady_clock::now() >= deadline) {
                return false;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }

    // The exit code, or -1 if the child is still running when the time is up.
    int wait_for_exit(double timeout_seconds) {
        if (_process == nullptr) {
            return -1;
        }
        const DWORD result =
            WaitForSingleObject(_process, static_cast<DWORD>(timeout_seconds * 1000.0));
        if (result != WAIT_OBJECT_0) {
            return -1;
        }
        DWORD code = 0;
        if (!GetExitCodeProcess(_process, &code)) {
            return -1;
        }
        _exited = true;
        return static_cast<int>(code);
    }

    bool running() const {
        if (_process == nullptr || _exited) {
            return false;
        }
        DWORD code = 0;
        return GetExitCodeProcess(_process, &code) && code == STILL_ACTIVE;
    }

    void terminate() {
        if (_process != nullptr && running()) {
            TerminateProcess(_process, 1);
        }
    }

private:
    void pump() {
        char buffer[4096];
        DWORD got = 0;
        while (ReadFile(_read, buffer, sizeof(buffer), &got, nullptr) && got > 0) {
            std::lock_guard<std::mutex> lock(_mutex);
            _text.append(buffer, got);
        }
    }

    HANDLE _read = nullptr;
    HANDLE _process = nullptr;
    std::thread _reader;
    mutable std::mutex _mutex;
    std::string _text;
    bool _exited = false;
};

}  // namespace steambridge_test
