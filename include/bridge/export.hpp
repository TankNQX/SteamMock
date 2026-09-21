#pragma once

// ---------------------------------------------------------------------------
//  Export and calling-convention macros for the stub DLL.
// ---------------------------------------------------------------------------
//  Games import the flat API by undecorated name (steam_api64.dll exports
//  SteamAPI_Init, not _SteamAPI_Init@0), which is why the stub also builds from
//  src/generated/steam_api_exports.def: the .def fixes the names regardless of
//  the calling convention, on x86 exactly as on x64.

// Windows only: the stub is a DLL, and a game loads it by name.
#define STEAMBRIDGE_EXPORT extern "C" __declspec(dllexport)
#define STEAMBRIDGE_CALL __cdecl
