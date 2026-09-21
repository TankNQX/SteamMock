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
# ---------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(STEAMBRIDGE_STUB_NAME "steam_api64" CACHE STRING
    "Output name of the stub DLL. Use steam_api for a 32-bit game, or any name you like for a custom load")
option(STEAMBRIDGE_BUILD_TESTS "Build the C++ tests" ON)
option(STEAMBRIDGE_WERROR "Treat warnings as errors" OFF)

find_package(Threads REQUIRED)

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

# Static libraries do not get -fPIC by default, and CMake does not hand the PIC
# flag of a shared target down to the static libraries it consumes. On ELF the
# stub DLL therefore fails to link with "relocation R_X86_64_PC32 ... can not be
# used when making a shared object; recompile with -fPIC" (the first offender is
# TcpTransport's vtable, which transport_tcp.cpp emits because it holds the key
# function). Ask for PIC here. MSVC needs none, and the flag is harmless for the
# test executables.
set_target_properties(steambridge_core PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(WIN32)
    # Winsock for the loopback transport.
    target_link_libraries(steambridge_core PUBLIC ws2_32)
endif()

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
target_link_libraries(steambridge_backend PUBLIC steambridge_core Threads::Threads)
set_target_properties(steambridge_backend PROPERTIES POSITION_INDEPENDENT_CODE ON)

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
# The exported names come straight from the generated .def, which CMake hands to
# the linker as a source: no decoration on x64, and the same undecorated names on
# Win32 that the real steam_api.dll uses.

# ---------------------------------------------------------------------------
#  The two programs: the backend a person runs, and the generator.
# ---------------------------------------------------------------------------
add_executable(steambridge src/backend_main.cpp)
target_link_libraries(steambridge PRIVATE steambridge_backend)
set_target_properties(steambridge PROPERTIES OUTPUT_NAME "steambridge")

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

    # Generated files are committed, so this fails if the IDL was edited without
    # regenerating them. Run from the checkout, which is where the default --idl
    # path points.
    add_test(NAME generated_files_are_current
             COMMAND $<TARGET_FILE:steambridge_codegen> --check)
    set_tests_properties(generated_files_are_current PROPERTIES
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    # The fake game turns the end-to-end test into an integration test: it loads
    # the built stub the way a game's import table would.
    if(WIN32)
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
    else()
        message(STATUS "SteamApiBridge: the end-to-end test loads the stub as a DLL, so it is Windows only")
    endif()
endif()

# ---------------------------------------------------------------------------
#  Warnings, in one place, for everything this project builds - the tests
#  included, because a test that nobody reads the warnings of is how a real
#  warning gets missed.
# ---------------------------------------------------------------------------
set(STEAMBRIDGE_TARGETS
    steambridge_core
    steambridge_backend
    ${STEAMBRIDGE_STUB_NAME}
    steambridge
    steambridge_codegen
)
if(STEAMBRIDGE_BUILD_TESTS)
    list(APPEND STEAMBRIDGE_TARGETS test_protocol test_backend)
    if(WIN32)
        list(APPEND STEAMBRIDGE_TARGETS fake_game test_end_to_end)
    endif()
endif()

foreach(_target ${STEAMBRIDGE_TARGETS})
    if(MSVC)
        target_compile_options(${_target} PRIVATE /W4 /permissive- /Zc:__cplusplus)
        # getenv and fopen are the portable spelling; the _s variants are MSVC
        # only, and the harness reads a handful of variables at startup.
        target_compile_definitions(${_target} PRIVATE _CRT_SECURE_NO_WARNINGS)
        if(STEAMBRIDGE_WERROR)
            target_compile_options(${_target} PRIVATE /WX)
        endif()
    else()
        target_compile_options(${_target} PRIVATE -Wall -Wextra -Wpedantic -Wshadow)
        if(STEAMBRIDGE_WERROR)
            target_compile_options(${_target} PRIVATE -Werror)
        endif()
    endif()
endforeach()
