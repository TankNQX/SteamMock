cmake_minimum_required(VERSION 3.20)
project(SteamApiBridge VERSION 0.1.0 LANGUAGES CXX)

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
#    steambridge(.exe)      the backend that answers them
#    steambridge_codegen    regenerates the stub from gen/steam_api.idl.json
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
        "SteamApiBridge is a Windows harness built with an MSVC-style compiler (cl.exe or "
        "clang-cl): the stub is a DLL a game loads by name, and the backend's server answers "
        "it over loopback Winsock. See README.md.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(STEAMBRIDGE_STUB_NAME "steam_api64" CACHE STRING
    "Output name of the stub DLL. Use steam_api for a 32-bit game, or any name you like for a custom load")
option(STEAMBRIDGE_BUILD_TESTS "Build the C++ tests" ON)
option(STEAMBRIDGE_WERROR "Treat warnings as errors" OFF)

# ---------------------------------------------------------------------------
#  Core: the wire format, the JSON subset, the transport and the client. Static,
#  so the stub, the backend and the tests all share one copy.
# ---------------------------------------------------------------------------
add_library(steambridge_core STATIC
    src/json.cpp
    src/log.cpp
    src/transport_tcp.cpp
    src/client.cpp
)
target_include_directories(steambridge_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(steambridge_core PUBLIC cxx_std_17)

# Winsock: the loopback transport and the backend's server. Public, because
# everything that links the core ends up needing it.
target_link_libraries(steambridge_core PUBLIC ws2_32)

# ---------------------------------------------------------------------------
#  The backend: the session state machine, the scenario dispatcher and the
#  server. Linkable on its own so a front end - the console, or a future GUI -
#  is only a way of driving it.
# ---------------------------------------------------------------------------
add_library(steambridge_backend STATIC
    src/protocol.cpp
    src/session.cpp
    src/scenario.cpp
    src/server.cpp
    src/generated/api_surface.cpp
)
target_link_libraries(steambridge_backend PUBLIC steambridge_core)

# ---------------------------------------------------------------------------
#  The stub DLL: DllMain, the client bootstrap and the generated trampolines.
# ---------------------------------------------------------------------------
add_library(${STEAMBRIDGE_STUB_NAME} SHARED
    src/dllmain.cpp
    src/generated/api_stub.cpp
    src/generated/steam_api_exports.def
)
target_link_libraries(${STEAMBRIDGE_STUB_NAME} PRIVATE steambridge_core)
set_target_properties(${STEAMBRIDGE_STUB_NAME} PROPERTIES
    PREFIX ""                       # steam_api64.dll, not libsteam_api64.dll
    OUTPUT_NAME "${STEAMBRIDGE_STUB_NAME}"
    DEBUG_POSTFIX "d"
)
# One version, from the project() call at the top, so the DLL's
# SteamBridge_Version and the backend's --version cannot report different builds.
target_compile_definitions(${STEAMBRIDGE_STUB_NAME} PRIVATE
    STEAMBRIDGE_VERSION="${PROJECT_VERSION}")
# The exported names come straight from the generated .def, which CMake hands to
# the linker as a source: no decoration on x64, and the same undecorated names on
# Win32 that the real steam_api.dll uses.

# ---------------------------------------------------------------------------
#  The two programs: the backend a person runs, and the generator.
# ---------------------------------------------------------------------------
add_executable(steambridge src/backend_main.cpp)
target_link_libraries(steambridge PRIVATE steambridge_backend)
set_target_properties(steambridge PROPERTIES OUTPUT_NAME "steambridge")
target_compile_definitions(steambridge PRIVATE STEAMBRIDGE_VERSION="${PROJECT_VERSION}")

add_executable(steambridge_codegen src/codegen_main.cpp src/idl.cpp)
target_link_libraries(steambridge_codegen PRIVATE steambridge_core)

# ---------------------------------------------------------------------------
#  Tests
# ---------------------------------------------------------------------------
if(STEAMBRIDGE_BUILD_TESTS)
    enable_testing()

    # The JSON subset and the frame header, on the side that has to be exact.
    add_executable(test_protocol tests/test_protocol.cpp)
    target_link_libraries(test_protocol PRIVATE steambridge_core)
    add_test(NAME protocol COMMAND test_protocol)

    # The backend's own logic: replies, the session state machine, scenarios, and
    # the promise that the state machine only answers calls the IDL exports.
    add_executable(test_backend tests/test_backend.cpp)
    target_link_libraries(test_backend PRIVATE steambridge_backend)
    # The bundled scenario lives in the source tree, and a test binary runs from
    # the build tree, so it is handed over as an absolute path.
    target_compile_definitions(test_backend PRIVATE
        STEAMBRIDGE_SCENARIO_PATH="${CMAKE_CURRENT_SOURCE_DIR}/scenarios/example.json")
    add_test(NAME backend COMMAND test_backend)

    # The server itself: a real port, a real connection through the stub's own
    # transport, and the snapshots and summary a live view would draw.
    add_executable(test_server tests/test_server.cpp)
    target_link_libraries(test_server PRIVATE steambridge_backend)
    add_test(NAME server COMMAND test_server)

    # Generated files are committed, so this fails if the IDL was edited without
    # regenerating them. Run from the checkout, which is where the default --idl
    # path points.
    add_test(NAME generated_files_are_current
             COMMAND $<TARGET_FILE:steambridge_codegen> --check)
    set_tests_properties(generated_files_are_current PROPERTIES
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    # The fake game is what makes the end-to-end test an integration test: it
    # loads the built stub the way a game's import table would.
    add_executable(fake_game tests/fake_game.cpp)

    add_executable(test_end_to_end tests/test_end_to_end.cpp)
    target_link_libraries(test_end_to_end PRIVATE steambridge_core)
    add_test(NAME end_to_end
             COMMAND test_end_to_end
                     --server $<TARGET_FILE:steambridge>
                     --stub $<TARGET_FILE:${STEAMBRIDGE_STUB_NAME}>
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
set(STEAMBRIDGE_TARGETS
    steambridge_core
    steambridge_backend
    ${STEAMBRIDGE_STUB_NAME}
    steambridge
    steambridge_codegen
)
if(STEAMBRIDGE_BUILD_TESTS)
    list(APPEND STEAMBRIDGE_TARGETS test_protocol test_backend test_server fake_game test_end_to_end)
endif()

foreach(_target ${STEAMBRIDGE_TARGETS})
    target_compile_options(${_target} PRIVATE /W4 /Zc:__cplusplus)
    # getenv and fopen are the C spelling; the _s variants are MSVC-only and the
    # harness reads a handful of variables at startup.
    target_compile_definitions(${_target} PRIVATE _CRT_SECURE_NO_WARNINGS)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # /W4 is only roughly -Wall -Wextra, and -Wshadow is separate from both,
        # so name them: this leg should see exactly what the removed Linux leg
        # saw, and nothing less.
        target_compile_options(${_target} PRIVATE -Wall -Wextra -Wshadow -Wpedantic)
    else()
        target_compile_options(${_target} PRIVATE /permissive-)
    endif()
    if(STEAMBRIDGE_WERROR)
        target_compile_options(${_target} PRIVATE /WX)
    endif()
endforeach()
