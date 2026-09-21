// ---------------------------------------------------------------------------
//  steambridge_gui - the live view.
// ---------------------------------------------------------------------------
//  The backend with a window on it: which games are attached, what they are
//  asking, what they were told, and - next - what they should be told instead.
//
//  It drives the same Server the console does, and only ever reads snapshots.
//  The server answers calls on its own threads; this thread draws copies of
//  what it has seen, so nothing here can stall a game and nothing here decides
//  an answer.
//
//  The panels are child regions of one full-window host, so the layout is
//  ImGui's problem rather than arithmetic: the last panel in each column takes
//  whatever is left, which means no panel can be clipped by the window size or
//  by the display's scaling.
//
//  Build it with -DSTEAMBRIDGE_BUILD_GUI=ON (see README, "The live view").
// ---------------------------------------------------------------------------

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "bridge/log.hpp"
#include "bridge/scenario.hpp"
#include "bridge/server.hpp"

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace {

using steambridge::CallRecord;
using steambridge::LogLevel;
using steambridge::Server;
using steambridge::ServerOptions;
using steambridge::SessionSnapshot;

// Enough log to see what happened, not enough to grow without bound while a
// game runs for hours.
constexpr std::size_t kMaxLogLines = 2000;

unsigned parse_port(const char* text, unsigned fallback) noexcept {
    if (text == nullptr || text[0] == '\0') {
        return fallback;
    }
    unsigned value = 0;
    for (const char* cursor = text; *cursor != '\0'; ++cursor) {
        if (*cursor < '0' || *cursor > '9') {
            return fallback;
        }
        value = value * 10u + static_cast<unsigned>(*cursor - '0');
        if (value > 65535u) {
            return fallback;
        }
    }
    return value;
}

// `out` is only worth showing when the call had any.
std::string out_suffix(const CallRecord& record) {
    if (!record.out.is_object() || record.out.members().empty()) {
        return std::string();
    }
    return "\nout " + record.out.dump();
}

// A panel's title, drawn inside the child region that holds it.
void panel_header(const char* title) {
    ImGui::TextDisabled("%s", title);
    ImGui::Separator();
}

// ---------------------------------------------------------------------------
//  The window's state: the server, what it has told us so far, and what the
//  person at the keyboard has typed.
// ---------------------------------------------------------------------------
class LiveView {
public:
    LiveView() = default;
    ~LiveView() { stop(); }

    LiveView(const LiveView&) = delete;
    LiveView& operator=(const LiveView&) = delete;

    void set_scenario(const char* scenario) {
        std::snprintf(_scenario, sizeof(_scenario), "%s", scenario);
    }

    void set_host(const char* host) { std::snprintf(_host, sizeof(_host), "%s", host); }

    void set_port(const char* port) { std::snprintf(_port, sizeof(_port), "%s", port); }

    void set_start(bool start) { _start_on_launch = start; }

    void draw() {
        if (_start_on_launch) {
            // --start, so the window comes up serving without a click - which is
            // also what makes it usable from a script.
            _start_on_launch = false;
            start();
        }
        pull();

        draw_layout();
    }

private:
    // -- layout ------------------------------------------------------------

    // One host window, with every panel placed into it from a size that is known
    // before anything is drawn. Each column's heights add up to the window, and
    // so do the two column widths, so no panel can be clipped by the window
    // being smaller than the arithmetic assumed - which is what children sized
    // with "take the rest" cannot promise, because they measure as they go.
    void draw_layout() {
        const ImVec2 display = ImGui::GetIO().DisplaySize;
        constexpr float margin = 8.0f;
        constexpr float gap = 8.0f;

        const float inner_w = std::max(display.x - margin * 2.0f, 640.0f);
        const float column_h = std::max(display.y - margin * 2.0f, 380.0f);

        const float left_w = std::max(inner_w * 0.42f, 300.0f);
        const float right_w = std::max(inner_w - left_w - gap, 300.0f);

        const float controls_h = 168.0f;
        const float games_h = std::max((column_h - controls_h - gap * 2.0f) * 0.5f, 90.0f);
        const float log_h = std::max(column_h - controls_h - games_h - gap * 2.0f, 90.0f);

        const float calls_h = std::max(column_h * 0.56f, 120.0f);
        const float state_h = std::max(column_h - calls_h - gap, 120.0f);

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(display);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(margin, margin));
        const ImGuiWindowFlags host =
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        ImGui::Begin("steambridge", nullptr, host);

        // Placed, not stacked: children flow, so each panel is given the corner
        // it belongs in rather than left to follow the one before it.
        const ImVec2 origin = ImGui::GetCursorScreenPos();
        place("controls", origin, ImVec2(left_w, controls_h), [this] { draw_controls(); });
        place("games", ImVec2(origin.x, origin.y + controls_h + gap), ImVec2(left_w, games_h),
              [this] { draw_games(); });
        place("log", ImVec2(origin.x, origin.y + controls_h + games_h + gap * 2.0f),
              ImVec2(left_w, log_h), [this] { draw_log(); });
        place("calls", ImVec2(origin.x + left_w + gap, origin.y), ImVec2(right_w, calls_h),
              [this] { draw_calls(); });
        place("state", ImVec2(origin.x + left_w + gap, origin.y + calls_h + gap),
              ImVec2(right_w, state_h), [this] { draw_game_state(); });

        ImGui::PopStyleVar();
        ImGui::End();
    }

    template <typename Draw>
    static void place(const char* name, const ImVec2& position, const ImVec2& size, Draw body) {
        ImGui::SetCursorScreenPos(position);
        ImGui::BeginChild(name, size, ImGuiChildFlags_Borders);
        body();
        ImGui::EndChild();
    }

    // -- the server --------------------------------------------------------

    bool start() {
        steambridge::Dispatcher dispatcher;
        std::string error;
        if (!steambridge::Dispatcher::load_file(_scenario, dispatcher, error)) {
            _status = error;
            return false;
        }

        ServerOptions options;
        options.host = _host;
        options.port = static_cast<std::uint16_t>(parse_port(_port, 0));
        options.log_level = LogLevel::debug;
        // The server logs from its own threads, so the sink has to be safe to
        // call from any of them.
        options.log = [this](LogLevel, const std::string& message) {
            std::lock_guard<std::mutex> lock(_log_mutex);
            _log.push_back(message);
            if (_log.size() > kMaxLogLines) {
                _log.erase(_log.begin(),
                           _log.begin() + static_cast<std::ptrdiff_t>(kMaxLogLines / 4));
            }
        };

        _server = std::make_unique<Server>(std::move(dispatcher), std::move(options));
        if (!_server->start(error)) {
            _server.reset();
            _status = error;
            return false;
        }

        // A new server means a new history.
        _calls.clear();
        _shown.clear();
        _seen = 0;
        _games.clear();
        _selected.clear();
        _status = "listening on " + std::string(_host) + ":" + std::to_string(_server->port());
        return true;
    }

    void stop() {
        if (_server) {
            _server->stop();
            _status = "stopped: " + _server->summary();
            _server.reset();
        }
    }

    // Copies what the server has seen since the last frame. The cursor is what
    // keeps this proportional to the new calls rather than to all of them, sixty
    // times a second.
    void pull() {
        if (!_server) {
            return;
        }
        _games = _server->sessions();
        // Show a game's state without making the reader click for it: with one
        // game attached - the usual case - the interesting panel would otherwise
        // sit empty.
        if (_selected.empty() && !_games.empty()) {
            _selected = _games.front().id;
        }
        std::vector<CallRecord> fresh = _server->records_since(_seen);
        _seen = _server->record_count();
        for (CallRecord& record : fresh) {
            _calls.push_back(std::move(record));
        }
    }

    // -- panels ------------------------------------------------------------

    void draw_controls() {
        ImGui::SetNextItemWidth(140);
        ImGui::InputText("host", _host, sizeof(_host));
        ImGui::SetNextItemWidth(64);
        ImGui::InputText("port", _port, sizeof(_port));
        ImGui::SetNextItemWidth(-1);
        ImGui::InputText("scenario", _scenario, sizeof(_scenario));
        ImGui::TextDisabled("a port of 0 picks a free one");

        if (_server) {
            if (ImGui::Button("Stop")) {
                stop();
            }
        } else if (ImGui::Button("Start")) {
            start();
        }
        if (_server) {
            ImGui::SameLine();
            ImGui::Text("%zu call(s), %zu left to the stub's defaults", _server->call_count(),
                        _server->unanswered_count());
        }
        if (!_status.empty()) {
            ImGui::TextWrapped("%s", _status.c_str());
        }
    }

    void draw_games() {
        panel_header("Games");
        if (_games.empty()) {
            ImGui::TextDisabled("no game attached yet");
            ImGui::TextWrapped("Start the server, then run a game with the stub beside it.");
            return;
        }
        const ImGuiTableFlags flags =
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp;
        if (ImGui::BeginTable("games", 4, flags)) {
            ImGui::TableSetupColumn("executable");
            ImGui::TableSetupColumn("pid");
            ImGui::TableSetupColumn("profile");
            ImGui::TableSetupColumn("state");
            ImGui::TableHeadersRow();

            for (const SessionSnapshot& game : _games) {
                // A game that has gone is still worth seeing: its calls are in
                // the history, and this is the state it was left with.
                const bool live = game.connected;
                if (!live) {
                    ImGui::PushStyleColor(ImGuiCol_Text,
                                          ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                }
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                if (ImGui::Selectable(game.exe.c_str(), _selected == game.id,
                                      ImGuiSelectableFlags_SpanAllColumns)) {
                    _selected = game.id;
                }
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("session %s, %zu call(s)", game.id.c_str(), game.call_count);
                }
                ImGui::TableNextColumn();
                ImGui::Text("%lld", static_cast<long long>(game.pid));
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(game.profile.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%s, %zu call(s)", live ? "live" : "gone", game.call_count);
                if (!live) {
                    ImGui::PopStyleColor();
                }
            }
            ImGui::EndTable();
        }
    }

    void draw_calls() {
        panel_header("Calls");
        ImGui::SetNextItemWidth(180);
        const bool filter_changed = _filter.Draw("filter");
        ImGui::SameLine();
        ImGui::Checkbox("follow", &_follow);
        ImGui::SameLine();
        ImGui::TextDisabled("%zu seen", _calls.size());

        // The rows the filter lets through, so the clipper below can skip the
        // ones it does not - clipping the raw list would count hidden rows.
        if (filter_changed || _shown.size() != _calls.size()) {
            _shown.clear();
            for (std::size_t index = 0; index < _calls.size(); ++index) {
                if (_filter.PassFilter(_calls[index].call.c_str())) {
                    _shown.push_back(index);
                }
            }
        }

        // Scrolling to the bottom is only wanted when the view is already there;
        // otherwise it fights whoever is reading further up.
        const bool at_bottom = ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 1.0f;

        const ImGuiTableFlags flags =
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY;
        if (ImGui::BeginTable("calls", 4, flags, ImVec2(0, -1))) {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("seq", ImGuiTableColumnFlags_WidthFixed, 44);
            ImGui::TableSetupColumn("call", ImGuiTableColumnFlags_WidthStretch, 3.0f);
            ImGui::TableSetupColumn("via / ms", ImGuiTableColumnFlags_WidthFixed, 92);
            ImGui::TableSetupColumn("answer", ImGuiTableColumnFlags_WidthStretch, 2.0f);
            ImGui::TableHeadersRow();

            // Only the rows on screen are drawn, so a game that has made tens of
            // thousands of calls still scrolls smoothly.
            ImGuiListClipper clipper;
            clipper.Begin(static_cast<int>(_shown.size()));
            while (clipper.Step()) {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row) {
                    const CallRecord& record = _calls[_shown[static_cast<std::size_t>(row)]];
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%lld", static_cast<long long>(record.seq));
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(record.call.c_str());
                    if (ImGui::IsItemHovered()) {
                        ImGui::SetTooltip("args %s\nret  %s%s", record.args.dump().c_str(),
                                          record.ret.dump().c_str(), out_suffix(record).c_str());
                    }
                    ImGui::TableNextColumn();
                    ImGui::Text("%s %.3f", record.via.c_str(), record.ms);
                    ImGui::TableNextColumn();
                    if (record.answered) {
                        ImGui::TextUnformatted(record.ret.dump().c_str());
                    } else {
                        ImGui::TextDisabled("left to the stub");
                    }
                }
            }
            if (_follow && at_bottom && !_shown.empty()) {
                ImGui::SetScrollHereY(1.0f);
            }
            ImGui::EndTable();
        }
    }

    void draw_game_state() {
        panel_header("Game state");
        const SessionSnapshot* game = selected();
        if (game == nullptr) {
            ImGui::TextDisabled("select a game above");
            ImGui::TextWrapped(
                "This is where a game's identity, stats and achievements will be editable while it "
                "runs - the same state the scenario seeded, and the same state the backend answers "
                "from.");
            return;
        }

        ImGui::Text("session %s", game->id.c_str());
        ImGui::Text("%s, pid %lld, %s", game->exe.c_str(), static_cast<long long>(game->pid),
                    game->arch.c_str());
        ImGui::Text("profile '%s'", game->profile.c_str());

        const ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
        if (ImGui::BeginTable("stats", 2, flags)) {
            ImGui::TableSetupColumn("stat");
            ImGui::TableSetupColumn("value");
            ImGui::TableHeadersRow();
            for (const auto& stat : game->stats) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(stat.first.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%lld", static_cast<long long>(stat.second));
            }
            ImGui::EndTable();
        }
        if (ImGui::BeginTable("achievements", 2, flags)) {
            ImGui::TableSetupColumn("achievement");
            ImGui::TableSetupColumn("unlocked");
            ImGui::TableHeadersRow();
            for (const steambridge::Achievement& achievement : game->achievements) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(achievement.name.c_str());
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(achievement.achieved ? "yes" : "no");
            }
            ImGui::EndTable();
        }
    }

    void draw_log() {
        panel_header("Log");
        const bool at_bottom = ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 1.0f;

        // Copied out under the lock, then drawn: the server's threads should
        // never wait on the window's drawing.
        std::vector<std::string> lines;
        {
            std::lock_guard<std::mutex> lock(_log_mutex);
            lines = _log;
        }
        for (const std::string& line : lines) {
            ImGui::TextUnformatted(line.c_str());
        }
        if (at_bottom) {
            ImGui::SetScrollHereY(1.0f);
        }
    }

    // -- helpers -----------------------------------------------------------

    const SessionSnapshot* selected() const {
        for (const SessionSnapshot& game : _games) {
            if (game.id == _selected) {
                return &game;
            }
        }
        return nullptr;
    }

    char _host[64] = "127.0.0.1";
    char _port[8] = "0";
    char _scenario[512] = "scenarios/example.json";

    std::unique_ptr<Server> _server;
    std::string _status;

    std::size_t _seen = 0;
    std::vector<CallRecord> _calls;
    std::vector<std::size_t> _shown;
    std::vector<SessionSnapshot> _games;
    std::string _selected;

    ImGuiTextFilter _filter;
    bool _follow = true;
    bool _start_on_launch = false;

    std::mutex _log_mutex;
    std::vector<std::string> _log;
};

// A window that fits the display it is opened on: asking for 1280x780 on a
// smaller desktop gets a window with its own panels cut off.
void window_size_for_display(int& width, int& height) {
    width = 1280;
    height = 780;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (monitor == nullptr) {
        return;
    }
    int area_x = 0;
    int area_y = 0;
    int area_w = 0;
    int area_h = 0;
    glfwGetMonitorWorkarea(monitor, &area_x, &area_y, &area_w, &area_h);
    if (area_w > 0 && area_h > 0) {
        width = std::min(width, std::max(area_w - 80, 640));
        height = std::min(height, std::max(area_h - 120, 480));
    }
}

}  // namespace

int main(int argc, char** argv) {
    LiveView view;
    for (int index = 1; index < argc; ++index) {
        const std::string argument = argv[index];
        if (argument == "--scenario" && index + 1 < argc) {
            view.set_scenario(argv[++index]);
        } else if (argument == "--host" && index + 1 < argc) {
            view.set_host(argv[++index]);
        } else if (argument == "--port" && index + 1 < argc) {
            view.set_port(argv[++index]);
        } else if (argument == "--start") {
            view.set_start(true);
        }
    }

    if (glfwInit() != GLFW_TRUE) {
        std::fprintf(stderr,
                     "steambridge_gui: cannot open a window (is a GPU and driver present?)\n");
        return 2;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int window_w = 0;
    int window_h = 0;
    window_size_for_display(window_w, window_h);
    GLFWwindow* window = glfwCreateWindow(window_w, window_h, "SteamApiBridge", nullptr, nullptr);
    if (window == nullptr) {
        std::fprintf(stderr, "steambridge_gui: cannot create the window\n");
        glfwTerminate();
        return 2;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // draw at the display's rate, not the GPU's

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    // The panels are laid out from the window they are given, so there is no
    // arrangement worth remembering - and a leftover imgui.ini would restore
    // geometry computed for a different window size.
    ImGui::GetIO().IniFilename = nullptr;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (glfwWindowShouldClose(window) == 0) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        view.draw();

        ImGui::Render();
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.09f, 0.09f, 0.11f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
