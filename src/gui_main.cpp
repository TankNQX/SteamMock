// ---------------------------------------------------------------------------
//  steammock_gui - the live view.
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
//  The calls have two views, because one game in its own loop is enough to make
//  the other one useless: a frame that polls a call that nobody answers buries
//  everything else, and the first thing anyone wants is the shape rather than
//  the sequence. "by function" counts what the game asks for, most-called first;
//  "live" is the call-by-call list it scrolls past.
//
//  Build it with -DSTEAMMOCK_BUILD_GUI=ON (see README, "The live view").
// ---------------------------------------------------------------------------

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <exception>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "bridge/defaults.hpp"
#include "bridge/log.hpp"
#include "bridge/scenario.hpp"
#include "bridge/server.hpp"
#include "bridge/surface.hpp"

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace
{

using steammock::CallRecord;
using steammock::Json;
using steammock::LogLevel;
using steammock::Server;
using steammock::ServerOptions;
using steammock::SessionSnapshot;
using steammock::SurfaceCall;

// Enough log to see what happened, not enough to grow without bound while a
// game runs for hours.
constexpr std::size_t kMaxLogLines = 2000;

// The calls list, bounded the same way and for the same reason: the run's own record is
// the transcript, and this is a window on the newest of it. Generous, because scrolling
// back through what a game asked is the point of the list.
constexpr std::size_t kMaxCallRows = 20000;

// The port field, read the way the console front end reads its own option. Empty
// or unreadable means "let the operating system pick one", which is what the hint
// beside the field says.
std::uint16_t port_of(const char* text)
{
    unsigned value = 0;
    return steammock::parse_number(text, 65535u, value) ? static_cast<std::uint16_t>(value)
                                                        : static_cast<std::uint16_t>(0);
}

// `out` is only worth showing when the call had any.
std::string out_suffix(const CallRecord& record)
{
    if (!record.out.is_object() || record.out.empty())
    {
        return std::string();
    }
    return "\nout " + record.out.dump();
}

// A panel's title, drawn inside the child region that holds it.
void panel_header(const char* title)
{
    ImGui::TextDisabled("%s", title);
    ImGui::Separator();
}

// The entry a call starts from when nobody has an opinion yet: the value that says "nothing
// happened", so an override begins at the dullest answer and the change is the part somebody
// meant. The kinds are the ones `gen/steam_api_surface.json` carries.
std::string default_entry_for(const char* returns)
{
    const char* kind = returns == nullptr ? "" : returns;
    Json entry = Json::object();
    if (std::strcmp(kind, "void") == 0)
    {
        return entry.dump(); // nothing comes back, so the entry says nothing
    }
    if (std::strcmp(kind, "bool") == 0)
    {
        entry["ret"] = false;
    }
    else if (std::strcmp(kind, "float") == 0 || std::strcmp(kind, "double") == 0)
    {
        entry["ret"] = 0.0;
    }
    else if (std::strcmp(kind, "cstring") == 0)
    {
        entry["ret"] = "";
    }
    else
    {
        entry["ret"] = 0;
    }
    return entry.dump();
}

// How often one call has been seen, and how much of it was answered. Kept as the
// calls arrive rather than counted from the history, because the history grows
// without bound while a game runs - and the panel that shows this is the one
// that has to stay usable when a game has made tens of thousands of calls.
// One record's answer in the words a `scripted` entry uses, which is what a scenario file, a
// transcript and the config tab all read.
std::string entry_of(const CallRecord& record)
{
    Json entry = Json::object();
    if (!record.ret.is_null())
    {
        entry["ret"] = record.ret;
    }
    // An out-parameter is the harder half of an entry to write by hand, and the half a call
    // cannot be answered without, so it comes along when there was one.
    if (record.out.is_object() && !record.out.empty())
    {
        entry["out"] = record.out;
    }
    return entry.dump();
}

struct CallTally
{
    std::size_t calls = 0;
    std::size_t answered = 0;
    double total_ms = 0.0;
    // The last answer this call gave, in the words a `scripted` entry uses. Kept as the calls
    // arrive rather than looked for in the history, because the config tab asks it of every
    // exported call it draws - and walking a twenty-thousand record history per row per frame
    // is not a thing a window can do.
    std::string entry;
};

using CallTallies = std::map<std::string, CallTally>;

// ---------------------------------------------------------------------------
//  The window's state: the server, what it has told us so far, and what the
//  person at the keyboard has typed.
// ---------------------------------------------------------------------------
class LiveView
{
  public:
    // The two fields a person would type are seeded from the one place the default
    // address lives, so the window and the console cannot disagree about where a
    // game is expected to connect.
    LiveView()
    {
        std::snprintf(_host, sizeof(_host), "%s", steammock::kDefaultHost);
        std::snprintf(_port, sizeof(_port), "%u", static_cast<unsigned>(steammock::kDefaultPort));
    }

    ~LiveView() { stop(); }

    LiveView(const LiveView&) = delete;
    LiveView& operator=(const LiveView&) = delete;

    void set_scenario(const char* scenario)
    {
        std::snprintf(_scenario, sizeof(_scenario), "%s", scenario);
    }

    void set_host(const char* host) { std::snprintf(_host, sizeof(_host), "%s", host); }

    void set_port(const char* port) { std::snprintf(_port, sizeof(_port), "%s", port); }

    void set_start(bool start) { _start_on_launch = start; }

    void set_transcript(const char* transcript)
    {
        std::snprintf(_transcript, sizeof(_transcript), "%s", transcript);
    }

    void set_state(const char* state) { std::snprintf(_state, sizeof(_state), "%s", state); }

    void draw()
    {
        if (_start_on_launch)
        {
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
    void draw_layout()
    {
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
        ImGui::Begin("steammock", nullptr, host);

        // Placed, not stacked: children flow, so each panel is given the corner
        // it belongs in rather than left to follow the one before it.
        const ImVec2 origin = ImGui::GetCursorScreenPos();
        place("controls", origin, ImVec2(left_w, controls_h), [this]
              { draw_controls(); });
        place("games", ImVec2(origin.x, origin.y + controls_h + gap), ImVec2(left_w, games_h),
              [this]
              { draw_games(); });
        place("log", ImVec2(origin.x, origin.y + controls_h + games_h + gap * 2.0f),
              ImVec2(left_w, log_h), [this]
              { draw_log(); });
        place("calls", ImVec2(origin.x + left_w + gap, origin.y), ImVec2(right_w, calls_h),
              [this]
              { draw_calls(); });
        place("state", ImVec2(origin.x + left_w + gap, origin.y + calls_h + gap),
              ImVec2(right_w, state_h), [this]
              { draw_game_state(); });

        ImGui::PopStyleVar();
        ImGui::End();
    }

    template <typename Draw>
    static void place(const char* name, const ImVec2& position, const ImVec2& size, Draw body)
    {
        ImGui::SetCursorScreenPos(position);
        ImGui::BeginChild(name, size, ImGuiChildFlags_Borders);
        body();
        ImGui::EndChild();
    }

    // -- the server --------------------------------------------------------

    bool start()
    {
        steammock::Dispatcher dispatcher;
        std::string error;
        if (!steammock::Dispatcher::load_file(_scenario, dispatcher, error))
        {
            _status = error;
            return false;
        }

        ServerOptions options;
        options.host = _host;
        options.port = port_of(_port);
        options.transcript = _transcript;
        // Empty keeps nothing, the same as the console backend's own option: a window not
        // told where to keep state keeps none, and behaves as this harness did before there
        // was anywhere to keep it.
        options.state = _state;
        options.log_level = LogLevel::debug;
        // The server logs from its own threads, so the sink has to be safe to
        // call from any of them.
        options.log = [this](LogLevel, const std::string& message)
        {
            std::scoped_lock lock(_log_mutex);
            _log.push_back(message);
            if (_log.size() > kMaxLogLines)
            {
                _log.erase(_log.begin(),
                           _log.begin() + static_cast<std::ptrdiff_t>(kMaxLogLines / 4));
            }
        };

        _server = std::make_unique<Server>(std::move(dispatcher), std::move(options));
        if (!_server->start(error))
        {
            _server.reset();
            _status = error;
            return false;
        }

        // A new server means a new history.
        _calls.clear();
        _shown.clear();
        _shown_dirty = true;
        _tallies.clear();
        _function_order.clear();
        _tallies_dirty = false;
        _seen = 0;
        _games.clear();
        _selected.clear();
        _status = "listening on " + std::string(_host) + ":" + std::to_string(_server->port());
        return true;
    }

    void stop()
    {
        if (_server)
        {
            _server->stop();
            _status = "stopped: " + _server->summary();
            _server.reset();
        }
    }

    // Copies what the server has seen since the last frame. The cursor is what
    // keeps this proportional to the new calls rather than to all of them, sixty
    // times a second.
    void pull()
    {
        if (!_server)
        {
            return;
        }
        _games = _server->sessions();
        // Show a game's state without making the reader click for it: with one
        // game attached - the usual case - the interesting panel would otherwise
        // sit empty.
        if (_selected.empty() && !_games.empty())
        {
            _selected = _games.front().id;
        }
        // The server keeps a window of recent calls and forgets the oldest, so a cursor
        // that fell off the front of it is moved up to the start of the window rather
        // than left asking for records that no longer exist - which would hand the same
        // retained ones back on every frame and show them twice.
        _seen = std::max(_seen, _server->records_begin());
        std::vector<CallRecord> fresh = _server->records_since(_seen);
        // The cursor moves by what was actually taken rather than to wherever the
        // history has got to by now. A call that arrived between these two reads used
        // to be counted as read without ever having been copied, so it was missing from
        // the live list for the rest of the run - a row silently dropped, which is the
        // one thing a view of a call sequence must not do.
        _seen += fresh.size();
        for (CallRecord& record : fresh)
        {
            CallTally& tally = _tallies[record.call];
            ++tally.calls;
            if (record.answered)
            {
                ++tally.answered;
                tally.entry = entry_of(record);
            }
            tally.total_ms += record.ms;
            // ...and the row the filter lets through joins the index here, so the list
            // below does not have to be walked again on every frame that has new calls.
            if (_filter.PassFilter(record.call.c_str()))
            {
                _shown.push_back(_calls.size());
            }
            _calls.push_back(std::move(record));
        }
        if (!fresh.empty())
        {
            _tallies_dirty = true;
        }
        if (_calls.size() > kMaxCallRows)
        {
            const std::size_t excess = _calls.size() - kMaxCallRows;
            _calls.erase(_calls.begin(), _calls.begin() + static_cast<std::ptrdiff_t>(excess));
            // Every index in the list has moved, so it is built again on the next frame
            // rather than adjusted here.
            _shown_dirty = true;
        }
    }

    // -- panels ------------------------------------------------------------

    void draw_controls()
    {
        ImGui::SetNextItemWidth(140);
        ImGui::InputText("host", _host, sizeof(_host));
        ImGui::SetNextItemWidth(64);
        ImGui::InputText("port", _port, sizeof(_port));
        ImGui::SetNextItemWidth(-1);
        ImGui::InputText("scenario", _scenario, sizeof(_scenario));
        ImGui::TextDisabled("a port of 0 picks a free one");

        if (_server)
        {
            if (ImGui::Button("Stop"))
            {
                stop();
            }
        }
        else if (ImGui::Button("Start"))
        {
            start();
        }
        if (_server)
        {
            ImGui::SameLine();
            ImGui::Text("%zu call(s), %zu left to the stub's defaults, %zu overridden",
                        _server->call_count(), _server->unanswered_count(),
                        _server->overrides().size());
        }
        if (!_status.empty())
        {
            ImGui::TextWrapped("%s", _status.c_str());
        }
    }

    void draw_games()
    {
        panel_header("Games");
        if (_games.empty())
        {
            ImGui::TextDisabled("no game attached yet");
            ImGui::TextWrapped("Start the server, then run a game with the stub beside it.");
            return;
        }
        const ImGuiTableFlags flags =
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp;
        if (ImGui::BeginTable("games", 4, flags))
        {
            ImGui::TableSetupColumn("executable");
            ImGui::TableSetupColumn("pid");
            ImGui::TableSetupColumn("profile");
            ImGui::TableSetupColumn("state");
            ImGui::TableHeadersRow();

            for (const SessionSnapshot& game : _games)
            {
                // A game that has gone is still worth seeing: its calls are in
                // the history, and this is the state it was left with.
                const bool live = game.connected;
                if (!live)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text,
                                          ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                }
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                // The session id is what makes a row its own. Three copies of one
                // executable all label themselves the same, and ImGui routes hover
                // and clicks by ID: without this the three rows are one item, and a
                // new-enough ImGui says so out loud with an ID-conflict popup.
                ImGui::PushID(game.id.c_str());
                if (ImGui::Selectable(game.exe.c_str(), _selected == game.id,
                                      ImGuiSelectableFlags_SpanAllColumns))
                {
                    _selected = game.id;
                }
                if (ImGui::IsItemHovered())
                {
                    ImGui::SetTooltip("session %s, %zu call(s)", game.id.c_str(), game.call_count);
                }
                ImGui::PopID();
                ImGui::TableNextColumn();
                ImGui::Text("%lld", static_cast<long long>(game.pid));
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(game.profile.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%s, %zu call(s)", live ? "live" : "gone", game.call_count);
                if (!live)
                {
                    ImGui::PopStyleColor();
                }
            }
            ImGui::EndTable();
        }
    }

    // Views of the same run: one game in its own loop is enough to make the first of
    // them useless, because a call somebody polls every frame buries every other call in
    // the list - so the count comes first and the sequence second. The third is not a
    // view at all, but the one thing here a person *sets* rather than reads.
    void draw_calls()
    {
        panel_header("Calls");
        if (ImGui::BeginTabBar("call_views"))
        {
            if (ImGui::BeginTabItem("by function"))
            {
                draw_calls_by_function();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("live"))
            {
                draw_calls_live();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("config"))
            {
                draw_config();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }

    // Every call the game has made, one row per name. A list of a hundred calls
    // is a profile, and the end worth reading is the top, so the order is by how
    // often each was asked for.
    void draw_calls_by_function()
    {
        ImGui::SetNextItemWidth(180);
        if (_filter.Draw("filter"))
        {
            // The live tab builds its row index with this same filter, and the edit happens
            // while *this* tab is the one on screen - so nothing there sees a change this
            // frame. Without marking the index dirty, switching to "live" found no change and
            // no dirty flag and kept the old subset: the rows that no longer match stayed in
            // and the ones that now match were never added. A filter edited here is a filter
            // edited, whichever view was asked for.
            _shown_dirty = true;
        }
        ImGui::SameLine();
        ImGui::TextDisabled("%zu function(s)", _tallies.size());

        // Sorted when the counts change rather than every frame, so a game that
        // has flooded the history does not make the window sort it sixty times a
        // second to draw the same order.
        if (_tallies_dirty)
        {
            _tallies_dirty = false;
            _function_order.clear();
            _function_order.reserve(_tallies.size());
            for (const CallTallies::value_type& entry : _tallies)
            {
                _function_order.push_back(&entry);
            }
            // A map hands these back in name order, which is not the order this
            // panel is for; the name settles a tie so rows do not swap places
            // between frames while their counts are equal.
            std::sort(
                _function_order.begin(), _function_order.end(),
                [](const CallTallies::value_type* left, const CallTallies::value_type* right)
                {
                    if (left->second.calls != right->second.calls)
                    {
                        return left->second.calls > right->second.calls;
                    }
                    return left->first < right->first;
                });
        }

        const ImGuiTableFlags flags =
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY;
        if (ImGui::BeginTable("by_function", 2, flags, ImVec2(0, -1)))
        {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("function", ImGuiTableColumnFlags_WidthStretch, 3.0f);
            ImGui::TableSetupColumn("calls", ImGuiTableColumnFlags_WidthFixed, 72.0f);
            ImGui::TableHeadersRow();

            // No clipper here, unlike the live list: the rows are the distinct
            // calls the game has made - a few dozen for a game that polls - where
            // that list is every call there has ever been.
            for (const CallTallies::value_type* entry : _function_order)
            {
                if (!_filter.PassFilter(entry->first.c_str()))
                {
                    continue;
                }
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(entry->first.c_str());
                if (ImGui::IsItemHovered())
                {
                    const CallTally& tally = entry->second;
                    ImGui::SetTooltip("%zu answered, %zu left to the stub\nmean %.3f ms",
                                      tally.answered, tally.calls - tally.answered,
                                      tally.calls == 0u
                                          ? 0.0
                                          : tally.total_ms / static_cast<double>(tally.calls));
                }
                ImGui::TableNextColumn();
                ImGui::Text("%zu", entry->second.calls);
            }
            ImGui::EndTable();
        }
    }

    // The calls in the order they were made, which is where a scenario or a
    // session's state gets read back.
    void draw_calls_live()
    {
        ImGui::SetNextItemWidth(180);
        const bool filter_changed = _filter.Draw("filter");
        ImGui::SameLine();
        ImGui::Checkbox("follow", &_follow);
        ImGui::SameLine();
        ImGui::TextDisabled("%zu seen", _calls.size());

        // The rows the filter lets through, so the clipper below can skip the
        // ones it does not - clipping the raw list would count hidden rows. Built
        // again only when the filter changes or when the list has been trimmed, and
        // added to in `pull()` otherwise: a frame with new calls used to walk the
        // whole history to find out that it had.
        if (filter_changed || _shown_dirty)
        {
            _shown_dirty = false;
            _shown.clear();
            for (std::size_t index = 0; index < _calls.size(); ++index)
            {
                if (_filter.PassFilter(_calls[index].call.c_str()))
                {
                    _shown.push_back(index);
                }
            }
        }

        const ImGuiTableFlags flags =
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY;
        if (ImGui::BeginTable("calls", 4, flags, ImVec2(0, -1)))
        {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("seq", ImGuiTableColumnFlags_WidthFixed, 44);
            ImGui::TableSetupColumn("call", ImGuiTableColumnFlags_WidthStretch, 3.0f);
            ImGui::TableSetupColumn("via / ms", ImGuiTableColumnFlags_WidthFixed, 92);
            ImGui::TableSetupColumn("answer", ImGuiTableColumnFlags_WidthStretch, 2.0f);
            ImGui::TableHeadersRow();

            // Read *inside* the table, which is where the scrolling is. The table owns an
            // inner window of its own (ImGuiTableFlags_ScrollY) and it is sized to fill the
            // child it sits in (ImVec2(0, -1)), so the child never overflows - which is why
            // asking before BeginTable read 0 of 0, made `at_bottom` always true, and with
            // `follow` on re-scrolled to the bottom every frame while somebody was trying to
            // read further up.
            //
            // Scrolling to the bottom is only wanted when the view is already there.
            const bool at_bottom = ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 1.0f;

            // Only the rows on screen are drawn, so a game that has made tens of
            // thousands of calls still scrolls smoothly.
            ImGuiListClipper clipper;
            clipper.Begin(static_cast<int>(_shown.size()));
            while (clipper.Step())
            {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row)
                {
                    const CallRecord& record = _calls[_shown[static_cast<std::size_t>(row)]];
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%lld", static_cast<long long>(record.seq));
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(record.call.c_str());
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::SetTooltip("args %s\nret  %s%s", record.args.dump().c_str(),
                                          record.ret.dump().c_str(), out_suffix(record).c_str());
                    }
                    ImGui::TableNextColumn();
                    ImGui::Text("%s %.3f", record.via.c_str(), record.ms);
                    ImGui::TableNextColumn();
                    if (record.answered)
                    {
                        ImGui::TextUnformatted(record.ret.dump().c_str());
                    }
                    else
                    {
                        ImGui::TextDisabled("left to the stub");
                    }
                }
            }
            if (_follow && at_bottom && !_shown.empty())
            {
                ImGui::SetScrollHereY(1.0f);
            }
            ImGui::EndTable();
        }
    }

    void draw_game_state()
    {
        panel_header("Game state");
        const SessionSnapshot* game = selected();
        if (game == nullptr)
        {
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
        if (ImGui::BeginTable("stats", 2, flags))
        {
            ImGui::TableSetupColumn("stat");
            ImGui::TableSetupColumn("value");
            ImGui::TableHeadersRow();
            for (const auto& [stat_name, stat_value] : game->stats)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(stat_name.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%lld", static_cast<long long>(stat_value));
            }
            ImGui::EndTable();
        }
        if (ImGui::BeginTable("achievements", 2, flags))
        {
            ImGui::TableSetupColumn("achievement");
            ImGui::TableSetupColumn("unlocked");
            ImGui::TableHeadersRow();
            for (const steammock::Achievement& achievement : game->achievements)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(achievement.name.c_str());
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(achievement.achieved ? "yes" : "no");
            }
            ImGui::EndTable();
        }
    }

    void draw_log()
    {
        panel_header("Log");
        const bool at_bottom = ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 1.0f;

        // Copied out under the lock, then drawn: the server's threads should
        // never wait on the window's drawing.
        std::vector<std::string> lines;
        {
            std::scoped_lock lock(_log_mutex);
            lines = _log;
        }
        for (const std::string& line : lines)
        {
            ImGui::TextUnformatted(line.c_str());
        }
        if (at_bottom)
        {
            ImGui::SetScrollHereY(1.0f);
        }
    }

    // -- helpers -----------------------------------------------------------

    const SessionSnapshot* selected() const
    {
        for (const SessionSnapshot& game : _games)
        {
            if (game.id == _selected)
            {
                return &game;
            }
        }
        return nullptr;
    }

    // The config tab: one row per call the stub exports, each answer editable where it
    // stands.
    //
    // One row per *name* is the whole reason this is a tab of its own. A call somebody polls
    // every frame is one row in the live list for every time it was made, so a control there
    // says "this call, this time" - which is not what an override is. Here the row and the
    // thing being set are the same thing, and every call the stub can send has a row whether
    // the game has asked for it yet or not.
    void draw_config()
    {
        ImGui::SetNextItemWidth(180);
        _known_filter.Draw("filter");
        ImGui::SameLine();
        ImGui::Checkbox("set only", &_only_overridden);

        // Taken once a frame rather than once a row: the server hands the overrides back by
        // value, and asking 826 times a frame to look at two of them is not worth doing.
        const std::vector<std::pair<std::string, Json>> in_force = overrides();
        _in_force.clear();
        for (const std::pair<std::string, Json>& entry : in_force)
        {
            _in_force.insert(entry);
        }

        std::size_t exported = 0;
        const SurfaceCall* calls = steammock::api_surface_calls(exported);
        std::vector<const SurfaceCall*> matches;
        for (std::size_t index = 0; index < exported; ++index)
        {
            if (!_known_filter.PassFilter(calls[index].name))
            {
                continue;
            }
            if (_only_overridden && _in_force.count(calls[index].name) == 0)
            {
                continue;
            }
            matches.push_back(&calls[index]);
        }

        ImGui::TextDisabled("%zu of %zu exported call(s), %zu overridden", matches.size(), exported,
                            _in_force.size());
        if (!_override_note.empty())
        {
            ImGui::TextWrapped("%s", _override_note.c_str());
        }

        const ImGuiTableFlags flags =
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY;
        if (ImGui::BeginTable("exported_calls", 3, flags))
        {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("call", ImGuiTableColumnFlags_WidthStretch, 3.0f);
            ImGui::TableSetupColumn("answers now", ImGuiTableColumnFlags_WidthStretch, 2.0f);
            ImGui::TableSetupColumn("override", ImGuiTableColumnFlags_WidthStretch, 2.0f);
            ImGui::TableHeadersRow();

            // The rows off screen are not drawn, and the height is handed in rather than
            // measured: a row holds a text box while it is being edited and a line of text
            // while it is not, and the table must not learn its height from whichever of the
            // two it happened to see first.
            const float row = ImGui::GetFrameHeightWithSpacing();
            ImGuiListClipper clipper;
            clipper.Begin(static_cast<int>(matches.size()), row);
            while (clipper.Step())
            {
                for (int at = clipper.DisplayStart; at < clipper.DisplayEnd; ++at)
                {
                    const SurfaceCall& call = *matches[static_cast<std::size_t>(at)];
                    // A name is unique in this table, so it is the row's identity - the one
                    // thing the live list's rows could not offer.
                    ImGui::PushID(call.name);
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(call.name);
                    ImGui::TableNextColumn();
                    draw_answers_now(call.name);
                    ImGui::TableNextColumn();
                    draw_inline_entry(call);
                    ImGui::PopID();
                }
            }
            ImGui::EndTable();
        }
        ImGui::TextDisabled(
            "click an override to edit it: Enter sets it, and an empty box takes it off");
    }

    // What the call gave this run, beside the cell a person edits, so the decision is made
    // next to the evidence. "never asked" is said out loud rather than left blank: a call the
    // game has not reached is exactly the one an override is often being written for.
    void draw_answers_now(const std::string& name) const
    {
        const auto tally = _tallies.find(name);
        if (tally == _tallies.end() || tally->second.entry.empty())
        {
            ImGui::TextDisabled("never asked");
            return;
        }
        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
        ImGui::TextUnformatted(tally->second.entry.c_str());
        ImGui::PopStyleColor();
    }

    // The override, edited where it stands. One buffer is enough because one row is open at a
    // time, and what the box holds when it opens is the answer already set, or the one this
    // run saw the call give, or the value its own return kind calls "nothing happened" - so
    // the usual edit is one field changed rather than an entry written from nothing.
    void draw_inline_entry(const SurfaceCall& call)
    {
        const auto in_force = _in_force.find(call.name);
        const bool set = in_force != _in_force.end();

        if (_editing != call.name)
        {
            if (!set)
            {
                ImGui::PushStyleColor(ImGuiCol_Text,
                                      ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
            }
            const bool clicked = ImGui::Selectable(set ? in_force->second.dump().c_str() : "not set");
            if (!set)
            {
                ImGui::PopStyleColor();
            }
            if (clicked)
            {
                begin_inline(call, set ? in_force->second.dump() : std::string());
            }
            return;
        }

        if (_focus_edit)
        {
            _focus_edit = false;
            // The row was just clicked, so the caret belongs in this box rather than wherever
            // the keyboard was pointed before.
            ImGui::SetKeyboardFocusHere();
        }
        ImGui::SetNextItemWidth(-1.0f);
        const bool entered = ImGui::InputText("##entry", _edit_text, sizeof(_edit_text),
                                              ImGuiInputTextFlags_EnterReturnsTrue);
        const bool left = ImGui::IsItemDeactivated();
        if (entered)
        {
            apply_inline(call);
        }
        else if (left)
        {
            _editing.clear(); // clicked away, or Escape: nothing was changed
        }
    }

    // Opens a row for editing. What the box holds is the override already set, or - when
    // there is none - the answer this run saw the call give, or the value its own return kind
    // calls "nothing happened". So the usual edit is one field changed rather than an entry
    // written from nothing.
    void begin_inline(const SurfaceCall& call, const std::string& already_set)
    {
        _editing = call.name;
        _override_note.clear();
        const std::string start = already_set.empty() ? suggested_entry(call) : already_set;
        std::snprintf(_edit_text, sizeof(_edit_text), "%s", start.c_str());
        _focus_edit = true;
    }

    // An empty box is how an override comes off. That is one control fewer than a Clear
    // button, and the same spelling an empty entry already means to the server.
    void apply_inline(const SurfaceCall& call)
    {
        _editing.clear();
        _override_note.clear();
        if (!_server)
        {
            _override_note = "the server is not running, so there is nothing to override";
            return;
        }
        const std::string text(_edit_text);
        if (text.find_first_not_of(" \t") == std::string::npos)
        {
            _server->clear_override(call.name);
            _override_note = std::string("no override on ") + call.name + " any more";
            return;
        }
        // Parsed without exceptions, because this is somebody's typing and a half-finished
        // entry arrives here every time.
        const Json entry = Json::parse(text, nullptr, false);
        if (entry.is_discarded() || !entry.is_object())
        {
            _override_note = std::string(call.name) + ": that is not an entry, so nothing changed";
            return;
        }
        _server->set_override(call.name, entry);
        _override_note = std::string("overriding ") + call.name;
    }

    // What the box starts from when nothing is set: what this run saw the call give, or the
    // value its own return kind calls "nothing happened".
    std::string suggested_entry(const SurfaceCall& call) const
    {
        const auto tally = _tallies.find(call.name);
        if (tally != _tallies.end() && !tally->second.entry.empty())
        {
            return tally->second.entry;
        }
        return default_entry_for(call.returns);
    }

    // What the server is holding. Asked for rather than kept here: the server owns the
    // overrides, and a copy in the window would be a second answer to what is in force.
    std::vector<std::pair<std::string, Json>> overrides() const
    {
        return _server ? _server->overrides() : std::vector<std::pair<std::string, Json>>();
    }

    char _host[64] = {};
    // The port the stub and the backend default to, because the whole point of the
    // window is to be started and then forgotten while a game is run beside it. A
    // port of 0 - whatever is free - is there for running two of these at once, and
    // the field says so.
    char _port[8] = {};
    char _scenario[512] = "scenarios/example.json";

    // Empty keeps no transcript, exactly as the console backend's own option does. A window
    // is lovely and a script still needs something to read: the rig that runs clients beside
    // this learns the lobby id from a transcript, and has nowhere else to learn it.
    char _transcript[512] = {};

    // And empty keeps no state, the same way. A run pointed at a file keeps what its games
    // write and starts from what the last run left there; without one, nothing is kept and
    // the window is exactly what it was before this existed.
    char _state[512] = {};

    std::unique_ptr<Server> _server;
    std::string _status;

    std::size_t _seen = 0;
    std::vector<CallRecord> _calls;
    std::vector<std::size_t> _shown;
    // Set when `_calls` loses its oldest rows, which moves every index in `_shown`.
    bool _shown_dirty = true;
    // What the "by function" view draws, and the order it draws it in: pointers
    // into the map, which a node-based container keeps valid as calls arrive.
    CallTallies _tallies;
    std::vector<const CallTallies::value_type*> _function_order;
    bool _tallies_dirty = false;
    std::vector<SessionSnapshot> _games;
    std::string _selected;

    ImGuiTextFilter _filter;
    bool _follow = true;
    bool _start_on_launch = false;

    // The config tab: the filter over the exported calls, the overrides taken once a frame so
    // that a row can look itself up, and one row at a time open for editing - which is why
    // one buffer is enough for the entry being typed. `_only_overridden` is how what is in
    // force is read in one place rather than hunted for among 826 rows.
    ImGuiTextFilter _known_filter;
    std::map<std::string, Json> _in_force;
    std::string _editing;
    char _edit_text[512] = {};
    bool _focus_edit = false;
    bool _only_overridden = false;
    std::string _override_note;

    std::mutex _log_mutex;
    std::vector<std::string> _log;
};

// A window that fits the display it is opened on: asking for 1280x780 on a
// smaller desktop gets a window with its own panels cut off.
void window_size_for_display(int& width, int& height)
{
    width = 1280;
    height = 780;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (monitor == nullptr)
    {
        return;
    }
    int area_x = 0;
    int area_y = 0;
    int area_w = 0;
    int area_h = 0;
    glfwGetMonitorWorkarea(monitor, &area_x, &area_y, &area_w, &area_h);
    if (area_w > 0 && area_h > 0)
    {
        width = std::min(width, std::max(area_w - 80, 640));
        height = std::min(height, std::max(area_h - 120, 480));
    }
}

} // namespace

int run(int argc, char** argv)
{
    LiveView view;
    for (int index = 1; index < argc; ++index)
    {
        const std::string argument = argv[index];
        if (argument == "--scenario" && index + 1 < argc)
        {
            view.set_scenario(argv[++index]);
        }
        else if (argument == "--host" && index + 1 < argc)
        {
            view.set_host(argv[++index]);
        }
        else if (argument == "--port" && index + 1 < argc)
        {
            view.set_port(argv[++index]);
        }
        else if (argument == "--start")
        {
            view.set_start(true);
        }
        else if (argument == "--transcript" && index + 1 < argc)
        {
            // The same server is behind this window, so the same transcript is worth having:
            // it is what a script driving a run reads.
            view.set_transcript(argv[++index]);
        }
        else if (argument == "--state" && index + 1 < argc)
        {
            // And the same state file, for the same reason: the window and the console
            // backend drive one Server, and a run that keeps state should keep it whichever
            // front end started it.
            view.set_state(argv[++index]);
        }
    }

    if (glfwInit() != GLFW_TRUE)
    {
        std::fprintf(stderr,
                     "steammock_gui: cannot open a window (is a GPU and driver present?)\n");
        return 2;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int window_w = 0;
    int window_h = 0;
    window_size_for_display(window_w, window_h);
    GLFWwindow* window = glfwCreateWindow(window_w, window_h, "SteamMock", nullptr, nullptr);
    if (window == nullptr)
    {
        std::fprintf(stderr, "steammock_gui: cannot create the window\n");
        glfwTerminate();
        return 2;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // draw at the display's rate, not the GPU's

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    // The panels are laid out from the window they are given, so there is no
    // arrangement worth remembering - and a leftover imgui.ini would restore
    // geometry computed for a different window size.
    ImGui::GetIO().IniFilename = nullptr;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Declared after the backends are up, so it covers the frame loop and everything that
    // can throw inside it - `view.draw()` allocates every frame, and a std::bad_alloc out of
    // it used to leave `main`'s catch handing back a return code with the window, the GL
    // context and the ImGui context still open and `glfwTerminate()` never run. The window is
    // released on every way out of this function now, not only the one where the loop ended
    // by itself.
    struct LiveViewCleanup
    {
        GLFWwindow* window;
        ~LiveViewCleanup()
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            glfwDestroyWindow(window);
            glfwTerminate();
        }
    } cleanup{window};

    while (glfwWindowShouldClose(window) == 0)
    {
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

    return 0;
}

// The window is the one thing here that allocates on a path nothing catches, and an
// exception escaping `main` says nothing at all. Say what happened instead.
int main(int argc, char** argv)
{
    try
    {
        return run(argc, argv);
    }
    catch (const std::exception& error)
    {
        std::fprintf(stderr, "steammock_gui: unexpected failure (%s)\n", error.what());
        return 2;
    }
    catch (...)
    {
        std::fprintf(stderr, "steammock_gui: unexpected failure\n");
        return 2;
    }
}
