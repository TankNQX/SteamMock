cmake_minimum_required(VERSION 3.20)
project(SteamMock VERSION 0.1.0 LANGUAGES CXX)

# ---------------------------------------------------------------------------
#  A local debugging harness for the Steamworks flat API.
#
#  The stub DLL is shaped like steam_api64.dll: it exports the same flat API
#  names and forwards every call over loopback TCP to a backend that decides the
#  answer. Nothing here talks to Valve, and nothing here is meant to ship inside
#  a game - see README.md, "What this is not".
#
#  Both halves are C++, built from this one project:
#
#    steam_api64.dll        loaded by the game, forwards the calls
#    steammock(.exe)      the backend that answers them
#    steammock_codegen    regenerates the stub from gen/steam_api.idl.json
#
#  Windows only, on purpose: the stub is a DLL a game loads by name, and the
#  backend answers it over loopback Winsock. Neither half has a meaningful build
#  anywhere else, so there is no portability layer to keep in step - and
#  configuring elsewhere stops here rather than half-way through a compile.
# ---------------------------------------------------------------------------

# CMAKE_CXX_COMPILER_FRONTEND_VARIANT is "MSVC" for both cl.exe and clang-cl, and
# "GNU" for a MinGW toolchain - which this is not written for.
if(NOT WIN32 OR NOT CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    message(FATAL_ERROR
        "SteamMock is a Windows harness built with an MSVC-style compiler (cl.exe or "
        "clang-cl): the stub is a DLL a game loads by name, and the backend's server answers "
        "it over loopback Winsock. See README.md.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(STEAMMOCK_STUB_NAME "steam_api64" CACHE STRING
    "Output name of the stub DLL. Use steam_api for a 32-bit game, or any name you like for a custom load")
option(STEAMMOCK_BUILD_TESTS "Build the C++ tests" ON)
option(STEAMMOCK_WERROR "Treat warnings as errors" OFF)

# ---------------------------------------------------------------------------
#  Core: the wire format, the JSON subset, the transport and the client. Static,
#  so the stub, the backend and the tests all share one copy.
# ---------------------------------------------------------------------------
# ---------------------------------------------------------------------------
#  nlohmann/json - the JSON the bridge speaks, and the one third party library the
#  stub and the backend share. Header only, so it is an interface target.
# ---------------------------------------------------------------------------
add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/nlohmann_json/single_include)

add_library(steammock_core STATIC
    src/log.cpp
    src/transport_tcp.cpp
    src/client.cpp
)
target_include_directories(steammock_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(steammock_core PUBLIC cxx_std_17)

# Winsock: the loopback transport and the backend's server. Public, because
# everything that links the core ends up needing it. And JSON, for the same reason:
# the protocol, the scenarios and the transcripts are all JSON.
target_link_libraries(steammock_core PUBLIC nlohmann_json ws2_32)

# ---------------------------------------------------------------------------
#  The backend: the session state machine, the scenario dispatcher and the
#  server. Linkable on its own so a front end - the console, or a future GUI -
#  is only a way of driving it.
# ---------------------------------------------------------------------------
add_library(steammock_backend STATIC
    src/protocol.cpp
    src/session.cpp
    src/scenario.cpp
    src/lobby.cpp
    src/server.cpp
    src/generated/api_surface.cpp
)
target_link_libraries(steammock_backend PUBLIC steammock_core)

# ---------------------------------------------------------------------------
#  The stub DLL: DllMain, the client bootstrap and the generated trampolines.
# ---------------------------------------------------------------------------
add_library(${STEAMMOCK_STUB_NAME} SHARED
    src/dllmain.cpp
    src/synth.cpp
    src/generated/api_stub.cpp
    src/generated/api_interfaces.cpp
    src/generated/steam_api_exports.def
)
target_link_libraries(${STEAMMOCK_STUB_NAME} PRIVATE steammock_core)
set_target_properties(${STEAMMOCK_STUB_NAME} PROPERTIES
    PREFIX ""                       # steam_api64.dll, not libsteam_api64.dll
    OUTPUT_NAME "${STEAMMOCK_STUB_NAME}"
    DEBUG_POSTFIX "d"
)
# One version, from the project() call at the top, so the DLL's
# SteamMock_Version and the backend's --version cannot report different builds.
target_compile_definitions(${STEAMMOCK_STUB_NAME} PRIVATE
    STEAMMOCK_VERSION="${PROJECT_VERSION}")
# The exported names come straight from the generated .def, which CMake hands to
# the linker as a source: no decoration on x64, and the same undecorated names on
# Win32 that the real steam_api.dll uses.

# ---------------------------------------------------------------------------
#  The two programs: the backend a person runs, and the generator.
# ---------------------------------------------------------------------------
add_executable(steammock src/backend_main.cpp)
target_link_libraries(steammock PRIVATE steammock_backend)
set_target_properties(steammock PROPERTIES OUTPUT_NAME "steammock")
target_compile_definitions(steammock PRIVATE STEAMMOCK_VERSION="${PROJECT_VERSION}")

add_executable(steammock_codegen src/codegen_main.cpp src/idl.cpp src/interfaces.cpp)
target_link_libraries(steammock_codegen PRIVATE steammock_core)

# ---------------------------------------------------------------------------
#  The generated files, written by that generator before anything reads them
# ---------------------------------------------------------------------------
#  Nothing under src/generated/ is committed: they are outputs, and the build runs
#  the generator for them. Neither is gen/steam_interfaces.json, because it holds
#  Valve's own interface layouts - whoever builds this imports them from an SDK
#  they have:
#
#    python tools/steamworks_sdk_import.py --sdk <sdk>/public/steam --out gen/steam_interfaces.json
#
#  Without one the stub hands out no interface objects - a game built against a
#  recent SDK gets null where it asks for a version string - and the end-to-end
#  test says so and skips those checks. Everything else works the same.
set(STEAMMOCK_IDL "${CMAKE_CURRENT_SOURCE_DIR}/gen/steam_api.idl.json"
    CACHE FILEPATH "The API surface the stub is generated from")
set(STEAMMOCK_INTERFACES "${CMAKE_CURRENT_SOURCE_DIR}/gen/steam_interfaces.json"
    CACHE FILEPATH "The interface layouts the stub is generated from (imported from an SDK)")

set(STEAMMOCK_GENERATED
    "${CMAKE_CURRENT_SOURCE_DIR}/src/generated/api_stub.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/generated/api_surface.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/generated/api_interfaces.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/generated/steam_api_exports.def"
)

# The layouts are an input only when they are there: a checkout with none still
# builds, and re-importing them means configuring again (or running the generator
# by hand), which is what regeneration has always meant here.
set(STEAMMOCK_GENERATED_INPUTS "${STEAMMOCK_IDL}")
if(EXISTS "${STEAMMOCK_INTERFACES}")
    list(APPEND STEAMMOCK_GENERATED_INPUTS "${STEAMMOCK_INTERFACES}")
else()
    message(STATUS "steammock: no interface layouts at ${STEAMMOCK_INTERFACES} - "
                   "the stub will hand out no interface objects")
endif()

add_custom_command(
    OUTPUT ${STEAMMOCK_GENERATED}
    COMMAND steammock_codegen
            --idl "${STEAMMOCK_IDL}"
            --interfaces "${STEAMMOCK_INTERFACES}"
            --root "${CMAKE_CURRENT_SOURCE_DIR}"
    DEPENDS steammock_codegen ${STEAMMOCK_GENERATED_INPUTS}
    COMMENT "steammock_codegen: ${STEAMMOCK_IDL}"
    VERBATIM
)
add_custom_target(steammock_generated DEPENDS ${STEAMMOCK_GENERATED})

# The backend's surface table and the stub's trampolines are those outputs, so the
# two of them wait for the generator - and so does the check below, which reads
# what it wrote.
add_dependencies(steammock_backend steammock_generated)
add_dependencies(${STEAMMOCK_STUB_NAME} steammock_generated)

# ---------------------------------------------------------------------------
#  The live view - optional, and the only part that uses other people's code.
# ---------------------------------------------------------------------------
#  The backend with a window on it: which games are attached, what they are
#  asking, what they were told. It drives the same Server the console does and
#  only ever reads snapshots, so there is one set of decisions behind both.
#
#  Off by default, because it is the one thing here that needs the GLFW and
#  Dear ImGui submodules - and the stub and the backend are useful without it.
option(STEAMMOCK_BUILD_GUI "Build the live view (needs the GLFW and Dear ImGui submodules)" OFF)

if(STEAMMOCK_BUILD_GUI)
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui.cpp OR
       NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/CMakeLists.txt)
        message(FATAL_ERROR
            "the live view needs its submodules: git submodule update --init --recursive. "
            "Configure with -DSTEAMMOCK_BUILD_GUI=OFF to build the stub and backend alone.")
    endif()

    # Dear ImGui ships sources rather than a build, so it gets one here. SYSTEM,
    # so its headers are not held to this project's warning level.
    add_library(imgui STATIC
        external/imgui/imgui.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_tables.cpp
        external/imgui/imgui_widgets.cpp
        external/imgui/backends/imgui_impl_glfw.cpp
        external/imgui/backends/imgui_impl_opengl3.cpp
    )
    target_include_directories(imgui SYSTEM PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends
    )
    # The GLFW backend needs GLFW's headers to compile; the application is what
    # links the library. SYSTEM again, so glfw3.h is not held to /W4 either.
    target_include_directories(imgui SYSTEM PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include)

    # GLFW's own build, minus the parts that only matter to GLFW's developers.
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    add_subdirectory(external/glfw EXCLUDE_FROM_ALL)

    add_executable(steammock_gui src/gui_main.cpp)
    target_link_libraries(steammock_gui PRIVATE steammock_backend imgui glfw opengl32)
    target_include_directories(steammock_gui SYSTEM PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include)
    target_compile_definitions(steammock_gui PRIVATE STEAMMOCK_VERSION="${PROJECT_VERSION}")
endif()

# ---------------------------------------------------------------------------
#  Tests
# ---------------------------------------------------------------------------
if(STEAMMOCK_BUILD_TESTS)
    enable_testing()

    # The JSON subset and the frame header, on the side that has to be exact.
    add_executable(test_protocol tests/test_protocol.cpp)
    target_link_libraries(test_protocol PRIVATE steammock_core)
    add_test(NAME protocol COMMAND test_protocol)

    # The backend's own logic: replies, the session state machine, scenarios, and
    # the promise that the state machine only answers calls the IDL exports.
    add_executable(test_backend tests/test_backend.cpp)
    target_link_libraries(test_backend PRIVATE steammock_backend)
    # The bundled scenario lives in the source tree, and a test binary runs from
    # the build tree, so it is handed over as an absolute path.
    target_compile_definitions(test_backend PRIVATE
        STEAMMOCK_SCENARIO_PATH="${CMAKE_CURRENT_SOURCE_DIR}/scenarios/example.json")
    add_test(NAME backend COMMAND test_backend)

    # Reading the interface layouts: the row format, and what it refuses. The
    # generator is the only thing between a hand edit and a vtable a game reads
    # the wrong way, so what it accepts is pinned here rather than assumed.
    add_executable(test_interfaces tests/test_interfaces.cpp src/interfaces.cpp)
    target_link_libraries(test_interfaces PRIVATE steammock_core)
    add_test(NAME interfaces COMMAND test_interfaces)

    # The server itself: a real port, a real connection through the stub's own
    # transport, and the snapshots and summary a live view would draw.
    add_executable(test_server tests/test_server.cpp)
    target_link_libraries(test_server PRIVATE steammock_backend)
    add_test(NAME server COMMAND test_server)

    # The generated files are outputs of the build, and this compares them against
    # the inputs they were generated from - so it fails if the IDL was edited and
    # the tree not rebuilt. Same inputs as the build, by the same paths.
    add_test(NAME generated_files_are_current
             COMMAND $<TARGET_FILE:steammock_codegen> --check
                     --idl "${STEAMMOCK_IDL}" --interfaces "${STEAMMOCK_INTERFACES}")
    set_tests_properties(generated_files_are_current PROPERTIES
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    # The fake game is what makes the end-to-end test an integration test: it
    # loads the built stub the way a game's import table would.
    add_executable(fake_game tests/fake_game.cpp)

    add_executable(test_end_to_end tests/test_end_to_end.cpp)
    target_link_libraries(test_end_to_end PRIVATE steammock_core)
    add_test(NAME end_to_end
             COMMAND test_end_to_end
                     --server $<TARGET_FILE:steammock>
                     --stub $<TARGET_FILE:${STEAMMOCK_STUB_NAME}>
                     --game $<TARGET_FILE:fake_game>
                     --scenario ${CMAKE_CURRENT_SOURCE_DIR}/scenarios/example.json)
    set_tests_properties(end_to_end PROPERTIES TIMEOUT 120)
endif()

# ---------------------------------------------------------------------------
#  Warnings, in one place, for everything this project builds - the tests
#  included, because a test that nobody reads the warnings of is how a real
#  warning gets missed.
#
#  Two compilers build this: MSVC, and clang-cl on the same ABI. Clang is worth
#  the second job because it can see what /W4 cannot - a constant nobody uses, a
#  name that shadows a member - which is how kTypeCount and a shadowed
#  from_json parameter were caught.
# ---------------------------------------------------------------------------
set(STEAMMOCK_TARGETS
    steammock_core
    steammock_backend
    ${STEAMMOCK_STUB_NAME}
    steammock
    steammock_codegen
)
if(STEAMMOCK_BUILD_TESTS)
    list(APPEND STEAMMOCK_TARGETS test_protocol test_backend test_interfaces test_server
         fake_game test_end_to_end)
endif()
if(STEAMMOCK_BUILD_GUI)
    list(APPEND STEAMMOCK_TARGETS steammock_gui)
endif()

foreach(_target ${STEAMMOCK_TARGETS})
    target_compile_options(${_target} PRIVATE /W4 /Zc:__cplusplus)
    # getenv and fopen are the C spelling; the _s variants are MSVC-only and the
    # harness reads a handful of variables at startup.
    target_compile_definitions(${_target} PRIVATE _CRT_SECURE_NO_WARNINGS)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # Name the diagnostics instead of importing a GNU flag group. Under
        # -fms-compatibility -Wall/-Wextra/-Wpedantic pull in the C++98
        # compatibility family, which is a flood of noise on any C++11 code -
        # and it fired even without them, so it is off explicitly.
        #
        # Neither suppression can hide a real problem here: the project is
        # Windows-only (so include-path case cannot matter) and C++17 (so C++98
        # compatibility is not a goal).
        target_compile_options(${_target} PRIVATE
            -Wshadow                    # a name that shadows another
            -Wunused-const-variable     # a constant nobody reads: how kTypeCount was caught
            -Wno-c++98-compat
            -Wno-nonportable-system-include-path)
    else()
        target_compile_options(${_target} PRIVATE /permissive-)
    endif()
    if(STEAMMOCK_WERROR)
        target_compile_options(${_target} PRIVATE /WX)
    endif()
endforeach()
