cmake_minimum_required(VERSION 3.20)
project(SteamApiBridge VERSION 0.1.0 LANGUAGES CXX)

# ---------------------------------------------------------------------------
#  A local debugging harness for the Steamworks flat API.
#
#  The stub DLL is shaped like steam_api64.dll: it exports the same flat API
#  names and forwards every call over loopback TCP to a Python backend, which
#  decides the answer. Nothing here talks to Valve, and nothing here is meant to
#  ship inside a game - see README.md, "What this is not".
# ---------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(STEAMBRIDGE_STUB_NAME "steam_api64" CACHE STRING
    "Output name of the stub DLL. Use steam_api for a 32-bit game, or any name you like for a custom load")
option(STEAMBRIDGE_BUILD_TESTS "Build the C++ tests" ON)
option(STEAMBRIDGE_WERROR "Treat warnings as errors" OFF)

# ---------------------------------------------------------------------------
#  Core: protocol, transport, client. Static, so the tests and the DLL share it.
# ---------------------------------------------------------------------------
add_library(steambridge_core STATIC
    src/json.cpp
    src/log.cpp
    src/transport_tcp.cpp
    src/client.cpp
)
target_include_directories(steambridge_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(steambridge_core PUBLIC cxx_std_17)

if(WIN32)
    # Winsock for the loopback transport.
    target_link_libraries(steambridge_core PUBLIC ws2_32)
endif()

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

foreach(_target steambridge_core ${STEAMBRIDGE_STUB_NAME})
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

# ---------------------------------------------------------------------------
#  Tests
# ---------------------------------------------------------------------------
if(STEAMBRIDGE_BUILD_TESTS)
    enable_testing()

    add_executable(test_protocol tests/test_protocol.cpp)
    target_link_libraries(test_protocol PRIVATE steambridge_core)
    add_test(NAME protocol COMMAND test_protocol)

    # The fake game is what turns the end-to-end test into an integration test:
    # it loads the built stub the way a game's import table would.
    if(WIN32)
        add_executable(fake_game tests/fake_game.cpp)
    endif()

    find_package(Python3 COMPONENTS Interpreter QUIET)
    if(Python3_Interpreter_FOUND)
        add_test(NAME python_protocol
                 COMMAND ${Python3_EXECUTABLE} -m unittest discover
                         -s ${CMAKE_CURRENT_SOURCE_DIR}/python/tests -p "test_protocol.py")
        set_tests_properties(python_protocol PROPERTIES
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python)

        # Generated files are committed, so this fails if the IDL was edited
        # without regenerating them.
        add_test(NAME generated_files_are_current
                 COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/gen/generate.py --check)

        if(WIN32)
            # C++ stub <-> Python backend, over a real socket, driven by a real game.
            add_test(NAME python_end_to_end
                     COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/python/tests/test_e2e.py
                             --stub $<TARGET_FILE:${STEAMBRIDGE_STUB_NAME}>
                             --game $<TARGET_FILE:fake_game>
                             --scenario ${CMAKE_CURRENT_SOURCE_DIR}/python/steambridge/scenarios/example.json)
            set_tests_properties(python_end_to_end PROPERTIES
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python
                TIMEOUT 120)
        else()
            message(STATUS "SteamApiBridge: the end-to-end test loads the stub as a DLL, so it is Windows only")
        endif()
    else()
        message(STATUS "SteamApiBridge: no Python3 interpreter found - the Python tests are skipped")
    endif()
endif()
