// ============================================================================
//  GENERATED FILE - do not edit by hand.
//
//  Source:   gen/steam_interfaces.json (58 interface versions, 2115 slots)
//  Regenerate: steambridge_codegen
// ============================================================================

#include "bridge/synth.hpp"

#include <cstring>

namespace steambridge {
namespace {

// ---------------------------------------------------------------------------
//  The types the declarations name.
// ---------------------------------------------------------------------------
//  None of them comes from an SDK. The ABI is a size and a shape, so a value
//  class is the one integer the wire carries it as, and a structure that only
//  ever travels by value is its members. The assertions check both against the
//  sizes the layouts were imported with, so getting one wrong is a build error
//  rather than a call the game reads the wrong way.

#pragma pack(push, 4)
struct CGameID {
    std::uint64_t m_gameid;
};
static_assert(sizeof(CGameID) == 8, "CGameID has to be the size the wire carries");
struct CSteamID {
    std::uint64_t m_steamid;
};
static_assert(sizeof(CSteamID) == 8, "CSteamID has to be the size the wire carries");

struct ControllerAnalogActionData_t {
    std::int32_t eMode;
    float x;
    float y;
    bool bActive;
};
static_assert(sizeof(ControllerAnalogActionData_t) == 16, "ControllerAnalogActionData_t has to be the size the ABI passes");
struct ControllerDigitalActionData_t {
    bool bState;
    bool bActive;
};
static_assert(sizeof(ControllerDigitalActionData_t) == 2, "ControllerDigitalActionData_t has to be the size the ABI passes");
struct ControllerMotionData_t {
    float rotQuatX;
    float rotQuatY;
    float rotQuatZ;
    float rotQuatW;
    float posAccelX;
    float posAccelY;
    float posAccelZ;
    float rotVelX;
    float rotVelY;
    float rotVelZ;
};
static_assert(sizeof(ControllerMotionData_t) == 40, "ControllerMotionData_t has to be the size the ABI passes");
struct InputAnalogActionData_t {
    std::int32_t eMode;
    float x;
    float y;
    bool bActive;
};
static_assert(sizeof(InputAnalogActionData_t) == 16, "InputAnalogActionData_t has to be the size the ABI passes");
struct InputDigitalActionData_t {
    bool bState;
    bool bActive;
};
static_assert(sizeof(InputDigitalActionData_t) == 2, "InputDigitalActionData_t has to be the size the ABI passes");
struct InputMotionData_t {
    float rotQuatX;
    float rotQuatY;
    float rotQuatZ;
    float rotQuatW;
    float posAccelX;
    float posAccelY;
    float posAccelZ;
    float rotVelX;
    float rotVelY;
    float rotVelZ;
};
static_assert(sizeof(InputMotionData_t) == 40, "InputMotionData_t has to be the size the ABI passes");
struct SteamIPAddress_t {
    std::uint8_t m_rgubIPv6[16];
    std::int32_t m_eType;
};
static_assert(sizeof(SteamIPAddress_t) == 20, "SteamIPAddress_t has to be the size the ABI passes");
struct SteamPartyBeaconLocation_t {
    std::int32_t m_eType;
    std::uint64_t m_ulLocationID;
};
static_assert(sizeof(SteamPartyBeaconLocation_t) == 12, "SteamPartyBeaconLocation_t has to be the size the ABI passes");
#pragma pack(pop)

}  // namespace

// The kinds for the types above: a value class is the one integer the
// wire carries it as, and a structure the wire cannot carry is a value the
// backend is told about but cannot set - so the call is reported and the
// caller gets a default, which is what a game sees with Steam absent.

template <> struct Kind<CGameID> {
    static constexpr bool out() noexcept { return false; }

    static Json in(CGameID value) noexcept {
        return arg_uint(value.m_gameid);
    }
    static CGameID from(const Json& reply) noexcept {
        return CGameID{reply_uint(reply)};
    }
    static void store(CGameID* target, const Json& value) noexcept {
        if (target != nullptr) {
            target->m_gameid = value.as_uint64();
        }
    }
    static CGameID fallback() noexcept { return CGameID{0}; }
};

template <> struct Kind<CSteamID> {
    static constexpr bool out() noexcept { return false; }

    static Json in(CSteamID value) noexcept {
        return arg_uint(value.m_steamid);
    }
    static CSteamID from(const Json& reply) noexcept {
        return CSteamID{reply_uint(reply)};
    }
    static void store(CSteamID* target, const Json& value) noexcept {
        if (target != nullptr) {
            target->m_steamid = value.as_uint64();
        }
    }
    static CSteamID fallback() noexcept { return CSteamID{0}; }
};

template <> struct Kind<ControllerAnalogActionData_t> {
    static constexpr bool out() noexcept { return false; }

    static Json in(const ControllerAnalogActionData_t&) noexcept {
        return Json::null();
    }
    static ControllerAnalogActionData_t from(const Json&) noexcept {
        return ControllerAnalogActionData_t{};
    }
    static ControllerAnalogActionData_t fallback() noexcept { return ControllerAnalogActionData_t{}; }
};

template <> struct Kind<ControllerDigitalActionData_t> {
    static constexpr bool out() noexcept { return false; }

    static Json in(const ControllerDigitalActionData_t&) noexcept {
        return Json::null();
    }
    static ControllerDigitalActionData_t from(const Json&) noexcept {
        return ControllerDigitalActionData_t{};
    }
    static ControllerDigitalActionData_t fallback() noexcept { return ControllerDigitalActionData_t{}; }
};

template <> struct Kind<ControllerMotionData_t> {
    static constexpr bool out() noexcept { return false; }

    static Json in(const ControllerMotionData_t&) noexcept {
        return Json::null();
    }
    static ControllerMotionData_t from(const Json&) noexcept {
        return ControllerMotionData_t{};
    }
    static ControllerMotionData_t fallback() noexcept { return ControllerMotionData_t{}; }
};

template <> struct Kind<InputAnalogActionData_t> {
    static constexpr bool out() noexcept { return false; }

    static Json in(const InputAnalogActionData_t&) noexcept {
        return Json::null();
    }
    static InputAnalogActionData_t from(const Json&) noexcept {
        return InputAnalogActionData_t{};
    }
    static InputAnalogActionData_t fallback() noexcept { return InputAnalogActionData_t{}; }
};

template <> struct Kind<InputDigitalActionData_t> {
    static constexpr bool out() noexcept { return false; }

    static Json in(const InputDigitalActionData_t&) noexcept {
        return Json::null();
    }
    static InputDigitalActionData_t from(const Json&) noexcept {
        return InputDigitalActionData_t{};
    }
    static InputDigitalActionData_t fallback() noexcept { return InputDigitalActionData_t{}; }
};

template <> struct Kind<InputMotionData_t> {
    static constexpr bool out() noexcept { return false; }

    static Json in(const InputMotionData_t&) noexcept {
        return Json::null();
    }
    static InputMotionData_t from(const Json&) noexcept {
        return InputMotionData_t{};
    }
    static InputMotionData_t fallback() noexcept { return InputMotionData_t{}; }
};

template <> struct Kind<SteamIPAddress_t> {
    static constexpr bool out() noexcept { return false; }

    static Json in(const SteamIPAddress_t&) noexcept {
        return Json::null();
    }
    static SteamIPAddress_t from(const Json&) noexcept {
        return SteamIPAddress_t{};
    }
    static SteamIPAddress_t fallback() noexcept { return SteamIPAddress_t{}; }
};

template <> struct Kind<SteamPartyBeaconLocation_t> {
    static constexpr bool out() noexcept { return false; }

    static Json in(const SteamPartyBeaconLocation_t&) noexcept {
        return Json::null();
    }
    static SteamPartyBeaconLocation_t from(const Json&) noexcept {
        return SteamPartyBeaconLocation_t{};
    }
    static SteamPartyBeaconLocation_t fallback() noexcept { return SteamPartyBeaconLocation_t{}; }
};

namespace {

const char* const kParams_STEAMAPPLIST_INTERFACE_VERSION001_1[] = {
    "pvecAppID",
    "unMaxAppIDs",
};
const char* const kParams_STEAMAPPLIST_INTERFACE_VERSION001_2[] = {
    "nAppID",
    "pchName",
    "cchNameMax",
};
const char* const kParams_STEAMAPPLIST_INTERFACE_VERSION001_3[] = {
    "nAppID",
    "pchDirectory",
    "cchNameMax",
};
const char* const kParams_STEAMAPPLIST_INTERFACE_VERSION001_4[] = {
    "nAppID",
};
// ISteamAppList STEAMAPPLIST_INTERFACE_VERSION001
const steambridge::SlotInfo kSlots_STEAMAPPLIST_INTERFACE_VERSION001[] = {
    {"SteamAPI_ISteamAppList_GetNumInstalledApps", nullptr},
    {"SteamAPI_ISteamAppList_GetInstalledApps", kParams_STEAMAPPLIST_INTERFACE_VERSION001_1},
    {"SteamAPI_ISteamAppList_GetAppName", kParams_STEAMAPPLIST_INTERFACE_VERSION001_2},
    {"SteamAPI_ISteamAppList_GetAppInstallDir", kParams_STEAMAPPLIST_INTERFACE_VERSION001_3},
    {"SteamAPI_ISteamAppList_GetAppBuildId", kParams_STEAMAPPLIST_INTERFACE_VERSION001_4},
};

class Version_STEAMAPPLIST_INTERFACE_VERSION001 {
public:
    virtual std::uint32_t GetNumInstalledApps() {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMAPPLIST_INTERFACE_VERSION001[0]);
    }
    virtual std::uint32_t GetInstalledApps(std::uint32_t* pvecAppID, std::uint32_t unMaxAppIDs) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMAPPLIST_INTERFACE_VERSION001[1], pvecAppID, unMaxAppIDs);
    }
    virtual std::int32_t GetAppName(std::uint32_t nAppID, void* pchName, std::int32_t cchNameMax) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMAPPLIST_INTERFACE_VERSION001[2], nAppID, pchName, cchNameMax);
    }
    virtual std::int32_t GetAppInstallDir(std::uint32_t nAppID, void* pchDirectory, std::int32_t cchNameMax) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMAPPLIST_INTERFACE_VERSION001[3], nAppID, pchDirectory, cchNameMax);
    }
    virtual std::int32_t GetAppBuildId(std::uint32_t nAppID) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMAPPLIST_INTERFACE_VERSION001[4], nAppID);
    }
};

Version_STEAMAPPLIST_INTERFACE_VERSION001 g_STEAMAPPLIST_INTERFACE_VERSION001;

const char* const kParams_STEAMAPPS_INTERFACE_VERSION008_6[] = {
    "appID",
};
const char* const kParams_STEAMAPPS_INTERFACE_VERSION008_7[] = {
    "appID",
};
const char* const kParams_STEAMAPPS_INTERFACE_VERSION008_8[] = {
    "nAppID",
};
const char* const kParams_STEAMAPPS_INTERFACE_VERSION008_11[] = {
    "iDLC",
    "pAppID",
    "pbAvailable",
    "pchName",
    "cchNameBufferSize",
};
const char* const kParams_STEAMAPPS_INTERFACE_VERSION008_12[] = {
    "nAppID",
};
const char* const kParams_STEAMAPPS_INTERFACE_VERSION008_13[] = {
    "nAppID",
};
const char* const kParams_STEAMAPPS_INTERFACE_VERSION008_14[] = {
    "nAppID",
};
const char* const kParams_STEAMAPPS_INTERFACE_VERSION008_15[] = {
    "pchName",
    "cchNameBufferSize",
};
const char* const kParams_STEAMAPPS_INTERFACE_VERSION008_16[] = {
    "bMissingFilesOnly",
};
const char* const kParams_STEAMAPPS_INTERFACE_VERSION008_17[] = {
    "appID",
    "pvecDepots",
    "cMaxDepots",
};
const char* const kParams_STEAMAPPS_INTERFACE_VERSION008_18[] = {
    "appID",
    "pchFolder",
    "cchFolderBufferSize",
};
const char* const kParams_STEAMAPPS_INTERFACE_VERSION008_19[] = {
    "appID",
};
const char* const kParams_STEAMAPPS_INTERFACE_VERSION008_21[] = {
    "pchKey",
};
const char* const kParams_STEAMAPPS_INTERFACE_VERSION008_22[] = {
    "nAppID",
    "punBytesDownloaded",
    "punBytesTotal",
};
const char* const kParams_STEAMAPPS_INTERFACE_VERSION008_25[] = {
    "pszFileName",
};
const char* const kParams_STEAMAPPS_INTERFACE_VERSION008_26[] = {
    "pszCommandLine",
    "cubCommandLine",
};
const char* const kParams_STEAMAPPS_INTERFACE_VERSION008_28[] = {
    "punSecondsAllowed",
    "punSecondsPlayed",
};
const char* const kParams_STEAMAPPS_INTERFACE_VERSION008_29[] = {
    "nAppID",
};
// ISteamApps STEAMAPPS_INTERFACE_VERSION008
const steambridge::SlotInfo kSlots_STEAMAPPS_INTERFACE_VERSION008[] = {
    {"SteamAPI_ISteamApps_BIsSubscribed", nullptr},
    {"SteamAPI_ISteamApps_BIsLowViolence", nullptr},
    {"SteamAPI_ISteamApps_BIsCybercafe", nullptr},
    {"SteamAPI_ISteamApps_BIsVACBanned", nullptr},
    {"SteamAPI_ISteamApps_GetCurrentGameLanguage", nullptr},
    {"SteamAPI_ISteamApps_GetAvailableGameLanguages", nullptr},
    {"SteamAPI_ISteamApps_BIsSubscribedApp", kParams_STEAMAPPS_INTERFACE_VERSION008_6},
    {"SteamAPI_ISteamApps_BIsDlcInstalled", kParams_STEAMAPPS_INTERFACE_VERSION008_7},
    {"SteamAPI_ISteamApps_GetEarliestPurchaseUnixTime", kParams_STEAMAPPS_INTERFACE_VERSION008_8},
    {"SteamAPI_ISteamApps_BIsSubscribedFromFreeWeekend", nullptr},
    {"SteamAPI_ISteamApps_GetDLCCount", nullptr},
    {"SteamAPI_ISteamApps_BGetDLCDataByIndex", kParams_STEAMAPPS_INTERFACE_VERSION008_11},
    {"SteamAPI_ISteamApps_InstallDLC", kParams_STEAMAPPS_INTERFACE_VERSION008_12},
    {"SteamAPI_ISteamApps_UninstallDLC", kParams_STEAMAPPS_INTERFACE_VERSION008_13},
    {"SteamAPI_ISteamApps_RequestAppProofOfPurchaseKey", kParams_STEAMAPPS_INTERFACE_VERSION008_14},
    {"SteamAPI_ISteamApps_GetCurrentBetaName", kParams_STEAMAPPS_INTERFACE_VERSION008_15},
    {"SteamAPI_ISteamApps_MarkContentCorrupt", kParams_STEAMAPPS_INTERFACE_VERSION008_16},
    {"SteamAPI_ISteamApps_GetInstalledDepots", kParams_STEAMAPPS_INTERFACE_VERSION008_17},
    {"SteamAPI_ISteamApps_GetAppInstallDir", kParams_STEAMAPPS_INTERFACE_VERSION008_18},
    {"SteamAPI_ISteamApps_BIsAppInstalled", kParams_STEAMAPPS_INTERFACE_VERSION008_19},
    {"SteamAPI_ISteamApps_GetAppOwner", nullptr},
    {"SteamAPI_ISteamApps_GetLaunchQueryParam", kParams_STEAMAPPS_INTERFACE_VERSION008_21},
    {"SteamAPI_ISteamApps_GetDlcDownloadProgress", kParams_STEAMAPPS_INTERFACE_VERSION008_22},
    {"SteamAPI_ISteamApps_GetAppBuildId", nullptr},
    {"SteamAPI_ISteamApps_RequestAllProofOfPurchaseKeys", nullptr},
    {"SteamAPI_ISteamApps_GetFileDetails", kParams_STEAMAPPS_INTERFACE_VERSION008_25},
    {"SteamAPI_ISteamApps_GetLaunchCommandLine", kParams_STEAMAPPS_INTERFACE_VERSION008_26},
    {"SteamAPI_ISteamApps_BIsSubscribedFromFamilySharing", nullptr},
    {"SteamAPI_ISteamApps_BIsTimedTrial", kParams_STEAMAPPS_INTERFACE_VERSION008_28},
    {"SteamAPI_ISteamApps_SetDlcContext", kParams_STEAMAPPS_INTERFACE_VERSION008_29},
};

class Version_STEAMAPPS_INTERFACE_VERSION008 {
public:
    virtual bool BIsSubscribed() {
        return steambridge::slot<bool>(kSlots_STEAMAPPS_INTERFACE_VERSION008[0]);
    }
    virtual bool BIsLowViolence() {
        return steambridge::slot<bool>(kSlots_STEAMAPPS_INTERFACE_VERSION008[1]);
    }
    virtual bool BIsCybercafe() {
        return steambridge::slot<bool>(kSlots_STEAMAPPS_INTERFACE_VERSION008[2]);
    }
    virtual bool BIsVACBanned() {
        return steambridge::slot<bool>(kSlots_STEAMAPPS_INTERFACE_VERSION008[3]);
    }
    virtual const char* GetCurrentGameLanguage() {
        return steambridge::slot<const char*>(kSlots_STEAMAPPS_INTERFACE_VERSION008[4]);
    }
    virtual const char* GetAvailableGameLanguages() {
        return steambridge::slot<const char*>(kSlots_STEAMAPPS_INTERFACE_VERSION008[5]);
    }
    virtual bool BIsSubscribedApp(std::uint32_t appID) {
        return steambridge::slot<bool>(kSlots_STEAMAPPS_INTERFACE_VERSION008[6], appID);
    }
    virtual bool BIsDlcInstalled(std::uint32_t appID) {
        return steambridge::slot<bool>(kSlots_STEAMAPPS_INTERFACE_VERSION008[7], appID);
    }
    virtual std::uint32_t GetEarliestPurchaseUnixTime(std::uint32_t nAppID) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMAPPS_INTERFACE_VERSION008[8], nAppID);
    }
    virtual bool BIsSubscribedFromFreeWeekend() {
        return steambridge::slot<bool>(kSlots_STEAMAPPS_INTERFACE_VERSION008[9]);
    }
    virtual std::int32_t GetDLCCount() {
        return steambridge::slot<std::int32_t>(kSlots_STEAMAPPS_INTERFACE_VERSION008[10]);
    }
    virtual bool BGetDLCDataByIndex(std::int32_t iDLC, std::uint32_t* pAppID, bool* pbAvailable, void* pchName, std::int32_t cchNameBufferSize) {
        return steambridge::slot<bool>(kSlots_STEAMAPPS_INTERFACE_VERSION008[11], iDLC, pAppID, pbAvailable, pchName, cchNameBufferSize);
    }
    virtual void InstallDLC(std::uint32_t nAppID) {
        steambridge::slot<void>(kSlots_STEAMAPPS_INTERFACE_VERSION008[12], nAppID);
    }
    virtual void UninstallDLC(std::uint32_t nAppID) {
        steambridge::slot<void>(kSlots_STEAMAPPS_INTERFACE_VERSION008[13], nAppID);
    }
    virtual void RequestAppProofOfPurchaseKey(std::uint32_t nAppID) {
        steambridge::slot<void>(kSlots_STEAMAPPS_INTERFACE_VERSION008[14], nAppID);
    }
    virtual bool GetCurrentBetaName(void* pchName, std::int32_t cchNameBufferSize) {
        return steambridge::slot<bool>(kSlots_STEAMAPPS_INTERFACE_VERSION008[15], pchName, cchNameBufferSize);
    }
    virtual bool MarkContentCorrupt(bool bMissingFilesOnly) {
        return steambridge::slot<bool>(kSlots_STEAMAPPS_INTERFACE_VERSION008[16], bMissingFilesOnly);
    }
    virtual std::uint32_t GetInstalledDepots(std::uint32_t appID, std::uint32_t* pvecDepots, std::uint32_t cMaxDepots) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMAPPS_INTERFACE_VERSION008[17], appID, pvecDepots, cMaxDepots);
    }
    virtual std::uint32_t GetAppInstallDir(std::uint32_t appID, void* pchFolder, std::uint32_t cchFolderBufferSize) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMAPPS_INTERFACE_VERSION008[18], appID, pchFolder, cchFolderBufferSize);
    }
    virtual bool BIsAppInstalled(std::uint32_t appID) {
        return steambridge::slot<bool>(kSlots_STEAMAPPS_INTERFACE_VERSION008[19], appID);
    }
    virtual CSteamID GetAppOwner() {
        return steambridge::slot<CSteamID>(kSlots_STEAMAPPS_INTERFACE_VERSION008[20]);
    }
    virtual const char* GetLaunchQueryParam(const char* pchKey) {
        return steambridge::slot<const char*>(kSlots_STEAMAPPS_INTERFACE_VERSION008[21], pchKey);
    }
    virtual bool GetDlcDownloadProgress(std::uint32_t nAppID, std::uint64_t* punBytesDownloaded, std::uint64_t* punBytesTotal) {
        return steambridge::slot<bool>(kSlots_STEAMAPPS_INTERFACE_VERSION008[22], nAppID, punBytesDownloaded, punBytesTotal);
    }
    virtual std::int32_t GetAppBuildId() {
        return steambridge::slot<std::int32_t>(kSlots_STEAMAPPS_INTERFACE_VERSION008[23]);
    }
    virtual void RequestAllProofOfPurchaseKeys() {
        steambridge::slot<void>(kSlots_STEAMAPPS_INTERFACE_VERSION008[24]);
    }
    virtual std::uint64_t GetFileDetails(const char* pszFileName) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMAPPS_INTERFACE_VERSION008[25], pszFileName);
    }
    virtual std::int32_t GetLaunchCommandLine(void* pszCommandLine, std::int32_t cubCommandLine) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMAPPS_INTERFACE_VERSION008[26], pszCommandLine, cubCommandLine);
    }
    virtual bool BIsSubscribedFromFamilySharing() {
        return steambridge::slot<bool>(kSlots_STEAMAPPS_INTERFACE_VERSION008[27]);
    }
    virtual bool BIsTimedTrial(std::uint32_t* punSecondsAllowed, std::uint32_t* punSecondsPlayed) {
        return steambridge::slot<bool>(kSlots_STEAMAPPS_INTERFACE_VERSION008[28], punSecondsAllowed, punSecondsPlayed);
    }
    virtual bool SetDlcContext(std::uint32_t nAppID) {
        return steambridge::slot<bool>(kSlots_STEAMAPPS_INTERFACE_VERSION008[29], nAppID);
    }
};

Version_STEAMAPPS_INTERFACE_VERSION008 g_STEAMAPPS_INTERFACE_VERSION008;

const char* const kParams_SteamClient017_1[] = {
    "hSteamPipe",
};
const char* const kParams_SteamClient017_2[] = {
    "hSteamPipe",
};
const char* const kParams_SteamClient017_3[] = {
    "phSteamPipe",
    "eAccountType",
};
const char* const kParams_SteamClient017_4[] = {
    "hSteamPipe",
    "hUser",
};
const char* const kParams_SteamClient017_5[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_6[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_7[] = {
    "unIP",
    "usPort",
};
const char* const kParams_SteamClient017_8[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_9[] = {
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_10[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_11[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_12[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_13[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_14[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_15[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_16[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_17[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_18[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_21[] = {
    "pFunction",
};
const char* const kParams_SteamClient017_23[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_24[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_25[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_26[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_27[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_28[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_29[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_30[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_31[] = {
    "arg0",
};
const char* const kParams_SteamClient017_32[] = {
    "arg0",
};
const char* const kParams_SteamClient017_33[] = {
    "func",
};
const char* const kParams_SteamClient017_34[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_35[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient017_36[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
// ISteamClient SteamClient017
const steambridge::SlotInfo kSlots_SteamClient017[] = {
    {"SteamAPI_ISteamClient_CreateSteamPipe", nullptr},
    {"SteamAPI_ISteamClient_BReleaseSteamPipe", kParams_SteamClient017_1},
    {"SteamAPI_ISteamClient_ConnectToGlobalUser", kParams_SteamClient017_2},
    {"SteamAPI_ISteamClient_CreateLocalUser", kParams_SteamClient017_3},
    {"SteamAPI_ISteamClient_ReleaseUser", kParams_SteamClient017_4},
    {"SteamAPI_ISteamClient_GetISteamUser", kParams_SteamClient017_5},
    {"SteamAPI_ISteamClient_GetISteamGameServer", kParams_SteamClient017_6},
    {"SteamAPI_ISteamClient_SetLocalIPBinding", kParams_SteamClient017_7},
    {"SteamAPI_ISteamClient_GetISteamFriends", kParams_SteamClient017_8},
    {"SteamAPI_ISteamClient_GetISteamUtils", kParams_SteamClient017_9},
    {"SteamAPI_ISteamClient_GetISteamMatchmaking", kParams_SteamClient017_10},
    {"SteamAPI_ISteamClient_GetISteamMatchmakingServers", kParams_SteamClient017_11},
    {"SteamAPI_ISteamClient_GetISteamGenericInterface", kParams_SteamClient017_12},
    {"SteamAPI_ISteamClient_GetISteamUserStats", kParams_SteamClient017_13},
    {"SteamAPI_ISteamClient_GetISteamGameServerStats", kParams_SteamClient017_14},
    {"SteamAPI_ISteamClient_GetISteamApps", kParams_SteamClient017_15},
    {"SteamAPI_ISteamClient_GetISteamNetworking", kParams_SteamClient017_16},
    {"SteamAPI_ISteamClient_GetISteamRemoteStorage", kParams_SteamClient017_17},
    {"SteamAPI_ISteamClient_GetISteamScreenshots", kParams_SteamClient017_18},
    {"ISteamClient::RunFrame", nullptr},
    {"SteamAPI_ISteamClient_GetIPCCallCount", nullptr},
    {"SteamAPI_ISteamClient_SetWarningMessageHook", kParams_SteamClient017_21},
    {"SteamAPI_ISteamClient_BShutdownIfAllPipesClosed", nullptr},
    {"SteamAPI_ISteamClient_GetISteamHTTP", kParams_SteamClient017_23},
    {"SteamAPI_ISteamClient_GetISteamUnifiedMessages", kParams_SteamClient017_24},
    {"SteamAPI_ISteamClient_GetISteamController", kParams_SteamClient017_25},
    {"SteamAPI_ISteamClient_GetISteamUGC", kParams_SteamClient017_26},
    {"SteamAPI_ISteamClient_GetISteamAppList", kParams_SteamClient017_27},
    {"SteamAPI_ISteamClient_GetISteamMusic", kParams_SteamClient017_28},
    {"SteamAPI_ISteamClient_GetISteamMusicRemote", kParams_SteamClient017_29},
    {"SteamAPI_ISteamClient_GetISteamHTMLSurface", kParams_SteamClient017_30},
    {"ISteamClient::DEPRECATED_Set_SteamAPI_CPostAPIResultInProcess", kParams_SteamClient017_31},
    {"ISteamClient::DEPRECATED_Remove_SteamAPI_CPostAPIResultInProcess", kParams_SteamClient017_32},
    {"ISteamClient::Set_SteamAPI_CCheckCallbackRegisteredInProcess", kParams_SteamClient017_33},
    {"SteamAPI_ISteamClient_GetISteamInventory", kParams_SteamClient017_34},
    {"SteamAPI_ISteamClient_GetISteamVideo", kParams_SteamClient017_35},
    {"SteamAPI_ISteamClient_GetISteamParentalSettings", kParams_SteamClient017_36},
};

class Version_SteamClient017 {
public:
    virtual std::int32_t CreateSteamPipe() {
        return steambridge::slot<std::int32_t>(kSlots_SteamClient017[0]);
    }
    virtual bool BReleaseSteamPipe(std::int32_t hSteamPipe) {
        return steambridge::slot<bool>(kSlots_SteamClient017[1], hSteamPipe);
    }
    virtual std::int32_t ConnectToGlobalUser(std::int32_t hSteamPipe) {
        return steambridge::slot<std::int32_t>(kSlots_SteamClient017[2], hSteamPipe);
    }
    virtual std::int32_t CreateLocalUser(std::int32_t* phSteamPipe, std::int32_t eAccountType) {
        return steambridge::slot<std::int32_t>(kSlots_SteamClient017[3], phSteamPipe, eAccountType);
    }
    virtual void ReleaseUser(std::int32_t hSteamPipe, std::int32_t hUser) {
        steambridge::slot<void>(kSlots_SteamClient017[4], hSteamPipe, hUser);
    }
    virtual void* GetISteamUser(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[5], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGameServer(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[6], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void SetLocalIPBinding(std::uint32_t unIP, std::uint16_t usPort) {
        steambridge::slot<void>(kSlots_SteamClient017[7], unIP, usPort);
    }
    virtual void* GetISteamFriends(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[8], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamUtils(std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[9], hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMatchmaking(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[10], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMatchmakingServers(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[11], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGenericInterface(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[12], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamUserStats(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[13], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGameServerStats(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[14], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamApps(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[15], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamNetworking(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[16], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamRemoteStorage(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[17], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamScreenshots(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[18], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void RunFrame() {
        steambridge::slot<void>(kSlots_SteamClient017[19]);
    }
    virtual std::uint32_t GetIPCCallCount() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamClient017[20]);
    }
    virtual void SetWarningMessageHook(void* pFunction) {
        steambridge::slot<void>(kSlots_SteamClient017[21], pFunction);
    }
    virtual bool BShutdownIfAllPipesClosed() {
        return steambridge::slot<bool>(kSlots_SteamClient017[22]);
    }
    virtual void* GetISteamHTTP(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[23], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamUnifiedMessages(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[24], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamController(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[25], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamUGC(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[26], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamAppList(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[27], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMusic(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[28], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMusicRemote(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[29], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamHTMLSurface(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[30], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void DEPRECATED_Set_SteamAPI_CPostAPIResultInProcess(void* arg0) {
        steambridge::slot<void>(kSlots_SteamClient017[31], arg0);
    }
    virtual void DEPRECATED_Remove_SteamAPI_CPostAPIResultInProcess(void* arg0) {
        steambridge::slot<void>(kSlots_SteamClient017[32], arg0);
    }
    virtual void Set_SteamAPI_CCheckCallbackRegisteredInProcess(void* func) {
        steambridge::slot<void>(kSlots_SteamClient017[33], func);
    }
    virtual void* GetISteamInventory(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[34], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamVideo(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[35], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamParentalSettings(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient017[36], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
};

Version_SteamClient017 g_SteamClient017;

const char* const kParams_SteamClient019_1[] = {
    "hSteamPipe",
};
const char* const kParams_SteamClient019_2[] = {
    "hSteamPipe",
};
const char* const kParams_SteamClient019_3[] = {
    "phSteamPipe",
    "eAccountType",
};
const char* const kParams_SteamClient019_4[] = {
    "hSteamPipe",
    "hUser",
};
const char* const kParams_SteamClient019_5[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_6[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_7[] = {
    "unIP",
    "usPort",
};
const char* const kParams_SteamClient019_8[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_9[] = {
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_10[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_11[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_12[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_13[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_14[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_15[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_16[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_17[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_18[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_19[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_22[] = {
    "pFunction",
};
const char* const kParams_SteamClient019_24[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_25[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_26[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_27[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_28[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_29[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_30[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_31[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_32[] = {
    "arg0",
};
const char* const kParams_SteamClient019_33[] = {
    "arg0",
};
const char* const kParams_SteamClient019_34[] = {
    "func",
};
const char* const kParams_SteamClient019_35[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_36[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_37[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_38[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_39[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient019_40[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
// ISteamClient SteamClient019
const steambridge::SlotInfo kSlots_SteamClient019[] = {
    {"SteamAPI_ISteamClient_CreateSteamPipe", nullptr},
    {"SteamAPI_ISteamClient_BReleaseSteamPipe", kParams_SteamClient019_1},
    {"SteamAPI_ISteamClient_ConnectToGlobalUser", kParams_SteamClient019_2},
    {"SteamAPI_ISteamClient_CreateLocalUser", kParams_SteamClient019_3},
    {"SteamAPI_ISteamClient_ReleaseUser", kParams_SteamClient019_4},
    {"SteamAPI_ISteamClient_GetISteamUser", kParams_SteamClient019_5},
    {"SteamAPI_ISteamClient_GetISteamGameServer", kParams_SteamClient019_6},
    {"SteamAPI_ISteamClient_SetLocalIPBinding", kParams_SteamClient019_7},
    {"SteamAPI_ISteamClient_GetISteamFriends", kParams_SteamClient019_8},
    {"SteamAPI_ISteamClient_GetISteamUtils", kParams_SteamClient019_9},
    {"SteamAPI_ISteamClient_GetISteamMatchmaking", kParams_SteamClient019_10},
    {"SteamAPI_ISteamClient_GetISteamMatchmakingServers", kParams_SteamClient019_11},
    {"SteamAPI_ISteamClient_GetISteamGenericInterface", kParams_SteamClient019_12},
    {"SteamAPI_ISteamClient_GetISteamUserStats", kParams_SteamClient019_13},
    {"SteamAPI_ISteamClient_GetISteamGameServerStats", kParams_SteamClient019_14},
    {"SteamAPI_ISteamClient_GetISteamApps", kParams_SteamClient019_15},
    {"SteamAPI_ISteamClient_GetISteamNetworking", kParams_SteamClient019_16},
    {"SteamAPI_ISteamClient_GetISteamRemoteStorage", kParams_SteamClient019_17},
    {"SteamAPI_ISteamClient_GetISteamScreenshots", kParams_SteamClient019_18},
    {"SteamAPI_ISteamClient_GetISteamGameSearch", kParams_SteamClient019_19},
    {"ISteamClient::RunFrame", nullptr},
    {"SteamAPI_ISteamClient_GetIPCCallCount", nullptr},
    {"SteamAPI_ISteamClient_SetWarningMessageHook", kParams_SteamClient019_22},
    {"SteamAPI_ISteamClient_BShutdownIfAllPipesClosed", nullptr},
    {"SteamAPI_ISteamClient_GetISteamHTTP", kParams_SteamClient019_24},
    {"ISteamClient::DEPRECATED_GetISteamUnifiedMessages", kParams_SteamClient019_25},
    {"SteamAPI_ISteamClient_GetISteamController", kParams_SteamClient019_26},
    {"SteamAPI_ISteamClient_GetISteamUGC", kParams_SteamClient019_27},
    {"SteamAPI_ISteamClient_GetISteamAppList", kParams_SteamClient019_28},
    {"SteamAPI_ISteamClient_GetISteamMusic", kParams_SteamClient019_29},
    {"SteamAPI_ISteamClient_GetISteamMusicRemote", kParams_SteamClient019_30},
    {"SteamAPI_ISteamClient_GetISteamHTMLSurface", kParams_SteamClient019_31},
    {"ISteamClient::DEPRECATED_Set_SteamAPI_CPostAPIResultInProcess", kParams_SteamClient019_32},
    {"ISteamClient::DEPRECATED_Remove_SteamAPI_CPostAPIResultInProcess", kParams_SteamClient019_33},
    {"ISteamClient::Set_SteamAPI_CCheckCallbackRegisteredInProcess", kParams_SteamClient019_34},
    {"SteamAPI_ISteamClient_GetISteamInventory", kParams_SteamClient019_35},
    {"SteamAPI_ISteamClient_GetISteamVideo", kParams_SteamClient019_36},
    {"SteamAPI_ISteamClient_GetISteamParentalSettings", kParams_SteamClient019_37},
    {"SteamAPI_ISteamClient_GetISteamInput", kParams_SteamClient019_38},
    {"SteamAPI_ISteamClient_GetISteamParties", kParams_SteamClient019_39},
    {"SteamAPI_ISteamClient_GetISteamRemotePlay", kParams_SteamClient019_40},
};

class Version_SteamClient019 {
public:
    virtual std::int32_t CreateSteamPipe() {
        return steambridge::slot<std::int32_t>(kSlots_SteamClient019[0]);
    }
    virtual bool BReleaseSteamPipe(std::int32_t hSteamPipe) {
        return steambridge::slot<bool>(kSlots_SteamClient019[1], hSteamPipe);
    }
    virtual std::int32_t ConnectToGlobalUser(std::int32_t hSteamPipe) {
        return steambridge::slot<std::int32_t>(kSlots_SteamClient019[2], hSteamPipe);
    }
    virtual std::int32_t CreateLocalUser(std::int32_t* phSteamPipe, std::int32_t eAccountType) {
        return steambridge::slot<std::int32_t>(kSlots_SteamClient019[3], phSteamPipe, eAccountType);
    }
    virtual void ReleaseUser(std::int32_t hSteamPipe, std::int32_t hUser) {
        steambridge::slot<void>(kSlots_SteamClient019[4], hSteamPipe, hUser);
    }
    virtual void* GetISteamUser(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[5], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGameServer(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[6], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void SetLocalIPBinding(std::uint32_t unIP, std::uint16_t usPort) {
        steambridge::slot<void>(kSlots_SteamClient019[7], unIP, usPort);
    }
    virtual void* GetISteamFriends(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[8], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamUtils(std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[9], hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMatchmaking(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[10], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMatchmakingServers(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[11], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGenericInterface(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[12], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamUserStats(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[13], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGameServerStats(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[14], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamApps(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[15], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamNetworking(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[16], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamRemoteStorage(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[17], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamScreenshots(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[18], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGameSearch(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[19], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void RunFrame() {
        steambridge::slot<void>(kSlots_SteamClient019[20]);
    }
    virtual std::uint32_t GetIPCCallCount() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamClient019[21]);
    }
    virtual void SetWarningMessageHook(void* pFunction) {
        steambridge::slot<void>(kSlots_SteamClient019[22], pFunction);
    }
    virtual bool BShutdownIfAllPipesClosed() {
        return steambridge::slot<bool>(kSlots_SteamClient019[23]);
    }
    virtual void* GetISteamHTTP(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[24], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* DEPRECATED_GetISteamUnifiedMessages(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[25], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamController(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[26], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamUGC(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[27], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamAppList(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[28], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMusic(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[29], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMusicRemote(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[30], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamHTMLSurface(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[31], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void DEPRECATED_Set_SteamAPI_CPostAPIResultInProcess(void* arg0) {
        steambridge::slot<void>(kSlots_SteamClient019[32], arg0);
    }
    virtual void DEPRECATED_Remove_SteamAPI_CPostAPIResultInProcess(void* arg0) {
        steambridge::slot<void>(kSlots_SteamClient019[33], arg0);
    }
    virtual void Set_SteamAPI_CCheckCallbackRegisteredInProcess(void* func) {
        steambridge::slot<void>(kSlots_SteamClient019[34], func);
    }
    virtual void* GetISteamInventory(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[35], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamVideo(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[36], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamParentalSettings(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[37], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamInput(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[38], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamParties(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[39], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamRemotePlay(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient019[40], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
};

Version_SteamClient019 g_SteamClient019;

const char* const kParams_SteamClient020_1[] = {
    "hSteamPipe",
};
const char* const kParams_SteamClient020_2[] = {
    "hSteamPipe",
};
const char* const kParams_SteamClient020_3[] = {
    "phSteamPipe",
    "eAccountType",
};
const char* const kParams_SteamClient020_4[] = {
    "hSteamPipe",
    "hUser",
};
const char* const kParams_SteamClient020_5[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_6[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_7[] = {
    "unIP",
    "usPort",
};
const char* const kParams_SteamClient020_8[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_9[] = {
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_10[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_11[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_12[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_13[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_14[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_15[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_16[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_17[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_18[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_19[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_22[] = {
    "pFunction",
};
const char* const kParams_SteamClient020_24[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_25[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_26[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_27[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_28[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_29[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_30[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_31[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_32[] = {
    "arg0",
};
const char* const kParams_SteamClient020_33[] = {
    "arg0",
};
const char* const kParams_SteamClient020_34[] = {
    "func",
};
const char* const kParams_SteamClient020_35[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_36[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_37[] = {
    "hSteamuser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_38[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_39[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
const char* const kParams_SteamClient020_40[] = {
    "hSteamUser",
    "hSteamPipe",
    "pchVersion",
};
// ISteamClient SteamClient020
const steambridge::SlotInfo kSlots_SteamClient020[] = {
    {"SteamAPI_ISteamClient_CreateSteamPipe", nullptr},
    {"SteamAPI_ISteamClient_BReleaseSteamPipe", kParams_SteamClient020_1},
    {"SteamAPI_ISteamClient_ConnectToGlobalUser", kParams_SteamClient020_2},
    {"SteamAPI_ISteamClient_CreateLocalUser", kParams_SteamClient020_3},
    {"SteamAPI_ISteamClient_ReleaseUser", kParams_SteamClient020_4},
    {"SteamAPI_ISteamClient_GetISteamUser", kParams_SteamClient020_5},
    {"SteamAPI_ISteamClient_GetISteamGameServer", kParams_SteamClient020_6},
    {"SteamAPI_ISteamClient_SetLocalIPBinding", kParams_SteamClient020_7},
    {"SteamAPI_ISteamClient_GetISteamFriends", kParams_SteamClient020_8},
    {"SteamAPI_ISteamClient_GetISteamUtils", kParams_SteamClient020_9},
    {"SteamAPI_ISteamClient_GetISteamMatchmaking", kParams_SteamClient020_10},
    {"SteamAPI_ISteamClient_GetISteamMatchmakingServers", kParams_SteamClient020_11},
    {"SteamAPI_ISteamClient_GetISteamGenericInterface", kParams_SteamClient020_12},
    {"SteamAPI_ISteamClient_GetISteamUserStats", kParams_SteamClient020_13},
    {"SteamAPI_ISteamClient_GetISteamGameServerStats", kParams_SteamClient020_14},
    {"SteamAPI_ISteamClient_GetISteamApps", kParams_SteamClient020_15},
    {"SteamAPI_ISteamClient_GetISteamNetworking", kParams_SteamClient020_16},
    {"SteamAPI_ISteamClient_GetISteamRemoteStorage", kParams_SteamClient020_17},
    {"SteamAPI_ISteamClient_GetISteamScreenshots", kParams_SteamClient020_18},
    {"SteamAPI_ISteamClient_GetISteamGameSearch", kParams_SteamClient020_19},
    {"ISteamClient::RunFrame", nullptr},
    {"SteamAPI_ISteamClient_GetIPCCallCount", nullptr},
    {"SteamAPI_ISteamClient_SetWarningMessageHook", kParams_SteamClient020_22},
    {"SteamAPI_ISteamClient_BShutdownIfAllPipesClosed", nullptr},
    {"SteamAPI_ISteamClient_GetISteamHTTP", kParams_SteamClient020_24},
    {"ISteamClient::DEPRECATED_GetISteamUnifiedMessages", kParams_SteamClient020_25},
    {"SteamAPI_ISteamClient_GetISteamController", kParams_SteamClient020_26},
    {"SteamAPI_ISteamClient_GetISteamUGC", kParams_SteamClient020_27},
    {"SteamAPI_ISteamClient_GetISteamAppList", kParams_SteamClient020_28},
    {"SteamAPI_ISteamClient_GetISteamMusic", kParams_SteamClient020_29},
    {"SteamAPI_ISteamClient_GetISteamMusicRemote", kParams_SteamClient020_30},
    {"SteamAPI_ISteamClient_GetISteamHTMLSurface", kParams_SteamClient020_31},
    {"ISteamClient::DEPRECATED_Set_SteamAPI_CPostAPIResultInProcess", kParams_SteamClient020_32},
    {"ISteamClient::DEPRECATED_Remove_SteamAPI_CPostAPIResultInProcess", kParams_SteamClient020_33},
    {"ISteamClient::Set_SteamAPI_CCheckCallbackRegisteredInProcess", kParams_SteamClient020_34},
    {"SteamAPI_ISteamClient_GetISteamInventory", kParams_SteamClient020_35},
    {"SteamAPI_ISteamClient_GetISteamVideo", kParams_SteamClient020_36},
    {"SteamAPI_ISteamClient_GetISteamParentalSettings", kParams_SteamClient020_37},
    {"SteamAPI_ISteamClient_GetISteamInput", kParams_SteamClient020_38},
    {"SteamAPI_ISteamClient_GetISteamParties", kParams_SteamClient020_39},
    {"SteamAPI_ISteamClient_GetISteamRemotePlay", kParams_SteamClient020_40},
    {"ISteamClient::DestroyAllInterfaces", nullptr},
};

class Version_SteamClient020 {
public:
    virtual std::int32_t CreateSteamPipe() {
        return steambridge::slot<std::int32_t>(kSlots_SteamClient020[0]);
    }
    virtual bool BReleaseSteamPipe(std::int32_t hSteamPipe) {
        return steambridge::slot<bool>(kSlots_SteamClient020[1], hSteamPipe);
    }
    virtual std::int32_t ConnectToGlobalUser(std::int32_t hSteamPipe) {
        return steambridge::slot<std::int32_t>(kSlots_SteamClient020[2], hSteamPipe);
    }
    virtual std::int32_t CreateLocalUser(std::int32_t* phSteamPipe, std::int32_t eAccountType) {
        return steambridge::slot<std::int32_t>(kSlots_SteamClient020[3], phSteamPipe, eAccountType);
    }
    virtual void ReleaseUser(std::int32_t hSteamPipe, std::int32_t hUser) {
        steambridge::slot<void>(kSlots_SteamClient020[4], hSteamPipe, hUser);
    }
    virtual void* GetISteamUser(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[5], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGameServer(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[6], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void SetLocalIPBinding(void* unIP, std::uint16_t usPort) {
        steambridge::slot<void>(kSlots_SteamClient020[7], unIP, usPort);
    }
    virtual void* GetISteamFriends(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[8], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamUtils(std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[9], hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMatchmaking(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[10], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMatchmakingServers(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[11], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGenericInterface(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[12], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamUserStats(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[13], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGameServerStats(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[14], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamApps(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[15], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamNetworking(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[16], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamRemoteStorage(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[17], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamScreenshots(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[18], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGameSearch(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[19], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void RunFrame() {
        steambridge::slot<void>(kSlots_SteamClient020[20]);
    }
    virtual std::uint32_t GetIPCCallCount() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamClient020[21]);
    }
    virtual void SetWarningMessageHook(void* pFunction) {
        steambridge::slot<void>(kSlots_SteamClient020[22], pFunction);
    }
    virtual bool BShutdownIfAllPipesClosed() {
        return steambridge::slot<bool>(kSlots_SteamClient020[23]);
    }
    virtual void* GetISteamHTTP(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[24], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* DEPRECATED_GetISteamUnifiedMessages(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[25], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamController(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[26], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamUGC(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[27], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamAppList(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[28], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMusic(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[29], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMusicRemote(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[30], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamHTMLSurface(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[31], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void DEPRECATED_Set_SteamAPI_CPostAPIResultInProcess(void* arg0) {
        steambridge::slot<void>(kSlots_SteamClient020[32], arg0);
    }
    virtual void DEPRECATED_Remove_SteamAPI_CPostAPIResultInProcess(void* arg0) {
        steambridge::slot<void>(kSlots_SteamClient020[33], arg0);
    }
    virtual void Set_SteamAPI_CCheckCallbackRegisteredInProcess(void* func) {
        steambridge::slot<void>(kSlots_SteamClient020[34], func);
    }
    virtual void* GetISteamInventory(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[35], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamVideo(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[36], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamParentalSettings(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[37], hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamInput(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[38], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamParties(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[39], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamRemotePlay(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steambridge::slot<void*>(kSlots_SteamClient020[40], hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steambridge::interface_object(pchVersion);
        }
        return result;
    }
    virtual void DestroyAllInterfaces() {
        steambridge::slot<void>(kSlots_SteamClient020[41]);
    }
};

Version_SteamClient020 g_SteamClient020;

const char* const kParams_SteamController005_3[] = {
    "handlesOut",
};
const char* const kParams_SteamController005_4[] = {
    "controllerHandle",
};
const char* const kParams_SteamController005_5[] = {
    "pszActionSetName",
};
const char* const kParams_SteamController005_6[] = {
    "controllerHandle",
    "actionSetHandle",
};
const char* const kParams_SteamController005_7[] = {
    "controllerHandle",
};
const char* const kParams_SteamController005_8[] = {
    "pszActionName",
};
const char* const kParams_SteamController005_9[] = {
    "controllerHandle",
    "digitalActionHandle",
};
const char* const kParams_SteamController005_10[] = {
    "controllerHandle",
    "actionSetHandle",
    "digitalActionHandle",
    "originsOut",
};
const char* const kParams_SteamController005_11[] = {
    "pszActionName",
};
const char* const kParams_SteamController005_12[] = {
    "controllerHandle",
    "analogActionHandle",
};
const char* const kParams_SteamController005_13[] = {
    "controllerHandle",
    "actionSetHandle",
    "analogActionHandle",
    "originsOut",
};
const char* const kParams_SteamController005_14[] = {
    "controllerHandle",
    "eAction",
};
const char* const kParams_SteamController005_15[] = {
    "controllerHandle",
    "eTargetPad",
    "usDurationMicroSec",
};
const char* const kParams_SteamController005_16[] = {
    "controllerHandle",
    "eTargetPad",
    "usDurationMicroSec",
    "usOffMicroSec",
    "unRepeat",
    "nFlags",
};
const char* const kParams_SteamController005_17[] = {
    "controllerHandle",
    "usLeftSpeed",
    "usRightSpeed",
};
const char* const kParams_SteamController005_18[] = {
    "controllerHandle",
    "nColorR",
    "nColorG",
    "nColorB",
    "nFlags",
};
const char* const kParams_SteamController005_19[] = {
    "ulControllerHandle",
};
const char* const kParams_SteamController005_20[] = {
    "nIndex",
};
const char* const kParams_SteamController005_21[] = {
    "controllerHandle",
};
const char* const kParams_SteamController005_22[] = {
    "controllerHandle",
    "digitalActionHandle",
    "flScale",
    "flXPosition",
    "flYPosition",
};
const char* const kParams_SteamController005_23[] = {
    "controllerHandle",
    "analogActionHandle",
    "flScale",
    "flXPosition",
    "flYPosition",
};
const char* const kParams_SteamController005_24[] = {
    "eOrigin",
};
const char* const kParams_SteamController005_25[] = {
    "eOrigin",
};
// ISteamController SteamController005
const steambridge::SlotInfo kSlots_SteamController005[] = {
    {"SteamAPI_ISteamController_Init", nullptr},
    {"SteamAPI_ISteamController_Shutdown", nullptr},
    {"SteamAPI_ISteamController_RunFrame", nullptr},
    {"SteamAPI_ISteamController_GetConnectedControllers", kParams_SteamController005_3},
    {"SteamAPI_ISteamController_ShowBindingPanel", kParams_SteamController005_4},
    {"SteamAPI_ISteamController_GetActionSetHandle", kParams_SteamController005_5},
    {"SteamAPI_ISteamController_ActivateActionSet", kParams_SteamController005_6},
    {"SteamAPI_ISteamController_GetCurrentActionSet", kParams_SteamController005_7},
    {"SteamAPI_ISteamController_GetDigitalActionHandle", kParams_SteamController005_8},
    {"SteamAPI_ISteamController_GetDigitalActionData", kParams_SteamController005_9},
    {"SteamAPI_ISteamController_GetDigitalActionOrigins", kParams_SteamController005_10},
    {"SteamAPI_ISteamController_GetAnalogActionHandle", kParams_SteamController005_11},
    {"SteamAPI_ISteamController_GetAnalogActionData", kParams_SteamController005_12},
    {"SteamAPI_ISteamController_GetAnalogActionOrigins", kParams_SteamController005_13},
    {"SteamAPI_ISteamController_StopAnalogActionMomentum", kParams_SteamController005_14},
    {"SteamAPI_ISteamController_TriggerHapticPulse", kParams_SteamController005_15},
    {"SteamAPI_ISteamController_TriggerRepeatedHapticPulse", kParams_SteamController005_16},
    {"SteamAPI_ISteamController_TriggerVibration", kParams_SteamController005_17},
    {"SteamAPI_ISteamController_SetLEDColor", kParams_SteamController005_18},
    {"SteamAPI_ISteamController_GetGamepadIndexForController", kParams_SteamController005_19},
    {"SteamAPI_ISteamController_GetControllerForGamepadIndex", kParams_SteamController005_20},
    {"SteamAPI_ISteamController_GetMotionData", kParams_SteamController005_21},
    {"SteamAPI_ISteamController_ShowDigitalActionOrigins", kParams_SteamController005_22},
    {"SteamAPI_ISteamController_ShowAnalogActionOrigins", kParams_SteamController005_23},
    {"SteamAPI_ISteamController_GetStringForActionOrigin", kParams_SteamController005_24},
    {"SteamAPI_ISteamController_GetGlyphForActionOrigin", kParams_SteamController005_25},
};

class Version_SteamController005 {
public:
    virtual bool Init() {
        return steambridge::slot<bool>(kSlots_SteamController005[0]);
    }
    virtual bool Shutdown() {
        return steambridge::slot<bool>(kSlots_SteamController005[1]);
    }
    virtual void RunFrame() {
        steambridge::slot<void>(kSlots_SteamController005[2]);
    }
    virtual std::int32_t GetConnectedControllers(std::uint64_t* handlesOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamController005[3], handlesOut);
    }
    virtual bool ShowBindingPanel(std::uint64_t controllerHandle) {
        return steambridge::slot<bool>(kSlots_SteamController005[4], controllerHandle);
    }
    virtual std::uint64_t GetActionSetHandle(const char* pszActionSetName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamController005[5], pszActionSetName);
    }
    virtual void ActivateActionSet(std::uint64_t controllerHandle, std::uint64_t actionSetHandle) {
        steambridge::slot<void>(kSlots_SteamController005[6], controllerHandle, actionSetHandle);
    }
    virtual std::uint64_t GetCurrentActionSet(std::uint64_t controllerHandle) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamController005[7], controllerHandle);
    }
    virtual std::uint64_t GetDigitalActionHandle(const char* pszActionName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamController005[8], pszActionName);
    }
    virtual ControllerDigitalActionData_t GetDigitalActionData(std::uint64_t controllerHandle, std::uint64_t digitalActionHandle) {
        return steambridge::slot<ControllerDigitalActionData_t>(kSlots_SteamController005[9], controllerHandle, digitalActionHandle);
    }
    virtual std::int32_t GetDigitalActionOrigins(std::uint64_t controllerHandle, std::uint64_t actionSetHandle, std::uint64_t digitalActionHandle, std::int32_t* originsOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamController005[10], controllerHandle, actionSetHandle, digitalActionHandle, originsOut);
    }
    virtual std::uint64_t GetAnalogActionHandle(const char* pszActionName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamController005[11], pszActionName);
    }
    virtual ControllerAnalogActionData_t GetAnalogActionData(std::uint64_t controllerHandle, std::uint64_t analogActionHandle) {
        return steambridge::slot<ControllerAnalogActionData_t>(kSlots_SteamController005[12], controllerHandle, analogActionHandle);
    }
    virtual std::int32_t GetAnalogActionOrigins(std::uint64_t controllerHandle, std::uint64_t actionSetHandle, std::uint64_t analogActionHandle, std::int32_t* originsOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamController005[13], controllerHandle, actionSetHandle, analogActionHandle, originsOut);
    }
    virtual void StopAnalogActionMomentum(std::uint64_t controllerHandle, std::uint64_t eAction) {
        steambridge::slot<void>(kSlots_SteamController005[14], controllerHandle, eAction);
    }
    virtual void TriggerHapticPulse(std::uint64_t controllerHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec) {
        steambridge::slot<void>(kSlots_SteamController005[15], controllerHandle, eTargetPad, usDurationMicroSec);
    }
    virtual void TriggerRepeatedHapticPulse(std::uint64_t controllerHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec, std::uint16_t usOffMicroSec, std::uint16_t unRepeat, std::uint32_t nFlags) {
        steambridge::slot<void>(kSlots_SteamController005[16], controllerHandle, eTargetPad, usDurationMicroSec, usOffMicroSec, unRepeat, nFlags);
    }
    virtual void TriggerVibration(std::uint64_t controllerHandle, std::uint16_t usLeftSpeed, std::uint16_t usRightSpeed) {
        steambridge::slot<void>(kSlots_SteamController005[17], controllerHandle, usLeftSpeed, usRightSpeed);
    }
    virtual void SetLEDColor(std::uint64_t controllerHandle, std::uint8_t nColorR, std::uint8_t nColorG, std::uint8_t nColorB, std::uint32_t nFlags) {
        steambridge::slot<void>(kSlots_SteamController005[18], controllerHandle, nColorR, nColorG, nColorB, nFlags);
    }
    virtual std::int32_t GetGamepadIndexForController(std::uint64_t ulControllerHandle) {
        return steambridge::slot<std::int32_t>(kSlots_SteamController005[19], ulControllerHandle);
    }
    virtual std::uint64_t GetControllerForGamepadIndex(std::int32_t nIndex) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamController005[20], nIndex);
    }
    virtual ControllerMotionData_t GetMotionData(std::uint64_t controllerHandle) {
        return steambridge::slot<ControllerMotionData_t>(kSlots_SteamController005[21], controllerHandle);
    }
    virtual bool ShowDigitalActionOrigins(std::uint64_t controllerHandle, std::uint64_t digitalActionHandle, float flScale, float flXPosition, float flYPosition) {
        return steambridge::slot<bool>(kSlots_SteamController005[22], controllerHandle, digitalActionHandle, flScale, flXPosition, flYPosition);
    }
    virtual bool ShowAnalogActionOrigins(std::uint64_t controllerHandle, std::uint64_t analogActionHandle, float flScale, float flXPosition, float flYPosition) {
        return steambridge::slot<bool>(kSlots_SteamController005[23], controllerHandle, analogActionHandle, flScale, flXPosition, flYPosition);
    }
    virtual const char* GetStringForActionOrigin(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamController005[24], eOrigin);
    }
    virtual const char* GetGlyphForActionOrigin(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamController005[25], eOrigin);
    }
};

Version_SteamController005 g_SteamController005;

const char* const kParams_SteamController007_3[] = {
    "handlesOut",
};
const char* const kParams_SteamController007_4[] = {
    "pszActionSetName",
};
const char* const kParams_SteamController007_5[] = {
    "controllerHandle",
    "actionSetHandle",
};
const char* const kParams_SteamController007_6[] = {
    "controllerHandle",
};
const char* const kParams_SteamController007_7[] = {
    "controllerHandle",
    "actionSetLayerHandle",
};
const char* const kParams_SteamController007_8[] = {
    "controllerHandle",
    "actionSetLayerHandle",
};
const char* const kParams_SteamController007_9[] = {
    "controllerHandle",
};
const char* const kParams_SteamController007_10[] = {
    "controllerHandle",
    "handlesOut",
};
const char* const kParams_SteamController007_11[] = {
    "pszActionName",
};
const char* const kParams_SteamController007_12[] = {
    "controllerHandle",
    "digitalActionHandle",
};
const char* const kParams_SteamController007_13[] = {
    "controllerHandle",
    "actionSetHandle",
    "digitalActionHandle",
    "originsOut",
};
const char* const kParams_SteamController007_14[] = {
    "pszActionName",
};
const char* const kParams_SteamController007_15[] = {
    "controllerHandle",
    "analogActionHandle",
};
const char* const kParams_SteamController007_16[] = {
    "controllerHandle",
    "actionSetHandle",
    "analogActionHandle",
    "originsOut",
};
const char* const kParams_SteamController007_17[] = {
    "eOrigin",
};
const char* const kParams_SteamController007_18[] = {
    "eOrigin",
};
const char* const kParams_SteamController007_19[] = {
    "controllerHandle",
    "eAction",
};
const char* const kParams_SteamController007_20[] = {
    "controllerHandle",
};
const char* const kParams_SteamController007_21[] = {
    "controllerHandle",
    "eTargetPad",
    "usDurationMicroSec",
};
const char* const kParams_SteamController007_22[] = {
    "controllerHandle",
    "eTargetPad",
    "usDurationMicroSec",
    "usOffMicroSec",
    "unRepeat",
    "nFlags",
};
const char* const kParams_SteamController007_23[] = {
    "controllerHandle",
    "usLeftSpeed",
    "usRightSpeed",
};
const char* const kParams_SteamController007_24[] = {
    "controllerHandle",
    "nColorR",
    "nColorG",
    "nColorB",
    "nFlags",
};
const char* const kParams_SteamController007_25[] = {
    "controllerHandle",
};
const char* const kParams_SteamController007_26[] = {
    "controllerHandle",
};
const char* const kParams_SteamController007_27[] = {
    "nIndex",
};
const char* const kParams_SteamController007_28[] = {
    "ulControllerHandle",
};
const char* const kParams_SteamController007_29[] = {
    "eOrigin",
};
const char* const kParams_SteamController007_30[] = {
    "eOrigin",
};
const char* const kParams_SteamController007_31[] = {
    "controllerHandle",
    "eOrigin",
};
const char* const kParams_SteamController007_32[] = {
    "eDestinationInputType",
    "eSourceOrigin",
};
const char* const kParams_SteamController007_33[] = {
    "controllerHandle",
    "pMajor",
    "pMinor",
};
// ISteamController SteamController007
const steambridge::SlotInfo kSlots_SteamController007[] = {
    {"SteamAPI_ISteamController_Init", nullptr},
    {"SteamAPI_ISteamController_Shutdown", nullptr},
    {"SteamAPI_ISteamController_RunFrame", nullptr},
    {"SteamAPI_ISteamController_GetConnectedControllers", kParams_SteamController007_3},
    {"SteamAPI_ISteamController_GetActionSetHandle", kParams_SteamController007_4},
    {"SteamAPI_ISteamController_ActivateActionSet", kParams_SteamController007_5},
    {"SteamAPI_ISteamController_GetCurrentActionSet", kParams_SteamController007_6},
    {"SteamAPI_ISteamController_ActivateActionSetLayer", kParams_SteamController007_7},
    {"SteamAPI_ISteamController_DeactivateActionSetLayer", kParams_SteamController007_8},
    {"SteamAPI_ISteamController_DeactivateAllActionSetLayers", kParams_SteamController007_9},
    {"SteamAPI_ISteamController_GetActiveActionSetLayers", kParams_SteamController007_10},
    {"SteamAPI_ISteamController_GetDigitalActionHandle", kParams_SteamController007_11},
    {"SteamAPI_ISteamController_GetDigitalActionData", kParams_SteamController007_12},
    {"SteamAPI_ISteamController_GetDigitalActionOrigins", kParams_SteamController007_13},
    {"SteamAPI_ISteamController_GetAnalogActionHandle", kParams_SteamController007_14},
    {"SteamAPI_ISteamController_GetAnalogActionData", kParams_SteamController007_15},
    {"SteamAPI_ISteamController_GetAnalogActionOrigins", kParams_SteamController007_16},
    {"SteamAPI_ISteamController_GetGlyphForActionOrigin", kParams_SteamController007_17},
    {"SteamAPI_ISteamController_GetStringForActionOrigin", kParams_SteamController007_18},
    {"SteamAPI_ISteamController_StopAnalogActionMomentum", kParams_SteamController007_19},
    {"SteamAPI_ISteamController_GetMotionData", kParams_SteamController007_20},
    {"SteamAPI_ISteamController_TriggerHapticPulse", kParams_SteamController007_21},
    {"SteamAPI_ISteamController_TriggerRepeatedHapticPulse", kParams_SteamController007_22},
    {"SteamAPI_ISteamController_TriggerVibration", kParams_SteamController007_23},
    {"SteamAPI_ISteamController_SetLEDColor", kParams_SteamController007_24},
    {"SteamAPI_ISteamController_ShowBindingPanel", kParams_SteamController007_25},
    {"SteamAPI_ISteamController_GetInputTypeForHandle", kParams_SteamController007_26},
    {"SteamAPI_ISteamController_GetControllerForGamepadIndex", kParams_SteamController007_27},
    {"SteamAPI_ISteamController_GetGamepadIndexForController", kParams_SteamController007_28},
    {"SteamAPI_ISteamController_GetStringForXboxOrigin", kParams_SteamController007_29},
    {"SteamAPI_ISteamController_GetGlyphForXboxOrigin", kParams_SteamController007_30},
    {"SteamAPI_ISteamController_GetActionOriginFromXboxOrigin", kParams_SteamController007_31},
    {"SteamAPI_ISteamController_TranslateActionOrigin", kParams_SteamController007_32},
    {"SteamAPI_ISteamController_GetControllerBindingRevision", kParams_SteamController007_33},
};

class Version_SteamController007 {
public:
    virtual bool Init() {
        return steambridge::slot<bool>(kSlots_SteamController007[0]);
    }
    virtual bool Shutdown() {
        return steambridge::slot<bool>(kSlots_SteamController007[1]);
    }
    virtual void RunFrame() {
        steambridge::slot<void>(kSlots_SteamController007[2]);
    }
    virtual std::int32_t GetConnectedControllers(std::uint64_t* handlesOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamController007[3], handlesOut);
    }
    virtual std::uint64_t GetActionSetHandle(const char* pszActionSetName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamController007[4], pszActionSetName);
    }
    virtual void ActivateActionSet(std::uint64_t controllerHandle, std::uint64_t actionSetHandle) {
        steambridge::slot<void>(kSlots_SteamController007[5], controllerHandle, actionSetHandle);
    }
    virtual std::uint64_t GetCurrentActionSet(std::uint64_t controllerHandle) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamController007[6], controllerHandle);
    }
    virtual void ActivateActionSetLayer(std::uint64_t controllerHandle, std::uint64_t actionSetLayerHandle) {
        steambridge::slot<void>(kSlots_SteamController007[7], controllerHandle, actionSetLayerHandle);
    }
    virtual void DeactivateActionSetLayer(std::uint64_t controllerHandle, std::uint64_t actionSetLayerHandle) {
        steambridge::slot<void>(kSlots_SteamController007[8], controllerHandle, actionSetLayerHandle);
    }
    virtual void DeactivateAllActionSetLayers(std::uint64_t controllerHandle) {
        steambridge::slot<void>(kSlots_SteamController007[9], controllerHandle);
    }
    virtual std::int32_t GetActiveActionSetLayers(std::uint64_t controllerHandle, std::uint64_t* handlesOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamController007[10], controllerHandle, handlesOut);
    }
    virtual std::uint64_t GetDigitalActionHandle(const char* pszActionName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamController007[11], pszActionName);
    }
    virtual ControllerDigitalActionData_t GetDigitalActionData(std::uint64_t controllerHandle, std::uint64_t digitalActionHandle) {
        return steambridge::slot<ControllerDigitalActionData_t>(kSlots_SteamController007[12], controllerHandle, digitalActionHandle);
    }
    virtual std::int32_t GetDigitalActionOrigins(std::uint64_t controllerHandle, std::uint64_t actionSetHandle, std::uint64_t digitalActionHandle, std::int32_t* originsOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamController007[13], controllerHandle, actionSetHandle, digitalActionHandle, originsOut);
    }
    virtual std::uint64_t GetAnalogActionHandle(const char* pszActionName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamController007[14], pszActionName);
    }
    virtual ControllerAnalogActionData_t GetAnalogActionData(std::uint64_t controllerHandle, std::uint64_t analogActionHandle) {
        return steambridge::slot<ControllerAnalogActionData_t>(kSlots_SteamController007[15], controllerHandle, analogActionHandle);
    }
    virtual std::int32_t GetAnalogActionOrigins(std::uint64_t controllerHandle, std::uint64_t actionSetHandle, std::uint64_t analogActionHandle, std::int32_t* originsOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamController007[16], controllerHandle, actionSetHandle, analogActionHandle, originsOut);
    }
    virtual const char* GetGlyphForActionOrigin(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamController007[17], eOrigin);
    }
    virtual const char* GetStringForActionOrigin(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamController007[18], eOrigin);
    }
    virtual void StopAnalogActionMomentum(std::uint64_t controllerHandle, std::uint64_t eAction) {
        steambridge::slot<void>(kSlots_SteamController007[19], controllerHandle, eAction);
    }
    virtual ControllerMotionData_t GetMotionData(std::uint64_t controllerHandle) {
        return steambridge::slot<ControllerMotionData_t>(kSlots_SteamController007[20], controllerHandle);
    }
    virtual void TriggerHapticPulse(std::uint64_t controllerHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec) {
        steambridge::slot<void>(kSlots_SteamController007[21], controllerHandle, eTargetPad, usDurationMicroSec);
    }
    virtual void TriggerRepeatedHapticPulse(std::uint64_t controllerHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec, std::uint16_t usOffMicroSec, std::uint16_t unRepeat, std::uint32_t nFlags) {
        steambridge::slot<void>(kSlots_SteamController007[22], controllerHandle, eTargetPad, usDurationMicroSec, usOffMicroSec, unRepeat, nFlags);
    }
    virtual void TriggerVibration(std::uint64_t controllerHandle, std::uint16_t usLeftSpeed, std::uint16_t usRightSpeed) {
        steambridge::slot<void>(kSlots_SteamController007[23], controllerHandle, usLeftSpeed, usRightSpeed);
    }
    virtual void SetLEDColor(std::uint64_t controllerHandle, std::uint8_t nColorR, std::uint8_t nColorG, std::uint8_t nColorB, std::uint32_t nFlags) {
        steambridge::slot<void>(kSlots_SteamController007[24], controllerHandle, nColorR, nColorG, nColorB, nFlags);
    }
    virtual bool ShowBindingPanel(std::uint64_t controllerHandle) {
        return steambridge::slot<bool>(kSlots_SteamController007[25], controllerHandle);
    }
    virtual std::int32_t GetInputTypeForHandle(std::uint64_t controllerHandle) {
        return steambridge::slot<std::int32_t>(kSlots_SteamController007[26], controllerHandle);
    }
    virtual std::uint64_t GetControllerForGamepadIndex(std::int32_t nIndex) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamController007[27], nIndex);
    }
    virtual std::int32_t GetGamepadIndexForController(std::uint64_t ulControllerHandle) {
        return steambridge::slot<std::int32_t>(kSlots_SteamController007[28], ulControllerHandle);
    }
    virtual const char* GetStringForXboxOrigin(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamController007[29], eOrigin);
    }
    virtual const char* GetGlyphForXboxOrigin(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamController007[30], eOrigin);
    }
    virtual std::int32_t GetActionOriginFromXboxOrigin(std::uint64_t controllerHandle, std::int32_t eOrigin) {
        return steambridge::slot<std::int32_t>(kSlots_SteamController007[31], controllerHandle, eOrigin);
    }
    virtual std::int32_t TranslateActionOrigin(std::int32_t eDestinationInputType, std::int32_t eSourceOrigin) {
        return steambridge::slot<std::int32_t>(kSlots_SteamController007[32], eDestinationInputType, eSourceOrigin);
    }
    virtual bool GetControllerBindingRevision(std::uint64_t controllerHandle, std::int32_t* pMajor, std::int32_t* pMinor) {
        return steambridge::slot<bool>(kSlots_SteamController007[33], controllerHandle, pMajor, pMinor);
    }
};

Version_SteamController007 g_SteamController007;

const char* const kParams_SteamController008_3[] = {
    "handlesOut",
};
const char* const kParams_SteamController008_4[] = {
    "pszActionSetName",
};
const char* const kParams_SteamController008_5[] = {
    "controllerHandle",
    "actionSetHandle",
};
const char* const kParams_SteamController008_6[] = {
    "controllerHandle",
};
const char* const kParams_SteamController008_7[] = {
    "controllerHandle",
    "actionSetLayerHandle",
};
const char* const kParams_SteamController008_8[] = {
    "controllerHandle",
    "actionSetLayerHandle",
};
const char* const kParams_SteamController008_9[] = {
    "controllerHandle",
};
const char* const kParams_SteamController008_10[] = {
    "controllerHandle",
    "handlesOut",
};
const char* const kParams_SteamController008_11[] = {
    "pszActionName",
};
const char* const kParams_SteamController008_12[] = {
    "controllerHandle",
    "digitalActionHandle",
};
const char* const kParams_SteamController008_13[] = {
    "controllerHandle",
    "actionSetHandle",
    "digitalActionHandle",
    "originsOut",
};
const char* const kParams_SteamController008_14[] = {
    "pszActionName",
};
const char* const kParams_SteamController008_15[] = {
    "controllerHandle",
    "analogActionHandle",
};
const char* const kParams_SteamController008_16[] = {
    "controllerHandle",
    "actionSetHandle",
    "analogActionHandle",
    "originsOut",
};
const char* const kParams_SteamController008_17[] = {
    "eOrigin",
};
const char* const kParams_SteamController008_18[] = {
    "eOrigin",
};
const char* const kParams_SteamController008_19[] = {
    "controllerHandle",
    "eAction",
};
const char* const kParams_SteamController008_20[] = {
    "controllerHandle",
};
const char* const kParams_SteamController008_21[] = {
    "controllerHandle",
    "eTargetPad",
    "usDurationMicroSec",
};
const char* const kParams_SteamController008_22[] = {
    "controllerHandle",
    "eTargetPad",
    "usDurationMicroSec",
    "usOffMicroSec",
    "unRepeat",
    "nFlags",
};
const char* const kParams_SteamController008_23[] = {
    "controllerHandle",
    "usLeftSpeed",
    "usRightSpeed",
};
const char* const kParams_SteamController008_24[] = {
    "controllerHandle",
    "nColorR",
    "nColorG",
    "nColorB",
    "nFlags",
};
const char* const kParams_SteamController008_25[] = {
    "controllerHandle",
};
const char* const kParams_SteamController008_26[] = {
    "controllerHandle",
};
const char* const kParams_SteamController008_27[] = {
    "nIndex",
};
const char* const kParams_SteamController008_28[] = {
    "ulControllerHandle",
};
const char* const kParams_SteamController008_29[] = {
    "eOrigin",
};
const char* const kParams_SteamController008_30[] = {
    "eOrigin",
};
const char* const kParams_SteamController008_31[] = {
    "controllerHandle",
    "eOrigin",
};
const char* const kParams_SteamController008_32[] = {
    "eDestinationInputType",
    "eSourceOrigin",
};
const char* const kParams_SteamController008_33[] = {
    "controllerHandle",
    "pMajor",
    "pMinor",
};
// ISteamController SteamController008
const steambridge::SlotInfo kSlots_SteamController008[] = {
    {"SteamAPI_ISteamController_Init", nullptr},
    {"SteamAPI_ISteamController_Shutdown", nullptr},
    {"SteamAPI_ISteamController_RunFrame", nullptr},
    {"SteamAPI_ISteamController_GetConnectedControllers", kParams_SteamController008_3},
    {"SteamAPI_ISteamController_GetActionSetHandle", kParams_SteamController008_4},
    {"SteamAPI_ISteamController_ActivateActionSet", kParams_SteamController008_5},
    {"SteamAPI_ISteamController_GetCurrentActionSet", kParams_SteamController008_6},
    {"SteamAPI_ISteamController_ActivateActionSetLayer", kParams_SteamController008_7},
    {"SteamAPI_ISteamController_DeactivateActionSetLayer", kParams_SteamController008_8},
    {"SteamAPI_ISteamController_DeactivateAllActionSetLayers", kParams_SteamController008_9},
    {"SteamAPI_ISteamController_GetActiveActionSetLayers", kParams_SteamController008_10},
    {"SteamAPI_ISteamController_GetDigitalActionHandle", kParams_SteamController008_11},
    {"SteamAPI_ISteamController_GetDigitalActionData", kParams_SteamController008_12},
    {"SteamAPI_ISteamController_GetDigitalActionOrigins", kParams_SteamController008_13},
    {"SteamAPI_ISteamController_GetAnalogActionHandle", kParams_SteamController008_14},
    {"SteamAPI_ISteamController_GetAnalogActionData", kParams_SteamController008_15},
    {"SteamAPI_ISteamController_GetAnalogActionOrigins", kParams_SteamController008_16},
    {"SteamAPI_ISteamController_GetGlyphForActionOrigin", kParams_SteamController008_17},
    {"SteamAPI_ISteamController_GetStringForActionOrigin", kParams_SteamController008_18},
    {"SteamAPI_ISteamController_StopAnalogActionMomentum", kParams_SteamController008_19},
    {"SteamAPI_ISteamController_GetMotionData", kParams_SteamController008_20},
    {"SteamAPI_ISteamController_TriggerHapticPulse", kParams_SteamController008_21},
    {"SteamAPI_ISteamController_TriggerRepeatedHapticPulse", kParams_SteamController008_22},
    {"SteamAPI_ISteamController_TriggerVibration", kParams_SteamController008_23},
    {"SteamAPI_ISteamController_SetLEDColor", kParams_SteamController008_24},
    {"SteamAPI_ISteamController_ShowBindingPanel", kParams_SteamController008_25},
    {"SteamAPI_ISteamController_GetInputTypeForHandle", kParams_SteamController008_26},
    {"SteamAPI_ISteamController_GetControllerForGamepadIndex", kParams_SteamController008_27},
    {"SteamAPI_ISteamController_GetGamepadIndexForController", kParams_SteamController008_28},
    {"SteamAPI_ISteamController_GetStringForXboxOrigin", kParams_SteamController008_29},
    {"SteamAPI_ISteamController_GetGlyphForXboxOrigin", kParams_SteamController008_30},
    {"SteamAPI_ISteamController_GetActionOriginFromXboxOrigin", kParams_SteamController008_31},
    {"SteamAPI_ISteamController_TranslateActionOrigin", kParams_SteamController008_32},
    {"SteamAPI_ISteamController_GetControllerBindingRevision", kParams_SteamController008_33},
};

class Version_SteamController008 {
public:
    virtual bool Init() {
        return steambridge::slot<bool>(kSlots_SteamController008[0]);
    }
    virtual bool Shutdown() {
        return steambridge::slot<bool>(kSlots_SteamController008[1]);
    }
    virtual void RunFrame() {
        steambridge::slot<void>(kSlots_SteamController008[2]);
    }
    virtual std::int32_t GetConnectedControllers(std::uint64_t* handlesOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamController008[3], handlesOut);
    }
    virtual std::uint64_t GetActionSetHandle(const char* pszActionSetName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamController008[4], pszActionSetName);
    }
    virtual void ActivateActionSet(std::uint64_t controllerHandle, std::uint64_t actionSetHandle) {
        steambridge::slot<void>(kSlots_SteamController008[5], controllerHandle, actionSetHandle);
    }
    virtual std::uint64_t GetCurrentActionSet(std::uint64_t controllerHandle) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamController008[6], controllerHandle);
    }
    virtual void ActivateActionSetLayer(std::uint64_t controllerHandle, std::uint64_t actionSetLayerHandle) {
        steambridge::slot<void>(kSlots_SteamController008[7], controllerHandle, actionSetLayerHandle);
    }
    virtual void DeactivateActionSetLayer(std::uint64_t controllerHandle, std::uint64_t actionSetLayerHandle) {
        steambridge::slot<void>(kSlots_SteamController008[8], controllerHandle, actionSetLayerHandle);
    }
    virtual void DeactivateAllActionSetLayers(std::uint64_t controllerHandle) {
        steambridge::slot<void>(kSlots_SteamController008[9], controllerHandle);
    }
    virtual std::int32_t GetActiveActionSetLayers(std::uint64_t controllerHandle, std::uint64_t* handlesOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamController008[10], controllerHandle, handlesOut);
    }
    virtual std::uint64_t GetDigitalActionHandle(const char* pszActionName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamController008[11], pszActionName);
    }
    virtual ControllerDigitalActionData_t GetDigitalActionData(std::uint64_t controllerHandle, std::uint64_t digitalActionHandle) {
        return steambridge::slot<ControllerDigitalActionData_t>(kSlots_SteamController008[12], controllerHandle, digitalActionHandle);
    }
    virtual std::int32_t GetDigitalActionOrigins(std::uint64_t controllerHandle, std::uint64_t actionSetHandle, std::uint64_t digitalActionHandle, std::int32_t* originsOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamController008[13], controllerHandle, actionSetHandle, digitalActionHandle, originsOut);
    }
    virtual std::uint64_t GetAnalogActionHandle(const char* pszActionName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamController008[14], pszActionName);
    }
    virtual ControllerAnalogActionData_t GetAnalogActionData(std::uint64_t controllerHandle, std::uint64_t analogActionHandle) {
        return steambridge::slot<ControllerAnalogActionData_t>(kSlots_SteamController008[15], controllerHandle, analogActionHandle);
    }
    virtual std::int32_t GetAnalogActionOrigins(std::uint64_t controllerHandle, std::uint64_t actionSetHandle, std::uint64_t analogActionHandle, std::int32_t* originsOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamController008[16], controllerHandle, actionSetHandle, analogActionHandle, originsOut);
    }
    virtual const char* GetGlyphForActionOrigin(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamController008[17], eOrigin);
    }
    virtual const char* GetStringForActionOrigin(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamController008[18], eOrigin);
    }
    virtual void StopAnalogActionMomentum(std::uint64_t controllerHandle, std::uint64_t eAction) {
        steambridge::slot<void>(kSlots_SteamController008[19], controllerHandle, eAction);
    }
    virtual ControllerMotionData_t GetMotionData(std::uint64_t controllerHandle) {
        return steambridge::slot<ControllerMotionData_t>(kSlots_SteamController008[20], controllerHandle);
    }
    virtual void TriggerHapticPulse(std::uint64_t controllerHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec) {
        steambridge::slot<void>(kSlots_SteamController008[21], controllerHandle, eTargetPad, usDurationMicroSec);
    }
    virtual void TriggerRepeatedHapticPulse(std::uint64_t controllerHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec, std::uint16_t usOffMicroSec, std::uint16_t unRepeat, std::uint32_t nFlags) {
        steambridge::slot<void>(kSlots_SteamController008[22], controllerHandle, eTargetPad, usDurationMicroSec, usOffMicroSec, unRepeat, nFlags);
    }
    virtual void TriggerVibration(std::uint64_t controllerHandle, std::uint16_t usLeftSpeed, std::uint16_t usRightSpeed) {
        steambridge::slot<void>(kSlots_SteamController008[23], controllerHandle, usLeftSpeed, usRightSpeed);
    }
    virtual void SetLEDColor(std::uint64_t controllerHandle, std::uint8_t nColorR, std::uint8_t nColorG, std::uint8_t nColorB, std::uint32_t nFlags) {
        steambridge::slot<void>(kSlots_SteamController008[24], controllerHandle, nColorR, nColorG, nColorB, nFlags);
    }
    virtual bool ShowBindingPanel(std::uint64_t controllerHandle) {
        return steambridge::slot<bool>(kSlots_SteamController008[25], controllerHandle);
    }
    virtual std::int32_t GetInputTypeForHandle(std::uint64_t controllerHandle) {
        return steambridge::slot<std::int32_t>(kSlots_SteamController008[26], controllerHandle);
    }
    virtual std::uint64_t GetControllerForGamepadIndex(std::int32_t nIndex) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamController008[27], nIndex);
    }
    virtual std::int32_t GetGamepadIndexForController(std::uint64_t ulControllerHandle) {
        return steambridge::slot<std::int32_t>(kSlots_SteamController008[28], ulControllerHandle);
    }
    virtual const char* GetStringForXboxOrigin(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamController008[29], eOrigin);
    }
    virtual const char* GetGlyphForXboxOrigin(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamController008[30], eOrigin);
    }
    virtual std::int32_t GetActionOriginFromXboxOrigin(std::uint64_t controllerHandle, std::int32_t eOrigin) {
        return steambridge::slot<std::int32_t>(kSlots_SteamController008[31], controllerHandle, eOrigin);
    }
    virtual std::int32_t TranslateActionOrigin(std::int32_t eDestinationInputType, std::int32_t eSourceOrigin) {
        return steambridge::slot<std::int32_t>(kSlots_SteamController008[32], eDestinationInputType, eSourceOrigin);
    }
    virtual bool GetControllerBindingRevision(std::uint64_t controllerHandle, std::int32_t* pMajor, std::int32_t* pMinor) {
        return steambridge::slot<bool>(kSlots_SteamController008[33], controllerHandle, pMajor, pMinor);
    }
};

Version_SteamController008 g_SteamController008;

const char* const kParams_SteamFriends015_1[] = {
    "pchPersonaName",
};
const char* const kParams_SteamFriends015_3[] = {
    "iFriendFlags",
};
const char* const kParams_SteamFriends015_4[] = {
    "iFriend",
    "iFriendFlags",
};
const char* const kParams_SteamFriends015_5[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends015_6[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends015_7[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends015_8[] = {
    "steamIDFriend",
    "pFriendGameInfo",
};
const char* const kParams_SteamFriends015_9[] = {
    "steamIDFriend",
    "iPersonaName",
};
const char* const kParams_SteamFriends015_10[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends015_11[] = {
    "steamIDPlayer",
};
const char* const kParams_SteamFriends015_13[] = {
    "iFG",
};
const char* const kParams_SteamFriends015_14[] = {
    "friendsGroupID",
};
const char* const kParams_SteamFriends015_15[] = {
    "friendsGroupID",
};
const char* const kParams_SteamFriends015_16[] = {
    "friendsGroupID",
    "pOutSteamIDMembers",
    "nMembersCount",
};
const char* const kParams_SteamFriends015_17[] = {
    "steamIDFriend",
    "iFriendFlags",
};
const char* const kParams_SteamFriends015_19[] = {
    "iClan",
};
const char* const kParams_SteamFriends015_20[] = {
    "steamIDClan",
};
const char* const kParams_SteamFriends015_21[] = {
    "steamIDClan",
};
const char* const kParams_SteamFriends015_22[] = {
    "steamIDClan",
    "pnOnline",
    "pnInGame",
    "pnChatting",
};
const char* const kParams_SteamFriends015_23[] = {
    "psteamIDClans",
    "cClansToRequest",
};
const char* const kParams_SteamFriends015_24[] = {
    "steamIDSource",
};
const char* const kParams_SteamFriends015_25[] = {
    "steamIDSource",
    "iFriend",
};
const char* const kParams_SteamFriends015_26[] = {
    "steamIDUser",
    "steamIDSource",
};
const char* const kParams_SteamFriends015_27[] = {
    "steamIDUser",
    "bSpeaking",
};
const char* const kParams_SteamFriends015_28[] = {
    "pchDialog",
};
const char* const kParams_SteamFriends015_29[] = {
    "pchDialog",
    "steamID",
};
const char* const kParams_SteamFriends015_30[] = {
    "pchURL",
};
const char* const kParams_SteamFriends015_31[] = {
    "nAppID",
    "eFlag",
};
const char* const kParams_SteamFriends015_32[] = {
    "steamIDUserPlayedWith",
};
const char* const kParams_SteamFriends015_33[] = {
    "steamIDLobby",
};
const char* const kParams_SteamFriends015_34[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends015_35[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends015_36[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends015_37[] = {
    "steamIDUser",
    "bRequireNameOnly",
};
const char* const kParams_SteamFriends015_38[] = {
    "steamIDClan",
};
const char* const kParams_SteamFriends015_39[] = {
    "steamIDClan",
};
const char* const kParams_SteamFriends015_40[] = {
    "steamIDClan",
};
const char* const kParams_SteamFriends015_41[] = {
    "steamIDClan",
    "iOfficer",
};
const char* const kParams_SteamFriends015_43[] = {
    "pchKey",
    "pchValue",
};
const char* const kParams_SteamFriends015_45[] = {
    "steamIDFriend",
    "pchKey",
};
const char* const kParams_SteamFriends015_46[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends015_47[] = {
    "steamIDFriend",
    "iKey",
};
const char* const kParams_SteamFriends015_48[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends015_49[] = {
    "steamIDFriend",
    "pchConnectString",
};
const char* const kParams_SteamFriends015_51[] = {
    "iCoplayFriend",
};
const char* const kParams_SteamFriends015_52[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends015_53[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends015_54[] = {
    "steamIDClan",
};
const char* const kParams_SteamFriends015_55[] = {
    "steamIDClan",
};
const char* const kParams_SteamFriends015_56[] = {
    "steamIDClan",
};
const char* const kParams_SteamFriends015_57[] = {
    "steamIDClan",
    "iUser",
};
const char* const kParams_SteamFriends015_58[] = {
    "steamIDClanChat",
    "pchText",
};
const char* const kParams_SteamFriends015_59[] = {
    "steamIDClanChat",
    "iMessage",
    "prgchText",
    "cchTextMax",
    "peChatEntryType",
    "psteamidChatter",
};
const char* const kParams_SteamFriends015_60[] = {
    "steamIDClanChat",
    "steamIDUser",
};
const char* const kParams_SteamFriends015_61[] = {
    "steamIDClanChat",
};
const char* const kParams_SteamFriends015_62[] = {
    "steamIDClanChat",
};
const char* const kParams_SteamFriends015_63[] = {
    "steamIDClanChat",
};
const char* const kParams_SteamFriends015_64[] = {
    "bInterceptEnabled",
};
const char* const kParams_SteamFriends015_65[] = {
    "steamIDFriend",
    "pchMsgToSend",
};
const char* const kParams_SteamFriends015_66[] = {
    "steamIDFriend",
    "iMessageID",
    "pvData",
    "cubData",
    "peChatEntryType",
};
const char* const kParams_SteamFriends015_67[] = {
    "steamID",
};
const char* const kParams_SteamFriends015_68[] = {
    "steamID",
};
const char* const kParams_SteamFriends015_69[] = {
    "unStartIndex",
};
// ISteamFriends SteamFriends015
const steambridge::SlotInfo kSlots_SteamFriends015[] = {
    {"SteamAPI_ISteamFriends_GetPersonaName", nullptr},
    {"SteamAPI_ISteamFriends_SetPersonaName", kParams_SteamFriends015_1},
    {"SteamAPI_ISteamFriends_GetPersonaState", nullptr},
    {"SteamAPI_ISteamFriends_GetFriendCount", kParams_SteamFriends015_3},
    {"SteamAPI_ISteamFriends_GetFriendByIndex", kParams_SteamFriends015_4},
    {"SteamAPI_ISteamFriends_GetFriendRelationship", kParams_SteamFriends015_5},
    {"SteamAPI_ISteamFriends_GetFriendPersonaState", kParams_SteamFriends015_6},
    {"SteamAPI_ISteamFriends_GetFriendPersonaName", kParams_SteamFriends015_7},
    {"SteamAPI_ISteamFriends_GetFriendGamePlayed", kParams_SteamFriends015_8},
    {"SteamAPI_ISteamFriends_GetFriendPersonaNameHistory", kParams_SteamFriends015_9},
    {"SteamAPI_ISteamFriends_GetFriendSteamLevel", kParams_SteamFriends015_10},
    {"SteamAPI_ISteamFriends_GetPlayerNickname", kParams_SteamFriends015_11},
    {"SteamAPI_ISteamFriends_GetFriendsGroupCount", nullptr},
    {"SteamAPI_ISteamFriends_GetFriendsGroupIDByIndex", kParams_SteamFriends015_13},
    {"SteamAPI_ISteamFriends_GetFriendsGroupName", kParams_SteamFriends015_14},
    {"SteamAPI_ISteamFriends_GetFriendsGroupMembersCount", kParams_SteamFriends015_15},
    {"SteamAPI_ISteamFriends_GetFriendsGroupMembersList", kParams_SteamFriends015_16},
    {"SteamAPI_ISteamFriends_HasFriend", kParams_SteamFriends015_17},
    {"SteamAPI_ISteamFriends_GetClanCount", nullptr},
    {"SteamAPI_ISteamFriends_GetClanByIndex", kParams_SteamFriends015_19},
    {"SteamAPI_ISteamFriends_GetClanName", kParams_SteamFriends015_20},
    {"SteamAPI_ISteamFriends_GetClanTag", kParams_SteamFriends015_21},
    {"SteamAPI_ISteamFriends_GetClanActivityCounts", kParams_SteamFriends015_22},
    {"SteamAPI_ISteamFriends_DownloadClanActivityCounts", kParams_SteamFriends015_23},
    {"SteamAPI_ISteamFriends_GetFriendCountFromSource", kParams_SteamFriends015_24},
    {"SteamAPI_ISteamFriends_GetFriendFromSourceByIndex", kParams_SteamFriends015_25},
    {"SteamAPI_ISteamFriends_IsUserInSource", kParams_SteamFriends015_26},
    {"SteamAPI_ISteamFriends_SetInGameVoiceSpeaking", kParams_SteamFriends015_27},
    {"SteamAPI_ISteamFriends_ActivateGameOverlay", kParams_SteamFriends015_28},
    {"SteamAPI_ISteamFriends_ActivateGameOverlayToUser", kParams_SteamFriends015_29},
    {"SteamAPI_ISteamFriends_ActivateGameOverlayToWebPage", kParams_SteamFriends015_30},
    {"SteamAPI_ISteamFriends_ActivateGameOverlayToStore", kParams_SteamFriends015_31},
    {"SteamAPI_ISteamFriends_SetPlayedWith", kParams_SteamFriends015_32},
    {"SteamAPI_ISteamFriends_ActivateGameOverlayInviteDialog", kParams_SteamFriends015_33},
    {"SteamAPI_ISteamFriends_GetSmallFriendAvatar", kParams_SteamFriends015_34},
    {"SteamAPI_ISteamFriends_GetMediumFriendAvatar", kParams_SteamFriends015_35},
    {"SteamAPI_ISteamFriends_GetLargeFriendAvatar", kParams_SteamFriends015_36},
    {"SteamAPI_ISteamFriends_RequestUserInformation", kParams_SteamFriends015_37},
    {"SteamAPI_ISteamFriends_RequestClanOfficerList", kParams_SteamFriends015_38},
    {"SteamAPI_ISteamFriends_GetClanOwner", kParams_SteamFriends015_39},
    {"SteamAPI_ISteamFriends_GetClanOfficerCount", kParams_SteamFriends015_40},
    {"SteamAPI_ISteamFriends_GetClanOfficerByIndex", kParams_SteamFriends015_41},
    {"SteamAPI_ISteamFriends_GetUserRestrictions", nullptr},
    {"SteamAPI_ISteamFriends_SetRichPresence", kParams_SteamFriends015_43},
    {"SteamAPI_ISteamFriends_ClearRichPresence", nullptr},
    {"SteamAPI_ISteamFriends_GetFriendRichPresence", kParams_SteamFriends015_45},
    {"SteamAPI_ISteamFriends_GetFriendRichPresenceKeyCount", kParams_SteamFriends015_46},
    {"SteamAPI_ISteamFriends_GetFriendRichPresenceKeyByIndex", kParams_SteamFriends015_47},
    {"SteamAPI_ISteamFriends_RequestFriendRichPresence", kParams_SteamFriends015_48},
    {"SteamAPI_ISteamFriends_InviteUserToGame", kParams_SteamFriends015_49},
    {"SteamAPI_ISteamFriends_GetCoplayFriendCount", nullptr},
    {"SteamAPI_ISteamFriends_GetCoplayFriend", kParams_SteamFriends015_51},
    {"SteamAPI_ISteamFriends_GetFriendCoplayTime", kParams_SteamFriends015_52},
    {"SteamAPI_ISteamFriends_GetFriendCoplayGame", kParams_SteamFriends015_53},
    {"SteamAPI_ISteamFriends_JoinClanChatRoom", kParams_SteamFriends015_54},
    {"SteamAPI_ISteamFriends_LeaveClanChatRoom", kParams_SteamFriends015_55},
    {"SteamAPI_ISteamFriends_GetClanChatMemberCount", kParams_SteamFriends015_56},
    {"SteamAPI_ISteamFriends_GetChatMemberByIndex", kParams_SteamFriends015_57},
    {"SteamAPI_ISteamFriends_SendClanChatMessage", kParams_SteamFriends015_58},
    {"SteamAPI_ISteamFriends_GetClanChatMessage", kParams_SteamFriends015_59},
    {"SteamAPI_ISteamFriends_IsClanChatAdmin", kParams_SteamFriends015_60},
    {"SteamAPI_ISteamFriends_IsClanChatWindowOpenInSteam", kParams_SteamFriends015_61},
    {"SteamAPI_ISteamFriends_OpenClanChatWindowInSteam", kParams_SteamFriends015_62},
    {"SteamAPI_ISteamFriends_CloseClanChatWindowInSteam", kParams_SteamFriends015_63},
    {"SteamAPI_ISteamFriends_SetListenForFriendsMessages", kParams_SteamFriends015_64},
    {"SteamAPI_ISteamFriends_ReplyToFriendMessage", kParams_SteamFriends015_65},
    {"SteamAPI_ISteamFriends_GetFriendMessage", kParams_SteamFriends015_66},
    {"SteamAPI_ISteamFriends_GetFollowerCount", kParams_SteamFriends015_67},
    {"SteamAPI_ISteamFriends_IsFollowing", kParams_SteamFriends015_68},
    {"SteamAPI_ISteamFriends_EnumerateFollowingList", kParams_SteamFriends015_69},
};

class Version_SteamFriends015 {
public:
    virtual const char* GetPersonaName() {
        return steambridge::slot<const char*>(kSlots_SteamFriends015[0]);
    }
    virtual std::uint64_t SetPersonaName(const char* pchPersonaName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamFriends015[1], pchPersonaName);
    }
    virtual std::int32_t GetPersonaState() {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends015[2]);
    }
    virtual std::int32_t GetFriendCount(std::int32_t iFriendFlags) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends015[3], iFriendFlags);
    }
    virtual CSteamID GetFriendByIndex(std::int32_t iFriend, std::int32_t iFriendFlags) {
        return steambridge::slot<CSteamID>(kSlots_SteamFriends015[4], iFriend, iFriendFlags);
    }
    virtual std::int32_t GetFriendRelationship(CSteamID steamIDFriend) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends015[5], steamIDFriend);
    }
    virtual std::int32_t GetFriendPersonaState(CSteamID steamIDFriend) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends015[6], steamIDFriend);
    }
    virtual const char* GetFriendPersonaName(CSteamID steamIDFriend) {
        return steambridge::slot<const char*>(kSlots_SteamFriends015[7], steamIDFriend);
    }
    virtual bool GetFriendGamePlayed(CSteamID steamIDFriend, void* pFriendGameInfo) {
        return steambridge::slot<bool>(kSlots_SteamFriends015[8], steamIDFriend, pFriendGameInfo);
    }
    virtual const char* GetFriendPersonaNameHistory(CSteamID steamIDFriend, std::int32_t iPersonaName) {
        return steambridge::slot<const char*>(kSlots_SteamFriends015[9], steamIDFriend, iPersonaName);
    }
    virtual std::int32_t GetFriendSteamLevel(CSteamID steamIDFriend) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends015[10], steamIDFriend);
    }
    virtual const char* GetPlayerNickname(CSteamID steamIDPlayer) {
        return steambridge::slot<const char*>(kSlots_SteamFriends015[11], steamIDPlayer);
    }
    virtual std::int32_t GetFriendsGroupCount() {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends015[12]);
    }
    virtual std::int16_t GetFriendsGroupIDByIndex(std::int32_t iFG) {
        return steambridge::slot<std::int16_t>(kSlots_SteamFriends015[13], iFG);
    }
    virtual const char* GetFriendsGroupName(std::int16_t friendsGroupID) {
        return steambridge::slot<const char*>(kSlots_SteamFriends015[14], friendsGroupID);
    }
    virtual std::int32_t GetFriendsGroupMembersCount(std::int16_t friendsGroupID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends015[15], friendsGroupID);
    }
    virtual void GetFriendsGroupMembersList(std::int16_t friendsGroupID, CSteamID* pOutSteamIDMembers, std::int32_t nMembersCount) {
        steambridge::slot<void>(kSlots_SteamFriends015[16], friendsGroupID, pOutSteamIDMembers, nMembersCount);
    }
    virtual bool HasFriend(CSteamID steamIDFriend, std::int32_t iFriendFlags) {
        return steambridge::slot<bool>(kSlots_SteamFriends015[17], steamIDFriend, iFriendFlags);
    }
    virtual std::int32_t GetClanCount() {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends015[18]);
    }
    virtual CSteamID GetClanByIndex(std::int32_t iClan) {
        return steambridge::slot<CSteamID>(kSlots_SteamFriends015[19], iClan);
    }
    virtual const char* GetClanName(CSteamID steamIDClan) {
        return steambridge::slot<const char*>(kSlots_SteamFriends015[20], steamIDClan);
    }
    virtual const char* GetClanTag(CSteamID steamIDClan) {
        return steambridge::slot<const char*>(kSlots_SteamFriends015[21], steamIDClan);
    }
    virtual bool GetClanActivityCounts(CSteamID steamIDClan, std::int32_t* pnOnline, std::int32_t* pnInGame, std::int32_t* pnChatting) {
        return steambridge::slot<bool>(kSlots_SteamFriends015[22], steamIDClan, pnOnline, pnInGame, pnChatting);
    }
    virtual std::uint64_t DownloadClanActivityCounts(CSteamID* psteamIDClans, std::int32_t cClansToRequest) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamFriends015[23], psteamIDClans, cClansToRequest);
    }
    virtual std::int32_t GetFriendCountFromSource(CSteamID steamIDSource) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends015[24], steamIDSource);
    }
    virtual CSteamID GetFriendFromSourceByIndex(CSteamID steamIDSource, std::int32_t iFriend) {
        return steambridge::slot<CSteamID>(kSlots_SteamFriends015[25], steamIDSource, iFriend);
    }
    virtual bool IsUserInSource(CSteamID steamIDUser, CSteamID steamIDSource) {
        return steambridge::slot<bool>(kSlots_SteamFriends015[26], steamIDUser, steamIDSource);
    }
    virtual void SetInGameVoiceSpeaking(CSteamID steamIDUser, bool bSpeaking) {
        steambridge::slot<void>(kSlots_SteamFriends015[27], steamIDUser, bSpeaking);
    }
    virtual void ActivateGameOverlay(const char* pchDialog) {
        steambridge::slot<void>(kSlots_SteamFriends015[28], pchDialog);
    }
    virtual void ActivateGameOverlayToUser(const char* pchDialog, CSteamID steamID) {
        steambridge::slot<void>(kSlots_SteamFriends015[29], pchDialog, steamID);
    }
    virtual void ActivateGameOverlayToWebPage(const char* pchURL) {
        steambridge::slot<void>(kSlots_SteamFriends015[30], pchURL);
    }
    virtual void ActivateGameOverlayToStore(std::uint32_t nAppID, std::int32_t eFlag) {
        steambridge::slot<void>(kSlots_SteamFriends015[31], nAppID, eFlag);
    }
    virtual void SetPlayedWith(CSteamID steamIDUserPlayedWith) {
        steambridge::slot<void>(kSlots_SteamFriends015[32], steamIDUserPlayedWith);
    }
    virtual void ActivateGameOverlayInviteDialog(CSteamID steamIDLobby) {
        steambridge::slot<void>(kSlots_SteamFriends015[33], steamIDLobby);
    }
    virtual std::int32_t GetSmallFriendAvatar(CSteamID steamIDFriend) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends015[34], steamIDFriend);
    }
    virtual std::int32_t GetMediumFriendAvatar(CSteamID steamIDFriend) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends015[35], steamIDFriend);
    }
    virtual std::int32_t GetLargeFriendAvatar(CSteamID steamIDFriend) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends015[36], steamIDFriend);
    }
    virtual bool RequestUserInformation(CSteamID steamIDUser, bool bRequireNameOnly) {
        return steambridge::slot<bool>(kSlots_SteamFriends015[37], steamIDUser, bRequireNameOnly);
    }
    virtual std::uint64_t RequestClanOfficerList(CSteamID steamIDClan) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamFriends015[38], steamIDClan);
    }
    virtual CSteamID GetClanOwner(CSteamID steamIDClan) {
        return steambridge::slot<CSteamID>(kSlots_SteamFriends015[39], steamIDClan);
    }
    virtual std::int32_t GetClanOfficerCount(CSteamID steamIDClan) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends015[40], steamIDClan);
    }
    virtual CSteamID GetClanOfficerByIndex(CSteamID steamIDClan, std::int32_t iOfficer) {
        return steambridge::slot<CSteamID>(kSlots_SteamFriends015[41], steamIDClan, iOfficer);
    }
    virtual std::uint32_t GetUserRestrictions() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamFriends015[42]);
    }
    virtual bool SetRichPresence(const char* pchKey, const char* pchValue) {
        return steambridge::slot<bool>(kSlots_SteamFriends015[43], pchKey, pchValue);
    }
    virtual void ClearRichPresence() {
        steambridge::slot<void>(kSlots_SteamFriends015[44]);
    }
    virtual const char* GetFriendRichPresence(CSteamID steamIDFriend, const char* pchKey) {
        return steambridge::slot<const char*>(kSlots_SteamFriends015[45], steamIDFriend, pchKey);
    }
    virtual std::int32_t GetFriendRichPresenceKeyCount(CSteamID steamIDFriend) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends015[46], steamIDFriend);
    }
    virtual const char* GetFriendRichPresenceKeyByIndex(CSteamID steamIDFriend, std::int32_t iKey) {
        return steambridge::slot<const char*>(kSlots_SteamFriends015[47], steamIDFriend, iKey);
    }
    virtual void RequestFriendRichPresence(CSteamID steamIDFriend) {
        steambridge::slot<void>(kSlots_SteamFriends015[48], steamIDFriend);
    }
    virtual bool InviteUserToGame(CSteamID steamIDFriend, const char* pchConnectString) {
        return steambridge::slot<bool>(kSlots_SteamFriends015[49], steamIDFriend, pchConnectString);
    }
    virtual std::int32_t GetCoplayFriendCount() {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends015[50]);
    }
    virtual CSteamID GetCoplayFriend(std::int32_t iCoplayFriend) {
        return steambridge::slot<CSteamID>(kSlots_SteamFriends015[51], iCoplayFriend);
    }
    virtual std::int32_t GetFriendCoplayTime(CSteamID steamIDFriend) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends015[52], steamIDFriend);
    }
    virtual std::uint32_t GetFriendCoplayGame(CSteamID steamIDFriend) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamFriends015[53], steamIDFriend);
    }
    virtual std::uint64_t JoinClanChatRoom(CSteamID steamIDClan) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamFriends015[54], steamIDClan);
    }
    virtual bool LeaveClanChatRoom(CSteamID steamIDClan) {
        return steambridge::slot<bool>(kSlots_SteamFriends015[55], steamIDClan);
    }
    virtual std::int32_t GetClanChatMemberCount(CSteamID steamIDClan) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends015[56], steamIDClan);
    }
    virtual CSteamID GetChatMemberByIndex(CSteamID steamIDClan, std::int32_t iUser) {
        return steambridge::slot<CSteamID>(kSlots_SteamFriends015[57], steamIDClan, iUser);
    }
    virtual bool SendClanChatMessage(CSteamID steamIDClanChat, const char* pchText) {
        return steambridge::slot<bool>(kSlots_SteamFriends015[58], steamIDClanChat, pchText);
    }
    virtual std::int32_t GetClanChatMessage(CSteamID steamIDClanChat, std::int32_t iMessage, void* prgchText, std::int32_t cchTextMax, std::int32_t* peChatEntryType, CSteamID* psteamidChatter) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends015[59], steamIDClanChat, iMessage, prgchText, cchTextMax, peChatEntryType, psteamidChatter);
    }
    virtual bool IsClanChatAdmin(CSteamID steamIDClanChat, CSteamID steamIDUser) {
        return steambridge::slot<bool>(kSlots_SteamFriends015[60], steamIDClanChat, steamIDUser);
    }
    virtual bool IsClanChatWindowOpenInSteam(CSteamID steamIDClanChat) {
        return steambridge::slot<bool>(kSlots_SteamFriends015[61], steamIDClanChat);
    }
    virtual bool OpenClanChatWindowInSteam(CSteamID steamIDClanChat) {
        return steambridge::slot<bool>(kSlots_SteamFriends015[62], steamIDClanChat);
    }
    virtual bool CloseClanChatWindowInSteam(CSteamID steamIDClanChat) {
        return steambridge::slot<bool>(kSlots_SteamFriends015[63], steamIDClanChat);
    }
    virtual bool SetListenForFriendsMessages(bool bInterceptEnabled) {
        return steambridge::slot<bool>(kSlots_SteamFriends015[64], bInterceptEnabled);
    }
    virtual bool ReplyToFriendMessage(CSteamID steamIDFriend, const char* pchMsgToSend) {
        return steambridge::slot<bool>(kSlots_SteamFriends015[65], steamIDFriend, pchMsgToSend);
    }
    virtual std::int32_t GetFriendMessage(CSteamID steamIDFriend, std::int32_t iMessageID, void* pvData, std::int32_t cubData, std::int32_t* peChatEntryType) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends015[66], steamIDFriend, iMessageID, pvData, cubData, peChatEntryType);
    }
    virtual std::uint64_t GetFollowerCount(CSteamID steamID) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamFriends015[67], steamID);
    }
    virtual std::uint64_t IsFollowing(CSteamID steamID) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamFriends015[68], steamID);
    }
    virtual std::uint64_t EnumerateFollowingList(std::uint32_t unStartIndex) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamFriends015[69], unStartIndex);
    }
};

Version_SteamFriends015 g_SteamFriends015;

const char* const kParams_SteamFriends017_1[] = {
    "pchPersonaName",
};
const char* const kParams_SteamFriends017_3[] = {
    "iFriendFlags",
};
const char* const kParams_SteamFriends017_4[] = {
    "iFriend",
    "iFriendFlags",
};
const char* const kParams_SteamFriends017_5[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends017_6[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends017_7[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends017_8[] = {
    "steamIDFriend",
    "pFriendGameInfo",
};
const char* const kParams_SteamFriends017_9[] = {
    "steamIDFriend",
    "iPersonaName",
};
const char* const kParams_SteamFriends017_10[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends017_11[] = {
    "steamIDPlayer",
};
const char* const kParams_SteamFriends017_13[] = {
    "iFG",
};
const char* const kParams_SteamFriends017_14[] = {
    "friendsGroupID",
};
const char* const kParams_SteamFriends017_15[] = {
    "friendsGroupID",
};
const char* const kParams_SteamFriends017_16[] = {
    "friendsGroupID",
    "pOutSteamIDMembers",
    "nMembersCount",
};
const char* const kParams_SteamFriends017_17[] = {
    "steamIDFriend",
    "iFriendFlags",
};
const char* const kParams_SteamFriends017_19[] = {
    "iClan",
};
const char* const kParams_SteamFriends017_20[] = {
    "steamIDClan",
};
const char* const kParams_SteamFriends017_21[] = {
    "steamIDClan",
};
const char* const kParams_SteamFriends017_22[] = {
    "steamIDClan",
    "pnOnline",
    "pnInGame",
    "pnChatting",
};
const char* const kParams_SteamFriends017_23[] = {
    "psteamIDClans",
    "cClansToRequest",
};
const char* const kParams_SteamFriends017_24[] = {
    "steamIDSource",
};
const char* const kParams_SteamFriends017_25[] = {
    "steamIDSource",
    "iFriend",
};
const char* const kParams_SteamFriends017_26[] = {
    "steamIDUser",
    "steamIDSource",
};
const char* const kParams_SteamFriends017_27[] = {
    "steamIDUser",
    "bSpeaking",
};
const char* const kParams_SteamFriends017_28[] = {
    "pchDialog",
};
const char* const kParams_SteamFriends017_29[] = {
    "pchDialog",
    "steamID",
};
const char* const kParams_SteamFriends017_30[] = {
    "pchURL",
    "eMode",
};
const char* const kParams_SteamFriends017_31[] = {
    "nAppID",
    "eFlag",
};
const char* const kParams_SteamFriends017_32[] = {
    "steamIDUserPlayedWith",
};
const char* const kParams_SteamFriends017_33[] = {
    "steamIDLobby",
};
const char* const kParams_SteamFriends017_34[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends017_35[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends017_36[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends017_37[] = {
    "steamIDUser",
    "bRequireNameOnly",
};
const char* const kParams_SteamFriends017_38[] = {
    "steamIDClan",
};
const char* const kParams_SteamFriends017_39[] = {
    "steamIDClan",
};
const char* const kParams_SteamFriends017_40[] = {
    "steamIDClan",
};
const char* const kParams_SteamFriends017_41[] = {
    "steamIDClan",
    "iOfficer",
};
const char* const kParams_SteamFriends017_43[] = {
    "pchKey",
    "pchValue",
};
const char* const kParams_SteamFriends017_45[] = {
    "steamIDFriend",
    "pchKey",
};
const char* const kParams_SteamFriends017_46[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends017_47[] = {
    "steamIDFriend",
    "iKey",
};
const char* const kParams_SteamFriends017_48[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends017_49[] = {
    "steamIDFriend",
    "pchConnectString",
};
const char* const kParams_SteamFriends017_51[] = {
    "iCoplayFriend",
};
const char* const kParams_SteamFriends017_52[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends017_53[] = {
    "steamIDFriend",
};
const char* const kParams_SteamFriends017_54[] = {
    "steamIDClan",
};
const char* const kParams_SteamFriends017_55[] = {
    "steamIDClan",
};
const char* const kParams_SteamFriends017_56[] = {
    "steamIDClan",
};
const char* const kParams_SteamFriends017_57[] = {
    "steamIDClan",
    "iUser",
};
const char* const kParams_SteamFriends017_58[] = {
    "steamIDClanChat",
    "pchText",
};
const char* const kParams_SteamFriends017_59[] = {
    "steamIDClanChat",
    "iMessage",
    "prgchText",
    "cchTextMax",
    "peChatEntryType",
    "psteamidChatter",
};
const char* const kParams_SteamFriends017_60[] = {
    "steamIDClanChat",
    "steamIDUser",
};
const char* const kParams_SteamFriends017_61[] = {
    "steamIDClanChat",
};
const char* const kParams_SteamFriends017_62[] = {
    "steamIDClanChat",
};
const char* const kParams_SteamFriends017_63[] = {
    "steamIDClanChat",
};
const char* const kParams_SteamFriends017_64[] = {
    "bInterceptEnabled",
};
const char* const kParams_SteamFriends017_65[] = {
    "steamIDFriend",
    "pchMsgToSend",
};
const char* const kParams_SteamFriends017_66[] = {
    "steamIDFriend",
    "iMessageID",
    "pvData",
    "cubData",
    "peChatEntryType",
};
const char* const kParams_SteamFriends017_67[] = {
    "steamID",
};
const char* const kParams_SteamFriends017_68[] = {
    "steamID",
};
const char* const kParams_SteamFriends017_69[] = {
    "unStartIndex",
};
const char* const kParams_SteamFriends017_70[] = {
    "steamIDClan",
};
const char* const kParams_SteamFriends017_71[] = {
    "steamIDClan",
};
const char* const kParams_SteamFriends017_73[] = {
    "steamIDLobby",
};
const char* const kParams_SteamFriends017_74[] = {
    "pchProtocol",
};
const char* const kParams_SteamFriends017_75[] = {
    "pchConnectString",
};
const char* const kParams_SteamFriends017_76[] = {
    "steamID",
};
const char* const kParams_SteamFriends017_77[] = {
    "steamID",
    "itemType",
};
const char* const kParams_SteamFriends017_78[] = {
    "steamID",
    "itemType",
    "prop",
};
const char* const kParams_SteamFriends017_79[] = {
    "steamID",
    "itemType",
    "prop",
};
// ISteamFriends SteamFriends017
const steambridge::SlotInfo kSlots_SteamFriends017[] = {
    {"SteamAPI_ISteamFriends_GetPersonaName", nullptr},
    {"SteamAPI_ISteamFriends_SetPersonaName", kParams_SteamFriends017_1},
    {"SteamAPI_ISteamFriends_GetPersonaState", nullptr},
    {"SteamAPI_ISteamFriends_GetFriendCount", kParams_SteamFriends017_3},
    {"SteamAPI_ISteamFriends_GetFriendByIndex", kParams_SteamFriends017_4},
    {"SteamAPI_ISteamFriends_GetFriendRelationship", kParams_SteamFriends017_5},
    {"SteamAPI_ISteamFriends_GetFriendPersonaState", kParams_SteamFriends017_6},
    {"SteamAPI_ISteamFriends_GetFriendPersonaName", kParams_SteamFriends017_7},
    {"SteamAPI_ISteamFriends_GetFriendGamePlayed", kParams_SteamFriends017_8},
    {"SteamAPI_ISteamFriends_GetFriendPersonaNameHistory", kParams_SteamFriends017_9},
    {"SteamAPI_ISteamFriends_GetFriendSteamLevel", kParams_SteamFriends017_10},
    {"SteamAPI_ISteamFriends_GetPlayerNickname", kParams_SteamFriends017_11},
    {"SteamAPI_ISteamFriends_GetFriendsGroupCount", nullptr},
    {"SteamAPI_ISteamFriends_GetFriendsGroupIDByIndex", kParams_SteamFriends017_13},
    {"SteamAPI_ISteamFriends_GetFriendsGroupName", kParams_SteamFriends017_14},
    {"SteamAPI_ISteamFriends_GetFriendsGroupMembersCount", kParams_SteamFriends017_15},
    {"SteamAPI_ISteamFriends_GetFriendsGroupMembersList", kParams_SteamFriends017_16},
    {"SteamAPI_ISteamFriends_HasFriend", kParams_SteamFriends017_17},
    {"SteamAPI_ISteamFriends_GetClanCount", nullptr},
    {"SteamAPI_ISteamFriends_GetClanByIndex", kParams_SteamFriends017_19},
    {"SteamAPI_ISteamFriends_GetClanName", kParams_SteamFriends017_20},
    {"SteamAPI_ISteamFriends_GetClanTag", kParams_SteamFriends017_21},
    {"SteamAPI_ISteamFriends_GetClanActivityCounts", kParams_SteamFriends017_22},
    {"SteamAPI_ISteamFriends_DownloadClanActivityCounts", kParams_SteamFriends017_23},
    {"SteamAPI_ISteamFriends_GetFriendCountFromSource", kParams_SteamFriends017_24},
    {"SteamAPI_ISteamFriends_GetFriendFromSourceByIndex", kParams_SteamFriends017_25},
    {"SteamAPI_ISteamFriends_IsUserInSource", kParams_SteamFriends017_26},
    {"SteamAPI_ISteamFriends_SetInGameVoiceSpeaking", kParams_SteamFriends017_27},
    {"SteamAPI_ISteamFriends_ActivateGameOverlay", kParams_SteamFriends017_28},
    {"SteamAPI_ISteamFriends_ActivateGameOverlayToUser", kParams_SteamFriends017_29},
    {"SteamAPI_ISteamFriends_ActivateGameOverlayToWebPage", kParams_SteamFriends017_30},
    {"SteamAPI_ISteamFriends_ActivateGameOverlayToStore", kParams_SteamFriends017_31},
    {"SteamAPI_ISteamFriends_SetPlayedWith", kParams_SteamFriends017_32},
    {"SteamAPI_ISteamFriends_ActivateGameOverlayInviteDialog", kParams_SteamFriends017_33},
    {"SteamAPI_ISteamFriends_GetSmallFriendAvatar", kParams_SteamFriends017_34},
    {"SteamAPI_ISteamFriends_GetMediumFriendAvatar", kParams_SteamFriends017_35},
    {"SteamAPI_ISteamFriends_GetLargeFriendAvatar", kParams_SteamFriends017_36},
    {"SteamAPI_ISteamFriends_RequestUserInformation", kParams_SteamFriends017_37},
    {"SteamAPI_ISteamFriends_RequestClanOfficerList", kParams_SteamFriends017_38},
    {"SteamAPI_ISteamFriends_GetClanOwner", kParams_SteamFriends017_39},
    {"SteamAPI_ISteamFriends_GetClanOfficerCount", kParams_SteamFriends017_40},
    {"SteamAPI_ISteamFriends_GetClanOfficerByIndex", kParams_SteamFriends017_41},
    {"SteamAPI_ISteamFriends_GetUserRestrictions", nullptr},
    {"SteamAPI_ISteamFriends_SetRichPresence", kParams_SteamFriends017_43},
    {"SteamAPI_ISteamFriends_ClearRichPresence", nullptr},
    {"SteamAPI_ISteamFriends_GetFriendRichPresence", kParams_SteamFriends017_45},
    {"SteamAPI_ISteamFriends_GetFriendRichPresenceKeyCount", kParams_SteamFriends017_46},
    {"SteamAPI_ISteamFriends_GetFriendRichPresenceKeyByIndex", kParams_SteamFriends017_47},
    {"SteamAPI_ISteamFriends_RequestFriendRichPresence", kParams_SteamFriends017_48},
    {"SteamAPI_ISteamFriends_InviteUserToGame", kParams_SteamFriends017_49},
    {"SteamAPI_ISteamFriends_GetCoplayFriendCount", nullptr},
    {"SteamAPI_ISteamFriends_GetCoplayFriend", kParams_SteamFriends017_51},
    {"SteamAPI_ISteamFriends_GetFriendCoplayTime", kParams_SteamFriends017_52},
    {"SteamAPI_ISteamFriends_GetFriendCoplayGame", kParams_SteamFriends017_53},
    {"SteamAPI_ISteamFriends_JoinClanChatRoom", kParams_SteamFriends017_54},
    {"SteamAPI_ISteamFriends_LeaveClanChatRoom", kParams_SteamFriends017_55},
    {"SteamAPI_ISteamFriends_GetClanChatMemberCount", kParams_SteamFriends017_56},
    {"SteamAPI_ISteamFriends_GetChatMemberByIndex", kParams_SteamFriends017_57},
    {"SteamAPI_ISteamFriends_SendClanChatMessage", kParams_SteamFriends017_58},
    {"SteamAPI_ISteamFriends_GetClanChatMessage", kParams_SteamFriends017_59},
    {"SteamAPI_ISteamFriends_IsClanChatAdmin", kParams_SteamFriends017_60},
    {"SteamAPI_ISteamFriends_IsClanChatWindowOpenInSteam", kParams_SteamFriends017_61},
    {"SteamAPI_ISteamFriends_OpenClanChatWindowInSteam", kParams_SteamFriends017_62},
    {"SteamAPI_ISteamFriends_CloseClanChatWindowInSteam", kParams_SteamFriends017_63},
    {"SteamAPI_ISteamFriends_SetListenForFriendsMessages", kParams_SteamFriends017_64},
    {"SteamAPI_ISteamFriends_ReplyToFriendMessage", kParams_SteamFriends017_65},
    {"SteamAPI_ISteamFriends_GetFriendMessage", kParams_SteamFriends017_66},
    {"SteamAPI_ISteamFriends_GetFollowerCount", kParams_SteamFriends017_67},
    {"SteamAPI_ISteamFriends_IsFollowing", kParams_SteamFriends017_68},
    {"SteamAPI_ISteamFriends_EnumerateFollowingList", kParams_SteamFriends017_69},
    {"SteamAPI_ISteamFriends_IsClanPublic", kParams_SteamFriends017_70},
    {"SteamAPI_ISteamFriends_IsClanOfficialGameGroup", kParams_SteamFriends017_71},
    {"SteamAPI_ISteamFriends_GetNumChatsWithUnreadPriorityMessages", nullptr},
    {"SteamAPI_ISteamFriends_ActivateGameOverlayRemotePlayTogetherInviteDialog", kParams_SteamFriends017_73},
    {"SteamAPI_ISteamFriends_RegisterProtocolInOverlayBrowser", kParams_SteamFriends017_74},
    {"SteamAPI_ISteamFriends_ActivateGameOverlayInviteDialogConnectString", kParams_SteamFriends017_75},
    {"SteamAPI_ISteamFriends_RequestEquippedProfileItems", kParams_SteamFriends017_76},
    {"SteamAPI_ISteamFriends_BHasEquippedProfileItem", kParams_SteamFriends017_77},
    {"SteamAPI_ISteamFriends_GetProfileItemPropertyString", kParams_SteamFriends017_78},
    {"SteamAPI_ISteamFriends_GetProfileItemPropertyUint", kParams_SteamFriends017_79},
};

class Version_SteamFriends017 {
public:
    virtual const char* GetPersonaName() {
        return steambridge::slot<const char*>(kSlots_SteamFriends017[0]);
    }
    virtual std::uint64_t SetPersonaName(const char* pchPersonaName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamFriends017[1], pchPersonaName);
    }
    virtual std::int32_t GetPersonaState() {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends017[2]);
    }
    virtual std::int32_t GetFriendCount(std::int32_t iFriendFlags) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends017[3], iFriendFlags);
    }
    virtual CSteamID GetFriendByIndex(std::int32_t iFriend, std::int32_t iFriendFlags) {
        return steambridge::slot<CSteamID>(kSlots_SteamFriends017[4], iFriend, iFriendFlags);
    }
    virtual std::int32_t GetFriendRelationship(CSteamID steamIDFriend) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends017[5], steamIDFriend);
    }
    virtual std::int32_t GetFriendPersonaState(CSteamID steamIDFriend) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends017[6], steamIDFriend);
    }
    virtual const char* GetFriendPersonaName(CSteamID steamIDFriend) {
        return steambridge::slot<const char*>(kSlots_SteamFriends017[7], steamIDFriend);
    }
    virtual bool GetFriendGamePlayed(CSteamID steamIDFriend, void* pFriendGameInfo) {
        return steambridge::slot<bool>(kSlots_SteamFriends017[8], steamIDFriend, pFriendGameInfo);
    }
    virtual const char* GetFriendPersonaNameHistory(CSteamID steamIDFriend, std::int32_t iPersonaName) {
        return steambridge::slot<const char*>(kSlots_SteamFriends017[9], steamIDFriend, iPersonaName);
    }
    virtual std::int32_t GetFriendSteamLevel(CSteamID steamIDFriend) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends017[10], steamIDFriend);
    }
    virtual const char* GetPlayerNickname(CSteamID steamIDPlayer) {
        return steambridge::slot<const char*>(kSlots_SteamFriends017[11], steamIDPlayer);
    }
    virtual std::int32_t GetFriendsGroupCount() {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends017[12]);
    }
    virtual std::int16_t GetFriendsGroupIDByIndex(std::int32_t iFG) {
        return steambridge::slot<std::int16_t>(kSlots_SteamFriends017[13], iFG);
    }
    virtual const char* GetFriendsGroupName(std::int16_t friendsGroupID) {
        return steambridge::slot<const char*>(kSlots_SteamFriends017[14], friendsGroupID);
    }
    virtual std::int32_t GetFriendsGroupMembersCount(std::int16_t friendsGroupID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends017[15], friendsGroupID);
    }
    virtual void GetFriendsGroupMembersList(std::int16_t friendsGroupID, CSteamID* pOutSteamIDMembers, std::int32_t nMembersCount) {
        steambridge::slot<void>(kSlots_SteamFriends017[16], friendsGroupID, pOutSteamIDMembers, nMembersCount);
    }
    virtual bool HasFriend(CSteamID steamIDFriend, std::int32_t iFriendFlags) {
        return steambridge::slot<bool>(kSlots_SteamFriends017[17], steamIDFriend, iFriendFlags);
    }
    virtual std::int32_t GetClanCount() {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends017[18]);
    }
    virtual CSteamID GetClanByIndex(std::int32_t iClan) {
        return steambridge::slot<CSteamID>(kSlots_SteamFriends017[19], iClan);
    }
    virtual const char* GetClanName(CSteamID steamIDClan) {
        return steambridge::slot<const char*>(kSlots_SteamFriends017[20], steamIDClan);
    }
    virtual const char* GetClanTag(CSteamID steamIDClan) {
        return steambridge::slot<const char*>(kSlots_SteamFriends017[21], steamIDClan);
    }
    virtual bool GetClanActivityCounts(CSteamID steamIDClan, std::int32_t* pnOnline, std::int32_t* pnInGame, std::int32_t* pnChatting) {
        return steambridge::slot<bool>(kSlots_SteamFriends017[22], steamIDClan, pnOnline, pnInGame, pnChatting);
    }
    virtual std::uint64_t DownloadClanActivityCounts(CSteamID* psteamIDClans, std::int32_t cClansToRequest) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamFriends017[23], psteamIDClans, cClansToRequest);
    }
    virtual std::int32_t GetFriendCountFromSource(CSteamID steamIDSource) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends017[24], steamIDSource);
    }
    virtual CSteamID GetFriendFromSourceByIndex(CSteamID steamIDSource, std::int32_t iFriend) {
        return steambridge::slot<CSteamID>(kSlots_SteamFriends017[25], steamIDSource, iFriend);
    }
    virtual bool IsUserInSource(CSteamID steamIDUser, CSteamID steamIDSource) {
        return steambridge::slot<bool>(kSlots_SteamFriends017[26], steamIDUser, steamIDSource);
    }
    virtual void SetInGameVoiceSpeaking(CSteamID steamIDUser, bool bSpeaking) {
        steambridge::slot<void>(kSlots_SteamFriends017[27], steamIDUser, bSpeaking);
    }
    virtual void ActivateGameOverlay(const char* pchDialog) {
        steambridge::slot<void>(kSlots_SteamFriends017[28], pchDialog);
    }
    virtual void ActivateGameOverlayToUser(const char* pchDialog, CSteamID steamID) {
        steambridge::slot<void>(kSlots_SteamFriends017[29], pchDialog, steamID);
    }
    virtual void ActivateGameOverlayToWebPage(const char* pchURL, std::int32_t eMode) {
        steambridge::slot<void>(kSlots_SteamFriends017[30], pchURL, eMode);
    }
    virtual void ActivateGameOverlayToStore(std::uint32_t nAppID, std::int32_t eFlag) {
        steambridge::slot<void>(kSlots_SteamFriends017[31], nAppID, eFlag);
    }
    virtual void SetPlayedWith(CSteamID steamIDUserPlayedWith) {
        steambridge::slot<void>(kSlots_SteamFriends017[32], steamIDUserPlayedWith);
    }
    virtual void ActivateGameOverlayInviteDialog(CSteamID steamIDLobby) {
        steambridge::slot<void>(kSlots_SteamFriends017[33], steamIDLobby);
    }
    virtual std::int32_t GetSmallFriendAvatar(CSteamID steamIDFriend) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends017[34], steamIDFriend);
    }
    virtual std::int32_t GetMediumFriendAvatar(CSteamID steamIDFriend) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends017[35], steamIDFriend);
    }
    virtual std::int32_t GetLargeFriendAvatar(CSteamID steamIDFriend) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends017[36], steamIDFriend);
    }
    virtual bool RequestUserInformation(CSteamID steamIDUser, bool bRequireNameOnly) {
        return steambridge::slot<bool>(kSlots_SteamFriends017[37], steamIDUser, bRequireNameOnly);
    }
    virtual std::uint64_t RequestClanOfficerList(CSteamID steamIDClan) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamFriends017[38], steamIDClan);
    }
    virtual CSteamID GetClanOwner(CSteamID steamIDClan) {
        return steambridge::slot<CSteamID>(kSlots_SteamFriends017[39], steamIDClan);
    }
    virtual std::int32_t GetClanOfficerCount(CSteamID steamIDClan) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends017[40], steamIDClan);
    }
    virtual CSteamID GetClanOfficerByIndex(CSteamID steamIDClan, std::int32_t iOfficer) {
        return steambridge::slot<CSteamID>(kSlots_SteamFriends017[41], steamIDClan, iOfficer);
    }
    virtual std::uint32_t GetUserRestrictions() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamFriends017[42]);
    }
    virtual bool SetRichPresence(const char* pchKey, const char* pchValue) {
        return steambridge::slot<bool>(kSlots_SteamFriends017[43], pchKey, pchValue);
    }
    virtual void ClearRichPresence() {
        steambridge::slot<void>(kSlots_SteamFriends017[44]);
    }
    virtual const char* GetFriendRichPresence(CSteamID steamIDFriend, const char* pchKey) {
        return steambridge::slot<const char*>(kSlots_SteamFriends017[45], steamIDFriend, pchKey);
    }
    virtual std::int32_t GetFriendRichPresenceKeyCount(CSteamID steamIDFriend) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends017[46], steamIDFriend);
    }
    virtual const char* GetFriendRichPresenceKeyByIndex(CSteamID steamIDFriend, std::int32_t iKey) {
        return steambridge::slot<const char*>(kSlots_SteamFriends017[47], steamIDFriend, iKey);
    }
    virtual void RequestFriendRichPresence(CSteamID steamIDFriend) {
        steambridge::slot<void>(kSlots_SteamFriends017[48], steamIDFriend);
    }
    virtual bool InviteUserToGame(CSteamID steamIDFriend, const char* pchConnectString) {
        return steambridge::slot<bool>(kSlots_SteamFriends017[49], steamIDFriend, pchConnectString);
    }
    virtual std::int32_t GetCoplayFriendCount() {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends017[50]);
    }
    virtual CSteamID GetCoplayFriend(std::int32_t iCoplayFriend) {
        return steambridge::slot<CSteamID>(kSlots_SteamFriends017[51], iCoplayFriend);
    }
    virtual std::int32_t GetFriendCoplayTime(CSteamID steamIDFriend) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends017[52], steamIDFriend);
    }
    virtual std::uint32_t GetFriendCoplayGame(CSteamID steamIDFriend) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamFriends017[53], steamIDFriend);
    }
    virtual std::uint64_t JoinClanChatRoom(CSteamID steamIDClan) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamFriends017[54], steamIDClan);
    }
    virtual bool LeaveClanChatRoom(CSteamID steamIDClan) {
        return steambridge::slot<bool>(kSlots_SteamFriends017[55], steamIDClan);
    }
    virtual std::int32_t GetClanChatMemberCount(CSteamID steamIDClan) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends017[56], steamIDClan);
    }
    virtual CSteamID GetChatMemberByIndex(CSteamID steamIDClan, std::int32_t iUser) {
        return steambridge::slot<CSteamID>(kSlots_SteamFriends017[57], steamIDClan, iUser);
    }
    virtual bool SendClanChatMessage(CSteamID steamIDClanChat, const char* pchText) {
        return steambridge::slot<bool>(kSlots_SteamFriends017[58], steamIDClanChat, pchText);
    }
    virtual std::int32_t GetClanChatMessage(CSteamID steamIDClanChat, std::int32_t iMessage, void* prgchText, std::int32_t cchTextMax, std::int32_t* peChatEntryType, CSteamID* psteamidChatter) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends017[59], steamIDClanChat, iMessage, prgchText, cchTextMax, peChatEntryType, psteamidChatter);
    }
    virtual bool IsClanChatAdmin(CSteamID steamIDClanChat, CSteamID steamIDUser) {
        return steambridge::slot<bool>(kSlots_SteamFriends017[60], steamIDClanChat, steamIDUser);
    }
    virtual bool IsClanChatWindowOpenInSteam(CSteamID steamIDClanChat) {
        return steambridge::slot<bool>(kSlots_SteamFriends017[61], steamIDClanChat);
    }
    virtual bool OpenClanChatWindowInSteam(CSteamID steamIDClanChat) {
        return steambridge::slot<bool>(kSlots_SteamFriends017[62], steamIDClanChat);
    }
    virtual bool CloseClanChatWindowInSteam(CSteamID steamIDClanChat) {
        return steambridge::slot<bool>(kSlots_SteamFriends017[63], steamIDClanChat);
    }
    virtual bool SetListenForFriendsMessages(bool bInterceptEnabled) {
        return steambridge::slot<bool>(kSlots_SteamFriends017[64], bInterceptEnabled);
    }
    virtual bool ReplyToFriendMessage(CSteamID steamIDFriend, const char* pchMsgToSend) {
        return steambridge::slot<bool>(kSlots_SteamFriends017[65], steamIDFriend, pchMsgToSend);
    }
    virtual std::int32_t GetFriendMessage(CSteamID steamIDFriend, std::int32_t iMessageID, void* pvData, std::int32_t cubData, std::int32_t* peChatEntryType) {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends017[66], steamIDFriend, iMessageID, pvData, cubData, peChatEntryType);
    }
    virtual std::uint64_t GetFollowerCount(CSteamID steamID) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamFriends017[67], steamID);
    }
    virtual std::uint64_t IsFollowing(CSteamID steamID) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamFriends017[68], steamID);
    }
    virtual std::uint64_t EnumerateFollowingList(std::uint32_t unStartIndex) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamFriends017[69], unStartIndex);
    }
    virtual bool IsClanPublic(CSteamID steamIDClan) {
        return steambridge::slot<bool>(kSlots_SteamFriends017[70], steamIDClan);
    }
    virtual bool IsClanOfficialGameGroup(CSteamID steamIDClan) {
        return steambridge::slot<bool>(kSlots_SteamFriends017[71], steamIDClan);
    }
    virtual std::int32_t GetNumChatsWithUnreadPriorityMessages() {
        return steambridge::slot<std::int32_t>(kSlots_SteamFriends017[72]);
    }
    virtual void ActivateGameOverlayRemotePlayTogetherInviteDialog(CSteamID steamIDLobby) {
        steambridge::slot<void>(kSlots_SteamFriends017[73], steamIDLobby);
    }
    virtual bool RegisterProtocolInOverlayBrowser(const char* pchProtocol) {
        return steambridge::slot<bool>(kSlots_SteamFriends017[74], pchProtocol);
    }
    virtual void ActivateGameOverlayInviteDialogConnectString(const char* pchConnectString) {
        steambridge::slot<void>(kSlots_SteamFriends017[75], pchConnectString);
    }
    virtual std::uint64_t RequestEquippedProfileItems(CSteamID steamID) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamFriends017[76], steamID);
    }
    virtual bool BHasEquippedProfileItem(CSteamID steamID, std::int32_t itemType) {
        return steambridge::slot<bool>(kSlots_SteamFriends017[77], steamID, itemType);
    }
    virtual const char* GetProfileItemPropertyString(CSteamID steamID, std::int32_t itemType, std::int32_t prop) {
        return steambridge::slot<const char*>(kSlots_SteamFriends017[78], steamID, itemType, prop);
    }
    virtual std::uint32_t GetProfileItemPropertyUint(CSteamID steamID, std::int32_t itemType, std::int32_t prop) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamFriends017[79], steamID, itemType, prop);
    }
};

Version_SteamFriends017 g_SteamFriends017;

const char* const kParams_SteamMatchGameSearch001_0[] = {
    "pchKeyToFind",
    "pchValuesToFind",
};
const char* const kParams_SteamMatchGameSearch001_1[] = {
    "steamIDLobby",
    "nPlayerMin",
    "nPlayerMax",
};
const char* const kParams_SteamMatchGameSearch001_2[] = {
    "nPlayerMin",
    "nPlayerMax",
};
const char* const kParams_SteamMatchGameSearch001_5[] = {
    "steamIDHost",
    "pchConnectionDetails",
    "cubConnectionDetails",
};
const char* const kParams_SteamMatchGameSearch001_7[] = {
    "pchKey",
    "pchValue",
};
const char* const kParams_SteamMatchGameSearch001_8[] = {
    "pchConnectionDetails",
    "cubConnectionDetails",
};
const char* const kParams_SteamMatchGameSearch001_9[] = {
    "nPlayerMin",
    "nPlayerMax",
    "nMaxTeamSize",
};
const char* const kParams_SteamMatchGameSearch001_10[] = {
    "ullUniqueGameID",
};
const char* const kParams_SteamMatchGameSearch001_12[] = {
    "ullUniqueGameID",
    "steamIDPlayer",
    "EPlayerResult",
};
const char* const kParams_SteamMatchGameSearch001_13[] = {
    "ullUniqueGameID",
};
// ISteamGameSearch SteamMatchGameSearch001
const steambridge::SlotInfo kSlots_SteamMatchGameSearch001[] = {
    {"SteamAPI_ISteamGameSearch_AddGameSearchParams", kParams_SteamMatchGameSearch001_0},
    {"SteamAPI_ISteamGameSearch_SearchForGameWithLobby", kParams_SteamMatchGameSearch001_1},
    {"SteamAPI_ISteamGameSearch_SearchForGameSolo", kParams_SteamMatchGameSearch001_2},
    {"SteamAPI_ISteamGameSearch_AcceptGame", nullptr},
    {"SteamAPI_ISteamGameSearch_DeclineGame", nullptr},
    {"SteamAPI_ISteamGameSearch_RetrieveConnectionDetails", kParams_SteamMatchGameSearch001_5},
    {"SteamAPI_ISteamGameSearch_EndGameSearch", nullptr},
    {"SteamAPI_ISteamGameSearch_SetGameHostParams", kParams_SteamMatchGameSearch001_7},
    {"SteamAPI_ISteamGameSearch_SetConnectionDetails", kParams_SteamMatchGameSearch001_8},
    {"SteamAPI_ISteamGameSearch_RequestPlayersForGame", kParams_SteamMatchGameSearch001_9},
    {"SteamAPI_ISteamGameSearch_HostConfirmGameStart", kParams_SteamMatchGameSearch001_10},
    {"SteamAPI_ISteamGameSearch_CancelRequestPlayersForGame", nullptr},
    {"SteamAPI_ISteamGameSearch_SubmitPlayerResult", kParams_SteamMatchGameSearch001_12},
    {"SteamAPI_ISteamGameSearch_EndGame", kParams_SteamMatchGameSearch001_13},
};

class Version_SteamMatchGameSearch001 {
public:
    virtual std::int32_t AddGameSearchParams(const char* pchKeyToFind, const char* pchValuesToFind) {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchGameSearch001[0], pchKeyToFind, pchValuesToFind);
    }
    virtual std::int32_t SearchForGameWithLobby(CSteamID steamIDLobby, std::int32_t nPlayerMin, std::int32_t nPlayerMax) {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchGameSearch001[1], steamIDLobby, nPlayerMin, nPlayerMax);
    }
    virtual std::int32_t SearchForGameSolo(std::int32_t nPlayerMin, std::int32_t nPlayerMax) {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchGameSearch001[2], nPlayerMin, nPlayerMax);
    }
    virtual std::int32_t AcceptGame() {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchGameSearch001[3]);
    }
    virtual std::int32_t DeclineGame() {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchGameSearch001[4]);
    }
    virtual std::int32_t RetrieveConnectionDetails(CSteamID steamIDHost, void* pchConnectionDetails, std::int32_t cubConnectionDetails) {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchGameSearch001[5], steamIDHost, pchConnectionDetails, cubConnectionDetails);
    }
    virtual std::int32_t EndGameSearch() {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchGameSearch001[6]);
    }
    virtual std::int32_t SetGameHostParams(const char* pchKey, const char* pchValue) {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchGameSearch001[7], pchKey, pchValue);
    }
    virtual std::int32_t SetConnectionDetails(const char* pchConnectionDetails, std::int32_t cubConnectionDetails) {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchGameSearch001[8], pchConnectionDetails, cubConnectionDetails);
    }
    virtual std::int32_t RequestPlayersForGame(std::int32_t nPlayerMin, std::int32_t nPlayerMax, std::int32_t nMaxTeamSize) {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchGameSearch001[9], nPlayerMin, nPlayerMax, nMaxTeamSize);
    }
    virtual std::int32_t HostConfirmGameStart(std::uint64_t ullUniqueGameID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchGameSearch001[10], ullUniqueGameID);
    }
    virtual std::int32_t CancelRequestPlayersForGame() {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchGameSearch001[11]);
    }
    virtual std::int32_t SubmitPlayerResult(std::uint64_t ullUniqueGameID, CSteamID steamIDPlayer, std::int32_t EPlayerResult) {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchGameSearch001[12], ullUniqueGameID, steamIDPlayer, EPlayerResult);
    }
    virtual std::int32_t EndGame(std::uint64_t ullUniqueGameID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchGameSearch001[13], ullUniqueGameID);
    }
};

Version_SteamMatchGameSearch001 g_SteamMatchGameSearch001;

const char* const kParams_SteamGameServer012_0[] = {
    "unIP",
    "usGamePort",
    "usQueryPort",
    "unFlags",
    "nGameAppId",
    "pchVersionString",
};
const char* const kParams_SteamGameServer012_1[] = {
    "pszProduct",
};
const char* const kParams_SteamGameServer012_2[] = {
    "pszGameDescription",
};
const char* const kParams_SteamGameServer012_3[] = {
    "pszModDir",
};
const char* const kParams_SteamGameServer012_4[] = {
    "bDedicated",
};
const char* const kParams_SteamGameServer012_5[] = {
    "pszToken",
};
const char* const kParams_SteamGameServer012_12[] = {
    "cPlayersMax",
};
const char* const kParams_SteamGameServer012_13[] = {
    "cBotplayers",
};
const char* const kParams_SteamGameServer012_14[] = {
    "pszServerName",
};
const char* const kParams_SteamGameServer012_15[] = {
    "pszMapName",
};
const char* const kParams_SteamGameServer012_16[] = {
    "bPasswordProtected",
};
const char* const kParams_SteamGameServer012_17[] = {
    "unSpectatorPort",
};
const char* const kParams_SteamGameServer012_18[] = {
    "pszSpectatorServerName",
};
const char* const kParams_SteamGameServer012_20[] = {
    "pKey",
    "pValue",
};
const char* const kParams_SteamGameServer012_21[] = {
    "pchGameTags",
};
const char* const kParams_SteamGameServer012_22[] = {
    "pchGameData",
};
const char* const kParams_SteamGameServer012_23[] = {
    "pszRegion",
};
const char* const kParams_SteamGameServer012_24[] = {
    "unIPClient",
    "pvAuthBlob",
    "cubAuthBlobSize",
    "pSteamIDUser",
};
const char* const kParams_SteamGameServer012_26[] = {
    "steamIDUser",
};
const char* const kParams_SteamGameServer012_27[] = {
    "steamIDUser",
    "pchPlayerName",
    "uScore",
};
const char* const kParams_SteamGameServer012_28[] = {
    "pTicket",
    "cbMaxTicket",
    "pcbTicket",
};
const char* const kParams_SteamGameServer012_29[] = {
    "pAuthTicket",
    "cbAuthTicket",
    "steamID",
};
const char* const kParams_SteamGameServer012_30[] = {
    "steamID",
};
const char* const kParams_SteamGameServer012_31[] = {
    "hAuthTicket",
};
const char* const kParams_SteamGameServer012_32[] = {
    "steamID",
    "appID",
};
const char* const kParams_SteamGameServer012_33[] = {
    "steamIDUser",
    "steamIDGroup",
};
const char* const kParams_SteamGameServer012_37[] = {
    "pData",
    "cbData",
    "srcIP",
    "srcPort",
};
const char* const kParams_SteamGameServer012_38[] = {
    "pOut",
    "cbMaxOut",
    "pNetAdr",
    "pPort",
};
const char* const kParams_SteamGameServer012_39[] = {
    "bActive",
};
const char* const kParams_SteamGameServer012_40[] = {
    "iHeartbeatInterval",
};
const char* const kParams_SteamGameServer012_42[] = {
    "steamIDClan",
};
const char* const kParams_SteamGameServer012_43[] = {
    "steamIDNewPlayer",
};
// ISteamGameServer SteamGameServer012
const steambridge::SlotInfo kSlots_SteamGameServer012[] = {
    {"SteamAPI_ISteamGameServer_InitGameServer", kParams_SteamGameServer012_0},
    {"SteamAPI_ISteamGameServer_SetProduct", kParams_SteamGameServer012_1},
    {"SteamAPI_ISteamGameServer_SetGameDescription", kParams_SteamGameServer012_2},
    {"SteamAPI_ISteamGameServer_SetModDir", kParams_SteamGameServer012_3},
    {"SteamAPI_ISteamGameServer_SetDedicatedServer", kParams_SteamGameServer012_4},
    {"SteamAPI_ISteamGameServer_LogOn", kParams_SteamGameServer012_5},
    {"SteamAPI_ISteamGameServer_LogOnAnonymous", nullptr},
    {"SteamAPI_ISteamGameServer_LogOff", nullptr},
    {"SteamAPI_ISteamGameServer_BLoggedOn", nullptr},
    {"SteamAPI_ISteamGameServer_BSecure", nullptr},
    {"SteamAPI_ISteamGameServer_GetSteamID", nullptr},
    {"SteamAPI_ISteamGameServer_WasRestartRequested", nullptr},
    {"SteamAPI_ISteamGameServer_SetMaxPlayerCount", kParams_SteamGameServer012_12},
    {"SteamAPI_ISteamGameServer_SetBotPlayerCount", kParams_SteamGameServer012_13},
    {"SteamAPI_ISteamGameServer_SetServerName", kParams_SteamGameServer012_14},
    {"SteamAPI_ISteamGameServer_SetMapName", kParams_SteamGameServer012_15},
    {"SteamAPI_ISteamGameServer_SetPasswordProtected", kParams_SteamGameServer012_16},
    {"SteamAPI_ISteamGameServer_SetSpectatorPort", kParams_SteamGameServer012_17},
    {"SteamAPI_ISteamGameServer_SetSpectatorServerName", kParams_SteamGameServer012_18},
    {"SteamAPI_ISteamGameServer_ClearAllKeyValues", nullptr},
    {"SteamAPI_ISteamGameServer_SetKeyValue", kParams_SteamGameServer012_20},
    {"SteamAPI_ISteamGameServer_SetGameTags", kParams_SteamGameServer012_21},
    {"SteamAPI_ISteamGameServer_SetGameData", kParams_SteamGameServer012_22},
    {"SteamAPI_ISteamGameServer_SetRegion", kParams_SteamGameServer012_23},
    {"SteamAPI_ISteamGameServer_SendUserConnectAndAuthenticate", kParams_SteamGameServer012_24},
    {"SteamAPI_ISteamGameServer_CreateUnauthenticatedUserConnection", nullptr},
    {"SteamAPI_ISteamGameServer_SendUserDisconnect", kParams_SteamGameServer012_26},
    {"SteamAPI_ISteamGameServer_BUpdateUserData", kParams_SteamGameServer012_27},
    {"SteamAPI_ISteamGameServer_GetAuthSessionTicket", kParams_SteamGameServer012_28},
    {"SteamAPI_ISteamGameServer_BeginAuthSession", kParams_SteamGameServer012_29},
    {"SteamAPI_ISteamGameServer_EndAuthSession", kParams_SteamGameServer012_30},
    {"SteamAPI_ISteamGameServer_CancelAuthTicket", kParams_SteamGameServer012_31},
    {"SteamAPI_ISteamGameServer_UserHasLicenseForApp", kParams_SteamGameServer012_32},
    {"SteamAPI_ISteamGameServer_RequestUserGroupStatus", kParams_SteamGameServer012_33},
    {"SteamAPI_ISteamGameServer_GetGameplayStats", nullptr},
    {"SteamAPI_ISteamGameServer_GetServerReputation", nullptr},
    {"SteamAPI_ISteamGameServer_GetPublicIP", nullptr},
    {"SteamAPI_ISteamGameServer_HandleIncomingPacket", kParams_SteamGameServer012_37},
    {"SteamAPI_ISteamGameServer_GetNextOutgoingPacket", kParams_SteamGameServer012_38},
    {"SteamAPI_ISteamGameServer_EnableHeartbeats", kParams_SteamGameServer012_39},
    {"SteamAPI_ISteamGameServer_SetHeartbeatInterval", kParams_SteamGameServer012_40},
    {"SteamAPI_ISteamGameServer_ForceHeartbeat", nullptr},
    {"SteamAPI_ISteamGameServer_AssociateWithClan", kParams_SteamGameServer012_42},
    {"SteamAPI_ISteamGameServer_ComputeNewPlayerCompatibility", kParams_SteamGameServer012_43},
};

class Version_SteamGameServer012 {
public:
    virtual bool InitGameServer(std::uint32_t unIP, std::uint16_t usGamePort, std::uint16_t usQueryPort, std::uint32_t unFlags, std::uint32_t nGameAppId, const char* pchVersionString) {
        return steambridge::slot<bool>(kSlots_SteamGameServer012[0], unIP, usGamePort, usQueryPort, unFlags, nGameAppId, pchVersionString);
    }
    virtual void SetProduct(const char* pszProduct) {
        steambridge::slot<void>(kSlots_SteamGameServer012[1], pszProduct);
    }
    virtual void SetGameDescription(const char* pszGameDescription) {
        steambridge::slot<void>(kSlots_SteamGameServer012[2], pszGameDescription);
    }
    virtual void SetModDir(const char* pszModDir) {
        steambridge::slot<void>(kSlots_SteamGameServer012[3], pszModDir);
    }
    virtual void SetDedicatedServer(bool bDedicated) {
        steambridge::slot<void>(kSlots_SteamGameServer012[4], bDedicated);
    }
    virtual void LogOn(const char* pszToken) {
        steambridge::slot<void>(kSlots_SteamGameServer012[5], pszToken);
    }
    virtual void LogOnAnonymous() {
        steambridge::slot<void>(kSlots_SteamGameServer012[6]);
    }
    virtual void LogOff() {
        steambridge::slot<void>(kSlots_SteamGameServer012[7]);
    }
    virtual bool BLoggedOn() {
        return steambridge::slot<bool>(kSlots_SteamGameServer012[8]);
    }
    virtual bool BSecure() {
        return steambridge::slot<bool>(kSlots_SteamGameServer012[9]);
    }
    virtual CSteamID GetSteamID() {
        return steambridge::slot<CSteamID>(kSlots_SteamGameServer012[10]);
    }
    virtual bool WasRestartRequested() {
        return steambridge::slot<bool>(kSlots_SteamGameServer012[11]);
    }
    virtual void SetMaxPlayerCount(std::int32_t cPlayersMax) {
        steambridge::slot<void>(kSlots_SteamGameServer012[12], cPlayersMax);
    }
    virtual void SetBotPlayerCount(std::int32_t cBotplayers) {
        steambridge::slot<void>(kSlots_SteamGameServer012[13], cBotplayers);
    }
    virtual void SetServerName(const char* pszServerName) {
        steambridge::slot<void>(kSlots_SteamGameServer012[14], pszServerName);
    }
    virtual void SetMapName(const char* pszMapName) {
        steambridge::slot<void>(kSlots_SteamGameServer012[15], pszMapName);
    }
    virtual void SetPasswordProtected(bool bPasswordProtected) {
        steambridge::slot<void>(kSlots_SteamGameServer012[16], bPasswordProtected);
    }
    virtual void SetSpectatorPort(std::uint16_t unSpectatorPort) {
        steambridge::slot<void>(kSlots_SteamGameServer012[17], unSpectatorPort);
    }
    virtual void SetSpectatorServerName(const char* pszSpectatorServerName) {
        steambridge::slot<void>(kSlots_SteamGameServer012[18], pszSpectatorServerName);
    }
    virtual void ClearAllKeyValues() {
        steambridge::slot<void>(kSlots_SteamGameServer012[19]);
    }
    virtual void SetKeyValue(const char* pKey, const char* pValue) {
        steambridge::slot<void>(kSlots_SteamGameServer012[20], pKey, pValue);
    }
    virtual void SetGameTags(const char* pchGameTags) {
        steambridge::slot<void>(kSlots_SteamGameServer012[21], pchGameTags);
    }
    virtual void SetGameData(const char* pchGameData) {
        steambridge::slot<void>(kSlots_SteamGameServer012[22], pchGameData);
    }
    virtual void SetRegion(const char* pszRegion) {
        steambridge::slot<void>(kSlots_SteamGameServer012[23], pszRegion);
    }
    virtual bool SendUserConnectAndAuthenticate(std::uint32_t unIPClient, void* pvAuthBlob, std::uint32_t cubAuthBlobSize, CSteamID* pSteamIDUser) {
        return steambridge::slot<bool>(kSlots_SteamGameServer012[24], unIPClient, pvAuthBlob, cubAuthBlobSize, pSteamIDUser);
    }
    virtual CSteamID CreateUnauthenticatedUserConnection() {
        return steambridge::slot<CSteamID>(kSlots_SteamGameServer012[25]);
    }
    virtual void SendUserDisconnect(CSteamID steamIDUser) {
        steambridge::slot<void>(kSlots_SteamGameServer012[26], steamIDUser);
    }
    virtual bool BUpdateUserData(CSteamID steamIDUser, const char* pchPlayerName, std::uint32_t uScore) {
        return steambridge::slot<bool>(kSlots_SteamGameServer012[27], steamIDUser, pchPlayerName, uScore);
    }
    virtual std::uint32_t GetAuthSessionTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamGameServer012[28], pTicket, cbMaxTicket, pcbTicket);
    }
    virtual std::int32_t BeginAuthSession(void* pAuthTicket, std::int32_t cbAuthTicket, CSteamID steamID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamGameServer012[29], pAuthTicket, cbAuthTicket, steamID);
    }
    virtual void EndAuthSession(CSteamID steamID) {
        steambridge::slot<void>(kSlots_SteamGameServer012[30], steamID);
    }
    virtual void CancelAuthTicket(std::uint32_t hAuthTicket) {
        steambridge::slot<void>(kSlots_SteamGameServer012[31], hAuthTicket);
    }
    virtual std::int32_t UserHasLicenseForApp(CSteamID steamID, std::uint32_t appID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamGameServer012[32], steamID, appID);
    }
    virtual bool RequestUserGroupStatus(CSteamID steamIDUser, CSteamID steamIDGroup) {
        return steambridge::slot<bool>(kSlots_SteamGameServer012[33], steamIDUser, steamIDGroup);
    }
    virtual void GetGameplayStats() {
        steambridge::slot<void>(kSlots_SteamGameServer012[34]);
    }
    virtual std::uint64_t GetServerReputation() {
        return steambridge::slot<std::uint64_t>(kSlots_SteamGameServer012[35]);
    }
    virtual std::uint32_t GetPublicIP() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamGameServer012[36]);
    }
    virtual bool HandleIncomingPacket(void* pData, std::int32_t cbData, std::uint32_t srcIP, std::uint16_t srcPort) {
        return steambridge::slot<bool>(kSlots_SteamGameServer012[37], pData, cbData, srcIP, srcPort);
    }
    virtual std::int32_t GetNextOutgoingPacket(void* pOut, std::int32_t cbMaxOut, std::uint32_t* pNetAdr, std::uint16_t* pPort) {
        return steambridge::slot<std::int32_t>(kSlots_SteamGameServer012[38], pOut, cbMaxOut, pNetAdr, pPort);
    }
    virtual void EnableHeartbeats(bool bActive) {
        steambridge::slot<void>(kSlots_SteamGameServer012[39], bActive);
    }
    virtual void SetHeartbeatInterval(std::int32_t iHeartbeatInterval) {
        steambridge::slot<void>(kSlots_SteamGameServer012[40], iHeartbeatInterval);
    }
    virtual void ForceHeartbeat() {
        steambridge::slot<void>(kSlots_SteamGameServer012[41]);
    }
    virtual std::uint64_t AssociateWithClan(CSteamID steamIDClan) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamGameServer012[42], steamIDClan);
    }
    virtual std::uint64_t ComputeNewPlayerCompatibility(CSteamID steamIDNewPlayer) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamGameServer012[43], steamIDNewPlayer);
    }
};

Version_SteamGameServer012 g_SteamGameServer012;

const char* const kParams_SteamGameServer013_0[] = {
    "unIP",
    "usGamePort",
    "usQueryPort",
    "unFlags",
    "nGameAppId",
    "pchVersionString",
};
const char* const kParams_SteamGameServer013_1[] = {
    "pszProduct",
};
const char* const kParams_SteamGameServer013_2[] = {
    "pszGameDescription",
};
const char* const kParams_SteamGameServer013_3[] = {
    "pszModDir",
};
const char* const kParams_SteamGameServer013_4[] = {
    "bDedicated",
};
const char* const kParams_SteamGameServer013_5[] = {
    "pszToken",
};
const char* const kParams_SteamGameServer013_12[] = {
    "cPlayersMax",
};
const char* const kParams_SteamGameServer013_13[] = {
    "cBotplayers",
};
const char* const kParams_SteamGameServer013_14[] = {
    "pszServerName",
};
const char* const kParams_SteamGameServer013_15[] = {
    "pszMapName",
};
const char* const kParams_SteamGameServer013_16[] = {
    "bPasswordProtected",
};
const char* const kParams_SteamGameServer013_17[] = {
    "unSpectatorPort",
};
const char* const kParams_SteamGameServer013_18[] = {
    "pszSpectatorServerName",
};
const char* const kParams_SteamGameServer013_20[] = {
    "pKey",
    "pValue",
};
const char* const kParams_SteamGameServer013_21[] = {
    "pchGameTags",
};
const char* const kParams_SteamGameServer013_22[] = {
    "pchGameData",
};
const char* const kParams_SteamGameServer013_23[] = {
    "pszRegion",
};
const char* const kParams_SteamGameServer013_24[] = {
    "unIPClient",
    "pvAuthBlob",
    "cubAuthBlobSize",
    "pSteamIDUser",
};
const char* const kParams_SteamGameServer013_26[] = {
    "steamIDUser",
};
const char* const kParams_SteamGameServer013_27[] = {
    "steamIDUser",
    "pchPlayerName",
    "uScore",
};
const char* const kParams_SteamGameServer013_28[] = {
    "pTicket",
    "cbMaxTicket",
    "pcbTicket",
};
const char* const kParams_SteamGameServer013_29[] = {
    "pAuthTicket",
    "cbAuthTicket",
    "steamID",
};
const char* const kParams_SteamGameServer013_30[] = {
    "steamID",
};
const char* const kParams_SteamGameServer013_31[] = {
    "hAuthTicket",
};
const char* const kParams_SteamGameServer013_32[] = {
    "steamID",
    "appID",
};
const char* const kParams_SteamGameServer013_33[] = {
    "steamIDUser",
    "steamIDGroup",
};
const char* const kParams_SteamGameServer013_37[] = {
    "pData",
    "cbData",
    "srcIP",
    "srcPort",
};
const char* const kParams_SteamGameServer013_38[] = {
    "pOut",
    "cbMaxOut",
    "pNetAdr",
    "pPort",
};
const char* const kParams_SteamGameServer013_39[] = {
    "bActive",
};
const char* const kParams_SteamGameServer013_40[] = {
    "iHeartbeatInterval",
};
const char* const kParams_SteamGameServer013_42[] = {
    "steamIDClan",
};
const char* const kParams_SteamGameServer013_43[] = {
    "steamIDNewPlayer",
};
// ISteamGameServer SteamGameServer013
const steambridge::SlotInfo kSlots_SteamGameServer013[] = {
    {"ISteamGameServer::InitGameServer", kParams_SteamGameServer013_0},
    {"SteamAPI_ISteamGameServer_SetProduct", kParams_SteamGameServer013_1},
    {"SteamAPI_ISteamGameServer_SetGameDescription", kParams_SteamGameServer013_2},
    {"SteamAPI_ISteamGameServer_SetModDir", kParams_SteamGameServer013_3},
    {"SteamAPI_ISteamGameServer_SetDedicatedServer", kParams_SteamGameServer013_4},
    {"SteamAPI_ISteamGameServer_LogOn", kParams_SteamGameServer013_5},
    {"SteamAPI_ISteamGameServer_LogOnAnonymous", nullptr},
    {"SteamAPI_ISteamGameServer_LogOff", nullptr},
    {"SteamAPI_ISteamGameServer_BLoggedOn", nullptr},
    {"SteamAPI_ISteamGameServer_BSecure", nullptr},
    {"SteamAPI_ISteamGameServer_GetSteamID", nullptr},
    {"SteamAPI_ISteamGameServer_WasRestartRequested", nullptr},
    {"SteamAPI_ISteamGameServer_SetMaxPlayerCount", kParams_SteamGameServer013_12},
    {"SteamAPI_ISteamGameServer_SetBotPlayerCount", kParams_SteamGameServer013_13},
    {"SteamAPI_ISteamGameServer_SetServerName", kParams_SteamGameServer013_14},
    {"SteamAPI_ISteamGameServer_SetMapName", kParams_SteamGameServer013_15},
    {"SteamAPI_ISteamGameServer_SetPasswordProtected", kParams_SteamGameServer013_16},
    {"SteamAPI_ISteamGameServer_SetSpectatorPort", kParams_SteamGameServer013_17},
    {"SteamAPI_ISteamGameServer_SetSpectatorServerName", kParams_SteamGameServer013_18},
    {"SteamAPI_ISteamGameServer_ClearAllKeyValues", nullptr},
    {"SteamAPI_ISteamGameServer_SetKeyValue", kParams_SteamGameServer013_20},
    {"SteamAPI_ISteamGameServer_SetGameTags", kParams_SteamGameServer013_21},
    {"SteamAPI_ISteamGameServer_SetGameData", kParams_SteamGameServer013_22},
    {"SteamAPI_ISteamGameServer_SetRegion", kParams_SteamGameServer013_23},
    {"SteamAPI_ISteamGameServer_SendUserConnectAndAuthenticate", kParams_SteamGameServer013_24},
    {"SteamAPI_ISteamGameServer_CreateUnauthenticatedUserConnection", nullptr},
    {"SteamAPI_ISteamGameServer_SendUserDisconnect", kParams_SteamGameServer013_26},
    {"SteamAPI_ISteamGameServer_BUpdateUserData", kParams_SteamGameServer013_27},
    {"SteamAPI_ISteamGameServer_GetAuthSessionTicket", kParams_SteamGameServer013_28},
    {"SteamAPI_ISteamGameServer_BeginAuthSession", kParams_SteamGameServer013_29},
    {"SteamAPI_ISteamGameServer_EndAuthSession", kParams_SteamGameServer013_30},
    {"SteamAPI_ISteamGameServer_CancelAuthTicket", kParams_SteamGameServer013_31},
    {"SteamAPI_ISteamGameServer_UserHasLicenseForApp", kParams_SteamGameServer013_32},
    {"SteamAPI_ISteamGameServer_RequestUserGroupStatus", kParams_SteamGameServer013_33},
    {"SteamAPI_ISteamGameServer_GetGameplayStats", nullptr},
    {"SteamAPI_ISteamGameServer_GetServerReputation", nullptr},
    {"SteamAPI_ISteamGameServer_GetPublicIP", nullptr},
    {"SteamAPI_ISteamGameServer_HandleIncomingPacket", kParams_SteamGameServer013_37},
    {"SteamAPI_ISteamGameServer_GetNextOutgoingPacket", kParams_SteamGameServer013_38},
    {"SteamAPI_ISteamGameServer_EnableHeartbeats", kParams_SteamGameServer013_39},
    {"SteamAPI_ISteamGameServer_SetHeartbeatInterval", kParams_SteamGameServer013_40},
    {"SteamAPI_ISteamGameServer_ForceHeartbeat", nullptr},
    {"SteamAPI_ISteamGameServer_AssociateWithClan", kParams_SteamGameServer013_42},
    {"SteamAPI_ISteamGameServer_ComputeNewPlayerCompatibility", kParams_SteamGameServer013_43},
};

class Version_SteamGameServer013 {
public:
    virtual bool InitGameServer(std::uint32_t unIP, std::uint16_t usGamePort, std::uint16_t usQueryPort, std::uint32_t unFlags, std::uint32_t nGameAppId, const char* pchVersionString) {
        return steambridge::slot<bool>(kSlots_SteamGameServer013[0], unIP, usGamePort, usQueryPort, unFlags, nGameAppId, pchVersionString);
    }
    virtual void SetProduct(const char* pszProduct) {
        steambridge::slot<void>(kSlots_SteamGameServer013[1], pszProduct);
    }
    virtual void SetGameDescription(const char* pszGameDescription) {
        steambridge::slot<void>(kSlots_SteamGameServer013[2], pszGameDescription);
    }
    virtual void SetModDir(const char* pszModDir) {
        steambridge::slot<void>(kSlots_SteamGameServer013[3], pszModDir);
    }
    virtual void SetDedicatedServer(bool bDedicated) {
        steambridge::slot<void>(kSlots_SteamGameServer013[4], bDedicated);
    }
    virtual void LogOn(const char* pszToken) {
        steambridge::slot<void>(kSlots_SteamGameServer013[5], pszToken);
    }
    virtual void LogOnAnonymous() {
        steambridge::slot<void>(kSlots_SteamGameServer013[6]);
    }
    virtual void LogOff() {
        steambridge::slot<void>(kSlots_SteamGameServer013[7]);
    }
    virtual bool BLoggedOn() {
        return steambridge::slot<bool>(kSlots_SteamGameServer013[8]);
    }
    virtual bool BSecure() {
        return steambridge::slot<bool>(kSlots_SteamGameServer013[9]);
    }
    virtual CSteamID GetSteamID() {
        return steambridge::slot<CSteamID>(kSlots_SteamGameServer013[10]);
    }
    virtual bool WasRestartRequested() {
        return steambridge::slot<bool>(kSlots_SteamGameServer013[11]);
    }
    virtual void SetMaxPlayerCount(std::int32_t cPlayersMax) {
        steambridge::slot<void>(kSlots_SteamGameServer013[12], cPlayersMax);
    }
    virtual void SetBotPlayerCount(std::int32_t cBotplayers) {
        steambridge::slot<void>(kSlots_SteamGameServer013[13], cBotplayers);
    }
    virtual void SetServerName(const char* pszServerName) {
        steambridge::slot<void>(kSlots_SteamGameServer013[14], pszServerName);
    }
    virtual void SetMapName(const char* pszMapName) {
        steambridge::slot<void>(kSlots_SteamGameServer013[15], pszMapName);
    }
    virtual void SetPasswordProtected(bool bPasswordProtected) {
        steambridge::slot<void>(kSlots_SteamGameServer013[16], bPasswordProtected);
    }
    virtual void SetSpectatorPort(std::uint16_t unSpectatorPort) {
        steambridge::slot<void>(kSlots_SteamGameServer013[17], unSpectatorPort);
    }
    virtual void SetSpectatorServerName(const char* pszSpectatorServerName) {
        steambridge::slot<void>(kSlots_SteamGameServer013[18], pszSpectatorServerName);
    }
    virtual void ClearAllKeyValues() {
        steambridge::slot<void>(kSlots_SteamGameServer013[19]);
    }
    virtual void SetKeyValue(const char* pKey, const char* pValue) {
        steambridge::slot<void>(kSlots_SteamGameServer013[20], pKey, pValue);
    }
    virtual void SetGameTags(const char* pchGameTags) {
        steambridge::slot<void>(kSlots_SteamGameServer013[21], pchGameTags);
    }
    virtual void SetGameData(const char* pchGameData) {
        steambridge::slot<void>(kSlots_SteamGameServer013[22], pchGameData);
    }
    virtual void SetRegion(const char* pszRegion) {
        steambridge::slot<void>(kSlots_SteamGameServer013[23], pszRegion);
    }
    virtual bool SendUserConnectAndAuthenticate(std::uint32_t unIPClient, void* pvAuthBlob, std::uint32_t cubAuthBlobSize, CSteamID* pSteamIDUser) {
        return steambridge::slot<bool>(kSlots_SteamGameServer013[24], unIPClient, pvAuthBlob, cubAuthBlobSize, pSteamIDUser);
    }
    virtual CSteamID CreateUnauthenticatedUserConnection() {
        return steambridge::slot<CSteamID>(kSlots_SteamGameServer013[25]);
    }
    virtual void SendUserDisconnect(CSteamID steamIDUser) {
        steambridge::slot<void>(kSlots_SteamGameServer013[26], steamIDUser);
    }
    virtual bool BUpdateUserData(CSteamID steamIDUser, const char* pchPlayerName, std::uint32_t uScore) {
        return steambridge::slot<bool>(kSlots_SteamGameServer013[27], steamIDUser, pchPlayerName, uScore);
    }
    virtual std::uint32_t GetAuthSessionTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamGameServer013[28], pTicket, cbMaxTicket, pcbTicket);
    }
    virtual std::int32_t BeginAuthSession(void* pAuthTicket, std::int32_t cbAuthTicket, CSteamID steamID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamGameServer013[29], pAuthTicket, cbAuthTicket, steamID);
    }
    virtual void EndAuthSession(CSteamID steamID) {
        steambridge::slot<void>(kSlots_SteamGameServer013[30], steamID);
    }
    virtual void CancelAuthTicket(std::uint32_t hAuthTicket) {
        steambridge::slot<void>(kSlots_SteamGameServer013[31], hAuthTicket);
    }
    virtual std::int32_t UserHasLicenseForApp(CSteamID steamID, std::uint32_t appID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamGameServer013[32], steamID, appID);
    }
    virtual bool RequestUserGroupStatus(CSteamID steamIDUser, CSteamID steamIDGroup) {
        return steambridge::slot<bool>(kSlots_SteamGameServer013[33], steamIDUser, steamIDGroup);
    }
    virtual void GetGameplayStats() {
        steambridge::slot<void>(kSlots_SteamGameServer013[34]);
    }
    virtual std::uint64_t GetServerReputation() {
        return steambridge::slot<std::uint64_t>(kSlots_SteamGameServer013[35]);
    }
    virtual SteamIPAddress_t GetPublicIP() {
        return steambridge::slot<SteamIPAddress_t>(kSlots_SteamGameServer013[36]);
    }
    virtual bool HandleIncomingPacket(void* pData, std::int32_t cbData, std::uint32_t srcIP, std::uint16_t srcPort) {
        return steambridge::slot<bool>(kSlots_SteamGameServer013[37], pData, cbData, srcIP, srcPort);
    }
    virtual std::int32_t GetNextOutgoingPacket(void* pOut, std::int32_t cbMaxOut, std::uint32_t* pNetAdr, std::uint16_t* pPort) {
        return steambridge::slot<std::int32_t>(kSlots_SteamGameServer013[38], pOut, cbMaxOut, pNetAdr, pPort);
    }
    virtual void EnableHeartbeats(bool bActive) {
        steambridge::slot<void>(kSlots_SteamGameServer013[39], bActive);
    }
    virtual void SetHeartbeatInterval(std::int32_t iHeartbeatInterval) {
        steambridge::slot<void>(kSlots_SteamGameServer013[40], iHeartbeatInterval);
    }
    virtual void ForceHeartbeat() {
        steambridge::slot<void>(kSlots_SteamGameServer013[41]);
    }
    virtual std::uint64_t AssociateWithClan(CSteamID steamIDClan) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamGameServer013[42], steamIDClan);
    }
    virtual std::uint64_t ComputeNewPlayerCompatibility(CSteamID steamIDNewPlayer) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamGameServer013[43], steamIDNewPlayer);
    }
};

Version_SteamGameServer013 g_SteamGameServer013;

const char* const kParams_SteamGameServer014_0[] = {
    "unIP",
    "usGamePort",
    "usQueryPort",
    "unFlags",
    "nGameAppId",
    "pchVersionString",
};
const char* const kParams_SteamGameServer014_1[] = {
    "pszProduct",
};
const char* const kParams_SteamGameServer014_2[] = {
    "pszGameDescription",
};
const char* const kParams_SteamGameServer014_3[] = {
    "pszModDir",
};
const char* const kParams_SteamGameServer014_4[] = {
    "bDedicated",
};
const char* const kParams_SteamGameServer014_5[] = {
    "pszToken",
};
const char* const kParams_SteamGameServer014_12[] = {
    "cPlayersMax",
};
const char* const kParams_SteamGameServer014_13[] = {
    "cBotplayers",
};
const char* const kParams_SteamGameServer014_14[] = {
    "pszServerName",
};
const char* const kParams_SteamGameServer014_15[] = {
    "pszMapName",
};
const char* const kParams_SteamGameServer014_16[] = {
    "bPasswordProtected",
};
const char* const kParams_SteamGameServer014_17[] = {
    "unSpectatorPort",
};
const char* const kParams_SteamGameServer014_18[] = {
    "pszSpectatorServerName",
};
const char* const kParams_SteamGameServer014_20[] = {
    "pKey",
    "pValue",
};
const char* const kParams_SteamGameServer014_21[] = {
    "pchGameTags",
};
const char* const kParams_SteamGameServer014_22[] = {
    "pchGameData",
};
const char* const kParams_SteamGameServer014_23[] = {
    "pszRegion",
};
const char* const kParams_SteamGameServer014_24[] = {
    "bActive",
};
const char* const kParams_SteamGameServer014_25[] = {
    "pTicket",
    "cbMaxTicket",
    "pcbTicket",
};
const char* const kParams_SteamGameServer014_26[] = {
    "pAuthTicket",
    "cbAuthTicket",
    "steamID",
};
const char* const kParams_SteamGameServer014_27[] = {
    "steamID",
};
const char* const kParams_SteamGameServer014_28[] = {
    "hAuthTicket",
};
const char* const kParams_SteamGameServer014_29[] = {
    "steamID",
    "appID",
};
const char* const kParams_SteamGameServer014_30[] = {
    "steamIDUser",
    "steamIDGroup",
};
const char* const kParams_SteamGameServer014_34[] = {
    "pData",
    "cbData",
    "srcIP",
    "srcPort",
};
const char* const kParams_SteamGameServer014_35[] = {
    "pOut",
    "cbMaxOut",
    "pNetAdr",
    "pPort",
};
const char* const kParams_SteamGameServer014_36[] = {
    "steamIDClan",
};
const char* const kParams_SteamGameServer014_37[] = {
    "steamIDNewPlayer",
};
const char* const kParams_SteamGameServer014_38[] = {
    "unIPClient",
    "pvAuthBlob",
    "cubAuthBlobSize",
    "pSteamIDUser",
};
const char* const kParams_SteamGameServer014_40[] = {
    "steamIDUser",
};
const char* const kParams_SteamGameServer014_41[] = {
    "steamIDUser",
    "pchPlayerName",
    "uScore",
};
const char* const kParams_SteamGameServer014_42[] = {
    "iHeartbeatInterval",
};
// ISteamGameServer SteamGameServer014
const steambridge::SlotInfo kSlots_SteamGameServer014[] = {
    {"ISteamGameServer::InitGameServer", kParams_SteamGameServer014_0},
    {"SteamAPI_ISteamGameServer_SetProduct", kParams_SteamGameServer014_1},
    {"SteamAPI_ISteamGameServer_SetGameDescription", kParams_SteamGameServer014_2},
    {"SteamAPI_ISteamGameServer_SetModDir", kParams_SteamGameServer014_3},
    {"SteamAPI_ISteamGameServer_SetDedicatedServer", kParams_SteamGameServer014_4},
    {"SteamAPI_ISteamGameServer_LogOn", kParams_SteamGameServer014_5},
    {"SteamAPI_ISteamGameServer_LogOnAnonymous", nullptr},
    {"SteamAPI_ISteamGameServer_LogOff", nullptr},
    {"SteamAPI_ISteamGameServer_BLoggedOn", nullptr},
    {"SteamAPI_ISteamGameServer_BSecure", nullptr},
    {"SteamAPI_ISteamGameServer_GetSteamID", nullptr},
    {"SteamAPI_ISteamGameServer_WasRestartRequested", nullptr},
    {"SteamAPI_ISteamGameServer_SetMaxPlayerCount", kParams_SteamGameServer014_12},
    {"SteamAPI_ISteamGameServer_SetBotPlayerCount", kParams_SteamGameServer014_13},
    {"SteamAPI_ISteamGameServer_SetServerName", kParams_SteamGameServer014_14},
    {"SteamAPI_ISteamGameServer_SetMapName", kParams_SteamGameServer014_15},
    {"SteamAPI_ISteamGameServer_SetPasswordProtected", kParams_SteamGameServer014_16},
    {"SteamAPI_ISteamGameServer_SetSpectatorPort", kParams_SteamGameServer014_17},
    {"SteamAPI_ISteamGameServer_SetSpectatorServerName", kParams_SteamGameServer014_18},
    {"SteamAPI_ISteamGameServer_ClearAllKeyValues", nullptr},
    {"SteamAPI_ISteamGameServer_SetKeyValue", kParams_SteamGameServer014_20},
    {"SteamAPI_ISteamGameServer_SetGameTags", kParams_SteamGameServer014_21},
    {"SteamAPI_ISteamGameServer_SetGameData", kParams_SteamGameServer014_22},
    {"SteamAPI_ISteamGameServer_SetRegion", kParams_SteamGameServer014_23},
    {"SteamAPI_ISteamGameServer_SetAdvertiseServerActive", kParams_SteamGameServer014_24},
    {"SteamAPI_ISteamGameServer_GetAuthSessionTicket", kParams_SteamGameServer014_25},
    {"SteamAPI_ISteamGameServer_BeginAuthSession", kParams_SteamGameServer014_26},
    {"SteamAPI_ISteamGameServer_EndAuthSession", kParams_SteamGameServer014_27},
    {"SteamAPI_ISteamGameServer_CancelAuthTicket", kParams_SteamGameServer014_28},
    {"SteamAPI_ISteamGameServer_UserHasLicenseForApp", kParams_SteamGameServer014_29},
    {"SteamAPI_ISteamGameServer_RequestUserGroupStatus", kParams_SteamGameServer014_30},
    {"SteamAPI_ISteamGameServer_GetGameplayStats", nullptr},
    {"SteamAPI_ISteamGameServer_GetServerReputation", nullptr},
    {"SteamAPI_ISteamGameServer_GetPublicIP", nullptr},
    {"SteamAPI_ISteamGameServer_HandleIncomingPacket", kParams_SteamGameServer014_34},
    {"SteamAPI_ISteamGameServer_GetNextOutgoingPacket", kParams_SteamGameServer014_35},
    {"SteamAPI_ISteamGameServer_AssociateWithClan", kParams_SteamGameServer014_36},
    {"SteamAPI_ISteamGameServer_ComputeNewPlayerCompatibility", kParams_SteamGameServer014_37},
    {"SteamAPI_ISteamGameServer_SendUserConnectAndAuthenticate_DEPRECATED", kParams_SteamGameServer014_38},
    {"SteamAPI_ISteamGameServer_CreateUnauthenticatedUserConnection", nullptr},
    {"SteamAPI_ISteamGameServer_SendUserDisconnect_DEPRECATED", kParams_SteamGameServer014_40},
    {"SteamAPI_ISteamGameServer_BUpdateUserData", kParams_SteamGameServer014_41},
    {"ISteamGameServer::SetMasterServerHeartbeatInterval_DEPRECATED", kParams_SteamGameServer014_42},
    {"ISteamGameServer::ForceMasterServerHeartbeat_DEPRECATED", nullptr},
};

class Version_SteamGameServer014 {
public:
    virtual bool InitGameServer(std::uint32_t unIP, std::uint16_t usGamePort, std::uint16_t usQueryPort, std::uint32_t unFlags, std::uint32_t nGameAppId, const char* pchVersionString) {
        return steambridge::slot<bool>(kSlots_SteamGameServer014[0], unIP, usGamePort, usQueryPort, unFlags, nGameAppId, pchVersionString);
    }
    virtual void SetProduct(const char* pszProduct) {
        steambridge::slot<void>(kSlots_SteamGameServer014[1], pszProduct);
    }
    virtual void SetGameDescription(const char* pszGameDescription) {
        steambridge::slot<void>(kSlots_SteamGameServer014[2], pszGameDescription);
    }
    virtual void SetModDir(const char* pszModDir) {
        steambridge::slot<void>(kSlots_SteamGameServer014[3], pszModDir);
    }
    virtual void SetDedicatedServer(bool bDedicated) {
        steambridge::slot<void>(kSlots_SteamGameServer014[4], bDedicated);
    }
    virtual void LogOn(const char* pszToken) {
        steambridge::slot<void>(kSlots_SteamGameServer014[5], pszToken);
    }
    virtual void LogOnAnonymous() {
        steambridge::slot<void>(kSlots_SteamGameServer014[6]);
    }
    virtual void LogOff() {
        steambridge::slot<void>(kSlots_SteamGameServer014[7]);
    }
    virtual bool BLoggedOn() {
        return steambridge::slot<bool>(kSlots_SteamGameServer014[8]);
    }
    virtual bool BSecure() {
        return steambridge::slot<bool>(kSlots_SteamGameServer014[9]);
    }
    virtual CSteamID GetSteamID() {
        return steambridge::slot<CSteamID>(kSlots_SteamGameServer014[10]);
    }
    virtual bool WasRestartRequested() {
        return steambridge::slot<bool>(kSlots_SteamGameServer014[11]);
    }
    virtual void SetMaxPlayerCount(std::int32_t cPlayersMax) {
        steambridge::slot<void>(kSlots_SteamGameServer014[12], cPlayersMax);
    }
    virtual void SetBotPlayerCount(std::int32_t cBotplayers) {
        steambridge::slot<void>(kSlots_SteamGameServer014[13], cBotplayers);
    }
    virtual void SetServerName(const char* pszServerName) {
        steambridge::slot<void>(kSlots_SteamGameServer014[14], pszServerName);
    }
    virtual void SetMapName(const char* pszMapName) {
        steambridge::slot<void>(kSlots_SteamGameServer014[15], pszMapName);
    }
    virtual void SetPasswordProtected(bool bPasswordProtected) {
        steambridge::slot<void>(kSlots_SteamGameServer014[16], bPasswordProtected);
    }
    virtual void SetSpectatorPort(std::uint16_t unSpectatorPort) {
        steambridge::slot<void>(kSlots_SteamGameServer014[17], unSpectatorPort);
    }
    virtual void SetSpectatorServerName(const char* pszSpectatorServerName) {
        steambridge::slot<void>(kSlots_SteamGameServer014[18], pszSpectatorServerName);
    }
    virtual void ClearAllKeyValues() {
        steambridge::slot<void>(kSlots_SteamGameServer014[19]);
    }
    virtual void SetKeyValue(const char* pKey, const char* pValue) {
        steambridge::slot<void>(kSlots_SteamGameServer014[20], pKey, pValue);
    }
    virtual void SetGameTags(const char* pchGameTags) {
        steambridge::slot<void>(kSlots_SteamGameServer014[21], pchGameTags);
    }
    virtual void SetGameData(const char* pchGameData) {
        steambridge::slot<void>(kSlots_SteamGameServer014[22], pchGameData);
    }
    virtual void SetRegion(const char* pszRegion) {
        steambridge::slot<void>(kSlots_SteamGameServer014[23], pszRegion);
    }
    virtual void SetAdvertiseServerActive(bool bActive) {
        steambridge::slot<void>(kSlots_SteamGameServer014[24], bActive);
    }
    virtual std::uint32_t GetAuthSessionTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamGameServer014[25], pTicket, cbMaxTicket, pcbTicket);
    }
    virtual std::int32_t BeginAuthSession(void* pAuthTicket, std::int32_t cbAuthTicket, CSteamID steamID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamGameServer014[26], pAuthTicket, cbAuthTicket, steamID);
    }
    virtual void EndAuthSession(CSteamID steamID) {
        steambridge::slot<void>(kSlots_SteamGameServer014[27], steamID);
    }
    virtual void CancelAuthTicket(std::uint32_t hAuthTicket) {
        steambridge::slot<void>(kSlots_SteamGameServer014[28], hAuthTicket);
    }
    virtual std::int32_t UserHasLicenseForApp(CSteamID steamID, std::uint32_t appID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamGameServer014[29], steamID, appID);
    }
    virtual bool RequestUserGroupStatus(CSteamID steamIDUser, CSteamID steamIDGroup) {
        return steambridge::slot<bool>(kSlots_SteamGameServer014[30], steamIDUser, steamIDGroup);
    }
    virtual void GetGameplayStats() {
        steambridge::slot<void>(kSlots_SteamGameServer014[31]);
    }
    virtual std::uint64_t GetServerReputation() {
        return steambridge::slot<std::uint64_t>(kSlots_SteamGameServer014[32]);
    }
    virtual SteamIPAddress_t GetPublicIP() {
        return steambridge::slot<SteamIPAddress_t>(kSlots_SteamGameServer014[33]);
    }
    virtual bool HandleIncomingPacket(void* pData, std::int32_t cbData, std::uint32_t srcIP, std::uint16_t srcPort) {
        return steambridge::slot<bool>(kSlots_SteamGameServer014[34], pData, cbData, srcIP, srcPort);
    }
    virtual std::int32_t GetNextOutgoingPacket(void* pOut, std::int32_t cbMaxOut, std::uint32_t* pNetAdr, std::uint16_t* pPort) {
        return steambridge::slot<std::int32_t>(kSlots_SteamGameServer014[35], pOut, cbMaxOut, pNetAdr, pPort);
    }
    virtual std::uint64_t AssociateWithClan(CSteamID steamIDClan) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamGameServer014[36], steamIDClan);
    }
    virtual std::uint64_t ComputeNewPlayerCompatibility(CSteamID steamIDNewPlayer) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamGameServer014[37], steamIDNewPlayer);
    }
    virtual bool SendUserConnectAndAuthenticate_DEPRECATED(std::uint32_t unIPClient, void* pvAuthBlob, std::uint32_t cubAuthBlobSize, CSteamID* pSteamIDUser) {
        return steambridge::slot<bool>(kSlots_SteamGameServer014[38], unIPClient, pvAuthBlob, cubAuthBlobSize, pSteamIDUser);
    }
    virtual CSteamID CreateUnauthenticatedUserConnection() {
        return steambridge::slot<CSteamID>(kSlots_SteamGameServer014[39]);
    }
    virtual void SendUserDisconnect_DEPRECATED(CSteamID steamIDUser) {
        steambridge::slot<void>(kSlots_SteamGameServer014[40], steamIDUser);
    }
    virtual bool BUpdateUserData(CSteamID steamIDUser, const char* pchPlayerName, std::uint32_t uScore) {
        return steambridge::slot<bool>(kSlots_SteamGameServer014[41], steamIDUser, pchPlayerName, uScore);
    }
    virtual void SetMasterServerHeartbeatInterval_DEPRECATED(std::int32_t iHeartbeatInterval) {
        steambridge::slot<void>(kSlots_SteamGameServer014[42], iHeartbeatInterval);
    }
    virtual void ForceMasterServerHeartbeat_DEPRECATED() {
        steambridge::slot<void>(kSlots_SteamGameServer014[43]);
    }
};

Version_SteamGameServer014 g_SteamGameServer014;

const char* const kParams_SteamGameServer015_0[] = {
    "unIP",
    "usGamePort",
    "usQueryPort",
    "unFlags",
    "nGameAppId",
    "pchVersionString",
};
const char* const kParams_SteamGameServer015_1[] = {
    "pszProduct",
};
const char* const kParams_SteamGameServer015_2[] = {
    "pszGameDescription",
};
const char* const kParams_SteamGameServer015_3[] = {
    "pszModDir",
};
const char* const kParams_SteamGameServer015_4[] = {
    "bDedicated",
};
const char* const kParams_SteamGameServer015_5[] = {
    "pszToken",
};
const char* const kParams_SteamGameServer015_12[] = {
    "cPlayersMax",
};
const char* const kParams_SteamGameServer015_13[] = {
    "cBotplayers",
};
const char* const kParams_SteamGameServer015_14[] = {
    "pszServerName",
};
const char* const kParams_SteamGameServer015_15[] = {
    "pszMapName",
};
const char* const kParams_SteamGameServer015_16[] = {
    "bPasswordProtected",
};
const char* const kParams_SteamGameServer015_17[] = {
    "unSpectatorPort",
};
const char* const kParams_SteamGameServer015_18[] = {
    "pszSpectatorServerName",
};
const char* const kParams_SteamGameServer015_20[] = {
    "pKey",
    "pValue",
};
const char* const kParams_SteamGameServer015_21[] = {
    "pchGameTags",
};
const char* const kParams_SteamGameServer015_22[] = {
    "pchGameData",
};
const char* const kParams_SteamGameServer015_23[] = {
    "pszRegion",
};
const char* const kParams_SteamGameServer015_24[] = {
    "bActive",
};
const char* const kParams_SteamGameServer015_25[] = {
    "pTicket",
    "cbMaxTicket",
    "pcbTicket",
    "pSnid",
};
const char* const kParams_SteamGameServer015_26[] = {
    "pAuthTicket",
    "cbAuthTicket",
    "steamID",
};
const char* const kParams_SteamGameServer015_27[] = {
    "steamID",
};
const char* const kParams_SteamGameServer015_28[] = {
    "hAuthTicket",
};
const char* const kParams_SteamGameServer015_29[] = {
    "steamID",
    "appID",
};
const char* const kParams_SteamGameServer015_30[] = {
    "steamIDUser",
    "steamIDGroup",
};
const char* const kParams_SteamGameServer015_34[] = {
    "pData",
    "cbData",
    "srcIP",
    "srcPort",
};
const char* const kParams_SteamGameServer015_35[] = {
    "pOut",
    "cbMaxOut",
    "pNetAdr",
    "pPort",
};
const char* const kParams_SteamGameServer015_36[] = {
    "steamIDClan",
};
const char* const kParams_SteamGameServer015_37[] = {
    "steamIDNewPlayer",
};
const char* const kParams_SteamGameServer015_38[] = {
    "unIPClient",
    "pvAuthBlob",
    "cubAuthBlobSize",
    "pSteamIDUser",
};
const char* const kParams_SteamGameServer015_40[] = {
    "steamIDUser",
};
const char* const kParams_SteamGameServer015_41[] = {
    "steamIDUser",
    "pchPlayerName",
    "uScore",
};
const char* const kParams_SteamGameServer015_42[] = {
    "iHeartbeatInterval",
};
// ISteamGameServer SteamGameServer015
const steambridge::SlotInfo kSlots_SteamGameServer015[] = {
    {"ISteamGameServer::InitGameServer", kParams_SteamGameServer015_0},
    {"SteamAPI_ISteamGameServer_SetProduct", kParams_SteamGameServer015_1},
    {"SteamAPI_ISteamGameServer_SetGameDescription", kParams_SteamGameServer015_2},
    {"SteamAPI_ISteamGameServer_SetModDir", kParams_SteamGameServer015_3},
    {"SteamAPI_ISteamGameServer_SetDedicatedServer", kParams_SteamGameServer015_4},
    {"SteamAPI_ISteamGameServer_LogOn", kParams_SteamGameServer015_5},
    {"SteamAPI_ISteamGameServer_LogOnAnonymous", nullptr},
    {"SteamAPI_ISteamGameServer_LogOff", nullptr},
    {"SteamAPI_ISteamGameServer_BLoggedOn", nullptr},
    {"SteamAPI_ISteamGameServer_BSecure", nullptr},
    {"SteamAPI_ISteamGameServer_GetSteamID", nullptr},
    {"SteamAPI_ISteamGameServer_WasRestartRequested", nullptr},
    {"SteamAPI_ISteamGameServer_SetMaxPlayerCount", kParams_SteamGameServer015_12},
    {"SteamAPI_ISteamGameServer_SetBotPlayerCount", kParams_SteamGameServer015_13},
    {"SteamAPI_ISteamGameServer_SetServerName", kParams_SteamGameServer015_14},
    {"SteamAPI_ISteamGameServer_SetMapName", kParams_SteamGameServer015_15},
    {"SteamAPI_ISteamGameServer_SetPasswordProtected", kParams_SteamGameServer015_16},
    {"SteamAPI_ISteamGameServer_SetSpectatorPort", kParams_SteamGameServer015_17},
    {"SteamAPI_ISteamGameServer_SetSpectatorServerName", kParams_SteamGameServer015_18},
    {"SteamAPI_ISteamGameServer_ClearAllKeyValues", nullptr},
    {"SteamAPI_ISteamGameServer_SetKeyValue", kParams_SteamGameServer015_20},
    {"SteamAPI_ISteamGameServer_SetGameTags", kParams_SteamGameServer015_21},
    {"SteamAPI_ISteamGameServer_SetGameData", kParams_SteamGameServer015_22},
    {"SteamAPI_ISteamGameServer_SetRegion", kParams_SteamGameServer015_23},
    {"SteamAPI_ISteamGameServer_SetAdvertiseServerActive", kParams_SteamGameServer015_24},
    {"SteamAPI_ISteamGameServer_GetAuthSessionTicket", kParams_SteamGameServer015_25},
    {"SteamAPI_ISteamGameServer_BeginAuthSession", kParams_SteamGameServer015_26},
    {"SteamAPI_ISteamGameServer_EndAuthSession", kParams_SteamGameServer015_27},
    {"SteamAPI_ISteamGameServer_CancelAuthTicket", kParams_SteamGameServer015_28},
    {"SteamAPI_ISteamGameServer_UserHasLicenseForApp", kParams_SteamGameServer015_29},
    {"SteamAPI_ISteamGameServer_RequestUserGroupStatus", kParams_SteamGameServer015_30},
    {"SteamAPI_ISteamGameServer_GetGameplayStats", nullptr},
    {"SteamAPI_ISteamGameServer_GetServerReputation", nullptr},
    {"SteamAPI_ISteamGameServer_GetPublicIP", nullptr},
    {"SteamAPI_ISteamGameServer_HandleIncomingPacket", kParams_SteamGameServer015_34},
    {"SteamAPI_ISteamGameServer_GetNextOutgoingPacket", kParams_SteamGameServer015_35},
    {"SteamAPI_ISteamGameServer_AssociateWithClan", kParams_SteamGameServer015_36},
    {"SteamAPI_ISteamGameServer_ComputeNewPlayerCompatibility", kParams_SteamGameServer015_37},
    {"SteamAPI_ISteamGameServer_SendUserConnectAndAuthenticate_DEPRECATED", kParams_SteamGameServer015_38},
    {"SteamAPI_ISteamGameServer_CreateUnauthenticatedUserConnection", nullptr},
    {"SteamAPI_ISteamGameServer_SendUserDisconnect_DEPRECATED", kParams_SteamGameServer015_40},
    {"SteamAPI_ISteamGameServer_BUpdateUserData", kParams_SteamGameServer015_41},
    {"ISteamGameServer::SetMasterServerHeartbeatInterval_DEPRECATED", kParams_SteamGameServer015_42},
    {"ISteamGameServer::ForceMasterServerHeartbeat_DEPRECATED", nullptr},
};

class Version_SteamGameServer015 {
public:
    virtual bool InitGameServer(std::uint32_t unIP, std::uint16_t usGamePort, std::uint16_t usQueryPort, std::uint32_t unFlags, std::uint32_t nGameAppId, const char* pchVersionString) {
        return steambridge::slot<bool>(kSlots_SteamGameServer015[0], unIP, usGamePort, usQueryPort, unFlags, nGameAppId, pchVersionString);
    }
    virtual void SetProduct(const char* pszProduct) {
        steambridge::slot<void>(kSlots_SteamGameServer015[1], pszProduct);
    }
    virtual void SetGameDescription(const char* pszGameDescription) {
        steambridge::slot<void>(kSlots_SteamGameServer015[2], pszGameDescription);
    }
    virtual void SetModDir(const char* pszModDir) {
        steambridge::slot<void>(kSlots_SteamGameServer015[3], pszModDir);
    }
    virtual void SetDedicatedServer(bool bDedicated) {
        steambridge::slot<void>(kSlots_SteamGameServer015[4], bDedicated);
    }
    virtual void LogOn(const char* pszToken) {
        steambridge::slot<void>(kSlots_SteamGameServer015[5], pszToken);
    }
    virtual void LogOnAnonymous() {
        steambridge::slot<void>(kSlots_SteamGameServer015[6]);
    }
    virtual void LogOff() {
        steambridge::slot<void>(kSlots_SteamGameServer015[7]);
    }
    virtual bool BLoggedOn() {
        return steambridge::slot<bool>(kSlots_SteamGameServer015[8]);
    }
    virtual bool BSecure() {
        return steambridge::slot<bool>(kSlots_SteamGameServer015[9]);
    }
    virtual CSteamID GetSteamID() {
        return steambridge::slot<CSteamID>(kSlots_SteamGameServer015[10]);
    }
    virtual bool WasRestartRequested() {
        return steambridge::slot<bool>(kSlots_SteamGameServer015[11]);
    }
    virtual void SetMaxPlayerCount(std::int32_t cPlayersMax) {
        steambridge::slot<void>(kSlots_SteamGameServer015[12], cPlayersMax);
    }
    virtual void SetBotPlayerCount(std::int32_t cBotplayers) {
        steambridge::slot<void>(kSlots_SteamGameServer015[13], cBotplayers);
    }
    virtual void SetServerName(const char* pszServerName) {
        steambridge::slot<void>(kSlots_SteamGameServer015[14], pszServerName);
    }
    virtual void SetMapName(const char* pszMapName) {
        steambridge::slot<void>(kSlots_SteamGameServer015[15], pszMapName);
    }
    virtual void SetPasswordProtected(bool bPasswordProtected) {
        steambridge::slot<void>(kSlots_SteamGameServer015[16], bPasswordProtected);
    }
    virtual void SetSpectatorPort(std::uint16_t unSpectatorPort) {
        steambridge::slot<void>(kSlots_SteamGameServer015[17], unSpectatorPort);
    }
    virtual void SetSpectatorServerName(const char* pszSpectatorServerName) {
        steambridge::slot<void>(kSlots_SteamGameServer015[18], pszSpectatorServerName);
    }
    virtual void ClearAllKeyValues() {
        steambridge::slot<void>(kSlots_SteamGameServer015[19]);
    }
    virtual void SetKeyValue(const char* pKey, const char* pValue) {
        steambridge::slot<void>(kSlots_SteamGameServer015[20], pKey, pValue);
    }
    virtual void SetGameTags(const char* pchGameTags) {
        steambridge::slot<void>(kSlots_SteamGameServer015[21], pchGameTags);
    }
    virtual void SetGameData(const char* pchGameData) {
        steambridge::slot<void>(kSlots_SteamGameServer015[22], pchGameData);
    }
    virtual void SetRegion(const char* pszRegion) {
        steambridge::slot<void>(kSlots_SteamGameServer015[23], pszRegion);
    }
    virtual void SetAdvertiseServerActive(bool bActive) {
        steambridge::slot<void>(kSlots_SteamGameServer015[24], bActive);
    }
    virtual std::uint32_t GetAuthSessionTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket, void* pSnid) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamGameServer015[25], pTicket, cbMaxTicket, pcbTicket, pSnid);
    }
    virtual std::int32_t BeginAuthSession(void* pAuthTicket, std::int32_t cbAuthTicket, CSteamID steamID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamGameServer015[26], pAuthTicket, cbAuthTicket, steamID);
    }
    virtual void EndAuthSession(CSteamID steamID) {
        steambridge::slot<void>(kSlots_SteamGameServer015[27], steamID);
    }
    virtual void CancelAuthTicket(std::uint32_t hAuthTicket) {
        steambridge::slot<void>(kSlots_SteamGameServer015[28], hAuthTicket);
    }
    virtual std::int32_t UserHasLicenseForApp(CSteamID steamID, std::uint32_t appID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamGameServer015[29], steamID, appID);
    }
    virtual bool RequestUserGroupStatus(CSteamID steamIDUser, CSteamID steamIDGroup) {
        return steambridge::slot<bool>(kSlots_SteamGameServer015[30], steamIDUser, steamIDGroup);
    }
    virtual void GetGameplayStats() {
        steambridge::slot<void>(kSlots_SteamGameServer015[31]);
    }
    virtual std::uint64_t GetServerReputation() {
        return steambridge::slot<std::uint64_t>(kSlots_SteamGameServer015[32]);
    }
    virtual SteamIPAddress_t GetPublicIP() {
        return steambridge::slot<SteamIPAddress_t>(kSlots_SteamGameServer015[33]);
    }
    virtual bool HandleIncomingPacket(void* pData, std::int32_t cbData, std::uint32_t srcIP, std::uint16_t srcPort) {
        return steambridge::slot<bool>(kSlots_SteamGameServer015[34], pData, cbData, srcIP, srcPort);
    }
    virtual std::int32_t GetNextOutgoingPacket(void* pOut, std::int32_t cbMaxOut, std::uint32_t* pNetAdr, std::uint16_t* pPort) {
        return steambridge::slot<std::int32_t>(kSlots_SteamGameServer015[35], pOut, cbMaxOut, pNetAdr, pPort);
    }
    virtual std::uint64_t AssociateWithClan(CSteamID steamIDClan) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamGameServer015[36], steamIDClan);
    }
    virtual std::uint64_t ComputeNewPlayerCompatibility(CSteamID steamIDNewPlayer) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamGameServer015[37], steamIDNewPlayer);
    }
    virtual bool SendUserConnectAndAuthenticate_DEPRECATED(std::uint32_t unIPClient, void* pvAuthBlob, std::uint32_t cubAuthBlobSize, CSteamID* pSteamIDUser) {
        return steambridge::slot<bool>(kSlots_SteamGameServer015[38], unIPClient, pvAuthBlob, cubAuthBlobSize, pSteamIDUser);
    }
    virtual CSteamID CreateUnauthenticatedUserConnection() {
        return steambridge::slot<CSteamID>(kSlots_SteamGameServer015[39]);
    }
    virtual void SendUserDisconnect_DEPRECATED(CSteamID steamIDUser) {
        steambridge::slot<void>(kSlots_SteamGameServer015[40], steamIDUser);
    }
    virtual bool BUpdateUserData(CSteamID steamIDUser, const char* pchPlayerName, std::uint32_t uScore) {
        return steambridge::slot<bool>(kSlots_SteamGameServer015[41], steamIDUser, pchPlayerName, uScore);
    }
    virtual void SetMasterServerHeartbeatInterval_DEPRECATED(std::int32_t iHeartbeatInterval) {
        steambridge::slot<void>(kSlots_SteamGameServer015[42], iHeartbeatInterval);
    }
    virtual void ForceMasterServerHeartbeat_DEPRECATED() {
        steambridge::slot<void>(kSlots_SteamGameServer015[43]);
    }
};

Version_SteamGameServer015 g_SteamGameServer015;

const char* const kParams_SteamGameServerStats001_0[] = {
    "steamIDUser",
};
const char* const kParams_SteamGameServerStats001_1[] = {
    "steamIDUser",
    "pchName",
    "pData",
};
const char* const kParams_SteamGameServerStats001_2[] = {
    "steamIDUser",
    "pchName",
    "pData",
};
const char* const kParams_SteamGameServerStats001_3[] = {
    "steamIDUser",
    "pchName",
    "pbAchieved",
};
const char* const kParams_SteamGameServerStats001_4[] = {
    "steamIDUser",
    "pchName",
    "nData",
};
const char* const kParams_SteamGameServerStats001_5[] = {
    "steamIDUser",
    "pchName",
    "fData",
};
const char* const kParams_SteamGameServerStats001_6[] = {
    "steamIDUser",
    "pchName",
    "flCountThisSession",
    "dSessionLength",
};
const char* const kParams_SteamGameServerStats001_7[] = {
    "steamIDUser",
    "pchName",
};
const char* const kParams_SteamGameServerStats001_8[] = {
    "steamIDUser",
    "pchName",
};
const char* const kParams_SteamGameServerStats001_9[] = {
    "steamIDUser",
};
// ISteamGameServerStats SteamGameServerStats001
const steambridge::SlotInfo kSlots_SteamGameServerStats001[] = {
    {"SteamAPI_ISteamGameServerStats_RequestUserStats", kParams_SteamGameServerStats001_0},
    {"SteamAPI_ISteamGameServerStats_GetUserStatInt32", kParams_SteamGameServerStats001_1},
    {"SteamAPI_ISteamGameServerStats_GetUserStatFloat", kParams_SteamGameServerStats001_2},
    {"SteamAPI_ISteamGameServerStats_GetUserAchievement", kParams_SteamGameServerStats001_3},
    {"SteamAPI_ISteamGameServerStats_SetUserStatInt32", kParams_SteamGameServerStats001_4},
    {"SteamAPI_ISteamGameServerStats_SetUserStatFloat", kParams_SteamGameServerStats001_5},
    {"SteamAPI_ISteamGameServerStats_UpdateUserAvgRateStat", kParams_SteamGameServerStats001_6},
    {"SteamAPI_ISteamGameServerStats_SetUserAchievement", kParams_SteamGameServerStats001_7},
    {"SteamAPI_ISteamGameServerStats_ClearUserAchievement", kParams_SteamGameServerStats001_8},
    {"SteamAPI_ISteamGameServerStats_StoreUserStats", kParams_SteamGameServerStats001_9},
};

class Version_SteamGameServerStats001 {
public:
    virtual std::uint64_t RequestUserStats(CSteamID steamIDUser) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamGameServerStats001[0], steamIDUser);
    }
    virtual bool GetUserStat(CSteamID steamIDUser, const char* pchName, std::int32_t* pData) {
        return steambridge::slot<bool>(kSlots_SteamGameServerStats001[1], steamIDUser, pchName, pData);
    }
    virtual bool GetUserStat(CSteamID steamIDUser, const char* pchName, float* pData) {
        return steambridge::slot<bool>(kSlots_SteamGameServerStats001[2], steamIDUser, pchName, pData);
    }
    virtual bool GetUserAchievement(CSteamID steamIDUser, const char* pchName, bool* pbAchieved) {
        return steambridge::slot<bool>(kSlots_SteamGameServerStats001[3], steamIDUser, pchName, pbAchieved);
    }
    virtual bool SetUserStat(CSteamID steamIDUser, const char* pchName, std::int32_t nData) {
        return steambridge::slot<bool>(kSlots_SteamGameServerStats001[4], steamIDUser, pchName, nData);
    }
    virtual bool SetUserStat(CSteamID steamIDUser, const char* pchName, float fData) {
        return steambridge::slot<bool>(kSlots_SteamGameServerStats001[5], steamIDUser, pchName, fData);
    }
    virtual bool UpdateUserAvgRateStat(CSteamID steamIDUser, const char* pchName, float flCountThisSession, double dSessionLength) {
        return steambridge::slot<bool>(kSlots_SteamGameServerStats001[6], steamIDUser, pchName, flCountThisSession, dSessionLength);
    }
    virtual bool SetUserAchievement(CSteamID steamIDUser, const char* pchName) {
        return steambridge::slot<bool>(kSlots_SteamGameServerStats001[7], steamIDUser, pchName);
    }
    virtual bool ClearUserAchievement(CSteamID steamIDUser, const char* pchName) {
        return steambridge::slot<bool>(kSlots_SteamGameServerStats001[8], steamIDUser, pchName);
    }
    virtual std::uint64_t StoreUserStats(CSteamID steamIDUser) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamGameServerStats001[9], steamIDUser);
    }
};

Version_SteamGameServerStats001 g_SteamGameServerStats001;

const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_3[] = {
    "pchUserAgent",
    "pchUserCSS",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_4[] = {
    "unBrowserHandle",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_5[] = {
    "unBrowserHandle",
    "pchURL",
    "pchPostData",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_6[] = {
    "unBrowserHandle",
    "unWidth",
    "unHeight",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_7[] = {
    "unBrowserHandle",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_8[] = {
    "unBrowserHandle",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_9[] = {
    "unBrowserHandle",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_10[] = {
    "unBrowserHandle",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_11[] = {
    "unBrowserHandle",
    "pchKey",
    "pchValue",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_12[] = {
    "unBrowserHandle",
    "pchScript",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_13[] = {
    "unBrowserHandle",
    "eMouseButton",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_14[] = {
    "unBrowserHandle",
    "eMouseButton",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_15[] = {
    "unBrowserHandle",
    "eMouseButton",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_16[] = {
    "unBrowserHandle",
    "x",
    "y",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_17[] = {
    "unBrowserHandle",
    "nDelta",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_18[] = {
    "unBrowserHandle",
    "nNativeKeyCode",
    "eHTMLKeyModifiers",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_19[] = {
    "unBrowserHandle",
    "nNativeKeyCode",
    "eHTMLKeyModifiers",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_20[] = {
    "unBrowserHandle",
    "cUnicodeChar",
    "eHTMLKeyModifiers",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_21[] = {
    "unBrowserHandle",
    "nAbsolutePixelScroll",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_22[] = {
    "unBrowserHandle",
    "nAbsolutePixelScroll",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_23[] = {
    "unBrowserHandle",
    "bHasKeyFocus",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_24[] = {
    "unBrowserHandle",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_25[] = {
    "unBrowserHandle",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_26[] = {
    "unBrowserHandle",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_27[] = {
    "unBrowserHandle",
    "pchSearchStr",
    "bCurrentlyInFind",
    "bReverse",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_28[] = {
    "unBrowserHandle",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_29[] = {
    "unBrowserHandle",
    "x",
    "y",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_30[] = {
    "pchHostname",
    "pchKey",
    "pchValue",
    "pchPath",
    "nExpires",
    "bSecure",
    "bHTTPOnly",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_31[] = {
    "unBrowserHandle",
    "flZoom",
    "nPointX",
    "nPointY",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_32[] = {
    "unBrowserHandle",
    "bBackgroundMode",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_33[] = {
    "unBrowserHandle",
    "flDPIScaling",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_34[] = {
    "unBrowserHandle",
    "bAllowed",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_35[] = {
    "unBrowserHandle",
    "bResult",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_36[] = {
    "unBrowserHandle",
    "pchSelectedFiles",
};
// ISteamHTMLSurface STEAMHTMLSURFACE_INTERFACE_VERSION_004
const steambridge::SlotInfo kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[] = {
    {nullptr, nullptr},  // the virtual destructor
    {"SteamAPI_ISteamHTMLSurface_Init", nullptr},
    {"SteamAPI_ISteamHTMLSurface_Shutdown", nullptr},
    {"SteamAPI_ISteamHTMLSurface_CreateBrowser", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_3},
    {"SteamAPI_ISteamHTMLSurface_RemoveBrowser", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_4},
    {"SteamAPI_ISteamHTMLSurface_LoadURL", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_5},
    {"SteamAPI_ISteamHTMLSurface_SetSize", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_6},
    {"SteamAPI_ISteamHTMLSurface_StopLoad", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_7},
    {"SteamAPI_ISteamHTMLSurface_Reload", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_8},
    {"SteamAPI_ISteamHTMLSurface_GoBack", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_9},
    {"SteamAPI_ISteamHTMLSurface_GoForward", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_10},
    {"SteamAPI_ISteamHTMLSurface_AddHeader", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_11},
    {"SteamAPI_ISteamHTMLSurface_ExecuteJavascript", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_12},
    {"SteamAPI_ISteamHTMLSurface_MouseUp", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_13},
    {"SteamAPI_ISteamHTMLSurface_MouseDown", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_14},
    {"SteamAPI_ISteamHTMLSurface_MouseDoubleClick", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_15},
    {"SteamAPI_ISteamHTMLSurface_MouseMove", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_16},
    {"SteamAPI_ISteamHTMLSurface_MouseWheel", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_17},
    {"SteamAPI_ISteamHTMLSurface_KeyDown", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_18},
    {"SteamAPI_ISteamHTMLSurface_KeyUp", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_19},
    {"SteamAPI_ISteamHTMLSurface_KeyChar", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_20},
    {"SteamAPI_ISteamHTMLSurface_SetHorizontalScroll", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_21},
    {"SteamAPI_ISteamHTMLSurface_SetVerticalScroll", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_22},
    {"SteamAPI_ISteamHTMLSurface_SetKeyFocus", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_23},
    {"SteamAPI_ISteamHTMLSurface_ViewSource", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_24},
    {"SteamAPI_ISteamHTMLSurface_CopyToClipboard", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_25},
    {"SteamAPI_ISteamHTMLSurface_PasteFromClipboard", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_26},
    {"SteamAPI_ISteamHTMLSurface_Find", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_27},
    {"SteamAPI_ISteamHTMLSurface_StopFind", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_28},
    {"SteamAPI_ISteamHTMLSurface_GetLinkAtPosition", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_29},
    {"SteamAPI_ISteamHTMLSurface_SetCookie", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_30},
    {"SteamAPI_ISteamHTMLSurface_SetPageScaleFactor", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_31},
    {"SteamAPI_ISteamHTMLSurface_SetBackgroundMode", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_32},
    {"SteamAPI_ISteamHTMLSurface_SetDPIScalingFactor", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_33},
    {"SteamAPI_ISteamHTMLSurface_AllowStartRequest", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_34},
    {"SteamAPI_ISteamHTMLSurface_JSDialogResponse", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_35},
    {"ISteamHTMLSurface::FileLoadDialogResponse", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_004_36},
};

class Version_STEAMHTMLSURFACE_INTERFACE_VERSION_004 {
public:
    virtual ~Version_STEAMHTMLSURFACE_INTERFACE_VERSION_004() {}
    virtual bool Init() {
        return steambridge::slot<bool>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[1]);
    }
    virtual bool Shutdown() {
        return steambridge::slot<bool>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[2]);
    }
    virtual std::uint64_t CreateBrowser(const char* pchUserAgent, const char* pchUserCSS) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[3], pchUserAgent, pchUserCSS);
    }
    virtual void RemoveBrowser(std::uint32_t unBrowserHandle) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[4], unBrowserHandle);
    }
    virtual void LoadURL(std::uint32_t unBrowserHandle, const char* pchURL, const char* pchPostData) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[5], unBrowserHandle, pchURL, pchPostData);
    }
    virtual void SetSize(std::uint32_t unBrowserHandle, std::uint32_t unWidth, std::uint32_t unHeight) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[6], unBrowserHandle, unWidth, unHeight);
    }
    virtual void StopLoad(std::uint32_t unBrowserHandle) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[7], unBrowserHandle);
    }
    virtual void Reload(std::uint32_t unBrowserHandle) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[8], unBrowserHandle);
    }
    virtual void GoBack(std::uint32_t unBrowserHandle) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[9], unBrowserHandle);
    }
    virtual void GoForward(std::uint32_t unBrowserHandle) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[10], unBrowserHandle);
    }
    virtual void AddHeader(std::uint32_t unBrowserHandle, const char* pchKey, const char* pchValue) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[11], unBrowserHandle, pchKey, pchValue);
    }
    virtual void ExecuteJavascript(std::uint32_t unBrowserHandle, const char* pchScript) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[12], unBrowserHandle, pchScript);
    }
    virtual void MouseUp(std::uint32_t unBrowserHandle, std::int32_t eMouseButton) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[13], unBrowserHandle, eMouseButton);
    }
    virtual void MouseDown(std::uint32_t unBrowserHandle, std::int32_t eMouseButton) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[14], unBrowserHandle, eMouseButton);
    }
    virtual void MouseDoubleClick(std::uint32_t unBrowserHandle, std::int32_t eMouseButton) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[15], unBrowserHandle, eMouseButton);
    }
    virtual void MouseMove(std::uint32_t unBrowserHandle, std::int32_t x, std::int32_t y) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[16], unBrowserHandle, x, y);
    }
    virtual void MouseWheel(std::uint32_t unBrowserHandle, std::int32_t nDelta) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[17], unBrowserHandle, nDelta);
    }
    virtual void KeyDown(std::uint32_t unBrowserHandle, std::uint32_t nNativeKeyCode, std::int32_t eHTMLKeyModifiers) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[18], unBrowserHandle, nNativeKeyCode, eHTMLKeyModifiers);
    }
    virtual void KeyUp(std::uint32_t unBrowserHandle, std::uint32_t nNativeKeyCode, std::int32_t eHTMLKeyModifiers) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[19], unBrowserHandle, nNativeKeyCode, eHTMLKeyModifiers);
    }
    virtual void KeyChar(std::uint32_t unBrowserHandle, std::uint32_t cUnicodeChar, std::int32_t eHTMLKeyModifiers) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[20], unBrowserHandle, cUnicodeChar, eHTMLKeyModifiers);
    }
    virtual void SetHorizontalScroll(std::uint32_t unBrowserHandle, std::uint32_t nAbsolutePixelScroll) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[21], unBrowserHandle, nAbsolutePixelScroll);
    }
    virtual void SetVerticalScroll(std::uint32_t unBrowserHandle, std::uint32_t nAbsolutePixelScroll) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[22], unBrowserHandle, nAbsolutePixelScroll);
    }
    virtual void SetKeyFocus(std::uint32_t unBrowserHandle, bool bHasKeyFocus) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[23], unBrowserHandle, bHasKeyFocus);
    }
    virtual void ViewSource(std::uint32_t unBrowserHandle) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[24], unBrowserHandle);
    }
    virtual void CopyToClipboard(std::uint32_t unBrowserHandle) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[25], unBrowserHandle);
    }
    virtual void PasteFromClipboard(std::uint32_t unBrowserHandle) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[26], unBrowserHandle);
    }
    virtual void Find(std::uint32_t unBrowserHandle, const char* pchSearchStr, bool bCurrentlyInFind, bool bReverse) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[27], unBrowserHandle, pchSearchStr, bCurrentlyInFind, bReverse);
    }
    virtual void StopFind(std::uint32_t unBrowserHandle) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[28], unBrowserHandle);
    }
    virtual void GetLinkAtPosition(std::uint32_t unBrowserHandle, std::int32_t x, std::int32_t y) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[29], unBrowserHandle, x, y);
    }
    virtual void SetCookie(const char* pchHostname, const char* pchKey, const char* pchValue, const char* pchPath, std::uint32_t nExpires, bool bSecure, bool bHTTPOnly) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[30], pchHostname, pchKey, pchValue, pchPath, nExpires, bSecure, bHTTPOnly);
    }
    virtual void SetPageScaleFactor(std::uint32_t unBrowserHandle, float flZoom, std::int32_t nPointX, std::int32_t nPointY) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[31], unBrowserHandle, flZoom, nPointX, nPointY);
    }
    virtual void SetBackgroundMode(std::uint32_t unBrowserHandle, bool bBackgroundMode) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[32], unBrowserHandle, bBackgroundMode);
    }
    virtual void SetDPIScalingFactor(std::uint32_t unBrowserHandle, float flDPIScaling) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[33], unBrowserHandle, flDPIScaling);
    }
    virtual void AllowStartRequest(std::uint32_t unBrowserHandle, bool bAllowed) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[34], unBrowserHandle, bAllowed);
    }
    virtual void JSDialogResponse(std::uint32_t unBrowserHandle, bool bResult) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[35], unBrowserHandle, bResult);
    }
    virtual void FileLoadDialogResponse(std::uint32_t unBrowserHandle, void* pchSelectedFiles) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_004[36], unBrowserHandle, pchSelectedFiles);
    }
};

Version_STEAMHTMLSURFACE_INTERFACE_VERSION_004 g_STEAMHTMLSURFACE_INTERFACE_VERSION_004;

const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_3[] = {
    "pchUserAgent",
    "pchUserCSS",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_4[] = {
    "unBrowserHandle",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_5[] = {
    "unBrowserHandle",
    "pchURL",
    "pchPostData",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_6[] = {
    "unBrowserHandle",
    "unWidth",
    "unHeight",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_7[] = {
    "unBrowserHandle",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_8[] = {
    "unBrowserHandle",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_9[] = {
    "unBrowserHandle",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_10[] = {
    "unBrowserHandle",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_11[] = {
    "unBrowserHandle",
    "pchKey",
    "pchValue",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_12[] = {
    "unBrowserHandle",
    "pchScript",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_13[] = {
    "unBrowserHandle",
    "eMouseButton",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_14[] = {
    "unBrowserHandle",
    "eMouseButton",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_15[] = {
    "unBrowserHandle",
    "eMouseButton",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_16[] = {
    "unBrowserHandle",
    "x",
    "y",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_17[] = {
    "unBrowserHandle",
    "nDelta",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_18[] = {
    "unBrowserHandle",
    "nNativeKeyCode",
    "eHTMLKeyModifiers",
    "bIsSystemKey",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_19[] = {
    "unBrowserHandle",
    "nNativeKeyCode",
    "eHTMLKeyModifiers",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_20[] = {
    "unBrowserHandle",
    "cUnicodeChar",
    "eHTMLKeyModifiers",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_21[] = {
    "unBrowserHandle",
    "nAbsolutePixelScroll",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_22[] = {
    "unBrowserHandle",
    "nAbsolutePixelScroll",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_23[] = {
    "unBrowserHandle",
    "bHasKeyFocus",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_24[] = {
    "unBrowserHandle",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_25[] = {
    "unBrowserHandle",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_26[] = {
    "unBrowserHandle",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_27[] = {
    "unBrowserHandle",
    "pchSearchStr",
    "bCurrentlyInFind",
    "bReverse",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_28[] = {
    "unBrowserHandle",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_29[] = {
    "unBrowserHandle",
    "x",
    "y",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_30[] = {
    "pchHostname",
    "pchKey",
    "pchValue",
    "pchPath",
    "nExpires",
    "bSecure",
    "bHTTPOnly",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_31[] = {
    "unBrowserHandle",
    "flZoom",
    "nPointX",
    "nPointY",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_32[] = {
    "unBrowserHandle",
    "bBackgroundMode",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_33[] = {
    "unBrowserHandle",
    "flDPIScaling",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_34[] = {
    "unBrowserHandle",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_35[] = {
    "unBrowserHandle",
    "bAllowed",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_36[] = {
    "unBrowserHandle",
    "bResult",
};
const char* const kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_37[] = {
    "unBrowserHandle",
    "pchSelectedFiles",
};
// ISteamHTMLSurface STEAMHTMLSURFACE_INTERFACE_VERSION_005
const steambridge::SlotInfo kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[] = {
    {nullptr, nullptr},  // the virtual destructor
    {"SteamAPI_ISteamHTMLSurface_Init", nullptr},
    {"SteamAPI_ISteamHTMLSurface_Shutdown", nullptr},
    {"SteamAPI_ISteamHTMLSurface_CreateBrowser", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_3},
    {"SteamAPI_ISteamHTMLSurface_RemoveBrowser", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_4},
    {"SteamAPI_ISteamHTMLSurface_LoadURL", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_5},
    {"SteamAPI_ISteamHTMLSurface_SetSize", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_6},
    {"SteamAPI_ISteamHTMLSurface_StopLoad", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_7},
    {"SteamAPI_ISteamHTMLSurface_Reload", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_8},
    {"SteamAPI_ISteamHTMLSurface_GoBack", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_9},
    {"SteamAPI_ISteamHTMLSurface_GoForward", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_10},
    {"SteamAPI_ISteamHTMLSurface_AddHeader", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_11},
    {"SteamAPI_ISteamHTMLSurface_ExecuteJavascript", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_12},
    {"SteamAPI_ISteamHTMLSurface_MouseUp", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_13},
    {"SteamAPI_ISteamHTMLSurface_MouseDown", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_14},
    {"SteamAPI_ISteamHTMLSurface_MouseDoubleClick", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_15},
    {"SteamAPI_ISteamHTMLSurface_MouseMove", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_16},
    {"SteamAPI_ISteamHTMLSurface_MouseWheel", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_17},
    {"SteamAPI_ISteamHTMLSurface_KeyDown", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_18},
    {"SteamAPI_ISteamHTMLSurface_KeyUp", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_19},
    {"SteamAPI_ISteamHTMLSurface_KeyChar", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_20},
    {"SteamAPI_ISteamHTMLSurface_SetHorizontalScroll", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_21},
    {"SteamAPI_ISteamHTMLSurface_SetVerticalScroll", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_22},
    {"SteamAPI_ISteamHTMLSurface_SetKeyFocus", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_23},
    {"SteamAPI_ISteamHTMLSurface_ViewSource", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_24},
    {"SteamAPI_ISteamHTMLSurface_CopyToClipboard", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_25},
    {"SteamAPI_ISteamHTMLSurface_PasteFromClipboard", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_26},
    {"SteamAPI_ISteamHTMLSurface_Find", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_27},
    {"SteamAPI_ISteamHTMLSurface_StopFind", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_28},
    {"SteamAPI_ISteamHTMLSurface_GetLinkAtPosition", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_29},
    {"SteamAPI_ISteamHTMLSurface_SetCookie", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_30},
    {"SteamAPI_ISteamHTMLSurface_SetPageScaleFactor", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_31},
    {"SteamAPI_ISteamHTMLSurface_SetBackgroundMode", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_32},
    {"SteamAPI_ISteamHTMLSurface_SetDPIScalingFactor", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_33},
    {"SteamAPI_ISteamHTMLSurface_OpenDeveloperTools", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_34},
    {"SteamAPI_ISteamHTMLSurface_AllowStartRequest", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_35},
    {"SteamAPI_ISteamHTMLSurface_JSDialogResponse", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_36},
    {"SteamAPI_ISteamHTMLSurface_FileLoadDialogResponse", kParams_STEAMHTMLSURFACE_INTERFACE_VERSION_005_37},
};

class Version_STEAMHTMLSURFACE_INTERFACE_VERSION_005 {
public:
    virtual ~Version_STEAMHTMLSURFACE_INTERFACE_VERSION_005() {}
    virtual bool Init() {
        return steambridge::slot<bool>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[1]);
    }
    virtual bool Shutdown() {
        return steambridge::slot<bool>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[2]);
    }
    virtual std::uint64_t CreateBrowser(const char* pchUserAgent, const char* pchUserCSS) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[3], pchUserAgent, pchUserCSS);
    }
    virtual void RemoveBrowser(std::uint32_t unBrowserHandle) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[4], unBrowserHandle);
    }
    virtual void LoadURL(std::uint32_t unBrowserHandle, const char* pchURL, const char* pchPostData) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[5], unBrowserHandle, pchURL, pchPostData);
    }
    virtual void SetSize(std::uint32_t unBrowserHandle, std::uint32_t unWidth, std::uint32_t unHeight) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[6], unBrowserHandle, unWidth, unHeight);
    }
    virtual void StopLoad(std::uint32_t unBrowserHandle) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[7], unBrowserHandle);
    }
    virtual void Reload(std::uint32_t unBrowserHandle) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[8], unBrowserHandle);
    }
    virtual void GoBack(std::uint32_t unBrowserHandle) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[9], unBrowserHandle);
    }
    virtual void GoForward(std::uint32_t unBrowserHandle) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[10], unBrowserHandle);
    }
    virtual void AddHeader(std::uint32_t unBrowserHandle, const char* pchKey, const char* pchValue) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[11], unBrowserHandle, pchKey, pchValue);
    }
    virtual void ExecuteJavascript(std::uint32_t unBrowserHandle, const char* pchScript) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[12], unBrowserHandle, pchScript);
    }
    virtual void MouseUp(std::uint32_t unBrowserHandle, std::int32_t eMouseButton) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[13], unBrowserHandle, eMouseButton);
    }
    virtual void MouseDown(std::uint32_t unBrowserHandle, std::int32_t eMouseButton) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[14], unBrowserHandle, eMouseButton);
    }
    virtual void MouseDoubleClick(std::uint32_t unBrowserHandle, std::int32_t eMouseButton) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[15], unBrowserHandle, eMouseButton);
    }
    virtual void MouseMove(std::uint32_t unBrowserHandle, std::int32_t x, std::int32_t y) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[16], unBrowserHandle, x, y);
    }
    virtual void MouseWheel(std::uint32_t unBrowserHandle, std::int32_t nDelta) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[17], unBrowserHandle, nDelta);
    }
    virtual void KeyDown(std::uint32_t unBrowserHandle, std::uint32_t nNativeKeyCode, std::int32_t eHTMLKeyModifiers, bool bIsSystemKey) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[18], unBrowserHandle, nNativeKeyCode, eHTMLKeyModifiers, bIsSystemKey);
    }
    virtual void KeyUp(std::uint32_t unBrowserHandle, std::uint32_t nNativeKeyCode, std::int32_t eHTMLKeyModifiers) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[19], unBrowserHandle, nNativeKeyCode, eHTMLKeyModifiers);
    }
    virtual void KeyChar(std::uint32_t unBrowserHandle, std::uint32_t cUnicodeChar, std::int32_t eHTMLKeyModifiers) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[20], unBrowserHandle, cUnicodeChar, eHTMLKeyModifiers);
    }
    virtual void SetHorizontalScroll(std::uint32_t unBrowserHandle, std::uint32_t nAbsolutePixelScroll) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[21], unBrowserHandle, nAbsolutePixelScroll);
    }
    virtual void SetVerticalScroll(std::uint32_t unBrowserHandle, std::uint32_t nAbsolutePixelScroll) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[22], unBrowserHandle, nAbsolutePixelScroll);
    }
    virtual void SetKeyFocus(std::uint32_t unBrowserHandle, bool bHasKeyFocus) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[23], unBrowserHandle, bHasKeyFocus);
    }
    virtual void ViewSource(std::uint32_t unBrowserHandle) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[24], unBrowserHandle);
    }
    virtual void CopyToClipboard(std::uint32_t unBrowserHandle) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[25], unBrowserHandle);
    }
    virtual void PasteFromClipboard(std::uint32_t unBrowserHandle) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[26], unBrowserHandle);
    }
    virtual void Find(std::uint32_t unBrowserHandle, const char* pchSearchStr, bool bCurrentlyInFind, bool bReverse) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[27], unBrowserHandle, pchSearchStr, bCurrentlyInFind, bReverse);
    }
    virtual void StopFind(std::uint32_t unBrowserHandle) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[28], unBrowserHandle);
    }
    virtual void GetLinkAtPosition(std::uint32_t unBrowserHandle, std::int32_t x, std::int32_t y) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[29], unBrowserHandle, x, y);
    }
    virtual void SetCookie(const char* pchHostname, const char* pchKey, const char* pchValue, const char* pchPath, std::uint32_t nExpires, bool bSecure, bool bHTTPOnly) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[30], pchHostname, pchKey, pchValue, pchPath, nExpires, bSecure, bHTTPOnly);
    }
    virtual void SetPageScaleFactor(std::uint32_t unBrowserHandle, float flZoom, std::int32_t nPointX, std::int32_t nPointY) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[31], unBrowserHandle, flZoom, nPointX, nPointY);
    }
    virtual void SetBackgroundMode(std::uint32_t unBrowserHandle, bool bBackgroundMode) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[32], unBrowserHandle, bBackgroundMode);
    }
    virtual void SetDPIScalingFactor(std::uint32_t unBrowserHandle, float flDPIScaling) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[33], unBrowserHandle, flDPIScaling);
    }
    virtual void OpenDeveloperTools(std::uint32_t unBrowserHandle) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[34], unBrowserHandle);
    }
    virtual void AllowStartRequest(std::uint32_t unBrowserHandle, bool bAllowed) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[35], unBrowserHandle, bAllowed);
    }
    virtual void JSDialogResponse(std::uint32_t unBrowserHandle, bool bResult) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[36], unBrowserHandle, bResult);
    }
    virtual void FileLoadDialogResponse(std::uint32_t unBrowserHandle, void* pchSelectedFiles) {
        steambridge::slot<void>(kSlots_STEAMHTMLSURFACE_INTERFACE_VERSION_005[37], unBrowserHandle, pchSelectedFiles);
    }
};

Version_STEAMHTMLSURFACE_INTERFACE_VERSION_005 g_STEAMHTMLSURFACE_INTERFACE_VERSION_005;

const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_0[] = {
    "eHTTPRequestMethod",
    "pchAbsoluteURL",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_1[] = {
    "hRequest",
    "ulContextValue",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_2[] = {
    "hRequest",
    "unTimeoutSeconds",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_3[] = {
    "hRequest",
    "pchHeaderName",
    "pchHeaderValue",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_4[] = {
    "hRequest",
    "pchParamName",
    "pchParamValue",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_5[] = {
    "hRequest",
    "pCallHandle",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_6[] = {
    "hRequest",
    "pCallHandle",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_7[] = {
    "hRequest",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_8[] = {
    "hRequest",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_9[] = {
    "hRequest",
    "pchHeaderName",
    "unResponseHeaderSize",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_10[] = {
    "hRequest",
    "pchHeaderName",
    "pHeaderValueBuffer",
    "unBufferSize",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_11[] = {
    "hRequest",
    "unBodySize",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_12[] = {
    "hRequest",
    "pBodyDataBuffer",
    "unBufferSize",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_13[] = {
    "hRequest",
    "cOffset",
    "pBodyDataBuffer",
    "unBufferSize",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_14[] = {
    "hRequest",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_15[] = {
    "hRequest",
    "pflPercentOut",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_16[] = {
    "hRequest",
    "pchContentType",
    "pubBody",
    "unBodyLen",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_17[] = {
    "bAllowResponsesToModify",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_18[] = {
    "hCookieContainer",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_19[] = {
    "hCookieContainer",
    "pchHost",
    "pchUrl",
    "pchCookie",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_20[] = {
    "hRequest",
    "hCookieContainer",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_21[] = {
    "hRequest",
    "pchUserAgentInfo",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_22[] = {
    "hRequest",
    "bRequireVerifiedCertificate",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_23[] = {
    "hRequest",
    "unMilliseconds",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION002_24[] = {
    "hRequest",
    "pbWasTimedOut",
};
// ISteamHTTP STEAMHTTP_INTERFACE_VERSION002
const steambridge::SlotInfo kSlots_STEAMHTTP_INTERFACE_VERSION002[] = {
    {"SteamAPI_ISteamHTTP_CreateHTTPRequest", kParams_STEAMHTTP_INTERFACE_VERSION002_0},
    {"SteamAPI_ISteamHTTP_SetHTTPRequestContextValue", kParams_STEAMHTTP_INTERFACE_VERSION002_1},
    {"SteamAPI_ISteamHTTP_SetHTTPRequestNetworkActivityTimeout", kParams_STEAMHTTP_INTERFACE_VERSION002_2},
    {"SteamAPI_ISteamHTTP_SetHTTPRequestHeaderValue", kParams_STEAMHTTP_INTERFACE_VERSION002_3},
    {"SteamAPI_ISteamHTTP_SetHTTPRequestGetOrPostParameter", kParams_STEAMHTTP_INTERFACE_VERSION002_4},
    {"SteamAPI_ISteamHTTP_SendHTTPRequest", kParams_STEAMHTTP_INTERFACE_VERSION002_5},
    {"SteamAPI_ISteamHTTP_SendHTTPRequestAndStreamResponse", kParams_STEAMHTTP_INTERFACE_VERSION002_6},
    {"SteamAPI_ISteamHTTP_DeferHTTPRequest", kParams_STEAMHTTP_INTERFACE_VERSION002_7},
    {"SteamAPI_ISteamHTTP_PrioritizeHTTPRequest", kParams_STEAMHTTP_INTERFACE_VERSION002_8},
    {"SteamAPI_ISteamHTTP_GetHTTPResponseHeaderSize", kParams_STEAMHTTP_INTERFACE_VERSION002_9},
    {"SteamAPI_ISteamHTTP_GetHTTPResponseHeaderValue", kParams_STEAMHTTP_INTERFACE_VERSION002_10},
    {"SteamAPI_ISteamHTTP_GetHTTPResponseBodySize", kParams_STEAMHTTP_INTERFACE_VERSION002_11},
    {"SteamAPI_ISteamHTTP_GetHTTPResponseBodyData", kParams_STEAMHTTP_INTERFACE_VERSION002_12},
    {"SteamAPI_ISteamHTTP_GetHTTPStreamingResponseBodyData", kParams_STEAMHTTP_INTERFACE_VERSION002_13},
    {"SteamAPI_ISteamHTTP_ReleaseHTTPRequest", kParams_STEAMHTTP_INTERFACE_VERSION002_14},
    {"SteamAPI_ISteamHTTP_GetHTTPDownloadProgressPct", kParams_STEAMHTTP_INTERFACE_VERSION002_15},
    {"SteamAPI_ISteamHTTP_SetHTTPRequestRawPostBody", kParams_STEAMHTTP_INTERFACE_VERSION002_16},
    {"SteamAPI_ISteamHTTP_CreateCookieContainer", kParams_STEAMHTTP_INTERFACE_VERSION002_17},
    {"SteamAPI_ISteamHTTP_ReleaseCookieContainer", kParams_STEAMHTTP_INTERFACE_VERSION002_18},
    {"SteamAPI_ISteamHTTP_SetCookie", kParams_STEAMHTTP_INTERFACE_VERSION002_19},
    {"SteamAPI_ISteamHTTP_SetHTTPRequestCookieContainer", kParams_STEAMHTTP_INTERFACE_VERSION002_20},
    {"SteamAPI_ISteamHTTP_SetHTTPRequestUserAgentInfo", kParams_STEAMHTTP_INTERFACE_VERSION002_21},
    {"SteamAPI_ISteamHTTP_SetHTTPRequestRequiresVerifiedCertificate", kParams_STEAMHTTP_INTERFACE_VERSION002_22},
    {"SteamAPI_ISteamHTTP_SetHTTPRequestAbsoluteTimeoutMS", kParams_STEAMHTTP_INTERFACE_VERSION002_23},
    {"SteamAPI_ISteamHTTP_GetHTTPRequestWasTimedOut", kParams_STEAMHTTP_INTERFACE_VERSION002_24},
};

class Version_STEAMHTTP_INTERFACE_VERSION002 {
public:
    virtual std::uint32_t CreateHTTPRequest(std::int32_t eHTTPRequestMethod, const char* pchAbsoluteURL) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMHTTP_INTERFACE_VERSION002[0], eHTTPRequestMethod, pchAbsoluteURL);
    }
    virtual bool SetHTTPRequestContextValue(std::uint32_t hRequest, std::uint64_t ulContextValue) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[1], hRequest, ulContextValue);
    }
    virtual bool SetHTTPRequestNetworkActivityTimeout(std::uint32_t hRequest, std::uint32_t unTimeoutSeconds) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[2], hRequest, unTimeoutSeconds);
    }
    virtual bool SetHTTPRequestHeaderValue(std::uint32_t hRequest, const char* pchHeaderName, const char* pchHeaderValue) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[3], hRequest, pchHeaderName, pchHeaderValue);
    }
    virtual bool SetHTTPRequestGetOrPostParameter(std::uint32_t hRequest, const char* pchParamName, const char* pchParamValue) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[4], hRequest, pchParamName, pchParamValue);
    }
    virtual bool SendHTTPRequest(std::uint32_t hRequest, std::uint64_t* pCallHandle) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[5], hRequest, pCallHandle);
    }
    virtual bool SendHTTPRequestAndStreamResponse(std::uint32_t hRequest, std::uint64_t* pCallHandle) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[6], hRequest, pCallHandle);
    }
    virtual bool DeferHTTPRequest(std::uint32_t hRequest) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[7], hRequest);
    }
    virtual bool PrioritizeHTTPRequest(std::uint32_t hRequest) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[8], hRequest);
    }
    virtual bool GetHTTPResponseHeaderSize(std::uint32_t hRequest, const char* pchHeaderName, std::uint32_t* unResponseHeaderSize) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[9], hRequest, pchHeaderName, unResponseHeaderSize);
    }
    virtual bool GetHTTPResponseHeaderValue(std::uint32_t hRequest, const char* pchHeaderName, std::uint8_t* pHeaderValueBuffer, std::uint32_t unBufferSize) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[10], hRequest, pchHeaderName, pHeaderValueBuffer, unBufferSize);
    }
    virtual bool GetHTTPResponseBodySize(std::uint32_t hRequest, std::uint32_t* unBodySize) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[11], hRequest, unBodySize);
    }
    virtual bool GetHTTPResponseBodyData(std::uint32_t hRequest, std::uint8_t* pBodyDataBuffer, std::uint32_t unBufferSize) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[12], hRequest, pBodyDataBuffer, unBufferSize);
    }
    virtual bool GetHTTPStreamingResponseBodyData(std::uint32_t hRequest, std::uint32_t cOffset, std::uint8_t* pBodyDataBuffer, std::uint32_t unBufferSize) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[13], hRequest, cOffset, pBodyDataBuffer, unBufferSize);
    }
    virtual bool ReleaseHTTPRequest(std::uint32_t hRequest) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[14], hRequest);
    }
    virtual bool GetHTTPDownloadProgressPct(std::uint32_t hRequest, float* pflPercentOut) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[15], hRequest, pflPercentOut);
    }
    virtual bool SetHTTPRequestRawPostBody(std::uint32_t hRequest, const char* pchContentType, std::uint8_t* pubBody, std::uint32_t unBodyLen) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[16], hRequest, pchContentType, pubBody, unBodyLen);
    }
    virtual std::uint32_t CreateCookieContainer(bool bAllowResponsesToModify) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMHTTP_INTERFACE_VERSION002[17], bAllowResponsesToModify);
    }
    virtual bool ReleaseCookieContainer(std::uint32_t hCookieContainer) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[18], hCookieContainer);
    }
    virtual bool SetCookie(std::uint32_t hCookieContainer, const char* pchHost, const char* pchUrl, const char* pchCookie) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[19], hCookieContainer, pchHost, pchUrl, pchCookie);
    }
    virtual bool SetHTTPRequestCookieContainer(std::uint32_t hRequest, std::uint32_t hCookieContainer) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[20], hRequest, hCookieContainer);
    }
    virtual bool SetHTTPRequestUserAgentInfo(std::uint32_t hRequest, const char* pchUserAgentInfo) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[21], hRequest, pchUserAgentInfo);
    }
    virtual bool SetHTTPRequestRequiresVerifiedCertificate(std::uint32_t hRequest, bool bRequireVerifiedCertificate) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[22], hRequest, bRequireVerifiedCertificate);
    }
    virtual bool SetHTTPRequestAbsoluteTimeoutMS(std::uint32_t hRequest, std::uint32_t unMilliseconds) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[23], hRequest, unMilliseconds);
    }
    virtual bool GetHTTPRequestWasTimedOut(std::uint32_t hRequest, bool* pbWasTimedOut) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION002[24], hRequest, pbWasTimedOut);
    }
};

Version_STEAMHTTP_INTERFACE_VERSION002 g_STEAMHTTP_INTERFACE_VERSION002;

const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_0[] = {
    "eHTTPRequestMethod",
    "pchAbsoluteURL",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_1[] = {
    "hRequest",
    "ulContextValue",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_2[] = {
    "hRequest",
    "unTimeoutSeconds",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_3[] = {
    "hRequest",
    "pchHeaderName",
    "pchHeaderValue",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_4[] = {
    "hRequest",
    "pchParamName",
    "pchParamValue",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_5[] = {
    "hRequest",
    "pCallHandle",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_6[] = {
    "hRequest",
    "pCallHandle",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_7[] = {
    "hRequest",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_8[] = {
    "hRequest",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_9[] = {
    "hRequest",
    "pchHeaderName",
    "unResponseHeaderSize",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_10[] = {
    "hRequest",
    "pchHeaderName",
    "pHeaderValueBuffer",
    "unBufferSize",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_11[] = {
    "hRequest",
    "unBodySize",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_12[] = {
    "hRequest",
    "pBodyDataBuffer",
    "unBufferSize",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_13[] = {
    "hRequest",
    "cOffset",
    "pBodyDataBuffer",
    "unBufferSize",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_14[] = {
    "hRequest",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_15[] = {
    "hRequest",
    "pflPercentOut",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_16[] = {
    "hRequest",
    "pchContentType",
    "pubBody",
    "unBodyLen",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_17[] = {
    "bAllowResponsesToModify",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_18[] = {
    "hCookieContainer",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_19[] = {
    "hCookieContainer",
    "pchHost",
    "pchUrl",
    "pchCookie",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_20[] = {
    "hRequest",
    "hCookieContainer",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_21[] = {
    "hRequest",
    "pchUserAgentInfo",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_22[] = {
    "hRequest",
    "bRequireVerifiedCertificate",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_23[] = {
    "hRequest",
    "unMilliseconds",
};
const char* const kParams_STEAMHTTP_INTERFACE_VERSION003_24[] = {
    "hRequest",
    "pbWasTimedOut",
};
// ISteamHTTP STEAMHTTP_INTERFACE_VERSION003
const steambridge::SlotInfo kSlots_STEAMHTTP_INTERFACE_VERSION003[] = {
    {"SteamAPI_ISteamHTTP_CreateHTTPRequest", kParams_STEAMHTTP_INTERFACE_VERSION003_0},
    {"SteamAPI_ISteamHTTP_SetHTTPRequestContextValue", kParams_STEAMHTTP_INTERFACE_VERSION003_1},
    {"SteamAPI_ISteamHTTP_SetHTTPRequestNetworkActivityTimeout", kParams_STEAMHTTP_INTERFACE_VERSION003_2},
    {"SteamAPI_ISteamHTTP_SetHTTPRequestHeaderValue", kParams_STEAMHTTP_INTERFACE_VERSION003_3},
    {"SteamAPI_ISteamHTTP_SetHTTPRequestGetOrPostParameter", kParams_STEAMHTTP_INTERFACE_VERSION003_4},
    {"SteamAPI_ISteamHTTP_SendHTTPRequest", kParams_STEAMHTTP_INTERFACE_VERSION003_5},
    {"SteamAPI_ISteamHTTP_SendHTTPRequestAndStreamResponse", kParams_STEAMHTTP_INTERFACE_VERSION003_6},
    {"SteamAPI_ISteamHTTP_DeferHTTPRequest", kParams_STEAMHTTP_INTERFACE_VERSION003_7},
    {"SteamAPI_ISteamHTTP_PrioritizeHTTPRequest", kParams_STEAMHTTP_INTERFACE_VERSION003_8},
    {"SteamAPI_ISteamHTTP_GetHTTPResponseHeaderSize", kParams_STEAMHTTP_INTERFACE_VERSION003_9},
    {"SteamAPI_ISteamHTTP_GetHTTPResponseHeaderValue", kParams_STEAMHTTP_INTERFACE_VERSION003_10},
    {"SteamAPI_ISteamHTTP_GetHTTPResponseBodySize", kParams_STEAMHTTP_INTERFACE_VERSION003_11},
    {"SteamAPI_ISteamHTTP_GetHTTPResponseBodyData", kParams_STEAMHTTP_INTERFACE_VERSION003_12},
    {"SteamAPI_ISteamHTTP_GetHTTPStreamingResponseBodyData", kParams_STEAMHTTP_INTERFACE_VERSION003_13},
    {"SteamAPI_ISteamHTTP_ReleaseHTTPRequest", kParams_STEAMHTTP_INTERFACE_VERSION003_14},
    {"SteamAPI_ISteamHTTP_GetHTTPDownloadProgressPct", kParams_STEAMHTTP_INTERFACE_VERSION003_15},
    {"SteamAPI_ISteamHTTP_SetHTTPRequestRawPostBody", kParams_STEAMHTTP_INTERFACE_VERSION003_16},
    {"SteamAPI_ISteamHTTP_CreateCookieContainer", kParams_STEAMHTTP_INTERFACE_VERSION003_17},
    {"SteamAPI_ISteamHTTP_ReleaseCookieContainer", kParams_STEAMHTTP_INTERFACE_VERSION003_18},
    {"SteamAPI_ISteamHTTP_SetCookie", kParams_STEAMHTTP_INTERFACE_VERSION003_19},
    {"SteamAPI_ISteamHTTP_SetHTTPRequestCookieContainer", kParams_STEAMHTTP_INTERFACE_VERSION003_20},
    {"SteamAPI_ISteamHTTP_SetHTTPRequestUserAgentInfo", kParams_STEAMHTTP_INTERFACE_VERSION003_21},
    {"SteamAPI_ISteamHTTP_SetHTTPRequestRequiresVerifiedCertificate", kParams_STEAMHTTP_INTERFACE_VERSION003_22},
    {"SteamAPI_ISteamHTTP_SetHTTPRequestAbsoluteTimeoutMS", kParams_STEAMHTTP_INTERFACE_VERSION003_23},
    {"SteamAPI_ISteamHTTP_GetHTTPRequestWasTimedOut", kParams_STEAMHTTP_INTERFACE_VERSION003_24},
};

class Version_STEAMHTTP_INTERFACE_VERSION003 {
public:
    virtual std::uint32_t CreateHTTPRequest(std::int32_t eHTTPRequestMethod, const char* pchAbsoluteURL) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMHTTP_INTERFACE_VERSION003[0], eHTTPRequestMethod, pchAbsoluteURL);
    }
    virtual bool SetHTTPRequestContextValue(std::uint32_t hRequest, std::uint64_t ulContextValue) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[1], hRequest, ulContextValue);
    }
    virtual bool SetHTTPRequestNetworkActivityTimeout(std::uint32_t hRequest, std::uint32_t unTimeoutSeconds) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[2], hRequest, unTimeoutSeconds);
    }
    virtual bool SetHTTPRequestHeaderValue(std::uint32_t hRequest, const char* pchHeaderName, const char* pchHeaderValue) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[3], hRequest, pchHeaderName, pchHeaderValue);
    }
    virtual bool SetHTTPRequestGetOrPostParameter(std::uint32_t hRequest, const char* pchParamName, const char* pchParamValue) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[4], hRequest, pchParamName, pchParamValue);
    }
    virtual bool SendHTTPRequest(std::uint32_t hRequest, std::uint64_t* pCallHandle) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[5], hRequest, pCallHandle);
    }
    virtual bool SendHTTPRequestAndStreamResponse(std::uint32_t hRequest, std::uint64_t* pCallHandle) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[6], hRequest, pCallHandle);
    }
    virtual bool DeferHTTPRequest(std::uint32_t hRequest) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[7], hRequest);
    }
    virtual bool PrioritizeHTTPRequest(std::uint32_t hRequest) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[8], hRequest);
    }
    virtual bool GetHTTPResponseHeaderSize(std::uint32_t hRequest, const char* pchHeaderName, std::uint32_t* unResponseHeaderSize) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[9], hRequest, pchHeaderName, unResponseHeaderSize);
    }
    virtual bool GetHTTPResponseHeaderValue(std::uint32_t hRequest, const char* pchHeaderName, std::uint8_t* pHeaderValueBuffer, std::uint32_t unBufferSize) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[10], hRequest, pchHeaderName, pHeaderValueBuffer, unBufferSize);
    }
    virtual bool GetHTTPResponseBodySize(std::uint32_t hRequest, std::uint32_t* unBodySize) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[11], hRequest, unBodySize);
    }
    virtual bool GetHTTPResponseBodyData(std::uint32_t hRequest, std::uint8_t* pBodyDataBuffer, std::uint32_t unBufferSize) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[12], hRequest, pBodyDataBuffer, unBufferSize);
    }
    virtual bool GetHTTPStreamingResponseBodyData(std::uint32_t hRequest, std::uint32_t cOffset, std::uint8_t* pBodyDataBuffer, std::uint32_t unBufferSize) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[13], hRequest, cOffset, pBodyDataBuffer, unBufferSize);
    }
    virtual bool ReleaseHTTPRequest(std::uint32_t hRequest) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[14], hRequest);
    }
    virtual bool GetHTTPDownloadProgressPct(std::uint32_t hRequest, float* pflPercentOut) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[15], hRequest, pflPercentOut);
    }
    virtual bool SetHTTPRequestRawPostBody(std::uint32_t hRequest, const char* pchContentType, std::uint8_t* pubBody, std::uint32_t unBodyLen) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[16], hRequest, pchContentType, pubBody, unBodyLen);
    }
    virtual std::uint32_t CreateCookieContainer(bool bAllowResponsesToModify) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMHTTP_INTERFACE_VERSION003[17], bAllowResponsesToModify);
    }
    virtual bool ReleaseCookieContainer(std::uint32_t hCookieContainer) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[18], hCookieContainer);
    }
    virtual bool SetCookie(std::uint32_t hCookieContainer, const char* pchHost, const char* pchUrl, const char* pchCookie) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[19], hCookieContainer, pchHost, pchUrl, pchCookie);
    }
    virtual bool SetHTTPRequestCookieContainer(std::uint32_t hRequest, std::uint32_t hCookieContainer) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[20], hRequest, hCookieContainer);
    }
    virtual bool SetHTTPRequestUserAgentInfo(std::uint32_t hRequest, const char* pchUserAgentInfo) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[21], hRequest, pchUserAgentInfo);
    }
    virtual bool SetHTTPRequestRequiresVerifiedCertificate(std::uint32_t hRequest, bool bRequireVerifiedCertificate) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[22], hRequest, bRequireVerifiedCertificate);
    }
    virtual bool SetHTTPRequestAbsoluteTimeoutMS(std::uint32_t hRequest, std::uint32_t unMilliseconds) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[23], hRequest, unMilliseconds);
    }
    virtual bool GetHTTPRequestWasTimedOut(std::uint32_t hRequest, bool* pbWasTimedOut) {
        return steambridge::slot<bool>(kSlots_STEAMHTTP_INTERFACE_VERSION003[24], hRequest, pbWasTimedOut);
    }
};

Version_STEAMHTTP_INTERFACE_VERSION003 g_STEAMHTTP_INTERFACE_VERSION003;

const char* const kParams_SteamInput001_3[] = {
    "handlesOut",
};
const char* const kParams_SteamInput001_4[] = {
    "pszActionSetName",
};
const char* const kParams_SteamInput001_5[] = {
    "inputHandle",
    "actionSetHandle",
};
const char* const kParams_SteamInput001_6[] = {
    "inputHandle",
};
const char* const kParams_SteamInput001_7[] = {
    "inputHandle",
    "actionSetLayerHandle",
};
const char* const kParams_SteamInput001_8[] = {
    "inputHandle",
    "actionSetLayerHandle",
};
const char* const kParams_SteamInput001_9[] = {
    "inputHandle",
};
const char* const kParams_SteamInput001_10[] = {
    "inputHandle",
    "handlesOut",
};
const char* const kParams_SteamInput001_11[] = {
    "pszActionName",
};
const char* const kParams_SteamInput001_12[] = {
    "inputHandle",
    "digitalActionHandle",
};
const char* const kParams_SteamInput001_13[] = {
    "inputHandle",
    "actionSetHandle",
    "digitalActionHandle",
    "originsOut",
};
const char* const kParams_SteamInput001_14[] = {
    "pszActionName",
};
const char* const kParams_SteamInput001_15[] = {
    "inputHandle",
    "analogActionHandle",
};
const char* const kParams_SteamInput001_16[] = {
    "inputHandle",
    "actionSetHandle",
    "analogActionHandle",
    "originsOut",
};
const char* const kParams_SteamInput001_17[] = {
    "eOrigin",
};
const char* const kParams_SteamInput001_18[] = {
    "eOrigin",
};
const char* const kParams_SteamInput001_19[] = {
    "inputHandle",
    "eAction",
};
const char* const kParams_SteamInput001_20[] = {
    "inputHandle",
};
const char* const kParams_SteamInput001_21[] = {
    "inputHandle",
    "usLeftSpeed",
    "usRightSpeed",
};
const char* const kParams_SteamInput001_22[] = {
    "inputHandle",
    "nColorR",
    "nColorG",
    "nColorB",
    "nFlags",
};
const char* const kParams_SteamInput001_23[] = {
    "inputHandle",
    "eTargetPad",
    "usDurationMicroSec",
};
const char* const kParams_SteamInput001_24[] = {
    "inputHandle",
    "eTargetPad",
    "usDurationMicroSec",
    "usOffMicroSec",
    "unRepeat",
    "nFlags",
};
const char* const kParams_SteamInput001_25[] = {
    "inputHandle",
};
const char* const kParams_SteamInput001_26[] = {
    "inputHandle",
};
const char* const kParams_SteamInput001_27[] = {
    "nIndex",
};
const char* const kParams_SteamInput001_28[] = {
    "ulinputHandle",
};
const char* const kParams_SteamInput001_29[] = {
    "eOrigin",
};
const char* const kParams_SteamInput001_30[] = {
    "eOrigin",
};
const char* const kParams_SteamInput001_31[] = {
    "inputHandle",
    "eOrigin",
};
const char* const kParams_SteamInput001_32[] = {
    "eDestinationInputType",
    "eSourceOrigin",
};
const char* const kParams_SteamInput001_33[] = {
    "inputHandle",
    "pMajor",
    "pMinor",
};
const char* const kParams_SteamInput001_34[] = {
    "inputHandle",
};
// ISteamInput SteamInput001
const steambridge::SlotInfo kSlots_SteamInput001[] = {
    {"SteamAPI_ISteamInput_Init", nullptr},
    {"SteamAPI_ISteamInput_Shutdown", nullptr},
    {"SteamAPI_ISteamInput_RunFrame", nullptr},
    {"SteamAPI_ISteamInput_GetConnectedControllers", kParams_SteamInput001_3},
    {"SteamAPI_ISteamInput_GetActionSetHandle", kParams_SteamInput001_4},
    {"SteamAPI_ISteamInput_ActivateActionSet", kParams_SteamInput001_5},
    {"SteamAPI_ISteamInput_GetCurrentActionSet", kParams_SteamInput001_6},
    {"SteamAPI_ISteamInput_ActivateActionSetLayer", kParams_SteamInput001_7},
    {"SteamAPI_ISteamInput_DeactivateActionSetLayer", kParams_SteamInput001_8},
    {"SteamAPI_ISteamInput_DeactivateAllActionSetLayers", kParams_SteamInput001_9},
    {"SteamAPI_ISteamInput_GetActiveActionSetLayers", kParams_SteamInput001_10},
    {"SteamAPI_ISteamInput_GetDigitalActionHandle", kParams_SteamInput001_11},
    {"SteamAPI_ISteamInput_GetDigitalActionData", kParams_SteamInput001_12},
    {"SteamAPI_ISteamInput_GetDigitalActionOrigins", kParams_SteamInput001_13},
    {"SteamAPI_ISteamInput_GetAnalogActionHandle", kParams_SteamInput001_14},
    {"SteamAPI_ISteamInput_GetAnalogActionData", kParams_SteamInput001_15},
    {"SteamAPI_ISteamInput_GetAnalogActionOrigins", kParams_SteamInput001_16},
    {"SteamAPI_ISteamInput_GetGlyphForActionOrigin", kParams_SteamInput001_17},
    {"SteamAPI_ISteamInput_GetStringForActionOrigin", kParams_SteamInput001_18},
    {"SteamAPI_ISteamInput_StopAnalogActionMomentum", kParams_SteamInput001_19},
    {"SteamAPI_ISteamInput_GetMotionData", kParams_SteamInput001_20},
    {"SteamAPI_ISteamInput_TriggerVibration", kParams_SteamInput001_21},
    {"SteamAPI_ISteamInput_SetLEDColor", kParams_SteamInput001_22},
    {"SteamAPI_ISteamInput_TriggerHapticPulse", kParams_SteamInput001_23},
    {"SteamAPI_ISteamInput_TriggerRepeatedHapticPulse", kParams_SteamInput001_24},
    {"SteamAPI_ISteamInput_ShowBindingPanel", kParams_SteamInput001_25},
    {"SteamAPI_ISteamInput_GetInputTypeForHandle", kParams_SteamInput001_26},
    {"SteamAPI_ISteamInput_GetControllerForGamepadIndex", kParams_SteamInput001_27},
    {"SteamAPI_ISteamInput_GetGamepadIndexForController", kParams_SteamInput001_28},
    {"SteamAPI_ISteamInput_GetStringForXboxOrigin", kParams_SteamInput001_29},
    {"SteamAPI_ISteamInput_GetGlyphForXboxOrigin", kParams_SteamInput001_30},
    {"SteamAPI_ISteamInput_GetActionOriginFromXboxOrigin", kParams_SteamInput001_31},
    {"SteamAPI_ISteamInput_TranslateActionOrigin", kParams_SteamInput001_32},
    {"SteamAPI_ISteamInput_GetDeviceBindingRevision", kParams_SteamInput001_33},
    {"SteamAPI_ISteamInput_GetRemotePlaySessionID", kParams_SteamInput001_34},
};

class Version_SteamInput001 {
public:
    virtual bool Init() {
        return steambridge::slot<bool>(kSlots_SteamInput001[0]);
    }
    virtual bool Shutdown() {
        return steambridge::slot<bool>(kSlots_SteamInput001[1]);
    }
    virtual void RunFrame() {
        steambridge::slot<void>(kSlots_SteamInput001[2]);
    }
    virtual std::int32_t GetConnectedControllers(std::uint64_t* handlesOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput001[3], handlesOut);
    }
    virtual std::uint64_t GetActionSetHandle(const char* pszActionSetName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamInput001[4], pszActionSetName);
    }
    virtual void ActivateActionSet(std::uint64_t inputHandle, std::uint64_t actionSetHandle) {
        steambridge::slot<void>(kSlots_SteamInput001[5], inputHandle, actionSetHandle);
    }
    virtual std::uint64_t GetCurrentActionSet(std::uint64_t inputHandle) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamInput001[6], inputHandle);
    }
    virtual void ActivateActionSetLayer(std::uint64_t inputHandle, std::uint64_t actionSetLayerHandle) {
        steambridge::slot<void>(kSlots_SteamInput001[7], inputHandle, actionSetLayerHandle);
    }
    virtual void DeactivateActionSetLayer(std::uint64_t inputHandle, std::uint64_t actionSetLayerHandle) {
        steambridge::slot<void>(kSlots_SteamInput001[8], inputHandle, actionSetLayerHandle);
    }
    virtual void DeactivateAllActionSetLayers(std::uint64_t inputHandle) {
        steambridge::slot<void>(kSlots_SteamInput001[9], inputHandle);
    }
    virtual std::int32_t GetActiveActionSetLayers(std::uint64_t inputHandle, std::uint64_t* handlesOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput001[10], inputHandle, handlesOut);
    }
    virtual std::uint64_t GetDigitalActionHandle(const char* pszActionName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamInput001[11], pszActionName);
    }
    virtual InputDigitalActionData_t GetDigitalActionData(std::uint64_t inputHandle, std::uint64_t digitalActionHandle) {
        return steambridge::slot<InputDigitalActionData_t>(kSlots_SteamInput001[12], inputHandle, digitalActionHandle);
    }
    virtual std::int32_t GetDigitalActionOrigins(std::uint64_t inputHandle, std::uint64_t actionSetHandle, std::uint64_t digitalActionHandle, std::int32_t* originsOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput001[13], inputHandle, actionSetHandle, digitalActionHandle, originsOut);
    }
    virtual std::uint64_t GetAnalogActionHandle(const char* pszActionName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamInput001[14], pszActionName);
    }
    virtual InputAnalogActionData_t GetAnalogActionData(std::uint64_t inputHandle, std::uint64_t analogActionHandle) {
        return steambridge::slot<InputAnalogActionData_t>(kSlots_SteamInput001[15], inputHandle, analogActionHandle);
    }
    virtual std::int32_t GetAnalogActionOrigins(std::uint64_t inputHandle, std::uint64_t actionSetHandle, std::uint64_t analogActionHandle, std::int32_t* originsOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput001[16], inputHandle, actionSetHandle, analogActionHandle, originsOut);
    }
    virtual const char* GetGlyphForActionOrigin(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamInput001[17], eOrigin);
    }
    virtual const char* GetStringForActionOrigin(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamInput001[18], eOrigin);
    }
    virtual void StopAnalogActionMomentum(std::uint64_t inputHandle, std::uint64_t eAction) {
        steambridge::slot<void>(kSlots_SteamInput001[19], inputHandle, eAction);
    }
    virtual InputMotionData_t GetMotionData(std::uint64_t inputHandle) {
        return steambridge::slot<InputMotionData_t>(kSlots_SteamInput001[20], inputHandle);
    }
    virtual void TriggerVibration(std::uint64_t inputHandle, std::uint16_t usLeftSpeed, std::uint16_t usRightSpeed) {
        steambridge::slot<void>(kSlots_SteamInput001[21], inputHandle, usLeftSpeed, usRightSpeed);
    }
    virtual void SetLEDColor(std::uint64_t inputHandle, std::uint8_t nColorR, std::uint8_t nColorG, std::uint8_t nColorB, std::uint32_t nFlags) {
        steambridge::slot<void>(kSlots_SteamInput001[22], inputHandle, nColorR, nColorG, nColorB, nFlags);
    }
    virtual void TriggerHapticPulse(std::uint64_t inputHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec) {
        steambridge::slot<void>(kSlots_SteamInput001[23], inputHandle, eTargetPad, usDurationMicroSec);
    }
    virtual void TriggerRepeatedHapticPulse(std::uint64_t inputHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec, std::uint16_t usOffMicroSec, std::uint16_t unRepeat, std::uint32_t nFlags) {
        steambridge::slot<void>(kSlots_SteamInput001[24], inputHandle, eTargetPad, usDurationMicroSec, usOffMicroSec, unRepeat, nFlags);
    }
    virtual bool ShowBindingPanel(std::uint64_t inputHandle) {
        return steambridge::slot<bool>(kSlots_SteamInput001[25], inputHandle);
    }
    virtual std::int32_t GetInputTypeForHandle(std::uint64_t inputHandle) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput001[26], inputHandle);
    }
    virtual std::uint64_t GetControllerForGamepadIndex(std::int32_t nIndex) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamInput001[27], nIndex);
    }
    virtual std::int32_t GetGamepadIndexForController(std::uint64_t ulinputHandle) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput001[28], ulinputHandle);
    }
    virtual const char* GetStringForXboxOrigin(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamInput001[29], eOrigin);
    }
    virtual const char* GetGlyphForXboxOrigin(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamInput001[30], eOrigin);
    }
    virtual std::int32_t GetActionOriginFromXboxOrigin(std::uint64_t inputHandle, std::int32_t eOrigin) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput001[31], inputHandle, eOrigin);
    }
    virtual std::int32_t TranslateActionOrigin(std::int32_t eDestinationInputType, std::int32_t eSourceOrigin) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput001[32], eDestinationInputType, eSourceOrigin);
    }
    virtual bool GetDeviceBindingRevision(std::uint64_t inputHandle, std::int32_t* pMajor, std::int32_t* pMinor) {
        return steambridge::slot<bool>(kSlots_SteamInput001[33], inputHandle, pMajor, pMinor);
    }
    virtual std::uint32_t GetRemotePlaySessionID(std::uint64_t inputHandle) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamInput001[34], inputHandle);
    }
};

Version_SteamInput001 g_SteamInput001;

const char* const kParams_SteamInput002_3[] = {
    "handlesOut",
};
const char* const kParams_SteamInput002_4[] = {
    "pszActionSetName",
};
const char* const kParams_SteamInput002_5[] = {
    "inputHandle",
    "actionSetHandle",
};
const char* const kParams_SteamInput002_6[] = {
    "inputHandle",
};
const char* const kParams_SteamInput002_7[] = {
    "inputHandle",
    "actionSetLayerHandle",
};
const char* const kParams_SteamInput002_8[] = {
    "inputHandle",
    "actionSetLayerHandle",
};
const char* const kParams_SteamInput002_9[] = {
    "inputHandle",
};
const char* const kParams_SteamInput002_10[] = {
    "inputHandle",
    "handlesOut",
};
const char* const kParams_SteamInput002_11[] = {
    "pszActionName",
};
const char* const kParams_SteamInput002_12[] = {
    "inputHandle",
    "digitalActionHandle",
};
const char* const kParams_SteamInput002_13[] = {
    "inputHandle",
    "actionSetHandle",
    "digitalActionHandle",
    "originsOut",
};
const char* const kParams_SteamInput002_14[] = {
    "pszActionName",
};
const char* const kParams_SteamInput002_15[] = {
    "inputHandle",
    "analogActionHandle",
};
const char* const kParams_SteamInput002_16[] = {
    "inputHandle",
    "actionSetHandle",
    "analogActionHandle",
    "originsOut",
};
const char* const kParams_SteamInput002_17[] = {
    "eOrigin",
};
const char* const kParams_SteamInput002_18[] = {
    "eOrigin",
};
const char* const kParams_SteamInput002_19[] = {
    "inputHandle",
    "eAction",
};
const char* const kParams_SteamInput002_20[] = {
    "inputHandle",
};
const char* const kParams_SteamInput002_21[] = {
    "inputHandle",
    "usLeftSpeed",
    "usRightSpeed",
};
const char* const kParams_SteamInput002_22[] = {
    "inputHandle",
    "nColorR",
    "nColorG",
    "nColorB",
    "nFlags",
};
const char* const kParams_SteamInput002_23[] = {
    "inputHandle",
    "eTargetPad",
    "usDurationMicroSec",
};
const char* const kParams_SteamInput002_24[] = {
    "inputHandle",
    "eTargetPad",
    "usDurationMicroSec",
    "usOffMicroSec",
    "unRepeat",
    "nFlags",
};
const char* const kParams_SteamInput002_25[] = {
    "inputHandle",
};
const char* const kParams_SteamInput002_26[] = {
    "inputHandle",
};
const char* const kParams_SteamInput002_27[] = {
    "nIndex",
};
const char* const kParams_SteamInput002_28[] = {
    "ulinputHandle",
};
const char* const kParams_SteamInput002_29[] = {
    "eOrigin",
};
const char* const kParams_SteamInput002_30[] = {
    "eOrigin",
};
const char* const kParams_SteamInput002_31[] = {
    "inputHandle",
    "eOrigin",
};
const char* const kParams_SteamInput002_32[] = {
    "eDestinationInputType",
    "eSourceOrigin",
};
const char* const kParams_SteamInput002_33[] = {
    "inputHandle",
    "pMajor",
    "pMinor",
};
const char* const kParams_SteamInput002_34[] = {
    "inputHandle",
};
// ISteamInput SteamInput002
const steambridge::SlotInfo kSlots_SteamInput002[] = {
    {"SteamAPI_ISteamInput_Init", nullptr},
    {"SteamAPI_ISteamInput_Shutdown", nullptr},
    {"SteamAPI_ISteamInput_RunFrame", nullptr},
    {"SteamAPI_ISteamInput_GetConnectedControllers", kParams_SteamInput002_3},
    {"SteamAPI_ISteamInput_GetActionSetHandle", kParams_SteamInput002_4},
    {"SteamAPI_ISteamInput_ActivateActionSet", kParams_SteamInput002_5},
    {"SteamAPI_ISteamInput_GetCurrentActionSet", kParams_SteamInput002_6},
    {"SteamAPI_ISteamInput_ActivateActionSetLayer", kParams_SteamInput002_7},
    {"SteamAPI_ISteamInput_DeactivateActionSetLayer", kParams_SteamInput002_8},
    {"SteamAPI_ISteamInput_DeactivateAllActionSetLayers", kParams_SteamInput002_9},
    {"SteamAPI_ISteamInput_GetActiveActionSetLayers", kParams_SteamInput002_10},
    {"SteamAPI_ISteamInput_GetDigitalActionHandle", kParams_SteamInput002_11},
    {"SteamAPI_ISteamInput_GetDigitalActionData", kParams_SteamInput002_12},
    {"SteamAPI_ISteamInput_GetDigitalActionOrigins", kParams_SteamInput002_13},
    {"SteamAPI_ISteamInput_GetAnalogActionHandle", kParams_SteamInput002_14},
    {"SteamAPI_ISteamInput_GetAnalogActionData", kParams_SteamInput002_15},
    {"SteamAPI_ISteamInput_GetAnalogActionOrigins", kParams_SteamInput002_16},
    {"SteamAPI_ISteamInput_GetGlyphForActionOrigin", kParams_SteamInput002_17},
    {"SteamAPI_ISteamInput_GetStringForActionOrigin", kParams_SteamInput002_18},
    {"SteamAPI_ISteamInput_StopAnalogActionMomentum", kParams_SteamInput002_19},
    {"SteamAPI_ISteamInput_GetMotionData", kParams_SteamInput002_20},
    {"SteamAPI_ISteamInput_TriggerVibration", kParams_SteamInput002_21},
    {"SteamAPI_ISteamInput_SetLEDColor", kParams_SteamInput002_22},
    {"SteamAPI_ISteamInput_TriggerHapticPulse", kParams_SteamInput002_23},
    {"SteamAPI_ISteamInput_TriggerRepeatedHapticPulse", kParams_SteamInput002_24},
    {"SteamAPI_ISteamInput_ShowBindingPanel", kParams_SteamInput002_25},
    {"SteamAPI_ISteamInput_GetInputTypeForHandle", kParams_SteamInput002_26},
    {"SteamAPI_ISteamInput_GetControllerForGamepadIndex", kParams_SteamInput002_27},
    {"SteamAPI_ISteamInput_GetGamepadIndexForController", kParams_SteamInput002_28},
    {"SteamAPI_ISteamInput_GetStringForXboxOrigin", kParams_SteamInput002_29},
    {"SteamAPI_ISteamInput_GetGlyphForXboxOrigin", kParams_SteamInput002_30},
    {"SteamAPI_ISteamInput_GetActionOriginFromXboxOrigin", kParams_SteamInput002_31},
    {"SteamAPI_ISteamInput_TranslateActionOrigin", kParams_SteamInput002_32},
    {"SteamAPI_ISteamInput_GetDeviceBindingRevision", kParams_SteamInput002_33},
    {"SteamAPI_ISteamInput_GetRemotePlaySessionID", kParams_SteamInput002_34},
};

class Version_SteamInput002 {
public:
    virtual bool Init() {
        return steambridge::slot<bool>(kSlots_SteamInput002[0]);
    }
    virtual bool Shutdown() {
        return steambridge::slot<bool>(kSlots_SteamInput002[1]);
    }
    virtual void RunFrame() {
        steambridge::slot<void>(kSlots_SteamInput002[2]);
    }
    virtual std::int32_t GetConnectedControllers(std::uint64_t* handlesOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput002[3], handlesOut);
    }
    virtual std::uint64_t GetActionSetHandle(const char* pszActionSetName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamInput002[4], pszActionSetName);
    }
    virtual void ActivateActionSet(std::uint64_t inputHandle, std::uint64_t actionSetHandle) {
        steambridge::slot<void>(kSlots_SteamInput002[5], inputHandle, actionSetHandle);
    }
    virtual std::uint64_t GetCurrentActionSet(std::uint64_t inputHandle) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamInput002[6], inputHandle);
    }
    virtual void ActivateActionSetLayer(std::uint64_t inputHandle, std::uint64_t actionSetLayerHandle) {
        steambridge::slot<void>(kSlots_SteamInput002[7], inputHandle, actionSetLayerHandle);
    }
    virtual void DeactivateActionSetLayer(std::uint64_t inputHandle, std::uint64_t actionSetLayerHandle) {
        steambridge::slot<void>(kSlots_SteamInput002[8], inputHandle, actionSetLayerHandle);
    }
    virtual void DeactivateAllActionSetLayers(std::uint64_t inputHandle) {
        steambridge::slot<void>(kSlots_SteamInput002[9], inputHandle);
    }
    virtual std::int32_t GetActiveActionSetLayers(std::uint64_t inputHandle, std::uint64_t* handlesOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput002[10], inputHandle, handlesOut);
    }
    virtual std::uint64_t GetDigitalActionHandle(const char* pszActionName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamInput002[11], pszActionName);
    }
    virtual InputDigitalActionData_t GetDigitalActionData(std::uint64_t inputHandle, std::uint64_t digitalActionHandle) {
        return steambridge::slot<InputDigitalActionData_t>(kSlots_SteamInput002[12], inputHandle, digitalActionHandle);
    }
    virtual std::int32_t GetDigitalActionOrigins(std::uint64_t inputHandle, std::uint64_t actionSetHandle, std::uint64_t digitalActionHandle, std::int32_t* originsOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput002[13], inputHandle, actionSetHandle, digitalActionHandle, originsOut);
    }
    virtual std::uint64_t GetAnalogActionHandle(const char* pszActionName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamInput002[14], pszActionName);
    }
    virtual InputAnalogActionData_t GetAnalogActionData(std::uint64_t inputHandle, std::uint64_t analogActionHandle) {
        return steambridge::slot<InputAnalogActionData_t>(kSlots_SteamInput002[15], inputHandle, analogActionHandle);
    }
    virtual std::int32_t GetAnalogActionOrigins(std::uint64_t inputHandle, std::uint64_t actionSetHandle, std::uint64_t analogActionHandle, std::int32_t* originsOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput002[16], inputHandle, actionSetHandle, analogActionHandle, originsOut);
    }
    virtual const char* GetGlyphForActionOrigin(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamInput002[17], eOrigin);
    }
    virtual const char* GetStringForActionOrigin(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamInput002[18], eOrigin);
    }
    virtual void StopAnalogActionMomentum(std::uint64_t inputHandle, std::uint64_t eAction) {
        steambridge::slot<void>(kSlots_SteamInput002[19], inputHandle, eAction);
    }
    virtual InputMotionData_t GetMotionData(std::uint64_t inputHandle) {
        return steambridge::slot<InputMotionData_t>(kSlots_SteamInput002[20], inputHandle);
    }
    virtual void TriggerVibration(std::uint64_t inputHandle, std::uint16_t usLeftSpeed, std::uint16_t usRightSpeed) {
        steambridge::slot<void>(kSlots_SteamInput002[21], inputHandle, usLeftSpeed, usRightSpeed);
    }
    virtual void SetLEDColor(std::uint64_t inputHandle, std::uint8_t nColorR, std::uint8_t nColorG, std::uint8_t nColorB, std::uint32_t nFlags) {
        steambridge::slot<void>(kSlots_SteamInput002[22], inputHandle, nColorR, nColorG, nColorB, nFlags);
    }
    virtual void TriggerHapticPulse(std::uint64_t inputHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec) {
        steambridge::slot<void>(kSlots_SteamInput002[23], inputHandle, eTargetPad, usDurationMicroSec);
    }
    virtual void TriggerRepeatedHapticPulse(std::uint64_t inputHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec, std::uint16_t usOffMicroSec, std::uint16_t unRepeat, std::uint32_t nFlags) {
        steambridge::slot<void>(kSlots_SteamInput002[24], inputHandle, eTargetPad, usDurationMicroSec, usOffMicroSec, unRepeat, nFlags);
    }
    virtual bool ShowBindingPanel(std::uint64_t inputHandle) {
        return steambridge::slot<bool>(kSlots_SteamInput002[25], inputHandle);
    }
    virtual std::int32_t GetInputTypeForHandle(std::uint64_t inputHandle) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput002[26], inputHandle);
    }
    virtual std::uint64_t GetControllerForGamepadIndex(std::int32_t nIndex) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamInput002[27], nIndex);
    }
    virtual std::int32_t GetGamepadIndexForController(std::uint64_t ulinputHandle) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput002[28], ulinputHandle);
    }
    virtual const char* GetStringForXboxOrigin(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamInput002[29], eOrigin);
    }
    virtual const char* GetGlyphForXboxOrigin(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamInput002[30], eOrigin);
    }
    virtual std::int32_t GetActionOriginFromXboxOrigin(std::uint64_t inputHandle, std::int32_t eOrigin) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput002[31], inputHandle, eOrigin);
    }
    virtual std::int32_t TranslateActionOrigin(std::int32_t eDestinationInputType, std::int32_t eSourceOrigin) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput002[32], eDestinationInputType, eSourceOrigin);
    }
    virtual bool GetDeviceBindingRevision(std::uint64_t inputHandle, std::int32_t* pMajor, std::int32_t* pMinor) {
        return steambridge::slot<bool>(kSlots_SteamInput002[33], inputHandle, pMajor, pMinor);
    }
    virtual std::uint32_t GetRemotePlaySessionID(std::uint64_t inputHandle) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamInput002[34], inputHandle);
    }
};

Version_SteamInput002 g_SteamInput002;

const char* const kParams_SteamInput006_0[] = {
    "bExplicitlyCallRunFrame",
};
const char* const kParams_SteamInput006_2[] = {
    "pchInputActionManifestAbsolutePath",
};
const char* const kParams_SteamInput006_3[] = {
    "bReservedValue",
};
const char* const kParams_SteamInput006_4[] = {
    "bWaitForever",
    "unTimeout",
};
const char* const kParams_SteamInput006_6[] = {
    "handlesOut",
};
const char* const kParams_SteamInput006_8[] = {
    "pCallback",
};
const char* const kParams_SteamInput006_9[] = {
    "pszActionSetName",
};
const char* const kParams_SteamInput006_10[] = {
    "inputHandle",
    "actionSetHandle",
};
const char* const kParams_SteamInput006_11[] = {
    "inputHandle",
};
const char* const kParams_SteamInput006_12[] = {
    "inputHandle",
    "actionSetLayerHandle",
};
const char* const kParams_SteamInput006_13[] = {
    "inputHandle",
    "actionSetLayerHandle",
};
const char* const kParams_SteamInput006_14[] = {
    "inputHandle",
};
const char* const kParams_SteamInput006_15[] = {
    "inputHandle",
    "handlesOut",
};
const char* const kParams_SteamInput006_16[] = {
    "pszActionName",
};
const char* const kParams_SteamInput006_17[] = {
    "inputHandle",
    "digitalActionHandle",
};
const char* const kParams_SteamInput006_18[] = {
    "inputHandle",
    "actionSetHandle",
    "digitalActionHandle",
    "originsOut",
};
const char* const kParams_SteamInput006_19[] = {
    "eActionHandle",
};
const char* const kParams_SteamInput006_20[] = {
    "pszActionName",
};
const char* const kParams_SteamInput006_21[] = {
    "inputHandle",
    "analogActionHandle",
};
const char* const kParams_SteamInput006_22[] = {
    "inputHandle",
    "actionSetHandle",
    "analogActionHandle",
    "originsOut",
};
const char* const kParams_SteamInput006_23[] = {
    "eOrigin",
    "eSize",
    "unFlags",
};
const char* const kParams_SteamInput006_24[] = {
    "eOrigin",
    "unFlags",
};
const char* const kParams_SteamInput006_25[] = {
    "eOrigin",
};
const char* const kParams_SteamInput006_26[] = {
    "eOrigin",
};
const char* const kParams_SteamInput006_27[] = {
    "eActionHandle",
};
const char* const kParams_SteamInput006_28[] = {
    "inputHandle",
    "eAction",
};
const char* const kParams_SteamInput006_29[] = {
    "inputHandle",
};
const char* const kParams_SteamInput006_30[] = {
    "inputHandle",
    "usLeftSpeed",
    "usRightSpeed",
};
const char* const kParams_SteamInput006_31[] = {
    "inputHandle",
    "usLeftSpeed",
    "usRightSpeed",
    "usLeftTriggerSpeed",
    "usRightTriggerSpeed",
};
const char* const kParams_SteamInput006_32[] = {
    "inputHandle",
    "eHapticLocation",
    "nIntensity",
    "nGainDB",
    "nOtherIntensity",
    "nOtherGainDB",
};
const char* const kParams_SteamInput006_33[] = {
    "inputHandle",
    "nColorR",
    "nColorG",
    "nColorB",
    "nFlags",
};
const char* const kParams_SteamInput006_34[] = {
    "inputHandle",
    "eTargetPad",
    "usDurationMicroSec",
};
const char* const kParams_SteamInput006_35[] = {
    "inputHandle",
    "eTargetPad",
    "usDurationMicroSec",
    "usOffMicroSec",
    "unRepeat",
    "nFlags",
};
const char* const kParams_SteamInput006_36[] = {
    "inputHandle",
};
const char* const kParams_SteamInput006_37[] = {
    "inputHandle",
};
const char* const kParams_SteamInput006_38[] = {
    "nIndex",
};
const char* const kParams_SteamInput006_39[] = {
    "ulinputHandle",
};
const char* const kParams_SteamInput006_40[] = {
    "eOrigin",
};
const char* const kParams_SteamInput006_41[] = {
    "eOrigin",
};
const char* const kParams_SteamInput006_42[] = {
    "inputHandle",
    "eOrigin",
};
const char* const kParams_SteamInput006_43[] = {
    "eDestinationInputType",
    "eSourceOrigin",
};
const char* const kParams_SteamInput006_44[] = {
    "inputHandle",
    "pMajor",
    "pMinor",
};
const char* const kParams_SteamInput006_45[] = {
    "inputHandle",
};
const char* const kParams_SteamInput006_47[] = {
    "inputHandle",
    "pParam",
};
// ISteamInput SteamInput006
const steambridge::SlotInfo kSlots_SteamInput006[] = {
    {"SteamAPI_ISteamInput_Init", kParams_SteamInput006_0},
    {"SteamAPI_ISteamInput_Shutdown", nullptr},
    {"SteamAPI_ISteamInput_SetInputActionManifestFilePath", kParams_SteamInput006_2},
    {"SteamAPI_ISteamInput_RunFrame", kParams_SteamInput006_3},
    {"SteamAPI_ISteamInput_BWaitForData", kParams_SteamInput006_4},
    {"SteamAPI_ISteamInput_BNewDataAvailable", nullptr},
    {"SteamAPI_ISteamInput_GetConnectedControllers", kParams_SteamInput006_6},
    {"SteamAPI_ISteamInput_EnableDeviceCallbacks", nullptr},
    {"SteamAPI_ISteamInput_EnableActionEventCallbacks", kParams_SteamInput006_8},
    {"SteamAPI_ISteamInput_GetActionSetHandle", kParams_SteamInput006_9},
    {"SteamAPI_ISteamInput_ActivateActionSet", kParams_SteamInput006_10},
    {"SteamAPI_ISteamInput_GetCurrentActionSet", kParams_SteamInput006_11},
    {"SteamAPI_ISteamInput_ActivateActionSetLayer", kParams_SteamInput006_12},
    {"SteamAPI_ISteamInput_DeactivateActionSetLayer", kParams_SteamInput006_13},
    {"SteamAPI_ISteamInput_DeactivateAllActionSetLayers", kParams_SteamInput006_14},
    {"SteamAPI_ISteamInput_GetActiveActionSetLayers", kParams_SteamInput006_15},
    {"SteamAPI_ISteamInput_GetDigitalActionHandle", kParams_SteamInput006_16},
    {"SteamAPI_ISteamInput_GetDigitalActionData", kParams_SteamInput006_17},
    {"SteamAPI_ISteamInput_GetDigitalActionOrigins", kParams_SteamInput006_18},
    {"SteamAPI_ISteamInput_GetStringForDigitalActionName", kParams_SteamInput006_19},
    {"SteamAPI_ISteamInput_GetAnalogActionHandle", kParams_SteamInput006_20},
    {"SteamAPI_ISteamInput_GetAnalogActionData", kParams_SteamInput006_21},
    {"SteamAPI_ISteamInput_GetAnalogActionOrigins", kParams_SteamInput006_22},
    {"SteamAPI_ISteamInput_GetGlyphPNGForActionOrigin", kParams_SteamInput006_23},
    {"SteamAPI_ISteamInput_GetGlyphSVGForActionOrigin", kParams_SteamInput006_24},
    {"SteamAPI_ISteamInput_GetGlyphForActionOrigin_Legacy", kParams_SteamInput006_25},
    {"SteamAPI_ISteamInput_GetStringForActionOrigin", kParams_SteamInput006_26},
    {"SteamAPI_ISteamInput_GetStringForAnalogActionName", kParams_SteamInput006_27},
    {"SteamAPI_ISteamInput_StopAnalogActionMomentum", kParams_SteamInput006_28},
    {"SteamAPI_ISteamInput_GetMotionData", kParams_SteamInput006_29},
    {"SteamAPI_ISteamInput_TriggerVibration", kParams_SteamInput006_30},
    {"SteamAPI_ISteamInput_TriggerVibrationExtended", kParams_SteamInput006_31},
    {"SteamAPI_ISteamInput_TriggerSimpleHapticEvent", kParams_SteamInput006_32},
    {"SteamAPI_ISteamInput_SetLEDColor", kParams_SteamInput006_33},
    {"SteamAPI_ISteamInput_Legacy_TriggerHapticPulse", kParams_SteamInput006_34},
    {"SteamAPI_ISteamInput_Legacy_TriggerRepeatedHapticPulse", kParams_SteamInput006_35},
    {"SteamAPI_ISteamInput_ShowBindingPanel", kParams_SteamInput006_36},
    {"SteamAPI_ISteamInput_GetInputTypeForHandle", kParams_SteamInput006_37},
    {"SteamAPI_ISteamInput_GetControllerForGamepadIndex", kParams_SteamInput006_38},
    {"SteamAPI_ISteamInput_GetGamepadIndexForController", kParams_SteamInput006_39},
    {"SteamAPI_ISteamInput_GetStringForXboxOrigin", kParams_SteamInput006_40},
    {"SteamAPI_ISteamInput_GetGlyphForXboxOrigin", kParams_SteamInput006_41},
    {"SteamAPI_ISteamInput_GetActionOriginFromXboxOrigin", kParams_SteamInput006_42},
    {"SteamAPI_ISteamInput_TranslateActionOrigin", kParams_SteamInput006_43},
    {"SteamAPI_ISteamInput_GetDeviceBindingRevision", kParams_SteamInput006_44},
    {"SteamAPI_ISteamInput_GetRemotePlaySessionID", kParams_SteamInput006_45},
    {"SteamAPI_ISteamInput_GetSessionInputConfigurationSettings", nullptr},
    {"SteamAPI_ISteamInput_SetDualSenseTriggerEffect", kParams_SteamInput006_47},
};

class Version_SteamInput006 {
public:
    virtual bool Init(bool bExplicitlyCallRunFrame) {
        return steambridge::slot<bool>(kSlots_SteamInput006[0], bExplicitlyCallRunFrame);
    }
    virtual bool Shutdown() {
        return steambridge::slot<bool>(kSlots_SteamInput006[1]);
    }
    virtual bool SetInputActionManifestFilePath(const char* pchInputActionManifestAbsolutePath) {
        return steambridge::slot<bool>(kSlots_SteamInput006[2], pchInputActionManifestAbsolutePath);
    }
    virtual void RunFrame(bool bReservedValue) {
        steambridge::slot<void>(kSlots_SteamInput006[3], bReservedValue);
    }
    virtual bool BWaitForData(bool bWaitForever, std::uint32_t unTimeout) {
        return steambridge::slot<bool>(kSlots_SteamInput006[4], bWaitForever, unTimeout);
    }
    virtual bool BNewDataAvailable() {
        return steambridge::slot<bool>(kSlots_SteamInput006[5]);
    }
    virtual std::int32_t GetConnectedControllers(std::uint64_t* handlesOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput006[6], handlesOut);
    }
    virtual void EnableDeviceCallbacks() {
        steambridge::slot<void>(kSlots_SteamInput006[7]);
    }
    virtual void EnableActionEventCallbacks(void* pCallback) {
        steambridge::slot<void>(kSlots_SteamInput006[8], pCallback);
    }
    virtual std::uint64_t GetActionSetHandle(const char* pszActionSetName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamInput006[9], pszActionSetName);
    }
    virtual void ActivateActionSet(std::uint64_t inputHandle, std::uint64_t actionSetHandle) {
        steambridge::slot<void>(kSlots_SteamInput006[10], inputHandle, actionSetHandle);
    }
    virtual std::uint64_t GetCurrentActionSet(std::uint64_t inputHandle) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamInput006[11], inputHandle);
    }
    virtual void ActivateActionSetLayer(std::uint64_t inputHandle, std::uint64_t actionSetLayerHandle) {
        steambridge::slot<void>(kSlots_SteamInput006[12], inputHandle, actionSetLayerHandle);
    }
    virtual void DeactivateActionSetLayer(std::uint64_t inputHandle, std::uint64_t actionSetLayerHandle) {
        steambridge::slot<void>(kSlots_SteamInput006[13], inputHandle, actionSetLayerHandle);
    }
    virtual void DeactivateAllActionSetLayers(std::uint64_t inputHandle) {
        steambridge::slot<void>(kSlots_SteamInput006[14], inputHandle);
    }
    virtual std::int32_t GetActiveActionSetLayers(std::uint64_t inputHandle, std::uint64_t* handlesOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput006[15], inputHandle, handlesOut);
    }
    virtual std::uint64_t GetDigitalActionHandle(const char* pszActionName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamInput006[16], pszActionName);
    }
    virtual InputDigitalActionData_t GetDigitalActionData(std::uint64_t inputHandle, std::uint64_t digitalActionHandle) {
        return steambridge::slot<InputDigitalActionData_t>(kSlots_SteamInput006[17], inputHandle, digitalActionHandle);
    }
    virtual std::int32_t GetDigitalActionOrigins(std::uint64_t inputHandle, std::uint64_t actionSetHandle, std::uint64_t digitalActionHandle, std::int32_t* originsOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput006[18], inputHandle, actionSetHandle, digitalActionHandle, originsOut);
    }
    virtual const char* GetStringForDigitalActionName(std::uint64_t eActionHandle) {
        return steambridge::slot<const char*>(kSlots_SteamInput006[19], eActionHandle);
    }
    virtual std::uint64_t GetAnalogActionHandle(const char* pszActionName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamInput006[20], pszActionName);
    }
    virtual InputAnalogActionData_t GetAnalogActionData(std::uint64_t inputHandle, std::uint64_t analogActionHandle) {
        return steambridge::slot<InputAnalogActionData_t>(kSlots_SteamInput006[21], inputHandle, analogActionHandle);
    }
    virtual std::int32_t GetAnalogActionOrigins(std::uint64_t inputHandle, std::uint64_t actionSetHandle, std::uint64_t analogActionHandle, std::int32_t* originsOut) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput006[22], inputHandle, actionSetHandle, analogActionHandle, originsOut);
    }
    virtual const char* GetGlyphPNGForActionOrigin(std::int32_t eOrigin, std::int32_t eSize, std::uint32_t unFlags) {
        return steambridge::slot<const char*>(kSlots_SteamInput006[23], eOrigin, eSize, unFlags);
    }
    virtual const char* GetGlyphSVGForActionOrigin(std::int32_t eOrigin, std::uint32_t unFlags) {
        return steambridge::slot<const char*>(kSlots_SteamInput006[24], eOrigin, unFlags);
    }
    virtual const char* GetGlyphForActionOrigin_Legacy(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamInput006[25], eOrigin);
    }
    virtual const char* GetStringForActionOrigin(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamInput006[26], eOrigin);
    }
    virtual const char* GetStringForAnalogActionName(std::uint64_t eActionHandle) {
        return steambridge::slot<const char*>(kSlots_SteamInput006[27], eActionHandle);
    }
    virtual void StopAnalogActionMomentum(std::uint64_t inputHandle, std::uint64_t eAction) {
        steambridge::slot<void>(kSlots_SteamInput006[28], inputHandle, eAction);
    }
    virtual InputMotionData_t GetMotionData(std::uint64_t inputHandle) {
        return steambridge::slot<InputMotionData_t>(kSlots_SteamInput006[29], inputHandle);
    }
    virtual void TriggerVibration(std::uint64_t inputHandle, std::uint16_t usLeftSpeed, std::uint16_t usRightSpeed) {
        steambridge::slot<void>(kSlots_SteamInput006[30], inputHandle, usLeftSpeed, usRightSpeed);
    }
    virtual void TriggerVibrationExtended(std::uint64_t inputHandle, std::uint16_t usLeftSpeed, std::uint16_t usRightSpeed, std::uint16_t usLeftTriggerSpeed, std::uint16_t usRightTriggerSpeed) {
        steambridge::slot<void>(kSlots_SteamInput006[31], inputHandle, usLeftSpeed, usRightSpeed, usLeftTriggerSpeed, usRightTriggerSpeed);
    }
    virtual void TriggerSimpleHapticEvent(std::uint64_t inputHandle, std::int32_t eHapticLocation, std::uint8_t nIntensity, std::int8_t nGainDB, std::uint8_t nOtherIntensity, std::int8_t nOtherGainDB) {
        steambridge::slot<void>(kSlots_SteamInput006[32], inputHandle, eHapticLocation, nIntensity, nGainDB, nOtherIntensity, nOtherGainDB);
    }
    virtual void SetLEDColor(std::uint64_t inputHandle, std::uint8_t nColorR, std::uint8_t nColorG, std::uint8_t nColorB, std::uint32_t nFlags) {
        steambridge::slot<void>(kSlots_SteamInput006[33], inputHandle, nColorR, nColorG, nColorB, nFlags);
    }
    virtual void Legacy_TriggerHapticPulse(std::uint64_t inputHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec) {
        steambridge::slot<void>(kSlots_SteamInput006[34], inputHandle, eTargetPad, usDurationMicroSec);
    }
    virtual void Legacy_TriggerRepeatedHapticPulse(std::uint64_t inputHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec, std::uint16_t usOffMicroSec, std::uint16_t unRepeat, std::uint32_t nFlags) {
        steambridge::slot<void>(kSlots_SteamInput006[35], inputHandle, eTargetPad, usDurationMicroSec, usOffMicroSec, unRepeat, nFlags);
    }
    virtual bool ShowBindingPanel(std::uint64_t inputHandle) {
        return steambridge::slot<bool>(kSlots_SteamInput006[36], inputHandle);
    }
    virtual std::int32_t GetInputTypeForHandle(std::uint64_t inputHandle) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput006[37], inputHandle);
    }
    virtual std::uint64_t GetControllerForGamepadIndex(std::int32_t nIndex) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamInput006[38], nIndex);
    }
    virtual std::int32_t GetGamepadIndexForController(std::uint64_t ulinputHandle) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput006[39], ulinputHandle);
    }
    virtual const char* GetStringForXboxOrigin(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamInput006[40], eOrigin);
    }
    virtual const char* GetGlyphForXboxOrigin(std::int32_t eOrigin) {
        return steambridge::slot<const char*>(kSlots_SteamInput006[41], eOrigin);
    }
    virtual std::int32_t GetActionOriginFromXboxOrigin(std::uint64_t inputHandle, std::int32_t eOrigin) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput006[42], inputHandle, eOrigin);
    }
    virtual std::int32_t TranslateActionOrigin(std::int32_t eDestinationInputType, std::int32_t eSourceOrigin) {
        return steambridge::slot<std::int32_t>(kSlots_SteamInput006[43], eDestinationInputType, eSourceOrigin);
    }
    virtual bool GetDeviceBindingRevision(std::uint64_t inputHandle, std::int32_t* pMajor, std::int32_t* pMinor) {
        return steambridge::slot<bool>(kSlots_SteamInput006[44], inputHandle, pMajor, pMinor);
    }
    virtual std::uint32_t GetRemotePlaySessionID(std::uint64_t inputHandle) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamInput006[45], inputHandle);
    }
    virtual std::uint16_t GetSessionInputConfigurationSettings() {
        return steambridge::slot<std::uint16_t>(kSlots_SteamInput006[46]);
    }
    virtual void SetDualSenseTriggerEffect(std::uint64_t inputHandle, void* pParam) {
        steambridge::slot<void>(kSlots_SteamInput006[47], inputHandle, pParam);
    }
};

Version_SteamInput006 g_SteamInput006;

const char* const kParams_STEAMINVENTORY_INTERFACE_V002_0[] = {
    "resultHandle",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_1[] = {
    "resultHandle",
    "pOutItemsArray",
    "punOutItemsArraySize",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_2[] = {
    "resultHandle",
    "unItemIndex",
    "pchPropertyName",
    "pchValueBuffer",
    "punValueBufferSizeOut",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_3[] = {
    "resultHandle",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_4[] = {
    "resultHandle",
    "steamIDExpected",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_5[] = {
    "resultHandle",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_6[] = {
    "pResultHandle",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_7[] = {
    "pResultHandle",
    "pInstanceIDs",
    "unCountInstanceIDs",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_8[] = {
    "resultHandle",
    "pOutBuffer",
    "punOutBufferSize",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_9[] = {
    "pOutResultHandle",
    "pBuffer",
    "unBufferSize",
    "bRESERVED_MUST_BE_FALSE",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_10[] = {
    "pResultHandle",
    "pArrayItemDefs",
    "punArrayQuantity",
    "unArrayLength",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_11[] = {
    "pResultHandle",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_12[] = {
    "pResultHandle",
    "itemDef",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_13[] = {
    "pResultHandle",
    "pArrayItemDefs",
    "unArrayLength",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_14[] = {
    "pResultHandle",
    "itemConsume",
    "unQuantity",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_15[] = {
    "pResultHandle",
    "pArrayGenerate",
    "punArrayGenerateQuantity",
    "unArrayGenerateLength",
    "pArrayDestroy",
    "punArrayDestroyQuantity",
    "unArrayDestroyLength",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_16[] = {
    "pResultHandle",
    "itemIdSource",
    "unQuantity",
    "itemIdDest",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_18[] = {
    "pResultHandle",
    "dropListDefinition",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_19[] = {
    "pResultHandle",
    "steamIDTradePartner",
    "pArrayGive",
    "pArrayGiveQuantity",
    "nArrayGiveLength",
    "pArrayGet",
    "pArrayGetQuantity",
    "nArrayGetLength",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_21[] = {
    "pItemDefIDs",
    "punItemDefIDsArraySize",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_22[] = {
    "iDefinition",
    "pchPropertyName",
    "pchValueBuffer",
    "punValueBufferSizeOut",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_23[] = {
    "steamID",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V002_24[] = {
    "steamID",
    "pItemDefIDs",
    "punItemDefIDsArraySize",
};
// ISteamInventory STEAMINVENTORY_INTERFACE_V002
const steambridge::SlotInfo kSlots_STEAMINVENTORY_INTERFACE_V002[] = {
    {"SteamAPI_ISteamInventory_GetResultStatus", kParams_STEAMINVENTORY_INTERFACE_V002_0},
    {"SteamAPI_ISteamInventory_GetResultItems", kParams_STEAMINVENTORY_INTERFACE_V002_1},
    {"SteamAPI_ISteamInventory_GetResultItemProperty", kParams_STEAMINVENTORY_INTERFACE_V002_2},
    {"SteamAPI_ISteamInventory_GetResultTimestamp", kParams_STEAMINVENTORY_INTERFACE_V002_3},
    {"SteamAPI_ISteamInventory_CheckResultSteamID", kParams_STEAMINVENTORY_INTERFACE_V002_4},
    {"SteamAPI_ISteamInventory_DestroyResult", kParams_STEAMINVENTORY_INTERFACE_V002_5},
    {"SteamAPI_ISteamInventory_GetAllItems", kParams_STEAMINVENTORY_INTERFACE_V002_6},
    {"SteamAPI_ISteamInventory_GetItemsByID", kParams_STEAMINVENTORY_INTERFACE_V002_7},
    {"SteamAPI_ISteamInventory_SerializeResult", kParams_STEAMINVENTORY_INTERFACE_V002_8},
    {"SteamAPI_ISteamInventory_DeserializeResult", kParams_STEAMINVENTORY_INTERFACE_V002_9},
    {"SteamAPI_ISteamInventory_GenerateItems", kParams_STEAMINVENTORY_INTERFACE_V002_10},
    {"SteamAPI_ISteamInventory_GrantPromoItems", kParams_STEAMINVENTORY_INTERFACE_V002_11},
    {"SteamAPI_ISteamInventory_AddPromoItem", kParams_STEAMINVENTORY_INTERFACE_V002_12},
    {"SteamAPI_ISteamInventory_AddPromoItems", kParams_STEAMINVENTORY_INTERFACE_V002_13},
    {"SteamAPI_ISteamInventory_ConsumeItem", kParams_STEAMINVENTORY_INTERFACE_V002_14},
    {"SteamAPI_ISteamInventory_ExchangeItems", kParams_STEAMINVENTORY_INTERFACE_V002_15},
    {"SteamAPI_ISteamInventory_TransferItemQuantity", kParams_STEAMINVENTORY_INTERFACE_V002_16},
    {"SteamAPI_ISteamInventory_SendItemDropHeartbeat", nullptr},
    {"SteamAPI_ISteamInventory_TriggerItemDrop", kParams_STEAMINVENTORY_INTERFACE_V002_18},
    {"SteamAPI_ISteamInventory_TradeItems", kParams_STEAMINVENTORY_INTERFACE_V002_19},
    {"SteamAPI_ISteamInventory_LoadItemDefinitions", nullptr},
    {"SteamAPI_ISteamInventory_GetItemDefinitionIDs", kParams_STEAMINVENTORY_INTERFACE_V002_21},
    {"SteamAPI_ISteamInventory_GetItemDefinitionProperty", kParams_STEAMINVENTORY_INTERFACE_V002_22},
    {"SteamAPI_ISteamInventory_RequestEligiblePromoItemDefinitionsIDs", kParams_STEAMINVENTORY_INTERFACE_V002_23},
    {"SteamAPI_ISteamInventory_GetEligiblePromoItemDefinitionIDs", kParams_STEAMINVENTORY_INTERFACE_V002_24},
};

class Version_STEAMINVENTORY_INTERFACE_V002 {
public:
    virtual std::int32_t GetResultStatus(std::int32_t resultHandle) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMINVENTORY_INTERFACE_V002[0], resultHandle);
    }
    virtual bool GetResultItems(std::int32_t resultHandle, void* pOutItemsArray, std::uint32_t* punOutItemsArraySize) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V002[1], resultHandle, pOutItemsArray, punOutItemsArraySize);
    }
    virtual bool GetResultItemProperty(std::int32_t resultHandle, std::uint32_t unItemIndex, const char* pchPropertyName, void* pchValueBuffer, std::uint32_t* punValueBufferSizeOut) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V002[2], resultHandle, unItemIndex, pchPropertyName, pchValueBuffer, punValueBufferSizeOut);
    }
    virtual std::uint32_t GetResultTimestamp(std::int32_t resultHandle) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMINVENTORY_INTERFACE_V002[3], resultHandle);
    }
    virtual bool CheckResultSteamID(std::int32_t resultHandle, CSteamID steamIDExpected) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V002[4], resultHandle, steamIDExpected);
    }
    virtual void DestroyResult(std::int32_t resultHandle) {
        steambridge::slot<void>(kSlots_STEAMINVENTORY_INTERFACE_V002[5], resultHandle);
    }
    virtual bool GetAllItems(std::int32_t* pResultHandle) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V002[6], pResultHandle);
    }
    virtual bool GetItemsByID(std::int32_t* pResultHandle, void* pInstanceIDs, std::uint32_t unCountInstanceIDs) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V002[7], pResultHandle, pInstanceIDs, unCountInstanceIDs);
    }
    virtual bool SerializeResult(std::int32_t resultHandle, void* pOutBuffer, std::uint32_t* punOutBufferSize) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V002[8], resultHandle, pOutBuffer, punOutBufferSize);
    }
    virtual bool DeserializeResult(std::int32_t* pOutResultHandle, void* pBuffer, std::uint32_t unBufferSize, bool bRESERVED_MUST_BE_FALSE) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V002[9], pOutResultHandle, pBuffer, unBufferSize, bRESERVED_MUST_BE_FALSE);
    }
    virtual bool GenerateItems(std::int32_t* pResultHandle, void* pArrayItemDefs, void* punArrayQuantity, std::uint32_t unArrayLength) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V002[10], pResultHandle, pArrayItemDefs, punArrayQuantity, unArrayLength);
    }
    virtual bool GrantPromoItems(std::int32_t* pResultHandle) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V002[11], pResultHandle);
    }
    virtual bool AddPromoItem(std::int32_t* pResultHandle, std::int32_t itemDef) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V002[12], pResultHandle, itemDef);
    }
    virtual bool AddPromoItems(std::int32_t* pResultHandle, void* pArrayItemDefs, std::uint32_t unArrayLength) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V002[13], pResultHandle, pArrayItemDefs, unArrayLength);
    }
    virtual bool ConsumeItem(std::int32_t* pResultHandle, std::uint64_t itemConsume, std::uint32_t unQuantity) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V002[14], pResultHandle, itemConsume, unQuantity);
    }
    virtual bool ExchangeItems(std::int32_t* pResultHandle, void* pArrayGenerate, void* punArrayGenerateQuantity, std::uint32_t unArrayGenerateLength, void* pArrayDestroy, void* punArrayDestroyQuantity, std::uint32_t unArrayDestroyLength) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V002[15], pResultHandle, pArrayGenerate, punArrayGenerateQuantity, unArrayGenerateLength, pArrayDestroy, punArrayDestroyQuantity, unArrayDestroyLength);
    }
    virtual bool TransferItemQuantity(std::int32_t* pResultHandle, std::uint64_t itemIdSource, std::uint32_t unQuantity, std::uint64_t itemIdDest) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V002[16], pResultHandle, itemIdSource, unQuantity, itemIdDest);
    }
    virtual void SendItemDropHeartbeat() {
        steambridge::slot<void>(kSlots_STEAMINVENTORY_INTERFACE_V002[17]);
    }
    virtual bool TriggerItemDrop(std::int32_t* pResultHandle, std::int32_t dropListDefinition) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V002[18], pResultHandle, dropListDefinition);
    }
    virtual bool TradeItems(std::int32_t* pResultHandle, CSteamID steamIDTradePartner, void* pArrayGive, void* pArrayGiveQuantity, std::uint32_t nArrayGiveLength, void* pArrayGet, void* pArrayGetQuantity, std::uint32_t nArrayGetLength) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V002[19], pResultHandle, steamIDTradePartner, pArrayGive, pArrayGiveQuantity, nArrayGiveLength, pArrayGet, pArrayGetQuantity, nArrayGetLength);
    }
    virtual bool LoadItemDefinitions() {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V002[20]);
    }
    virtual bool GetItemDefinitionIDs(std::int32_t* pItemDefIDs, std::uint32_t* punItemDefIDsArraySize) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V002[21], pItemDefIDs, punItemDefIDsArraySize);
    }
    virtual bool GetItemDefinitionProperty(std::int32_t iDefinition, const char* pchPropertyName, void* pchValueBuffer, std::uint32_t* punValueBufferSizeOut) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V002[22], iDefinition, pchPropertyName, pchValueBuffer, punValueBufferSizeOut);
    }
    virtual std::uint64_t RequestEligiblePromoItemDefinitionsIDs(CSteamID steamID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMINVENTORY_INTERFACE_V002[23], steamID);
    }
    virtual bool GetEligiblePromoItemDefinitionIDs(CSteamID steamID, std::int32_t* pItemDefIDs, std::uint32_t* punItemDefIDsArraySize) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V002[24], steamID, pItemDefIDs, punItemDefIDsArraySize);
    }
};

Version_STEAMINVENTORY_INTERFACE_V002 g_STEAMINVENTORY_INTERFACE_V002;

const char* const kParams_STEAMINVENTORY_INTERFACE_V003_0[] = {
    "resultHandle",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_1[] = {
    "resultHandle",
    "pOutItemsArray",
    "punOutItemsArraySize",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_2[] = {
    "resultHandle",
    "unItemIndex",
    "pchPropertyName",
    "pchValueBuffer",
    "punValueBufferSizeOut",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_3[] = {
    "resultHandle",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_4[] = {
    "resultHandle",
    "steamIDExpected",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_5[] = {
    "resultHandle",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_6[] = {
    "pResultHandle",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_7[] = {
    "pResultHandle",
    "pInstanceIDs",
    "unCountInstanceIDs",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_8[] = {
    "resultHandle",
    "pOutBuffer",
    "punOutBufferSize",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_9[] = {
    "pOutResultHandle",
    "pBuffer",
    "unBufferSize",
    "bRESERVED_MUST_BE_FALSE",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_10[] = {
    "pResultHandle",
    "pArrayItemDefs",
    "punArrayQuantity",
    "unArrayLength",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_11[] = {
    "pResultHandle",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_12[] = {
    "pResultHandle",
    "itemDef",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_13[] = {
    "pResultHandle",
    "pArrayItemDefs",
    "unArrayLength",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_14[] = {
    "pResultHandle",
    "itemConsume",
    "unQuantity",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_15[] = {
    "pResultHandle",
    "pArrayGenerate",
    "punArrayGenerateQuantity",
    "unArrayGenerateLength",
    "pArrayDestroy",
    "punArrayDestroyQuantity",
    "unArrayDestroyLength",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_16[] = {
    "pResultHandle",
    "itemIdSource",
    "unQuantity",
    "itemIdDest",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_18[] = {
    "pResultHandle",
    "dropListDefinition",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_19[] = {
    "pResultHandle",
    "steamIDTradePartner",
    "pArrayGive",
    "pArrayGiveQuantity",
    "nArrayGiveLength",
    "pArrayGet",
    "pArrayGetQuantity",
    "nArrayGetLength",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_21[] = {
    "pItemDefIDs",
    "punItemDefIDsArraySize",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_22[] = {
    "iDefinition",
    "pchPropertyName",
    "pchValueBuffer",
    "punValueBufferSizeOut",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_23[] = {
    "steamID",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_24[] = {
    "steamID",
    "pItemDefIDs",
    "punItemDefIDsArraySize",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_25[] = {
    "pArrayItemDefs",
    "punArrayQuantity",
    "unArrayLength",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_28[] = {
    "pArrayItemDefs",
    "pCurrentPrices",
    "pBasePrices",
    "unArrayLength",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_29[] = {
    "iDefinition",
    "pCurrentPrice",
    "pBasePrice",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_31[] = {
    "handle",
    "nItemID",
    "pchPropertyName",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_32[] = {
    "handle",
    "nItemID",
    "pchPropertyName",
    "pchPropertyValue",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_33[] = {
    "handle",
    "nItemID",
    "pchPropertyName",
    "bValue",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_34[] = {
    "handle",
    "nItemID",
    "pchPropertyName",
    "nValue",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_35[] = {
    "handle",
    "nItemID",
    "pchPropertyName",
    "flValue",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_36[] = {
    "handle",
    "pResultHandle",
};
const char* const kParams_STEAMINVENTORY_INTERFACE_V003_37[] = {
    "pResultHandle",
    "pchItemToken",
};
// ISteamInventory STEAMINVENTORY_INTERFACE_V003
const steambridge::SlotInfo kSlots_STEAMINVENTORY_INTERFACE_V003[] = {
    {"SteamAPI_ISteamInventory_GetResultStatus", kParams_STEAMINVENTORY_INTERFACE_V003_0},
    {"SteamAPI_ISteamInventory_GetResultItems", kParams_STEAMINVENTORY_INTERFACE_V003_1},
    {"SteamAPI_ISteamInventory_GetResultItemProperty", kParams_STEAMINVENTORY_INTERFACE_V003_2},
    {"SteamAPI_ISteamInventory_GetResultTimestamp", kParams_STEAMINVENTORY_INTERFACE_V003_3},
    {"SteamAPI_ISteamInventory_CheckResultSteamID", kParams_STEAMINVENTORY_INTERFACE_V003_4},
    {"SteamAPI_ISteamInventory_DestroyResult", kParams_STEAMINVENTORY_INTERFACE_V003_5},
    {"SteamAPI_ISteamInventory_GetAllItems", kParams_STEAMINVENTORY_INTERFACE_V003_6},
    {"SteamAPI_ISteamInventory_GetItemsByID", kParams_STEAMINVENTORY_INTERFACE_V003_7},
    {"SteamAPI_ISteamInventory_SerializeResult", kParams_STEAMINVENTORY_INTERFACE_V003_8},
    {"SteamAPI_ISteamInventory_DeserializeResult", kParams_STEAMINVENTORY_INTERFACE_V003_9},
    {"SteamAPI_ISteamInventory_GenerateItems", kParams_STEAMINVENTORY_INTERFACE_V003_10},
    {"SteamAPI_ISteamInventory_GrantPromoItems", kParams_STEAMINVENTORY_INTERFACE_V003_11},
    {"SteamAPI_ISteamInventory_AddPromoItem", kParams_STEAMINVENTORY_INTERFACE_V003_12},
    {"SteamAPI_ISteamInventory_AddPromoItems", kParams_STEAMINVENTORY_INTERFACE_V003_13},
    {"SteamAPI_ISteamInventory_ConsumeItem", kParams_STEAMINVENTORY_INTERFACE_V003_14},
    {"SteamAPI_ISteamInventory_ExchangeItems", kParams_STEAMINVENTORY_INTERFACE_V003_15},
    {"SteamAPI_ISteamInventory_TransferItemQuantity", kParams_STEAMINVENTORY_INTERFACE_V003_16},
    {"SteamAPI_ISteamInventory_SendItemDropHeartbeat", nullptr},
    {"SteamAPI_ISteamInventory_TriggerItemDrop", kParams_STEAMINVENTORY_INTERFACE_V003_18},
    {"SteamAPI_ISteamInventory_TradeItems", kParams_STEAMINVENTORY_INTERFACE_V003_19},
    {"SteamAPI_ISteamInventory_LoadItemDefinitions", nullptr},
    {"SteamAPI_ISteamInventory_GetItemDefinitionIDs", kParams_STEAMINVENTORY_INTERFACE_V003_21},
    {"SteamAPI_ISteamInventory_GetItemDefinitionProperty", kParams_STEAMINVENTORY_INTERFACE_V003_22},
    {"SteamAPI_ISteamInventory_RequestEligiblePromoItemDefinitionsIDs", kParams_STEAMINVENTORY_INTERFACE_V003_23},
    {"SteamAPI_ISteamInventory_GetEligiblePromoItemDefinitionIDs", kParams_STEAMINVENTORY_INTERFACE_V003_24},
    {"SteamAPI_ISteamInventory_StartPurchase", kParams_STEAMINVENTORY_INTERFACE_V003_25},
    {"SteamAPI_ISteamInventory_RequestPrices", nullptr},
    {"SteamAPI_ISteamInventory_GetNumItemsWithPrices", nullptr},
    {"SteamAPI_ISteamInventory_GetItemsWithPrices", kParams_STEAMINVENTORY_INTERFACE_V003_28},
    {"SteamAPI_ISteamInventory_GetItemPrice", kParams_STEAMINVENTORY_INTERFACE_V003_29},
    {"SteamAPI_ISteamInventory_StartUpdateProperties", nullptr},
    {"SteamAPI_ISteamInventory_RemoveProperty", kParams_STEAMINVENTORY_INTERFACE_V003_31},
    {"SteamAPI_ISteamInventory_SetPropertyString", kParams_STEAMINVENTORY_INTERFACE_V003_32},
    {"SteamAPI_ISteamInventory_SetPropertyBool", kParams_STEAMINVENTORY_INTERFACE_V003_33},
    {"SteamAPI_ISteamInventory_SetPropertyInt64", kParams_STEAMINVENTORY_INTERFACE_V003_34},
    {"SteamAPI_ISteamInventory_SetPropertyFloat", kParams_STEAMINVENTORY_INTERFACE_V003_35},
    {"SteamAPI_ISteamInventory_SubmitUpdateProperties", kParams_STEAMINVENTORY_INTERFACE_V003_36},
    {"SteamAPI_ISteamInventory_InspectItem", kParams_STEAMINVENTORY_INTERFACE_V003_37},
};

class Version_STEAMINVENTORY_INTERFACE_V003 {
public:
    virtual std::int32_t GetResultStatus(std::int32_t resultHandle) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMINVENTORY_INTERFACE_V003[0], resultHandle);
    }
    virtual bool GetResultItems(std::int32_t resultHandle, void* pOutItemsArray, std::uint32_t* punOutItemsArraySize) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[1], resultHandle, pOutItemsArray, punOutItemsArraySize);
    }
    virtual bool GetResultItemProperty(std::int32_t resultHandle, std::uint32_t unItemIndex, const char* pchPropertyName, void* pchValueBuffer, std::uint32_t* punValueBufferSizeOut) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[2], resultHandle, unItemIndex, pchPropertyName, pchValueBuffer, punValueBufferSizeOut);
    }
    virtual std::uint32_t GetResultTimestamp(std::int32_t resultHandle) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMINVENTORY_INTERFACE_V003[3], resultHandle);
    }
    virtual bool CheckResultSteamID(std::int32_t resultHandle, CSteamID steamIDExpected) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[4], resultHandle, steamIDExpected);
    }
    virtual void DestroyResult(std::int32_t resultHandle) {
        steambridge::slot<void>(kSlots_STEAMINVENTORY_INTERFACE_V003[5], resultHandle);
    }
    virtual bool GetAllItems(std::int32_t* pResultHandle) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[6], pResultHandle);
    }
    virtual bool GetItemsByID(std::int32_t* pResultHandle, void* pInstanceIDs, std::uint32_t unCountInstanceIDs) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[7], pResultHandle, pInstanceIDs, unCountInstanceIDs);
    }
    virtual bool SerializeResult(std::int32_t resultHandle, void* pOutBuffer, std::uint32_t* punOutBufferSize) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[8], resultHandle, pOutBuffer, punOutBufferSize);
    }
    virtual bool DeserializeResult(std::int32_t* pOutResultHandle, void* pBuffer, std::uint32_t unBufferSize, bool bRESERVED_MUST_BE_FALSE) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[9], pOutResultHandle, pBuffer, unBufferSize, bRESERVED_MUST_BE_FALSE);
    }
    virtual bool GenerateItems(std::int32_t* pResultHandle, void* pArrayItemDefs, void* punArrayQuantity, std::uint32_t unArrayLength) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[10], pResultHandle, pArrayItemDefs, punArrayQuantity, unArrayLength);
    }
    virtual bool GrantPromoItems(std::int32_t* pResultHandle) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[11], pResultHandle);
    }
    virtual bool AddPromoItem(std::int32_t* pResultHandle, std::int32_t itemDef) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[12], pResultHandle, itemDef);
    }
    virtual bool AddPromoItems(std::int32_t* pResultHandle, void* pArrayItemDefs, std::uint32_t unArrayLength) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[13], pResultHandle, pArrayItemDefs, unArrayLength);
    }
    virtual bool ConsumeItem(std::int32_t* pResultHandle, std::uint64_t itemConsume, std::uint32_t unQuantity) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[14], pResultHandle, itemConsume, unQuantity);
    }
    virtual bool ExchangeItems(std::int32_t* pResultHandle, void* pArrayGenerate, void* punArrayGenerateQuantity, std::uint32_t unArrayGenerateLength, void* pArrayDestroy, void* punArrayDestroyQuantity, std::uint32_t unArrayDestroyLength) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[15], pResultHandle, pArrayGenerate, punArrayGenerateQuantity, unArrayGenerateLength, pArrayDestroy, punArrayDestroyQuantity, unArrayDestroyLength);
    }
    virtual bool TransferItemQuantity(std::int32_t* pResultHandle, std::uint64_t itemIdSource, std::uint32_t unQuantity, std::uint64_t itemIdDest) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[16], pResultHandle, itemIdSource, unQuantity, itemIdDest);
    }
    virtual void SendItemDropHeartbeat() {
        steambridge::slot<void>(kSlots_STEAMINVENTORY_INTERFACE_V003[17]);
    }
    virtual bool TriggerItemDrop(std::int32_t* pResultHandle, std::int32_t dropListDefinition) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[18], pResultHandle, dropListDefinition);
    }
    virtual bool TradeItems(std::int32_t* pResultHandle, CSteamID steamIDTradePartner, void* pArrayGive, void* pArrayGiveQuantity, std::uint32_t nArrayGiveLength, void* pArrayGet, void* pArrayGetQuantity, std::uint32_t nArrayGetLength) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[19], pResultHandle, steamIDTradePartner, pArrayGive, pArrayGiveQuantity, nArrayGiveLength, pArrayGet, pArrayGetQuantity, nArrayGetLength);
    }
    virtual bool LoadItemDefinitions() {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[20]);
    }
    virtual bool GetItemDefinitionIDs(std::int32_t* pItemDefIDs, std::uint32_t* punItemDefIDsArraySize) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[21], pItemDefIDs, punItemDefIDsArraySize);
    }
    virtual bool GetItemDefinitionProperty(std::int32_t iDefinition, const char* pchPropertyName, void* pchValueBuffer, std::uint32_t* punValueBufferSizeOut) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[22], iDefinition, pchPropertyName, pchValueBuffer, punValueBufferSizeOut);
    }
    virtual std::uint64_t RequestEligiblePromoItemDefinitionsIDs(CSteamID steamID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMINVENTORY_INTERFACE_V003[23], steamID);
    }
    virtual bool GetEligiblePromoItemDefinitionIDs(CSteamID steamID, std::int32_t* pItemDefIDs, std::uint32_t* punItemDefIDsArraySize) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[24], steamID, pItemDefIDs, punItemDefIDsArraySize);
    }
    virtual std::uint64_t StartPurchase(void* pArrayItemDefs, void* punArrayQuantity, std::uint32_t unArrayLength) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMINVENTORY_INTERFACE_V003[25], pArrayItemDefs, punArrayQuantity, unArrayLength);
    }
    virtual std::uint64_t RequestPrices() {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMINVENTORY_INTERFACE_V003[26]);
    }
    virtual std::uint32_t GetNumItemsWithPrices() {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMINVENTORY_INTERFACE_V003[27]);
    }
    virtual bool GetItemsWithPrices(std::int32_t* pArrayItemDefs, std::uint64_t* pCurrentPrices, std::uint64_t* pBasePrices, std::uint32_t unArrayLength) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[28], pArrayItemDefs, pCurrentPrices, pBasePrices, unArrayLength);
    }
    virtual bool GetItemPrice(std::int32_t iDefinition, std::uint64_t* pCurrentPrice, std::uint64_t* pBasePrice) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[29], iDefinition, pCurrentPrice, pBasePrice);
    }
    virtual std::uint64_t StartUpdateProperties() {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMINVENTORY_INTERFACE_V003[30]);
    }
    virtual bool RemoveProperty(std::uint64_t handle, std::uint64_t nItemID, const char* pchPropertyName) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[31], handle, nItemID, pchPropertyName);
    }
    virtual bool SetProperty(std::uint64_t handle, std::uint64_t nItemID, const char* pchPropertyName, const char* pchPropertyValue) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[32], handle, nItemID, pchPropertyName, pchPropertyValue);
    }
    virtual bool SetProperty(std::uint64_t handle, std::uint64_t nItemID, const char* pchPropertyName, bool bValue) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[33], handle, nItemID, pchPropertyName, bValue);
    }
    virtual bool SetProperty(std::uint64_t handle, std::uint64_t nItemID, const char* pchPropertyName, std::int64_t nValue) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[34], handle, nItemID, pchPropertyName, nValue);
    }
    virtual bool SetProperty(std::uint64_t handle, std::uint64_t nItemID, const char* pchPropertyName, float flValue) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[35], handle, nItemID, pchPropertyName, flValue);
    }
    virtual bool SubmitUpdateProperties(std::uint64_t handle, std::int32_t* pResultHandle) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[36], handle, pResultHandle);
    }
    virtual bool InspectItem(std::int32_t* pResultHandle, const char* pchItemToken) {
        return steambridge::slot<bool>(kSlots_STEAMINVENTORY_INTERFACE_V003[37], pResultHandle, pchItemToken);
    }
};

Version_STEAMINVENTORY_INTERFACE_V003 g_STEAMINVENTORY_INTERFACE_V003;

const char* const kParams_SteamMatchMaking009_1[] = {
    "iGame",
    "pnAppID",
    "pnIP",
    "pnConnPort",
    "pnQueryPort",
    "punFlags",
    "pRTime32LastPlayedOnServer",
};
const char* const kParams_SteamMatchMaking009_2[] = {
    "nAppID",
    "nIP",
    "nConnPort",
    "nQueryPort",
    "unFlags",
    "rTime32LastPlayedOnServer",
};
const char* const kParams_SteamMatchMaking009_3[] = {
    "nAppID",
    "nIP",
    "nConnPort",
    "nQueryPort",
    "unFlags",
};
const char* const kParams_SteamMatchMaking009_5[] = {
    "pchKeyToMatch",
    "pchValueToMatch",
    "eComparisonType",
};
const char* const kParams_SteamMatchMaking009_6[] = {
    "pchKeyToMatch",
    "nValueToMatch",
    "eComparisonType",
};
const char* const kParams_SteamMatchMaking009_7[] = {
    "pchKeyToMatch",
    "nValueToBeCloseTo",
};
const char* const kParams_SteamMatchMaking009_8[] = {
    "nSlotsAvailable",
};
const char* const kParams_SteamMatchMaking009_9[] = {
    "eLobbyDistanceFilter",
};
const char* const kParams_SteamMatchMaking009_10[] = {
    "cMaxResults",
};
const char* const kParams_SteamMatchMaking009_11[] = {
    "steamIDLobby",
};
const char* const kParams_SteamMatchMaking009_12[] = {
    "iLobby",
};
const char* const kParams_SteamMatchMaking009_13[] = {
    "eLobbyType",
    "cMaxMembers",
};
const char* const kParams_SteamMatchMaking009_14[] = {
    "steamIDLobby",
};
const char* const kParams_SteamMatchMaking009_15[] = {
    "steamIDLobby",
};
const char* const kParams_SteamMatchMaking009_16[] = {
    "steamIDLobby",
    "steamIDInvitee",
};
const char* const kParams_SteamMatchMaking009_17[] = {
    "steamIDLobby",
};
const char* const kParams_SteamMatchMaking009_18[] = {
    "steamIDLobby",
    "iMember",
};
const char* const kParams_SteamMatchMaking009_19[] = {
    "steamIDLobby",
    "pchKey",
};
const char* const kParams_SteamMatchMaking009_20[] = {
    "steamIDLobby",
    "pchKey",
    "pchValue",
};
const char* const kParams_SteamMatchMaking009_21[] = {
    "steamIDLobby",
};
const char* const kParams_SteamMatchMaking009_22[] = {
    "steamIDLobby",
    "iLobbyData",
    "pchKey",
    "cchKeyBufferSize",
    "pchValue",
    "cchValueBufferSize",
};
const char* const kParams_SteamMatchMaking009_23[] = {
    "steamIDLobby",
    "pchKey",
};
const char* const kParams_SteamMatchMaking009_24[] = {
    "steamIDLobby",
    "steamIDUser",
    "pchKey",
};
const char* const kParams_SteamMatchMaking009_25[] = {
    "steamIDLobby",
    "pchKey",
    "pchValue",
};
const char* const kParams_SteamMatchMaking009_26[] = {
    "steamIDLobby",
    "pvMsgBody",
    "cubMsgBody",
};
const char* const kParams_SteamMatchMaking009_27[] = {
    "steamIDLobby",
    "iChatID",
    "pSteamIDUser",
    "pvData",
    "cubData",
    "peChatEntryType",
};
const char* const kParams_SteamMatchMaking009_28[] = {
    "steamIDLobby",
};
const char* const kParams_SteamMatchMaking009_29[] = {
    "steamIDLobby",
    "unGameServerIP",
    "unGameServerPort",
    "steamIDGameServer",
};
const char* const kParams_SteamMatchMaking009_30[] = {
    "steamIDLobby",
    "punGameServerIP",
    "punGameServerPort",
    "psteamIDGameServer",
};
const char* const kParams_SteamMatchMaking009_31[] = {
    "steamIDLobby",
    "cMaxMembers",
};
const char* const kParams_SteamMatchMaking009_32[] = {
    "steamIDLobby",
};
const char* const kParams_SteamMatchMaking009_33[] = {
    "steamIDLobby",
    "eLobbyType",
};
const char* const kParams_SteamMatchMaking009_34[] = {
    "steamIDLobby",
    "bLobbyJoinable",
};
const char* const kParams_SteamMatchMaking009_35[] = {
    "steamIDLobby",
};
const char* const kParams_SteamMatchMaking009_36[] = {
    "steamIDLobby",
    "steamIDNewOwner",
};
const char* const kParams_SteamMatchMaking009_37[] = {
    "steamIDLobby",
    "steamIDLobbyDependent",
};
// ISteamMatchmaking SteamMatchMaking009
const steambridge::SlotInfo kSlots_SteamMatchMaking009[] = {
    {"SteamAPI_ISteamMatchmaking_GetFavoriteGameCount", nullptr},
    {"SteamAPI_ISteamMatchmaking_GetFavoriteGame", kParams_SteamMatchMaking009_1},
    {"SteamAPI_ISteamMatchmaking_AddFavoriteGame", kParams_SteamMatchMaking009_2},
    {"SteamAPI_ISteamMatchmaking_RemoveFavoriteGame", kParams_SteamMatchMaking009_3},
    {"SteamAPI_ISteamMatchmaking_RequestLobbyList", nullptr},
    {"SteamAPI_ISteamMatchmaking_AddRequestLobbyListStringFilter", kParams_SteamMatchMaking009_5},
    {"SteamAPI_ISteamMatchmaking_AddRequestLobbyListNumericalFilter", kParams_SteamMatchMaking009_6},
    {"SteamAPI_ISteamMatchmaking_AddRequestLobbyListNearValueFilter", kParams_SteamMatchMaking009_7},
    {"SteamAPI_ISteamMatchmaking_AddRequestLobbyListFilterSlotsAvailable", kParams_SteamMatchMaking009_8},
    {"SteamAPI_ISteamMatchmaking_AddRequestLobbyListDistanceFilter", kParams_SteamMatchMaking009_9},
    {"SteamAPI_ISteamMatchmaking_AddRequestLobbyListResultCountFilter", kParams_SteamMatchMaking009_10},
    {"SteamAPI_ISteamMatchmaking_AddRequestLobbyListCompatibleMembersFilter", kParams_SteamMatchMaking009_11},
    {"SteamAPI_ISteamMatchmaking_GetLobbyByIndex", kParams_SteamMatchMaking009_12},
    {"SteamAPI_ISteamMatchmaking_CreateLobby", kParams_SteamMatchMaking009_13},
    {"SteamAPI_ISteamMatchmaking_JoinLobby", kParams_SteamMatchMaking009_14},
    {"SteamAPI_ISteamMatchmaking_LeaveLobby", kParams_SteamMatchMaking009_15},
    {"SteamAPI_ISteamMatchmaking_InviteUserToLobby", kParams_SteamMatchMaking009_16},
    {"SteamAPI_ISteamMatchmaking_GetNumLobbyMembers", kParams_SteamMatchMaking009_17},
    {"SteamAPI_ISteamMatchmaking_GetLobbyMemberByIndex", kParams_SteamMatchMaking009_18},
    {"SteamAPI_ISteamMatchmaking_GetLobbyData", kParams_SteamMatchMaking009_19},
    {"SteamAPI_ISteamMatchmaking_SetLobbyData", kParams_SteamMatchMaking009_20},
    {"SteamAPI_ISteamMatchmaking_GetLobbyDataCount", kParams_SteamMatchMaking009_21},
    {"SteamAPI_ISteamMatchmaking_GetLobbyDataByIndex", kParams_SteamMatchMaking009_22},
    {"SteamAPI_ISteamMatchmaking_DeleteLobbyData", kParams_SteamMatchMaking009_23},
    {"SteamAPI_ISteamMatchmaking_GetLobbyMemberData", kParams_SteamMatchMaking009_24},
    {"SteamAPI_ISteamMatchmaking_SetLobbyMemberData", kParams_SteamMatchMaking009_25},
    {"SteamAPI_ISteamMatchmaking_SendLobbyChatMsg", kParams_SteamMatchMaking009_26},
    {"SteamAPI_ISteamMatchmaking_GetLobbyChatEntry", kParams_SteamMatchMaking009_27},
    {"SteamAPI_ISteamMatchmaking_RequestLobbyData", kParams_SteamMatchMaking009_28},
    {"SteamAPI_ISteamMatchmaking_SetLobbyGameServer", kParams_SteamMatchMaking009_29},
    {"SteamAPI_ISteamMatchmaking_GetLobbyGameServer", kParams_SteamMatchMaking009_30},
    {"SteamAPI_ISteamMatchmaking_SetLobbyMemberLimit", kParams_SteamMatchMaking009_31},
    {"SteamAPI_ISteamMatchmaking_GetLobbyMemberLimit", kParams_SteamMatchMaking009_32},
    {"SteamAPI_ISteamMatchmaking_SetLobbyType", kParams_SteamMatchMaking009_33},
    {"SteamAPI_ISteamMatchmaking_SetLobbyJoinable", kParams_SteamMatchMaking009_34},
    {"SteamAPI_ISteamMatchmaking_GetLobbyOwner", kParams_SteamMatchMaking009_35},
    {"SteamAPI_ISteamMatchmaking_SetLobbyOwner", kParams_SteamMatchMaking009_36},
    {"SteamAPI_ISteamMatchmaking_SetLinkedLobby", kParams_SteamMatchMaking009_37},
};

class Version_SteamMatchMaking009 {
public:
    virtual std::int32_t GetFavoriteGameCount() {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchMaking009[0]);
    }
    virtual bool GetFavoriteGame(std::int32_t iGame, std::uint32_t* pnAppID, std::uint32_t* pnIP, std::uint16_t* pnConnPort, std::uint16_t* pnQueryPort, std::uint32_t* punFlags, std::uint32_t* pRTime32LastPlayedOnServer) {
        return steambridge::slot<bool>(kSlots_SteamMatchMaking009[1], iGame, pnAppID, pnIP, pnConnPort, pnQueryPort, punFlags, pRTime32LastPlayedOnServer);
    }
    virtual std::int32_t AddFavoriteGame(std::uint32_t nAppID, std::uint32_t nIP, std::uint16_t nConnPort, std::uint16_t nQueryPort, std::uint32_t unFlags, std::uint32_t rTime32LastPlayedOnServer) {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchMaking009[2], nAppID, nIP, nConnPort, nQueryPort, unFlags, rTime32LastPlayedOnServer);
    }
    virtual bool RemoveFavoriteGame(std::uint32_t nAppID, std::uint32_t nIP, std::uint16_t nConnPort, std::uint16_t nQueryPort, std::uint32_t unFlags) {
        return steambridge::slot<bool>(kSlots_SteamMatchMaking009[3], nAppID, nIP, nConnPort, nQueryPort, unFlags);
    }
    virtual std::uint64_t RequestLobbyList() {
        return steambridge::slot<std::uint64_t>(kSlots_SteamMatchMaking009[4]);
    }
    virtual void AddRequestLobbyListStringFilter(const char* pchKeyToMatch, const char* pchValueToMatch, std::int32_t eComparisonType) {
        steambridge::slot<void>(kSlots_SteamMatchMaking009[5], pchKeyToMatch, pchValueToMatch, eComparisonType);
    }
    virtual void AddRequestLobbyListNumericalFilter(const char* pchKeyToMatch, std::int32_t nValueToMatch, std::int32_t eComparisonType) {
        steambridge::slot<void>(kSlots_SteamMatchMaking009[6], pchKeyToMatch, nValueToMatch, eComparisonType);
    }
    virtual void AddRequestLobbyListNearValueFilter(const char* pchKeyToMatch, std::int32_t nValueToBeCloseTo) {
        steambridge::slot<void>(kSlots_SteamMatchMaking009[7], pchKeyToMatch, nValueToBeCloseTo);
    }
    virtual void AddRequestLobbyListFilterSlotsAvailable(std::int32_t nSlotsAvailable) {
        steambridge::slot<void>(kSlots_SteamMatchMaking009[8], nSlotsAvailable);
    }
    virtual void AddRequestLobbyListDistanceFilter(std::int32_t eLobbyDistanceFilter) {
        steambridge::slot<void>(kSlots_SteamMatchMaking009[9], eLobbyDistanceFilter);
    }
    virtual void AddRequestLobbyListResultCountFilter(std::int32_t cMaxResults) {
        steambridge::slot<void>(kSlots_SteamMatchMaking009[10], cMaxResults);
    }
    virtual void AddRequestLobbyListCompatibleMembersFilter(CSteamID steamIDLobby) {
        steambridge::slot<void>(kSlots_SteamMatchMaking009[11], steamIDLobby);
    }
    virtual CSteamID GetLobbyByIndex(std::int32_t iLobby) {
        return steambridge::slot<CSteamID>(kSlots_SteamMatchMaking009[12], iLobby);
    }
    virtual std::uint64_t CreateLobby(std::int32_t eLobbyType, std::int32_t cMaxMembers) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamMatchMaking009[13], eLobbyType, cMaxMembers);
    }
    virtual std::uint64_t JoinLobby(CSteamID steamIDLobby) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamMatchMaking009[14], steamIDLobby);
    }
    virtual void LeaveLobby(CSteamID steamIDLobby) {
        steambridge::slot<void>(kSlots_SteamMatchMaking009[15], steamIDLobby);
    }
    virtual bool InviteUserToLobby(CSteamID steamIDLobby, CSteamID steamIDInvitee) {
        return steambridge::slot<bool>(kSlots_SteamMatchMaking009[16], steamIDLobby, steamIDInvitee);
    }
    virtual std::int32_t GetNumLobbyMembers(CSteamID steamIDLobby) {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchMaking009[17], steamIDLobby);
    }
    virtual CSteamID GetLobbyMemberByIndex(CSteamID steamIDLobby, std::int32_t iMember) {
        return steambridge::slot<CSteamID>(kSlots_SteamMatchMaking009[18], steamIDLobby, iMember);
    }
    virtual const char* GetLobbyData(CSteamID steamIDLobby, const char* pchKey) {
        return steambridge::slot<const char*>(kSlots_SteamMatchMaking009[19], steamIDLobby, pchKey);
    }
    virtual bool SetLobbyData(CSteamID steamIDLobby, const char* pchKey, const char* pchValue) {
        return steambridge::slot<bool>(kSlots_SteamMatchMaking009[20], steamIDLobby, pchKey, pchValue);
    }
    virtual std::int32_t GetLobbyDataCount(CSteamID steamIDLobby) {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchMaking009[21], steamIDLobby);
    }
    virtual bool GetLobbyDataByIndex(CSteamID steamIDLobby, std::int32_t iLobbyData, void* pchKey, std::int32_t cchKeyBufferSize, void* pchValue, std::int32_t cchValueBufferSize) {
        return steambridge::slot<bool>(kSlots_SteamMatchMaking009[22], steamIDLobby, iLobbyData, pchKey, cchKeyBufferSize, pchValue, cchValueBufferSize);
    }
    virtual bool DeleteLobbyData(CSteamID steamIDLobby, const char* pchKey) {
        return steambridge::slot<bool>(kSlots_SteamMatchMaking009[23], steamIDLobby, pchKey);
    }
    virtual const char* GetLobbyMemberData(CSteamID steamIDLobby, CSteamID steamIDUser, const char* pchKey) {
        return steambridge::slot<const char*>(kSlots_SteamMatchMaking009[24], steamIDLobby, steamIDUser, pchKey);
    }
    virtual void SetLobbyMemberData(CSteamID steamIDLobby, const char* pchKey, const char* pchValue) {
        steambridge::slot<void>(kSlots_SteamMatchMaking009[25], steamIDLobby, pchKey, pchValue);
    }
    virtual bool SendLobbyChatMsg(CSteamID steamIDLobby, void* pvMsgBody, std::int32_t cubMsgBody) {
        return steambridge::slot<bool>(kSlots_SteamMatchMaking009[26], steamIDLobby, pvMsgBody, cubMsgBody);
    }
    virtual std::int32_t GetLobbyChatEntry(CSteamID steamIDLobby, std::int32_t iChatID, CSteamID* pSteamIDUser, void* pvData, std::int32_t cubData, std::int32_t* peChatEntryType) {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchMaking009[27], steamIDLobby, iChatID, pSteamIDUser, pvData, cubData, peChatEntryType);
    }
    virtual bool RequestLobbyData(CSteamID steamIDLobby) {
        return steambridge::slot<bool>(kSlots_SteamMatchMaking009[28], steamIDLobby);
    }
    virtual void SetLobbyGameServer(CSteamID steamIDLobby, std::uint32_t unGameServerIP, std::uint16_t unGameServerPort, CSteamID steamIDGameServer) {
        steambridge::slot<void>(kSlots_SteamMatchMaking009[29], steamIDLobby, unGameServerIP, unGameServerPort, steamIDGameServer);
    }
    virtual bool GetLobbyGameServer(CSteamID steamIDLobby, std::uint32_t* punGameServerIP, std::uint16_t* punGameServerPort, CSteamID* psteamIDGameServer) {
        return steambridge::slot<bool>(kSlots_SteamMatchMaking009[30], steamIDLobby, punGameServerIP, punGameServerPort, psteamIDGameServer);
    }
    virtual bool SetLobbyMemberLimit(CSteamID steamIDLobby, std::int32_t cMaxMembers) {
        return steambridge::slot<bool>(kSlots_SteamMatchMaking009[31], steamIDLobby, cMaxMembers);
    }
    virtual std::int32_t GetLobbyMemberLimit(CSteamID steamIDLobby) {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchMaking009[32], steamIDLobby);
    }
    virtual bool SetLobbyType(CSteamID steamIDLobby, std::int32_t eLobbyType) {
        return steambridge::slot<bool>(kSlots_SteamMatchMaking009[33], steamIDLobby, eLobbyType);
    }
    virtual bool SetLobbyJoinable(CSteamID steamIDLobby, bool bLobbyJoinable) {
        return steambridge::slot<bool>(kSlots_SteamMatchMaking009[34], steamIDLobby, bLobbyJoinable);
    }
    virtual CSteamID GetLobbyOwner(CSteamID steamIDLobby) {
        return steambridge::slot<CSteamID>(kSlots_SteamMatchMaking009[35], steamIDLobby);
    }
    virtual bool SetLobbyOwner(CSteamID steamIDLobby, CSteamID steamIDNewOwner) {
        return steambridge::slot<bool>(kSlots_SteamMatchMaking009[36], steamIDLobby, steamIDNewOwner);
    }
    virtual bool SetLinkedLobby(CSteamID steamIDLobby, CSteamID steamIDLobbyDependent) {
        return steambridge::slot<bool>(kSlots_SteamMatchMaking009[37], steamIDLobby, steamIDLobbyDependent);
    }
};

Version_SteamMatchMaking009 g_SteamMatchMaking009;

const char* const kParams_SteamMatchMakingServers002_0[] = {
    "iApp",
    "ppchFilters",
    "nFilters",
    "pRequestServersResponse",
};
const char* const kParams_SteamMatchMakingServers002_1[] = {
    "iApp",
    "pRequestServersResponse",
};
const char* const kParams_SteamMatchMakingServers002_2[] = {
    "iApp",
    "ppchFilters",
    "nFilters",
    "pRequestServersResponse",
};
const char* const kParams_SteamMatchMakingServers002_3[] = {
    "iApp",
    "ppchFilters",
    "nFilters",
    "pRequestServersResponse",
};
const char* const kParams_SteamMatchMakingServers002_4[] = {
    "iApp",
    "ppchFilters",
    "nFilters",
    "pRequestServersResponse",
};
const char* const kParams_SteamMatchMakingServers002_5[] = {
    "iApp",
    "ppchFilters",
    "nFilters",
    "pRequestServersResponse",
};
const char* const kParams_SteamMatchMakingServers002_6[] = {
    "hServerListRequest",
};
const char* const kParams_SteamMatchMakingServers002_7[] = {
    "hRequest",
    "iServer",
};
const char* const kParams_SteamMatchMakingServers002_8[] = {
    "hRequest",
};
const char* const kParams_SteamMatchMakingServers002_9[] = {
    "hRequest",
};
const char* const kParams_SteamMatchMakingServers002_10[] = {
    "hRequest",
};
const char* const kParams_SteamMatchMakingServers002_11[] = {
    "hRequest",
};
const char* const kParams_SteamMatchMakingServers002_12[] = {
    "hRequest",
    "iServer",
};
const char* const kParams_SteamMatchMakingServers002_13[] = {
    "unIP",
    "usPort",
    "pRequestServersResponse",
};
const char* const kParams_SteamMatchMakingServers002_14[] = {
    "unIP",
    "usPort",
    "pRequestServersResponse",
};
const char* const kParams_SteamMatchMakingServers002_15[] = {
    "unIP",
    "usPort",
    "pRequestServersResponse",
};
const char* const kParams_SteamMatchMakingServers002_16[] = {
    "hServerQuery",
};
// ISteamMatchmakingServers SteamMatchMakingServers002
const steambridge::SlotInfo kSlots_SteamMatchMakingServers002[] = {
    {"SteamAPI_ISteamMatchmakingServers_RequestInternetServerList", kParams_SteamMatchMakingServers002_0},
    {"SteamAPI_ISteamMatchmakingServers_RequestLANServerList", kParams_SteamMatchMakingServers002_1},
    {"SteamAPI_ISteamMatchmakingServers_RequestFriendsServerList", kParams_SteamMatchMakingServers002_2},
    {"SteamAPI_ISteamMatchmakingServers_RequestFavoritesServerList", kParams_SteamMatchMakingServers002_3},
    {"SteamAPI_ISteamMatchmakingServers_RequestHistoryServerList", kParams_SteamMatchMakingServers002_4},
    {"SteamAPI_ISteamMatchmakingServers_RequestSpectatorServerList", kParams_SteamMatchMakingServers002_5},
    {"SteamAPI_ISteamMatchmakingServers_ReleaseRequest", kParams_SteamMatchMakingServers002_6},
    {"SteamAPI_ISteamMatchmakingServers_GetServerDetails", kParams_SteamMatchMakingServers002_7},
    {"SteamAPI_ISteamMatchmakingServers_CancelQuery", kParams_SteamMatchMakingServers002_8},
    {"SteamAPI_ISteamMatchmakingServers_RefreshQuery", kParams_SteamMatchMakingServers002_9},
    {"SteamAPI_ISteamMatchmakingServers_IsRefreshing", kParams_SteamMatchMakingServers002_10},
    {"SteamAPI_ISteamMatchmakingServers_GetServerCount", kParams_SteamMatchMakingServers002_11},
    {"SteamAPI_ISteamMatchmakingServers_RefreshServer", kParams_SteamMatchMakingServers002_12},
    {"SteamAPI_ISteamMatchmakingServers_PingServer", kParams_SteamMatchMakingServers002_13},
    {"SteamAPI_ISteamMatchmakingServers_PlayerDetails", kParams_SteamMatchMakingServers002_14},
    {"SteamAPI_ISteamMatchmakingServers_ServerRules", kParams_SteamMatchMakingServers002_15},
    {"SteamAPI_ISteamMatchmakingServers_CancelServerQuery", kParams_SteamMatchMakingServers002_16},
};

class Version_SteamMatchMakingServers002 {
public:
    virtual void* RequestInternetServerList(std::uint32_t iApp, void* ppchFilters, std::uint32_t nFilters, void* pRequestServersResponse) {
        return steambridge::slot<void*>(kSlots_SteamMatchMakingServers002[0], iApp, ppchFilters, nFilters, pRequestServersResponse);
    }
    virtual void* RequestLANServerList(std::uint32_t iApp, void* pRequestServersResponse) {
        return steambridge::slot<void*>(kSlots_SteamMatchMakingServers002[1], iApp, pRequestServersResponse);
    }
    virtual void* RequestFriendsServerList(std::uint32_t iApp, void* ppchFilters, std::uint32_t nFilters, void* pRequestServersResponse) {
        return steambridge::slot<void*>(kSlots_SteamMatchMakingServers002[2], iApp, ppchFilters, nFilters, pRequestServersResponse);
    }
    virtual void* RequestFavoritesServerList(std::uint32_t iApp, void* ppchFilters, std::uint32_t nFilters, void* pRequestServersResponse) {
        return steambridge::slot<void*>(kSlots_SteamMatchMakingServers002[3], iApp, ppchFilters, nFilters, pRequestServersResponse);
    }
    virtual void* RequestHistoryServerList(std::uint32_t iApp, void* ppchFilters, std::uint32_t nFilters, void* pRequestServersResponse) {
        return steambridge::slot<void*>(kSlots_SteamMatchMakingServers002[4], iApp, ppchFilters, nFilters, pRequestServersResponse);
    }
    virtual void* RequestSpectatorServerList(std::uint32_t iApp, void* ppchFilters, std::uint32_t nFilters, void* pRequestServersResponse) {
        return steambridge::slot<void*>(kSlots_SteamMatchMakingServers002[5], iApp, ppchFilters, nFilters, pRequestServersResponse);
    }
    virtual void ReleaseRequest(void* hServerListRequest) {
        steambridge::slot<void>(kSlots_SteamMatchMakingServers002[6], hServerListRequest);
    }
    virtual void* GetServerDetails(void* hRequest, std::int32_t iServer) {
        return steambridge::slot<void*>(kSlots_SteamMatchMakingServers002[7], hRequest, iServer);
    }
    virtual void CancelQuery(void* hRequest) {
        steambridge::slot<void>(kSlots_SteamMatchMakingServers002[8], hRequest);
    }
    virtual void RefreshQuery(void* hRequest) {
        steambridge::slot<void>(kSlots_SteamMatchMakingServers002[9], hRequest);
    }
    virtual bool IsRefreshing(void* hRequest) {
        return steambridge::slot<bool>(kSlots_SteamMatchMakingServers002[10], hRequest);
    }
    virtual std::int32_t GetServerCount(void* hRequest) {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchMakingServers002[11], hRequest);
    }
    virtual void RefreshServer(void* hRequest, std::int32_t iServer) {
        steambridge::slot<void>(kSlots_SteamMatchMakingServers002[12], hRequest, iServer);
    }
    virtual std::int32_t PingServer(std::uint32_t unIP, std::uint16_t usPort, void* pRequestServersResponse) {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchMakingServers002[13], unIP, usPort, pRequestServersResponse);
    }
    virtual std::int32_t PlayerDetails(std::uint32_t unIP, std::uint16_t usPort, void* pRequestServersResponse) {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchMakingServers002[14], unIP, usPort, pRequestServersResponse);
    }
    virtual std::int32_t ServerRules(std::uint32_t unIP, std::uint16_t usPort, void* pRequestServersResponse) {
        return steambridge::slot<std::int32_t>(kSlots_SteamMatchMakingServers002[15], unIP, usPort, pRequestServersResponse);
    }
    virtual void CancelServerQuery(std::int32_t hServerQuery) {
        steambridge::slot<void>(kSlots_SteamMatchMakingServers002[16], hServerQuery);
    }
};

Version_SteamMatchMakingServers002 g_SteamMatchMakingServers002;

const char* const kParams_STEAMMUSIC_INTERFACE_VERSION001_7[] = {
    "flVolume",
};
// ISteamMusic STEAMMUSIC_INTERFACE_VERSION001
const steambridge::SlotInfo kSlots_STEAMMUSIC_INTERFACE_VERSION001[] = {
    {"SteamAPI_ISteamMusic_BIsEnabled", nullptr},
    {"SteamAPI_ISteamMusic_BIsPlaying", nullptr},
    {"SteamAPI_ISteamMusic_GetPlaybackStatus", nullptr},
    {"SteamAPI_ISteamMusic_Play", nullptr},
    {"SteamAPI_ISteamMusic_Pause", nullptr},
    {"SteamAPI_ISteamMusic_PlayPrevious", nullptr},
    {"SteamAPI_ISteamMusic_PlayNext", nullptr},
    {"SteamAPI_ISteamMusic_SetVolume", kParams_STEAMMUSIC_INTERFACE_VERSION001_7},
    {"SteamAPI_ISteamMusic_GetVolume", nullptr},
};

class Version_STEAMMUSIC_INTERFACE_VERSION001 {
public:
    virtual bool BIsEnabled() {
        return steambridge::slot<bool>(kSlots_STEAMMUSIC_INTERFACE_VERSION001[0]);
    }
    virtual bool BIsPlaying() {
        return steambridge::slot<bool>(kSlots_STEAMMUSIC_INTERFACE_VERSION001[1]);
    }
    virtual std::int32_t GetPlaybackStatus() {
        return steambridge::slot<std::int32_t>(kSlots_STEAMMUSIC_INTERFACE_VERSION001[2]);
    }
    virtual void Play() {
        steambridge::slot<void>(kSlots_STEAMMUSIC_INTERFACE_VERSION001[3]);
    }
    virtual void Pause() {
        steambridge::slot<void>(kSlots_STEAMMUSIC_INTERFACE_VERSION001[4]);
    }
    virtual void PlayPrevious() {
        steambridge::slot<void>(kSlots_STEAMMUSIC_INTERFACE_VERSION001[5]);
    }
    virtual void PlayNext() {
        steambridge::slot<void>(kSlots_STEAMMUSIC_INTERFACE_VERSION001[6]);
    }
    virtual void SetVolume(float flVolume) {
        steambridge::slot<void>(kSlots_STEAMMUSIC_INTERFACE_VERSION001[7], flVolume);
    }
    virtual float GetVolume() {
        return steambridge::slot<float>(kSlots_STEAMMUSIC_INTERFACE_VERSION001[8]);
    }
};

Version_STEAMMUSIC_INTERFACE_VERSION001 g_STEAMMUSIC_INTERFACE_VERSION001;

const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_0[] = {
    "pchName",
};
const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_3[] = {
    "bValue",
};
const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_4[] = {
    "pchDisplayName",
};
const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_5[] = {
    "pvBuffer",
    "cbBufferLength",
};
const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_6[] = {
    "bValue",
};
const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_7[] = {
    "bValue",
};
const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_8[] = {
    "bValue",
};
const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_9[] = {
    "bValue",
};
const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_10[] = {
    "bValue",
};
const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_11[] = {
    "bValue",
};
const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_12[] = {
    "nStatus",
};
const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_13[] = {
    "bValue",
};
const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_14[] = {
    "bValue",
};
const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_15[] = {
    "flValue",
};
const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_17[] = {
    "bAvailable",
};
const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_18[] = {
    "pchText",
};
const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_19[] = {
    "nValue",
};
const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_20[] = {
    "pvBuffer",
    "cbBufferLength",
};
const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_24[] = {
    "nID",
    "nPosition",
    "pchEntryText",
};
const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_25[] = {
    "nID",
};
const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_29[] = {
    "nID",
    "nPosition",
    "pchEntryText",
};
const char* const kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_30[] = {
    "nID",
};
// ISteamMusicRemote STEAMMUSICREMOTE_INTERFACE_VERSION001
const steambridge::SlotInfo kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[] = {
    {"SteamAPI_ISteamMusicRemote_RegisterSteamMusicRemote", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_0},
    {"SteamAPI_ISteamMusicRemote_DeregisterSteamMusicRemote", nullptr},
    {"SteamAPI_ISteamMusicRemote_BIsCurrentMusicRemote", nullptr},
    {"SteamAPI_ISteamMusicRemote_BActivationSuccess", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_3},
    {"SteamAPI_ISteamMusicRemote_SetDisplayName", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_4},
    {"SteamAPI_ISteamMusicRemote_SetPNGIcon_64x64", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_5},
    {"SteamAPI_ISteamMusicRemote_EnablePlayPrevious", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_6},
    {"SteamAPI_ISteamMusicRemote_EnablePlayNext", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_7},
    {"SteamAPI_ISteamMusicRemote_EnableShuffled", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_8},
    {"SteamAPI_ISteamMusicRemote_EnableLooped", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_9},
    {"SteamAPI_ISteamMusicRemote_EnableQueue", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_10},
    {"SteamAPI_ISteamMusicRemote_EnablePlaylists", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_11},
    {"SteamAPI_ISteamMusicRemote_UpdatePlaybackStatus", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_12},
    {"SteamAPI_ISteamMusicRemote_UpdateShuffled", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_13},
    {"SteamAPI_ISteamMusicRemote_UpdateLooped", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_14},
    {"SteamAPI_ISteamMusicRemote_UpdateVolume", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_15},
    {"SteamAPI_ISteamMusicRemote_CurrentEntryWillChange", nullptr},
    {"SteamAPI_ISteamMusicRemote_CurrentEntryIsAvailable", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_17},
    {"SteamAPI_ISteamMusicRemote_UpdateCurrentEntryText", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_18},
    {"SteamAPI_ISteamMusicRemote_UpdateCurrentEntryElapsedSeconds", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_19},
    {"SteamAPI_ISteamMusicRemote_UpdateCurrentEntryCoverArt", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_20},
    {"SteamAPI_ISteamMusicRemote_CurrentEntryDidChange", nullptr},
    {"SteamAPI_ISteamMusicRemote_QueueWillChange", nullptr},
    {"SteamAPI_ISteamMusicRemote_ResetQueueEntries", nullptr},
    {"SteamAPI_ISteamMusicRemote_SetQueueEntry", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_24},
    {"SteamAPI_ISteamMusicRemote_SetCurrentQueueEntry", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_25},
    {"SteamAPI_ISteamMusicRemote_QueueDidChange", nullptr},
    {"SteamAPI_ISteamMusicRemote_PlaylistWillChange", nullptr},
    {"SteamAPI_ISteamMusicRemote_ResetPlaylistEntries", nullptr},
    {"SteamAPI_ISteamMusicRemote_SetPlaylistEntry", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_29},
    {"SteamAPI_ISteamMusicRemote_SetCurrentPlaylistEntry", kParams_STEAMMUSICREMOTE_INTERFACE_VERSION001_30},
    {"SteamAPI_ISteamMusicRemote_PlaylistDidChange", nullptr},
};

class Version_STEAMMUSICREMOTE_INTERFACE_VERSION001 {
public:
    virtual bool RegisterSteamMusicRemote(const char* pchName) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[0], pchName);
    }
    virtual bool DeregisterSteamMusicRemote() {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[1]);
    }
    virtual bool BIsCurrentMusicRemote() {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[2]);
    }
    virtual bool BActivationSuccess(bool bValue) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[3], bValue);
    }
    virtual bool SetDisplayName(const char* pchDisplayName) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[4], pchDisplayName);
    }
    virtual bool SetPNGIcon_64x64(void* pvBuffer, std::uint32_t cbBufferLength) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[5], pvBuffer, cbBufferLength);
    }
    virtual bool EnablePlayPrevious(bool bValue) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[6], bValue);
    }
    virtual bool EnablePlayNext(bool bValue) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[7], bValue);
    }
    virtual bool EnableShuffled(bool bValue) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[8], bValue);
    }
    virtual bool EnableLooped(bool bValue) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[9], bValue);
    }
    virtual bool EnableQueue(bool bValue) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[10], bValue);
    }
    virtual bool EnablePlaylists(bool bValue) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[11], bValue);
    }
    virtual bool UpdatePlaybackStatus(std::int32_t nStatus) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[12], nStatus);
    }
    virtual bool UpdateShuffled(bool bValue) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[13], bValue);
    }
    virtual bool UpdateLooped(bool bValue) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[14], bValue);
    }
    virtual bool UpdateVolume(float flValue) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[15], flValue);
    }
    virtual bool CurrentEntryWillChange() {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[16]);
    }
    virtual bool CurrentEntryIsAvailable(bool bAvailable) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[17], bAvailable);
    }
    virtual bool UpdateCurrentEntryText(const char* pchText) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[18], pchText);
    }
    virtual bool UpdateCurrentEntryElapsedSeconds(std::int32_t nValue) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[19], nValue);
    }
    virtual bool UpdateCurrentEntryCoverArt(void* pvBuffer, std::uint32_t cbBufferLength) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[20], pvBuffer, cbBufferLength);
    }
    virtual bool CurrentEntryDidChange() {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[21]);
    }
    virtual bool QueueWillChange() {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[22]);
    }
    virtual bool ResetQueueEntries() {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[23]);
    }
    virtual bool SetQueueEntry(std::int32_t nID, std::int32_t nPosition, const char* pchEntryText) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[24], nID, nPosition, pchEntryText);
    }
    virtual bool SetCurrentQueueEntry(std::int32_t nID) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[25], nID);
    }
    virtual bool QueueDidChange() {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[26]);
    }
    virtual bool PlaylistWillChange() {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[27]);
    }
    virtual bool ResetPlaylistEntries() {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[28]);
    }
    virtual bool SetPlaylistEntry(std::int32_t nID, std::int32_t nPosition, const char* pchEntryText) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[29], nID, nPosition, pchEntryText);
    }
    virtual bool SetCurrentPlaylistEntry(std::int32_t nID) {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[30], nID);
    }
    virtual bool PlaylistDidChange() {
        return steambridge::slot<bool>(kSlots_STEAMMUSICREMOTE_INTERFACE_VERSION001[31]);
    }
};

Version_STEAMMUSICREMOTE_INTERFACE_VERSION001 g_STEAMMUSICREMOTE_INTERFACE_VERSION001;

const char* const kParams_SteamNetworking005_0[] = {
    "steamIDRemote",
    "pubData",
    "cubData",
    "eP2PSendType",
    "nChannel",
};
const char* const kParams_SteamNetworking005_1[] = {
    "pcubMsgSize",
    "nChannel",
};
const char* const kParams_SteamNetworking005_2[] = {
    "pubDest",
    "cubDest",
    "pcubMsgSize",
    "psteamIDRemote",
    "nChannel",
};
const char* const kParams_SteamNetworking005_3[] = {
    "steamIDRemote",
};
const char* const kParams_SteamNetworking005_4[] = {
    "steamIDRemote",
};
const char* const kParams_SteamNetworking005_5[] = {
    "steamIDRemote",
    "nChannel",
};
const char* const kParams_SteamNetworking005_6[] = {
    "steamIDRemote",
    "pConnectionState",
};
const char* const kParams_SteamNetworking005_7[] = {
    "bAllow",
};
const char* const kParams_SteamNetworking005_8[] = {
    "nVirtualP2PPort",
    "nIP",
    "nPort",
    "bAllowUseOfPacketRelay",
};
const char* const kParams_SteamNetworking005_9[] = {
    "steamIDTarget",
    "nVirtualPort",
    "nTimeoutSec",
    "bAllowUseOfPacketRelay",
};
const char* const kParams_SteamNetworking005_10[] = {
    "nIP",
    "nPort",
    "nTimeoutSec",
};
const char* const kParams_SteamNetworking005_11[] = {
    "hSocket",
    "bNotifyRemoteEnd",
};
const char* const kParams_SteamNetworking005_12[] = {
    "hSocket",
    "bNotifyRemoteEnd",
};
const char* const kParams_SteamNetworking005_13[] = {
    "hSocket",
    "pubData",
    "cubData",
    "bReliable",
};
const char* const kParams_SteamNetworking005_14[] = {
    "hSocket",
    "pcubMsgSize",
};
const char* const kParams_SteamNetworking005_15[] = {
    "hSocket",
    "pubDest",
    "cubDest",
    "pcubMsgSize",
};
const char* const kParams_SteamNetworking005_16[] = {
    "hListenSocket",
    "pcubMsgSize",
    "phSocket",
};
const char* const kParams_SteamNetworking005_17[] = {
    "hListenSocket",
    "pubDest",
    "cubDest",
    "pcubMsgSize",
    "phSocket",
};
const char* const kParams_SteamNetworking005_18[] = {
    "hSocket",
    "pSteamIDRemote",
    "peSocketStatus",
    "punIPRemote",
    "punPortRemote",
};
const char* const kParams_SteamNetworking005_19[] = {
    "hListenSocket",
    "pnIP",
    "pnPort",
};
const char* const kParams_SteamNetworking005_20[] = {
    "hSocket",
};
const char* const kParams_SteamNetworking005_21[] = {
    "hSocket",
};
// ISteamNetworking SteamNetworking005
const steambridge::SlotInfo kSlots_SteamNetworking005[] = {
    {"SteamAPI_ISteamNetworking_SendP2PPacket", kParams_SteamNetworking005_0},
    {"SteamAPI_ISteamNetworking_IsP2PPacketAvailable", kParams_SteamNetworking005_1},
    {"SteamAPI_ISteamNetworking_ReadP2PPacket", kParams_SteamNetworking005_2},
    {"SteamAPI_ISteamNetworking_AcceptP2PSessionWithUser", kParams_SteamNetworking005_3},
    {"SteamAPI_ISteamNetworking_CloseP2PSessionWithUser", kParams_SteamNetworking005_4},
    {"SteamAPI_ISteamNetworking_CloseP2PChannelWithUser", kParams_SteamNetworking005_5},
    {"SteamAPI_ISteamNetworking_GetP2PSessionState", kParams_SteamNetworking005_6},
    {"SteamAPI_ISteamNetworking_AllowP2PPacketRelay", kParams_SteamNetworking005_7},
    {"SteamAPI_ISteamNetworking_CreateListenSocket", kParams_SteamNetworking005_8},
    {"SteamAPI_ISteamNetworking_CreateP2PConnectionSocket", kParams_SteamNetworking005_9},
    {"SteamAPI_ISteamNetworking_CreateConnectionSocket", kParams_SteamNetworking005_10},
    {"SteamAPI_ISteamNetworking_DestroySocket", kParams_SteamNetworking005_11},
    {"SteamAPI_ISteamNetworking_DestroyListenSocket", kParams_SteamNetworking005_12},
    {"SteamAPI_ISteamNetworking_SendDataOnSocket", kParams_SteamNetworking005_13},
    {"SteamAPI_ISteamNetworking_IsDataAvailableOnSocket", kParams_SteamNetworking005_14},
    {"SteamAPI_ISteamNetworking_RetrieveDataFromSocket", kParams_SteamNetworking005_15},
    {"SteamAPI_ISteamNetworking_IsDataAvailable", kParams_SteamNetworking005_16},
    {"SteamAPI_ISteamNetworking_RetrieveData", kParams_SteamNetworking005_17},
    {"SteamAPI_ISteamNetworking_GetSocketInfo", kParams_SteamNetworking005_18},
    {"SteamAPI_ISteamNetworking_GetListenSocketInfo", kParams_SteamNetworking005_19},
    {"SteamAPI_ISteamNetworking_GetSocketConnectionType", kParams_SteamNetworking005_20},
    {"SteamAPI_ISteamNetworking_GetMaxPacketSize", kParams_SteamNetworking005_21},
};

class Version_SteamNetworking005 {
public:
    virtual bool SendP2PPacket(CSteamID steamIDRemote, void* pubData, std::uint32_t cubData, std::int32_t eP2PSendType, std::int32_t nChannel) {
        return steambridge::slot<bool>(kSlots_SteamNetworking005[0], steamIDRemote, pubData, cubData, eP2PSendType, nChannel);
    }
    virtual bool IsP2PPacketAvailable(std::uint32_t* pcubMsgSize, std::int32_t nChannel) {
        return steambridge::slot<bool>(kSlots_SteamNetworking005[1], pcubMsgSize, nChannel);
    }
    virtual bool ReadP2PPacket(void* pubDest, std::uint32_t cubDest, std::uint32_t* pcubMsgSize, CSteamID* psteamIDRemote, std::int32_t nChannel) {
        return steambridge::slot<bool>(kSlots_SteamNetworking005[2], pubDest, cubDest, pcubMsgSize, psteamIDRemote, nChannel);
    }
    virtual bool AcceptP2PSessionWithUser(CSteamID steamIDRemote) {
        return steambridge::slot<bool>(kSlots_SteamNetworking005[3], steamIDRemote);
    }
    virtual bool CloseP2PSessionWithUser(CSteamID steamIDRemote) {
        return steambridge::slot<bool>(kSlots_SteamNetworking005[4], steamIDRemote);
    }
    virtual bool CloseP2PChannelWithUser(CSteamID steamIDRemote, std::int32_t nChannel) {
        return steambridge::slot<bool>(kSlots_SteamNetworking005[5], steamIDRemote, nChannel);
    }
    virtual bool GetP2PSessionState(CSteamID steamIDRemote, void* pConnectionState) {
        return steambridge::slot<bool>(kSlots_SteamNetworking005[6], steamIDRemote, pConnectionState);
    }
    virtual bool AllowP2PPacketRelay(bool bAllow) {
        return steambridge::slot<bool>(kSlots_SteamNetworking005[7], bAllow);
    }
    virtual std::uint32_t CreateListenSocket(std::int32_t nVirtualP2PPort, std::uint32_t nIP, std::uint16_t nPort, bool bAllowUseOfPacketRelay) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworking005[8], nVirtualP2PPort, nIP, nPort, bAllowUseOfPacketRelay);
    }
    virtual std::uint32_t CreateP2PConnectionSocket(CSteamID steamIDTarget, std::int32_t nVirtualPort, std::int32_t nTimeoutSec, bool bAllowUseOfPacketRelay) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworking005[9], steamIDTarget, nVirtualPort, nTimeoutSec, bAllowUseOfPacketRelay);
    }
    virtual std::uint32_t CreateConnectionSocket(std::uint32_t nIP, std::uint16_t nPort, std::int32_t nTimeoutSec) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworking005[10], nIP, nPort, nTimeoutSec);
    }
    virtual bool DestroySocket(std::uint32_t hSocket, bool bNotifyRemoteEnd) {
        return steambridge::slot<bool>(kSlots_SteamNetworking005[11], hSocket, bNotifyRemoteEnd);
    }
    virtual bool DestroyListenSocket(std::uint32_t hSocket, bool bNotifyRemoteEnd) {
        return steambridge::slot<bool>(kSlots_SteamNetworking005[12], hSocket, bNotifyRemoteEnd);
    }
    virtual bool SendDataOnSocket(std::uint32_t hSocket, void* pubData, std::uint32_t cubData, bool bReliable) {
        return steambridge::slot<bool>(kSlots_SteamNetworking005[13], hSocket, pubData, cubData, bReliable);
    }
    virtual bool IsDataAvailableOnSocket(std::uint32_t hSocket, std::uint32_t* pcubMsgSize) {
        return steambridge::slot<bool>(kSlots_SteamNetworking005[14], hSocket, pcubMsgSize);
    }
    virtual bool RetrieveDataFromSocket(std::uint32_t hSocket, void* pubDest, std::uint32_t cubDest, std::uint32_t* pcubMsgSize) {
        return steambridge::slot<bool>(kSlots_SteamNetworking005[15], hSocket, pubDest, cubDest, pcubMsgSize);
    }
    virtual bool IsDataAvailable(std::uint32_t hListenSocket, std::uint32_t* pcubMsgSize, std::uint32_t* phSocket) {
        return steambridge::slot<bool>(kSlots_SteamNetworking005[16], hListenSocket, pcubMsgSize, phSocket);
    }
    virtual bool RetrieveData(std::uint32_t hListenSocket, void* pubDest, std::uint32_t cubDest, std::uint32_t* pcubMsgSize, std::uint32_t* phSocket) {
        return steambridge::slot<bool>(kSlots_SteamNetworking005[17], hListenSocket, pubDest, cubDest, pcubMsgSize, phSocket);
    }
    virtual bool GetSocketInfo(std::uint32_t hSocket, CSteamID* pSteamIDRemote, std::int32_t* peSocketStatus, std::uint32_t* punIPRemote, std::uint16_t* punPortRemote) {
        return steambridge::slot<bool>(kSlots_SteamNetworking005[18], hSocket, pSteamIDRemote, peSocketStatus, punIPRemote, punPortRemote);
    }
    virtual bool GetListenSocketInfo(std::uint32_t hListenSocket, std::uint32_t* pnIP, std::uint16_t* pnPort) {
        return steambridge::slot<bool>(kSlots_SteamNetworking005[19], hListenSocket, pnIP, pnPort);
    }
    virtual std::int32_t GetSocketConnectionType(std::uint32_t hSocket) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworking005[20], hSocket);
    }
    virtual std::int32_t GetMaxPacketSize(std::uint32_t hSocket) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworking005[21], hSocket);
    }
};

Version_SteamNetworking005 g_SteamNetworking005;

const char* const kParams_SteamNetworking006_0[] = {
    "steamIDRemote",
    "pubData",
    "cubData",
    "eP2PSendType",
    "nChannel",
};
const char* const kParams_SteamNetworking006_1[] = {
    "pcubMsgSize",
    "nChannel",
};
const char* const kParams_SteamNetworking006_2[] = {
    "pubDest",
    "cubDest",
    "pcubMsgSize",
    "psteamIDRemote",
    "nChannel",
};
const char* const kParams_SteamNetworking006_3[] = {
    "steamIDRemote",
};
const char* const kParams_SteamNetworking006_4[] = {
    "steamIDRemote",
};
const char* const kParams_SteamNetworking006_5[] = {
    "steamIDRemote",
    "nChannel",
};
const char* const kParams_SteamNetworking006_6[] = {
    "steamIDRemote",
    "pConnectionState",
};
const char* const kParams_SteamNetworking006_7[] = {
    "bAllow",
};
const char* const kParams_SteamNetworking006_8[] = {
    "nVirtualP2PPort",
    "nIP",
    "nPort",
    "bAllowUseOfPacketRelay",
};
const char* const kParams_SteamNetworking006_9[] = {
    "steamIDTarget",
    "nVirtualPort",
    "nTimeoutSec",
    "bAllowUseOfPacketRelay",
};
const char* const kParams_SteamNetworking006_10[] = {
    "nIP",
    "nPort",
    "nTimeoutSec",
};
const char* const kParams_SteamNetworking006_11[] = {
    "hSocket",
    "bNotifyRemoteEnd",
};
const char* const kParams_SteamNetworking006_12[] = {
    "hSocket",
    "bNotifyRemoteEnd",
};
const char* const kParams_SteamNetworking006_13[] = {
    "hSocket",
    "pubData",
    "cubData",
    "bReliable",
};
const char* const kParams_SteamNetworking006_14[] = {
    "hSocket",
    "pcubMsgSize",
};
const char* const kParams_SteamNetworking006_15[] = {
    "hSocket",
    "pubDest",
    "cubDest",
    "pcubMsgSize",
};
const char* const kParams_SteamNetworking006_16[] = {
    "hListenSocket",
    "pcubMsgSize",
    "phSocket",
};
const char* const kParams_SteamNetworking006_17[] = {
    "hListenSocket",
    "pubDest",
    "cubDest",
    "pcubMsgSize",
    "phSocket",
};
const char* const kParams_SteamNetworking006_18[] = {
    "hSocket",
    "pSteamIDRemote",
    "peSocketStatus",
    "punIPRemote",
    "punPortRemote",
};
const char* const kParams_SteamNetworking006_19[] = {
    "hListenSocket",
    "pnIP",
    "pnPort",
};
const char* const kParams_SteamNetworking006_20[] = {
    "hSocket",
};
const char* const kParams_SteamNetworking006_21[] = {
    "hSocket",
};
// ISteamNetworking SteamNetworking006
const steambridge::SlotInfo kSlots_SteamNetworking006[] = {
    {"SteamAPI_ISteamNetworking_SendP2PPacket", kParams_SteamNetworking006_0},
    {"SteamAPI_ISteamNetworking_IsP2PPacketAvailable", kParams_SteamNetworking006_1},
    {"SteamAPI_ISteamNetworking_ReadP2PPacket", kParams_SteamNetworking006_2},
    {"SteamAPI_ISteamNetworking_AcceptP2PSessionWithUser", kParams_SteamNetworking006_3},
    {"SteamAPI_ISteamNetworking_CloseP2PSessionWithUser", kParams_SteamNetworking006_4},
    {"SteamAPI_ISteamNetworking_CloseP2PChannelWithUser", kParams_SteamNetworking006_5},
    {"SteamAPI_ISteamNetworking_GetP2PSessionState", kParams_SteamNetworking006_6},
    {"SteamAPI_ISteamNetworking_AllowP2PPacketRelay", kParams_SteamNetworking006_7},
    {"SteamAPI_ISteamNetworking_CreateListenSocket", kParams_SteamNetworking006_8},
    {"SteamAPI_ISteamNetworking_CreateP2PConnectionSocket", kParams_SteamNetworking006_9},
    {"SteamAPI_ISteamNetworking_CreateConnectionSocket", kParams_SteamNetworking006_10},
    {"SteamAPI_ISteamNetworking_DestroySocket", kParams_SteamNetworking006_11},
    {"SteamAPI_ISteamNetworking_DestroyListenSocket", kParams_SteamNetworking006_12},
    {"SteamAPI_ISteamNetworking_SendDataOnSocket", kParams_SteamNetworking006_13},
    {"SteamAPI_ISteamNetworking_IsDataAvailableOnSocket", kParams_SteamNetworking006_14},
    {"SteamAPI_ISteamNetworking_RetrieveDataFromSocket", kParams_SteamNetworking006_15},
    {"SteamAPI_ISteamNetworking_IsDataAvailable", kParams_SteamNetworking006_16},
    {"SteamAPI_ISteamNetworking_RetrieveData", kParams_SteamNetworking006_17},
    {"SteamAPI_ISteamNetworking_GetSocketInfo", kParams_SteamNetworking006_18},
    {"SteamAPI_ISteamNetworking_GetListenSocketInfo", kParams_SteamNetworking006_19},
    {"SteamAPI_ISteamNetworking_GetSocketConnectionType", kParams_SteamNetworking006_20},
    {"SteamAPI_ISteamNetworking_GetMaxPacketSize", kParams_SteamNetworking006_21},
};

class Version_SteamNetworking006 {
public:
    virtual bool SendP2PPacket(CSteamID steamIDRemote, void* pubData, std::uint32_t cubData, std::int32_t eP2PSendType, std::int32_t nChannel) {
        return steambridge::slot<bool>(kSlots_SteamNetworking006[0], steamIDRemote, pubData, cubData, eP2PSendType, nChannel);
    }
    virtual bool IsP2PPacketAvailable(std::uint32_t* pcubMsgSize, std::int32_t nChannel) {
        return steambridge::slot<bool>(kSlots_SteamNetworking006[1], pcubMsgSize, nChannel);
    }
    virtual bool ReadP2PPacket(void* pubDest, std::uint32_t cubDest, std::uint32_t* pcubMsgSize, CSteamID* psteamIDRemote, std::int32_t nChannel) {
        return steambridge::slot<bool>(kSlots_SteamNetworking006[2], pubDest, cubDest, pcubMsgSize, psteamIDRemote, nChannel);
    }
    virtual bool AcceptP2PSessionWithUser(CSteamID steamIDRemote) {
        return steambridge::slot<bool>(kSlots_SteamNetworking006[3], steamIDRemote);
    }
    virtual bool CloseP2PSessionWithUser(CSteamID steamIDRemote) {
        return steambridge::slot<bool>(kSlots_SteamNetworking006[4], steamIDRemote);
    }
    virtual bool CloseP2PChannelWithUser(CSteamID steamIDRemote, std::int32_t nChannel) {
        return steambridge::slot<bool>(kSlots_SteamNetworking006[5], steamIDRemote, nChannel);
    }
    virtual bool GetP2PSessionState(CSteamID steamIDRemote, void* pConnectionState) {
        return steambridge::slot<bool>(kSlots_SteamNetworking006[6], steamIDRemote, pConnectionState);
    }
    virtual bool AllowP2PPacketRelay(bool bAllow) {
        return steambridge::slot<bool>(kSlots_SteamNetworking006[7], bAllow);
    }
    virtual std::uint32_t CreateListenSocket(std::int32_t nVirtualP2PPort, SteamIPAddress_t nIP, std::uint16_t nPort, bool bAllowUseOfPacketRelay) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworking006[8], nVirtualP2PPort, nIP, nPort, bAllowUseOfPacketRelay);
    }
    virtual std::uint32_t CreateP2PConnectionSocket(CSteamID steamIDTarget, std::int32_t nVirtualPort, std::int32_t nTimeoutSec, bool bAllowUseOfPacketRelay) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworking006[9], steamIDTarget, nVirtualPort, nTimeoutSec, bAllowUseOfPacketRelay);
    }
    virtual std::uint32_t CreateConnectionSocket(SteamIPAddress_t nIP, std::uint16_t nPort, std::int32_t nTimeoutSec) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworking006[10], nIP, nPort, nTimeoutSec);
    }
    virtual bool DestroySocket(std::uint32_t hSocket, bool bNotifyRemoteEnd) {
        return steambridge::slot<bool>(kSlots_SteamNetworking006[11], hSocket, bNotifyRemoteEnd);
    }
    virtual bool DestroyListenSocket(std::uint32_t hSocket, bool bNotifyRemoteEnd) {
        return steambridge::slot<bool>(kSlots_SteamNetworking006[12], hSocket, bNotifyRemoteEnd);
    }
    virtual bool SendDataOnSocket(std::uint32_t hSocket, void* pubData, std::uint32_t cubData, bool bReliable) {
        return steambridge::slot<bool>(kSlots_SteamNetworking006[13], hSocket, pubData, cubData, bReliable);
    }
    virtual bool IsDataAvailableOnSocket(std::uint32_t hSocket, std::uint32_t* pcubMsgSize) {
        return steambridge::slot<bool>(kSlots_SteamNetworking006[14], hSocket, pcubMsgSize);
    }
    virtual bool RetrieveDataFromSocket(std::uint32_t hSocket, void* pubDest, std::uint32_t cubDest, std::uint32_t* pcubMsgSize) {
        return steambridge::slot<bool>(kSlots_SteamNetworking006[15], hSocket, pubDest, cubDest, pcubMsgSize);
    }
    virtual bool IsDataAvailable(std::uint32_t hListenSocket, std::uint32_t* pcubMsgSize, std::uint32_t* phSocket) {
        return steambridge::slot<bool>(kSlots_SteamNetworking006[16], hListenSocket, pcubMsgSize, phSocket);
    }
    virtual bool RetrieveData(std::uint32_t hListenSocket, void* pubDest, std::uint32_t cubDest, std::uint32_t* pcubMsgSize, std::uint32_t* phSocket) {
        return steambridge::slot<bool>(kSlots_SteamNetworking006[17], hListenSocket, pubDest, cubDest, pcubMsgSize, phSocket);
    }
    virtual bool GetSocketInfo(std::uint32_t hSocket, CSteamID* pSteamIDRemote, std::int32_t* peSocketStatus, void* punIPRemote, std::uint16_t* punPortRemote) {
        return steambridge::slot<bool>(kSlots_SteamNetworking006[18], hSocket, pSteamIDRemote, peSocketStatus, punIPRemote, punPortRemote);
    }
    virtual bool GetListenSocketInfo(std::uint32_t hListenSocket, void* pnIP, std::uint16_t* pnPort) {
        return steambridge::slot<bool>(kSlots_SteamNetworking006[19], hListenSocket, pnIP, pnPort);
    }
    virtual std::int32_t GetSocketConnectionType(std::uint32_t hSocket) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworking006[20], hSocket);
    }
    virtual std::int32_t GetMaxPacketSize(std::uint32_t hSocket) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworking006[21], hSocket);
    }
};

Version_SteamNetworking006 g_SteamNetworking006;

const char* const kParams_SteamNetworkingMessages002_0[] = {
    "identityRemote",
    "pubData",
    "cubData",
    "nSendFlags",
    "nRemoteChannel",
};
const char* const kParams_SteamNetworkingMessages002_1[] = {
    "nLocalChannel",
    "ppOutMessages",
    "nMaxMessages",
};
const char* const kParams_SteamNetworkingMessages002_2[] = {
    "identityRemote",
};
const char* const kParams_SteamNetworkingMessages002_3[] = {
    "identityRemote",
};
const char* const kParams_SteamNetworkingMessages002_4[] = {
    "identityRemote",
    "nLocalChannel",
};
const char* const kParams_SteamNetworkingMessages002_5[] = {
    "identityRemote",
    "pConnectionInfo",
    "pQuickStatus",
};
// ISteamNetworkingMessages SteamNetworkingMessages002
const steambridge::SlotInfo kSlots_SteamNetworkingMessages002[] = {
    {"SteamAPI_ISteamNetworkingMessages_SendMessageToUser", kParams_SteamNetworkingMessages002_0},
    {"SteamAPI_ISteamNetworkingMessages_ReceiveMessagesOnChannel", kParams_SteamNetworkingMessages002_1},
    {"SteamAPI_ISteamNetworkingMessages_AcceptSessionWithUser", kParams_SteamNetworkingMessages002_2},
    {"SteamAPI_ISteamNetworkingMessages_CloseSessionWithUser", kParams_SteamNetworkingMessages002_3},
    {"SteamAPI_ISteamNetworkingMessages_CloseChannelWithUser", kParams_SteamNetworkingMessages002_4},
    {"SteamAPI_ISteamNetworkingMessages_GetSessionConnectionInfo", kParams_SteamNetworkingMessages002_5},
};

class Version_SteamNetworkingMessages002 {
public:
    virtual std::int32_t SendMessageToUser(void* identityRemote, void* pubData, std::uint32_t cubData, std::int32_t nSendFlags, std::int32_t nRemoteChannel) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingMessages002[0], identityRemote, pubData, cubData, nSendFlags, nRemoteChannel);
    }
    virtual std::int32_t ReceiveMessagesOnChannel(std::int32_t nLocalChannel, void* ppOutMessages, std::int32_t nMaxMessages) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingMessages002[1], nLocalChannel, ppOutMessages, nMaxMessages);
    }
    virtual bool AcceptSessionWithUser(void* identityRemote) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingMessages002[2], identityRemote);
    }
    virtual bool CloseSessionWithUser(void* identityRemote) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingMessages002[3], identityRemote);
    }
    virtual bool CloseChannelWithUser(void* identityRemote, std::int32_t nLocalChannel) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingMessages002[4], identityRemote, nLocalChannel);
    }
    virtual std::int32_t GetSessionConnectionInfo(void* identityRemote, void* pConnectionInfo, void* pQuickStatus) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingMessages002[5], identityRemote, pConnectionInfo, pQuickStatus);
    }
};

Version_SteamNetworkingMessages002 g_SteamNetworkingMessages002;

const char* const kParams_SteamNetworkingSockets009_0[] = {
    "localAddress",
    "nOptions",
    "pOptions",
};
const char* const kParams_SteamNetworkingSockets009_1[] = {
    "address",
    "nOptions",
    "pOptions",
};
const char* const kParams_SteamNetworkingSockets009_2[] = {
    "nLocalVirtualPort",
    "nOptions",
    "pOptions",
};
const char* const kParams_SteamNetworkingSockets009_3[] = {
    "identityRemote",
    "nRemoteVirtualPort",
    "nOptions",
    "pOptions",
};
const char* const kParams_SteamNetworkingSockets009_4[] = {
    "hConn",
};
const char* const kParams_SteamNetworkingSockets009_5[] = {
    "hPeer",
    "nReason",
    "pszDebug",
    "bEnableLinger",
};
const char* const kParams_SteamNetworkingSockets009_6[] = {
    "hSocket",
};
const char* const kParams_SteamNetworkingSockets009_7[] = {
    "hPeer",
    "nUserData",
};
const char* const kParams_SteamNetworkingSockets009_8[] = {
    "hPeer",
};
const char* const kParams_SteamNetworkingSockets009_9[] = {
    "hPeer",
    "pszName",
};
const char* const kParams_SteamNetworkingSockets009_10[] = {
    "hPeer",
    "pszName",
    "nMaxLen",
};
const char* const kParams_SteamNetworkingSockets009_11[] = {
    "hConn",
    "pData",
    "cbData",
    "nSendFlags",
    "pOutMessageNumber",
};
const char* const kParams_SteamNetworkingSockets009_12[] = {
    "nMessages",
    "pMessages",
    "pOutMessageNumberOrResult",
};
const char* const kParams_SteamNetworkingSockets009_13[] = {
    "hConn",
};
const char* const kParams_SteamNetworkingSockets009_14[] = {
    "hConn",
    "ppOutMessages",
    "nMaxMessages",
};
const char* const kParams_SteamNetworkingSockets009_15[] = {
    "hConn",
    "pInfo",
};
const char* const kParams_SteamNetworkingSockets009_16[] = {
    "hConn",
    "pStats",
};
const char* const kParams_SteamNetworkingSockets009_17[] = {
    "hConn",
    "pszBuf",
    "cbBuf",
};
const char* const kParams_SteamNetworkingSockets009_18[] = {
    "hSocket",
    "address",
};
const char* const kParams_SteamNetworkingSockets009_19[] = {
    "pOutConnection1",
    "pOutConnection2",
    "bUseNetworkLoopback",
    "pIdentity1",
    "pIdentity2",
};
const char* const kParams_SteamNetworkingSockets009_20[] = {
    "pIdentity",
};
const char* const kParams_SteamNetworkingSockets009_22[] = {
    "pDetails",
};
const char* const kParams_SteamNetworkingSockets009_24[] = {
    "hPollGroup",
};
const char* const kParams_SteamNetworkingSockets009_25[] = {
    "hConn",
    "hPollGroup",
};
const char* const kParams_SteamNetworkingSockets009_26[] = {
    "hPollGroup",
    "ppOutMessages",
    "nMaxMessages",
};
const char* const kParams_SteamNetworkingSockets009_27[] = {
    "pvTicket",
    "cbTicket",
    "pOutParsedTicket",
};
const char* const kParams_SteamNetworkingSockets009_28[] = {
    "identityGameServer",
    "nRemoteVirtualPort",
    "pOutParsedTicket",
};
const char* const kParams_SteamNetworkingSockets009_29[] = {
    "identityTarget",
    "nRemoteVirtualPort",
    "nOptions",
    "pOptions",
};
const char* const kParams_SteamNetworkingSockets009_32[] = {
    "pRouting",
};
const char* const kParams_SteamNetworkingSockets009_33[] = {
    "nLocalVirtualPort",
    "nOptions",
    "pOptions",
};
const char* const kParams_SteamNetworkingSockets009_34[] = {
    "pLoginInfo",
    "pcbSignedBlob",
    "pBlob",
};
const char* const kParams_SteamNetworkingSockets009_35[] = {
    "pSignaling",
    "pPeerIdentity",
    "nRemoteVirtualPort",
    "nOptions",
    "pOptions",
};
const char* const kParams_SteamNetworkingSockets009_36[] = {
    "pMsg",
    "cbMsg",
    "pContext",
};
const char* const kParams_SteamNetworkingSockets009_37[] = {
    "pcbBlob",
    "pBlob",
    "errMsg",
};
const char* const kParams_SteamNetworkingSockets009_38[] = {
    "pCertificate",
    "cbCertificate",
    "errMsg",
};
// ISteamNetworkingSockets SteamNetworkingSockets009
const steambridge::SlotInfo kSlots_SteamNetworkingSockets009[] = {
    {"SteamAPI_ISteamNetworkingSockets_CreateListenSocketIP", kParams_SteamNetworkingSockets009_0},
    {"SteamAPI_ISteamNetworkingSockets_ConnectByIPAddress", kParams_SteamNetworkingSockets009_1},
    {"SteamAPI_ISteamNetworkingSockets_CreateListenSocketP2P", kParams_SteamNetworkingSockets009_2},
    {"SteamAPI_ISteamNetworkingSockets_ConnectP2P", kParams_SteamNetworkingSockets009_3},
    {"SteamAPI_ISteamNetworkingSockets_AcceptConnection", kParams_SteamNetworkingSockets009_4},
    {"SteamAPI_ISteamNetworkingSockets_CloseConnection", kParams_SteamNetworkingSockets009_5},
    {"SteamAPI_ISteamNetworkingSockets_CloseListenSocket", kParams_SteamNetworkingSockets009_6},
    {"SteamAPI_ISteamNetworkingSockets_SetConnectionUserData", kParams_SteamNetworkingSockets009_7},
    {"SteamAPI_ISteamNetworkingSockets_GetConnectionUserData", kParams_SteamNetworkingSockets009_8},
    {"SteamAPI_ISteamNetworkingSockets_SetConnectionName", kParams_SteamNetworkingSockets009_9},
    {"SteamAPI_ISteamNetworkingSockets_GetConnectionName", kParams_SteamNetworkingSockets009_10},
    {"SteamAPI_ISteamNetworkingSockets_SendMessageToConnection", kParams_SteamNetworkingSockets009_11},
    {"SteamAPI_ISteamNetworkingSockets_SendMessages", kParams_SteamNetworkingSockets009_12},
    {"SteamAPI_ISteamNetworkingSockets_FlushMessagesOnConnection", kParams_SteamNetworkingSockets009_13},
    {"SteamAPI_ISteamNetworkingSockets_ReceiveMessagesOnConnection", kParams_SteamNetworkingSockets009_14},
    {"SteamAPI_ISteamNetworkingSockets_GetConnectionInfo", kParams_SteamNetworkingSockets009_15},
    {"SteamAPI_ISteamNetworkingSockets_GetQuickConnectionStatus", kParams_SteamNetworkingSockets009_16},
    {"SteamAPI_ISteamNetworkingSockets_GetDetailedConnectionStatus", kParams_SteamNetworkingSockets009_17},
    {"SteamAPI_ISteamNetworkingSockets_GetListenSocketAddress", kParams_SteamNetworkingSockets009_18},
    {"SteamAPI_ISteamNetworkingSockets_CreateSocketPair", kParams_SteamNetworkingSockets009_19},
    {"SteamAPI_ISteamNetworkingSockets_GetIdentity", kParams_SteamNetworkingSockets009_20},
    {"SteamAPI_ISteamNetworkingSockets_InitAuthentication", nullptr},
    {"SteamAPI_ISteamNetworkingSockets_GetAuthenticationStatus", kParams_SteamNetworkingSockets009_22},
    {"SteamAPI_ISteamNetworkingSockets_CreatePollGroup", nullptr},
    {"SteamAPI_ISteamNetworkingSockets_DestroyPollGroup", kParams_SteamNetworkingSockets009_24},
    {"SteamAPI_ISteamNetworkingSockets_SetConnectionPollGroup", kParams_SteamNetworkingSockets009_25},
    {"SteamAPI_ISteamNetworkingSockets_ReceiveMessagesOnPollGroup", kParams_SteamNetworkingSockets009_26},
    {"SteamAPI_ISteamNetworkingSockets_ReceivedRelayAuthTicket", kParams_SteamNetworkingSockets009_27},
    {"SteamAPI_ISteamNetworkingSockets_FindRelayAuthTicketForServer", kParams_SteamNetworkingSockets009_28},
    {"SteamAPI_ISteamNetworkingSockets_ConnectToHostedDedicatedServer", kParams_SteamNetworkingSockets009_29},
    {"SteamAPI_ISteamNetworkingSockets_GetHostedDedicatedServerPort", nullptr},
    {"SteamAPI_ISteamNetworkingSockets_GetHostedDedicatedServerPOPID", nullptr},
    {"SteamAPI_ISteamNetworkingSockets_GetHostedDedicatedServerAddress", kParams_SteamNetworkingSockets009_32},
    {"SteamAPI_ISteamNetworkingSockets_CreateHostedDedicatedServerListenSocket", kParams_SteamNetworkingSockets009_33},
    {"SteamAPI_ISteamNetworkingSockets_GetGameCoordinatorServerLogin", kParams_SteamNetworkingSockets009_34},
    {"SteamAPI_ISteamNetworkingSockets_ConnectP2PCustomSignaling", kParams_SteamNetworkingSockets009_35},
    {"SteamAPI_ISteamNetworkingSockets_ReceivedP2PCustomSignal", kParams_SteamNetworkingSockets009_36},
    {"SteamAPI_ISteamNetworkingSockets_GetCertificateRequest", kParams_SteamNetworkingSockets009_37},
    {"SteamAPI_ISteamNetworkingSockets_SetCertificate", kParams_SteamNetworkingSockets009_38},
    {"SteamAPI_ISteamNetworkingSockets_RunCallbacks", nullptr},
};

class Version_SteamNetworkingSockets009 {
public:
    virtual std::uint32_t CreateListenSocketIP(void* localAddress, std::int32_t nOptions, void* pOptions) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworkingSockets009[0], localAddress, nOptions, pOptions);
    }
    virtual std::uint32_t ConnectByIPAddress(void* address, std::int32_t nOptions, void* pOptions) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworkingSockets009[1], address, nOptions, pOptions);
    }
    virtual std::uint32_t CreateListenSocketP2P(std::int32_t nLocalVirtualPort, std::int32_t nOptions, void* pOptions) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworkingSockets009[2], nLocalVirtualPort, nOptions, pOptions);
    }
    virtual std::uint32_t ConnectP2P(void* identityRemote, std::int32_t nRemoteVirtualPort, std::int32_t nOptions, void* pOptions) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworkingSockets009[3], identityRemote, nRemoteVirtualPort, nOptions, pOptions);
    }
    virtual std::int32_t AcceptConnection(std::uint32_t hConn) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets009[4], hConn);
    }
    virtual bool CloseConnection(std::uint32_t hPeer, std::int32_t nReason, const char* pszDebug, bool bEnableLinger) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets009[5], hPeer, nReason, pszDebug, bEnableLinger);
    }
    virtual bool CloseListenSocket(std::uint32_t hSocket) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets009[6], hSocket);
    }
    virtual bool SetConnectionUserData(std::uint32_t hPeer, std::int64_t nUserData) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets009[7], hPeer, nUserData);
    }
    virtual std::int64_t GetConnectionUserData(std::uint32_t hPeer) {
        return steambridge::slot<std::int64_t>(kSlots_SteamNetworkingSockets009[8], hPeer);
    }
    virtual void SetConnectionName(std::uint32_t hPeer, const char* pszName) {
        steambridge::slot<void>(kSlots_SteamNetworkingSockets009[9], hPeer, pszName);
    }
    virtual bool GetConnectionName(std::uint32_t hPeer, void* pszName, std::int32_t nMaxLen) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets009[10], hPeer, pszName, nMaxLen);
    }
    virtual std::int32_t SendMessageToConnection(std::uint32_t hConn, void* pData, std::uint32_t cbData, std::int32_t nSendFlags, std::int64_t* pOutMessageNumber) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets009[11], hConn, pData, cbData, nSendFlags, pOutMessageNumber);
    }
    virtual void SendMessages(std::int32_t nMessages, void* pMessages, std::int64_t* pOutMessageNumberOrResult) {
        steambridge::slot<void>(kSlots_SteamNetworkingSockets009[12], nMessages, pMessages, pOutMessageNumberOrResult);
    }
    virtual std::int32_t FlushMessagesOnConnection(std::uint32_t hConn) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets009[13], hConn);
    }
    virtual std::int32_t ReceiveMessagesOnConnection(std::uint32_t hConn, void* ppOutMessages, std::int32_t nMaxMessages) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets009[14], hConn, ppOutMessages, nMaxMessages);
    }
    virtual bool GetConnectionInfo(std::uint32_t hConn, void* pInfo) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets009[15], hConn, pInfo);
    }
    virtual bool GetQuickConnectionStatus(std::uint32_t hConn, void* pStats) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets009[16], hConn, pStats);
    }
    virtual std::int32_t GetDetailedConnectionStatus(std::uint32_t hConn, void* pszBuf, std::int32_t cbBuf) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets009[17], hConn, pszBuf, cbBuf);
    }
    virtual bool GetListenSocketAddress(std::uint32_t hSocket, void* address) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets009[18], hSocket, address);
    }
    virtual bool CreateSocketPair(std::uint32_t* pOutConnection1, std::uint32_t* pOutConnection2, bool bUseNetworkLoopback, void* pIdentity1, void* pIdentity2) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets009[19], pOutConnection1, pOutConnection2, bUseNetworkLoopback, pIdentity1, pIdentity2);
    }
    virtual bool GetIdentity(void* pIdentity) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets009[20], pIdentity);
    }
    virtual std::int32_t InitAuthentication() {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets009[21]);
    }
    virtual std::int32_t GetAuthenticationStatus(void* pDetails) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets009[22], pDetails);
    }
    virtual std::uint32_t CreatePollGroup() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworkingSockets009[23]);
    }
    virtual bool DestroyPollGroup(std::uint32_t hPollGroup) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets009[24], hPollGroup);
    }
    virtual bool SetConnectionPollGroup(std::uint32_t hConn, std::uint32_t hPollGroup) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets009[25], hConn, hPollGroup);
    }
    virtual std::int32_t ReceiveMessagesOnPollGroup(std::uint32_t hPollGroup, void* ppOutMessages, std::int32_t nMaxMessages) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets009[26], hPollGroup, ppOutMessages, nMaxMessages);
    }
    virtual bool ReceivedRelayAuthTicket(void* pvTicket, std::int32_t cbTicket, void* pOutParsedTicket) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets009[27], pvTicket, cbTicket, pOutParsedTicket);
    }
    virtual std::int32_t FindRelayAuthTicketForServer(void* identityGameServer, std::int32_t nRemoteVirtualPort, void* pOutParsedTicket) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets009[28], identityGameServer, nRemoteVirtualPort, pOutParsedTicket);
    }
    virtual std::uint32_t ConnectToHostedDedicatedServer(void* identityTarget, std::int32_t nRemoteVirtualPort, std::int32_t nOptions, void* pOptions) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworkingSockets009[29], identityTarget, nRemoteVirtualPort, nOptions, pOptions);
    }
    virtual std::uint16_t GetHostedDedicatedServerPort() {
        return steambridge::slot<std::uint16_t>(kSlots_SteamNetworkingSockets009[30]);
    }
    virtual std::uint32_t GetHostedDedicatedServerPOPID() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworkingSockets009[31]);
    }
    virtual std::int32_t GetHostedDedicatedServerAddress(void* pRouting) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets009[32], pRouting);
    }
    virtual std::uint32_t CreateHostedDedicatedServerListenSocket(std::int32_t nLocalVirtualPort, std::int32_t nOptions, void* pOptions) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworkingSockets009[33], nLocalVirtualPort, nOptions, pOptions);
    }
    virtual std::int32_t GetGameCoordinatorServerLogin(void* pLoginInfo, std::int32_t* pcbSignedBlob, void* pBlob) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets009[34], pLoginInfo, pcbSignedBlob, pBlob);
    }
    virtual std::uint32_t ConnectP2PCustomSignaling(void* pSignaling, void* pPeerIdentity, std::int32_t nRemoteVirtualPort, std::int32_t nOptions, void* pOptions) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworkingSockets009[35], pSignaling, pPeerIdentity, nRemoteVirtualPort, nOptions, pOptions);
    }
    virtual bool ReceivedP2PCustomSignal(void* pMsg, std::int32_t cbMsg, void* pContext) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets009[36], pMsg, cbMsg, pContext);
    }
    virtual bool GetCertificateRequest(std::int32_t* pcbBlob, void* pBlob, void* errMsg) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets009[37], pcbBlob, pBlob, errMsg);
    }
    virtual bool SetCertificate(void* pCertificate, std::int32_t cbCertificate, void* errMsg) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets009[38], pCertificate, cbCertificate, errMsg);
    }
    virtual void RunCallbacks() {
        steambridge::slot<void>(kSlots_SteamNetworkingSockets009[39]);
    }
};

Version_SteamNetworkingSockets009 g_SteamNetworkingSockets009;

const char* const kParams_SteamNetworkingSockets012_0[] = {
    "localAddress",
    "nOptions",
    "pOptions",
};
const char* const kParams_SteamNetworkingSockets012_1[] = {
    "address",
    "nOptions",
    "pOptions",
};
const char* const kParams_SteamNetworkingSockets012_2[] = {
    "nLocalVirtualPort",
    "nOptions",
    "pOptions",
};
const char* const kParams_SteamNetworkingSockets012_3[] = {
    "identityRemote",
    "nRemoteVirtualPort",
    "nOptions",
    "pOptions",
};
const char* const kParams_SteamNetworkingSockets012_4[] = {
    "hConn",
};
const char* const kParams_SteamNetworkingSockets012_5[] = {
    "hPeer",
    "nReason",
    "pszDebug",
    "bEnableLinger",
};
const char* const kParams_SteamNetworkingSockets012_6[] = {
    "hSocket",
};
const char* const kParams_SteamNetworkingSockets012_7[] = {
    "hPeer",
    "nUserData",
};
const char* const kParams_SteamNetworkingSockets012_8[] = {
    "hPeer",
};
const char* const kParams_SteamNetworkingSockets012_9[] = {
    "hPeer",
    "pszName",
};
const char* const kParams_SteamNetworkingSockets012_10[] = {
    "hPeer",
    "pszName",
    "nMaxLen",
};
const char* const kParams_SteamNetworkingSockets012_11[] = {
    "hConn",
    "pData",
    "cbData",
    "nSendFlags",
    "pOutMessageNumber",
};
const char* const kParams_SteamNetworkingSockets012_12[] = {
    "nMessages",
    "pMessages",
    "pOutMessageNumberOrResult",
};
const char* const kParams_SteamNetworkingSockets012_13[] = {
    "hConn",
};
const char* const kParams_SteamNetworkingSockets012_14[] = {
    "hConn",
    "ppOutMessages",
    "nMaxMessages",
};
const char* const kParams_SteamNetworkingSockets012_15[] = {
    "hConn",
    "pInfo",
};
const char* const kParams_SteamNetworkingSockets012_16[] = {
    "hConn",
    "pStatus",
    "nLanes",
    "pLanes",
};
const char* const kParams_SteamNetworkingSockets012_17[] = {
    "hConn",
    "pszBuf",
    "cbBuf",
};
const char* const kParams_SteamNetworkingSockets012_18[] = {
    "hSocket",
    "address",
};
const char* const kParams_SteamNetworkingSockets012_19[] = {
    "pOutConnection1",
    "pOutConnection2",
    "bUseNetworkLoopback",
    "pIdentity1",
    "pIdentity2",
};
const char* const kParams_SteamNetworkingSockets012_20[] = {
    "hConn",
    "nNumLanes",
    "pLanePriorities",
    "pLaneWeights",
};
const char* const kParams_SteamNetworkingSockets012_21[] = {
    "pIdentity",
};
const char* const kParams_SteamNetworkingSockets012_23[] = {
    "pDetails",
};
const char* const kParams_SteamNetworkingSockets012_25[] = {
    "hPollGroup",
};
const char* const kParams_SteamNetworkingSockets012_26[] = {
    "hConn",
    "hPollGroup",
};
const char* const kParams_SteamNetworkingSockets012_27[] = {
    "hPollGroup",
    "ppOutMessages",
    "nMaxMessages",
};
const char* const kParams_SteamNetworkingSockets012_28[] = {
    "pvTicket",
    "cbTicket",
    "pOutParsedTicket",
};
const char* const kParams_SteamNetworkingSockets012_29[] = {
    "identityGameServer",
    "nRemoteVirtualPort",
    "pOutParsedTicket",
};
const char* const kParams_SteamNetworkingSockets012_30[] = {
    "identityTarget",
    "nRemoteVirtualPort",
    "nOptions",
    "pOptions",
};
const char* const kParams_SteamNetworkingSockets012_33[] = {
    "pRouting",
};
const char* const kParams_SteamNetworkingSockets012_34[] = {
    "nLocalVirtualPort",
    "nOptions",
    "pOptions",
};
const char* const kParams_SteamNetworkingSockets012_35[] = {
    "pLoginInfo",
    "pcbSignedBlob",
    "pBlob",
};
const char* const kParams_SteamNetworkingSockets012_36[] = {
    "pSignaling",
    "pPeerIdentity",
    "nRemoteVirtualPort",
    "nOptions",
    "pOptions",
};
const char* const kParams_SteamNetworkingSockets012_37[] = {
    "pMsg",
    "cbMsg",
    "pContext",
};
const char* const kParams_SteamNetworkingSockets012_38[] = {
    "pcbBlob",
    "pBlob",
    "errMsg",
};
const char* const kParams_SteamNetworkingSockets012_39[] = {
    "pCertificate",
    "cbCertificate",
    "errMsg",
};
const char* const kParams_SteamNetworkingSockets012_40[] = {
    "pIdentity",
};
const char* const kParams_SteamNetworkingSockets012_42[] = {
    "nNumPorts",
};
const char* const kParams_SteamNetworkingSockets012_43[] = {
    "idxFirstPort",
    "pInfo",
};
const char* const kParams_SteamNetworkingSockets012_44[] = {
    "idxFakePort",
    "nOptions",
    "pOptions",
};
const char* const kParams_SteamNetworkingSockets012_45[] = {
    "hConn",
    "pOutAddr",
};
const char* const kParams_SteamNetworkingSockets012_46[] = {
    "idxFakeServerPort",
};
// ISteamNetworkingSockets SteamNetworkingSockets012
const steambridge::SlotInfo kSlots_SteamNetworkingSockets012[] = {
    {"SteamAPI_ISteamNetworkingSockets_CreateListenSocketIP", kParams_SteamNetworkingSockets012_0},
    {"SteamAPI_ISteamNetworkingSockets_ConnectByIPAddress", kParams_SteamNetworkingSockets012_1},
    {"SteamAPI_ISteamNetworkingSockets_CreateListenSocketP2P", kParams_SteamNetworkingSockets012_2},
    {"SteamAPI_ISteamNetworkingSockets_ConnectP2P", kParams_SteamNetworkingSockets012_3},
    {"SteamAPI_ISteamNetworkingSockets_AcceptConnection", kParams_SteamNetworkingSockets012_4},
    {"SteamAPI_ISteamNetworkingSockets_CloseConnection", kParams_SteamNetworkingSockets012_5},
    {"SteamAPI_ISteamNetworkingSockets_CloseListenSocket", kParams_SteamNetworkingSockets012_6},
    {"SteamAPI_ISteamNetworkingSockets_SetConnectionUserData", kParams_SteamNetworkingSockets012_7},
    {"SteamAPI_ISteamNetworkingSockets_GetConnectionUserData", kParams_SteamNetworkingSockets012_8},
    {"SteamAPI_ISteamNetworkingSockets_SetConnectionName", kParams_SteamNetworkingSockets012_9},
    {"SteamAPI_ISteamNetworkingSockets_GetConnectionName", kParams_SteamNetworkingSockets012_10},
    {"SteamAPI_ISteamNetworkingSockets_SendMessageToConnection", kParams_SteamNetworkingSockets012_11},
    {"SteamAPI_ISteamNetworkingSockets_SendMessages", kParams_SteamNetworkingSockets012_12},
    {"SteamAPI_ISteamNetworkingSockets_FlushMessagesOnConnection", kParams_SteamNetworkingSockets012_13},
    {"SteamAPI_ISteamNetworkingSockets_ReceiveMessagesOnConnection", kParams_SteamNetworkingSockets012_14},
    {"SteamAPI_ISteamNetworkingSockets_GetConnectionInfo", kParams_SteamNetworkingSockets012_15},
    {"SteamAPI_ISteamNetworkingSockets_GetConnectionRealTimeStatus", kParams_SteamNetworkingSockets012_16},
    {"SteamAPI_ISteamNetworkingSockets_GetDetailedConnectionStatus", kParams_SteamNetworkingSockets012_17},
    {"SteamAPI_ISteamNetworkingSockets_GetListenSocketAddress", kParams_SteamNetworkingSockets012_18},
    {"SteamAPI_ISteamNetworkingSockets_CreateSocketPair", kParams_SteamNetworkingSockets012_19},
    {"SteamAPI_ISteamNetworkingSockets_ConfigureConnectionLanes", kParams_SteamNetworkingSockets012_20},
    {"SteamAPI_ISteamNetworkingSockets_GetIdentity", kParams_SteamNetworkingSockets012_21},
    {"SteamAPI_ISteamNetworkingSockets_InitAuthentication", nullptr},
    {"SteamAPI_ISteamNetworkingSockets_GetAuthenticationStatus", kParams_SteamNetworkingSockets012_23},
    {"SteamAPI_ISteamNetworkingSockets_CreatePollGroup", nullptr},
    {"SteamAPI_ISteamNetworkingSockets_DestroyPollGroup", kParams_SteamNetworkingSockets012_25},
    {"SteamAPI_ISteamNetworkingSockets_SetConnectionPollGroup", kParams_SteamNetworkingSockets012_26},
    {"SteamAPI_ISteamNetworkingSockets_ReceiveMessagesOnPollGroup", kParams_SteamNetworkingSockets012_27},
    {"SteamAPI_ISteamNetworkingSockets_ReceivedRelayAuthTicket", kParams_SteamNetworkingSockets012_28},
    {"SteamAPI_ISteamNetworkingSockets_FindRelayAuthTicketForServer", kParams_SteamNetworkingSockets012_29},
    {"SteamAPI_ISteamNetworkingSockets_ConnectToHostedDedicatedServer", kParams_SteamNetworkingSockets012_30},
    {"SteamAPI_ISteamNetworkingSockets_GetHostedDedicatedServerPort", nullptr},
    {"SteamAPI_ISteamNetworkingSockets_GetHostedDedicatedServerPOPID", nullptr},
    {"SteamAPI_ISteamNetworkingSockets_GetHostedDedicatedServerAddress", kParams_SteamNetworkingSockets012_33},
    {"SteamAPI_ISteamNetworkingSockets_CreateHostedDedicatedServerListenSocket", kParams_SteamNetworkingSockets012_34},
    {"SteamAPI_ISteamNetworkingSockets_GetGameCoordinatorServerLogin", kParams_SteamNetworkingSockets012_35},
    {"SteamAPI_ISteamNetworkingSockets_ConnectP2PCustomSignaling", kParams_SteamNetworkingSockets012_36},
    {"SteamAPI_ISteamNetworkingSockets_ReceivedP2PCustomSignal", kParams_SteamNetworkingSockets012_37},
    {"SteamAPI_ISteamNetworkingSockets_GetCertificateRequest", kParams_SteamNetworkingSockets012_38},
    {"SteamAPI_ISteamNetworkingSockets_SetCertificate", kParams_SteamNetworkingSockets012_39},
    {"SteamAPI_ISteamNetworkingSockets_ResetIdentity", kParams_SteamNetworkingSockets012_40},
    {"SteamAPI_ISteamNetworkingSockets_RunCallbacks", nullptr},
    {"SteamAPI_ISteamNetworkingSockets_BeginAsyncRequestFakeIP", kParams_SteamNetworkingSockets012_42},
    {"SteamAPI_ISteamNetworkingSockets_GetFakeIP", kParams_SteamNetworkingSockets012_43},
    {"SteamAPI_ISteamNetworkingSockets_CreateListenSocketP2PFakeIP", kParams_SteamNetworkingSockets012_44},
    {"SteamAPI_ISteamNetworkingSockets_GetRemoteFakeIPForConnection", kParams_SteamNetworkingSockets012_45},
    {"SteamAPI_ISteamNetworkingSockets_CreateFakeUDPPort", kParams_SteamNetworkingSockets012_46},
};

class Version_SteamNetworkingSockets012 {
public:
    virtual std::uint32_t CreateListenSocketIP(void* localAddress, std::int32_t nOptions, void* pOptions) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworkingSockets012[0], localAddress, nOptions, pOptions);
    }
    virtual std::uint32_t ConnectByIPAddress(void* address, std::int32_t nOptions, void* pOptions) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworkingSockets012[1], address, nOptions, pOptions);
    }
    virtual std::uint32_t CreateListenSocketP2P(std::int32_t nLocalVirtualPort, std::int32_t nOptions, void* pOptions) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworkingSockets012[2], nLocalVirtualPort, nOptions, pOptions);
    }
    virtual std::uint32_t ConnectP2P(void* identityRemote, std::int32_t nRemoteVirtualPort, std::int32_t nOptions, void* pOptions) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworkingSockets012[3], identityRemote, nRemoteVirtualPort, nOptions, pOptions);
    }
    virtual std::int32_t AcceptConnection(std::uint32_t hConn) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets012[4], hConn);
    }
    virtual bool CloseConnection(std::uint32_t hPeer, std::int32_t nReason, const char* pszDebug, bool bEnableLinger) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets012[5], hPeer, nReason, pszDebug, bEnableLinger);
    }
    virtual bool CloseListenSocket(std::uint32_t hSocket) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets012[6], hSocket);
    }
    virtual bool SetConnectionUserData(std::uint32_t hPeer, std::int64_t nUserData) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets012[7], hPeer, nUserData);
    }
    virtual std::int64_t GetConnectionUserData(std::uint32_t hPeer) {
        return steambridge::slot<std::int64_t>(kSlots_SteamNetworkingSockets012[8], hPeer);
    }
    virtual void SetConnectionName(std::uint32_t hPeer, const char* pszName) {
        steambridge::slot<void>(kSlots_SteamNetworkingSockets012[9], hPeer, pszName);
    }
    virtual bool GetConnectionName(std::uint32_t hPeer, void* pszName, std::int32_t nMaxLen) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets012[10], hPeer, pszName, nMaxLen);
    }
    virtual std::int32_t SendMessageToConnection(std::uint32_t hConn, void* pData, std::uint32_t cbData, std::int32_t nSendFlags, std::int64_t* pOutMessageNumber) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets012[11], hConn, pData, cbData, nSendFlags, pOutMessageNumber);
    }
    virtual void SendMessages(std::int32_t nMessages, void* pMessages, std::int64_t* pOutMessageNumberOrResult) {
        steambridge::slot<void>(kSlots_SteamNetworkingSockets012[12], nMessages, pMessages, pOutMessageNumberOrResult);
    }
    virtual std::int32_t FlushMessagesOnConnection(std::uint32_t hConn) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets012[13], hConn);
    }
    virtual std::int32_t ReceiveMessagesOnConnection(std::uint32_t hConn, void* ppOutMessages, std::int32_t nMaxMessages) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets012[14], hConn, ppOutMessages, nMaxMessages);
    }
    virtual bool GetConnectionInfo(std::uint32_t hConn, void* pInfo) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets012[15], hConn, pInfo);
    }
    virtual std::int32_t GetConnectionRealTimeStatus(std::uint32_t hConn, void* pStatus, std::int32_t nLanes, void* pLanes) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets012[16], hConn, pStatus, nLanes, pLanes);
    }
    virtual std::int32_t GetDetailedConnectionStatus(std::uint32_t hConn, void* pszBuf, std::int32_t cbBuf) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets012[17], hConn, pszBuf, cbBuf);
    }
    virtual bool GetListenSocketAddress(std::uint32_t hSocket, void* address) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets012[18], hSocket, address);
    }
    virtual bool CreateSocketPair(std::uint32_t* pOutConnection1, std::uint32_t* pOutConnection2, bool bUseNetworkLoopback, void* pIdentity1, void* pIdentity2) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets012[19], pOutConnection1, pOutConnection2, bUseNetworkLoopback, pIdentity1, pIdentity2);
    }
    virtual std::int32_t ConfigureConnectionLanes(std::uint32_t hConn, std::int32_t nNumLanes, void* pLanePriorities, void* pLaneWeights) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets012[20], hConn, nNumLanes, pLanePriorities, pLaneWeights);
    }
    virtual bool GetIdentity(void* pIdentity) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets012[21], pIdentity);
    }
    virtual std::int32_t InitAuthentication() {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets012[22]);
    }
    virtual std::int32_t GetAuthenticationStatus(void* pDetails) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets012[23], pDetails);
    }
    virtual std::uint32_t CreatePollGroup() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworkingSockets012[24]);
    }
    virtual bool DestroyPollGroup(std::uint32_t hPollGroup) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets012[25], hPollGroup);
    }
    virtual bool SetConnectionPollGroup(std::uint32_t hConn, std::uint32_t hPollGroup) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets012[26], hConn, hPollGroup);
    }
    virtual std::int32_t ReceiveMessagesOnPollGroup(std::uint32_t hPollGroup, void* ppOutMessages, std::int32_t nMaxMessages) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets012[27], hPollGroup, ppOutMessages, nMaxMessages);
    }
    virtual bool ReceivedRelayAuthTicket(void* pvTicket, std::int32_t cbTicket, void* pOutParsedTicket) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets012[28], pvTicket, cbTicket, pOutParsedTicket);
    }
    virtual std::int32_t FindRelayAuthTicketForServer(void* identityGameServer, std::int32_t nRemoteVirtualPort, void* pOutParsedTicket) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets012[29], identityGameServer, nRemoteVirtualPort, pOutParsedTicket);
    }
    virtual std::uint32_t ConnectToHostedDedicatedServer(void* identityTarget, std::int32_t nRemoteVirtualPort, std::int32_t nOptions, void* pOptions) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworkingSockets012[30], identityTarget, nRemoteVirtualPort, nOptions, pOptions);
    }
    virtual std::uint16_t GetHostedDedicatedServerPort() {
        return steambridge::slot<std::uint16_t>(kSlots_SteamNetworkingSockets012[31]);
    }
    virtual std::uint32_t GetHostedDedicatedServerPOPID() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworkingSockets012[32]);
    }
    virtual std::int32_t GetHostedDedicatedServerAddress(void* pRouting) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets012[33], pRouting);
    }
    virtual std::uint32_t CreateHostedDedicatedServerListenSocket(std::int32_t nLocalVirtualPort, std::int32_t nOptions, void* pOptions) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworkingSockets012[34], nLocalVirtualPort, nOptions, pOptions);
    }
    virtual std::int32_t GetGameCoordinatorServerLogin(void* pLoginInfo, std::int32_t* pcbSignedBlob, void* pBlob) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets012[35], pLoginInfo, pcbSignedBlob, pBlob);
    }
    virtual std::uint32_t ConnectP2PCustomSignaling(void* pSignaling, void* pPeerIdentity, std::int32_t nRemoteVirtualPort, std::int32_t nOptions, void* pOptions) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworkingSockets012[36], pSignaling, pPeerIdentity, nRemoteVirtualPort, nOptions, pOptions);
    }
    virtual bool ReceivedP2PCustomSignal(void* pMsg, std::int32_t cbMsg, void* pContext) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets012[37], pMsg, cbMsg, pContext);
    }
    virtual bool GetCertificateRequest(std::int32_t* pcbBlob, void* pBlob, void* errMsg) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets012[38], pcbBlob, pBlob, errMsg);
    }
    virtual bool SetCertificate(void* pCertificate, std::int32_t cbCertificate, void* errMsg) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets012[39], pCertificate, cbCertificate, errMsg);
    }
    virtual void ResetIdentity(void* pIdentity) {
        steambridge::slot<void>(kSlots_SteamNetworkingSockets012[40], pIdentity);
    }
    virtual void RunCallbacks() {
        steambridge::slot<void>(kSlots_SteamNetworkingSockets012[41]);
    }
    virtual bool BeginAsyncRequestFakeIP(std::int32_t nNumPorts) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingSockets012[42], nNumPorts);
    }
    virtual void GetFakeIP(std::int32_t idxFirstPort, void* pInfo) {
        steambridge::slot<void>(kSlots_SteamNetworkingSockets012[43], idxFirstPort, pInfo);
    }
    virtual std::uint32_t CreateListenSocketP2PFakeIP(std::int32_t idxFakePort, std::int32_t nOptions, void* pOptions) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamNetworkingSockets012[44], idxFakePort, nOptions, pOptions);
    }
    virtual std::int32_t GetRemoteFakeIPForConnection(std::uint32_t hConn, void* pOutAddr) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingSockets012[45], hConn, pOutAddr);
    }
    virtual void* CreateFakeUDPPort(std::int32_t idxFakeServerPort) {
        return steambridge::slot<void*>(kSlots_SteamNetworkingSockets012[46], idxFakeServerPort);
    }
};

Version_SteamNetworkingSockets012 g_SteamNetworkingSockets012;

const char* const kParams_SteamNetworkingUtils003_0[] = {
    "cbAllocateBuffer",
};
const char* const kParams_SteamNetworkingUtils003_1[] = {
    "pDetails",
};
const char* const kParams_SteamNetworkingUtils003_2[] = {
    "result",
};
const char* const kParams_SteamNetworkingUtils003_3[] = {
    "location1",
    "location2",
};
const char* const kParams_SteamNetworkingUtils003_4[] = {
    "remoteLocation",
};
const char* const kParams_SteamNetworkingUtils003_5[] = {
    "location",
    "pszBuf",
    "cchBufSize",
};
const char* const kParams_SteamNetworkingUtils003_6[] = {
    "pszString",
    "result",
};
const char* const kParams_SteamNetworkingUtils003_7[] = {
    "flMaxAgeSeconds",
};
const char* const kParams_SteamNetworkingUtils003_8[] = {
    "popID",
    "pViaRelayPoP",
};
const char* const kParams_SteamNetworkingUtils003_9[] = {
    "popID",
};
const char* const kParams_SteamNetworkingUtils003_11[] = {
    "list",
    "nListSz",
};
const char* const kParams_SteamNetworkingUtils003_13[] = {
    "eDetailLevel",
    "pfnFunc",
};
const char* const kParams_SteamNetworkingUtils003_14[] = {
    "eValue",
    "eScopeType",
    "scopeObj",
    "eDataType",
    "pArg",
};
const char* const kParams_SteamNetworkingUtils003_15[] = {
    "eValue",
    "eScopeType",
    "scopeObj",
    "pOutDataType",
    "pResult",
    "cbResult",
};
const char* const kParams_SteamNetworkingUtils003_16[] = {
    "eValue",
    "pOutName",
    "pOutDataType",
    "pOutScope",
    "pOutNextValue",
};
const char* const kParams_SteamNetworkingUtils003_18[] = {
    "addr",
    "buf",
    "cbBuf",
    "bWithPort",
};
const char* const kParams_SteamNetworkingUtils003_19[] = {
    "pAddr",
    "pszStr",
};
const char* const kParams_SteamNetworkingUtils003_20[] = {
    "identity",
    "buf",
    "cbBuf",
};
const char* const kParams_SteamNetworkingUtils003_21[] = {
    "pIdentity",
    "pszStr",
};
// ISteamNetworkingUtils SteamNetworkingUtils003
const steambridge::SlotInfo kSlots_SteamNetworkingUtils003[] = {
    {"SteamAPI_ISteamNetworkingUtils_AllocateMessage", kParams_SteamNetworkingUtils003_0},
    {"SteamAPI_ISteamNetworkingUtils_GetRelayNetworkStatus", kParams_SteamNetworkingUtils003_1},
    {"SteamAPI_ISteamNetworkingUtils_GetLocalPingLocation", kParams_SteamNetworkingUtils003_2},
    {"SteamAPI_ISteamNetworkingUtils_EstimatePingTimeBetweenTwoLocations", kParams_SteamNetworkingUtils003_3},
    {"SteamAPI_ISteamNetworkingUtils_EstimatePingTimeFromLocalHost", kParams_SteamNetworkingUtils003_4},
    {"SteamAPI_ISteamNetworkingUtils_ConvertPingLocationToString", kParams_SteamNetworkingUtils003_5},
    {"SteamAPI_ISteamNetworkingUtils_ParsePingLocationString", kParams_SteamNetworkingUtils003_6},
    {"SteamAPI_ISteamNetworkingUtils_CheckPingDataUpToDate", kParams_SteamNetworkingUtils003_7},
    {"SteamAPI_ISteamNetworkingUtils_GetPingToDataCenter", kParams_SteamNetworkingUtils003_8},
    {"SteamAPI_ISteamNetworkingUtils_GetDirectPingToPOP", kParams_SteamNetworkingUtils003_9},
    {"SteamAPI_ISteamNetworkingUtils_GetPOPCount", nullptr},
    {"SteamAPI_ISteamNetworkingUtils_GetPOPList", kParams_SteamNetworkingUtils003_11},
    {"SteamAPI_ISteamNetworkingUtils_GetLocalTimestamp", nullptr},
    {"SteamAPI_ISteamNetworkingUtils_SetDebugOutputFunction", kParams_SteamNetworkingUtils003_13},
    {"SteamAPI_ISteamNetworkingUtils_SetConfigValue", kParams_SteamNetworkingUtils003_14},
    {"SteamAPI_ISteamNetworkingUtils_GetConfigValue", kParams_SteamNetworkingUtils003_15},
    {"SteamAPI_ISteamNetworkingUtils_GetConfigValueInfo", kParams_SteamNetworkingUtils003_16},
    {"SteamAPI_ISteamNetworkingUtils_GetFirstConfigValue", nullptr},
    {"SteamAPI_ISteamNetworkingUtils_SteamNetworkingIPAddr_ToString", kParams_SteamNetworkingUtils003_18},
    {"SteamAPI_ISteamNetworkingUtils_SteamNetworkingIPAddr_ParseString", kParams_SteamNetworkingUtils003_19},
    {"SteamAPI_ISteamNetworkingUtils_SteamNetworkingIdentity_ToString", kParams_SteamNetworkingUtils003_20},
    {"SteamAPI_ISteamNetworkingUtils_SteamNetworkingIdentity_ParseString", kParams_SteamNetworkingUtils003_21},
};

class Version_SteamNetworkingUtils003 {
public:
    virtual void* AllocateMessage(std::int32_t cbAllocateBuffer) {
        return steambridge::slot<void*>(kSlots_SteamNetworkingUtils003[0], cbAllocateBuffer);
    }
    virtual std::int32_t GetRelayNetworkStatus(void* pDetails) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingUtils003[1], pDetails);
    }
    virtual float GetLocalPingLocation(void* result) {
        return steambridge::slot<float>(kSlots_SteamNetworkingUtils003[2], result);
    }
    virtual std::int32_t EstimatePingTimeBetweenTwoLocations(void* location1, void* location2) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingUtils003[3], location1, location2);
    }
    virtual std::int32_t EstimatePingTimeFromLocalHost(void* remoteLocation) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingUtils003[4], remoteLocation);
    }
    virtual void ConvertPingLocationToString(void* location, void* pszBuf, std::int32_t cchBufSize) {
        steambridge::slot<void>(kSlots_SteamNetworkingUtils003[5], location, pszBuf, cchBufSize);
    }
    virtual bool ParsePingLocationString(const char* pszString, void* result) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingUtils003[6], pszString, result);
    }
    virtual bool CheckPingDataUpToDate(float flMaxAgeSeconds) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingUtils003[7], flMaxAgeSeconds);
    }
    virtual std::int32_t GetPingToDataCenter(std::uint32_t popID, std::uint32_t* pViaRelayPoP) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingUtils003[8], popID, pViaRelayPoP);
    }
    virtual std::int32_t GetDirectPingToPOP(std::uint32_t popID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingUtils003[9], popID);
    }
    virtual std::int32_t GetPOPCount() {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingUtils003[10]);
    }
    virtual std::int32_t GetPOPList(std::uint32_t* list, std::int32_t nListSz) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingUtils003[11], list, nListSz);
    }
    virtual std::int64_t GetLocalTimestamp() {
        return steambridge::slot<std::int64_t>(kSlots_SteamNetworkingUtils003[12]);
    }
    virtual void SetDebugOutputFunction(std::int32_t eDetailLevel, void* pfnFunc) {
        steambridge::slot<void>(kSlots_SteamNetworkingUtils003[13], eDetailLevel, pfnFunc);
    }
    virtual bool SetConfigValue(std::int32_t eValue, std::int32_t eScopeType, std::intptr_t scopeObj, std::int32_t eDataType, void* pArg) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingUtils003[14], eValue, eScopeType, scopeObj, eDataType, pArg);
    }
    virtual std::int32_t GetConfigValue(std::int32_t eValue, std::int32_t eScopeType, std::intptr_t scopeObj, std::int32_t* pOutDataType, void* pResult, std::size_t* cbResult) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingUtils003[15], eValue, eScopeType, scopeObj, pOutDataType, pResult, cbResult);
    }
    virtual bool GetConfigValueInfo(std::int32_t eValue, void* pOutName, std::int32_t* pOutDataType, std::int32_t* pOutScope, std::int32_t* pOutNextValue) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingUtils003[16], eValue, pOutName, pOutDataType, pOutScope, pOutNextValue);
    }
    virtual std::int32_t GetFirstConfigValue() {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingUtils003[17]);
    }
    virtual void SteamNetworkingIPAddr_ToString(void* addr, void* buf, std::size_t cbBuf, bool bWithPort) {
        steambridge::slot<void>(kSlots_SteamNetworkingUtils003[18], addr, buf, cbBuf, bWithPort);
    }
    virtual bool SteamNetworkingIPAddr_ParseString(void* pAddr, const char* pszStr) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingUtils003[19], pAddr, pszStr);
    }
    virtual void SteamNetworkingIdentity_ToString(void* identity, void* buf, std::size_t cbBuf) {
        steambridge::slot<void>(kSlots_SteamNetworkingUtils003[20], identity, buf, cbBuf);
    }
    virtual bool SteamNetworkingIdentity_ParseString(void* pIdentity, const char* pszStr) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingUtils003[21], pIdentity, pszStr);
    }
};

Version_SteamNetworkingUtils003 g_SteamNetworkingUtils003;

const char* const kParams_SteamNetworkingUtils004_0[] = {
    "cbAllocateBuffer",
};
const char* const kParams_SteamNetworkingUtils004_1[] = {
    "pDetails",
};
const char* const kParams_SteamNetworkingUtils004_2[] = {
    "result",
};
const char* const kParams_SteamNetworkingUtils004_3[] = {
    "location1",
    "location2",
};
const char* const kParams_SteamNetworkingUtils004_4[] = {
    "remoteLocation",
};
const char* const kParams_SteamNetworkingUtils004_5[] = {
    "location",
    "pszBuf",
    "cchBufSize",
};
const char* const kParams_SteamNetworkingUtils004_6[] = {
    "pszString",
    "result",
};
const char* const kParams_SteamNetworkingUtils004_7[] = {
    "flMaxAgeSeconds",
};
const char* const kParams_SteamNetworkingUtils004_8[] = {
    "popID",
    "pViaRelayPoP",
};
const char* const kParams_SteamNetworkingUtils004_9[] = {
    "popID",
};
const char* const kParams_SteamNetworkingUtils004_11[] = {
    "list",
    "nListSz",
};
const char* const kParams_SteamNetworkingUtils004_13[] = {
    "eDetailLevel",
    "pfnFunc",
};
const char* const kParams_SteamNetworkingUtils004_14[] = {
    "nIPv4",
};
const char* const kParams_SteamNetworkingUtils004_15[] = {
    "fakeIP",
    "pOutRealIdentity",
};
const char* const kParams_SteamNetworkingUtils004_16[] = {
    "eValue",
    "eScopeType",
    "scopeObj",
    "eDataType",
    "pArg",
};
const char* const kParams_SteamNetworkingUtils004_17[] = {
    "eValue",
    "eScopeType",
    "scopeObj",
    "pOutDataType",
    "pResult",
    "cbResult",
};
const char* const kParams_SteamNetworkingUtils004_18[] = {
    "eValue",
    "pOutDataType",
    "pOutScope",
};
const char* const kParams_SteamNetworkingUtils004_19[] = {
    "eCurrent",
    "bEnumerateDevVars",
};
const char* const kParams_SteamNetworkingUtils004_20[] = {
    "addr",
    "buf",
    "cbBuf",
    "bWithPort",
};
const char* const kParams_SteamNetworkingUtils004_21[] = {
    "pAddr",
    "pszStr",
};
const char* const kParams_SteamNetworkingUtils004_22[] = {
    "addr",
};
const char* const kParams_SteamNetworkingUtils004_23[] = {
    "identity",
    "buf",
    "cbBuf",
};
const char* const kParams_SteamNetworkingUtils004_24[] = {
    "pIdentity",
    "pszStr",
};
// ISteamNetworkingUtils SteamNetworkingUtils004
const steambridge::SlotInfo kSlots_SteamNetworkingUtils004[] = {
    {"SteamAPI_ISteamNetworkingUtils_AllocateMessage", kParams_SteamNetworkingUtils004_0},
    {"SteamAPI_ISteamNetworkingUtils_GetRelayNetworkStatus", kParams_SteamNetworkingUtils004_1},
    {"SteamAPI_ISteamNetworkingUtils_GetLocalPingLocation", kParams_SteamNetworkingUtils004_2},
    {"SteamAPI_ISteamNetworkingUtils_EstimatePingTimeBetweenTwoLocations", kParams_SteamNetworkingUtils004_3},
    {"SteamAPI_ISteamNetworkingUtils_EstimatePingTimeFromLocalHost", kParams_SteamNetworkingUtils004_4},
    {"SteamAPI_ISteamNetworkingUtils_ConvertPingLocationToString", kParams_SteamNetworkingUtils004_5},
    {"SteamAPI_ISteamNetworkingUtils_ParsePingLocationString", kParams_SteamNetworkingUtils004_6},
    {"SteamAPI_ISteamNetworkingUtils_CheckPingDataUpToDate", kParams_SteamNetworkingUtils004_7},
    {"SteamAPI_ISteamNetworkingUtils_GetPingToDataCenter", kParams_SteamNetworkingUtils004_8},
    {"SteamAPI_ISteamNetworkingUtils_GetDirectPingToPOP", kParams_SteamNetworkingUtils004_9},
    {"SteamAPI_ISteamNetworkingUtils_GetPOPCount", nullptr},
    {"SteamAPI_ISteamNetworkingUtils_GetPOPList", kParams_SteamNetworkingUtils004_11},
    {"SteamAPI_ISteamNetworkingUtils_GetLocalTimestamp", nullptr},
    {"SteamAPI_ISteamNetworkingUtils_SetDebugOutputFunction", kParams_SteamNetworkingUtils004_13},
    {"SteamAPI_ISteamNetworkingUtils_GetIPv4FakeIPType", kParams_SteamNetworkingUtils004_14},
    {"SteamAPI_ISteamNetworkingUtils_GetRealIdentityForFakeIP", kParams_SteamNetworkingUtils004_15},
    {"SteamAPI_ISteamNetworkingUtils_SetConfigValue", kParams_SteamNetworkingUtils004_16},
    {"SteamAPI_ISteamNetworkingUtils_GetConfigValue", kParams_SteamNetworkingUtils004_17},
    {"SteamAPI_ISteamNetworkingUtils_GetConfigValueInfo", kParams_SteamNetworkingUtils004_18},
    {"SteamAPI_ISteamNetworkingUtils_IterateGenericEditableConfigValues", kParams_SteamNetworkingUtils004_19},
    {"SteamAPI_ISteamNetworkingUtils_SteamNetworkingIPAddr_ToString", kParams_SteamNetworkingUtils004_20},
    {"SteamAPI_ISteamNetworkingUtils_SteamNetworkingIPAddr_ParseString", kParams_SteamNetworkingUtils004_21},
    {"SteamAPI_ISteamNetworkingUtils_SteamNetworkingIPAddr_GetFakeIPType", kParams_SteamNetworkingUtils004_22},
    {"SteamAPI_ISteamNetworkingUtils_SteamNetworkingIdentity_ToString", kParams_SteamNetworkingUtils004_23},
    {"SteamAPI_ISteamNetworkingUtils_SteamNetworkingIdentity_ParseString", kParams_SteamNetworkingUtils004_24},
};

class Version_SteamNetworkingUtils004 {
public:
    virtual void* AllocateMessage(std::int32_t cbAllocateBuffer) {
        return steambridge::slot<void*>(kSlots_SteamNetworkingUtils004[0], cbAllocateBuffer);
    }
    virtual std::int32_t GetRelayNetworkStatus(void* pDetails) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingUtils004[1], pDetails);
    }
    virtual float GetLocalPingLocation(void* result) {
        return steambridge::slot<float>(kSlots_SteamNetworkingUtils004[2], result);
    }
    virtual std::int32_t EstimatePingTimeBetweenTwoLocations(void* location1, void* location2) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingUtils004[3], location1, location2);
    }
    virtual std::int32_t EstimatePingTimeFromLocalHost(void* remoteLocation) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingUtils004[4], remoteLocation);
    }
    virtual void ConvertPingLocationToString(void* location, void* pszBuf, std::int32_t cchBufSize) {
        steambridge::slot<void>(kSlots_SteamNetworkingUtils004[5], location, pszBuf, cchBufSize);
    }
    virtual bool ParsePingLocationString(const char* pszString, void* result) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingUtils004[6], pszString, result);
    }
    virtual bool CheckPingDataUpToDate(float flMaxAgeSeconds) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingUtils004[7], flMaxAgeSeconds);
    }
    virtual std::int32_t GetPingToDataCenter(std::uint32_t popID, std::uint32_t* pViaRelayPoP) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingUtils004[8], popID, pViaRelayPoP);
    }
    virtual std::int32_t GetDirectPingToPOP(std::uint32_t popID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingUtils004[9], popID);
    }
    virtual std::int32_t GetPOPCount() {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingUtils004[10]);
    }
    virtual std::int32_t GetPOPList(std::uint32_t* list, std::int32_t nListSz) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingUtils004[11], list, nListSz);
    }
    virtual std::int64_t GetLocalTimestamp() {
        return steambridge::slot<std::int64_t>(kSlots_SteamNetworkingUtils004[12]);
    }
    virtual void SetDebugOutputFunction(std::int32_t eDetailLevel, void* pfnFunc) {
        steambridge::slot<void>(kSlots_SteamNetworkingUtils004[13], eDetailLevel, pfnFunc);
    }
    virtual std::int32_t GetIPv4FakeIPType(std::uint32_t nIPv4) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingUtils004[14], nIPv4);
    }
    virtual std::int32_t GetRealIdentityForFakeIP(void* fakeIP, void* pOutRealIdentity) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingUtils004[15], fakeIP, pOutRealIdentity);
    }
    virtual bool SetConfigValue(std::int32_t eValue, std::int32_t eScopeType, std::intptr_t scopeObj, std::int32_t eDataType, void* pArg) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingUtils004[16], eValue, eScopeType, scopeObj, eDataType, pArg);
    }
    virtual std::int32_t GetConfigValue(std::int32_t eValue, std::int32_t eScopeType, std::intptr_t scopeObj, std::int32_t* pOutDataType, void* pResult, std::size_t* cbResult) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingUtils004[17], eValue, eScopeType, scopeObj, pOutDataType, pResult, cbResult);
    }
    virtual const char* GetConfigValueInfo(std::int32_t eValue, std::int32_t* pOutDataType, std::int32_t* pOutScope) {
        return steambridge::slot<const char*>(kSlots_SteamNetworkingUtils004[18], eValue, pOutDataType, pOutScope);
    }
    virtual std::int32_t IterateGenericEditableConfigValues(std::int32_t eCurrent, bool bEnumerateDevVars) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingUtils004[19], eCurrent, bEnumerateDevVars);
    }
    virtual void SteamNetworkingIPAddr_ToString(void* addr, void* buf, std::size_t cbBuf, bool bWithPort) {
        steambridge::slot<void>(kSlots_SteamNetworkingUtils004[20], addr, buf, cbBuf, bWithPort);
    }
    virtual bool SteamNetworkingIPAddr_ParseString(void* pAddr, const char* pszStr) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingUtils004[21], pAddr, pszStr);
    }
    virtual std::int32_t SteamNetworkingIPAddr_GetFakeIPType(void* addr) {
        return steambridge::slot<std::int32_t>(kSlots_SteamNetworkingUtils004[22], addr);
    }
    virtual void SteamNetworkingIdentity_ToString(void* identity, void* buf, std::size_t cbBuf) {
        steambridge::slot<void>(kSlots_SteamNetworkingUtils004[23], identity, buf, cbBuf);
    }
    virtual bool SteamNetworkingIdentity_ParseString(void* pIdentity, const char* pszStr) {
        return steambridge::slot<bool>(kSlots_SteamNetworkingUtils004[24], pIdentity, pszStr);
    }
};

Version_SteamNetworkingUtils004 g_SteamNetworkingUtils004;

const char* const kParams_STEAMPARENTALSETTINGS_INTERFACE_VERSION001_2[] = {
    "nAppID",
};
const char* const kParams_STEAMPARENTALSETTINGS_INTERFACE_VERSION001_3[] = {
    "nAppID",
};
const char* const kParams_STEAMPARENTALSETTINGS_INTERFACE_VERSION001_4[] = {
    "eFeature",
};
const char* const kParams_STEAMPARENTALSETTINGS_INTERFACE_VERSION001_5[] = {
    "eFeature",
};
// ISteamParentalSettings STEAMPARENTALSETTINGS_INTERFACE_VERSION001
const steambridge::SlotInfo kSlots_STEAMPARENTALSETTINGS_INTERFACE_VERSION001[] = {
    {"SteamAPI_ISteamParentalSettings_BIsParentalLockEnabled", nullptr},
    {"SteamAPI_ISteamParentalSettings_BIsParentalLockLocked", nullptr},
    {"SteamAPI_ISteamParentalSettings_BIsAppBlocked", kParams_STEAMPARENTALSETTINGS_INTERFACE_VERSION001_2},
    {"SteamAPI_ISteamParentalSettings_BIsAppInBlockList", kParams_STEAMPARENTALSETTINGS_INTERFACE_VERSION001_3},
    {"SteamAPI_ISteamParentalSettings_BIsFeatureBlocked", kParams_STEAMPARENTALSETTINGS_INTERFACE_VERSION001_4},
    {"SteamAPI_ISteamParentalSettings_BIsFeatureInBlockList", kParams_STEAMPARENTALSETTINGS_INTERFACE_VERSION001_5},
};

class Version_STEAMPARENTALSETTINGS_INTERFACE_VERSION001 {
public:
    virtual bool BIsParentalLockEnabled() {
        return steambridge::slot<bool>(kSlots_STEAMPARENTALSETTINGS_INTERFACE_VERSION001[0]);
    }
    virtual bool BIsParentalLockLocked() {
        return steambridge::slot<bool>(kSlots_STEAMPARENTALSETTINGS_INTERFACE_VERSION001[1]);
    }
    virtual bool BIsAppBlocked(std::uint32_t nAppID) {
        return steambridge::slot<bool>(kSlots_STEAMPARENTALSETTINGS_INTERFACE_VERSION001[2], nAppID);
    }
    virtual bool BIsAppInBlockList(std::uint32_t nAppID) {
        return steambridge::slot<bool>(kSlots_STEAMPARENTALSETTINGS_INTERFACE_VERSION001[3], nAppID);
    }
    virtual bool BIsFeatureBlocked(std::int32_t eFeature) {
        return steambridge::slot<bool>(kSlots_STEAMPARENTALSETTINGS_INTERFACE_VERSION001[4], eFeature);
    }
    virtual bool BIsFeatureInBlockList(std::int32_t eFeature) {
        return steambridge::slot<bool>(kSlots_STEAMPARENTALSETTINGS_INTERFACE_VERSION001[5], eFeature);
    }
};

Version_STEAMPARENTALSETTINGS_INTERFACE_VERSION001 g_STEAMPARENTALSETTINGS_INTERFACE_VERSION001;

const char* const kParams_SteamParties002_1[] = {
    "unIndex",
};
const char* const kParams_SteamParties002_2[] = {
    "ulBeaconID",
    "pSteamIDBeaconOwner",
    "pLocation",
    "pchMetadata",
    "cchMetadata",
};
const char* const kParams_SteamParties002_3[] = {
    "ulBeaconID",
};
const char* const kParams_SteamParties002_4[] = {
    "puNumLocations",
};
const char* const kParams_SteamParties002_5[] = {
    "pLocationList",
    "uMaxNumLocations",
};
const char* const kParams_SteamParties002_6[] = {
    "unOpenSlots",
    "pBeaconLocation",
    "pchConnectString",
    "pchMetadata",
};
const char* const kParams_SteamParties002_7[] = {
    "ulBeacon",
    "steamIDUser",
};
const char* const kParams_SteamParties002_8[] = {
    "ulBeacon",
    "steamIDUser",
};
const char* const kParams_SteamParties002_9[] = {
    "ulBeacon",
    "unOpenSlots",
};
const char* const kParams_SteamParties002_10[] = {
    "ulBeacon",
};
const char* const kParams_SteamParties002_11[] = {
    "BeaconLocation",
    "eData",
    "pchDataStringOut",
    "cchDataStringOut",
};
// ISteamParties SteamParties002
const steambridge::SlotInfo kSlots_SteamParties002[] = {
    {"SteamAPI_ISteamParties_GetNumActiveBeacons", nullptr},
    {"SteamAPI_ISteamParties_GetBeaconByIndex", kParams_SteamParties002_1},
    {"SteamAPI_ISteamParties_GetBeaconDetails", kParams_SteamParties002_2},
    {"SteamAPI_ISteamParties_JoinParty", kParams_SteamParties002_3},
    {"SteamAPI_ISteamParties_GetNumAvailableBeaconLocations", kParams_SteamParties002_4},
    {"SteamAPI_ISteamParties_GetAvailableBeaconLocations", kParams_SteamParties002_5},
    {"SteamAPI_ISteamParties_CreateBeacon", kParams_SteamParties002_6},
    {"SteamAPI_ISteamParties_OnReservationCompleted", kParams_SteamParties002_7},
    {"SteamAPI_ISteamParties_CancelReservation", kParams_SteamParties002_8},
    {"SteamAPI_ISteamParties_ChangeNumOpenSlots", kParams_SteamParties002_9},
    {"SteamAPI_ISteamParties_DestroyBeacon", kParams_SteamParties002_10},
    {"SteamAPI_ISteamParties_GetBeaconLocationData", kParams_SteamParties002_11},
};

class Version_SteamParties002 {
public:
    virtual std::uint32_t GetNumActiveBeacons() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamParties002[0]);
    }
    virtual std::uint64_t GetBeaconByIndex(std::uint32_t unIndex) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamParties002[1], unIndex);
    }
    virtual bool GetBeaconDetails(std::uint64_t ulBeaconID, CSteamID* pSteamIDBeaconOwner, void* pLocation, void* pchMetadata, std::int32_t cchMetadata) {
        return steambridge::slot<bool>(kSlots_SteamParties002[2], ulBeaconID, pSteamIDBeaconOwner, pLocation, pchMetadata, cchMetadata);
    }
    virtual std::uint64_t JoinParty(std::uint64_t ulBeaconID) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamParties002[3], ulBeaconID);
    }
    virtual bool GetNumAvailableBeaconLocations(std::uint32_t* puNumLocations) {
        return steambridge::slot<bool>(kSlots_SteamParties002[4], puNumLocations);
    }
    virtual bool GetAvailableBeaconLocations(void* pLocationList, std::uint32_t uMaxNumLocations) {
        return steambridge::slot<bool>(kSlots_SteamParties002[5], pLocationList, uMaxNumLocations);
    }
    virtual std::uint64_t CreateBeacon(std::uint32_t unOpenSlots, void* pBeaconLocation, const char* pchConnectString, const char* pchMetadata) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamParties002[6], unOpenSlots, pBeaconLocation, pchConnectString, pchMetadata);
    }
    virtual void OnReservationCompleted(std::uint64_t ulBeacon, CSteamID steamIDUser) {
        steambridge::slot<void>(kSlots_SteamParties002[7], ulBeacon, steamIDUser);
    }
    virtual void CancelReservation(std::uint64_t ulBeacon, CSteamID steamIDUser) {
        steambridge::slot<void>(kSlots_SteamParties002[8], ulBeacon, steamIDUser);
    }
    virtual std::uint64_t ChangeNumOpenSlots(std::uint64_t ulBeacon, std::uint32_t unOpenSlots) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamParties002[9], ulBeacon, unOpenSlots);
    }
    virtual bool DestroyBeacon(std::uint64_t ulBeacon) {
        return steambridge::slot<bool>(kSlots_SteamParties002[10], ulBeacon);
    }
    virtual bool GetBeaconLocationData(SteamPartyBeaconLocation_t BeaconLocation, std::int32_t eData, void* pchDataStringOut, std::int32_t cchDataStringOut) {
        return steambridge::slot<bool>(kSlots_SteamParties002[11], BeaconLocation, eData, pchDataStringOut, cchDataStringOut);
    }
};

Version_SteamParties002 g_SteamParties002;

const char* const kParams_STEAMREMOTEPLAY_INTERFACE_VERSION001_1[] = {
    "iSessionIndex",
};
const char* const kParams_STEAMREMOTEPLAY_INTERFACE_VERSION001_2[] = {
    "unSessionID",
};
const char* const kParams_STEAMREMOTEPLAY_INTERFACE_VERSION001_3[] = {
    "unSessionID",
};
const char* const kParams_STEAMREMOTEPLAY_INTERFACE_VERSION001_4[] = {
    "unSessionID",
};
const char* const kParams_STEAMREMOTEPLAY_INTERFACE_VERSION001_5[] = {
    "unSessionID",
    "pnResolutionX",
    "pnResolutionY",
};
const char* const kParams_STEAMREMOTEPLAY_INTERFACE_VERSION001_6[] = {
    "steamIDFriend",
};
// ISteamRemotePlay STEAMREMOTEPLAY_INTERFACE_VERSION001
const steambridge::SlotInfo kSlots_STEAMREMOTEPLAY_INTERFACE_VERSION001[] = {
    {"SteamAPI_ISteamRemotePlay_GetSessionCount", nullptr},
    {"SteamAPI_ISteamRemotePlay_GetSessionID", kParams_STEAMREMOTEPLAY_INTERFACE_VERSION001_1},
    {"SteamAPI_ISteamRemotePlay_GetSessionSteamID", kParams_STEAMREMOTEPLAY_INTERFACE_VERSION001_2},
    {"SteamAPI_ISteamRemotePlay_GetSessionClientName", kParams_STEAMREMOTEPLAY_INTERFACE_VERSION001_3},
    {"SteamAPI_ISteamRemotePlay_GetSessionClientFormFactor", kParams_STEAMREMOTEPLAY_INTERFACE_VERSION001_4},
    {"SteamAPI_ISteamRemotePlay_BGetSessionClientResolution", kParams_STEAMREMOTEPLAY_INTERFACE_VERSION001_5},
    {"SteamAPI_ISteamRemotePlay_BSendRemotePlayTogetherInvite", kParams_STEAMREMOTEPLAY_INTERFACE_VERSION001_6},
};

class Version_STEAMREMOTEPLAY_INTERFACE_VERSION001 {
public:
    virtual std::uint32_t GetSessionCount() {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMREMOTEPLAY_INTERFACE_VERSION001[0]);
    }
    virtual std::uint32_t GetSessionID(std::int32_t iSessionIndex) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMREMOTEPLAY_INTERFACE_VERSION001[1], iSessionIndex);
    }
    virtual CSteamID GetSessionSteamID(std::uint32_t unSessionID) {
        return steambridge::slot<CSteamID>(kSlots_STEAMREMOTEPLAY_INTERFACE_VERSION001[2], unSessionID);
    }
    virtual const char* GetSessionClientName(std::uint32_t unSessionID) {
        return steambridge::slot<const char*>(kSlots_STEAMREMOTEPLAY_INTERFACE_VERSION001[3], unSessionID);
    }
    virtual std::int32_t GetSessionClientFormFactor(std::uint32_t unSessionID) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMREMOTEPLAY_INTERFACE_VERSION001[4], unSessionID);
    }
    virtual bool BGetSessionClientResolution(std::uint32_t unSessionID, std::int32_t* pnResolutionX, std::int32_t* pnResolutionY) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTEPLAY_INTERFACE_VERSION001[5], unSessionID, pnResolutionX, pnResolutionY);
    }
    virtual bool BSendRemotePlayTogetherInvite(CSteamID steamIDFriend) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTEPLAY_INTERFACE_VERSION001[6], steamIDFriend);
    }
};

Version_STEAMREMOTEPLAY_INTERFACE_VERSION001 g_STEAMREMOTEPLAY_INTERFACE_VERSION001;

const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_0[] = {
    "pchFile",
    "pvData",
    "cubData",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_1[] = {
    "pchFile",
    "pvData",
    "cubDataToRead",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_2[] = {
    "pchFile",
    "pvData",
    "cubData",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_3[] = {
    "pchFile",
    "nOffset",
    "cubToRead",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_4[] = {
    "hReadCall",
    "pvBuffer",
    "cubToRead",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_5[] = {
    "pchFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_6[] = {
    "pchFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_7[] = {
    "pchFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_8[] = {
    "pchFile",
    "eRemoteStoragePlatform",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_9[] = {
    "pchFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_10[] = {
    "writeHandle",
    "pvData",
    "cubData",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_11[] = {
    "writeHandle",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_12[] = {
    "writeHandle",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_13[] = {
    "pchFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_14[] = {
    "pchFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_15[] = {
    "pchFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_16[] = {
    "pchFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_17[] = {
    "pchFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_19[] = {
    "iFile",
    "pnFileSizeInBytes",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_20[] = {
    "pnTotalBytes",
    "puAvailableBytes",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_23[] = {
    "bEnabled",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_24[] = {
    "hContent",
    "unPriority",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_25[] = {
    "hContent",
    "pnBytesDownloaded",
    "pnBytesExpected",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_26[] = {
    "hContent",
    "pnAppID",
    "ppchName",
    "pnFileSizeInBytes",
    "pSteamIDOwner",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_27[] = {
    "hContent",
    "pvData",
    "cubDataToRead",
    "cOffset",
    "eAction",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_29[] = {
    "iCachedContent",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_30[] = {
    "pchFile",
    "pchPreviewFile",
    "nConsumerAppId",
    "pchTitle",
    "pchDescription",
    "eVisibility",
    "pTags",
    "eWorkshopFileType",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_31[] = {
    "unPublishedFileId",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_32[] = {
    "updateHandle",
    "pchFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_33[] = {
    "updateHandle",
    "pchPreviewFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_34[] = {
    "updateHandle",
    "pchTitle",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_35[] = {
    "updateHandle",
    "pchDescription",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_36[] = {
    "updateHandle",
    "eVisibility",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_37[] = {
    "updateHandle",
    "pTags",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_38[] = {
    "updateHandle",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_39[] = {
    "unPublishedFileId",
    "unMaxSecondsOld",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_40[] = {
    "unPublishedFileId",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_41[] = {
    "unStartIndex",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_42[] = {
    "unPublishedFileId",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_43[] = {
    "unStartIndex",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_44[] = {
    "unPublishedFileId",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_45[] = {
    "updateHandle",
    "pchChangeDescription",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_46[] = {
    "unPublishedFileId",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_47[] = {
    "unPublishedFileId",
    "bVoteUp",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_48[] = {
    "unPublishedFileId",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_49[] = {
    "steamId",
    "unStartIndex",
    "pRequiredTags",
    "pExcludedTags",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_50[] = {
    "eVideoProvider",
    "pchVideoAccount",
    "pchVideoIdentifier",
    "pchPreviewFile",
    "nConsumerAppId",
    "pchTitle",
    "pchDescription",
    "eVisibility",
    "pTags",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_51[] = {
    "unPublishedFileId",
    "eAction",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_52[] = {
    "eAction",
    "unStartIndex",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_53[] = {
    "eEnumerationType",
    "unStartIndex",
    "unCount",
    "unDays",
    "pTags",
    "pUserTags",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_54[] = {
    "hContent",
    "pchLocation",
    "unPriority",
};
// ISteamRemoteStorage STEAMREMOTESTORAGE_INTERFACE_VERSION014
const steambridge::SlotInfo kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[] = {
    {"SteamAPI_ISteamRemoteStorage_FileWrite", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_0},
    {"SteamAPI_ISteamRemoteStorage_FileRead", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_1},
    {"SteamAPI_ISteamRemoteStorage_FileWriteAsync", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_2},
    {"SteamAPI_ISteamRemoteStorage_FileReadAsync", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_3},
    {"SteamAPI_ISteamRemoteStorage_FileReadAsyncComplete", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_4},
    {"SteamAPI_ISteamRemoteStorage_FileForget", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_5},
    {"SteamAPI_ISteamRemoteStorage_FileDelete", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_6},
    {"SteamAPI_ISteamRemoteStorage_FileShare", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_7},
    {"SteamAPI_ISteamRemoteStorage_SetSyncPlatforms", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_8},
    {"SteamAPI_ISteamRemoteStorage_FileWriteStreamOpen", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_9},
    {"SteamAPI_ISteamRemoteStorage_FileWriteStreamWriteChunk", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_10},
    {"SteamAPI_ISteamRemoteStorage_FileWriteStreamClose", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_11},
    {"SteamAPI_ISteamRemoteStorage_FileWriteStreamCancel", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_12},
    {"SteamAPI_ISteamRemoteStorage_FileExists", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_13},
    {"SteamAPI_ISteamRemoteStorage_FilePersisted", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_14},
    {"SteamAPI_ISteamRemoteStorage_GetFileSize", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_15},
    {"SteamAPI_ISteamRemoteStorage_GetFileTimestamp", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_16},
    {"SteamAPI_ISteamRemoteStorage_GetSyncPlatforms", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_17},
    {"SteamAPI_ISteamRemoteStorage_GetFileCount", nullptr},
    {"SteamAPI_ISteamRemoteStorage_GetFileNameAndSize", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_19},
    {"SteamAPI_ISteamRemoteStorage_GetQuota", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_20},
    {"SteamAPI_ISteamRemoteStorage_IsCloudEnabledForAccount", nullptr},
    {"SteamAPI_ISteamRemoteStorage_IsCloudEnabledForApp", nullptr},
    {"SteamAPI_ISteamRemoteStorage_SetCloudEnabledForApp", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_23},
    {"SteamAPI_ISteamRemoteStorage_UGCDownload", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_24},
    {"SteamAPI_ISteamRemoteStorage_GetUGCDownloadProgress", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_25},
    {"SteamAPI_ISteamRemoteStorage_GetUGCDetails", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_26},
    {"SteamAPI_ISteamRemoteStorage_UGCRead", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_27},
    {"SteamAPI_ISteamRemoteStorage_GetCachedUGCCount", nullptr},
    {"SteamAPI_ISteamRemoteStorage_GetCachedUGCHandle", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_29},
    {"SteamAPI_ISteamRemoteStorage_PublishWorkshopFile", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_30},
    {"SteamAPI_ISteamRemoteStorage_CreatePublishedFileUpdateRequest", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_31},
    {"SteamAPI_ISteamRemoteStorage_UpdatePublishedFileFile", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_32},
    {"SteamAPI_ISteamRemoteStorage_UpdatePublishedFilePreviewFile", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_33},
    {"SteamAPI_ISteamRemoteStorage_UpdatePublishedFileTitle", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_34},
    {"SteamAPI_ISteamRemoteStorage_UpdatePublishedFileDescription", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_35},
    {"SteamAPI_ISteamRemoteStorage_UpdatePublishedFileVisibility", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_36},
    {"SteamAPI_ISteamRemoteStorage_UpdatePublishedFileTags", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_37},
    {"SteamAPI_ISteamRemoteStorage_CommitPublishedFileUpdate", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_38},
    {"SteamAPI_ISteamRemoteStorage_GetPublishedFileDetails", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_39},
    {"SteamAPI_ISteamRemoteStorage_DeletePublishedFile", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_40},
    {"SteamAPI_ISteamRemoteStorage_EnumerateUserPublishedFiles", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_41},
    {"SteamAPI_ISteamRemoteStorage_SubscribePublishedFile", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_42},
    {"SteamAPI_ISteamRemoteStorage_EnumerateUserSubscribedFiles", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_43},
    {"SteamAPI_ISteamRemoteStorage_UnsubscribePublishedFile", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_44},
    {"SteamAPI_ISteamRemoteStorage_UpdatePublishedFileSetChangeDescription", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_45},
    {"SteamAPI_ISteamRemoteStorage_GetPublishedItemVoteDetails", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_46},
    {"SteamAPI_ISteamRemoteStorage_UpdateUserPublishedItemVote", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_47},
    {"SteamAPI_ISteamRemoteStorage_GetUserPublishedItemVoteDetails", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_48},
    {"SteamAPI_ISteamRemoteStorage_EnumerateUserSharedWorkshopFiles", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_49},
    {"SteamAPI_ISteamRemoteStorage_PublishVideo", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_50},
    {"SteamAPI_ISteamRemoteStorage_SetUserPublishedFileAction", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_51},
    {"SteamAPI_ISteamRemoteStorage_EnumeratePublishedFilesByUserAction", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_52},
    {"SteamAPI_ISteamRemoteStorage_EnumeratePublishedWorkshopFiles", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_53},
    {"SteamAPI_ISteamRemoteStorage_UGCDownloadToLocation", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION014_54},
};

class Version_STEAMREMOTESTORAGE_INTERFACE_VERSION014 {
public:
    virtual bool FileWrite(const char* pchFile, void* pvData, std::int32_t cubData) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[0], pchFile, pvData, cubData);
    }
    virtual std::int32_t FileRead(const char* pchFile, void* pvData, std::int32_t cubDataToRead) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[1], pchFile, pvData, cubDataToRead);
    }
    virtual std::uint64_t FileWriteAsync(const char* pchFile, void* pvData, std::uint32_t cubData) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[2], pchFile, pvData, cubData);
    }
    virtual std::uint64_t FileReadAsync(const char* pchFile, std::uint32_t nOffset, std::uint32_t cubToRead) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[3], pchFile, nOffset, cubToRead);
    }
    virtual bool FileReadAsyncComplete(std::uint64_t hReadCall, void* pvBuffer, std::uint32_t cubToRead) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[4], hReadCall, pvBuffer, cubToRead);
    }
    virtual bool FileForget(const char* pchFile) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[5], pchFile);
    }
    virtual bool FileDelete(const char* pchFile) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[6], pchFile);
    }
    virtual std::uint64_t FileShare(const char* pchFile) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[7], pchFile);
    }
    virtual bool SetSyncPlatforms(const char* pchFile, std::int32_t eRemoteStoragePlatform) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[8], pchFile, eRemoteStoragePlatform);
    }
    virtual std::uint64_t FileWriteStreamOpen(const char* pchFile) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[9], pchFile);
    }
    virtual bool FileWriteStreamWriteChunk(std::uint64_t writeHandle, void* pvData, std::int32_t cubData) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[10], writeHandle, pvData, cubData);
    }
    virtual bool FileWriteStreamClose(std::uint64_t writeHandle) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[11], writeHandle);
    }
    virtual bool FileWriteStreamCancel(std::uint64_t writeHandle) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[12], writeHandle);
    }
    virtual bool FileExists(const char* pchFile) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[13], pchFile);
    }
    virtual bool FilePersisted(const char* pchFile) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[14], pchFile);
    }
    virtual std::int32_t GetFileSize(const char* pchFile) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[15], pchFile);
    }
    virtual std::int64_t GetFileTimestamp(const char* pchFile) {
        return steambridge::slot<std::int64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[16], pchFile);
    }
    virtual std::int32_t GetSyncPlatforms(const char* pchFile) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[17], pchFile);
    }
    virtual std::int32_t GetFileCount() {
        return steambridge::slot<std::int32_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[18]);
    }
    virtual const char* GetFileNameAndSize(std::int32_t iFile, std::int32_t* pnFileSizeInBytes) {
        return steambridge::slot<const char*>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[19], iFile, pnFileSizeInBytes);
    }
    virtual bool GetQuota(std::uint64_t* pnTotalBytes, std::uint64_t* puAvailableBytes) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[20], pnTotalBytes, puAvailableBytes);
    }
    virtual bool IsCloudEnabledForAccount() {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[21]);
    }
    virtual bool IsCloudEnabledForApp() {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[22]);
    }
    virtual void SetCloudEnabledForApp(bool bEnabled) {
        steambridge::slot<void>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[23], bEnabled);
    }
    virtual std::uint64_t UGCDownload(std::uint64_t hContent, std::uint32_t unPriority) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[24], hContent, unPriority);
    }
    virtual bool GetUGCDownloadProgress(std::uint64_t hContent, std::int32_t* pnBytesDownloaded, std::int32_t* pnBytesExpected) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[25], hContent, pnBytesDownloaded, pnBytesExpected);
    }
    virtual bool GetUGCDetails(std::uint64_t hContent, std::uint32_t* pnAppID, void* ppchName, std::int32_t* pnFileSizeInBytes, CSteamID* pSteamIDOwner) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[26], hContent, pnAppID, ppchName, pnFileSizeInBytes, pSteamIDOwner);
    }
    virtual std::int32_t UGCRead(std::uint64_t hContent, void* pvData, std::int32_t cubDataToRead, std::uint32_t cOffset, std::int32_t eAction) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[27], hContent, pvData, cubDataToRead, cOffset, eAction);
    }
    virtual std::int32_t GetCachedUGCCount() {
        return steambridge::slot<std::int32_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[28]);
    }
    virtual std::uint64_t GetCachedUGCHandle(std::int32_t iCachedContent) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[29], iCachedContent);
    }
    virtual std::uint64_t PublishWorkshopFile(const char* pchFile, const char* pchPreviewFile, std::uint32_t nConsumerAppId, const char* pchTitle, const char* pchDescription, std::int32_t eVisibility, void* pTags, std::int32_t eWorkshopFileType) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[30], pchFile, pchPreviewFile, nConsumerAppId, pchTitle, pchDescription, eVisibility, pTags, eWorkshopFileType);
    }
    virtual std::uint64_t CreatePublishedFileUpdateRequest(std::uint64_t unPublishedFileId) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[31], unPublishedFileId);
    }
    virtual bool UpdatePublishedFileFile(std::uint64_t updateHandle, const char* pchFile) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[32], updateHandle, pchFile);
    }
    virtual bool UpdatePublishedFilePreviewFile(std::uint64_t updateHandle, const char* pchPreviewFile) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[33], updateHandle, pchPreviewFile);
    }
    virtual bool UpdatePublishedFileTitle(std::uint64_t updateHandle, const char* pchTitle) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[34], updateHandle, pchTitle);
    }
    virtual bool UpdatePublishedFileDescription(std::uint64_t updateHandle, const char* pchDescription) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[35], updateHandle, pchDescription);
    }
    virtual bool UpdatePublishedFileVisibility(std::uint64_t updateHandle, std::int32_t eVisibility) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[36], updateHandle, eVisibility);
    }
    virtual bool UpdatePublishedFileTags(std::uint64_t updateHandle, void* pTags) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[37], updateHandle, pTags);
    }
    virtual std::uint64_t CommitPublishedFileUpdate(std::uint64_t updateHandle) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[38], updateHandle);
    }
    virtual std::uint64_t GetPublishedFileDetails(std::uint64_t unPublishedFileId, std::uint32_t unMaxSecondsOld) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[39], unPublishedFileId, unMaxSecondsOld);
    }
    virtual std::uint64_t DeletePublishedFile(std::uint64_t unPublishedFileId) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[40], unPublishedFileId);
    }
    virtual std::uint64_t EnumerateUserPublishedFiles(std::uint32_t unStartIndex) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[41], unStartIndex);
    }
    virtual std::uint64_t SubscribePublishedFile(std::uint64_t unPublishedFileId) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[42], unPublishedFileId);
    }
    virtual std::uint64_t EnumerateUserSubscribedFiles(std::uint32_t unStartIndex) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[43], unStartIndex);
    }
    virtual std::uint64_t UnsubscribePublishedFile(std::uint64_t unPublishedFileId) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[44], unPublishedFileId);
    }
    virtual bool UpdatePublishedFileSetChangeDescription(std::uint64_t updateHandle, const char* pchChangeDescription) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[45], updateHandle, pchChangeDescription);
    }
    virtual std::uint64_t GetPublishedItemVoteDetails(std::uint64_t unPublishedFileId) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[46], unPublishedFileId);
    }
    virtual std::uint64_t UpdateUserPublishedItemVote(std::uint64_t unPublishedFileId, bool bVoteUp) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[47], unPublishedFileId, bVoteUp);
    }
    virtual std::uint64_t GetUserPublishedItemVoteDetails(std::uint64_t unPublishedFileId) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[48], unPublishedFileId);
    }
    virtual std::uint64_t EnumerateUserSharedWorkshopFiles(CSteamID steamId, std::uint32_t unStartIndex, void* pRequiredTags, void* pExcludedTags) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[49], steamId, unStartIndex, pRequiredTags, pExcludedTags);
    }
    virtual std::uint64_t PublishVideo(std::int32_t eVideoProvider, const char* pchVideoAccount, const char* pchVideoIdentifier, const char* pchPreviewFile, std::uint32_t nConsumerAppId, const char* pchTitle, const char* pchDescription, std::int32_t eVisibility, void* pTags) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[50], eVideoProvider, pchVideoAccount, pchVideoIdentifier, pchPreviewFile, nConsumerAppId, pchTitle, pchDescription, eVisibility, pTags);
    }
    virtual std::uint64_t SetUserPublishedFileAction(std::uint64_t unPublishedFileId, std::int32_t eAction) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[51], unPublishedFileId, eAction);
    }
    virtual std::uint64_t EnumeratePublishedFilesByUserAction(std::int32_t eAction, std::uint32_t unStartIndex) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[52], eAction, unStartIndex);
    }
    virtual std::uint64_t EnumeratePublishedWorkshopFiles(std::int32_t eEnumerationType, std::uint32_t unStartIndex, std::uint32_t unCount, std::uint32_t unDays, void* pTags, void* pUserTags) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[53], eEnumerationType, unStartIndex, unCount, unDays, pTags, pUserTags);
    }
    virtual std::uint64_t UGCDownloadToLocation(std::uint64_t hContent, const char* pchLocation, std::uint32_t unPriority) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION014[54], hContent, pchLocation, unPriority);
    }
};

Version_STEAMREMOTESTORAGE_INTERFACE_VERSION014 g_STEAMREMOTESTORAGE_INTERFACE_VERSION014;

const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_0[] = {
    "pchFile",
    "pvData",
    "cubData",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_1[] = {
    "pchFile",
    "pvData",
    "cubDataToRead",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_2[] = {
    "pchFile",
    "pvData",
    "cubData",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_3[] = {
    "pchFile",
    "nOffset",
    "cubToRead",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_4[] = {
    "hReadCall",
    "pvBuffer",
    "cubToRead",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_5[] = {
    "pchFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_6[] = {
    "pchFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_7[] = {
    "pchFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_8[] = {
    "pchFile",
    "eRemoteStoragePlatform",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_9[] = {
    "pchFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_10[] = {
    "writeHandle",
    "pvData",
    "cubData",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_11[] = {
    "writeHandle",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_12[] = {
    "writeHandle",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_13[] = {
    "pchFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_14[] = {
    "pchFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_15[] = {
    "pchFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_16[] = {
    "pchFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_17[] = {
    "pchFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_19[] = {
    "iFile",
    "pnFileSizeInBytes",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_20[] = {
    "pnTotalBytes",
    "puAvailableBytes",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_23[] = {
    "bEnabled",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_24[] = {
    "hContent",
    "unPriority",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_25[] = {
    "hContent",
    "pnBytesDownloaded",
    "pnBytesExpected",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_26[] = {
    "hContent",
    "pnAppID",
    "ppchName",
    "pnFileSizeInBytes",
    "pSteamIDOwner",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_27[] = {
    "hContent",
    "pvData",
    "cubDataToRead",
    "cOffset",
    "eAction",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_29[] = {
    "iCachedContent",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_30[] = {
    "pchFile",
    "pchPreviewFile",
    "nConsumerAppId",
    "pchTitle",
    "pchDescription",
    "eVisibility",
    "pTags",
    "eWorkshopFileType",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_31[] = {
    "unPublishedFileId",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_32[] = {
    "updateHandle",
    "pchFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_33[] = {
    "updateHandle",
    "pchPreviewFile",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_34[] = {
    "updateHandle",
    "pchTitle",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_35[] = {
    "updateHandle",
    "pchDescription",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_36[] = {
    "updateHandle",
    "eVisibility",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_37[] = {
    "updateHandle",
    "pTags",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_38[] = {
    "updateHandle",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_39[] = {
    "unPublishedFileId",
    "unMaxSecondsOld",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_40[] = {
    "unPublishedFileId",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_41[] = {
    "unStartIndex",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_42[] = {
    "unPublishedFileId",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_43[] = {
    "unStartIndex",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_44[] = {
    "unPublishedFileId",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_45[] = {
    "updateHandle",
    "pchChangeDescription",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_46[] = {
    "unPublishedFileId",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_47[] = {
    "unPublishedFileId",
    "bVoteUp",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_48[] = {
    "unPublishedFileId",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_49[] = {
    "steamId",
    "unStartIndex",
    "pRequiredTags",
    "pExcludedTags",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_50[] = {
    "eVideoProvider",
    "pchVideoAccount",
    "pchVideoIdentifier",
    "pchPreviewFile",
    "nConsumerAppId",
    "pchTitle",
    "pchDescription",
    "eVisibility",
    "pTags",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_51[] = {
    "unPublishedFileId",
    "eAction",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_52[] = {
    "eAction",
    "unStartIndex",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_53[] = {
    "eEnumerationType",
    "unStartIndex",
    "unCount",
    "unDays",
    "pTags",
    "pUserTags",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_54[] = {
    "hContent",
    "pchLocation",
    "unPriority",
};
const char* const kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_56[] = {
    "iFile",
    "pEChangeType",
    "pEFilePathType",
};
// ISteamRemoteStorage STEAMREMOTESTORAGE_INTERFACE_VERSION016
const steambridge::SlotInfo kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[] = {
    {"SteamAPI_ISteamRemoteStorage_FileWrite", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_0},
    {"SteamAPI_ISteamRemoteStorage_FileRead", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_1},
    {"SteamAPI_ISteamRemoteStorage_FileWriteAsync", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_2},
    {"SteamAPI_ISteamRemoteStorage_FileReadAsync", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_3},
    {"SteamAPI_ISteamRemoteStorage_FileReadAsyncComplete", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_4},
    {"SteamAPI_ISteamRemoteStorage_FileForget", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_5},
    {"SteamAPI_ISteamRemoteStorage_FileDelete", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_6},
    {"SteamAPI_ISteamRemoteStorage_FileShare", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_7},
    {"SteamAPI_ISteamRemoteStorage_SetSyncPlatforms", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_8},
    {"SteamAPI_ISteamRemoteStorage_FileWriteStreamOpen", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_9},
    {"SteamAPI_ISteamRemoteStorage_FileWriteStreamWriteChunk", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_10},
    {"SteamAPI_ISteamRemoteStorage_FileWriteStreamClose", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_11},
    {"SteamAPI_ISteamRemoteStorage_FileWriteStreamCancel", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_12},
    {"SteamAPI_ISteamRemoteStorage_FileExists", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_13},
    {"SteamAPI_ISteamRemoteStorage_FilePersisted", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_14},
    {"SteamAPI_ISteamRemoteStorage_GetFileSize", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_15},
    {"SteamAPI_ISteamRemoteStorage_GetFileTimestamp", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_16},
    {"SteamAPI_ISteamRemoteStorage_GetSyncPlatforms", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_17},
    {"SteamAPI_ISteamRemoteStorage_GetFileCount", nullptr},
    {"SteamAPI_ISteamRemoteStorage_GetFileNameAndSize", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_19},
    {"SteamAPI_ISteamRemoteStorage_GetQuota", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_20},
    {"SteamAPI_ISteamRemoteStorage_IsCloudEnabledForAccount", nullptr},
    {"SteamAPI_ISteamRemoteStorage_IsCloudEnabledForApp", nullptr},
    {"SteamAPI_ISteamRemoteStorage_SetCloudEnabledForApp", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_23},
    {"SteamAPI_ISteamRemoteStorage_UGCDownload", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_24},
    {"SteamAPI_ISteamRemoteStorage_GetUGCDownloadProgress", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_25},
    {"SteamAPI_ISteamRemoteStorage_GetUGCDetails", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_26},
    {"SteamAPI_ISteamRemoteStorage_UGCRead", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_27},
    {"SteamAPI_ISteamRemoteStorage_GetCachedUGCCount", nullptr},
    {"SteamAPI_ISteamRemoteStorage_GetCachedUGCHandle", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_29},
    {"SteamAPI_ISteamRemoteStorage_PublishWorkshopFile", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_30},
    {"SteamAPI_ISteamRemoteStorage_CreatePublishedFileUpdateRequest", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_31},
    {"SteamAPI_ISteamRemoteStorage_UpdatePublishedFileFile", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_32},
    {"SteamAPI_ISteamRemoteStorage_UpdatePublishedFilePreviewFile", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_33},
    {"SteamAPI_ISteamRemoteStorage_UpdatePublishedFileTitle", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_34},
    {"SteamAPI_ISteamRemoteStorage_UpdatePublishedFileDescription", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_35},
    {"SteamAPI_ISteamRemoteStorage_UpdatePublishedFileVisibility", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_36},
    {"SteamAPI_ISteamRemoteStorage_UpdatePublishedFileTags", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_37},
    {"SteamAPI_ISteamRemoteStorage_CommitPublishedFileUpdate", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_38},
    {"SteamAPI_ISteamRemoteStorage_GetPublishedFileDetails", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_39},
    {"SteamAPI_ISteamRemoteStorage_DeletePublishedFile", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_40},
    {"SteamAPI_ISteamRemoteStorage_EnumerateUserPublishedFiles", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_41},
    {"SteamAPI_ISteamRemoteStorage_SubscribePublishedFile", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_42},
    {"SteamAPI_ISteamRemoteStorage_EnumerateUserSubscribedFiles", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_43},
    {"SteamAPI_ISteamRemoteStorage_UnsubscribePublishedFile", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_44},
    {"SteamAPI_ISteamRemoteStorage_UpdatePublishedFileSetChangeDescription", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_45},
    {"SteamAPI_ISteamRemoteStorage_GetPublishedItemVoteDetails", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_46},
    {"SteamAPI_ISteamRemoteStorage_UpdateUserPublishedItemVote", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_47},
    {"SteamAPI_ISteamRemoteStorage_GetUserPublishedItemVoteDetails", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_48},
    {"SteamAPI_ISteamRemoteStorage_EnumerateUserSharedWorkshopFiles", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_49},
    {"SteamAPI_ISteamRemoteStorage_PublishVideo", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_50},
    {"SteamAPI_ISteamRemoteStorage_SetUserPublishedFileAction", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_51},
    {"SteamAPI_ISteamRemoteStorage_EnumeratePublishedFilesByUserAction", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_52},
    {"SteamAPI_ISteamRemoteStorage_EnumeratePublishedWorkshopFiles", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_53},
    {"SteamAPI_ISteamRemoteStorage_UGCDownloadToLocation", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_54},
    {"SteamAPI_ISteamRemoteStorage_GetLocalFileChangeCount", nullptr},
    {"SteamAPI_ISteamRemoteStorage_GetLocalFileChange", kParams_STEAMREMOTESTORAGE_INTERFACE_VERSION016_56},
    {"SteamAPI_ISteamRemoteStorage_BeginFileWriteBatch", nullptr},
    {"SteamAPI_ISteamRemoteStorage_EndFileWriteBatch", nullptr},
};

class Version_STEAMREMOTESTORAGE_INTERFACE_VERSION016 {
public:
    virtual bool FileWrite(const char* pchFile, void* pvData, std::int32_t cubData) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[0], pchFile, pvData, cubData);
    }
    virtual std::int32_t FileRead(const char* pchFile, void* pvData, std::int32_t cubDataToRead) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[1], pchFile, pvData, cubDataToRead);
    }
    virtual std::uint64_t FileWriteAsync(const char* pchFile, void* pvData, std::uint32_t cubData) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[2], pchFile, pvData, cubData);
    }
    virtual std::uint64_t FileReadAsync(const char* pchFile, std::uint32_t nOffset, std::uint32_t cubToRead) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[3], pchFile, nOffset, cubToRead);
    }
    virtual bool FileReadAsyncComplete(std::uint64_t hReadCall, void* pvBuffer, std::uint32_t cubToRead) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[4], hReadCall, pvBuffer, cubToRead);
    }
    virtual bool FileForget(const char* pchFile) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[5], pchFile);
    }
    virtual bool FileDelete(const char* pchFile) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[6], pchFile);
    }
    virtual std::uint64_t FileShare(const char* pchFile) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[7], pchFile);
    }
    virtual bool SetSyncPlatforms(const char* pchFile, std::int32_t eRemoteStoragePlatform) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[8], pchFile, eRemoteStoragePlatform);
    }
    virtual std::uint64_t FileWriteStreamOpen(const char* pchFile) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[9], pchFile);
    }
    virtual bool FileWriteStreamWriteChunk(std::uint64_t writeHandle, void* pvData, std::int32_t cubData) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[10], writeHandle, pvData, cubData);
    }
    virtual bool FileWriteStreamClose(std::uint64_t writeHandle) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[11], writeHandle);
    }
    virtual bool FileWriteStreamCancel(std::uint64_t writeHandle) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[12], writeHandle);
    }
    virtual bool FileExists(const char* pchFile) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[13], pchFile);
    }
    virtual bool FilePersisted(const char* pchFile) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[14], pchFile);
    }
    virtual std::int32_t GetFileSize(const char* pchFile) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[15], pchFile);
    }
    virtual std::int64_t GetFileTimestamp(const char* pchFile) {
        return steambridge::slot<std::int64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[16], pchFile);
    }
    virtual std::int32_t GetSyncPlatforms(const char* pchFile) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[17], pchFile);
    }
    virtual std::int32_t GetFileCount() {
        return steambridge::slot<std::int32_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[18]);
    }
    virtual const char* GetFileNameAndSize(std::int32_t iFile, std::int32_t* pnFileSizeInBytes) {
        return steambridge::slot<const char*>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[19], iFile, pnFileSizeInBytes);
    }
    virtual bool GetQuota(std::uint64_t* pnTotalBytes, std::uint64_t* puAvailableBytes) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[20], pnTotalBytes, puAvailableBytes);
    }
    virtual bool IsCloudEnabledForAccount() {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[21]);
    }
    virtual bool IsCloudEnabledForApp() {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[22]);
    }
    virtual void SetCloudEnabledForApp(bool bEnabled) {
        steambridge::slot<void>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[23], bEnabled);
    }
    virtual std::uint64_t UGCDownload(std::uint64_t hContent, std::uint32_t unPriority) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[24], hContent, unPriority);
    }
    virtual bool GetUGCDownloadProgress(std::uint64_t hContent, std::int32_t* pnBytesDownloaded, std::int32_t* pnBytesExpected) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[25], hContent, pnBytesDownloaded, pnBytesExpected);
    }
    virtual bool GetUGCDetails(std::uint64_t hContent, std::uint32_t* pnAppID, void* ppchName, std::int32_t* pnFileSizeInBytes, CSteamID* pSteamIDOwner) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[26], hContent, pnAppID, ppchName, pnFileSizeInBytes, pSteamIDOwner);
    }
    virtual std::int32_t UGCRead(std::uint64_t hContent, void* pvData, std::int32_t cubDataToRead, std::uint32_t cOffset, std::int32_t eAction) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[27], hContent, pvData, cubDataToRead, cOffset, eAction);
    }
    virtual std::int32_t GetCachedUGCCount() {
        return steambridge::slot<std::int32_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[28]);
    }
    virtual std::uint64_t GetCachedUGCHandle(std::int32_t iCachedContent) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[29], iCachedContent);
    }
    virtual std::uint64_t PublishWorkshopFile(const char* pchFile, const char* pchPreviewFile, std::uint32_t nConsumerAppId, const char* pchTitle, const char* pchDescription, std::int32_t eVisibility, void* pTags, std::int32_t eWorkshopFileType) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[30], pchFile, pchPreviewFile, nConsumerAppId, pchTitle, pchDescription, eVisibility, pTags, eWorkshopFileType);
    }
    virtual std::uint64_t CreatePublishedFileUpdateRequest(std::uint64_t unPublishedFileId) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[31], unPublishedFileId);
    }
    virtual bool UpdatePublishedFileFile(std::uint64_t updateHandle, const char* pchFile) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[32], updateHandle, pchFile);
    }
    virtual bool UpdatePublishedFilePreviewFile(std::uint64_t updateHandle, const char* pchPreviewFile) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[33], updateHandle, pchPreviewFile);
    }
    virtual bool UpdatePublishedFileTitle(std::uint64_t updateHandle, const char* pchTitle) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[34], updateHandle, pchTitle);
    }
    virtual bool UpdatePublishedFileDescription(std::uint64_t updateHandle, const char* pchDescription) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[35], updateHandle, pchDescription);
    }
    virtual bool UpdatePublishedFileVisibility(std::uint64_t updateHandle, std::int32_t eVisibility) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[36], updateHandle, eVisibility);
    }
    virtual bool UpdatePublishedFileTags(std::uint64_t updateHandle, void* pTags) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[37], updateHandle, pTags);
    }
    virtual std::uint64_t CommitPublishedFileUpdate(std::uint64_t updateHandle) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[38], updateHandle);
    }
    virtual std::uint64_t GetPublishedFileDetails(std::uint64_t unPublishedFileId, std::uint32_t unMaxSecondsOld) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[39], unPublishedFileId, unMaxSecondsOld);
    }
    virtual std::uint64_t DeletePublishedFile(std::uint64_t unPublishedFileId) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[40], unPublishedFileId);
    }
    virtual std::uint64_t EnumerateUserPublishedFiles(std::uint32_t unStartIndex) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[41], unStartIndex);
    }
    virtual std::uint64_t SubscribePublishedFile(std::uint64_t unPublishedFileId) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[42], unPublishedFileId);
    }
    virtual std::uint64_t EnumerateUserSubscribedFiles(std::uint32_t unStartIndex) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[43], unStartIndex);
    }
    virtual std::uint64_t UnsubscribePublishedFile(std::uint64_t unPublishedFileId) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[44], unPublishedFileId);
    }
    virtual bool UpdatePublishedFileSetChangeDescription(std::uint64_t updateHandle, const char* pchChangeDescription) {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[45], updateHandle, pchChangeDescription);
    }
    virtual std::uint64_t GetPublishedItemVoteDetails(std::uint64_t unPublishedFileId) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[46], unPublishedFileId);
    }
    virtual std::uint64_t UpdateUserPublishedItemVote(std::uint64_t unPublishedFileId, bool bVoteUp) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[47], unPublishedFileId, bVoteUp);
    }
    virtual std::uint64_t GetUserPublishedItemVoteDetails(std::uint64_t unPublishedFileId) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[48], unPublishedFileId);
    }
    virtual std::uint64_t EnumerateUserSharedWorkshopFiles(CSteamID steamId, std::uint32_t unStartIndex, void* pRequiredTags, void* pExcludedTags) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[49], steamId, unStartIndex, pRequiredTags, pExcludedTags);
    }
    virtual std::uint64_t PublishVideo(std::int32_t eVideoProvider, const char* pchVideoAccount, const char* pchVideoIdentifier, const char* pchPreviewFile, std::uint32_t nConsumerAppId, const char* pchTitle, const char* pchDescription, std::int32_t eVisibility, void* pTags) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[50], eVideoProvider, pchVideoAccount, pchVideoIdentifier, pchPreviewFile, nConsumerAppId, pchTitle, pchDescription, eVisibility, pTags);
    }
    virtual std::uint64_t SetUserPublishedFileAction(std::uint64_t unPublishedFileId, std::int32_t eAction) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[51], unPublishedFileId, eAction);
    }
    virtual std::uint64_t EnumeratePublishedFilesByUserAction(std::int32_t eAction, std::uint32_t unStartIndex) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[52], eAction, unStartIndex);
    }
    virtual std::uint64_t EnumeratePublishedWorkshopFiles(std::int32_t eEnumerationType, std::uint32_t unStartIndex, std::uint32_t unCount, std::uint32_t unDays, void* pTags, void* pUserTags) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[53], eEnumerationType, unStartIndex, unCount, unDays, pTags, pUserTags);
    }
    virtual std::uint64_t UGCDownloadToLocation(std::uint64_t hContent, const char* pchLocation, std::uint32_t unPriority) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[54], hContent, pchLocation, unPriority);
    }
    virtual std::int32_t GetLocalFileChangeCount() {
        return steambridge::slot<std::int32_t>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[55]);
    }
    virtual const char* GetLocalFileChange(std::int32_t iFile, std::int32_t* pEChangeType, std::int32_t* pEFilePathType) {
        return steambridge::slot<const char*>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[56], iFile, pEChangeType, pEFilePathType);
    }
    virtual bool BeginFileWriteBatch() {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[57]);
    }
    virtual bool EndFileWriteBatch() {
        return steambridge::slot<bool>(kSlots_STEAMREMOTESTORAGE_INTERFACE_VERSION016[58]);
    }
};

Version_STEAMREMOTESTORAGE_INTERFACE_VERSION016 g_STEAMREMOTESTORAGE_INTERFACE_VERSION016;

const char* const kParams_STEAMSCREENSHOTS_INTERFACE_VERSION003_0[] = {
    "pubRGB",
    "cubRGB",
    "nWidth",
    "nHeight",
};
const char* const kParams_STEAMSCREENSHOTS_INTERFACE_VERSION003_1[] = {
    "pchFilename",
    "pchThumbnailFilename",
    "nWidth",
    "nHeight",
};
const char* const kParams_STEAMSCREENSHOTS_INTERFACE_VERSION003_3[] = {
    "bHook",
};
const char* const kParams_STEAMSCREENSHOTS_INTERFACE_VERSION003_4[] = {
    "hScreenshot",
    "pchLocation",
};
const char* const kParams_STEAMSCREENSHOTS_INTERFACE_VERSION003_5[] = {
    "hScreenshot",
    "steamID",
};
const char* const kParams_STEAMSCREENSHOTS_INTERFACE_VERSION003_6[] = {
    "hScreenshot",
    "unPublishedFileID",
};
const char* const kParams_STEAMSCREENSHOTS_INTERFACE_VERSION003_8[] = {
    "eType",
    "pchFilename",
    "pchVRFilename",
};
// ISteamScreenshots STEAMSCREENSHOTS_INTERFACE_VERSION003
const steambridge::SlotInfo kSlots_STEAMSCREENSHOTS_INTERFACE_VERSION003[] = {
    {"SteamAPI_ISteamScreenshots_WriteScreenshot", kParams_STEAMSCREENSHOTS_INTERFACE_VERSION003_0},
    {"SteamAPI_ISteamScreenshots_AddScreenshotToLibrary", kParams_STEAMSCREENSHOTS_INTERFACE_VERSION003_1},
    {"SteamAPI_ISteamScreenshots_TriggerScreenshot", nullptr},
    {"SteamAPI_ISteamScreenshots_HookScreenshots", kParams_STEAMSCREENSHOTS_INTERFACE_VERSION003_3},
    {"SteamAPI_ISteamScreenshots_SetLocation", kParams_STEAMSCREENSHOTS_INTERFACE_VERSION003_4},
    {"SteamAPI_ISteamScreenshots_TagUser", kParams_STEAMSCREENSHOTS_INTERFACE_VERSION003_5},
    {"SteamAPI_ISteamScreenshots_TagPublishedFile", kParams_STEAMSCREENSHOTS_INTERFACE_VERSION003_6},
    {"SteamAPI_ISteamScreenshots_IsScreenshotsHooked", nullptr},
    {"SteamAPI_ISteamScreenshots_AddVRScreenshotToLibrary", kParams_STEAMSCREENSHOTS_INTERFACE_VERSION003_8},
};

class Version_STEAMSCREENSHOTS_INTERFACE_VERSION003 {
public:
    virtual std::uint32_t WriteScreenshot(void* pubRGB, std::uint32_t cubRGB, std::int32_t nWidth, std::int32_t nHeight) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMSCREENSHOTS_INTERFACE_VERSION003[0], pubRGB, cubRGB, nWidth, nHeight);
    }
    virtual std::uint32_t AddScreenshotToLibrary(const char* pchFilename, const char* pchThumbnailFilename, std::int32_t nWidth, std::int32_t nHeight) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMSCREENSHOTS_INTERFACE_VERSION003[1], pchFilename, pchThumbnailFilename, nWidth, nHeight);
    }
    virtual void TriggerScreenshot() {
        steambridge::slot<void>(kSlots_STEAMSCREENSHOTS_INTERFACE_VERSION003[2]);
    }
    virtual void HookScreenshots(bool bHook) {
        steambridge::slot<void>(kSlots_STEAMSCREENSHOTS_INTERFACE_VERSION003[3], bHook);
    }
    virtual bool SetLocation(std::uint32_t hScreenshot, const char* pchLocation) {
        return steambridge::slot<bool>(kSlots_STEAMSCREENSHOTS_INTERFACE_VERSION003[4], hScreenshot, pchLocation);
    }
    virtual bool TagUser(std::uint32_t hScreenshot, CSteamID steamID) {
        return steambridge::slot<bool>(kSlots_STEAMSCREENSHOTS_INTERFACE_VERSION003[5], hScreenshot, steamID);
    }
    virtual bool TagPublishedFile(std::uint32_t hScreenshot, std::uint64_t unPublishedFileID) {
        return steambridge::slot<bool>(kSlots_STEAMSCREENSHOTS_INTERFACE_VERSION003[6], hScreenshot, unPublishedFileID);
    }
    virtual bool IsScreenshotsHooked() {
        return steambridge::slot<bool>(kSlots_STEAMSCREENSHOTS_INTERFACE_VERSION003[7]);
    }
    virtual std::uint32_t AddVRScreenshotToLibrary(std::int32_t eType, const char* pchFilename, const char* pchVRFilename) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMSCREENSHOTS_INTERFACE_VERSION003[8], eType, pchFilename, pchVRFilename);
    }
};

Version_STEAMSCREENSHOTS_INTERFACE_VERSION003 g_STEAMSCREENSHOTS_INTERFACE_VERSION003;

const char* const kParams_STEAMUGC_INTERFACE_VERSION010_0[] = {
    "unAccountID",
    "eListType",
    "eMatchingUGCType",
    "eSortOrder",
    "nCreatorAppID",
    "nConsumerAppID",
    "unPage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_1[] = {
    "eQueryType",
    "eMatchingeMatchingUGCTypeFileType",
    "nCreatorAppID",
    "nConsumerAppID",
    "unPage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_2[] = {
    "pvecPublishedFileID",
    "unNumPublishedFileIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_3[] = {
    "handle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_4[] = {
    "handle",
    "index",
    "pDetails",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_5[] = {
    "handle",
    "index",
    "pchURL",
    "cchURLSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_6[] = {
    "handle",
    "index",
    "pchMetadata",
    "cchMetadatasize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_7[] = {
    "handle",
    "index",
    "pvecPublishedFileID",
    "cMaxEntries",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_8[] = {
    "handle",
    "index",
    "eStatType",
    "pStatValue",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_9[] = {
    "handle",
    "index",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_10[] = {
    "handle",
    "index",
    "previewIndex",
    "pchURLOrVideoID",
    "cchURLSize",
    "pchOriginalFileName",
    "cchOriginalFileNameSize",
    "pPreviewType",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_11[] = {
    "handle",
    "index",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_12[] = {
    "handle",
    "index",
    "keyValueTagIndex",
    "pchKey",
    "cchKeySize",
    "pchValue",
    "cchValueSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_13[] = {
    "handle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_14[] = {
    "handle",
    "pTagName",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_15[] = {
    "handle",
    "pTagName",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_16[] = {
    "handle",
    "bReturnOnlyIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_17[] = {
    "handle",
    "bReturnKeyValueTags",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_18[] = {
    "handle",
    "bReturnLongDescription",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_19[] = {
    "handle",
    "bReturnMetadata",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_20[] = {
    "handle",
    "bReturnChildren",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_21[] = {
    "handle",
    "bReturnAdditionalPreviews",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_22[] = {
    "handle",
    "bReturnTotalOnly",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_23[] = {
    "handle",
    "unDays",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_24[] = {
    "handle",
    "pchLanguage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_25[] = {
    "handle",
    "unMaxAgeSeconds",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_26[] = {
    "handle",
    "pMatchCloudFileName",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_27[] = {
    "handle",
    "bMatchAnyTag",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_28[] = {
    "handle",
    "pSearchText",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_29[] = {
    "handle",
    "unDays",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_30[] = {
    "handle",
    "pKey",
    "pValue",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_31[] = {
    "nPublishedFileID",
    "unMaxAgeSeconds",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_32[] = {
    "nConsumerAppId",
    "eFileType",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_33[] = {
    "nConsumerAppId",
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_34[] = {
    "handle",
    "pchTitle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_35[] = {
    "handle",
    "pchDescription",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_36[] = {
    "handle",
    "pchLanguage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_37[] = {
    "handle",
    "pchMetaData",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_38[] = {
    "handle",
    "eVisibility",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_39[] = {
    "updateHandle",
    "pTags",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_40[] = {
    "handle",
    "pszContentFolder",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_41[] = {
    "handle",
    "pszPreviewFile",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_42[] = {
    "handle",
    "pchKey",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_43[] = {
    "handle",
    "pchKey",
    "pchValue",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_44[] = {
    "handle",
    "pszPreviewFile",
    "type",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_45[] = {
    "handle",
    "pszVideoID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_46[] = {
    "handle",
    "index",
    "pszPreviewFile",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_47[] = {
    "handle",
    "index",
    "pszVideoID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_48[] = {
    "handle",
    "index",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_49[] = {
    "handle",
    "pchChangeNote",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_50[] = {
    "handle",
    "punBytesProcessed",
    "punBytesTotal",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_51[] = {
    "nPublishedFileID",
    "bVoteUp",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_52[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_53[] = {
    "nAppId",
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_54[] = {
    "nAppId",
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_55[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_56[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_58[] = {
    "pvecPublishedFileID",
    "cMaxEntries",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_59[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_60[] = {
    "nPublishedFileID",
    "punSizeOnDisk",
    "pchFolder",
    "cchFolderSize",
    "punTimeStamp",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_61[] = {
    "nPublishedFileID",
    "punBytesDownloaded",
    "punBytesTotal",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_62[] = {
    "nPublishedFileID",
    "bHighPriority",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_63[] = {
    "unWorkshopDepotID",
    "pszFolder",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_64[] = {
    "bSuspend",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_65[] = {
    "pvecPublishedFileID",
    "unNumPublishedFileIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_66[] = {
    "pvecPublishedFileID",
    "unNumPublishedFileIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_68[] = {
    "nParentPublishedFileID",
    "nChildPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_69[] = {
    "nParentPublishedFileID",
    "nChildPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_70[] = {
    "nPublishedFileID",
    "nAppID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_71[] = {
    "nPublishedFileID",
    "nAppID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_72[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION010_73[] = {
    "nPublishedFileID",
};
// ISteamUGC STEAMUGC_INTERFACE_VERSION010
const steambridge::SlotInfo kSlots_STEAMUGC_INTERFACE_VERSION010[] = {
    {"SteamAPI_ISteamUGC_CreateQueryUserUGCRequest", kParams_STEAMUGC_INTERFACE_VERSION010_0},
    {"SteamAPI_ISteamUGC_CreateQueryAllUGCRequestPage", kParams_STEAMUGC_INTERFACE_VERSION010_1},
    {"SteamAPI_ISteamUGC_CreateQueryUGCDetailsRequest", kParams_STEAMUGC_INTERFACE_VERSION010_2},
    {"SteamAPI_ISteamUGC_SendQueryUGCRequest", kParams_STEAMUGC_INTERFACE_VERSION010_3},
    {"SteamAPI_ISteamUGC_GetQueryUGCResult", kParams_STEAMUGC_INTERFACE_VERSION010_4},
    {"SteamAPI_ISteamUGC_GetQueryUGCPreviewURL", kParams_STEAMUGC_INTERFACE_VERSION010_5},
    {"SteamAPI_ISteamUGC_GetQueryUGCMetadata", kParams_STEAMUGC_INTERFACE_VERSION010_6},
    {"SteamAPI_ISteamUGC_GetQueryUGCChildren", kParams_STEAMUGC_INTERFACE_VERSION010_7},
    {"SteamAPI_ISteamUGC_GetQueryUGCStatistic", kParams_STEAMUGC_INTERFACE_VERSION010_8},
    {"SteamAPI_ISteamUGC_GetQueryUGCNumAdditionalPreviews", kParams_STEAMUGC_INTERFACE_VERSION010_9},
    {"SteamAPI_ISteamUGC_GetQueryUGCAdditionalPreview", kParams_STEAMUGC_INTERFACE_VERSION010_10},
    {"SteamAPI_ISteamUGC_GetQueryUGCNumKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION010_11},
    {"SteamAPI_ISteamUGC_GetQueryUGCKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION010_12},
    {"SteamAPI_ISteamUGC_ReleaseQueryUGCRequest", kParams_STEAMUGC_INTERFACE_VERSION010_13},
    {"SteamAPI_ISteamUGC_AddRequiredTag", kParams_STEAMUGC_INTERFACE_VERSION010_14},
    {"SteamAPI_ISteamUGC_AddExcludedTag", kParams_STEAMUGC_INTERFACE_VERSION010_15},
    {"SteamAPI_ISteamUGC_SetReturnOnlyIDs", kParams_STEAMUGC_INTERFACE_VERSION010_16},
    {"SteamAPI_ISteamUGC_SetReturnKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION010_17},
    {"SteamAPI_ISteamUGC_SetReturnLongDescription", kParams_STEAMUGC_INTERFACE_VERSION010_18},
    {"SteamAPI_ISteamUGC_SetReturnMetadata", kParams_STEAMUGC_INTERFACE_VERSION010_19},
    {"SteamAPI_ISteamUGC_SetReturnChildren", kParams_STEAMUGC_INTERFACE_VERSION010_20},
    {"SteamAPI_ISteamUGC_SetReturnAdditionalPreviews", kParams_STEAMUGC_INTERFACE_VERSION010_21},
    {"SteamAPI_ISteamUGC_SetReturnTotalOnly", kParams_STEAMUGC_INTERFACE_VERSION010_22},
    {"SteamAPI_ISteamUGC_SetReturnPlaytimeStats", kParams_STEAMUGC_INTERFACE_VERSION010_23},
    {"SteamAPI_ISteamUGC_SetLanguage", kParams_STEAMUGC_INTERFACE_VERSION010_24},
    {"SteamAPI_ISteamUGC_SetAllowCachedResponse", kParams_STEAMUGC_INTERFACE_VERSION010_25},
    {"SteamAPI_ISteamUGC_SetCloudFileNameFilter", kParams_STEAMUGC_INTERFACE_VERSION010_26},
    {"SteamAPI_ISteamUGC_SetMatchAnyTag", kParams_STEAMUGC_INTERFACE_VERSION010_27},
    {"SteamAPI_ISteamUGC_SetSearchText", kParams_STEAMUGC_INTERFACE_VERSION010_28},
    {"SteamAPI_ISteamUGC_SetRankedByTrendDays", kParams_STEAMUGC_INTERFACE_VERSION010_29},
    {"SteamAPI_ISteamUGC_AddRequiredKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION010_30},
    {"SteamAPI_ISteamUGC_RequestUGCDetails", kParams_STEAMUGC_INTERFACE_VERSION010_31},
    {"SteamAPI_ISteamUGC_CreateItem", kParams_STEAMUGC_INTERFACE_VERSION010_32},
    {"SteamAPI_ISteamUGC_StartItemUpdate", kParams_STEAMUGC_INTERFACE_VERSION010_33},
    {"SteamAPI_ISteamUGC_SetItemTitle", kParams_STEAMUGC_INTERFACE_VERSION010_34},
    {"SteamAPI_ISteamUGC_SetItemDescription", kParams_STEAMUGC_INTERFACE_VERSION010_35},
    {"SteamAPI_ISteamUGC_SetItemUpdateLanguage", kParams_STEAMUGC_INTERFACE_VERSION010_36},
    {"SteamAPI_ISteamUGC_SetItemMetadata", kParams_STEAMUGC_INTERFACE_VERSION010_37},
    {"SteamAPI_ISteamUGC_SetItemVisibility", kParams_STEAMUGC_INTERFACE_VERSION010_38},
    {"SteamAPI_ISteamUGC_SetItemTags", kParams_STEAMUGC_INTERFACE_VERSION010_39},
    {"SteamAPI_ISteamUGC_SetItemContent", kParams_STEAMUGC_INTERFACE_VERSION010_40},
    {"SteamAPI_ISteamUGC_SetItemPreview", kParams_STEAMUGC_INTERFACE_VERSION010_41},
    {"SteamAPI_ISteamUGC_RemoveItemKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION010_42},
    {"SteamAPI_ISteamUGC_AddItemKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION010_43},
    {"SteamAPI_ISteamUGC_AddItemPreviewFile", kParams_STEAMUGC_INTERFACE_VERSION010_44},
    {"SteamAPI_ISteamUGC_AddItemPreviewVideo", kParams_STEAMUGC_INTERFACE_VERSION010_45},
    {"SteamAPI_ISteamUGC_UpdateItemPreviewFile", kParams_STEAMUGC_INTERFACE_VERSION010_46},
    {"SteamAPI_ISteamUGC_UpdateItemPreviewVideo", kParams_STEAMUGC_INTERFACE_VERSION010_47},
    {"SteamAPI_ISteamUGC_RemoveItemPreview", kParams_STEAMUGC_INTERFACE_VERSION010_48},
    {"SteamAPI_ISteamUGC_SubmitItemUpdate", kParams_STEAMUGC_INTERFACE_VERSION010_49},
    {"SteamAPI_ISteamUGC_GetItemUpdateProgress", kParams_STEAMUGC_INTERFACE_VERSION010_50},
    {"SteamAPI_ISteamUGC_SetUserItemVote", kParams_STEAMUGC_INTERFACE_VERSION010_51},
    {"SteamAPI_ISteamUGC_GetUserItemVote", kParams_STEAMUGC_INTERFACE_VERSION010_52},
    {"SteamAPI_ISteamUGC_AddItemToFavorites", kParams_STEAMUGC_INTERFACE_VERSION010_53},
    {"SteamAPI_ISteamUGC_RemoveItemFromFavorites", kParams_STEAMUGC_INTERFACE_VERSION010_54},
    {"SteamAPI_ISteamUGC_SubscribeItem", kParams_STEAMUGC_INTERFACE_VERSION010_55},
    {"SteamAPI_ISteamUGC_UnsubscribeItem", kParams_STEAMUGC_INTERFACE_VERSION010_56},
    {"SteamAPI_ISteamUGC_GetNumSubscribedItems", nullptr},
    {"SteamAPI_ISteamUGC_GetSubscribedItems", kParams_STEAMUGC_INTERFACE_VERSION010_58},
    {"SteamAPI_ISteamUGC_GetItemState", kParams_STEAMUGC_INTERFACE_VERSION010_59},
    {"SteamAPI_ISteamUGC_GetItemInstallInfo", kParams_STEAMUGC_INTERFACE_VERSION010_60},
    {"SteamAPI_ISteamUGC_GetItemDownloadInfo", kParams_STEAMUGC_INTERFACE_VERSION010_61},
    {"SteamAPI_ISteamUGC_DownloadItem", kParams_STEAMUGC_INTERFACE_VERSION010_62},
    {"SteamAPI_ISteamUGC_BInitWorkshopForGameServer", kParams_STEAMUGC_INTERFACE_VERSION010_63},
    {"SteamAPI_ISteamUGC_SuspendDownloads", kParams_STEAMUGC_INTERFACE_VERSION010_64},
    {"SteamAPI_ISteamUGC_StartPlaytimeTracking", kParams_STEAMUGC_INTERFACE_VERSION010_65},
    {"SteamAPI_ISteamUGC_StopPlaytimeTracking", kParams_STEAMUGC_INTERFACE_VERSION010_66},
    {"SteamAPI_ISteamUGC_StopPlaytimeTrackingForAllItems", nullptr},
    {"SteamAPI_ISteamUGC_AddDependency", kParams_STEAMUGC_INTERFACE_VERSION010_68},
    {"SteamAPI_ISteamUGC_RemoveDependency", kParams_STEAMUGC_INTERFACE_VERSION010_69},
    {"SteamAPI_ISteamUGC_AddAppDependency", kParams_STEAMUGC_INTERFACE_VERSION010_70},
    {"SteamAPI_ISteamUGC_RemoveAppDependency", kParams_STEAMUGC_INTERFACE_VERSION010_71},
    {"SteamAPI_ISteamUGC_GetAppDependencies", kParams_STEAMUGC_INTERFACE_VERSION010_72},
    {"SteamAPI_ISteamUGC_DeleteItem", kParams_STEAMUGC_INTERFACE_VERSION010_73},
};

class Version_STEAMUGC_INTERFACE_VERSION010 {
public:
    virtual std::uint64_t CreateQueryUserUGCRequest(std::uint32_t unAccountID, std::int32_t eListType, std::int32_t eMatchingUGCType, std::int32_t eSortOrder, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[0], unAccountID, eListType, eMatchingUGCType, eSortOrder, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[1], eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryUGCDetailsRequest(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[2], pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t SendQueryUGCRequest(std::uint64_t handle) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[3], handle);
    }
    virtual bool GetQueryUGCResult(std::uint64_t handle, std::uint32_t index, void* pDetails) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[4], handle, index, pDetails);
    }
    virtual bool GetQueryUGCPreviewURL(std::uint64_t handle, std::uint32_t index, void* pchURL, std::uint32_t cchURLSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[5], handle, index, pchURL, cchURLSize);
    }
    virtual bool GetQueryUGCMetadata(std::uint64_t handle, std::uint32_t index, void* pchMetadata, std::uint32_t cchMetadatasize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[6], handle, index, pchMetadata, cchMetadatasize);
    }
    virtual bool GetQueryUGCChildren(std::uint64_t handle, std::uint32_t index, std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[7], handle, index, pvecPublishedFileID, cMaxEntries);
    }
    virtual bool GetQueryUGCStatistic(std::uint64_t handle, std::uint32_t index, std::int32_t eStatType, std::uint64_t* pStatValue) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[8], handle, index, eStatType, pStatValue);
    }
    virtual std::uint32_t GetQueryUGCNumAdditionalPreviews(std::uint64_t handle, std::uint32_t index) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[9], handle, index);
    }
    virtual bool GetQueryUGCAdditionalPreview(std::uint64_t handle, std::uint32_t index, std::uint32_t previewIndex, void* pchURLOrVideoID, std::uint32_t cchURLSize, void* pchOriginalFileName, std::uint32_t cchOriginalFileNameSize, std::int32_t* pPreviewType) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[10], handle, index, previewIndex, pchURLOrVideoID, cchURLSize, pchOriginalFileName, cchOriginalFileNameSize, pPreviewType);
    }
    virtual std::uint32_t GetQueryUGCNumKeyValueTags(std::uint64_t handle, std::uint32_t index) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[11], handle, index);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, std::uint32_t keyValueTagIndex, void* pchKey, std::uint32_t cchKeySize, void* pchValue, std::uint32_t cchValueSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[12], handle, index, keyValueTagIndex, pchKey, cchKeySize, pchValue, cchValueSize);
    }
    virtual bool ReleaseQueryUGCRequest(std::uint64_t handle) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[13], handle);
    }
    virtual bool AddRequiredTag(std::uint64_t handle, const char* pTagName) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[14], handle, pTagName);
    }
    virtual bool AddExcludedTag(std::uint64_t handle, const char* pTagName) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[15], handle, pTagName);
    }
    virtual bool SetReturnOnlyIDs(std::uint64_t handle, bool bReturnOnlyIDs) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[16], handle, bReturnOnlyIDs);
    }
    virtual bool SetReturnKeyValueTags(std::uint64_t handle, bool bReturnKeyValueTags) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[17], handle, bReturnKeyValueTags);
    }
    virtual bool SetReturnLongDescription(std::uint64_t handle, bool bReturnLongDescription) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[18], handle, bReturnLongDescription);
    }
    virtual bool SetReturnMetadata(std::uint64_t handle, bool bReturnMetadata) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[19], handle, bReturnMetadata);
    }
    virtual bool SetReturnChildren(std::uint64_t handle, bool bReturnChildren) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[20], handle, bReturnChildren);
    }
    virtual bool SetReturnAdditionalPreviews(std::uint64_t handle, bool bReturnAdditionalPreviews) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[21], handle, bReturnAdditionalPreviews);
    }
    virtual bool SetReturnTotalOnly(std::uint64_t handle, bool bReturnTotalOnly) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[22], handle, bReturnTotalOnly);
    }
    virtual bool SetReturnPlaytimeStats(std::uint64_t handle, std::uint32_t unDays) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[23], handle, unDays);
    }
    virtual bool SetLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[24], handle, pchLanguage);
    }
    virtual bool SetAllowCachedResponse(std::uint64_t handle, std::uint32_t unMaxAgeSeconds) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[25], handle, unMaxAgeSeconds);
    }
    virtual bool SetCloudFileNameFilter(std::uint64_t handle, const char* pMatchCloudFileName) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[26], handle, pMatchCloudFileName);
    }
    virtual bool SetMatchAnyTag(std::uint64_t handle, bool bMatchAnyTag) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[27], handle, bMatchAnyTag);
    }
    virtual bool SetSearchText(std::uint64_t handle, const char* pSearchText) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[28], handle, pSearchText);
    }
    virtual bool SetRankedByTrendDays(std::uint64_t handle, std::uint32_t unDays) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[29], handle, unDays);
    }
    virtual bool AddRequiredKeyValueTag(std::uint64_t handle, const char* pKey, const char* pValue) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[30], handle, pKey, pValue);
    }
    virtual std::uint64_t RequestUGCDetails(std::uint64_t nPublishedFileID, std::uint32_t unMaxAgeSeconds) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[31], nPublishedFileID, unMaxAgeSeconds);
    }
    virtual std::uint64_t CreateItem(std::uint32_t nConsumerAppId, std::int32_t eFileType) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[32], nConsumerAppId, eFileType);
    }
    virtual std::uint64_t StartItemUpdate(std::uint32_t nConsumerAppId, std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[33], nConsumerAppId, nPublishedFileID);
    }
    virtual bool SetItemTitle(std::uint64_t handle, const char* pchTitle) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[34], handle, pchTitle);
    }
    virtual bool SetItemDescription(std::uint64_t handle, const char* pchDescription) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[35], handle, pchDescription);
    }
    virtual bool SetItemUpdateLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[36], handle, pchLanguage);
    }
    virtual bool SetItemMetadata(std::uint64_t handle, const char* pchMetaData) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[37], handle, pchMetaData);
    }
    virtual bool SetItemVisibility(std::uint64_t handle, std::int32_t eVisibility) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[38], handle, eVisibility);
    }
    virtual bool SetItemTags(std::uint64_t updateHandle, void* pTags) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[39], updateHandle, pTags);
    }
    virtual bool SetItemContent(std::uint64_t handle, const char* pszContentFolder) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[40], handle, pszContentFolder);
    }
    virtual bool SetItemPreview(std::uint64_t handle, const char* pszPreviewFile) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[41], handle, pszPreviewFile);
    }
    virtual bool RemoveItemKeyValueTags(std::uint64_t handle, const char* pchKey) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[42], handle, pchKey);
    }
    virtual bool AddItemKeyValueTag(std::uint64_t handle, const char* pchKey, const char* pchValue) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[43], handle, pchKey, pchValue);
    }
    virtual bool AddItemPreviewFile(std::uint64_t handle, const char* pszPreviewFile, std::int32_t type) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[44], handle, pszPreviewFile, type);
    }
    virtual bool AddItemPreviewVideo(std::uint64_t handle, const char* pszVideoID) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[45], handle, pszVideoID);
    }
    virtual bool UpdateItemPreviewFile(std::uint64_t handle, std::uint32_t index, const char* pszPreviewFile) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[46], handle, index, pszPreviewFile);
    }
    virtual bool UpdateItemPreviewVideo(std::uint64_t handle, std::uint32_t index, const char* pszVideoID) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[47], handle, index, pszVideoID);
    }
    virtual bool RemoveItemPreview(std::uint64_t handle, std::uint32_t index) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[48], handle, index);
    }
    virtual std::uint64_t SubmitItemUpdate(std::uint64_t handle, const char* pchChangeNote) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[49], handle, pchChangeNote);
    }
    virtual std::int32_t GetItemUpdateProgress(std::uint64_t handle, std::uint64_t* punBytesProcessed, std::uint64_t* punBytesTotal) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[50], handle, punBytesProcessed, punBytesTotal);
    }
    virtual std::uint64_t SetUserItemVote(std::uint64_t nPublishedFileID, bool bVoteUp) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[51], nPublishedFileID, bVoteUp);
    }
    virtual std::uint64_t GetUserItemVote(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[52], nPublishedFileID);
    }
    virtual std::uint64_t AddItemToFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[53], nAppId, nPublishedFileID);
    }
    virtual std::uint64_t RemoveItemFromFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[54], nAppId, nPublishedFileID);
    }
    virtual std::uint64_t SubscribeItem(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[55], nPublishedFileID);
    }
    virtual std::uint64_t UnsubscribeItem(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[56], nPublishedFileID);
    }
    virtual std::uint32_t GetNumSubscribedItems() {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[57]);
    }
    virtual std::uint32_t GetSubscribedItems(std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[58], pvecPublishedFileID, cMaxEntries);
    }
    virtual std::uint32_t GetItemState(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[59], nPublishedFileID);
    }
    virtual bool GetItemInstallInfo(std::uint64_t nPublishedFileID, std::uint64_t* punSizeOnDisk, void* pchFolder, std::uint32_t cchFolderSize, std::uint32_t* punTimeStamp) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[60], nPublishedFileID, punSizeOnDisk, pchFolder, cchFolderSize, punTimeStamp);
    }
    virtual bool GetItemDownloadInfo(std::uint64_t nPublishedFileID, std::uint64_t* punBytesDownloaded, std::uint64_t* punBytesTotal) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[61], nPublishedFileID, punBytesDownloaded, punBytesTotal);
    }
    virtual bool DownloadItem(std::uint64_t nPublishedFileID, bool bHighPriority) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[62], nPublishedFileID, bHighPriority);
    }
    virtual bool BInitWorkshopForGameServer(std::uint32_t unWorkshopDepotID, const char* pszFolder) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION010[63], unWorkshopDepotID, pszFolder);
    }
    virtual void SuspendDownloads(bool bSuspend) {
        steambridge::slot<void>(kSlots_STEAMUGC_INTERFACE_VERSION010[64], bSuspend);
    }
    virtual std::uint64_t StartPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[65], pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[66], pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTrackingForAllItems() {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[67]);
    }
    virtual std::uint64_t AddDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[68], nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t RemoveDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[69], nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t AddAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[70], nPublishedFileID, nAppID);
    }
    virtual std::uint64_t RemoveAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[71], nPublishedFileID, nAppID);
    }
    virtual std::uint64_t GetAppDependencies(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[72], nPublishedFileID);
    }
    virtual std::uint64_t DeleteItem(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION010[73], nPublishedFileID);
    }
};

Version_STEAMUGC_INTERFACE_VERSION010 g_STEAMUGC_INTERFACE_VERSION010;

const char* const kParams_STEAMUGC_INTERFACE_VERSION013_0[] = {
    "unAccountID",
    "eListType",
    "eMatchingUGCType",
    "eSortOrder",
    "nCreatorAppID",
    "nConsumerAppID",
    "unPage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_1[] = {
    "eQueryType",
    "eMatchingeMatchingUGCTypeFileType",
    "nCreatorAppID",
    "nConsumerAppID",
    "unPage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_2[] = {
    "eQueryType",
    "eMatchingeMatchingUGCTypeFileType",
    "nCreatorAppID",
    "nConsumerAppID",
    "pchCursor",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_3[] = {
    "pvecPublishedFileID",
    "unNumPublishedFileIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_4[] = {
    "handle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_5[] = {
    "handle",
    "index",
    "pDetails",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_6[] = {
    "handle",
    "index",
    "pchURL",
    "cchURLSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_7[] = {
    "handle",
    "index",
    "pchMetadata",
    "cchMetadatasize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_8[] = {
    "handle",
    "index",
    "pvecPublishedFileID",
    "cMaxEntries",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_9[] = {
    "handle",
    "index",
    "eStatType",
    "pStatValue",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_10[] = {
    "handle",
    "index",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_11[] = {
    "handle",
    "index",
    "previewIndex",
    "pchURLOrVideoID",
    "cchURLSize",
    "pchOriginalFileName",
    "cchOriginalFileNameSize",
    "pPreviewType",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_12[] = {
    "handle",
    "index",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_13[] = {
    "handle",
    "index",
    "keyValueTagIndex",
    "pchKey",
    "cchKeySize",
    "pchValue",
    "cchValueSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_14[] = {
    "handle",
    "index",
    "pchKey",
    "pchValue",
    "cchValueSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_15[] = {
    "handle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_16[] = {
    "handle",
    "pTagName",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_17[] = {
    "handle",
    "pTagName",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_18[] = {
    "handle",
    "bReturnOnlyIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_19[] = {
    "handle",
    "bReturnKeyValueTags",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_20[] = {
    "handle",
    "bReturnLongDescription",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_21[] = {
    "handle",
    "bReturnMetadata",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_22[] = {
    "handle",
    "bReturnChildren",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_23[] = {
    "handle",
    "bReturnAdditionalPreviews",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_24[] = {
    "handle",
    "bReturnTotalOnly",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_25[] = {
    "handle",
    "unDays",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_26[] = {
    "handle",
    "pchLanguage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_27[] = {
    "handle",
    "unMaxAgeSeconds",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_28[] = {
    "handle",
    "pMatchCloudFileName",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_29[] = {
    "handle",
    "bMatchAnyTag",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_30[] = {
    "handle",
    "pSearchText",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_31[] = {
    "handle",
    "unDays",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_32[] = {
    "handle",
    "pKey",
    "pValue",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_33[] = {
    "nPublishedFileID",
    "unMaxAgeSeconds",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_34[] = {
    "nConsumerAppId",
    "eFileType",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_35[] = {
    "nConsumerAppId",
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_36[] = {
    "handle",
    "pchTitle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_37[] = {
    "handle",
    "pchDescription",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_38[] = {
    "handle",
    "pchLanguage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_39[] = {
    "handle",
    "pchMetaData",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_40[] = {
    "handle",
    "eVisibility",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_41[] = {
    "updateHandle",
    "pTags",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_42[] = {
    "handle",
    "pszContentFolder",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_43[] = {
    "handle",
    "pszPreviewFile",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_44[] = {
    "handle",
    "bAllowLegacyUpload",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_45[] = {
    "handle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_46[] = {
    "handle",
    "pchKey",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_47[] = {
    "handle",
    "pchKey",
    "pchValue",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_48[] = {
    "handle",
    "pszPreviewFile",
    "type",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_49[] = {
    "handle",
    "pszVideoID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_50[] = {
    "handle",
    "index",
    "pszPreviewFile",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_51[] = {
    "handle",
    "index",
    "pszVideoID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_52[] = {
    "handle",
    "index",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_53[] = {
    "handle",
    "pchChangeNote",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_54[] = {
    "handle",
    "punBytesProcessed",
    "punBytesTotal",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_55[] = {
    "nPublishedFileID",
    "bVoteUp",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_56[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_57[] = {
    "nAppId",
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_58[] = {
    "nAppId",
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_59[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_60[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_62[] = {
    "pvecPublishedFileID",
    "cMaxEntries",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_63[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_64[] = {
    "nPublishedFileID",
    "punSizeOnDisk",
    "pchFolder",
    "cchFolderSize",
    "punTimeStamp",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_65[] = {
    "nPublishedFileID",
    "punBytesDownloaded",
    "punBytesTotal",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_66[] = {
    "nPublishedFileID",
    "bHighPriority",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_67[] = {
    "unWorkshopDepotID",
    "pszFolder",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_68[] = {
    "bSuspend",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_69[] = {
    "pvecPublishedFileID",
    "unNumPublishedFileIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_70[] = {
    "pvecPublishedFileID",
    "unNumPublishedFileIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_72[] = {
    "nParentPublishedFileID",
    "nChildPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_73[] = {
    "nParentPublishedFileID",
    "nChildPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_74[] = {
    "nPublishedFileID",
    "nAppID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_75[] = {
    "nPublishedFileID",
    "nAppID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_76[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION013_77[] = {
    "nPublishedFileID",
};
// ISteamUGC STEAMUGC_INTERFACE_VERSION013
const steambridge::SlotInfo kSlots_STEAMUGC_INTERFACE_VERSION013[] = {
    {"SteamAPI_ISteamUGC_CreateQueryUserUGCRequest", kParams_STEAMUGC_INTERFACE_VERSION013_0},
    {"SteamAPI_ISteamUGC_CreateQueryAllUGCRequestPage", kParams_STEAMUGC_INTERFACE_VERSION013_1},
    {"SteamAPI_ISteamUGC_CreateQueryAllUGCRequestCursor", kParams_STEAMUGC_INTERFACE_VERSION013_2},
    {"SteamAPI_ISteamUGC_CreateQueryUGCDetailsRequest", kParams_STEAMUGC_INTERFACE_VERSION013_3},
    {"SteamAPI_ISteamUGC_SendQueryUGCRequest", kParams_STEAMUGC_INTERFACE_VERSION013_4},
    {"SteamAPI_ISteamUGC_GetQueryUGCResult", kParams_STEAMUGC_INTERFACE_VERSION013_5},
    {"SteamAPI_ISteamUGC_GetQueryUGCPreviewURL", kParams_STEAMUGC_INTERFACE_VERSION013_6},
    {"SteamAPI_ISteamUGC_GetQueryUGCMetadata", kParams_STEAMUGC_INTERFACE_VERSION013_7},
    {"SteamAPI_ISteamUGC_GetQueryUGCChildren", kParams_STEAMUGC_INTERFACE_VERSION013_8},
    {"SteamAPI_ISteamUGC_GetQueryUGCStatistic", kParams_STEAMUGC_INTERFACE_VERSION013_9},
    {"SteamAPI_ISteamUGC_GetQueryUGCNumAdditionalPreviews", kParams_STEAMUGC_INTERFACE_VERSION013_10},
    {"SteamAPI_ISteamUGC_GetQueryUGCAdditionalPreview", kParams_STEAMUGC_INTERFACE_VERSION013_11},
    {"SteamAPI_ISteamUGC_GetQueryUGCNumKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION013_12},
    {"SteamAPI_ISteamUGC_GetQueryUGCKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION013_13},
    {"SteamAPI_ISteamUGC_GetQueryFirstUGCKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION013_14},
    {"SteamAPI_ISteamUGC_ReleaseQueryUGCRequest", kParams_STEAMUGC_INTERFACE_VERSION013_15},
    {"SteamAPI_ISteamUGC_AddRequiredTag", kParams_STEAMUGC_INTERFACE_VERSION013_16},
    {"SteamAPI_ISteamUGC_AddExcludedTag", kParams_STEAMUGC_INTERFACE_VERSION013_17},
    {"SteamAPI_ISteamUGC_SetReturnOnlyIDs", kParams_STEAMUGC_INTERFACE_VERSION013_18},
    {"SteamAPI_ISteamUGC_SetReturnKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION013_19},
    {"SteamAPI_ISteamUGC_SetReturnLongDescription", kParams_STEAMUGC_INTERFACE_VERSION013_20},
    {"SteamAPI_ISteamUGC_SetReturnMetadata", kParams_STEAMUGC_INTERFACE_VERSION013_21},
    {"SteamAPI_ISteamUGC_SetReturnChildren", kParams_STEAMUGC_INTERFACE_VERSION013_22},
    {"SteamAPI_ISteamUGC_SetReturnAdditionalPreviews", kParams_STEAMUGC_INTERFACE_VERSION013_23},
    {"SteamAPI_ISteamUGC_SetReturnTotalOnly", kParams_STEAMUGC_INTERFACE_VERSION013_24},
    {"SteamAPI_ISteamUGC_SetReturnPlaytimeStats", kParams_STEAMUGC_INTERFACE_VERSION013_25},
    {"SteamAPI_ISteamUGC_SetLanguage", kParams_STEAMUGC_INTERFACE_VERSION013_26},
    {"SteamAPI_ISteamUGC_SetAllowCachedResponse", kParams_STEAMUGC_INTERFACE_VERSION013_27},
    {"SteamAPI_ISteamUGC_SetCloudFileNameFilter", kParams_STEAMUGC_INTERFACE_VERSION013_28},
    {"SteamAPI_ISteamUGC_SetMatchAnyTag", kParams_STEAMUGC_INTERFACE_VERSION013_29},
    {"SteamAPI_ISteamUGC_SetSearchText", kParams_STEAMUGC_INTERFACE_VERSION013_30},
    {"SteamAPI_ISteamUGC_SetRankedByTrendDays", kParams_STEAMUGC_INTERFACE_VERSION013_31},
    {"SteamAPI_ISteamUGC_AddRequiredKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION013_32},
    {"SteamAPI_ISteamUGC_RequestUGCDetails", kParams_STEAMUGC_INTERFACE_VERSION013_33},
    {"SteamAPI_ISteamUGC_CreateItem", kParams_STEAMUGC_INTERFACE_VERSION013_34},
    {"SteamAPI_ISteamUGC_StartItemUpdate", kParams_STEAMUGC_INTERFACE_VERSION013_35},
    {"SteamAPI_ISteamUGC_SetItemTitle", kParams_STEAMUGC_INTERFACE_VERSION013_36},
    {"SteamAPI_ISteamUGC_SetItemDescription", kParams_STEAMUGC_INTERFACE_VERSION013_37},
    {"SteamAPI_ISteamUGC_SetItemUpdateLanguage", kParams_STEAMUGC_INTERFACE_VERSION013_38},
    {"SteamAPI_ISteamUGC_SetItemMetadata", kParams_STEAMUGC_INTERFACE_VERSION013_39},
    {"SteamAPI_ISteamUGC_SetItemVisibility", kParams_STEAMUGC_INTERFACE_VERSION013_40},
    {"SteamAPI_ISteamUGC_SetItemTags", kParams_STEAMUGC_INTERFACE_VERSION013_41},
    {"SteamAPI_ISteamUGC_SetItemContent", kParams_STEAMUGC_INTERFACE_VERSION013_42},
    {"SteamAPI_ISteamUGC_SetItemPreview", kParams_STEAMUGC_INTERFACE_VERSION013_43},
    {"SteamAPI_ISteamUGC_SetAllowLegacyUpload", kParams_STEAMUGC_INTERFACE_VERSION013_44},
    {"SteamAPI_ISteamUGC_RemoveAllItemKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION013_45},
    {"SteamAPI_ISteamUGC_RemoveItemKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION013_46},
    {"SteamAPI_ISteamUGC_AddItemKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION013_47},
    {"SteamAPI_ISteamUGC_AddItemPreviewFile", kParams_STEAMUGC_INTERFACE_VERSION013_48},
    {"SteamAPI_ISteamUGC_AddItemPreviewVideo", kParams_STEAMUGC_INTERFACE_VERSION013_49},
    {"SteamAPI_ISteamUGC_UpdateItemPreviewFile", kParams_STEAMUGC_INTERFACE_VERSION013_50},
    {"SteamAPI_ISteamUGC_UpdateItemPreviewVideo", kParams_STEAMUGC_INTERFACE_VERSION013_51},
    {"SteamAPI_ISteamUGC_RemoveItemPreview", kParams_STEAMUGC_INTERFACE_VERSION013_52},
    {"SteamAPI_ISteamUGC_SubmitItemUpdate", kParams_STEAMUGC_INTERFACE_VERSION013_53},
    {"SteamAPI_ISteamUGC_GetItemUpdateProgress", kParams_STEAMUGC_INTERFACE_VERSION013_54},
    {"SteamAPI_ISteamUGC_SetUserItemVote", kParams_STEAMUGC_INTERFACE_VERSION013_55},
    {"SteamAPI_ISteamUGC_GetUserItemVote", kParams_STEAMUGC_INTERFACE_VERSION013_56},
    {"SteamAPI_ISteamUGC_AddItemToFavorites", kParams_STEAMUGC_INTERFACE_VERSION013_57},
    {"SteamAPI_ISteamUGC_RemoveItemFromFavorites", kParams_STEAMUGC_INTERFACE_VERSION013_58},
    {"SteamAPI_ISteamUGC_SubscribeItem", kParams_STEAMUGC_INTERFACE_VERSION013_59},
    {"SteamAPI_ISteamUGC_UnsubscribeItem", kParams_STEAMUGC_INTERFACE_VERSION013_60},
    {"SteamAPI_ISteamUGC_GetNumSubscribedItems", nullptr},
    {"SteamAPI_ISteamUGC_GetSubscribedItems", kParams_STEAMUGC_INTERFACE_VERSION013_62},
    {"SteamAPI_ISteamUGC_GetItemState", kParams_STEAMUGC_INTERFACE_VERSION013_63},
    {"SteamAPI_ISteamUGC_GetItemInstallInfo", kParams_STEAMUGC_INTERFACE_VERSION013_64},
    {"SteamAPI_ISteamUGC_GetItemDownloadInfo", kParams_STEAMUGC_INTERFACE_VERSION013_65},
    {"SteamAPI_ISteamUGC_DownloadItem", kParams_STEAMUGC_INTERFACE_VERSION013_66},
    {"SteamAPI_ISteamUGC_BInitWorkshopForGameServer", kParams_STEAMUGC_INTERFACE_VERSION013_67},
    {"SteamAPI_ISteamUGC_SuspendDownloads", kParams_STEAMUGC_INTERFACE_VERSION013_68},
    {"SteamAPI_ISteamUGC_StartPlaytimeTracking", kParams_STEAMUGC_INTERFACE_VERSION013_69},
    {"SteamAPI_ISteamUGC_StopPlaytimeTracking", kParams_STEAMUGC_INTERFACE_VERSION013_70},
    {"SteamAPI_ISteamUGC_StopPlaytimeTrackingForAllItems", nullptr},
    {"SteamAPI_ISteamUGC_AddDependency", kParams_STEAMUGC_INTERFACE_VERSION013_72},
    {"SteamAPI_ISteamUGC_RemoveDependency", kParams_STEAMUGC_INTERFACE_VERSION013_73},
    {"SteamAPI_ISteamUGC_AddAppDependency", kParams_STEAMUGC_INTERFACE_VERSION013_74},
    {"SteamAPI_ISteamUGC_RemoveAppDependency", kParams_STEAMUGC_INTERFACE_VERSION013_75},
    {"SteamAPI_ISteamUGC_GetAppDependencies", kParams_STEAMUGC_INTERFACE_VERSION013_76},
    {"SteamAPI_ISteamUGC_DeleteItem", kParams_STEAMUGC_INTERFACE_VERSION013_77},
};

class Version_STEAMUGC_INTERFACE_VERSION013 {
public:
    virtual std::uint64_t CreateQueryUserUGCRequest(std::uint32_t unAccountID, std::int32_t eListType, std::int32_t eMatchingUGCType, std::int32_t eSortOrder, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[0], unAccountID, eListType, eMatchingUGCType, eSortOrder, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[1], eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, const char* pchCursor) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[2], eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, pchCursor);
    }
    virtual std::uint64_t CreateQueryUGCDetailsRequest(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[3], pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t SendQueryUGCRequest(std::uint64_t handle) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[4], handle);
    }
    virtual bool GetQueryUGCResult(std::uint64_t handle, std::uint32_t index, void* pDetails) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[5], handle, index, pDetails);
    }
    virtual bool GetQueryUGCPreviewURL(std::uint64_t handle, std::uint32_t index, void* pchURL, std::uint32_t cchURLSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[6], handle, index, pchURL, cchURLSize);
    }
    virtual bool GetQueryUGCMetadata(std::uint64_t handle, std::uint32_t index, void* pchMetadata, std::uint32_t cchMetadatasize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[7], handle, index, pchMetadata, cchMetadatasize);
    }
    virtual bool GetQueryUGCChildren(std::uint64_t handle, std::uint32_t index, std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[8], handle, index, pvecPublishedFileID, cMaxEntries);
    }
    virtual bool GetQueryUGCStatistic(std::uint64_t handle, std::uint32_t index, std::int32_t eStatType, std::uint64_t* pStatValue) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[9], handle, index, eStatType, pStatValue);
    }
    virtual std::uint32_t GetQueryUGCNumAdditionalPreviews(std::uint64_t handle, std::uint32_t index) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[10], handle, index);
    }
    virtual bool GetQueryUGCAdditionalPreview(std::uint64_t handle, std::uint32_t index, std::uint32_t previewIndex, void* pchURLOrVideoID, std::uint32_t cchURLSize, void* pchOriginalFileName, std::uint32_t cchOriginalFileNameSize, std::int32_t* pPreviewType) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[11], handle, index, previewIndex, pchURLOrVideoID, cchURLSize, pchOriginalFileName, cchOriginalFileNameSize, pPreviewType);
    }
    virtual std::uint32_t GetQueryUGCNumKeyValueTags(std::uint64_t handle, std::uint32_t index) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[12], handle, index);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, std::uint32_t keyValueTagIndex, void* pchKey, std::uint32_t cchKeySize, void* pchValue, std::uint32_t cchValueSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[13], handle, index, keyValueTagIndex, pchKey, cchKeySize, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, const char* pchKey, void* pchValue, std::uint32_t cchValueSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[14], handle, index, pchKey, pchValue, cchValueSize);
    }
    virtual bool ReleaseQueryUGCRequest(std::uint64_t handle) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[15], handle);
    }
    virtual bool AddRequiredTag(std::uint64_t handle, const char* pTagName) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[16], handle, pTagName);
    }
    virtual bool AddExcludedTag(std::uint64_t handle, const char* pTagName) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[17], handle, pTagName);
    }
    virtual bool SetReturnOnlyIDs(std::uint64_t handle, bool bReturnOnlyIDs) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[18], handle, bReturnOnlyIDs);
    }
    virtual bool SetReturnKeyValueTags(std::uint64_t handle, bool bReturnKeyValueTags) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[19], handle, bReturnKeyValueTags);
    }
    virtual bool SetReturnLongDescription(std::uint64_t handle, bool bReturnLongDescription) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[20], handle, bReturnLongDescription);
    }
    virtual bool SetReturnMetadata(std::uint64_t handle, bool bReturnMetadata) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[21], handle, bReturnMetadata);
    }
    virtual bool SetReturnChildren(std::uint64_t handle, bool bReturnChildren) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[22], handle, bReturnChildren);
    }
    virtual bool SetReturnAdditionalPreviews(std::uint64_t handle, bool bReturnAdditionalPreviews) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[23], handle, bReturnAdditionalPreviews);
    }
    virtual bool SetReturnTotalOnly(std::uint64_t handle, bool bReturnTotalOnly) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[24], handle, bReturnTotalOnly);
    }
    virtual bool SetReturnPlaytimeStats(std::uint64_t handle, std::uint32_t unDays) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[25], handle, unDays);
    }
    virtual bool SetLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[26], handle, pchLanguage);
    }
    virtual bool SetAllowCachedResponse(std::uint64_t handle, std::uint32_t unMaxAgeSeconds) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[27], handle, unMaxAgeSeconds);
    }
    virtual bool SetCloudFileNameFilter(std::uint64_t handle, const char* pMatchCloudFileName) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[28], handle, pMatchCloudFileName);
    }
    virtual bool SetMatchAnyTag(std::uint64_t handle, bool bMatchAnyTag) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[29], handle, bMatchAnyTag);
    }
    virtual bool SetSearchText(std::uint64_t handle, const char* pSearchText) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[30], handle, pSearchText);
    }
    virtual bool SetRankedByTrendDays(std::uint64_t handle, std::uint32_t unDays) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[31], handle, unDays);
    }
    virtual bool AddRequiredKeyValueTag(std::uint64_t handle, const char* pKey, const char* pValue) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[32], handle, pKey, pValue);
    }
    virtual std::uint64_t RequestUGCDetails(std::uint64_t nPublishedFileID, std::uint32_t unMaxAgeSeconds) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[33], nPublishedFileID, unMaxAgeSeconds);
    }
    virtual std::uint64_t CreateItem(std::uint32_t nConsumerAppId, std::int32_t eFileType) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[34], nConsumerAppId, eFileType);
    }
    virtual std::uint64_t StartItemUpdate(std::uint32_t nConsumerAppId, std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[35], nConsumerAppId, nPublishedFileID);
    }
    virtual bool SetItemTitle(std::uint64_t handle, const char* pchTitle) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[36], handle, pchTitle);
    }
    virtual bool SetItemDescription(std::uint64_t handle, const char* pchDescription) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[37], handle, pchDescription);
    }
    virtual bool SetItemUpdateLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[38], handle, pchLanguage);
    }
    virtual bool SetItemMetadata(std::uint64_t handle, const char* pchMetaData) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[39], handle, pchMetaData);
    }
    virtual bool SetItemVisibility(std::uint64_t handle, std::int32_t eVisibility) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[40], handle, eVisibility);
    }
    virtual bool SetItemTags(std::uint64_t updateHandle, void* pTags) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[41], updateHandle, pTags);
    }
    virtual bool SetItemContent(std::uint64_t handle, const char* pszContentFolder) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[42], handle, pszContentFolder);
    }
    virtual bool SetItemPreview(std::uint64_t handle, const char* pszPreviewFile) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[43], handle, pszPreviewFile);
    }
    virtual bool SetAllowLegacyUpload(std::uint64_t handle, bool bAllowLegacyUpload) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[44], handle, bAllowLegacyUpload);
    }
    virtual bool RemoveAllItemKeyValueTags(std::uint64_t handle) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[45], handle);
    }
    virtual bool RemoveItemKeyValueTags(std::uint64_t handle, const char* pchKey) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[46], handle, pchKey);
    }
    virtual bool AddItemKeyValueTag(std::uint64_t handle, const char* pchKey, const char* pchValue) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[47], handle, pchKey, pchValue);
    }
    virtual bool AddItemPreviewFile(std::uint64_t handle, const char* pszPreviewFile, std::int32_t type) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[48], handle, pszPreviewFile, type);
    }
    virtual bool AddItemPreviewVideo(std::uint64_t handle, const char* pszVideoID) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[49], handle, pszVideoID);
    }
    virtual bool UpdateItemPreviewFile(std::uint64_t handle, std::uint32_t index, const char* pszPreviewFile) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[50], handle, index, pszPreviewFile);
    }
    virtual bool UpdateItemPreviewVideo(std::uint64_t handle, std::uint32_t index, const char* pszVideoID) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[51], handle, index, pszVideoID);
    }
    virtual bool RemoveItemPreview(std::uint64_t handle, std::uint32_t index) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[52], handle, index);
    }
    virtual std::uint64_t SubmitItemUpdate(std::uint64_t handle, const char* pchChangeNote) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[53], handle, pchChangeNote);
    }
    virtual std::int32_t GetItemUpdateProgress(std::uint64_t handle, std::uint64_t* punBytesProcessed, std::uint64_t* punBytesTotal) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[54], handle, punBytesProcessed, punBytesTotal);
    }
    virtual std::uint64_t SetUserItemVote(std::uint64_t nPublishedFileID, bool bVoteUp) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[55], nPublishedFileID, bVoteUp);
    }
    virtual std::uint64_t GetUserItemVote(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[56], nPublishedFileID);
    }
    virtual std::uint64_t AddItemToFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[57], nAppId, nPublishedFileID);
    }
    virtual std::uint64_t RemoveItemFromFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[58], nAppId, nPublishedFileID);
    }
    virtual std::uint64_t SubscribeItem(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[59], nPublishedFileID);
    }
    virtual std::uint64_t UnsubscribeItem(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[60], nPublishedFileID);
    }
    virtual std::uint32_t GetNumSubscribedItems() {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[61]);
    }
    virtual std::uint32_t GetSubscribedItems(std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[62], pvecPublishedFileID, cMaxEntries);
    }
    virtual std::uint32_t GetItemState(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[63], nPublishedFileID);
    }
    virtual bool GetItemInstallInfo(std::uint64_t nPublishedFileID, std::uint64_t* punSizeOnDisk, void* pchFolder, std::uint32_t cchFolderSize, std::uint32_t* punTimeStamp) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[64], nPublishedFileID, punSizeOnDisk, pchFolder, cchFolderSize, punTimeStamp);
    }
    virtual bool GetItemDownloadInfo(std::uint64_t nPublishedFileID, std::uint64_t* punBytesDownloaded, std::uint64_t* punBytesTotal) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[65], nPublishedFileID, punBytesDownloaded, punBytesTotal);
    }
    virtual bool DownloadItem(std::uint64_t nPublishedFileID, bool bHighPriority) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[66], nPublishedFileID, bHighPriority);
    }
    virtual bool BInitWorkshopForGameServer(std::uint32_t unWorkshopDepotID, const char* pszFolder) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION013[67], unWorkshopDepotID, pszFolder);
    }
    virtual void SuspendDownloads(bool bSuspend) {
        steambridge::slot<void>(kSlots_STEAMUGC_INTERFACE_VERSION013[68], bSuspend);
    }
    virtual std::uint64_t StartPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[69], pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[70], pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTrackingForAllItems() {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[71]);
    }
    virtual std::uint64_t AddDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[72], nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t RemoveDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[73], nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t AddAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[74], nPublishedFileID, nAppID);
    }
    virtual std::uint64_t RemoveAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[75], nPublishedFileID, nAppID);
    }
    virtual std::uint64_t GetAppDependencies(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[76], nPublishedFileID);
    }
    virtual std::uint64_t DeleteItem(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION013[77], nPublishedFileID);
    }
};

Version_STEAMUGC_INTERFACE_VERSION013 g_STEAMUGC_INTERFACE_VERSION013;

const char* const kParams_STEAMUGC_INTERFACE_VERSION014_0[] = {
    "unAccountID",
    "eListType",
    "eMatchingUGCType",
    "eSortOrder",
    "nCreatorAppID",
    "nConsumerAppID",
    "unPage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_1[] = {
    "eQueryType",
    "eMatchingeMatchingUGCTypeFileType",
    "nCreatorAppID",
    "nConsumerAppID",
    "unPage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_2[] = {
    "eQueryType",
    "eMatchingeMatchingUGCTypeFileType",
    "nCreatorAppID",
    "nConsumerAppID",
    "pchCursor",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_3[] = {
    "pvecPublishedFileID",
    "unNumPublishedFileIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_4[] = {
    "handle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_5[] = {
    "handle",
    "index",
    "pDetails",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_6[] = {
    "handle",
    "index",
    "pchURL",
    "cchURLSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_7[] = {
    "handle",
    "index",
    "pchMetadata",
    "cchMetadatasize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_8[] = {
    "handle",
    "index",
    "pvecPublishedFileID",
    "cMaxEntries",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_9[] = {
    "handle",
    "index",
    "eStatType",
    "pStatValue",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_10[] = {
    "handle",
    "index",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_11[] = {
    "handle",
    "index",
    "previewIndex",
    "pchURLOrVideoID",
    "cchURLSize",
    "pchOriginalFileName",
    "cchOriginalFileNameSize",
    "pPreviewType",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_12[] = {
    "handle",
    "index",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_13[] = {
    "handle",
    "index",
    "keyValueTagIndex",
    "pchKey",
    "cchKeySize",
    "pchValue",
    "cchValueSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_14[] = {
    "handle",
    "index",
    "pchKey",
    "pchValue",
    "cchValueSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_15[] = {
    "handle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_16[] = {
    "handle",
    "pTagName",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_17[] = {
    "handle",
    "pTagGroups",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_18[] = {
    "handle",
    "pTagName",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_19[] = {
    "handle",
    "bReturnOnlyIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_20[] = {
    "handle",
    "bReturnKeyValueTags",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_21[] = {
    "handle",
    "bReturnLongDescription",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_22[] = {
    "handle",
    "bReturnMetadata",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_23[] = {
    "handle",
    "bReturnChildren",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_24[] = {
    "handle",
    "bReturnAdditionalPreviews",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_25[] = {
    "handle",
    "bReturnTotalOnly",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_26[] = {
    "handle",
    "unDays",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_27[] = {
    "handle",
    "pchLanguage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_28[] = {
    "handle",
    "unMaxAgeSeconds",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_29[] = {
    "handle",
    "pMatchCloudFileName",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_30[] = {
    "handle",
    "bMatchAnyTag",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_31[] = {
    "handle",
    "pSearchText",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_32[] = {
    "handle",
    "unDays",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_33[] = {
    "handle",
    "pKey",
    "pValue",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_34[] = {
    "nPublishedFileID",
    "unMaxAgeSeconds",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_35[] = {
    "nConsumerAppId",
    "eFileType",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_36[] = {
    "nConsumerAppId",
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_37[] = {
    "handle",
    "pchTitle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_38[] = {
    "handle",
    "pchDescription",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_39[] = {
    "handle",
    "pchLanguage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_40[] = {
    "handle",
    "pchMetaData",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_41[] = {
    "handle",
    "eVisibility",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_42[] = {
    "updateHandle",
    "pTags",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_43[] = {
    "handle",
    "pszContentFolder",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_44[] = {
    "handle",
    "pszPreviewFile",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_45[] = {
    "handle",
    "bAllowLegacyUpload",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_46[] = {
    "handle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_47[] = {
    "handle",
    "pchKey",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_48[] = {
    "handle",
    "pchKey",
    "pchValue",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_49[] = {
    "handle",
    "pszPreviewFile",
    "type",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_50[] = {
    "handle",
    "pszVideoID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_51[] = {
    "handle",
    "index",
    "pszPreviewFile",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_52[] = {
    "handle",
    "index",
    "pszVideoID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_53[] = {
    "handle",
    "index",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_54[] = {
    "handle",
    "pchChangeNote",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_55[] = {
    "handle",
    "punBytesProcessed",
    "punBytesTotal",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_56[] = {
    "nPublishedFileID",
    "bVoteUp",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_57[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_58[] = {
    "nAppId",
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_59[] = {
    "nAppId",
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_60[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_61[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_63[] = {
    "pvecPublishedFileID",
    "cMaxEntries",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_64[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_65[] = {
    "nPublishedFileID",
    "punSizeOnDisk",
    "pchFolder",
    "cchFolderSize",
    "punTimeStamp",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_66[] = {
    "nPublishedFileID",
    "punBytesDownloaded",
    "punBytesTotal",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_67[] = {
    "nPublishedFileID",
    "bHighPriority",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_68[] = {
    "unWorkshopDepotID",
    "pszFolder",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_69[] = {
    "bSuspend",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_70[] = {
    "pvecPublishedFileID",
    "unNumPublishedFileIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_71[] = {
    "pvecPublishedFileID",
    "unNumPublishedFileIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_73[] = {
    "nParentPublishedFileID",
    "nChildPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_74[] = {
    "nParentPublishedFileID",
    "nChildPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_75[] = {
    "nPublishedFileID",
    "nAppID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_76[] = {
    "nPublishedFileID",
    "nAppID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_77[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION014_78[] = {
    "nPublishedFileID",
};
// ISteamUGC STEAMUGC_INTERFACE_VERSION014
const steambridge::SlotInfo kSlots_STEAMUGC_INTERFACE_VERSION014[] = {
    {"SteamAPI_ISteamUGC_CreateQueryUserUGCRequest", kParams_STEAMUGC_INTERFACE_VERSION014_0},
    {"SteamAPI_ISteamUGC_CreateQueryAllUGCRequestPage", kParams_STEAMUGC_INTERFACE_VERSION014_1},
    {"SteamAPI_ISteamUGC_CreateQueryAllUGCRequestCursor", kParams_STEAMUGC_INTERFACE_VERSION014_2},
    {"SteamAPI_ISteamUGC_CreateQueryUGCDetailsRequest", kParams_STEAMUGC_INTERFACE_VERSION014_3},
    {"SteamAPI_ISteamUGC_SendQueryUGCRequest", kParams_STEAMUGC_INTERFACE_VERSION014_4},
    {"SteamAPI_ISteamUGC_GetQueryUGCResult", kParams_STEAMUGC_INTERFACE_VERSION014_5},
    {"SteamAPI_ISteamUGC_GetQueryUGCPreviewURL", kParams_STEAMUGC_INTERFACE_VERSION014_6},
    {"SteamAPI_ISteamUGC_GetQueryUGCMetadata", kParams_STEAMUGC_INTERFACE_VERSION014_7},
    {"SteamAPI_ISteamUGC_GetQueryUGCChildren", kParams_STEAMUGC_INTERFACE_VERSION014_8},
    {"SteamAPI_ISteamUGC_GetQueryUGCStatistic", kParams_STEAMUGC_INTERFACE_VERSION014_9},
    {"SteamAPI_ISteamUGC_GetQueryUGCNumAdditionalPreviews", kParams_STEAMUGC_INTERFACE_VERSION014_10},
    {"SteamAPI_ISteamUGC_GetQueryUGCAdditionalPreview", kParams_STEAMUGC_INTERFACE_VERSION014_11},
    {"SteamAPI_ISteamUGC_GetQueryUGCNumKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION014_12},
    {"SteamAPI_ISteamUGC_GetQueryUGCKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION014_13},
    {"SteamAPI_ISteamUGC_GetQueryFirstUGCKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION014_14},
    {"SteamAPI_ISteamUGC_ReleaseQueryUGCRequest", kParams_STEAMUGC_INTERFACE_VERSION014_15},
    {"SteamAPI_ISteamUGC_AddRequiredTag", kParams_STEAMUGC_INTERFACE_VERSION014_16},
    {"SteamAPI_ISteamUGC_AddRequiredTagGroup", kParams_STEAMUGC_INTERFACE_VERSION014_17},
    {"SteamAPI_ISteamUGC_AddExcludedTag", kParams_STEAMUGC_INTERFACE_VERSION014_18},
    {"SteamAPI_ISteamUGC_SetReturnOnlyIDs", kParams_STEAMUGC_INTERFACE_VERSION014_19},
    {"SteamAPI_ISteamUGC_SetReturnKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION014_20},
    {"SteamAPI_ISteamUGC_SetReturnLongDescription", kParams_STEAMUGC_INTERFACE_VERSION014_21},
    {"SteamAPI_ISteamUGC_SetReturnMetadata", kParams_STEAMUGC_INTERFACE_VERSION014_22},
    {"SteamAPI_ISteamUGC_SetReturnChildren", kParams_STEAMUGC_INTERFACE_VERSION014_23},
    {"SteamAPI_ISteamUGC_SetReturnAdditionalPreviews", kParams_STEAMUGC_INTERFACE_VERSION014_24},
    {"SteamAPI_ISteamUGC_SetReturnTotalOnly", kParams_STEAMUGC_INTERFACE_VERSION014_25},
    {"SteamAPI_ISteamUGC_SetReturnPlaytimeStats", kParams_STEAMUGC_INTERFACE_VERSION014_26},
    {"SteamAPI_ISteamUGC_SetLanguage", kParams_STEAMUGC_INTERFACE_VERSION014_27},
    {"SteamAPI_ISteamUGC_SetAllowCachedResponse", kParams_STEAMUGC_INTERFACE_VERSION014_28},
    {"SteamAPI_ISteamUGC_SetCloudFileNameFilter", kParams_STEAMUGC_INTERFACE_VERSION014_29},
    {"SteamAPI_ISteamUGC_SetMatchAnyTag", kParams_STEAMUGC_INTERFACE_VERSION014_30},
    {"SteamAPI_ISteamUGC_SetSearchText", kParams_STEAMUGC_INTERFACE_VERSION014_31},
    {"SteamAPI_ISteamUGC_SetRankedByTrendDays", kParams_STEAMUGC_INTERFACE_VERSION014_32},
    {"SteamAPI_ISteamUGC_AddRequiredKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION014_33},
    {"SteamAPI_ISteamUGC_RequestUGCDetails", kParams_STEAMUGC_INTERFACE_VERSION014_34},
    {"SteamAPI_ISteamUGC_CreateItem", kParams_STEAMUGC_INTERFACE_VERSION014_35},
    {"SteamAPI_ISteamUGC_StartItemUpdate", kParams_STEAMUGC_INTERFACE_VERSION014_36},
    {"SteamAPI_ISteamUGC_SetItemTitle", kParams_STEAMUGC_INTERFACE_VERSION014_37},
    {"SteamAPI_ISteamUGC_SetItemDescription", kParams_STEAMUGC_INTERFACE_VERSION014_38},
    {"SteamAPI_ISteamUGC_SetItemUpdateLanguage", kParams_STEAMUGC_INTERFACE_VERSION014_39},
    {"SteamAPI_ISteamUGC_SetItemMetadata", kParams_STEAMUGC_INTERFACE_VERSION014_40},
    {"SteamAPI_ISteamUGC_SetItemVisibility", kParams_STEAMUGC_INTERFACE_VERSION014_41},
    {"SteamAPI_ISteamUGC_SetItemTags", kParams_STEAMUGC_INTERFACE_VERSION014_42},
    {"SteamAPI_ISteamUGC_SetItemContent", kParams_STEAMUGC_INTERFACE_VERSION014_43},
    {"SteamAPI_ISteamUGC_SetItemPreview", kParams_STEAMUGC_INTERFACE_VERSION014_44},
    {"SteamAPI_ISteamUGC_SetAllowLegacyUpload", kParams_STEAMUGC_INTERFACE_VERSION014_45},
    {"SteamAPI_ISteamUGC_RemoveAllItemKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION014_46},
    {"SteamAPI_ISteamUGC_RemoveItemKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION014_47},
    {"SteamAPI_ISteamUGC_AddItemKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION014_48},
    {"SteamAPI_ISteamUGC_AddItemPreviewFile", kParams_STEAMUGC_INTERFACE_VERSION014_49},
    {"SteamAPI_ISteamUGC_AddItemPreviewVideo", kParams_STEAMUGC_INTERFACE_VERSION014_50},
    {"SteamAPI_ISteamUGC_UpdateItemPreviewFile", kParams_STEAMUGC_INTERFACE_VERSION014_51},
    {"SteamAPI_ISteamUGC_UpdateItemPreviewVideo", kParams_STEAMUGC_INTERFACE_VERSION014_52},
    {"SteamAPI_ISteamUGC_RemoveItemPreview", kParams_STEAMUGC_INTERFACE_VERSION014_53},
    {"SteamAPI_ISteamUGC_SubmitItemUpdate", kParams_STEAMUGC_INTERFACE_VERSION014_54},
    {"SteamAPI_ISteamUGC_GetItemUpdateProgress", kParams_STEAMUGC_INTERFACE_VERSION014_55},
    {"SteamAPI_ISteamUGC_SetUserItemVote", kParams_STEAMUGC_INTERFACE_VERSION014_56},
    {"SteamAPI_ISteamUGC_GetUserItemVote", kParams_STEAMUGC_INTERFACE_VERSION014_57},
    {"SteamAPI_ISteamUGC_AddItemToFavorites", kParams_STEAMUGC_INTERFACE_VERSION014_58},
    {"SteamAPI_ISteamUGC_RemoveItemFromFavorites", kParams_STEAMUGC_INTERFACE_VERSION014_59},
    {"SteamAPI_ISteamUGC_SubscribeItem", kParams_STEAMUGC_INTERFACE_VERSION014_60},
    {"SteamAPI_ISteamUGC_UnsubscribeItem", kParams_STEAMUGC_INTERFACE_VERSION014_61},
    {"SteamAPI_ISteamUGC_GetNumSubscribedItems", nullptr},
    {"SteamAPI_ISteamUGC_GetSubscribedItems", kParams_STEAMUGC_INTERFACE_VERSION014_63},
    {"SteamAPI_ISteamUGC_GetItemState", kParams_STEAMUGC_INTERFACE_VERSION014_64},
    {"SteamAPI_ISteamUGC_GetItemInstallInfo", kParams_STEAMUGC_INTERFACE_VERSION014_65},
    {"SteamAPI_ISteamUGC_GetItemDownloadInfo", kParams_STEAMUGC_INTERFACE_VERSION014_66},
    {"SteamAPI_ISteamUGC_DownloadItem", kParams_STEAMUGC_INTERFACE_VERSION014_67},
    {"SteamAPI_ISteamUGC_BInitWorkshopForGameServer", kParams_STEAMUGC_INTERFACE_VERSION014_68},
    {"SteamAPI_ISteamUGC_SuspendDownloads", kParams_STEAMUGC_INTERFACE_VERSION014_69},
    {"SteamAPI_ISteamUGC_StartPlaytimeTracking", kParams_STEAMUGC_INTERFACE_VERSION014_70},
    {"SteamAPI_ISteamUGC_StopPlaytimeTracking", kParams_STEAMUGC_INTERFACE_VERSION014_71},
    {"SteamAPI_ISteamUGC_StopPlaytimeTrackingForAllItems", nullptr},
    {"SteamAPI_ISteamUGC_AddDependency", kParams_STEAMUGC_INTERFACE_VERSION014_73},
    {"SteamAPI_ISteamUGC_RemoveDependency", kParams_STEAMUGC_INTERFACE_VERSION014_74},
    {"SteamAPI_ISteamUGC_AddAppDependency", kParams_STEAMUGC_INTERFACE_VERSION014_75},
    {"SteamAPI_ISteamUGC_RemoveAppDependency", kParams_STEAMUGC_INTERFACE_VERSION014_76},
    {"SteamAPI_ISteamUGC_GetAppDependencies", kParams_STEAMUGC_INTERFACE_VERSION014_77},
    {"SteamAPI_ISteamUGC_DeleteItem", kParams_STEAMUGC_INTERFACE_VERSION014_78},
};

class Version_STEAMUGC_INTERFACE_VERSION014 {
public:
    virtual std::uint64_t CreateQueryUserUGCRequest(std::uint32_t unAccountID, std::int32_t eListType, std::int32_t eMatchingUGCType, std::int32_t eSortOrder, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[0], unAccountID, eListType, eMatchingUGCType, eSortOrder, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[1], eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, const char* pchCursor) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[2], eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, pchCursor);
    }
    virtual std::uint64_t CreateQueryUGCDetailsRequest(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[3], pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t SendQueryUGCRequest(std::uint64_t handle) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[4], handle);
    }
    virtual bool GetQueryUGCResult(std::uint64_t handle, std::uint32_t index, void* pDetails) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[5], handle, index, pDetails);
    }
    virtual bool GetQueryUGCPreviewURL(std::uint64_t handle, std::uint32_t index, void* pchURL, std::uint32_t cchURLSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[6], handle, index, pchURL, cchURLSize);
    }
    virtual bool GetQueryUGCMetadata(std::uint64_t handle, std::uint32_t index, void* pchMetadata, std::uint32_t cchMetadatasize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[7], handle, index, pchMetadata, cchMetadatasize);
    }
    virtual bool GetQueryUGCChildren(std::uint64_t handle, std::uint32_t index, std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[8], handle, index, pvecPublishedFileID, cMaxEntries);
    }
    virtual bool GetQueryUGCStatistic(std::uint64_t handle, std::uint32_t index, std::int32_t eStatType, std::uint64_t* pStatValue) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[9], handle, index, eStatType, pStatValue);
    }
    virtual std::uint32_t GetQueryUGCNumAdditionalPreviews(std::uint64_t handle, std::uint32_t index) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[10], handle, index);
    }
    virtual bool GetQueryUGCAdditionalPreview(std::uint64_t handle, std::uint32_t index, std::uint32_t previewIndex, void* pchURLOrVideoID, std::uint32_t cchURLSize, void* pchOriginalFileName, std::uint32_t cchOriginalFileNameSize, std::int32_t* pPreviewType) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[11], handle, index, previewIndex, pchURLOrVideoID, cchURLSize, pchOriginalFileName, cchOriginalFileNameSize, pPreviewType);
    }
    virtual std::uint32_t GetQueryUGCNumKeyValueTags(std::uint64_t handle, std::uint32_t index) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[12], handle, index);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, std::uint32_t keyValueTagIndex, void* pchKey, std::uint32_t cchKeySize, void* pchValue, std::uint32_t cchValueSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[13], handle, index, keyValueTagIndex, pchKey, cchKeySize, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, const char* pchKey, void* pchValue, std::uint32_t cchValueSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[14], handle, index, pchKey, pchValue, cchValueSize);
    }
    virtual bool ReleaseQueryUGCRequest(std::uint64_t handle) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[15], handle);
    }
    virtual bool AddRequiredTag(std::uint64_t handle, const char* pTagName) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[16], handle, pTagName);
    }
    virtual bool AddRequiredTagGroup(std::uint64_t handle, void* pTagGroups) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[17], handle, pTagGroups);
    }
    virtual bool AddExcludedTag(std::uint64_t handle, const char* pTagName) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[18], handle, pTagName);
    }
    virtual bool SetReturnOnlyIDs(std::uint64_t handle, bool bReturnOnlyIDs) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[19], handle, bReturnOnlyIDs);
    }
    virtual bool SetReturnKeyValueTags(std::uint64_t handle, bool bReturnKeyValueTags) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[20], handle, bReturnKeyValueTags);
    }
    virtual bool SetReturnLongDescription(std::uint64_t handle, bool bReturnLongDescription) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[21], handle, bReturnLongDescription);
    }
    virtual bool SetReturnMetadata(std::uint64_t handle, bool bReturnMetadata) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[22], handle, bReturnMetadata);
    }
    virtual bool SetReturnChildren(std::uint64_t handle, bool bReturnChildren) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[23], handle, bReturnChildren);
    }
    virtual bool SetReturnAdditionalPreviews(std::uint64_t handle, bool bReturnAdditionalPreviews) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[24], handle, bReturnAdditionalPreviews);
    }
    virtual bool SetReturnTotalOnly(std::uint64_t handle, bool bReturnTotalOnly) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[25], handle, bReturnTotalOnly);
    }
    virtual bool SetReturnPlaytimeStats(std::uint64_t handle, std::uint32_t unDays) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[26], handle, unDays);
    }
    virtual bool SetLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[27], handle, pchLanguage);
    }
    virtual bool SetAllowCachedResponse(std::uint64_t handle, std::uint32_t unMaxAgeSeconds) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[28], handle, unMaxAgeSeconds);
    }
    virtual bool SetCloudFileNameFilter(std::uint64_t handle, const char* pMatchCloudFileName) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[29], handle, pMatchCloudFileName);
    }
    virtual bool SetMatchAnyTag(std::uint64_t handle, bool bMatchAnyTag) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[30], handle, bMatchAnyTag);
    }
    virtual bool SetSearchText(std::uint64_t handle, const char* pSearchText) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[31], handle, pSearchText);
    }
    virtual bool SetRankedByTrendDays(std::uint64_t handle, std::uint32_t unDays) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[32], handle, unDays);
    }
    virtual bool AddRequiredKeyValueTag(std::uint64_t handle, const char* pKey, const char* pValue) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[33], handle, pKey, pValue);
    }
    virtual std::uint64_t RequestUGCDetails(std::uint64_t nPublishedFileID, std::uint32_t unMaxAgeSeconds) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[34], nPublishedFileID, unMaxAgeSeconds);
    }
    virtual std::uint64_t CreateItem(std::uint32_t nConsumerAppId, std::int32_t eFileType) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[35], nConsumerAppId, eFileType);
    }
    virtual std::uint64_t StartItemUpdate(std::uint32_t nConsumerAppId, std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[36], nConsumerAppId, nPublishedFileID);
    }
    virtual bool SetItemTitle(std::uint64_t handle, const char* pchTitle) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[37], handle, pchTitle);
    }
    virtual bool SetItemDescription(std::uint64_t handle, const char* pchDescription) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[38], handle, pchDescription);
    }
    virtual bool SetItemUpdateLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[39], handle, pchLanguage);
    }
    virtual bool SetItemMetadata(std::uint64_t handle, const char* pchMetaData) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[40], handle, pchMetaData);
    }
    virtual bool SetItemVisibility(std::uint64_t handle, std::int32_t eVisibility) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[41], handle, eVisibility);
    }
    virtual bool SetItemTags(std::uint64_t updateHandle, void* pTags) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[42], updateHandle, pTags);
    }
    virtual bool SetItemContent(std::uint64_t handle, const char* pszContentFolder) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[43], handle, pszContentFolder);
    }
    virtual bool SetItemPreview(std::uint64_t handle, const char* pszPreviewFile) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[44], handle, pszPreviewFile);
    }
    virtual bool SetAllowLegacyUpload(std::uint64_t handle, bool bAllowLegacyUpload) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[45], handle, bAllowLegacyUpload);
    }
    virtual bool RemoveAllItemKeyValueTags(std::uint64_t handle) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[46], handle);
    }
    virtual bool RemoveItemKeyValueTags(std::uint64_t handle, const char* pchKey) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[47], handle, pchKey);
    }
    virtual bool AddItemKeyValueTag(std::uint64_t handle, const char* pchKey, const char* pchValue) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[48], handle, pchKey, pchValue);
    }
    virtual bool AddItemPreviewFile(std::uint64_t handle, const char* pszPreviewFile, std::int32_t type) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[49], handle, pszPreviewFile, type);
    }
    virtual bool AddItemPreviewVideo(std::uint64_t handle, const char* pszVideoID) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[50], handle, pszVideoID);
    }
    virtual bool UpdateItemPreviewFile(std::uint64_t handle, std::uint32_t index, const char* pszPreviewFile) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[51], handle, index, pszPreviewFile);
    }
    virtual bool UpdateItemPreviewVideo(std::uint64_t handle, std::uint32_t index, const char* pszVideoID) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[52], handle, index, pszVideoID);
    }
    virtual bool RemoveItemPreview(std::uint64_t handle, std::uint32_t index) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[53], handle, index);
    }
    virtual std::uint64_t SubmitItemUpdate(std::uint64_t handle, const char* pchChangeNote) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[54], handle, pchChangeNote);
    }
    virtual std::int32_t GetItemUpdateProgress(std::uint64_t handle, std::uint64_t* punBytesProcessed, std::uint64_t* punBytesTotal) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[55], handle, punBytesProcessed, punBytesTotal);
    }
    virtual std::uint64_t SetUserItemVote(std::uint64_t nPublishedFileID, bool bVoteUp) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[56], nPublishedFileID, bVoteUp);
    }
    virtual std::uint64_t GetUserItemVote(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[57], nPublishedFileID);
    }
    virtual std::uint64_t AddItemToFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[58], nAppId, nPublishedFileID);
    }
    virtual std::uint64_t RemoveItemFromFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[59], nAppId, nPublishedFileID);
    }
    virtual std::uint64_t SubscribeItem(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[60], nPublishedFileID);
    }
    virtual std::uint64_t UnsubscribeItem(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[61], nPublishedFileID);
    }
    virtual std::uint32_t GetNumSubscribedItems() {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[62]);
    }
    virtual std::uint32_t GetSubscribedItems(std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[63], pvecPublishedFileID, cMaxEntries);
    }
    virtual std::uint32_t GetItemState(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[64], nPublishedFileID);
    }
    virtual bool GetItemInstallInfo(std::uint64_t nPublishedFileID, std::uint64_t* punSizeOnDisk, void* pchFolder, std::uint32_t cchFolderSize, std::uint32_t* punTimeStamp) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[65], nPublishedFileID, punSizeOnDisk, pchFolder, cchFolderSize, punTimeStamp);
    }
    virtual bool GetItemDownloadInfo(std::uint64_t nPublishedFileID, std::uint64_t* punBytesDownloaded, std::uint64_t* punBytesTotal) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[66], nPublishedFileID, punBytesDownloaded, punBytesTotal);
    }
    virtual bool DownloadItem(std::uint64_t nPublishedFileID, bool bHighPriority) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[67], nPublishedFileID, bHighPriority);
    }
    virtual bool BInitWorkshopForGameServer(std::uint32_t unWorkshopDepotID, const char* pszFolder) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION014[68], unWorkshopDepotID, pszFolder);
    }
    virtual void SuspendDownloads(bool bSuspend) {
        steambridge::slot<void>(kSlots_STEAMUGC_INTERFACE_VERSION014[69], bSuspend);
    }
    virtual std::uint64_t StartPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[70], pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[71], pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTrackingForAllItems() {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[72]);
    }
    virtual std::uint64_t AddDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[73], nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t RemoveDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[74], nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t AddAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[75], nPublishedFileID, nAppID);
    }
    virtual std::uint64_t RemoveAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[76], nPublishedFileID, nAppID);
    }
    virtual std::uint64_t GetAppDependencies(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[77], nPublishedFileID);
    }
    virtual std::uint64_t DeleteItem(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION014[78], nPublishedFileID);
    }
};

Version_STEAMUGC_INTERFACE_VERSION014 g_STEAMUGC_INTERFACE_VERSION014;

const char* const kParams_STEAMUGC_INTERFACE_VERSION015_0[] = {
    "unAccountID",
    "eListType",
    "eMatchingUGCType",
    "eSortOrder",
    "nCreatorAppID",
    "nConsumerAppID",
    "unPage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_1[] = {
    "eQueryType",
    "eMatchingeMatchingUGCTypeFileType",
    "nCreatorAppID",
    "nConsumerAppID",
    "unPage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_2[] = {
    "eQueryType",
    "eMatchingeMatchingUGCTypeFileType",
    "nCreatorAppID",
    "nConsumerAppID",
    "pchCursor",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_3[] = {
    "pvecPublishedFileID",
    "unNumPublishedFileIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_4[] = {
    "handle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_5[] = {
    "handle",
    "index",
    "pDetails",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_6[] = {
    "handle",
    "index",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_7[] = {
    "handle",
    "index",
    "indexTag",
    "pchValue",
    "cchValueSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_8[] = {
    "handle",
    "index",
    "indexTag",
    "pchValue",
    "cchValueSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_9[] = {
    "handle",
    "index",
    "pchURL",
    "cchURLSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_10[] = {
    "handle",
    "index",
    "pchMetadata",
    "cchMetadatasize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_11[] = {
    "handle",
    "index",
    "pvecPublishedFileID",
    "cMaxEntries",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_12[] = {
    "handle",
    "index",
    "eStatType",
    "pStatValue",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_13[] = {
    "handle",
    "index",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_14[] = {
    "handle",
    "index",
    "previewIndex",
    "pchURLOrVideoID",
    "cchURLSize",
    "pchOriginalFileName",
    "cchOriginalFileNameSize",
    "pPreviewType",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_15[] = {
    "handle",
    "index",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_16[] = {
    "handle",
    "index",
    "keyValueTagIndex",
    "pchKey",
    "cchKeySize",
    "pchValue",
    "cchValueSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_17[] = {
    "handle",
    "index",
    "pchKey",
    "pchValue",
    "cchValueSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_18[] = {
    "handle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_19[] = {
    "handle",
    "pTagName",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_20[] = {
    "handle",
    "pTagGroups",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_21[] = {
    "handle",
    "pTagName",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_22[] = {
    "handle",
    "bReturnOnlyIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_23[] = {
    "handle",
    "bReturnKeyValueTags",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_24[] = {
    "handle",
    "bReturnLongDescription",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_25[] = {
    "handle",
    "bReturnMetadata",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_26[] = {
    "handle",
    "bReturnChildren",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_27[] = {
    "handle",
    "bReturnAdditionalPreviews",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_28[] = {
    "handle",
    "bReturnTotalOnly",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_29[] = {
    "handle",
    "unDays",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_30[] = {
    "handle",
    "pchLanguage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_31[] = {
    "handle",
    "unMaxAgeSeconds",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_32[] = {
    "handle",
    "pMatchCloudFileName",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_33[] = {
    "handle",
    "bMatchAnyTag",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_34[] = {
    "handle",
    "pSearchText",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_35[] = {
    "handle",
    "unDays",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_36[] = {
    "handle",
    "pKey",
    "pValue",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_37[] = {
    "nPublishedFileID",
    "unMaxAgeSeconds",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_38[] = {
    "nConsumerAppId",
    "eFileType",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_39[] = {
    "nConsumerAppId",
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_40[] = {
    "handle",
    "pchTitle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_41[] = {
    "handle",
    "pchDescription",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_42[] = {
    "handle",
    "pchLanguage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_43[] = {
    "handle",
    "pchMetaData",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_44[] = {
    "handle",
    "eVisibility",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_45[] = {
    "updateHandle",
    "pTags",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_46[] = {
    "handle",
    "pszContentFolder",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_47[] = {
    "handle",
    "pszPreviewFile",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_48[] = {
    "handle",
    "bAllowLegacyUpload",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_49[] = {
    "handle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_50[] = {
    "handle",
    "pchKey",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_51[] = {
    "handle",
    "pchKey",
    "pchValue",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_52[] = {
    "handle",
    "pszPreviewFile",
    "type",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_53[] = {
    "handle",
    "pszVideoID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_54[] = {
    "handle",
    "index",
    "pszPreviewFile",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_55[] = {
    "handle",
    "index",
    "pszVideoID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_56[] = {
    "handle",
    "index",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_57[] = {
    "handle",
    "pchChangeNote",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_58[] = {
    "handle",
    "punBytesProcessed",
    "punBytesTotal",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_59[] = {
    "nPublishedFileID",
    "bVoteUp",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_60[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_61[] = {
    "nAppId",
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_62[] = {
    "nAppId",
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_63[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_64[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_66[] = {
    "pvecPublishedFileID",
    "cMaxEntries",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_67[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_68[] = {
    "nPublishedFileID",
    "punSizeOnDisk",
    "pchFolder",
    "cchFolderSize",
    "punTimeStamp",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_69[] = {
    "nPublishedFileID",
    "punBytesDownloaded",
    "punBytesTotal",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_70[] = {
    "nPublishedFileID",
    "bHighPriority",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_71[] = {
    "unWorkshopDepotID",
    "pszFolder",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_72[] = {
    "bSuspend",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_73[] = {
    "pvecPublishedFileID",
    "unNumPublishedFileIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_74[] = {
    "pvecPublishedFileID",
    "unNumPublishedFileIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_76[] = {
    "nParentPublishedFileID",
    "nChildPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_77[] = {
    "nParentPublishedFileID",
    "nChildPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_78[] = {
    "nPublishedFileID",
    "nAppID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_79[] = {
    "nPublishedFileID",
    "nAppID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_80[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION015_81[] = {
    "nPublishedFileID",
};
// ISteamUGC STEAMUGC_INTERFACE_VERSION015
const steambridge::SlotInfo kSlots_STEAMUGC_INTERFACE_VERSION015[] = {
    {"SteamAPI_ISteamUGC_CreateQueryUserUGCRequest", kParams_STEAMUGC_INTERFACE_VERSION015_0},
    {"SteamAPI_ISteamUGC_CreateQueryAllUGCRequestPage", kParams_STEAMUGC_INTERFACE_VERSION015_1},
    {"SteamAPI_ISteamUGC_CreateQueryAllUGCRequestCursor", kParams_STEAMUGC_INTERFACE_VERSION015_2},
    {"SteamAPI_ISteamUGC_CreateQueryUGCDetailsRequest", kParams_STEAMUGC_INTERFACE_VERSION015_3},
    {"SteamAPI_ISteamUGC_SendQueryUGCRequest", kParams_STEAMUGC_INTERFACE_VERSION015_4},
    {"SteamAPI_ISteamUGC_GetQueryUGCResult", kParams_STEAMUGC_INTERFACE_VERSION015_5},
    {"SteamAPI_ISteamUGC_GetQueryUGCNumTags", kParams_STEAMUGC_INTERFACE_VERSION015_6},
    {"SteamAPI_ISteamUGC_GetQueryUGCTag", kParams_STEAMUGC_INTERFACE_VERSION015_7},
    {"SteamAPI_ISteamUGC_GetQueryUGCTagDisplayName", kParams_STEAMUGC_INTERFACE_VERSION015_8},
    {"SteamAPI_ISteamUGC_GetQueryUGCPreviewURL", kParams_STEAMUGC_INTERFACE_VERSION015_9},
    {"SteamAPI_ISteamUGC_GetQueryUGCMetadata", kParams_STEAMUGC_INTERFACE_VERSION015_10},
    {"SteamAPI_ISteamUGC_GetQueryUGCChildren", kParams_STEAMUGC_INTERFACE_VERSION015_11},
    {"SteamAPI_ISteamUGC_GetQueryUGCStatistic", kParams_STEAMUGC_INTERFACE_VERSION015_12},
    {"SteamAPI_ISteamUGC_GetQueryUGCNumAdditionalPreviews", kParams_STEAMUGC_INTERFACE_VERSION015_13},
    {"SteamAPI_ISteamUGC_GetQueryUGCAdditionalPreview", kParams_STEAMUGC_INTERFACE_VERSION015_14},
    {"SteamAPI_ISteamUGC_GetQueryUGCNumKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION015_15},
    {"SteamAPI_ISteamUGC_GetQueryUGCKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION015_16},
    {"SteamAPI_ISteamUGC_GetQueryFirstUGCKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION015_17},
    {"SteamAPI_ISteamUGC_ReleaseQueryUGCRequest", kParams_STEAMUGC_INTERFACE_VERSION015_18},
    {"SteamAPI_ISteamUGC_AddRequiredTag", kParams_STEAMUGC_INTERFACE_VERSION015_19},
    {"SteamAPI_ISteamUGC_AddRequiredTagGroup", kParams_STEAMUGC_INTERFACE_VERSION015_20},
    {"SteamAPI_ISteamUGC_AddExcludedTag", kParams_STEAMUGC_INTERFACE_VERSION015_21},
    {"SteamAPI_ISteamUGC_SetReturnOnlyIDs", kParams_STEAMUGC_INTERFACE_VERSION015_22},
    {"SteamAPI_ISteamUGC_SetReturnKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION015_23},
    {"SteamAPI_ISteamUGC_SetReturnLongDescription", kParams_STEAMUGC_INTERFACE_VERSION015_24},
    {"SteamAPI_ISteamUGC_SetReturnMetadata", kParams_STEAMUGC_INTERFACE_VERSION015_25},
    {"SteamAPI_ISteamUGC_SetReturnChildren", kParams_STEAMUGC_INTERFACE_VERSION015_26},
    {"SteamAPI_ISteamUGC_SetReturnAdditionalPreviews", kParams_STEAMUGC_INTERFACE_VERSION015_27},
    {"SteamAPI_ISteamUGC_SetReturnTotalOnly", kParams_STEAMUGC_INTERFACE_VERSION015_28},
    {"SteamAPI_ISteamUGC_SetReturnPlaytimeStats", kParams_STEAMUGC_INTERFACE_VERSION015_29},
    {"SteamAPI_ISteamUGC_SetLanguage", kParams_STEAMUGC_INTERFACE_VERSION015_30},
    {"SteamAPI_ISteamUGC_SetAllowCachedResponse", kParams_STEAMUGC_INTERFACE_VERSION015_31},
    {"SteamAPI_ISteamUGC_SetCloudFileNameFilter", kParams_STEAMUGC_INTERFACE_VERSION015_32},
    {"SteamAPI_ISteamUGC_SetMatchAnyTag", kParams_STEAMUGC_INTERFACE_VERSION015_33},
    {"SteamAPI_ISteamUGC_SetSearchText", kParams_STEAMUGC_INTERFACE_VERSION015_34},
    {"SteamAPI_ISteamUGC_SetRankedByTrendDays", kParams_STEAMUGC_INTERFACE_VERSION015_35},
    {"SteamAPI_ISteamUGC_AddRequiredKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION015_36},
    {"SteamAPI_ISteamUGC_RequestUGCDetails", kParams_STEAMUGC_INTERFACE_VERSION015_37},
    {"SteamAPI_ISteamUGC_CreateItem", kParams_STEAMUGC_INTERFACE_VERSION015_38},
    {"SteamAPI_ISteamUGC_StartItemUpdate", kParams_STEAMUGC_INTERFACE_VERSION015_39},
    {"SteamAPI_ISteamUGC_SetItemTitle", kParams_STEAMUGC_INTERFACE_VERSION015_40},
    {"SteamAPI_ISteamUGC_SetItemDescription", kParams_STEAMUGC_INTERFACE_VERSION015_41},
    {"SteamAPI_ISteamUGC_SetItemUpdateLanguage", kParams_STEAMUGC_INTERFACE_VERSION015_42},
    {"SteamAPI_ISteamUGC_SetItemMetadata", kParams_STEAMUGC_INTERFACE_VERSION015_43},
    {"SteamAPI_ISteamUGC_SetItemVisibility", kParams_STEAMUGC_INTERFACE_VERSION015_44},
    {"SteamAPI_ISteamUGC_SetItemTags", kParams_STEAMUGC_INTERFACE_VERSION015_45},
    {"SteamAPI_ISteamUGC_SetItemContent", kParams_STEAMUGC_INTERFACE_VERSION015_46},
    {"SteamAPI_ISteamUGC_SetItemPreview", kParams_STEAMUGC_INTERFACE_VERSION015_47},
    {"SteamAPI_ISteamUGC_SetAllowLegacyUpload", kParams_STEAMUGC_INTERFACE_VERSION015_48},
    {"SteamAPI_ISteamUGC_RemoveAllItemKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION015_49},
    {"SteamAPI_ISteamUGC_RemoveItemKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION015_50},
    {"SteamAPI_ISteamUGC_AddItemKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION015_51},
    {"SteamAPI_ISteamUGC_AddItemPreviewFile", kParams_STEAMUGC_INTERFACE_VERSION015_52},
    {"SteamAPI_ISteamUGC_AddItemPreviewVideo", kParams_STEAMUGC_INTERFACE_VERSION015_53},
    {"SteamAPI_ISteamUGC_UpdateItemPreviewFile", kParams_STEAMUGC_INTERFACE_VERSION015_54},
    {"SteamAPI_ISteamUGC_UpdateItemPreviewVideo", kParams_STEAMUGC_INTERFACE_VERSION015_55},
    {"SteamAPI_ISteamUGC_RemoveItemPreview", kParams_STEAMUGC_INTERFACE_VERSION015_56},
    {"SteamAPI_ISteamUGC_SubmitItemUpdate", kParams_STEAMUGC_INTERFACE_VERSION015_57},
    {"SteamAPI_ISteamUGC_GetItemUpdateProgress", kParams_STEAMUGC_INTERFACE_VERSION015_58},
    {"SteamAPI_ISteamUGC_SetUserItemVote", kParams_STEAMUGC_INTERFACE_VERSION015_59},
    {"SteamAPI_ISteamUGC_GetUserItemVote", kParams_STEAMUGC_INTERFACE_VERSION015_60},
    {"SteamAPI_ISteamUGC_AddItemToFavorites", kParams_STEAMUGC_INTERFACE_VERSION015_61},
    {"SteamAPI_ISteamUGC_RemoveItemFromFavorites", kParams_STEAMUGC_INTERFACE_VERSION015_62},
    {"SteamAPI_ISteamUGC_SubscribeItem", kParams_STEAMUGC_INTERFACE_VERSION015_63},
    {"SteamAPI_ISteamUGC_UnsubscribeItem", kParams_STEAMUGC_INTERFACE_VERSION015_64},
    {"SteamAPI_ISteamUGC_GetNumSubscribedItems", nullptr},
    {"SteamAPI_ISteamUGC_GetSubscribedItems", kParams_STEAMUGC_INTERFACE_VERSION015_66},
    {"SteamAPI_ISteamUGC_GetItemState", kParams_STEAMUGC_INTERFACE_VERSION015_67},
    {"SteamAPI_ISteamUGC_GetItemInstallInfo", kParams_STEAMUGC_INTERFACE_VERSION015_68},
    {"SteamAPI_ISteamUGC_GetItemDownloadInfo", kParams_STEAMUGC_INTERFACE_VERSION015_69},
    {"SteamAPI_ISteamUGC_DownloadItem", kParams_STEAMUGC_INTERFACE_VERSION015_70},
    {"SteamAPI_ISteamUGC_BInitWorkshopForGameServer", kParams_STEAMUGC_INTERFACE_VERSION015_71},
    {"SteamAPI_ISteamUGC_SuspendDownloads", kParams_STEAMUGC_INTERFACE_VERSION015_72},
    {"SteamAPI_ISteamUGC_StartPlaytimeTracking", kParams_STEAMUGC_INTERFACE_VERSION015_73},
    {"SteamAPI_ISteamUGC_StopPlaytimeTracking", kParams_STEAMUGC_INTERFACE_VERSION015_74},
    {"SteamAPI_ISteamUGC_StopPlaytimeTrackingForAllItems", nullptr},
    {"SteamAPI_ISteamUGC_AddDependency", kParams_STEAMUGC_INTERFACE_VERSION015_76},
    {"SteamAPI_ISteamUGC_RemoveDependency", kParams_STEAMUGC_INTERFACE_VERSION015_77},
    {"SteamAPI_ISteamUGC_AddAppDependency", kParams_STEAMUGC_INTERFACE_VERSION015_78},
    {"SteamAPI_ISteamUGC_RemoveAppDependency", kParams_STEAMUGC_INTERFACE_VERSION015_79},
    {"SteamAPI_ISteamUGC_GetAppDependencies", kParams_STEAMUGC_INTERFACE_VERSION015_80},
    {"SteamAPI_ISteamUGC_DeleteItem", kParams_STEAMUGC_INTERFACE_VERSION015_81},
};

class Version_STEAMUGC_INTERFACE_VERSION015 {
public:
    virtual std::uint64_t CreateQueryUserUGCRequest(std::uint32_t unAccountID, std::int32_t eListType, std::int32_t eMatchingUGCType, std::int32_t eSortOrder, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[0], unAccountID, eListType, eMatchingUGCType, eSortOrder, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[1], eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, const char* pchCursor) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[2], eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, pchCursor);
    }
    virtual std::uint64_t CreateQueryUGCDetailsRequest(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[3], pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t SendQueryUGCRequest(std::uint64_t handle) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[4], handle);
    }
    virtual bool GetQueryUGCResult(std::uint64_t handle, std::uint32_t index, void* pDetails) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[5], handle, index, pDetails);
    }
    virtual std::uint32_t GetQueryUGCNumTags(std::uint64_t handle, std::uint32_t index) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[6], handle, index);
    }
    virtual bool GetQueryUGCTag(std::uint64_t handle, std::uint32_t index, std::uint32_t indexTag, void* pchValue, std::uint32_t cchValueSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[7], handle, index, indexTag, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCTagDisplayName(std::uint64_t handle, std::uint32_t index, std::uint32_t indexTag, void* pchValue, std::uint32_t cchValueSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[8], handle, index, indexTag, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCPreviewURL(std::uint64_t handle, std::uint32_t index, void* pchURL, std::uint32_t cchURLSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[9], handle, index, pchURL, cchURLSize);
    }
    virtual bool GetQueryUGCMetadata(std::uint64_t handle, std::uint32_t index, void* pchMetadata, std::uint32_t cchMetadatasize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[10], handle, index, pchMetadata, cchMetadatasize);
    }
    virtual bool GetQueryUGCChildren(std::uint64_t handle, std::uint32_t index, std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[11], handle, index, pvecPublishedFileID, cMaxEntries);
    }
    virtual bool GetQueryUGCStatistic(std::uint64_t handle, std::uint32_t index, std::int32_t eStatType, std::uint64_t* pStatValue) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[12], handle, index, eStatType, pStatValue);
    }
    virtual std::uint32_t GetQueryUGCNumAdditionalPreviews(std::uint64_t handle, std::uint32_t index) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[13], handle, index);
    }
    virtual bool GetQueryUGCAdditionalPreview(std::uint64_t handle, std::uint32_t index, std::uint32_t previewIndex, void* pchURLOrVideoID, std::uint32_t cchURLSize, void* pchOriginalFileName, std::uint32_t cchOriginalFileNameSize, std::int32_t* pPreviewType) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[14], handle, index, previewIndex, pchURLOrVideoID, cchURLSize, pchOriginalFileName, cchOriginalFileNameSize, pPreviewType);
    }
    virtual std::uint32_t GetQueryUGCNumKeyValueTags(std::uint64_t handle, std::uint32_t index) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[15], handle, index);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, std::uint32_t keyValueTagIndex, void* pchKey, std::uint32_t cchKeySize, void* pchValue, std::uint32_t cchValueSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[16], handle, index, keyValueTagIndex, pchKey, cchKeySize, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, const char* pchKey, void* pchValue, std::uint32_t cchValueSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[17], handle, index, pchKey, pchValue, cchValueSize);
    }
    virtual bool ReleaseQueryUGCRequest(std::uint64_t handle) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[18], handle);
    }
    virtual bool AddRequiredTag(std::uint64_t handle, const char* pTagName) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[19], handle, pTagName);
    }
    virtual bool AddRequiredTagGroup(std::uint64_t handle, void* pTagGroups) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[20], handle, pTagGroups);
    }
    virtual bool AddExcludedTag(std::uint64_t handle, const char* pTagName) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[21], handle, pTagName);
    }
    virtual bool SetReturnOnlyIDs(std::uint64_t handle, bool bReturnOnlyIDs) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[22], handle, bReturnOnlyIDs);
    }
    virtual bool SetReturnKeyValueTags(std::uint64_t handle, bool bReturnKeyValueTags) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[23], handle, bReturnKeyValueTags);
    }
    virtual bool SetReturnLongDescription(std::uint64_t handle, bool bReturnLongDescription) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[24], handle, bReturnLongDescription);
    }
    virtual bool SetReturnMetadata(std::uint64_t handle, bool bReturnMetadata) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[25], handle, bReturnMetadata);
    }
    virtual bool SetReturnChildren(std::uint64_t handle, bool bReturnChildren) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[26], handle, bReturnChildren);
    }
    virtual bool SetReturnAdditionalPreviews(std::uint64_t handle, bool bReturnAdditionalPreviews) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[27], handle, bReturnAdditionalPreviews);
    }
    virtual bool SetReturnTotalOnly(std::uint64_t handle, bool bReturnTotalOnly) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[28], handle, bReturnTotalOnly);
    }
    virtual bool SetReturnPlaytimeStats(std::uint64_t handle, std::uint32_t unDays) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[29], handle, unDays);
    }
    virtual bool SetLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[30], handle, pchLanguage);
    }
    virtual bool SetAllowCachedResponse(std::uint64_t handle, std::uint32_t unMaxAgeSeconds) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[31], handle, unMaxAgeSeconds);
    }
    virtual bool SetCloudFileNameFilter(std::uint64_t handle, const char* pMatchCloudFileName) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[32], handle, pMatchCloudFileName);
    }
    virtual bool SetMatchAnyTag(std::uint64_t handle, bool bMatchAnyTag) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[33], handle, bMatchAnyTag);
    }
    virtual bool SetSearchText(std::uint64_t handle, const char* pSearchText) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[34], handle, pSearchText);
    }
    virtual bool SetRankedByTrendDays(std::uint64_t handle, std::uint32_t unDays) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[35], handle, unDays);
    }
    virtual bool AddRequiredKeyValueTag(std::uint64_t handle, const char* pKey, const char* pValue) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[36], handle, pKey, pValue);
    }
    virtual std::uint64_t RequestUGCDetails(std::uint64_t nPublishedFileID, std::uint32_t unMaxAgeSeconds) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[37], nPublishedFileID, unMaxAgeSeconds);
    }
    virtual std::uint64_t CreateItem(std::uint32_t nConsumerAppId, std::int32_t eFileType) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[38], nConsumerAppId, eFileType);
    }
    virtual std::uint64_t StartItemUpdate(std::uint32_t nConsumerAppId, std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[39], nConsumerAppId, nPublishedFileID);
    }
    virtual bool SetItemTitle(std::uint64_t handle, const char* pchTitle) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[40], handle, pchTitle);
    }
    virtual bool SetItemDescription(std::uint64_t handle, const char* pchDescription) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[41], handle, pchDescription);
    }
    virtual bool SetItemUpdateLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[42], handle, pchLanguage);
    }
    virtual bool SetItemMetadata(std::uint64_t handle, const char* pchMetaData) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[43], handle, pchMetaData);
    }
    virtual bool SetItemVisibility(std::uint64_t handle, std::int32_t eVisibility) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[44], handle, eVisibility);
    }
    virtual bool SetItemTags(std::uint64_t updateHandle, void* pTags) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[45], updateHandle, pTags);
    }
    virtual bool SetItemContent(std::uint64_t handle, const char* pszContentFolder) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[46], handle, pszContentFolder);
    }
    virtual bool SetItemPreview(std::uint64_t handle, const char* pszPreviewFile) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[47], handle, pszPreviewFile);
    }
    virtual bool SetAllowLegacyUpload(std::uint64_t handle, bool bAllowLegacyUpload) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[48], handle, bAllowLegacyUpload);
    }
    virtual bool RemoveAllItemKeyValueTags(std::uint64_t handle) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[49], handle);
    }
    virtual bool RemoveItemKeyValueTags(std::uint64_t handle, const char* pchKey) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[50], handle, pchKey);
    }
    virtual bool AddItemKeyValueTag(std::uint64_t handle, const char* pchKey, const char* pchValue) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[51], handle, pchKey, pchValue);
    }
    virtual bool AddItemPreviewFile(std::uint64_t handle, const char* pszPreviewFile, std::int32_t type) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[52], handle, pszPreviewFile, type);
    }
    virtual bool AddItemPreviewVideo(std::uint64_t handle, const char* pszVideoID) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[53], handle, pszVideoID);
    }
    virtual bool UpdateItemPreviewFile(std::uint64_t handle, std::uint32_t index, const char* pszPreviewFile) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[54], handle, index, pszPreviewFile);
    }
    virtual bool UpdateItemPreviewVideo(std::uint64_t handle, std::uint32_t index, const char* pszVideoID) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[55], handle, index, pszVideoID);
    }
    virtual bool RemoveItemPreview(std::uint64_t handle, std::uint32_t index) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[56], handle, index);
    }
    virtual std::uint64_t SubmitItemUpdate(std::uint64_t handle, const char* pchChangeNote) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[57], handle, pchChangeNote);
    }
    virtual std::int32_t GetItemUpdateProgress(std::uint64_t handle, std::uint64_t* punBytesProcessed, std::uint64_t* punBytesTotal) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[58], handle, punBytesProcessed, punBytesTotal);
    }
    virtual std::uint64_t SetUserItemVote(std::uint64_t nPublishedFileID, bool bVoteUp) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[59], nPublishedFileID, bVoteUp);
    }
    virtual std::uint64_t GetUserItemVote(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[60], nPublishedFileID);
    }
    virtual std::uint64_t AddItemToFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[61], nAppId, nPublishedFileID);
    }
    virtual std::uint64_t RemoveItemFromFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[62], nAppId, nPublishedFileID);
    }
    virtual std::uint64_t SubscribeItem(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[63], nPublishedFileID);
    }
    virtual std::uint64_t UnsubscribeItem(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[64], nPublishedFileID);
    }
    virtual std::uint32_t GetNumSubscribedItems() {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[65]);
    }
    virtual std::uint32_t GetSubscribedItems(std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[66], pvecPublishedFileID, cMaxEntries);
    }
    virtual std::uint32_t GetItemState(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[67], nPublishedFileID);
    }
    virtual bool GetItemInstallInfo(std::uint64_t nPublishedFileID, std::uint64_t* punSizeOnDisk, void* pchFolder, std::uint32_t cchFolderSize, std::uint32_t* punTimeStamp) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[68], nPublishedFileID, punSizeOnDisk, pchFolder, cchFolderSize, punTimeStamp);
    }
    virtual bool GetItemDownloadInfo(std::uint64_t nPublishedFileID, std::uint64_t* punBytesDownloaded, std::uint64_t* punBytesTotal) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[69], nPublishedFileID, punBytesDownloaded, punBytesTotal);
    }
    virtual bool DownloadItem(std::uint64_t nPublishedFileID, bool bHighPriority) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[70], nPublishedFileID, bHighPriority);
    }
    virtual bool BInitWorkshopForGameServer(std::uint32_t unWorkshopDepotID, const char* pszFolder) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION015[71], unWorkshopDepotID, pszFolder);
    }
    virtual void SuspendDownloads(bool bSuspend) {
        steambridge::slot<void>(kSlots_STEAMUGC_INTERFACE_VERSION015[72], bSuspend);
    }
    virtual std::uint64_t StartPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[73], pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[74], pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTrackingForAllItems() {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[75]);
    }
    virtual std::uint64_t AddDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[76], nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t RemoveDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[77], nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t AddAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[78], nPublishedFileID, nAppID);
    }
    virtual std::uint64_t RemoveAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[79], nPublishedFileID, nAppID);
    }
    virtual std::uint64_t GetAppDependencies(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[80], nPublishedFileID);
    }
    virtual std::uint64_t DeleteItem(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION015[81], nPublishedFileID);
    }
};

Version_STEAMUGC_INTERFACE_VERSION015 g_STEAMUGC_INTERFACE_VERSION015;

const char* const kParams_STEAMUGC_INTERFACE_VERSION016_0[] = {
    "unAccountID",
    "eListType",
    "eMatchingUGCType",
    "eSortOrder",
    "nCreatorAppID",
    "nConsumerAppID",
    "unPage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_1[] = {
    "eQueryType",
    "eMatchingeMatchingUGCTypeFileType",
    "nCreatorAppID",
    "nConsumerAppID",
    "unPage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_2[] = {
    "eQueryType",
    "eMatchingeMatchingUGCTypeFileType",
    "nCreatorAppID",
    "nConsumerAppID",
    "pchCursor",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_3[] = {
    "pvecPublishedFileID",
    "unNumPublishedFileIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_4[] = {
    "handle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_5[] = {
    "handle",
    "index",
    "pDetails",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_6[] = {
    "handle",
    "index",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_7[] = {
    "handle",
    "index",
    "indexTag",
    "pchValue",
    "cchValueSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_8[] = {
    "handle",
    "index",
    "indexTag",
    "pchValue",
    "cchValueSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_9[] = {
    "handle",
    "index",
    "pchURL",
    "cchURLSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_10[] = {
    "handle",
    "index",
    "pchMetadata",
    "cchMetadatasize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_11[] = {
    "handle",
    "index",
    "pvecPublishedFileID",
    "cMaxEntries",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_12[] = {
    "handle",
    "index",
    "eStatType",
    "pStatValue",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_13[] = {
    "handle",
    "index",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_14[] = {
    "handle",
    "index",
    "previewIndex",
    "pchURLOrVideoID",
    "cchURLSize",
    "pchOriginalFileName",
    "cchOriginalFileNameSize",
    "pPreviewType",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_15[] = {
    "handle",
    "index",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_16[] = {
    "handle",
    "index",
    "keyValueTagIndex",
    "pchKey",
    "cchKeySize",
    "pchValue",
    "cchValueSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_17[] = {
    "handle",
    "index",
    "pchKey",
    "pchValue",
    "cchValueSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_18[] = {
    "handle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_19[] = {
    "handle",
    "pTagName",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_20[] = {
    "handle",
    "pTagGroups",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_21[] = {
    "handle",
    "pTagName",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_22[] = {
    "handle",
    "bReturnOnlyIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_23[] = {
    "handle",
    "bReturnKeyValueTags",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_24[] = {
    "handle",
    "bReturnLongDescription",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_25[] = {
    "handle",
    "bReturnMetadata",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_26[] = {
    "handle",
    "bReturnChildren",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_27[] = {
    "handle",
    "bReturnAdditionalPreviews",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_28[] = {
    "handle",
    "bReturnTotalOnly",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_29[] = {
    "handle",
    "unDays",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_30[] = {
    "handle",
    "pchLanguage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_31[] = {
    "handle",
    "unMaxAgeSeconds",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_32[] = {
    "handle",
    "pMatchCloudFileName",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_33[] = {
    "handle",
    "bMatchAnyTag",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_34[] = {
    "handle",
    "pSearchText",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_35[] = {
    "handle",
    "unDays",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_36[] = {
    "handle",
    "rtStart",
    "rtEnd",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_37[] = {
    "handle",
    "rtStart",
    "rtEnd",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_38[] = {
    "handle",
    "pKey",
    "pValue",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_39[] = {
    "nPublishedFileID",
    "unMaxAgeSeconds",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_40[] = {
    "nConsumerAppId",
    "eFileType",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_41[] = {
    "nConsumerAppId",
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_42[] = {
    "handle",
    "pchTitle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_43[] = {
    "handle",
    "pchDescription",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_44[] = {
    "handle",
    "pchLanguage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_45[] = {
    "handle",
    "pchMetaData",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_46[] = {
    "handle",
    "eVisibility",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_47[] = {
    "updateHandle",
    "pTags",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_48[] = {
    "handle",
    "pszContentFolder",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_49[] = {
    "handle",
    "pszPreviewFile",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_50[] = {
    "handle",
    "bAllowLegacyUpload",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_51[] = {
    "handle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_52[] = {
    "handle",
    "pchKey",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_53[] = {
    "handle",
    "pchKey",
    "pchValue",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_54[] = {
    "handle",
    "pszPreviewFile",
    "type",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_55[] = {
    "handle",
    "pszVideoID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_56[] = {
    "handle",
    "index",
    "pszPreviewFile",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_57[] = {
    "handle",
    "index",
    "pszVideoID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_58[] = {
    "handle",
    "index",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_59[] = {
    "handle",
    "pchChangeNote",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_60[] = {
    "handle",
    "punBytesProcessed",
    "punBytesTotal",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_61[] = {
    "nPublishedFileID",
    "bVoteUp",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_62[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_63[] = {
    "nAppId",
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_64[] = {
    "nAppId",
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_65[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_66[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_68[] = {
    "pvecPublishedFileID",
    "cMaxEntries",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_69[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_70[] = {
    "nPublishedFileID",
    "punSizeOnDisk",
    "pchFolder",
    "cchFolderSize",
    "punTimeStamp",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_71[] = {
    "nPublishedFileID",
    "punBytesDownloaded",
    "punBytesTotal",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_72[] = {
    "nPublishedFileID",
    "bHighPriority",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_73[] = {
    "unWorkshopDepotID",
    "pszFolder",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_74[] = {
    "bSuspend",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_75[] = {
    "pvecPublishedFileID",
    "unNumPublishedFileIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_76[] = {
    "pvecPublishedFileID",
    "unNumPublishedFileIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_78[] = {
    "nParentPublishedFileID",
    "nChildPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_79[] = {
    "nParentPublishedFileID",
    "nChildPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_80[] = {
    "nPublishedFileID",
    "nAppID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_81[] = {
    "nPublishedFileID",
    "nAppID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_82[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION016_83[] = {
    "nPublishedFileID",
};
// ISteamUGC STEAMUGC_INTERFACE_VERSION016
const steambridge::SlotInfo kSlots_STEAMUGC_INTERFACE_VERSION016[] = {
    {"SteamAPI_ISteamUGC_CreateQueryUserUGCRequest", kParams_STEAMUGC_INTERFACE_VERSION016_0},
    {"SteamAPI_ISteamUGC_CreateQueryAllUGCRequestPage", kParams_STEAMUGC_INTERFACE_VERSION016_1},
    {"SteamAPI_ISteamUGC_CreateQueryAllUGCRequestCursor", kParams_STEAMUGC_INTERFACE_VERSION016_2},
    {"SteamAPI_ISteamUGC_CreateQueryUGCDetailsRequest", kParams_STEAMUGC_INTERFACE_VERSION016_3},
    {"SteamAPI_ISteamUGC_SendQueryUGCRequest", kParams_STEAMUGC_INTERFACE_VERSION016_4},
    {"SteamAPI_ISteamUGC_GetQueryUGCResult", kParams_STEAMUGC_INTERFACE_VERSION016_5},
    {"SteamAPI_ISteamUGC_GetQueryUGCNumTags", kParams_STEAMUGC_INTERFACE_VERSION016_6},
    {"SteamAPI_ISteamUGC_GetQueryUGCTag", kParams_STEAMUGC_INTERFACE_VERSION016_7},
    {"SteamAPI_ISteamUGC_GetQueryUGCTagDisplayName", kParams_STEAMUGC_INTERFACE_VERSION016_8},
    {"SteamAPI_ISteamUGC_GetQueryUGCPreviewURL", kParams_STEAMUGC_INTERFACE_VERSION016_9},
    {"SteamAPI_ISteamUGC_GetQueryUGCMetadata", kParams_STEAMUGC_INTERFACE_VERSION016_10},
    {"SteamAPI_ISteamUGC_GetQueryUGCChildren", kParams_STEAMUGC_INTERFACE_VERSION016_11},
    {"SteamAPI_ISteamUGC_GetQueryUGCStatistic", kParams_STEAMUGC_INTERFACE_VERSION016_12},
    {"SteamAPI_ISteamUGC_GetQueryUGCNumAdditionalPreviews", kParams_STEAMUGC_INTERFACE_VERSION016_13},
    {"SteamAPI_ISteamUGC_GetQueryUGCAdditionalPreview", kParams_STEAMUGC_INTERFACE_VERSION016_14},
    {"SteamAPI_ISteamUGC_GetQueryUGCNumKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION016_15},
    {"SteamAPI_ISteamUGC_GetQueryUGCKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION016_16},
    {"SteamAPI_ISteamUGC_GetQueryFirstUGCKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION016_17},
    {"SteamAPI_ISteamUGC_ReleaseQueryUGCRequest", kParams_STEAMUGC_INTERFACE_VERSION016_18},
    {"SteamAPI_ISteamUGC_AddRequiredTag", kParams_STEAMUGC_INTERFACE_VERSION016_19},
    {"SteamAPI_ISteamUGC_AddRequiredTagGroup", kParams_STEAMUGC_INTERFACE_VERSION016_20},
    {"SteamAPI_ISteamUGC_AddExcludedTag", kParams_STEAMUGC_INTERFACE_VERSION016_21},
    {"SteamAPI_ISteamUGC_SetReturnOnlyIDs", kParams_STEAMUGC_INTERFACE_VERSION016_22},
    {"SteamAPI_ISteamUGC_SetReturnKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION016_23},
    {"SteamAPI_ISteamUGC_SetReturnLongDescription", kParams_STEAMUGC_INTERFACE_VERSION016_24},
    {"SteamAPI_ISteamUGC_SetReturnMetadata", kParams_STEAMUGC_INTERFACE_VERSION016_25},
    {"SteamAPI_ISteamUGC_SetReturnChildren", kParams_STEAMUGC_INTERFACE_VERSION016_26},
    {"SteamAPI_ISteamUGC_SetReturnAdditionalPreviews", kParams_STEAMUGC_INTERFACE_VERSION016_27},
    {"SteamAPI_ISteamUGC_SetReturnTotalOnly", kParams_STEAMUGC_INTERFACE_VERSION016_28},
    {"SteamAPI_ISteamUGC_SetReturnPlaytimeStats", kParams_STEAMUGC_INTERFACE_VERSION016_29},
    {"SteamAPI_ISteamUGC_SetLanguage", kParams_STEAMUGC_INTERFACE_VERSION016_30},
    {"SteamAPI_ISteamUGC_SetAllowCachedResponse", kParams_STEAMUGC_INTERFACE_VERSION016_31},
    {"SteamAPI_ISteamUGC_SetCloudFileNameFilter", kParams_STEAMUGC_INTERFACE_VERSION016_32},
    {"SteamAPI_ISteamUGC_SetMatchAnyTag", kParams_STEAMUGC_INTERFACE_VERSION016_33},
    {"SteamAPI_ISteamUGC_SetSearchText", kParams_STEAMUGC_INTERFACE_VERSION016_34},
    {"SteamAPI_ISteamUGC_SetRankedByTrendDays", kParams_STEAMUGC_INTERFACE_VERSION016_35},
    {"SteamAPI_ISteamUGC_SetTimeCreatedDateRange", kParams_STEAMUGC_INTERFACE_VERSION016_36},
    {"SteamAPI_ISteamUGC_SetTimeUpdatedDateRange", kParams_STEAMUGC_INTERFACE_VERSION016_37},
    {"SteamAPI_ISteamUGC_AddRequiredKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION016_38},
    {"SteamAPI_ISteamUGC_RequestUGCDetails", kParams_STEAMUGC_INTERFACE_VERSION016_39},
    {"SteamAPI_ISteamUGC_CreateItem", kParams_STEAMUGC_INTERFACE_VERSION016_40},
    {"SteamAPI_ISteamUGC_StartItemUpdate", kParams_STEAMUGC_INTERFACE_VERSION016_41},
    {"SteamAPI_ISteamUGC_SetItemTitle", kParams_STEAMUGC_INTERFACE_VERSION016_42},
    {"SteamAPI_ISteamUGC_SetItemDescription", kParams_STEAMUGC_INTERFACE_VERSION016_43},
    {"SteamAPI_ISteamUGC_SetItemUpdateLanguage", kParams_STEAMUGC_INTERFACE_VERSION016_44},
    {"SteamAPI_ISteamUGC_SetItemMetadata", kParams_STEAMUGC_INTERFACE_VERSION016_45},
    {"SteamAPI_ISteamUGC_SetItemVisibility", kParams_STEAMUGC_INTERFACE_VERSION016_46},
    {"SteamAPI_ISteamUGC_SetItemTags", kParams_STEAMUGC_INTERFACE_VERSION016_47},
    {"SteamAPI_ISteamUGC_SetItemContent", kParams_STEAMUGC_INTERFACE_VERSION016_48},
    {"SteamAPI_ISteamUGC_SetItemPreview", kParams_STEAMUGC_INTERFACE_VERSION016_49},
    {"SteamAPI_ISteamUGC_SetAllowLegacyUpload", kParams_STEAMUGC_INTERFACE_VERSION016_50},
    {"SteamAPI_ISteamUGC_RemoveAllItemKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION016_51},
    {"SteamAPI_ISteamUGC_RemoveItemKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION016_52},
    {"SteamAPI_ISteamUGC_AddItemKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION016_53},
    {"SteamAPI_ISteamUGC_AddItemPreviewFile", kParams_STEAMUGC_INTERFACE_VERSION016_54},
    {"SteamAPI_ISteamUGC_AddItemPreviewVideo", kParams_STEAMUGC_INTERFACE_VERSION016_55},
    {"SteamAPI_ISteamUGC_UpdateItemPreviewFile", kParams_STEAMUGC_INTERFACE_VERSION016_56},
    {"SteamAPI_ISteamUGC_UpdateItemPreviewVideo", kParams_STEAMUGC_INTERFACE_VERSION016_57},
    {"SteamAPI_ISteamUGC_RemoveItemPreview", kParams_STEAMUGC_INTERFACE_VERSION016_58},
    {"SteamAPI_ISteamUGC_SubmitItemUpdate", kParams_STEAMUGC_INTERFACE_VERSION016_59},
    {"SteamAPI_ISteamUGC_GetItemUpdateProgress", kParams_STEAMUGC_INTERFACE_VERSION016_60},
    {"SteamAPI_ISteamUGC_SetUserItemVote", kParams_STEAMUGC_INTERFACE_VERSION016_61},
    {"SteamAPI_ISteamUGC_GetUserItemVote", kParams_STEAMUGC_INTERFACE_VERSION016_62},
    {"SteamAPI_ISteamUGC_AddItemToFavorites", kParams_STEAMUGC_INTERFACE_VERSION016_63},
    {"SteamAPI_ISteamUGC_RemoveItemFromFavorites", kParams_STEAMUGC_INTERFACE_VERSION016_64},
    {"SteamAPI_ISteamUGC_SubscribeItem", kParams_STEAMUGC_INTERFACE_VERSION016_65},
    {"SteamAPI_ISteamUGC_UnsubscribeItem", kParams_STEAMUGC_INTERFACE_VERSION016_66},
    {"SteamAPI_ISteamUGC_GetNumSubscribedItems", nullptr},
    {"SteamAPI_ISteamUGC_GetSubscribedItems", kParams_STEAMUGC_INTERFACE_VERSION016_68},
    {"SteamAPI_ISteamUGC_GetItemState", kParams_STEAMUGC_INTERFACE_VERSION016_69},
    {"SteamAPI_ISteamUGC_GetItemInstallInfo", kParams_STEAMUGC_INTERFACE_VERSION016_70},
    {"SteamAPI_ISteamUGC_GetItemDownloadInfo", kParams_STEAMUGC_INTERFACE_VERSION016_71},
    {"SteamAPI_ISteamUGC_DownloadItem", kParams_STEAMUGC_INTERFACE_VERSION016_72},
    {"SteamAPI_ISteamUGC_BInitWorkshopForGameServer", kParams_STEAMUGC_INTERFACE_VERSION016_73},
    {"SteamAPI_ISteamUGC_SuspendDownloads", kParams_STEAMUGC_INTERFACE_VERSION016_74},
    {"SteamAPI_ISteamUGC_StartPlaytimeTracking", kParams_STEAMUGC_INTERFACE_VERSION016_75},
    {"SteamAPI_ISteamUGC_StopPlaytimeTracking", kParams_STEAMUGC_INTERFACE_VERSION016_76},
    {"SteamAPI_ISteamUGC_StopPlaytimeTrackingForAllItems", nullptr},
    {"SteamAPI_ISteamUGC_AddDependency", kParams_STEAMUGC_INTERFACE_VERSION016_78},
    {"SteamAPI_ISteamUGC_RemoveDependency", kParams_STEAMUGC_INTERFACE_VERSION016_79},
    {"SteamAPI_ISteamUGC_AddAppDependency", kParams_STEAMUGC_INTERFACE_VERSION016_80},
    {"SteamAPI_ISteamUGC_RemoveAppDependency", kParams_STEAMUGC_INTERFACE_VERSION016_81},
    {"SteamAPI_ISteamUGC_GetAppDependencies", kParams_STEAMUGC_INTERFACE_VERSION016_82},
    {"SteamAPI_ISteamUGC_DeleteItem", kParams_STEAMUGC_INTERFACE_VERSION016_83},
    {"SteamAPI_ISteamUGC_ShowWorkshopEULA", nullptr},
    {"SteamAPI_ISteamUGC_GetWorkshopEULAStatus", nullptr},
};

class Version_STEAMUGC_INTERFACE_VERSION016 {
public:
    virtual std::uint64_t CreateQueryUserUGCRequest(std::uint32_t unAccountID, std::int32_t eListType, std::int32_t eMatchingUGCType, std::int32_t eSortOrder, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[0], unAccountID, eListType, eMatchingUGCType, eSortOrder, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[1], eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, const char* pchCursor) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[2], eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, pchCursor);
    }
    virtual std::uint64_t CreateQueryUGCDetailsRequest(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[3], pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t SendQueryUGCRequest(std::uint64_t handle) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[4], handle);
    }
    virtual bool GetQueryUGCResult(std::uint64_t handle, std::uint32_t index, void* pDetails) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[5], handle, index, pDetails);
    }
    virtual std::uint32_t GetQueryUGCNumTags(std::uint64_t handle, std::uint32_t index) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[6], handle, index);
    }
    virtual bool GetQueryUGCTag(std::uint64_t handle, std::uint32_t index, std::uint32_t indexTag, void* pchValue, std::uint32_t cchValueSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[7], handle, index, indexTag, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCTagDisplayName(std::uint64_t handle, std::uint32_t index, std::uint32_t indexTag, void* pchValue, std::uint32_t cchValueSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[8], handle, index, indexTag, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCPreviewURL(std::uint64_t handle, std::uint32_t index, void* pchURL, std::uint32_t cchURLSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[9], handle, index, pchURL, cchURLSize);
    }
    virtual bool GetQueryUGCMetadata(std::uint64_t handle, std::uint32_t index, void* pchMetadata, std::uint32_t cchMetadatasize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[10], handle, index, pchMetadata, cchMetadatasize);
    }
    virtual bool GetQueryUGCChildren(std::uint64_t handle, std::uint32_t index, std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[11], handle, index, pvecPublishedFileID, cMaxEntries);
    }
    virtual bool GetQueryUGCStatistic(std::uint64_t handle, std::uint32_t index, std::int32_t eStatType, std::uint64_t* pStatValue) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[12], handle, index, eStatType, pStatValue);
    }
    virtual std::uint32_t GetQueryUGCNumAdditionalPreviews(std::uint64_t handle, std::uint32_t index) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[13], handle, index);
    }
    virtual bool GetQueryUGCAdditionalPreview(std::uint64_t handle, std::uint32_t index, std::uint32_t previewIndex, void* pchURLOrVideoID, std::uint32_t cchURLSize, void* pchOriginalFileName, std::uint32_t cchOriginalFileNameSize, std::int32_t* pPreviewType) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[14], handle, index, previewIndex, pchURLOrVideoID, cchURLSize, pchOriginalFileName, cchOriginalFileNameSize, pPreviewType);
    }
    virtual std::uint32_t GetQueryUGCNumKeyValueTags(std::uint64_t handle, std::uint32_t index) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[15], handle, index);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, std::uint32_t keyValueTagIndex, void* pchKey, std::uint32_t cchKeySize, void* pchValue, std::uint32_t cchValueSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[16], handle, index, keyValueTagIndex, pchKey, cchKeySize, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, const char* pchKey, void* pchValue, std::uint32_t cchValueSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[17], handle, index, pchKey, pchValue, cchValueSize);
    }
    virtual bool ReleaseQueryUGCRequest(std::uint64_t handle) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[18], handle);
    }
    virtual bool AddRequiredTag(std::uint64_t handle, const char* pTagName) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[19], handle, pTagName);
    }
    virtual bool AddRequiredTagGroup(std::uint64_t handle, void* pTagGroups) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[20], handle, pTagGroups);
    }
    virtual bool AddExcludedTag(std::uint64_t handle, const char* pTagName) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[21], handle, pTagName);
    }
    virtual bool SetReturnOnlyIDs(std::uint64_t handle, bool bReturnOnlyIDs) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[22], handle, bReturnOnlyIDs);
    }
    virtual bool SetReturnKeyValueTags(std::uint64_t handle, bool bReturnKeyValueTags) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[23], handle, bReturnKeyValueTags);
    }
    virtual bool SetReturnLongDescription(std::uint64_t handle, bool bReturnLongDescription) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[24], handle, bReturnLongDescription);
    }
    virtual bool SetReturnMetadata(std::uint64_t handle, bool bReturnMetadata) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[25], handle, bReturnMetadata);
    }
    virtual bool SetReturnChildren(std::uint64_t handle, bool bReturnChildren) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[26], handle, bReturnChildren);
    }
    virtual bool SetReturnAdditionalPreviews(std::uint64_t handle, bool bReturnAdditionalPreviews) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[27], handle, bReturnAdditionalPreviews);
    }
    virtual bool SetReturnTotalOnly(std::uint64_t handle, bool bReturnTotalOnly) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[28], handle, bReturnTotalOnly);
    }
    virtual bool SetReturnPlaytimeStats(std::uint64_t handle, std::uint32_t unDays) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[29], handle, unDays);
    }
    virtual bool SetLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[30], handle, pchLanguage);
    }
    virtual bool SetAllowCachedResponse(std::uint64_t handle, std::uint32_t unMaxAgeSeconds) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[31], handle, unMaxAgeSeconds);
    }
    virtual bool SetCloudFileNameFilter(std::uint64_t handle, const char* pMatchCloudFileName) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[32], handle, pMatchCloudFileName);
    }
    virtual bool SetMatchAnyTag(std::uint64_t handle, bool bMatchAnyTag) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[33], handle, bMatchAnyTag);
    }
    virtual bool SetSearchText(std::uint64_t handle, const char* pSearchText) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[34], handle, pSearchText);
    }
    virtual bool SetRankedByTrendDays(std::uint64_t handle, std::uint32_t unDays) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[35], handle, unDays);
    }
    virtual bool SetTimeCreatedDateRange(std::uint64_t handle, std::uint32_t rtStart, std::uint32_t rtEnd) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[36], handle, rtStart, rtEnd);
    }
    virtual bool SetTimeUpdatedDateRange(std::uint64_t handle, std::uint32_t rtStart, std::uint32_t rtEnd) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[37], handle, rtStart, rtEnd);
    }
    virtual bool AddRequiredKeyValueTag(std::uint64_t handle, const char* pKey, const char* pValue) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[38], handle, pKey, pValue);
    }
    virtual std::uint64_t RequestUGCDetails(std::uint64_t nPublishedFileID, std::uint32_t unMaxAgeSeconds) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[39], nPublishedFileID, unMaxAgeSeconds);
    }
    virtual std::uint64_t CreateItem(std::uint32_t nConsumerAppId, std::int32_t eFileType) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[40], nConsumerAppId, eFileType);
    }
    virtual std::uint64_t StartItemUpdate(std::uint32_t nConsumerAppId, std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[41], nConsumerAppId, nPublishedFileID);
    }
    virtual bool SetItemTitle(std::uint64_t handle, const char* pchTitle) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[42], handle, pchTitle);
    }
    virtual bool SetItemDescription(std::uint64_t handle, const char* pchDescription) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[43], handle, pchDescription);
    }
    virtual bool SetItemUpdateLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[44], handle, pchLanguage);
    }
    virtual bool SetItemMetadata(std::uint64_t handle, const char* pchMetaData) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[45], handle, pchMetaData);
    }
    virtual bool SetItemVisibility(std::uint64_t handle, std::int32_t eVisibility) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[46], handle, eVisibility);
    }
    virtual bool SetItemTags(std::uint64_t updateHandle, void* pTags) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[47], updateHandle, pTags);
    }
    virtual bool SetItemContent(std::uint64_t handle, const char* pszContentFolder) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[48], handle, pszContentFolder);
    }
    virtual bool SetItemPreview(std::uint64_t handle, const char* pszPreviewFile) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[49], handle, pszPreviewFile);
    }
    virtual bool SetAllowLegacyUpload(std::uint64_t handle, bool bAllowLegacyUpload) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[50], handle, bAllowLegacyUpload);
    }
    virtual bool RemoveAllItemKeyValueTags(std::uint64_t handle) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[51], handle);
    }
    virtual bool RemoveItemKeyValueTags(std::uint64_t handle, const char* pchKey) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[52], handle, pchKey);
    }
    virtual bool AddItemKeyValueTag(std::uint64_t handle, const char* pchKey, const char* pchValue) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[53], handle, pchKey, pchValue);
    }
    virtual bool AddItemPreviewFile(std::uint64_t handle, const char* pszPreviewFile, std::int32_t type) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[54], handle, pszPreviewFile, type);
    }
    virtual bool AddItemPreviewVideo(std::uint64_t handle, const char* pszVideoID) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[55], handle, pszVideoID);
    }
    virtual bool UpdateItemPreviewFile(std::uint64_t handle, std::uint32_t index, const char* pszPreviewFile) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[56], handle, index, pszPreviewFile);
    }
    virtual bool UpdateItemPreviewVideo(std::uint64_t handle, std::uint32_t index, const char* pszVideoID) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[57], handle, index, pszVideoID);
    }
    virtual bool RemoveItemPreview(std::uint64_t handle, std::uint32_t index) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[58], handle, index);
    }
    virtual std::uint64_t SubmitItemUpdate(std::uint64_t handle, const char* pchChangeNote) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[59], handle, pchChangeNote);
    }
    virtual std::int32_t GetItemUpdateProgress(std::uint64_t handle, std::uint64_t* punBytesProcessed, std::uint64_t* punBytesTotal) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[60], handle, punBytesProcessed, punBytesTotal);
    }
    virtual std::uint64_t SetUserItemVote(std::uint64_t nPublishedFileID, bool bVoteUp) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[61], nPublishedFileID, bVoteUp);
    }
    virtual std::uint64_t GetUserItemVote(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[62], nPublishedFileID);
    }
    virtual std::uint64_t AddItemToFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[63], nAppId, nPublishedFileID);
    }
    virtual std::uint64_t RemoveItemFromFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[64], nAppId, nPublishedFileID);
    }
    virtual std::uint64_t SubscribeItem(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[65], nPublishedFileID);
    }
    virtual std::uint64_t UnsubscribeItem(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[66], nPublishedFileID);
    }
    virtual std::uint32_t GetNumSubscribedItems() {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[67]);
    }
    virtual std::uint32_t GetSubscribedItems(std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[68], pvecPublishedFileID, cMaxEntries);
    }
    virtual std::uint32_t GetItemState(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[69], nPublishedFileID);
    }
    virtual bool GetItemInstallInfo(std::uint64_t nPublishedFileID, std::uint64_t* punSizeOnDisk, void* pchFolder, std::uint32_t cchFolderSize, std::uint32_t* punTimeStamp) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[70], nPublishedFileID, punSizeOnDisk, pchFolder, cchFolderSize, punTimeStamp);
    }
    virtual bool GetItemDownloadInfo(std::uint64_t nPublishedFileID, std::uint64_t* punBytesDownloaded, std::uint64_t* punBytesTotal) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[71], nPublishedFileID, punBytesDownloaded, punBytesTotal);
    }
    virtual bool DownloadItem(std::uint64_t nPublishedFileID, bool bHighPriority) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[72], nPublishedFileID, bHighPriority);
    }
    virtual bool BInitWorkshopForGameServer(std::uint32_t unWorkshopDepotID, const char* pszFolder) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[73], unWorkshopDepotID, pszFolder);
    }
    virtual void SuspendDownloads(bool bSuspend) {
        steambridge::slot<void>(kSlots_STEAMUGC_INTERFACE_VERSION016[74], bSuspend);
    }
    virtual std::uint64_t StartPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[75], pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[76], pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTrackingForAllItems() {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[77]);
    }
    virtual std::uint64_t AddDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[78], nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t RemoveDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[79], nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t AddAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[80], nPublishedFileID, nAppID);
    }
    virtual std::uint64_t RemoveAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[81], nPublishedFileID, nAppID);
    }
    virtual std::uint64_t GetAppDependencies(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[82], nPublishedFileID);
    }
    virtual std::uint64_t DeleteItem(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[83], nPublishedFileID);
    }
    virtual bool ShowWorkshopEULA() {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION016[84]);
    }
    virtual std::uint64_t GetWorkshopEULAStatus() {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION016[85]);
    }
};

Version_STEAMUGC_INTERFACE_VERSION016 g_STEAMUGC_INTERFACE_VERSION016;

const char* const kParams_STEAMUGC_INTERFACE_VERSION017_0[] = {
    "unAccountID",
    "eListType",
    "eMatchingUGCType",
    "eSortOrder",
    "nCreatorAppID",
    "nConsumerAppID",
    "unPage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_1[] = {
    "eQueryType",
    "eMatchingeMatchingUGCTypeFileType",
    "nCreatorAppID",
    "nConsumerAppID",
    "unPage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_2[] = {
    "eQueryType",
    "eMatchingeMatchingUGCTypeFileType",
    "nCreatorAppID",
    "nConsumerAppID",
    "pchCursor",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_3[] = {
    "pvecPublishedFileID",
    "unNumPublishedFileIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_4[] = {
    "handle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_5[] = {
    "handle",
    "index",
    "pDetails",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_6[] = {
    "handle",
    "index",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_7[] = {
    "handle",
    "index",
    "indexTag",
    "pchValue",
    "cchValueSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_8[] = {
    "handle",
    "index",
    "indexTag",
    "pchValue",
    "cchValueSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_9[] = {
    "handle",
    "index",
    "pchURL",
    "cchURLSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_10[] = {
    "handle",
    "index",
    "pchMetadata",
    "cchMetadatasize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_11[] = {
    "handle",
    "index",
    "pvecPublishedFileID",
    "cMaxEntries",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_12[] = {
    "handle",
    "index",
    "eStatType",
    "pStatValue",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_13[] = {
    "handle",
    "index",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_14[] = {
    "handle",
    "index",
    "previewIndex",
    "pchURLOrVideoID",
    "cchURLSize",
    "pchOriginalFileName",
    "cchOriginalFileNameSize",
    "pPreviewType",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_15[] = {
    "handle",
    "index",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_16[] = {
    "handle",
    "index",
    "keyValueTagIndex",
    "pchKey",
    "cchKeySize",
    "pchValue",
    "cchValueSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_17[] = {
    "handle",
    "index",
    "pchKey",
    "pchValue",
    "cchValueSize",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_18[] = {
    "handle",
    "index",
    "pvecDescriptors",
    "cMaxEntries",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_19[] = {
    "handle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_20[] = {
    "handle",
    "pTagName",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_21[] = {
    "handle",
    "pTagGroups",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_22[] = {
    "handle",
    "pTagName",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_23[] = {
    "handle",
    "bReturnOnlyIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_24[] = {
    "handle",
    "bReturnKeyValueTags",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_25[] = {
    "handle",
    "bReturnLongDescription",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_26[] = {
    "handle",
    "bReturnMetadata",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_27[] = {
    "handle",
    "bReturnChildren",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_28[] = {
    "handle",
    "bReturnAdditionalPreviews",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_29[] = {
    "handle",
    "bReturnTotalOnly",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_30[] = {
    "handle",
    "unDays",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_31[] = {
    "handle",
    "pchLanguage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_32[] = {
    "handle",
    "unMaxAgeSeconds",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_33[] = {
    "handle",
    "pMatchCloudFileName",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_34[] = {
    "handle",
    "bMatchAnyTag",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_35[] = {
    "handle",
    "pSearchText",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_36[] = {
    "handle",
    "unDays",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_37[] = {
    "handle",
    "rtStart",
    "rtEnd",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_38[] = {
    "handle",
    "rtStart",
    "rtEnd",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_39[] = {
    "handle",
    "pKey",
    "pValue",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_40[] = {
    "nPublishedFileID",
    "unMaxAgeSeconds",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_41[] = {
    "nConsumerAppId",
    "eFileType",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_42[] = {
    "nConsumerAppId",
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_43[] = {
    "handle",
    "pchTitle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_44[] = {
    "handle",
    "pchDescription",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_45[] = {
    "handle",
    "pchLanguage",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_46[] = {
    "handle",
    "pchMetaData",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_47[] = {
    "handle",
    "eVisibility",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_48[] = {
    "updateHandle",
    "pTags",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_49[] = {
    "handle",
    "pszContentFolder",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_50[] = {
    "handle",
    "pszPreviewFile",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_51[] = {
    "handle",
    "bAllowLegacyUpload",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_52[] = {
    "handle",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_53[] = {
    "handle",
    "pchKey",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_54[] = {
    "handle",
    "pchKey",
    "pchValue",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_55[] = {
    "handle",
    "pszPreviewFile",
    "type",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_56[] = {
    "handle",
    "pszVideoID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_57[] = {
    "handle",
    "index",
    "pszPreviewFile",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_58[] = {
    "handle",
    "index",
    "pszVideoID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_59[] = {
    "handle",
    "index",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_60[] = {
    "handle",
    "descid",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_61[] = {
    "handle",
    "descid",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_62[] = {
    "handle",
    "pchChangeNote",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_63[] = {
    "handle",
    "punBytesProcessed",
    "punBytesTotal",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_64[] = {
    "nPublishedFileID",
    "bVoteUp",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_65[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_66[] = {
    "nAppId",
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_67[] = {
    "nAppId",
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_68[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_69[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_71[] = {
    "pvecPublishedFileID",
    "cMaxEntries",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_72[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_73[] = {
    "nPublishedFileID",
    "punSizeOnDisk",
    "pchFolder",
    "cchFolderSize",
    "punTimeStamp",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_74[] = {
    "nPublishedFileID",
    "punBytesDownloaded",
    "punBytesTotal",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_75[] = {
    "nPublishedFileID",
    "bHighPriority",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_76[] = {
    "unWorkshopDepotID",
    "pszFolder",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_77[] = {
    "bSuspend",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_78[] = {
    "pvecPublishedFileID",
    "unNumPublishedFileIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_79[] = {
    "pvecPublishedFileID",
    "unNumPublishedFileIDs",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_81[] = {
    "nParentPublishedFileID",
    "nChildPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_82[] = {
    "nParentPublishedFileID",
    "nChildPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_83[] = {
    "nPublishedFileID",
    "nAppID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_84[] = {
    "nPublishedFileID",
    "nAppID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_85[] = {
    "nPublishedFileID",
};
const char* const kParams_STEAMUGC_INTERFACE_VERSION017_86[] = {
    "nPublishedFileID",
};
// ISteamUGC STEAMUGC_INTERFACE_VERSION017
const steambridge::SlotInfo kSlots_STEAMUGC_INTERFACE_VERSION017[] = {
    {"SteamAPI_ISteamUGC_CreateQueryUserUGCRequest", kParams_STEAMUGC_INTERFACE_VERSION017_0},
    {"SteamAPI_ISteamUGC_CreateQueryAllUGCRequestPage", kParams_STEAMUGC_INTERFACE_VERSION017_1},
    {"SteamAPI_ISteamUGC_CreateQueryAllUGCRequestCursor", kParams_STEAMUGC_INTERFACE_VERSION017_2},
    {"SteamAPI_ISteamUGC_CreateQueryUGCDetailsRequest", kParams_STEAMUGC_INTERFACE_VERSION017_3},
    {"SteamAPI_ISteamUGC_SendQueryUGCRequest", kParams_STEAMUGC_INTERFACE_VERSION017_4},
    {"SteamAPI_ISteamUGC_GetQueryUGCResult", kParams_STEAMUGC_INTERFACE_VERSION017_5},
    {"SteamAPI_ISteamUGC_GetQueryUGCNumTags", kParams_STEAMUGC_INTERFACE_VERSION017_6},
    {"SteamAPI_ISteamUGC_GetQueryUGCTag", kParams_STEAMUGC_INTERFACE_VERSION017_7},
    {"SteamAPI_ISteamUGC_GetQueryUGCTagDisplayName", kParams_STEAMUGC_INTERFACE_VERSION017_8},
    {"SteamAPI_ISteamUGC_GetQueryUGCPreviewURL", kParams_STEAMUGC_INTERFACE_VERSION017_9},
    {"SteamAPI_ISteamUGC_GetQueryUGCMetadata", kParams_STEAMUGC_INTERFACE_VERSION017_10},
    {"SteamAPI_ISteamUGC_GetQueryUGCChildren", kParams_STEAMUGC_INTERFACE_VERSION017_11},
    {"SteamAPI_ISteamUGC_GetQueryUGCStatistic", kParams_STEAMUGC_INTERFACE_VERSION017_12},
    {"SteamAPI_ISteamUGC_GetQueryUGCNumAdditionalPreviews", kParams_STEAMUGC_INTERFACE_VERSION017_13},
    {"SteamAPI_ISteamUGC_GetQueryUGCAdditionalPreview", kParams_STEAMUGC_INTERFACE_VERSION017_14},
    {"SteamAPI_ISteamUGC_GetQueryUGCNumKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION017_15},
    {"SteamAPI_ISteamUGC_GetQueryUGCKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION017_16},
    {"SteamAPI_ISteamUGC_GetQueryFirstUGCKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION017_17},
    {"SteamAPI_ISteamUGC_GetQueryUGCContentDescriptors", kParams_STEAMUGC_INTERFACE_VERSION017_18},
    {"SteamAPI_ISteamUGC_ReleaseQueryUGCRequest", kParams_STEAMUGC_INTERFACE_VERSION017_19},
    {"SteamAPI_ISteamUGC_AddRequiredTag", kParams_STEAMUGC_INTERFACE_VERSION017_20},
    {"SteamAPI_ISteamUGC_AddRequiredTagGroup", kParams_STEAMUGC_INTERFACE_VERSION017_21},
    {"SteamAPI_ISteamUGC_AddExcludedTag", kParams_STEAMUGC_INTERFACE_VERSION017_22},
    {"SteamAPI_ISteamUGC_SetReturnOnlyIDs", kParams_STEAMUGC_INTERFACE_VERSION017_23},
    {"SteamAPI_ISteamUGC_SetReturnKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION017_24},
    {"SteamAPI_ISteamUGC_SetReturnLongDescription", kParams_STEAMUGC_INTERFACE_VERSION017_25},
    {"SteamAPI_ISteamUGC_SetReturnMetadata", kParams_STEAMUGC_INTERFACE_VERSION017_26},
    {"SteamAPI_ISteamUGC_SetReturnChildren", kParams_STEAMUGC_INTERFACE_VERSION017_27},
    {"SteamAPI_ISteamUGC_SetReturnAdditionalPreviews", kParams_STEAMUGC_INTERFACE_VERSION017_28},
    {"SteamAPI_ISteamUGC_SetReturnTotalOnly", kParams_STEAMUGC_INTERFACE_VERSION017_29},
    {"SteamAPI_ISteamUGC_SetReturnPlaytimeStats", kParams_STEAMUGC_INTERFACE_VERSION017_30},
    {"SteamAPI_ISteamUGC_SetLanguage", kParams_STEAMUGC_INTERFACE_VERSION017_31},
    {"SteamAPI_ISteamUGC_SetAllowCachedResponse", kParams_STEAMUGC_INTERFACE_VERSION017_32},
    {"SteamAPI_ISteamUGC_SetCloudFileNameFilter", kParams_STEAMUGC_INTERFACE_VERSION017_33},
    {"SteamAPI_ISteamUGC_SetMatchAnyTag", kParams_STEAMUGC_INTERFACE_VERSION017_34},
    {"SteamAPI_ISteamUGC_SetSearchText", kParams_STEAMUGC_INTERFACE_VERSION017_35},
    {"SteamAPI_ISteamUGC_SetRankedByTrendDays", kParams_STEAMUGC_INTERFACE_VERSION017_36},
    {"SteamAPI_ISteamUGC_SetTimeCreatedDateRange", kParams_STEAMUGC_INTERFACE_VERSION017_37},
    {"SteamAPI_ISteamUGC_SetTimeUpdatedDateRange", kParams_STEAMUGC_INTERFACE_VERSION017_38},
    {"SteamAPI_ISteamUGC_AddRequiredKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION017_39},
    {"SteamAPI_ISteamUGC_RequestUGCDetails", kParams_STEAMUGC_INTERFACE_VERSION017_40},
    {"SteamAPI_ISteamUGC_CreateItem", kParams_STEAMUGC_INTERFACE_VERSION017_41},
    {"SteamAPI_ISteamUGC_StartItemUpdate", kParams_STEAMUGC_INTERFACE_VERSION017_42},
    {"SteamAPI_ISteamUGC_SetItemTitle", kParams_STEAMUGC_INTERFACE_VERSION017_43},
    {"SteamAPI_ISteamUGC_SetItemDescription", kParams_STEAMUGC_INTERFACE_VERSION017_44},
    {"SteamAPI_ISteamUGC_SetItemUpdateLanguage", kParams_STEAMUGC_INTERFACE_VERSION017_45},
    {"SteamAPI_ISteamUGC_SetItemMetadata", kParams_STEAMUGC_INTERFACE_VERSION017_46},
    {"SteamAPI_ISteamUGC_SetItemVisibility", kParams_STEAMUGC_INTERFACE_VERSION017_47},
    {"SteamAPI_ISteamUGC_SetItemTags", kParams_STEAMUGC_INTERFACE_VERSION017_48},
    {"SteamAPI_ISteamUGC_SetItemContent", kParams_STEAMUGC_INTERFACE_VERSION017_49},
    {"SteamAPI_ISteamUGC_SetItemPreview", kParams_STEAMUGC_INTERFACE_VERSION017_50},
    {"SteamAPI_ISteamUGC_SetAllowLegacyUpload", kParams_STEAMUGC_INTERFACE_VERSION017_51},
    {"SteamAPI_ISteamUGC_RemoveAllItemKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION017_52},
    {"SteamAPI_ISteamUGC_RemoveItemKeyValueTags", kParams_STEAMUGC_INTERFACE_VERSION017_53},
    {"SteamAPI_ISteamUGC_AddItemKeyValueTag", kParams_STEAMUGC_INTERFACE_VERSION017_54},
    {"SteamAPI_ISteamUGC_AddItemPreviewFile", kParams_STEAMUGC_INTERFACE_VERSION017_55},
    {"SteamAPI_ISteamUGC_AddItemPreviewVideo", kParams_STEAMUGC_INTERFACE_VERSION017_56},
    {"SteamAPI_ISteamUGC_UpdateItemPreviewFile", kParams_STEAMUGC_INTERFACE_VERSION017_57},
    {"SteamAPI_ISteamUGC_UpdateItemPreviewVideo", kParams_STEAMUGC_INTERFACE_VERSION017_58},
    {"SteamAPI_ISteamUGC_RemoveItemPreview", kParams_STEAMUGC_INTERFACE_VERSION017_59},
    {"SteamAPI_ISteamUGC_AddContentDescriptor", kParams_STEAMUGC_INTERFACE_VERSION017_60},
    {"SteamAPI_ISteamUGC_RemoveContentDescriptor", kParams_STEAMUGC_INTERFACE_VERSION017_61},
    {"SteamAPI_ISteamUGC_SubmitItemUpdate", kParams_STEAMUGC_INTERFACE_VERSION017_62},
    {"SteamAPI_ISteamUGC_GetItemUpdateProgress", kParams_STEAMUGC_INTERFACE_VERSION017_63},
    {"SteamAPI_ISteamUGC_SetUserItemVote", kParams_STEAMUGC_INTERFACE_VERSION017_64},
    {"SteamAPI_ISteamUGC_GetUserItemVote", kParams_STEAMUGC_INTERFACE_VERSION017_65},
    {"SteamAPI_ISteamUGC_AddItemToFavorites", kParams_STEAMUGC_INTERFACE_VERSION017_66},
    {"SteamAPI_ISteamUGC_RemoveItemFromFavorites", kParams_STEAMUGC_INTERFACE_VERSION017_67},
    {"SteamAPI_ISteamUGC_SubscribeItem", kParams_STEAMUGC_INTERFACE_VERSION017_68},
    {"SteamAPI_ISteamUGC_UnsubscribeItem", kParams_STEAMUGC_INTERFACE_VERSION017_69},
    {"SteamAPI_ISteamUGC_GetNumSubscribedItems", nullptr},
    {"SteamAPI_ISteamUGC_GetSubscribedItems", kParams_STEAMUGC_INTERFACE_VERSION017_71},
    {"SteamAPI_ISteamUGC_GetItemState", kParams_STEAMUGC_INTERFACE_VERSION017_72},
    {"SteamAPI_ISteamUGC_GetItemInstallInfo", kParams_STEAMUGC_INTERFACE_VERSION017_73},
    {"SteamAPI_ISteamUGC_GetItemDownloadInfo", kParams_STEAMUGC_INTERFACE_VERSION017_74},
    {"SteamAPI_ISteamUGC_DownloadItem", kParams_STEAMUGC_INTERFACE_VERSION017_75},
    {"SteamAPI_ISteamUGC_BInitWorkshopForGameServer", kParams_STEAMUGC_INTERFACE_VERSION017_76},
    {"SteamAPI_ISteamUGC_SuspendDownloads", kParams_STEAMUGC_INTERFACE_VERSION017_77},
    {"SteamAPI_ISteamUGC_StartPlaytimeTracking", kParams_STEAMUGC_INTERFACE_VERSION017_78},
    {"SteamAPI_ISteamUGC_StopPlaytimeTracking", kParams_STEAMUGC_INTERFACE_VERSION017_79},
    {"SteamAPI_ISteamUGC_StopPlaytimeTrackingForAllItems", nullptr},
    {"SteamAPI_ISteamUGC_AddDependency", kParams_STEAMUGC_INTERFACE_VERSION017_81},
    {"SteamAPI_ISteamUGC_RemoveDependency", kParams_STEAMUGC_INTERFACE_VERSION017_82},
    {"SteamAPI_ISteamUGC_AddAppDependency", kParams_STEAMUGC_INTERFACE_VERSION017_83},
    {"SteamAPI_ISteamUGC_RemoveAppDependency", kParams_STEAMUGC_INTERFACE_VERSION017_84},
    {"SteamAPI_ISteamUGC_GetAppDependencies", kParams_STEAMUGC_INTERFACE_VERSION017_85},
    {"SteamAPI_ISteamUGC_DeleteItem", kParams_STEAMUGC_INTERFACE_VERSION017_86},
    {"SteamAPI_ISteamUGC_ShowWorkshopEULA", nullptr},
    {"SteamAPI_ISteamUGC_GetWorkshopEULAStatus", nullptr},
};

class Version_STEAMUGC_INTERFACE_VERSION017 {
public:
    virtual std::uint64_t CreateQueryUserUGCRequest(std::uint32_t unAccountID, std::int32_t eListType, std::int32_t eMatchingUGCType, std::int32_t eSortOrder, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[0], unAccountID, eListType, eMatchingUGCType, eSortOrder, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[1], eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, const char* pchCursor) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[2], eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, pchCursor);
    }
    virtual std::uint64_t CreateQueryUGCDetailsRequest(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[3], pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t SendQueryUGCRequest(std::uint64_t handle) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[4], handle);
    }
    virtual bool GetQueryUGCResult(std::uint64_t handle, std::uint32_t index, void* pDetails) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[5], handle, index, pDetails);
    }
    virtual std::uint32_t GetQueryUGCNumTags(std::uint64_t handle, std::uint32_t index) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[6], handle, index);
    }
    virtual bool GetQueryUGCTag(std::uint64_t handle, std::uint32_t index, std::uint32_t indexTag, void* pchValue, std::uint32_t cchValueSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[7], handle, index, indexTag, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCTagDisplayName(std::uint64_t handle, std::uint32_t index, std::uint32_t indexTag, void* pchValue, std::uint32_t cchValueSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[8], handle, index, indexTag, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCPreviewURL(std::uint64_t handle, std::uint32_t index, void* pchURL, std::uint32_t cchURLSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[9], handle, index, pchURL, cchURLSize);
    }
    virtual bool GetQueryUGCMetadata(std::uint64_t handle, std::uint32_t index, void* pchMetadata, std::uint32_t cchMetadatasize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[10], handle, index, pchMetadata, cchMetadatasize);
    }
    virtual bool GetQueryUGCChildren(std::uint64_t handle, std::uint32_t index, std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[11], handle, index, pvecPublishedFileID, cMaxEntries);
    }
    virtual bool GetQueryUGCStatistic(std::uint64_t handle, std::uint32_t index, std::int32_t eStatType, std::uint64_t* pStatValue) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[12], handle, index, eStatType, pStatValue);
    }
    virtual std::uint32_t GetQueryUGCNumAdditionalPreviews(std::uint64_t handle, std::uint32_t index) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[13], handle, index);
    }
    virtual bool GetQueryUGCAdditionalPreview(std::uint64_t handle, std::uint32_t index, std::uint32_t previewIndex, void* pchURLOrVideoID, std::uint32_t cchURLSize, void* pchOriginalFileName, std::uint32_t cchOriginalFileNameSize, std::int32_t* pPreviewType) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[14], handle, index, previewIndex, pchURLOrVideoID, cchURLSize, pchOriginalFileName, cchOriginalFileNameSize, pPreviewType);
    }
    virtual std::uint32_t GetQueryUGCNumKeyValueTags(std::uint64_t handle, std::uint32_t index) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[15], handle, index);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, std::uint32_t keyValueTagIndex, void* pchKey, std::uint32_t cchKeySize, void* pchValue, std::uint32_t cchValueSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[16], handle, index, keyValueTagIndex, pchKey, cchKeySize, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, const char* pchKey, void* pchValue, std::uint32_t cchValueSize) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[17], handle, index, pchKey, pchValue, cchValueSize);
    }
    virtual std::uint32_t GetQueryUGCContentDescriptors(std::uint64_t handle, std::uint32_t index, std::int32_t* pvecDescriptors, std::uint32_t cMaxEntries) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[18], handle, index, pvecDescriptors, cMaxEntries);
    }
    virtual bool ReleaseQueryUGCRequest(std::uint64_t handle) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[19], handle);
    }
    virtual bool AddRequiredTag(std::uint64_t handle, const char* pTagName) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[20], handle, pTagName);
    }
    virtual bool AddRequiredTagGroup(std::uint64_t handle, void* pTagGroups) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[21], handle, pTagGroups);
    }
    virtual bool AddExcludedTag(std::uint64_t handle, const char* pTagName) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[22], handle, pTagName);
    }
    virtual bool SetReturnOnlyIDs(std::uint64_t handle, bool bReturnOnlyIDs) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[23], handle, bReturnOnlyIDs);
    }
    virtual bool SetReturnKeyValueTags(std::uint64_t handle, bool bReturnKeyValueTags) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[24], handle, bReturnKeyValueTags);
    }
    virtual bool SetReturnLongDescription(std::uint64_t handle, bool bReturnLongDescription) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[25], handle, bReturnLongDescription);
    }
    virtual bool SetReturnMetadata(std::uint64_t handle, bool bReturnMetadata) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[26], handle, bReturnMetadata);
    }
    virtual bool SetReturnChildren(std::uint64_t handle, bool bReturnChildren) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[27], handle, bReturnChildren);
    }
    virtual bool SetReturnAdditionalPreviews(std::uint64_t handle, bool bReturnAdditionalPreviews) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[28], handle, bReturnAdditionalPreviews);
    }
    virtual bool SetReturnTotalOnly(std::uint64_t handle, bool bReturnTotalOnly) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[29], handle, bReturnTotalOnly);
    }
    virtual bool SetReturnPlaytimeStats(std::uint64_t handle, std::uint32_t unDays) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[30], handle, unDays);
    }
    virtual bool SetLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[31], handle, pchLanguage);
    }
    virtual bool SetAllowCachedResponse(std::uint64_t handle, std::uint32_t unMaxAgeSeconds) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[32], handle, unMaxAgeSeconds);
    }
    virtual bool SetCloudFileNameFilter(std::uint64_t handle, const char* pMatchCloudFileName) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[33], handle, pMatchCloudFileName);
    }
    virtual bool SetMatchAnyTag(std::uint64_t handle, bool bMatchAnyTag) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[34], handle, bMatchAnyTag);
    }
    virtual bool SetSearchText(std::uint64_t handle, const char* pSearchText) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[35], handle, pSearchText);
    }
    virtual bool SetRankedByTrendDays(std::uint64_t handle, std::uint32_t unDays) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[36], handle, unDays);
    }
    virtual bool SetTimeCreatedDateRange(std::uint64_t handle, std::uint32_t rtStart, std::uint32_t rtEnd) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[37], handle, rtStart, rtEnd);
    }
    virtual bool SetTimeUpdatedDateRange(std::uint64_t handle, std::uint32_t rtStart, std::uint32_t rtEnd) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[38], handle, rtStart, rtEnd);
    }
    virtual bool AddRequiredKeyValueTag(std::uint64_t handle, const char* pKey, const char* pValue) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[39], handle, pKey, pValue);
    }
    virtual std::uint64_t RequestUGCDetails(std::uint64_t nPublishedFileID, std::uint32_t unMaxAgeSeconds) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[40], nPublishedFileID, unMaxAgeSeconds);
    }
    virtual std::uint64_t CreateItem(std::uint32_t nConsumerAppId, std::int32_t eFileType) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[41], nConsumerAppId, eFileType);
    }
    virtual std::uint64_t StartItemUpdate(std::uint32_t nConsumerAppId, std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[42], nConsumerAppId, nPublishedFileID);
    }
    virtual bool SetItemTitle(std::uint64_t handle, const char* pchTitle) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[43], handle, pchTitle);
    }
    virtual bool SetItemDescription(std::uint64_t handle, const char* pchDescription) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[44], handle, pchDescription);
    }
    virtual bool SetItemUpdateLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[45], handle, pchLanguage);
    }
    virtual bool SetItemMetadata(std::uint64_t handle, const char* pchMetaData) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[46], handle, pchMetaData);
    }
    virtual bool SetItemVisibility(std::uint64_t handle, std::int32_t eVisibility) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[47], handle, eVisibility);
    }
    virtual bool SetItemTags(std::uint64_t updateHandle, void* pTags) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[48], updateHandle, pTags);
    }
    virtual bool SetItemContent(std::uint64_t handle, const char* pszContentFolder) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[49], handle, pszContentFolder);
    }
    virtual bool SetItemPreview(std::uint64_t handle, const char* pszPreviewFile) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[50], handle, pszPreviewFile);
    }
    virtual bool SetAllowLegacyUpload(std::uint64_t handle, bool bAllowLegacyUpload) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[51], handle, bAllowLegacyUpload);
    }
    virtual bool RemoveAllItemKeyValueTags(std::uint64_t handle) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[52], handle);
    }
    virtual bool RemoveItemKeyValueTags(std::uint64_t handle, const char* pchKey) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[53], handle, pchKey);
    }
    virtual bool AddItemKeyValueTag(std::uint64_t handle, const char* pchKey, const char* pchValue) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[54], handle, pchKey, pchValue);
    }
    virtual bool AddItemPreviewFile(std::uint64_t handle, const char* pszPreviewFile, std::int32_t type) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[55], handle, pszPreviewFile, type);
    }
    virtual bool AddItemPreviewVideo(std::uint64_t handle, const char* pszVideoID) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[56], handle, pszVideoID);
    }
    virtual bool UpdateItemPreviewFile(std::uint64_t handle, std::uint32_t index, const char* pszPreviewFile) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[57], handle, index, pszPreviewFile);
    }
    virtual bool UpdateItemPreviewVideo(std::uint64_t handle, std::uint32_t index, const char* pszVideoID) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[58], handle, index, pszVideoID);
    }
    virtual bool RemoveItemPreview(std::uint64_t handle, std::uint32_t index) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[59], handle, index);
    }
    virtual bool AddContentDescriptor(std::uint64_t handle, std::int32_t descid) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[60], handle, descid);
    }
    virtual bool RemoveContentDescriptor(std::uint64_t handle, std::int32_t descid) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[61], handle, descid);
    }
    virtual std::uint64_t SubmitItemUpdate(std::uint64_t handle, const char* pchChangeNote) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[62], handle, pchChangeNote);
    }
    virtual std::int32_t GetItemUpdateProgress(std::uint64_t handle, std::uint64_t* punBytesProcessed, std::uint64_t* punBytesTotal) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[63], handle, punBytesProcessed, punBytesTotal);
    }
    virtual std::uint64_t SetUserItemVote(std::uint64_t nPublishedFileID, bool bVoteUp) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[64], nPublishedFileID, bVoteUp);
    }
    virtual std::uint64_t GetUserItemVote(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[65], nPublishedFileID);
    }
    virtual std::uint64_t AddItemToFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[66], nAppId, nPublishedFileID);
    }
    virtual std::uint64_t RemoveItemFromFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[67], nAppId, nPublishedFileID);
    }
    virtual std::uint64_t SubscribeItem(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[68], nPublishedFileID);
    }
    virtual std::uint64_t UnsubscribeItem(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[69], nPublishedFileID);
    }
    virtual std::uint32_t GetNumSubscribedItems() {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[70]);
    }
    virtual std::uint32_t GetSubscribedItems(std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[71], pvecPublishedFileID, cMaxEntries);
    }
    virtual std::uint32_t GetItemState(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[72], nPublishedFileID);
    }
    virtual bool GetItemInstallInfo(std::uint64_t nPublishedFileID, std::uint64_t* punSizeOnDisk, void* pchFolder, std::uint32_t cchFolderSize, std::uint32_t* punTimeStamp) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[73], nPublishedFileID, punSizeOnDisk, pchFolder, cchFolderSize, punTimeStamp);
    }
    virtual bool GetItemDownloadInfo(std::uint64_t nPublishedFileID, std::uint64_t* punBytesDownloaded, std::uint64_t* punBytesTotal) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[74], nPublishedFileID, punBytesDownloaded, punBytesTotal);
    }
    virtual bool DownloadItem(std::uint64_t nPublishedFileID, bool bHighPriority) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[75], nPublishedFileID, bHighPriority);
    }
    virtual bool BInitWorkshopForGameServer(std::uint32_t unWorkshopDepotID, const char* pszFolder) {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[76], unWorkshopDepotID, pszFolder);
    }
    virtual void SuspendDownloads(bool bSuspend) {
        steambridge::slot<void>(kSlots_STEAMUGC_INTERFACE_VERSION017[77], bSuspend);
    }
    virtual std::uint64_t StartPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[78], pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[79], pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTrackingForAllItems() {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[80]);
    }
    virtual std::uint64_t AddDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[81], nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t RemoveDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[82], nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t AddAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[83], nPublishedFileID, nAppID);
    }
    virtual std::uint64_t RemoveAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[84], nPublishedFileID, nAppID);
    }
    virtual std::uint64_t GetAppDependencies(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[85], nPublishedFileID);
    }
    virtual std::uint64_t DeleteItem(std::uint64_t nPublishedFileID) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[86], nPublishedFileID);
    }
    virtual bool ShowWorkshopEULA() {
        return steambridge::slot<bool>(kSlots_STEAMUGC_INTERFACE_VERSION017[87]);
    }
    virtual std::uint64_t GetWorkshopEULAStatus() {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUGC_INTERFACE_VERSION017[88]);
    }
};

Version_STEAMUGC_INTERFACE_VERSION017 g_STEAMUGC_INTERFACE_VERSION017;

const char* const kParams_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001_0[] = {
    "pchServiceMethod",
    "pRequestBuffer",
    "unRequestBufferSize",
    "unContext",
};
const char* const kParams_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001_1[] = {
    "hHandle",
    "punResponseSize",
    "peResult",
};
const char* const kParams_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001_2[] = {
    "hHandle",
    "pResponseBuffer",
    "unResponseBufferSize",
    "bAutoRelease",
};
const char* const kParams_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001_3[] = {
    "hHandle",
};
const char* const kParams_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001_4[] = {
    "pchServiceNotification",
    "pNotificationBuffer",
    "unNotificationBufferSize",
};
// ISteamUnifiedMessages STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001
const steambridge::SlotInfo kSlots_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001[] = {
    {"SteamAPI_ISteamUnifiedMessages_SendMethod", kParams_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001_0},
    {"SteamAPI_ISteamUnifiedMessages_GetMethodResponseInfo", kParams_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001_1},
    {"SteamAPI_ISteamUnifiedMessages_GetMethodResponseData", kParams_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001_2},
    {"SteamAPI_ISteamUnifiedMessages_ReleaseMethod", kParams_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001_3},
    {"SteamAPI_ISteamUnifiedMessages_SendNotification", kParams_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001_4},
};

class Version_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001 {
public:
    virtual std::uint64_t SendMethod(const char* pchServiceMethod, void* pRequestBuffer, std::uint32_t unRequestBufferSize, std::uint64_t unContext) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001[0], pchServiceMethod, pRequestBuffer, unRequestBufferSize, unContext);
    }
    virtual bool GetMethodResponseInfo(std::uint64_t hHandle, std::uint32_t* punResponseSize, std::int32_t* peResult) {
        return steambridge::slot<bool>(kSlots_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001[1], hHandle, punResponseSize, peResult);
    }
    virtual bool GetMethodResponseData(std::uint64_t hHandle, void* pResponseBuffer, std::uint32_t unResponseBufferSize, bool bAutoRelease) {
        return steambridge::slot<bool>(kSlots_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001[2], hHandle, pResponseBuffer, unResponseBufferSize, bAutoRelease);
    }
    virtual bool ReleaseMethod(std::uint64_t hHandle) {
        return steambridge::slot<bool>(kSlots_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001[3], hHandle);
    }
    virtual bool SendNotification(const char* pchServiceNotification, void* pNotificationBuffer, std::uint32_t unNotificationBufferSize) {
        return steambridge::slot<bool>(kSlots_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001[4], pchServiceNotification, pNotificationBuffer, unNotificationBufferSize);
    }
};

Version_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001 g_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001;

const char* const kParams_SteamUser019_3[] = {
    "pAuthBlob",
    "cbMaxAuthBlob",
    "steamIDGameServer",
    "unIPServer",
    "usPortServer",
    "bSecure",
};
const char* const kParams_SteamUser019_4[] = {
    "unIPServer",
    "usPortServer",
};
const char* const kParams_SteamUser019_5[] = {
    "gameID",
    "eAppUsageEvent",
    "pchExtraInfo",
};
const char* const kParams_SteamUser019_6[] = {
    "pchBuffer",
    "cubBuffer",
};
const char* const kParams_SteamUser019_9[] = {
    "pcbCompressed",
    "pcbUncompressed_Deprecated",
    "nUncompressedVoiceDesiredSampleRate_Deprecated",
};
const char* const kParams_SteamUser019_10[] = {
    "bWantCompressed",
    "pDestBuffer",
    "cbDestBufferSize",
    "nBytesWritten",
    "bWantUncompressed_Deprecated",
    "pUncompressedDestBuffer_Deprecated",
    "cbUncompressedDestBufferSize_Deprecated",
    "nUncompressBytesWritten_Deprecated",
    "nUncompressedVoiceDesiredSampleRate_Deprecated",
};
const char* const kParams_SteamUser019_11[] = {
    "pCompressed",
    "cbCompressed",
    "pDestBuffer",
    "cbDestBufferSize",
    "nBytesWritten",
    "nDesiredSampleRate",
};
const char* const kParams_SteamUser019_13[] = {
    "pTicket",
    "cbMaxTicket",
    "pcbTicket",
};
const char* const kParams_SteamUser019_14[] = {
    "pAuthTicket",
    "cbAuthTicket",
    "steamID",
};
const char* const kParams_SteamUser019_15[] = {
    "steamID",
};
const char* const kParams_SteamUser019_16[] = {
    "hAuthTicket",
};
const char* const kParams_SteamUser019_17[] = {
    "steamID",
    "appID",
};
const char* const kParams_SteamUser019_19[] = {
    "steamIDGameServer",
    "unIPServer",
    "usPortServer",
};
const char* const kParams_SteamUser019_20[] = {
    "pDataToInclude",
    "cbDataToInclude",
};
const char* const kParams_SteamUser019_21[] = {
    "pTicket",
    "cbMaxTicket",
    "pcbTicket",
};
const char* const kParams_SteamUser019_22[] = {
    "nSeries",
    "bFoil",
};
const char* const kParams_SteamUser019_24[] = {
    "pchRedirectURL",
};
// ISteamUser SteamUser019
const steambridge::SlotInfo kSlots_SteamUser019[] = {
    {"SteamAPI_ISteamUser_GetHSteamUser", nullptr},
    {"SteamAPI_ISteamUser_BLoggedOn", nullptr},
    {"SteamAPI_ISteamUser_GetSteamID", nullptr},
    {"SteamAPI_ISteamUser_InitiateGameConnection", kParams_SteamUser019_3},
    {"SteamAPI_ISteamUser_TerminateGameConnection", kParams_SteamUser019_4},
    {"SteamAPI_ISteamUser_TrackAppUsageEvent", kParams_SteamUser019_5},
    {"SteamAPI_ISteamUser_GetUserDataFolder", kParams_SteamUser019_6},
    {"SteamAPI_ISteamUser_StartVoiceRecording", nullptr},
    {"SteamAPI_ISteamUser_StopVoiceRecording", nullptr},
    {"SteamAPI_ISteamUser_GetAvailableVoice", kParams_SteamUser019_9},
    {"SteamAPI_ISteamUser_GetVoice", kParams_SteamUser019_10},
    {"SteamAPI_ISteamUser_DecompressVoice", kParams_SteamUser019_11},
    {"SteamAPI_ISteamUser_GetVoiceOptimalSampleRate", nullptr},
    {"SteamAPI_ISteamUser_GetAuthSessionTicket", kParams_SteamUser019_13},
    {"SteamAPI_ISteamUser_BeginAuthSession", kParams_SteamUser019_14},
    {"SteamAPI_ISteamUser_EndAuthSession", kParams_SteamUser019_15},
    {"SteamAPI_ISteamUser_CancelAuthTicket", kParams_SteamUser019_16},
    {"SteamAPI_ISteamUser_UserHasLicenseForApp", kParams_SteamUser019_17},
    {"SteamAPI_ISteamUser_BIsBehindNAT", nullptr},
    {"SteamAPI_ISteamUser_AdvertiseGame", kParams_SteamUser019_19},
    {"SteamAPI_ISteamUser_RequestEncryptedAppTicket", kParams_SteamUser019_20},
    {"SteamAPI_ISteamUser_GetEncryptedAppTicket", kParams_SteamUser019_21},
    {"SteamAPI_ISteamUser_GetGameBadgeLevel", kParams_SteamUser019_22},
    {"SteamAPI_ISteamUser_GetPlayerSteamLevel", nullptr},
    {"SteamAPI_ISteamUser_RequestStoreAuthURL", kParams_SteamUser019_24},
    {"SteamAPI_ISteamUser_BIsPhoneVerified", nullptr},
    {"SteamAPI_ISteamUser_BIsTwoFactorEnabled", nullptr},
    {"SteamAPI_ISteamUser_BIsPhoneIdentifying", nullptr},
    {"SteamAPI_ISteamUser_BIsPhoneRequiringVerification", nullptr},
};

class Version_SteamUser019 {
public:
    virtual std::int32_t GetHSteamUser() {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser019[0]);
    }
    virtual bool BLoggedOn() {
        return steambridge::slot<bool>(kSlots_SteamUser019[1]);
    }
    virtual CSteamID GetSteamID() {
        return steambridge::slot<CSteamID>(kSlots_SteamUser019[2]);
    }
    virtual std::int32_t InitiateGameConnection(void* pAuthBlob, std::int32_t cbMaxAuthBlob, CSteamID steamIDGameServer, std::uint32_t unIPServer, std::uint16_t usPortServer, bool bSecure) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser019[3], pAuthBlob, cbMaxAuthBlob, steamIDGameServer, unIPServer, usPortServer, bSecure);
    }
    virtual void TerminateGameConnection(std::uint32_t unIPServer, std::uint16_t usPortServer) {
        steambridge::slot<void>(kSlots_SteamUser019[4], unIPServer, usPortServer);
    }
    virtual void TrackAppUsageEvent(CGameID gameID, std::int32_t eAppUsageEvent, const char* pchExtraInfo) {
        steambridge::slot<void>(kSlots_SteamUser019[5], gameID, eAppUsageEvent, pchExtraInfo);
    }
    virtual bool GetUserDataFolder(void* pchBuffer, std::int32_t cubBuffer) {
        return steambridge::slot<bool>(kSlots_SteamUser019[6], pchBuffer, cubBuffer);
    }
    virtual void StartVoiceRecording() {
        steambridge::slot<void>(kSlots_SteamUser019[7]);
    }
    virtual void StopVoiceRecording() {
        steambridge::slot<void>(kSlots_SteamUser019[8]);
    }
    virtual std::int32_t GetAvailableVoice(std::uint32_t* pcbCompressed, std::uint32_t* pcbUncompressed_Deprecated, std::uint32_t nUncompressedVoiceDesiredSampleRate_Deprecated) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser019[9], pcbCompressed, pcbUncompressed_Deprecated, nUncompressedVoiceDesiredSampleRate_Deprecated);
    }
    virtual std::int32_t GetVoice(bool bWantCompressed, void* pDestBuffer, std::uint32_t cbDestBufferSize, std::uint32_t* nBytesWritten, bool bWantUncompressed_Deprecated, void* pUncompressedDestBuffer_Deprecated, std::uint32_t cbUncompressedDestBufferSize_Deprecated, std::uint32_t* nUncompressBytesWritten_Deprecated, std::uint32_t nUncompressedVoiceDesiredSampleRate_Deprecated) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser019[10], bWantCompressed, pDestBuffer, cbDestBufferSize, nBytesWritten, bWantUncompressed_Deprecated, pUncompressedDestBuffer_Deprecated, cbUncompressedDestBufferSize_Deprecated, nUncompressBytesWritten_Deprecated, nUncompressedVoiceDesiredSampleRate_Deprecated);
    }
    virtual std::int32_t DecompressVoice(void* pCompressed, std::uint32_t cbCompressed, void* pDestBuffer, std::uint32_t cbDestBufferSize, std::uint32_t* nBytesWritten, std::uint32_t nDesiredSampleRate) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser019[11], pCompressed, cbCompressed, pDestBuffer, cbDestBufferSize, nBytesWritten, nDesiredSampleRate);
    }
    virtual std::uint32_t GetVoiceOptimalSampleRate() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamUser019[12]);
    }
    virtual std::uint32_t GetAuthSessionTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamUser019[13], pTicket, cbMaxTicket, pcbTicket);
    }
    virtual std::int32_t BeginAuthSession(void* pAuthTicket, std::int32_t cbAuthTicket, CSteamID steamID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser019[14], pAuthTicket, cbAuthTicket, steamID);
    }
    virtual void EndAuthSession(CSteamID steamID) {
        steambridge::slot<void>(kSlots_SteamUser019[15], steamID);
    }
    virtual void CancelAuthTicket(std::uint32_t hAuthTicket) {
        steambridge::slot<void>(kSlots_SteamUser019[16], hAuthTicket);
    }
    virtual std::int32_t UserHasLicenseForApp(CSteamID steamID, std::uint32_t appID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser019[17], steamID, appID);
    }
    virtual bool BIsBehindNAT() {
        return steambridge::slot<bool>(kSlots_SteamUser019[18]);
    }
    virtual void AdvertiseGame(CSteamID steamIDGameServer, std::uint32_t unIPServer, std::uint16_t usPortServer) {
        steambridge::slot<void>(kSlots_SteamUser019[19], steamIDGameServer, unIPServer, usPortServer);
    }
    virtual std::uint64_t RequestEncryptedAppTicket(void* pDataToInclude, std::int32_t cbDataToInclude) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamUser019[20], pDataToInclude, cbDataToInclude);
    }
    virtual bool GetEncryptedAppTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket) {
        return steambridge::slot<bool>(kSlots_SteamUser019[21], pTicket, cbMaxTicket, pcbTicket);
    }
    virtual std::int32_t GetGameBadgeLevel(std::int32_t nSeries, bool bFoil) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser019[22], nSeries, bFoil);
    }
    virtual std::int32_t GetPlayerSteamLevel() {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser019[23]);
    }
    virtual std::uint64_t RequestStoreAuthURL(const char* pchRedirectURL) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamUser019[24], pchRedirectURL);
    }
    virtual bool BIsPhoneVerified() {
        return steambridge::slot<bool>(kSlots_SteamUser019[25]);
    }
    virtual bool BIsTwoFactorEnabled() {
        return steambridge::slot<bool>(kSlots_SteamUser019[26]);
    }
    virtual bool BIsPhoneIdentifying() {
        return steambridge::slot<bool>(kSlots_SteamUser019[27]);
    }
    virtual bool BIsPhoneRequiringVerification() {
        return steambridge::slot<bool>(kSlots_SteamUser019[28]);
    }
};

Version_SteamUser019 g_SteamUser019;

const char* const kParams_SteamUser020_3[] = {
    "pAuthBlob",
    "cbMaxAuthBlob",
    "steamIDGameServer",
    "unIPServer",
    "usPortServer",
    "bSecure",
};
const char* const kParams_SteamUser020_4[] = {
    "unIPServer",
    "usPortServer",
};
const char* const kParams_SteamUser020_5[] = {
    "gameID",
    "eAppUsageEvent",
    "pchExtraInfo",
};
const char* const kParams_SteamUser020_6[] = {
    "pchBuffer",
    "cubBuffer",
};
const char* const kParams_SteamUser020_9[] = {
    "pcbCompressed",
    "pcbUncompressed_Deprecated",
    "nUncompressedVoiceDesiredSampleRate_Deprecated",
};
const char* const kParams_SteamUser020_10[] = {
    "bWantCompressed",
    "pDestBuffer",
    "cbDestBufferSize",
    "nBytesWritten",
    "bWantUncompressed_Deprecated",
    "pUncompressedDestBuffer_Deprecated",
    "cbUncompressedDestBufferSize_Deprecated",
    "nUncompressBytesWritten_Deprecated",
    "nUncompressedVoiceDesiredSampleRate_Deprecated",
};
const char* const kParams_SteamUser020_11[] = {
    "pCompressed",
    "cbCompressed",
    "pDestBuffer",
    "cbDestBufferSize",
    "nBytesWritten",
    "nDesiredSampleRate",
};
const char* const kParams_SteamUser020_13[] = {
    "pTicket",
    "cbMaxTicket",
    "pcbTicket",
};
const char* const kParams_SteamUser020_14[] = {
    "pAuthTicket",
    "cbAuthTicket",
    "steamID",
};
const char* const kParams_SteamUser020_15[] = {
    "steamID",
};
const char* const kParams_SteamUser020_16[] = {
    "hAuthTicket",
};
const char* const kParams_SteamUser020_17[] = {
    "steamID",
    "appID",
};
const char* const kParams_SteamUser020_19[] = {
    "steamIDGameServer",
    "unIPServer",
    "usPortServer",
};
const char* const kParams_SteamUser020_20[] = {
    "pDataToInclude",
    "cbDataToInclude",
};
const char* const kParams_SteamUser020_21[] = {
    "pTicket",
    "cbMaxTicket",
    "pcbTicket",
};
const char* const kParams_SteamUser020_22[] = {
    "nSeries",
    "bFoil",
};
const char* const kParams_SteamUser020_24[] = {
    "pchRedirectURL",
};
// ISteamUser SteamUser020
const steambridge::SlotInfo kSlots_SteamUser020[] = {
    {"SteamAPI_ISteamUser_GetHSteamUser", nullptr},
    {"SteamAPI_ISteamUser_BLoggedOn", nullptr},
    {"SteamAPI_ISteamUser_GetSteamID", nullptr},
    {"SteamAPI_ISteamUser_InitiateGameConnection", kParams_SteamUser020_3},
    {"SteamAPI_ISteamUser_TerminateGameConnection", kParams_SteamUser020_4},
    {"SteamAPI_ISteamUser_TrackAppUsageEvent", kParams_SteamUser020_5},
    {"SteamAPI_ISteamUser_GetUserDataFolder", kParams_SteamUser020_6},
    {"SteamAPI_ISteamUser_StartVoiceRecording", nullptr},
    {"SteamAPI_ISteamUser_StopVoiceRecording", nullptr},
    {"SteamAPI_ISteamUser_GetAvailableVoice", kParams_SteamUser020_9},
    {"SteamAPI_ISteamUser_GetVoice", kParams_SteamUser020_10},
    {"SteamAPI_ISteamUser_DecompressVoice", kParams_SteamUser020_11},
    {"SteamAPI_ISteamUser_GetVoiceOptimalSampleRate", nullptr},
    {"SteamAPI_ISteamUser_GetAuthSessionTicket", kParams_SteamUser020_13},
    {"SteamAPI_ISteamUser_BeginAuthSession", kParams_SteamUser020_14},
    {"SteamAPI_ISteamUser_EndAuthSession", kParams_SteamUser020_15},
    {"SteamAPI_ISteamUser_CancelAuthTicket", kParams_SteamUser020_16},
    {"SteamAPI_ISteamUser_UserHasLicenseForApp", kParams_SteamUser020_17},
    {"SteamAPI_ISteamUser_BIsBehindNAT", nullptr},
    {"SteamAPI_ISteamUser_AdvertiseGame", kParams_SteamUser020_19},
    {"SteamAPI_ISteamUser_RequestEncryptedAppTicket", kParams_SteamUser020_20},
    {"SteamAPI_ISteamUser_GetEncryptedAppTicket", kParams_SteamUser020_21},
    {"SteamAPI_ISteamUser_GetGameBadgeLevel", kParams_SteamUser020_22},
    {"SteamAPI_ISteamUser_GetPlayerSteamLevel", nullptr},
    {"SteamAPI_ISteamUser_RequestStoreAuthURL", kParams_SteamUser020_24},
    {"SteamAPI_ISteamUser_BIsPhoneVerified", nullptr},
    {"SteamAPI_ISteamUser_BIsTwoFactorEnabled", nullptr},
    {"SteamAPI_ISteamUser_BIsPhoneIdentifying", nullptr},
    {"SteamAPI_ISteamUser_BIsPhoneRequiringVerification", nullptr},
    {"SteamAPI_ISteamUser_GetMarketEligibility", nullptr},
    {"SteamAPI_ISteamUser_GetDurationControl", nullptr},
};

class Version_SteamUser020 {
public:
    virtual std::int32_t GetHSteamUser() {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser020[0]);
    }
    virtual bool BLoggedOn() {
        return steambridge::slot<bool>(kSlots_SteamUser020[1]);
    }
    virtual CSteamID GetSteamID() {
        return steambridge::slot<CSteamID>(kSlots_SteamUser020[2]);
    }
    virtual std::int32_t InitiateGameConnection(void* pAuthBlob, std::int32_t cbMaxAuthBlob, CSteamID steamIDGameServer, std::uint32_t unIPServer, std::uint16_t usPortServer, bool bSecure) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser020[3], pAuthBlob, cbMaxAuthBlob, steamIDGameServer, unIPServer, usPortServer, bSecure);
    }
    virtual void TerminateGameConnection(std::uint32_t unIPServer, std::uint16_t usPortServer) {
        steambridge::slot<void>(kSlots_SteamUser020[4], unIPServer, usPortServer);
    }
    virtual void TrackAppUsageEvent(CGameID gameID, std::int32_t eAppUsageEvent, const char* pchExtraInfo) {
        steambridge::slot<void>(kSlots_SteamUser020[5], gameID, eAppUsageEvent, pchExtraInfo);
    }
    virtual bool GetUserDataFolder(void* pchBuffer, std::int32_t cubBuffer) {
        return steambridge::slot<bool>(kSlots_SteamUser020[6], pchBuffer, cubBuffer);
    }
    virtual void StartVoiceRecording() {
        steambridge::slot<void>(kSlots_SteamUser020[7]);
    }
    virtual void StopVoiceRecording() {
        steambridge::slot<void>(kSlots_SteamUser020[8]);
    }
    virtual std::int32_t GetAvailableVoice(std::uint32_t* pcbCompressed, std::uint32_t* pcbUncompressed_Deprecated, std::uint32_t nUncompressedVoiceDesiredSampleRate_Deprecated) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser020[9], pcbCompressed, pcbUncompressed_Deprecated, nUncompressedVoiceDesiredSampleRate_Deprecated);
    }
    virtual std::int32_t GetVoice(bool bWantCompressed, void* pDestBuffer, std::uint32_t cbDestBufferSize, std::uint32_t* nBytesWritten, bool bWantUncompressed_Deprecated, void* pUncompressedDestBuffer_Deprecated, std::uint32_t cbUncompressedDestBufferSize_Deprecated, std::uint32_t* nUncompressBytesWritten_Deprecated, std::uint32_t nUncompressedVoiceDesiredSampleRate_Deprecated) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser020[10], bWantCompressed, pDestBuffer, cbDestBufferSize, nBytesWritten, bWantUncompressed_Deprecated, pUncompressedDestBuffer_Deprecated, cbUncompressedDestBufferSize_Deprecated, nUncompressBytesWritten_Deprecated, nUncompressedVoiceDesiredSampleRate_Deprecated);
    }
    virtual std::int32_t DecompressVoice(void* pCompressed, std::uint32_t cbCompressed, void* pDestBuffer, std::uint32_t cbDestBufferSize, std::uint32_t* nBytesWritten, std::uint32_t nDesiredSampleRate) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser020[11], pCompressed, cbCompressed, pDestBuffer, cbDestBufferSize, nBytesWritten, nDesiredSampleRate);
    }
    virtual std::uint32_t GetVoiceOptimalSampleRate() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamUser020[12]);
    }
    virtual std::uint32_t GetAuthSessionTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamUser020[13], pTicket, cbMaxTicket, pcbTicket);
    }
    virtual std::int32_t BeginAuthSession(void* pAuthTicket, std::int32_t cbAuthTicket, CSteamID steamID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser020[14], pAuthTicket, cbAuthTicket, steamID);
    }
    virtual void EndAuthSession(CSteamID steamID) {
        steambridge::slot<void>(kSlots_SteamUser020[15], steamID);
    }
    virtual void CancelAuthTicket(std::uint32_t hAuthTicket) {
        steambridge::slot<void>(kSlots_SteamUser020[16], hAuthTicket);
    }
    virtual std::int32_t UserHasLicenseForApp(CSteamID steamID, std::uint32_t appID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser020[17], steamID, appID);
    }
    virtual bool BIsBehindNAT() {
        return steambridge::slot<bool>(kSlots_SteamUser020[18]);
    }
    virtual void AdvertiseGame(CSteamID steamIDGameServer, std::uint32_t unIPServer, std::uint16_t usPortServer) {
        steambridge::slot<void>(kSlots_SteamUser020[19], steamIDGameServer, unIPServer, usPortServer);
    }
    virtual std::uint64_t RequestEncryptedAppTicket(void* pDataToInclude, std::int32_t cbDataToInclude) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamUser020[20], pDataToInclude, cbDataToInclude);
    }
    virtual bool GetEncryptedAppTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket) {
        return steambridge::slot<bool>(kSlots_SteamUser020[21], pTicket, cbMaxTicket, pcbTicket);
    }
    virtual std::int32_t GetGameBadgeLevel(std::int32_t nSeries, bool bFoil) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser020[22], nSeries, bFoil);
    }
    virtual std::int32_t GetPlayerSteamLevel() {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser020[23]);
    }
    virtual std::uint64_t RequestStoreAuthURL(const char* pchRedirectURL) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamUser020[24], pchRedirectURL);
    }
    virtual bool BIsPhoneVerified() {
        return steambridge::slot<bool>(kSlots_SteamUser020[25]);
    }
    virtual bool BIsTwoFactorEnabled() {
        return steambridge::slot<bool>(kSlots_SteamUser020[26]);
    }
    virtual bool BIsPhoneIdentifying() {
        return steambridge::slot<bool>(kSlots_SteamUser020[27]);
    }
    virtual bool BIsPhoneRequiringVerification() {
        return steambridge::slot<bool>(kSlots_SteamUser020[28]);
    }
    virtual std::uint64_t GetMarketEligibility() {
        return steambridge::slot<std::uint64_t>(kSlots_SteamUser020[29]);
    }
    virtual std::uint64_t GetDurationControl() {
        return steambridge::slot<std::uint64_t>(kSlots_SteamUser020[30]);
    }
};

Version_SteamUser020 g_SteamUser020;

const char* const kParams_SteamUser021_3[] = {
    "pAuthBlob",
    "cbMaxAuthBlob",
    "steamIDGameServer",
    "unIPServer",
    "usPortServer",
    "bSecure",
};
const char* const kParams_SteamUser021_4[] = {
    "unIPServer",
    "usPortServer",
};
const char* const kParams_SteamUser021_5[] = {
    "gameID",
    "eAppUsageEvent",
    "pchExtraInfo",
};
const char* const kParams_SteamUser021_6[] = {
    "pchBuffer",
    "cubBuffer",
};
const char* const kParams_SteamUser021_9[] = {
    "pcbCompressed",
    "pcbUncompressed_Deprecated",
    "nUncompressedVoiceDesiredSampleRate_Deprecated",
};
const char* const kParams_SteamUser021_10[] = {
    "bWantCompressed",
    "pDestBuffer",
    "cbDestBufferSize",
    "nBytesWritten",
    "bWantUncompressed_Deprecated",
    "pUncompressedDestBuffer_Deprecated",
    "cbUncompressedDestBufferSize_Deprecated",
    "nUncompressBytesWritten_Deprecated",
    "nUncompressedVoiceDesiredSampleRate_Deprecated",
};
const char* const kParams_SteamUser021_11[] = {
    "pCompressed",
    "cbCompressed",
    "pDestBuffer",
    "cbDestBufferSize",
    "nBytesWritten",
    "nDesiredSampleRate",
};
const char* const kParams_SteamUser021_13[] = {
    "pTicket",
    "cbMaxTicket",
    "pcbTicket",
};
const char* const kParams_SteamUser021_14[] = {
    "pAuthTicket",
    "cbAuthTicket",
    "steamID",
};
const char* const kParams_SteamUser021_15[] = {
    "steamID",
};
const char* const kParams_SteamUser021_16[] = {
    "hAuthTicket",
};
const char* const kParams_SteamUser021_17[] = {
    "steamID",
    "appID",
};
const char* const kParams_SteamUser021_19[] = {
    "steamIDGameServer",
    "unIPServer",
    "usPortServer",
};
const char* const kParams_SteamUser021_20[] = {
    "pDataToInclude",
    "cbDataToInclude",
};
const char* const kParams_SteamUser021_21[] = {
    "pTicket",
    "cbMaxTicket",
    "pcbTicket",
};
const char* const kParams_SteamUser021_22[] = {
    "nSeries",
    "bFoil",
};
const char* const kParams_SteamUser021_24[] = {
    "pchRedirectURL",
};
const char* const kParams_SteamUser021_31[] = {
    "eNewState",
};
// ISteamUser SteamUser021
const steambridge::SlotInfo kSlots_SteamUser021[] = {
    {"SteamAPI_ISteamUser_GetHSteamUser", nullptr},
    {"SteamAPI_ISteamUser_BLoggedOn", nullptr},
    {"SteamAPI_ISteamUser_GetSteamID", nullptr},
    {"SteamAPI_ISteamUser_InitiateGameConnection_DEPRECATED", kParams_SteamUser021_3},
    {"SteamAPI_ISteamUser_TerminateGameConnection_DEPRECATED", kParams_SteamUser021_4},
    {"SteamAPI_ISteamUser_TrackAppUsageEvent", kParams_SteamUser021_5},
    {"SteamAPI_ISteamUser_GetUserDataFolder", kParams_SteamUser021_6},
    {"SteamAPI_ISteamUser_StartVoiceRecording", nullptr},
    {"SteamAPI_ISteamUser_StopVoiceRecording", nullptr},
    {"SteamAPI_ISteamUser_GetAvailableVoice", kParams_SteamUser021_9},
    {"SteamAPI_ISteamUser_GetVoice", kParams_SteamUser021_10},
    {"SteamAPI_ISteamUser_DecompressVoice", kParams_SteamUser021_11},
    {"SteamAPI_ISteamUser_GetVoiceOptimalSampleRate", nullptr},
    {"SteamAPI_ISteamUser_GetAuthSessionTicket", kParams_SteamUser021_13},
    {"SteamAPI_ISteamUser_BeginAuthSession", kParams_SteamUser021_14},
    {"SteamAPI_ISteamUser_EndAuthSession", kParams_SteamUser021_15},
    {"SteamAPI_ISteamUser_CancelAuthTicket", kParams_SteamUser021_16},
    {"SteamAPI_ISteamUser_UserHasLicenseForApp", kParams_SteamUser021_17},
    {"SteamAPI_ISteamUser_BIsBehindNAT", nullptr},
    {"SteamAPI_ISteamUser_AdvertiseGame", kParams_SteamUser021_19},
    {"SteamAPI_ISteamUser_RequestEncryptedAppTicket", kParams_SteamUser021_20},
    {"SteamAPI_ISteamUser_GetEncryptedAppTicket", kParams_SteamUser021_21},
    {"SteamAPI_ISteamUser_GetGameBadgeLevel", kParams_SteamUser021_22},
    {"SteamAPI_ISteamUser_GetPlayerSteamLevel", nullptr},
    {"SteamAPI_ISteamUser_RequestStoreAuthURL", kParams_SteamUser021_24},
    {"SteamAPI_ISteamUser_BIsPhoneVerified", nullptr},
    {"SteamAPI_ISteamUser_BIsTwoFactorEnabled", nullptr},
    {"SteamAPI_ISteamUser_BIsPhoneIdentifying", nullptr},
    {"SteamAPI_ISteamUser_BIsPhoneRequiringVerification", nullptr},
    {"SteamAPI_ISteamUser_GetMarketEligibility", nullptr},
    {"SteamAPI_ISteamUser_GetDurationControl", nullptr},
    {"SteamAPI_ISteamUser_BSetDurationControlOnlineState", kParams_SteamUser021_31},
};

class Version_SteamUser021 {
public:
    virtual std::int32_t GetHSteamUser() {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser021[0]);
    }
    virtual bool BLoggedOn() {
        return steambridge::slot<bool>(kSlots_SteamUser021[1]);
    }
    virtual CSteamID GetSteamID() {
        return steambridge::slot<CSteamID>(kSlots_SteamUser021[2]);
    }
    virtual std::int32_t InitiateGameConnection_DEPRECATED(void* pAuthBlob, std::int32_t cbMaxAuthBlob, CSteamID steamIDGameServer, std::uint32_t unIPServer, std::uint16_t usPortServer, bool bSecure) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser021[3], pAuthBlob, cbMaxAuthBlob, steamIDGameServer, unIPServer, usPortServer, bSecure);
    }
    virtual void TerminateGameConnection_DEPRECATED(std::uint32_t unIPServer, std::uint16_t usPortServer) {
        steambridge::slot<void>(kSlots_SteamUser021[4], unIPServer, usPortServer);
    }
    virtual void TrackAppUsageEvent(CGameID gameID, std::int32_t eAppUsageEvent, const char* pchExtraInfo) {
        steambridge::slot<void>(kSlots_SteamUser021[5], gameID, eAppUsageEvent, pchExtraInfo);
    }
    virtual bool GetUserDataFolder(void* pchBuffer, std::int32_t cubBuffer) {
        return steambridge::slot<bool>(kSlots_SteamUser021[6], pchBuffer, cubBuffer);
    }
    virtual void StartVoiceRecording() {
        steambridge::slot<void>(kSlots_SteamUser021[7]);
    }
    virtual void StopVoiceRecording() {
        steambridge::slot<void>(kSlots_SteamUser021[8]);
    }
    virtual std::int32_t GetAvailableVoice(std::uint32_t* pcbCompressed, std::uint32_t* pcbUncompressed_Deprecated, std::uint32_t nUncompressedVoiceDesiredSampleRate_Deprecated) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser021[9], pcbCompressed, pcbUncompressed_Deprecated, nUncompressedVoiceDesiredSampleRate_Deprecated);
    }
    virtual std::int32_t GetVoice(bool bWantCompressed, void* pDestBuffer, std::uint32_t cbDestBufferSize, std::uint32_t* nBytesWritten, bool bWantUncompressed_Deprecated, void* pUncompressedDestBuffer_Deprecated, std::uint32_t cbUncompressedDestBufferSize_Deprecated, std::uint32_t* nUncompressBytesWritten_Deprecated, std::uint32_t nUncompressedVoiceDesiredSampleRate_Deprecated) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser021[10], bWantCompressed, pDestBuffer, cbDestBufferSize, nBytesWritten, bWantUncompressed_Deprecated, pUncompressedDestBuffer_Deprecated, cbUncompressedDestBufferSize_Deprecated, nUncompressBytesWritten_Deprecated, nUncompressedVoiceDesiredSampleRate_Deprecated);
    }
    virtual std::int32_t DecompressVoice(void* pCompressed, std::uint32_t cbCompressed, void* pDestBuffer, std::uint32_t cbDestBufferSize, std::uint32_t* nBytesWritten, std::uint32_t nDesiredSampleRate) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser021[11], pCompressed, cbCompressed, pDestBuffer, cbDestBufferSize, nBytesWritten, nDesiredSampleRate);
    }
    virtual std::uint32_t GetVoiceOptimalSampleRate() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamUser021[12]);
    }
    virtual std::uint32_t GetAuthSessionTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamUser021[13], pTicket, cbMaxTicket, pcbTicket);
    }
    virtual std::int32_t BeginAuthSession(void* pAuthTicket, std::int32_t cbAuthTicket, CSteamID steamID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser021[14], pAuthTicket, cbAuthTicket, steamID);
    }
    virtual void EndAuthSession(CSteamID steamID) {
        steambridge::slot<void>(kSlots_SteamUser021[15], steamID);
    }
    virtual void CancelAuthTicket(std::uint32_t hAuthTicket) {
        steambridge::slot<void>(kSlots_SteamUser021[16], hAuthTicket);
    }
    virtual std::int32_t UserHasLicenseForApp(CSteamID steamID, std::uint32_t appID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser021[17], steamID, appID);
    }
    virtual bool BIsBehindNAT() {
        return steambridge::slot<bool>(kSlots_SteamUser021[18]);
    }
    virtual void AdvertiseGame(CSteamID steamIDGameServer, std::uint32_t unIPServer, std::uint16_t usPortServer) {
        steambridge::slot<void>(kSlots_SteamUser021[19], steamIDGameServer, unIPServer, usPortServer);
    }
    virtual std::uint64_t RequestEncryptedAppTicket(void* pDataToInclude, std::int32_t cbDataToInclude) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamUser021[20], pDataToInclude, cbDataToInclude);
    }
    virtual bool GetEncryptedAppTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket) {
        return steambridge::slot<bool>(kSlots_SteamUser021[21], pTicket, cbMaxTicket, pcbTicket);
    }
    virtual std::int32_t GetGameBadgeLevel(std::int32_t nSeries, bool bFoil) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser021[22], nSeries, bFoil);
    }
    virtual std::int32_t GetPlayerSteamLevel() {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser021[23]);
    }
    virtual std::uint64_t RequestStoreAuthURL(const char* pchRedirectURL) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamUser021[24], pchRedirectURL);
    }
    virtual bool BIsPhoneVerified() {
        return steambridge::slot<bool>(kSlots_SteamUser021[25]);
    }
    virtual bool BIsTwoFactorEnabled() {
        return steambridge::slot<bool>(kSlots_SteamUser021[26]);
    }
    virtual bool BIsPhoneIdentifying() {
        return steambridge::slot<bool>(kSlots_SteamUser021[27]);
    }
    virtual bool BIsPhoneRequiringVerification() {
        return steambridge::slot<bool>(kSlots_SteamUser021[28]);
    }
    virtual std::uint64_t GetMarketEligibility() {
        return steambridge::slot<std::uint64_t>(kSlots_SteamUser021[29]);
    }
    virtual std::uint64_t GetDurationControl() {
        return steambridge::slot<std::uint64_t>(kSlots_SteamUser021[30]);
    }
    virtual bool BSetDurationControlOnlineState(std::int32_t eNewState) {
        return steambridge::slot<bool>(kSlots_SteamUser021[31], eNewState);
    }
};

Version_SteamUser021 g_SteamUser021;

const char* const kParams_SteamUser023_3[] = {
    "pAuthBlob",
    "cbMaxAuthBlob",
    "steamIDGameServer",
    "unIPServer",
    "usPortServer",
    "bSecure",
};
const char* const kParams_SteamUser023_4[] = {
    "unIPServer",
    "usPortServer",
};
const char* const kParams_SteamUser023_5[] = {
    "gameID",
    "eAppUsageEvent",
    "pchExtraInfo",
};
const char* const kParams_SteamUser023_6[] = {
    "pchBuffer",
    "cubBuffer",
};
const char* const kParams_SteamUser023_9[] = {
    "pcbCompressed",
    "pcbUncompressed_Deprecated",
    "nUncompressedVoiceDesiredSampleRate_Deprecated",
};
const char* const kParams_SteamUser023_10[] = {
    "bWantCompressed",
    "pDestBuffer",
    "cbDestBufferSize",
    "nBytesWritten",
    "bWantUncompressed_Deprecated",
    "pUncompressedDestBuffer_Deprecated",
    "cbUncompressedDestBufferSize_Deprecated",
    "nUncompressBytesWritten_Deprecated",
    "nUncompressedVoiceDesiredSampleRate_Deprecated",
};
const char* const kParams_SteamUser023_11[] = {
    "pCompressed",
    "cbCompressed",
    "pDestBuffer",
    "cbDestBufferSize",
    "nBytesWritten",
    "nDesiredSampleRate",
};
const char* const kParams_SteamUser023_13[] = {
    "pTicket",
    "cbMaxTicket",
    "pcbTicket",
    "pSteamNetworkingIdentity",
};
const char* const kParams_SteamUser023_14[] = {
    "pchIdentity",
};
const char* const kParams_SteamUser023_15[] = {
    "pAuthTicket",
    "cbAuthTicket",
    "steamID",
};
const char* const kParams_SteamUser023_16[] = {
    "steamID",
};
const char* const kParams_SteamUser023_17[] = {
    "hAuthTicket",
};
const char* const kParams_SteamUser023_18[] = {
    "steamID",
    "appID",
};
const char* const kParams_SteamUser023_20[] = {
    "steamIDGameServer",
    "unIPServer",
    "usPortServer",
};
const char* const kParams_SteamUser023_21[] = {
    "pDataToInclude",
    "cbDataToInclude",
};
const char* const kParams_SteamUser023_22[] = {
    "pTicket",
    "cbMaxTicket",
    "pcbTicket",
};
const char* const kParams_SteamUser023_23[] = {
    "nSeries",
    "bFoil",
};
const char* const kParams_SteamUser023_25[] = {
    "pchRedirectURL",
};
const char* const kParams_SteamUser023_32[] = {
    "eNewState",
};
// ISteamUser SteamUser023
const steambridge::SlotInfo kSlots_SteamUser023[] = {
    {"SteamAPI_ISteamUser_GetHSteamUser", nullptr},
    {"SteamAPI_ISteamUser_BLoggedOn", nullptr},
    {"SteamAPI_ISteamUser_GetSteamID", nullptr},
    {"SteamAPI_ISteamUser_InitiateGameConnection_DEPRECATED", kParams_SteamUser023_3},
    {"SteamAPI_ISteamUser_TerminateGameConnection_DEPRECATED", kParams_SteamUser023_4},
    {"SteamAPI_ISteamUser_TrackAppUsageEvent", kParams_SteamUser023_5},
    {"SteamAPI_ISteamUser_GetUserDataFolder", kParams_SteamUser023_6},
    {"SteamAPI_ISteamUser_StartVoiceRecording", nullptr},
    {"SteamAPI_ISteamUser_StopVoiceRecording", nullptr},
    {"SteamAPI_ISteamUser_GetAvailableVoice", kParams_SteamUser023_9},
    {"SteamAPI_ISteamUser_GetVoice", kParams_SteamUser023_10},
    {"SteamAPI_ISteamUser_DecompressVoice", kParams_SteamUser023_11},
    {"SteamAPI_ISteamUser_GetVoiceOptimalSampleRate", nullptr},
    {"SteamAPI_ISteamUser_GetAuthSessionTicket", kParams_SteamUser023_13},
    {"SteamAPI_ISteamUser_GetAuthTicketForWebApi", kParams_SteamUser023_14},
    {"SteamAPI_ISteamUser_BeginAuthSession", kParams_SteamUser023_15},
    {"SteamAPI_ISteamUser_EndAuthSession", kParams_SteamUser023_16},
    {"SteamAPI_ISteamUser_CancelAuthTicket", kParams_SteamUser023_17},
    {"SteamAPI_ISteamUser_UserHasLicenseForApp", kParams_SteamUser023_18},
    {"SteamAPI_ISteamUser_BIsBehindNAT", nullptr},
    {"SteamAPI_ISteamUser_AdvertiseGame", kParams_SteamUser023_20},
    {"SteamAPI_ISteamUser_RequestEncryptedAppTicket", kParams_SteamUser023_21},
    {"SteamAPI_ISteamUser_GetEncryptedAppTicket", kParams_SteamUser023_22},
    {"SteamAPI_ISteamUser_GetGameBadgeLevel", kParams_SteamUser023_23},
    {"SteamAPI_ISteamUser_GetPlayerSteamLevel", nullptr},
    {"SteamAPI_ISteamUser_RequestStoreAuthURL", kParams_SteamUser023_25},
    {"SteamAPI_ISteamUser_BIsPhoneVerified", nullptr},
    {"SteamAPI_ISteamUser_BIsTwoFactorEnabled", nullptr},
    {"SteamAPI_ISteamUser_BIsPhoneIdentifying", nullptr},
    {"SteamAPI_ISteamUser_BIsPhoneRequiringVerification", nullptr},
    {"SteamAPI_ISteamUser_GetMarketEligibility", nullptr},
    {"SteamAPI_ISteamUser_GetDurationControl", nullptr},
    {"SteamAPI_ISteamUser_BSetDurationControlOnlineState", kParams_SteamUser023_32},
};

class Version_SteamUser023 {
public:
    virtual std::int32_t GetHSteamUser() {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser023[0]);
    }
    virtual bool BLoggedOn() {
        return steambridge::slot<bool>(kSlots_SteamUser023[1]);
    }
    virtual CSteamID GetSteamID() {
        return steambridge::slot<CSteamID>(kSlots_SteamUser023[2]);
    }
    virtual std::int32_t InitiateGameConnection_DEPRECATED(void* pAuthBlob, std::int32_t cbMaxAuthBlob, CSteamID steamIDGameServer, std::uint32_t unIPServer, std::uint16_t usPortServer, bool bSecure) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser023[3], pAuthBlob, cbMaxAuthBlob, steamIDGameServer, unIPServer, usPortServer, bSecure);
    }
    virtual void TerminateGameConnection_DEPRECATED(std::uint32_t unIPServer, std::uint16_t usPortServer) {
        steambridge::slot<void>(kSlots_SteamUser023[4], unIPServer, usPortServer);
    }
    virtual void TrackAppUsageEvent(CGameID gameID, std::int32_t eAppUsageEvent, const char* pchExtraInfo) {
        steambridge::slot<void>(kSlots_SteamUser023[5], gameID, eAppUsageEvent, pchExtraInfo);
    }
    virtual bool GetUserDataFolder(void* pchBuffer, std::int32_t cubBuffer) {
        return steambridge::slot<bool>(kSlots_SteamUser023[6], pchBuffer, cubBuffer);
    }
    virtual void StartVoiceRecording() {
        steambridge::slot<void>(kSlots_SteamUser023[7]);
    }
    virtual void StopVoiceRecording() {
        steambridge::slot<void>(kSlots_SteamUser023[8]);
    }
    virtual std::int32_t GetAvailableVoice(std::uint32_t* pcbCompressed, std::uint32_t* pcbUncompressed_Deprecated, std::uint32_t nUncompressedVoiceDesiredSampleRate_Deprecated) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser023[9], pcbCompressed, pcbUncompressed_Deprecated, nUncompressedVoiceDesiredSampleRate_Deprecated);
    }
    virtual std::int32_t GetVoice(bool bWantCompressed, void* pDestBuffer, std::uint32_t cbDestBufferSize, std::uint32_t* nBytesWritten, bool bWantUncompressed_Deprecated, void* pUncompressedDestBuffer_Deprecated, std::uint32_t cbUncompressedDestBufferSize_Deprecated, std::uint32_t* nUncompressBytesWritten_Deprecated, std::uint32_t nUncompressedVoiceDesiredSampleRate_Deprecated) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser023[10], bWantCompressed, pDestBuffer, cbDestBufferSize, nBytesWritten, bWantUncompressed_Deprecated, pUncompressedDestBuffer_Deprecated, cbUncompressedDestBufferSize_Deprecated, nUncompressBytesWritten_Deprecated, nUncompressedVoiceDesiredSampleRate_Deprecated);
    }
    virtual std::int32_t DecompressVoice(void* pCompressed, std::uint32_t cbCompressed, void* pDestBuffer, std::uint32_t cbDestBufferSize, std::uint32_t* nBytesWritten, std::uint32_t nDesiredSampleRate) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser023[11], pCompressed, cbCompressed, pDestBuffer, cbDestBufferSize, nBytesWritten, nDesiredSampleRate);
    }
    virtual std::uint32_t GetVoiceOptimalSampleRate() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamUser023[12]);
    }
    virtual std::uint32_t GetAuthSessionTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket, void* pSteamNetworkingIdentity) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamUser023[13], pTicket, cbMaxTicket, pcbTicket, pSteamNetworkingIdentity);
    }
    virtual std::uint32_t GetAuthTicketForWebApi(const char* pchIdentity) {
        return steambridge::slot<std::uint32_t>(kSlots_SteamUser023[14], pchIdentity);
    }
    virtual std::int32_t BeginAuthSession(void* pAuthTicket, std::int32_t cbAuthTicket, CSteamID steamID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser023[15], pAuthTicket, cbAuthTicket, steamID);
    }
    virtual void EndAuthSession(CSteamID steamID) {
        steambridge::slot<void>(kSlots_SteamUser023[16], steamID);
    }
    virtual void CancelAuthTicket(std::uint32_t hAuthTicket) {
        steambridge::slot<void>(kSlots_SteamUser023[17], hAuthTicket);
    }
    virtual std::int32_t UserHasLicenseForApp(CSteamID steamID, std::uint32_t appID) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser023[18], steamID, appID);
    }
    virtual bool BIsBehindNAT() {
        return steambridge::slot<bool>(kSlots_SteamUser023[19]);
    }
    virtual void AdvertiseGame(CSteamID steamIDGameServer, std::uint32_t unIPServer, std::uint16_t usPortServer) {
        steambridge::slot<void>(kSlots_SteamUser023[20], steamIDGameServer, unIPServer, usPortServer);
    }
    virtual std::uint64_t RequestEncryptedAppTicket(void* pDataToInclude, std::int32_t cbDataToInclude) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamUser023[21], pDataToInclude, cbDataToInclude);
    }
    virtual bool GetEncryptedAppTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket) {
        return steambridge::slot<bool>(kSlots_SteamUser023[22], pTicket, cbMaxTicket, pcbTicket);
    }
    virtual std::int32_t GetGameBadgeLevel(std::int32_t nSeries, bool bFoil) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser023[23], nSeries, bFoil);
    }
    virtual std::int32_t GetPlayerSteamLevel() {
        return steambridge::slot<std::int32_t>(kSlots_SteamUser023[24]);
    }
    virtual std::uint64_t RequestStoreAuthURL(const char* pchRedirectURL) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamUser023[25], pchRedirectURL);
    }
    virtual bool BIsPhoneVerified() {
        return steambridge::slot<bool>(kSlots_SteamUser023[26]);
    }
    virtual bool BIsTwoFactorEnabled() {
        return steambridge::slot<bool>(kSlots_SteamUser023[27]);
    }
    virtual bool BIsPhoneIdentifying() {
        return steambridge::slot<bool>(kSlots_SteamUser023[28]);
    }
    virtual bool BIsPhoneRequiringVerification() {
        return steambridge::slot<bool>(kSlots_SteamUser023[29]);
    }
    virtual std::uint64_t GetMarketEligibility() {
        return steambridge::slot<std::uint64_t>(kSlots_SteamUser023[30]);
    }
    virtual std::uint64_t GetDurationControl() {
        return steambridge::slot<std::uint64_t>(kSlots_SteamUser023[31]);
    }
    virtual bool BSetDurationControlOnlineState(std::int32_t eNewState) {
        return steambridge::slot<bool>(kSlots_SteamUser023[32], eNewState);
    }
};

Version_SteamUser023 g_SteamUser023;

const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_1[] = {
    "pchName",
    "pData",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_2[] = {
    "pchName",
    "pData",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_3[] = {
    "pchName",
    "nData",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_4[] = {
    "pchName",
    "fData",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_5[] = {
    "pchName",
    "flCountThisSession",
    "dSessionLength",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_6[] = {
    "pchName",
    "pbAchieved",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_7[] = {
    "pchName",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_8[] = {
    "pchName",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_9[] = {
    "pchName",
    "pbAchieved",
    "punUnlockTime",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_11[] = {
    "pchName",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_12[] = {
    "pchName",
    "pchKey",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_13[] = {
    "pchName",
    "nCurProgress",
    "nMaxProgress",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_15[] = {
    "iAchievement",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_16[] = {
    "steamIDUser",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_17[] = {
    "steamIDUser",
    "pchName",
    "pData",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_18[] = {
    "steamIDUser",
    "pchName",
    "pData",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_19[] = {
    "steamIDUser",
    "pchName",
    "pbAchieved",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_20[] = {
    "steamIDUser",
    "pchName",
    "pbAchieved",
    "punUnlockTime",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_21[] = {
    "bAchievementsToo",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_22[] = {
    "pchLeaderboardName",
    "eLeaderboardSortMethod",
    "eLeaderboardDisplayType",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_23[] = {
    "pchLeaderboardName",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_24[] = {
    "hSteamLeaderboard",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_25[] = {
    "hSteamLeaderboard",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_26[] = {
    "hSteamLeaderboard",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_27[] = {
    "hSteamLeaderboard",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_28[] = {
    "hSteamLeaderboard",
    "eLeaderboardDataRequest",
    "nRangeStart",
    "nRangeEnd",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_29[] = {
    "hSteamLeaderboard",
    "prgUsers",
    "cUsers",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_30[] = {
    "hSteamLeaderboardEntries",
    "index",
    "pLeaderboardEntry",
    "pDetails",
    "cDetailsMax",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_31[] = {
    "hSteamLeaderboard",
    "eLeaderboardUploadScoreMethod",
    "nScore",
    "pScoreDetails",
    "cScoreDetailsCount",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_32[] = {
    "hSteamLeaderboard",
    "hUGC",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_35[] = {
    "pchName",
    "unNameBufLen",
    "pflPercent",
    "pbAchieved",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_36[] = {
    "iIteratorPrevious",
    "pchName",
    "unNameBufLen",
    "pflPercent",
    "pbAchieved",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_37[] = {
    "pchName",
    "pflPercent",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_38[] = {
    "nHistoryDays",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_39[] = {
    "pchStatName",
    "pData",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_40[] = {
    "pchStatName",
    "pData",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_41[] = {
    "pchStatName",
    "pData",
    "cubData",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION011_42[] = {
    "pchStatName",
    "pData",
    "cubData",
};
// ISteamUserStats STEAMUSERSTATS_INTERFACE_VERSION011
const steambridge::SlotInfo kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[] = {
    {"SteamAPI_ISteamUserStats_RequestCurrentStats", nullptr},
    {"SteamAPI_ISteamUserStats_GetStatInt32", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_1},
    {"SteamAPI_ISteamUserStats_GetStatFloat", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_2},
    {"SteamAPI_ISteamUserStats_SetStatInt32", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_3},
    {"SteamAPI_ISteamUserStats_SetStatFloat", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_4},
    {"SteamAPI_ISteamUserStats_UpdateAvgRateStat", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_5},
    {"SteamAPI_ISteamUserStats_GetAchievement", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_6},
    {"SteamAPI_ISteamUserStats_SetAchievement", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_7},
    {"SteamAPI_ISteamUserStats_ClearAchievement", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_8},
    {"SteamAPI_ISteamUserStats_GetAchievementAndUnlockTime", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_9},
    {"SteamAPI_ISteamUserStats_StoreStats", nullptr},
    {"SteamAPI_ISteamUserStats_GetAchievementIcon", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_11},
    {"SteamAPI_ISteamUserStats_GetAchievementDisplayAttribute", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_12},
    {"SteamAPI_ISteamUserStats_IndicateAchievementProgress", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_13},
    {"SteamAPI_ISteamUserStats_GetNumAchievements", nullptr},
    {"SteamAPI_ISteamUserStats_GetAchievementName", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_15},
    {"SteamAPI_ISteamUserStats_RequestUserStats", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_16},
    {"SteamAPI_ISteamUserStats_GetUserStatInt32", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_17},
    {"SteamAPI_ISteamUserStats_GetUserStatFloat", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_18},
    {"SteamAPI_ISteamUserStats_GetUserAchievement", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_19},
    {"SteamAPI_ISteamUserStats_GetUserAchievementAndUnlockTime", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_20},
    {"SteamAPI_ISteamUserStats_ResetAllStats", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_21},
    {"SteamAPI_ISteamUserStats_FindOrCreateLeaderboard", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_22},
    {"SteamAPI_ISteamUserStats_FindLeaderboard", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_23},
    {"SteamAPI_ISteamUserStats_GetLeaderboardName", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_24},
    {"SteamAPI_ISteamUserStats_GetLeaderboardEntryCount", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_25},
    {"SteamAPI_ISteamUserStats_GetLeaderboardSortMethod", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_26},
    {"SteamAPI_ISteamUserStats_GetLeaderboardDisplayType", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_27},
    {"SteamAPI_ISteamUserStats_DownloadLeaderboardEntries", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_28},
    {"SteamAPI_ISteamUserStats_DownloadLeaderboardEntriesForUsers", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_29},
    {"SteamAPI_ISteamUserStats_GetDownloadedLeaderboardEntry", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_30},
    {"SteamAPI_ISteamUserStats_UploadLeaderboardScore", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_31},
    {"SteamAPI_ISteamUserStats_AttachLeaderboardUGC", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_32},
    {"SteamAPI_ISteamUserStats_GetNumberOfCurrentPlayers", nullptr},
    {"SteamAPI_ISteamUserStats_RequestGlobalAchievementPercentages", nullptr},
    {"SteamAPI_ISteamUserStats_GetMostAchievedAchievementInfo", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_35},
    {"SteamAPI_ISteamUserStats_GetNextMostAchievedAchievementInfo", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_36},
    {"SteamAPI_ISteamUserStats_GetAchievementAchievedPercent", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_37},
    {"SteamAPI_ISteamUserStats_RequestGlobalStats", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_38},
    {"SteamAPI_ISteamUserStats_GetGlobalStatInt64", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_39},
    {"SteamAPI_ISteamUserStats_GetGlobalStatDouble", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_40},
    {"SteamAPI_ISteamUserStats_GetGlobalStatHistoryInt64", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_41},
    {"SteamAPI_ISteamUserStats_GetGlobalStatHistoryDouble", kParams_STEAMUSERSTATS_INTERFACE_VERSION011_42},
};

class Version_STEAMUSERSTATS_INTERFACE_VERSION011 {
public:
    virtual bool RequestCurrentStats() {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[0]);
    }
    virtual bool GetStat(const char* pchName, std::int32_t* pData) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[1], pchName, pData);
    }
    virtual bool GetStat(const char* pchName, float* pData) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[2], pchName, pData);
    }
    virtual bool SetStat(const char* pchName, std::int32_t nData) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[3], pchName, nData);
    }
    virtual bool SetStat(const char* pchName, float fData) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[4], pchName, fData);
    }
    virtual bool UpdateAvgRateStat(const char* pchName, float flCountThisSession, double dSessionLength) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[5], pchName, flCountThisSession, dSessionLength);
    }
    virtual bool GetAchievement(const char* pchName, bool* pbAchieved) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[6], pchName, pbAchieved);
    }
    virtual bool SetAchievement(const char* pchName) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[7], pchName);
    }
    virtual bool ClearAchievement(const char* pchName) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[8], pchName);
    }
    virtual bool GetAchievementAndUnlockTime(const char* pchName, bool* pbAchieved, std::uint32_t* punUnlockTime) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[9], pchName, pbAchieved, punUnlockTime);
    }
    virtual bool StoreStats() {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[10]);
    }
    virtual std::int32_t GetAchievementIcon(const char* pchName) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[11], pchName);
    }
    virtual const char* GetAchievementDisplayAttribute(const char* pchName, const char* pchKey) {
        return steambridge::slot<const char*>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[12], pchName, pchKey);
    }
    virtual bool IndicateAchievementProgress(const char* pchName, std::uint32_t nCurProgress, std::uint32_t nMaxProgress) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[13], pchName, nCurProgress, nMaxProgress);
    }
    virtual std::uint32_t GetNumAchievements() {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[14]);
    }
    virtual const char* GetAchievementName(std::uint32_t iAchievement) {
        return steambridge::slot<const char*>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[15], iAchievement);
    }
    virtual std::uint64_t RequestUserStats(CSteamID steamIDUser) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[16], steamIDUser);
    }
    virtual bool GetUserStat(CSteamID steamIDUser, const char* pchName, std::int32_t* pData) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[17], steamIDUser, pchName, pData);
    }
    virtual bool GetUserStat(CSteamID steamIDUser, const char* pchName, float* pData) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[18], steamIDUser, pchName, pData);
    }
    virtual bool GetUserAchievement(CSteamID steamIDUser, const char* pchName, bool* pbAchieved) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[19], steamIDUser, pchName, pbAchieved);
    }
    virtual bool GetUserAchievementAndUnlockTime(CSteamID steamIDUser, const char* pchName, bool* pbAchieved, std::uint32_t* punUnlockTime) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[20], steamIDUser, pchName, pbAchieved, punUnlockTime);
    }
    virtual bool ResetAllStats(bool bAchievementsToo) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[21], bAchievementsToo);
    }
    virtual std::uint64_t FindOrCreateLeaderboard(const char* pchLeaderboardName, std::int32_t eLeaderboardSortMethod, std::int32_t eLeaderboardDisplayType) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[22], pchLeaderboardName, eLeaderboardSortMethod, eLeaderboardDisplayType);
    }
    virtual std::uint64_t FindLeaderboard(const char* pchLeaderboardName) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[23], pchLeaderboardName);
    }
    virtual const char* GetLeaderboardName(std::uint64_t hSteamLeaderboard) {
        return steambridge::slot<const char*>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[24], hSteamLeaderboard);
    }
    virtual std::int32_t GetLeaderboardEntryCount(std::uint64_t hSteamLeaderboard) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[25], hSteamLeaderboard);
    }
    virtual std::int32_t GetLeaderboardSortMethod(std::uint64_t hSteamLeaderboard) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[26], hSteamLeaderboard);
    }
    virtual std::int32_t GetLeaderboardDisplayType(std::uint64_t hSteamLeaderboard) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[27], hSteamLeaderboard);
    }
    virtual std::uint64_t DownloadLeaderboardEntries(std::uint64_t hSteamLeaderboard, std::int32_t eLeaderboardDataRequest, std::int32_t nRangeStart, std::int32_t nRangeEnd) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[28], hSteamLeaderboard, eLeaderboardDataRequest, nRangeStart, nRangeEnd);
    }
    virtual std::uint64_t DownloadLeaderboardEntriesForUsers(std::uint64_t hSteamLeaderboard, CSteamID* prgUsers, std::int32_t cUsers) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[29], hSteamLeaderboard, prgUsers, cUsers);
    }
    virtual bool GetDownloadedLeaderboardEntry(std::uint64_t hSteamLeaderboardEntries, std::int32_t index, void* pLeaderboardEntry, std::int32_t* pDetails, std::int32_t cDetailsMax) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[30], hSteamLeaderboardEntries, index, pLeaderboardEntry, pDetails, cDetailsMax);
    }
    virtual std::uint64_t UploadLeaderboardScore(std::uint64_t hSteamLeaderboard, std::int32_t eLeaderboardUploadScoreMethod, std::int32_t nScore, void* pScoreDetails, std::int32_t cScoreDetailsCount) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[31], hSteamLeaderboard, eLeaderboardUploadScoreMethod, nScore, pScoreDetails, cScoreDetailsCount);
    }
    virtual std::uint64_t AttachLeaderboardUGC(std::uint64_t hSteamLeaderboard, std::uint64_t hUGC) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[32], hSteamLeaderboard, hUGC);
    }
    virtual std::uint64_t GetNumberOfCurrentPlayers() {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[33]);
    }
    virtual std::uint64_t RequestGlobalAchievementPercentages() {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[34]);
    }
    virtual std::int32_t GetMostAchievedAchievementInfo(void* pchName, std::uint32_t unNameBufLen, float* pflPercent, bool* pbAchieved) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[35], pchName, unNameBufLen, pflPercent, pbAchieved);
    }
    virtual std::int32_t GetNextMostAchievedAchievementInfo(std::int32_t iIteratorPrevious, void* pchName, std::uint32_t unNameBufLen, float* pflPercent, bool* pbAchieved) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[36], iIteratorPrevious, pchName, unNameBufLen, pflPercent, pbAchieved);
    }
    virtual bool GetAchievementAchievedPercent(const char* pchName, float* pflPercent) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[37], pchName, pflPercent);
    }
    virtual std::uint64_t RequestGlobalStats(std::int32_t nHistoryDays) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[38], nHistoryDays);
    }
    virtual bool GetGlobalStat(const char* pchStatName, std::int64_t* pData) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[39], pchStatName, pData);
    }
    virtual bool GetGlobalStat(const char* pchStatName, double* pData) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[40], pchStatName, pData);
    }
    virtual std::int32_t GetGlobalStatHistory(const char* pchStatName, std::int64_t* pData, std::uint32_t cubData) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[41], pchStatName, pData, cubData);
    }
    virtual std::int32_t GetGlobalStatHistory(const char* pchStatName, double* pData, std::uint32_t cubData) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION011[42], pchStatName, pData, cubData);
    }
};

Version_STEAMUSERSTATS_INTERFACE_VERSION011 g_STEAMUSERSTATS_INTERFACE_VERSION011;

const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_1[] = {
    "pchName",
    "pData",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_2[] = {
    "pchName",
    "pData",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_3[] = {
    "pchName",
    "nData",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_4[] = {
    "pchName",
    "fData",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_5[] = {
    "pchName",
    "flCountThisSession",
    "dSessionLength",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_6[] = {
    "pchName",
    "pbAchieved",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_7[] = {
    "pchName",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_8[] = {
    "pchName",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_9[] = {
    "pchName",
    "pbAchieved",
    "punUnlockTime",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_11[] = {
    "pchName",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_12[] = {
    "pchName",
    "pchKey",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_13[] = {
    "pchName",
    "nCurProgress",
    "nMaxProgress",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_15[] = {
    "iAchievement",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_16[] = {
    "steamIDUser",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_17[] = {
    "steamIDUser",
    "pchName",
    "pData",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_18[] = {
    "steamIDUser",
    "pchName",
    "pData",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_19[] = {
    "steamIDUser",
    "pchName",
    "pbAchieved",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_20[] = {
    "steamIDUser",
    "pchName",
    "pbAchieved",
    "punUnlockTime",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_21[] = {
    "bAchievementsToo",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_22[] = {
    "pchLeaderboardName",
    "eLeaderboardSortMethod",
    "eLeaderboardDisplayType",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_23[] = {
    "pchLeaderboardName",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_24[] = {
    "hSteamLeaderboard",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_25[] = {
    "hSteamLeaderboard",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_26[] = {
    "hSteamLeaderboard",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_27[] = {
    "hSteamLeaderboard",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_28[] = {
    "hSteamLeaderboard",
    "eLeaderboardDataRequest",
    "nRangeStart",
    "nRangeEnd",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_29[] = {
    "hSteamLeaderboard",
    "prgUsers",
    "cUsers",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_30[] = {
    "hSteamLeaderboardEntries",
    "index",
    "pLeaderboardEntry",
    "pDetails",
    "cDetailsMax",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_31[] = {
    "hSteamLeaderboard",
    "eLeaderboardUploadScoreMethod",
    "nScore",
    "pScoreDetails",
    "cScoreDetailsCount",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_32[] = {
    "hSteamLeaderboard",
    "hUGC",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_35[] = {
    "pchName",
    "unNameBufLen",
    "pflPercent",
    "pbAchieved",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_36[] = {
    "iIteratorPrevious",
    "pchName",
    "unNameBufLen",
    "pflPercent",
    "pbAchieved",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_37[] = {
    "pchName",
    "pflPercent",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_38[] = {
    "nHistoryDays",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_39[] = {
    "pchStatName",
    "pData",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_40[] = {
    "pchStatName",
    "pData",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_41[] = {
    "pchStatName",
    "pData",
    "cubData",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_42[] = {
    "pchStatName",
    "pData",
    "cubData",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_43[] = {
    "pchName",
    "pnMinProgress",
    "pnMaxProgress",
};
const char* const kParams_STEAMUSERSTATS_INTERFACE_VERSION012_44[] = {
    "pchName",
    "pfMinProgress",
    "pfMaxProgress",
};
// ISteamUserStats STEAMUSERSTATS_INTERFACE_VERSION012
const steambridge::SlotInfo kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[] = {
    {"SteamAPI_ISteamUserStats_RequestCurrentStats", nullptr},
    {"SteamAPI_ISteamUserStats_GetStatInt32", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_1},
    {"SteamAPI_ISteamUserStats_GetStatFloat", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_2},
    {"SteamAPI_ISteamUserStats_SetStatInt32", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_3},
    {"SteamAPI_ISteamUserStats_SetStatFloat", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_4},
    {"SteamAPI_ISteamUserStats_UpdateAvgRateStat", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_5},
    {"SteamAPI_ISteamUserStats_GetAchievement", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_6},
    {"SteamAPI_ISteamUserStats_SetAchievement", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_7},
    {"SteamAPI_ISteamUserStats_ClearAchievement", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_8},
    {"SteamAPI_ISteamUserStats_GetAchievementAndUnlockTime", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_9},
    {"SteamAPI_ISteamUserStats_StoreStats", nullptr},
    {"SteamAPI_ISteamUserStats_GetAchievementIcon", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_11},
    {"SteamAPI_ISteamUserStats_GetAchievementDisplayAttribute", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_12},
    {"SteamAPI_ISteamUserStats_IndicateAchievementProgress", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_13},
    {"SteamAPI_ISteamUserStats_GetNumAchievements", nullptr},
    {"SteamAPI_ISteamUserStats_GetAchievementName", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_15},
    {"SteamAPI_ISteamUserStats_RequestUserStats", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_16},
    {"SteamAPI_ISteamUserStats_GetUserStatInt32", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_17},
    {"SteamAPI_ISteamUserStats_GetUserStatFloat", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_18},
    {"SteamAPI_ISteamUserStats_GetUserAchievement", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_19},
    {"SteamAPI_ISteamUserStats_GetUserAchievementAndUnlockTime", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_20},
    {"SteamAPI_ISteamUserStats_ResetAllStats", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_21},
    {"SteamAPI_ISteamUserStats_FindOrCreateLeaderboard", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_22},
    {"SteamAPI_ISteamUserStats_FindLeaderboard", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_23},
    {"SteamAPI_ISteamUserStats_GetLeaderboardName", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_24},
    {"SteamAPI_ISteamUserStats_GetLeaderboardEntryCount", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_25},
    {"SteamAPI_ISteamUserStats_GetLeaderboardSortMethod", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_26},
    {"SteamAPI_ISteamUserStats_GetLeaderboardDisplayType", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_27},
    {"SteamAPI_ISteamUserStats_DownloadLeaderboardEntries", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_28},
    {"SteamAPI_ISteamUserStats_DownloadLeaderboardEntriesForUsers", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_29},
    {"SteamAPI_ISteamUserStats_GetDownloadedLeaderboardEntry", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_30},
    {"SteamAPI_ISteamUserStats_UploadLeaderboardScore", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_31},
    {"SteamAPI_ISteamUserStats_AttachLeaderboardUGC", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_32},
    {"SteamAPI_ISteamUserStats_GetNumberOfCurrentPlayers", nullptr},
    {"SteamAPI_ISteamUserStats_RequestGlobalAchievementPercentages", nullptr},
    {"SteamAPI_ISteamUserStats_GetMostAchievedAchievementInfo", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_35},
    {"SteamAPI_ISteamUserStats_GetNextMostAchievedAchievementInfo", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_36},
    {"SteamAPI_ISteamUserStats_GetAchievementAchievedPercent", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_37},
    {"SteamAPI_ISteamUserStats_RequestGlobalStats", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_38},
    {"SteamAPI_ISteamUserStats_GetGlobalStatInt64", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_39},
    {"SteamAPI_ISteamUserStats_GetGlobalStatDouble", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_40},
    {"SteamAPI_ISteamUserStats_GetGlobalStatHistoryInt64", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_41},
    {"SteamAPI_ISteamUserStats_GetGlobalStatHistoryDouble", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_42},
    {"SteamAPI_ISteamUserStats_GetAchievementProgressLimitsInt32", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_43},
    {"SteamAPI_ISteamUserStats_GetAchievementProgressLimitsFloat", kParams_STEAMUSERSTATS_INTERFACE_VERSION012_44},
};

class Version_STEAMUSERSTATS_INTERFACE_VERSION012 {
public:
    virtual bool RequestCurrentStats() {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[0]);
    }
    virtual bool GetStat(const char* pchName, std::int32_t* pData) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[1], pchName, pData);
    }
    virtual bool GetStat(const char* pchName, float* pData) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[2], pchName, pData);
    }
    virtual bool SetStat(const char* pchName, std::int32_t nData) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[3], pchName, nData);
    }
    virtual bool SetStat(const char* pchName, float fData) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[4], pchName, fData);
    }
    virtual bool UpdateAvgRateStat(const char* pchName, float flCountThisSession, double dSessionLength) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[5], pchName, flCountThisSession, dSessionLength);
    }
    virtual bool GetAchievement(const char* pchName, bool* pbAchieved) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[6], pchName, pbAchieved);
    }
    virtual bool SetAchievement(const char* pchName) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[7], pchName);
    }
    virtual bool ClearAchievement(const char* pchName) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[8], pchName);
    }
    virtual bool GetAchievementAndUnlockTime(const char* pchName, bool* pbAchieved, std::uint32_t* punUnlockTime) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[9], pchName, pbAchieved, punUnlockTime);
    }
    virtual bool StoreStats() {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[10]);
    }
    virtual std::int32_t GetAchievementIcon(const char* pchName) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[11], pchName);
    }
    virtual const char* GetAchievementDisplayAttribute(const char* pchName, const char* pchKey) {
        return steambridge::slot<const char*>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[12], pchName, pchKey);
    }
    virtual bool IndicateAchievementProgress(const char* pchName, std::uint32_t nCurProgress, std::uint32_t nMaxProgress) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[13], pchName, nCurProgress, nMaxProgress);
    }
    virtual std::uint32_t GetNumAchievements() {
        return steambridge::slot<std::uint32_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[14]);
    }
    virtual const char* GetAchievementName(std::uint32_t iAchievement) {
        return steambridge::slot<const char*>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[15], iAchievement);
    }
    virtual std::uint64_t RequestUserStats(CSteamID steamIDUser) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[16], steamIDUser);
    }
    virtual bool GetUserStat(CSteamID steamIDUser, const char* pchName, std::int32_t* pData) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[17], steamIDUser, pchName, pData);
    }
    virtual bool GetUserStat(CSteamID steamIDUser, const char* pchName, float* pData) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[18], steamIDUser, pchName, pData);
    }
    virtual bool GetUserAchievement(CSteamID steamIDUser, const char* pchName, bool* pbAchieved) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[19], steamIDUser, pchName, pbAchieved);
    }
    virtual bool GetUserAchievementAndUnlockTime(CSteamID steamIDUser, const char* pchName, bool* pbAchieved, std::uint32_t* punUnlockTime) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[20], steamIDUser, pchName, pbAchieved, punUnlockTime);
    }
    virtual bool ResetAllStats(bool bAchievementsToo) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[21], bAchievementsToo);
    }
    virtual std::uint64_t FindOrCreateLeaderboard(const char* pchLeaderboardName, std::int32_t eLeaderboardSortMethod, std::int32_t eLeaderboardDisplayType) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[22], pchLeaderboardName, eLeaderboardSortMethod, eLeaderboardDisplayType);
    }
    virtual std::uint64_t FindLeaderboard(const char* pchLeaderboardName) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[23], pchLeaderboardName);
    }
    virtual const char* GetLeaderboardName(std::uint64_t hSteamLeaderboard) {
        return steambridge::slot<const char*>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[24], hSteamLeaderboard);
    }
    virtual std::int32_t GetLeaderboardEntryCount(std::uint64_t hSteamLeaderboard) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[25], hSteamLeaderboard);
    }
    virtual std::int32_t GetLeaderboardSortMethod(std::uint64_t hSteamLeaderboard) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[26], hSteamLeaderboard);
    }
    virtual std::int32_t GetLeaderboardDisplayType(std::uint64_t hSteamLeaderboard) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[27], hSteamLeaderboard);
    }
    virtual std::uint64_t DownloadLeaderboardEntries(std::uint64_t hSteamLeaderboard, std::int32_t eLeaderboardDataRequest, std::int32_t nRangeStart, std::int32_t nRangeEnd) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[28], hSteamLeaderboard, eLeaderboardDataRequest, nRangeStart, nRangeEnd);
    }
    virtual std::uint64_t DownloadLeaderboardEntriesForUsers(std::uint64_t hSteamLeaderboard, CSteamID* prgUsers, std::int32_t cUsers) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[29], hSteamLeaderboard, prgUsers, cUsers);
    }
    virtual bool GetDownloadedLeaderboardEntry(std::uint64_t hSteamLeaderboardEntries, std::int32_t index, void* pLeaderboardEntry, std::int32_t* pDetails, std::int32_t cDetailsMax) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[30], hSteamLeaderboardEntries, index, pLeaderboardEntry, pDetails, cDetailsMax);
    }
    virtual std::uint64_t UploadLeaderboardScore(std::uint64_t hSteamLeaderboard, std::int32_t eLeaderboardUploadScoreMethod, std::int32_t nScore, void* pScoreDetails, std::int32_t cScoreDetailsCount) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[31], hSteamLeaderboard, eLeaderboardUploadScoreMethod, nScore, pScoreDetails, cScoreDetailsCount);
    }
    virtual std::uint64_t AttachLeaderboardUGC(std::uint64_t hSteamLeaderboard, std::uint64_t hUGC) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[32], hSteamLeaderboard, hUGC);
    }
    virtual std::uint64_t GetNumberOfCurrentPlayers() {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[33]);
    }
    virtual std::uint64_t RequestGlobalAchievementPercentages() {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[34]);
    }
    virtual std::int32_t GetMostAchievedAchievementInfo(void* pchName, std::uint32_t unNameBufLen, float* pflPercent, bool* pbAchieved) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[35], pchName, unNameBufLen, pflPercent, pbAchieved);
    }
    virtual std::int32_t GetNextMostAchievedAchievementInfo(std::int32_t iIteratorPrevious, void* pchName, std::uint32_t unNameBufLen, float* pflPercent, bool* pbAchieved) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[36], iIteratorPrevious, pchName, unNameBufLen, pflPercent, pbAchieved);
    }
    virtual bool GetAchievementAchievedPercent(const char* pchName, float* pflPercent) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[37], pchName, pflPercent);
    }
    virtual std::uint64_t RequestGlobalStats(std::int32_t nHistoryDays) {
        return steambridge::slot<std::uint64_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[38], nHistoryDays);
    }
    virtual bool GetGlobalStat(const char* pchStatName, std::int64_t* pData) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[39], pchStatName, pData);
    }
    virtual bool GetGlobalStat(const char* pchStatName, double* pData) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[40], pchStatName, pData);
    }
    virtual std::int32_t GetGlobalStatHistory(const char* pchStatName, std::int64_t* pData, std::uint32_t cubData) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[41], pchStatName, pData, cubData);
    }
    virtual std::int32_t GetGlobalStatHistory(const char* pchStatName, double* pData, std::uint32_t cubData) {
        return steambridge::slot<std::int32_t>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[42], pchStatName, pData, cubData);
    }
    virtual bool GetAchievementProgressLimits(const char* pchName, std::int32_t* pnMinProgress, std::int32_t* pnMaxProgress) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[43], pchName, pnMinProgress, pnMaxProgress);
    }
    virtual bool GetAchievementProgressLimits(const char* pchName, float* pfMinProgress, float* pfMaxProgress) {
        return steambridge::slot<bool>(kSlots_STEAMUSERSTATS_INTERFACE_VERSION012[44], pchName, pfMinProgress, pfMaxProgress);
    }
};

Version_STEAMUSERSTATS_INTERFACE_VERSION012 g_STEAMUSERSTATS_INTERFACE_VERSION012;

const char* const kParams_SteamUtils009_5[] = {
    "iImage",
    "pnWidth",
    "pnHeight",
};
const char* const kParams_SteamUtils009_6[] = {
    "iImage",
    "pubDest",
    "nDestBufferSize",
};
const char* const kParams_SteamUtils009_7[] = {
    "unIP",
    "usPort",
};
const char* const kParams_SteamUtils009_10[] = {
    "eNotificationPosition",
};
const char* const kParams_SteamUtils009_11[] = {
    "hSteamAPICall",
    "pbFailed",
};
const char* const kParams_SteamUtils009_12[] = {
    "hSteamAPICall",
};
const char* const kParams_SteamUtils009_13[] = {
    "hSteamAPICall",
    "pCallback",
    "cubCallback",
    "iCallbackExpected",
    "pbFailed",
};
const char* const kParams_SteamUtils009_16[] = {
    "pFunction",
};
const char* const kParams_SteamUtils009_19[] = {
    "szFileName",
};
const char* const kParams_SteamUtils009_20[] = {
    "eInputMode",
    "eLineInputMode",
    "pchDescription",
    "unCharMax",
    "pchExistingText",
};
const char* const kParams_SteamUtils009_22[] = {
    "pchText",
    "cchText",
};
const char* const kParams_SteamUtils009_25[] = {
    "nHorizontalInset",
    "nVerticalInset",
};
const char* const kParams_SteamUtils009_29[] = {
    "bEnabled",
};
const char* const kParams_SteamUtils009_32[] = {
    "pchOutFilteredText",
    "nByteSizeOutFilteredText",
    "pchInputMessage",
    "bLegalOnly",
};
const char* const kParams_SteamUtils009_33[] = {
    "eProtocol",
};
// ISteamUtils SteamUtils009
const steambridge::SlotInfo kSlots_SteamUtils009[] = {
    {"SteamAPI_ISteamUtils_GetSecondsSinceAppActive", nullptr},
    {"SteamAPI_ISteamUtils_GetSecondsSinceComputerActive", nullptr},
    {"SteamAPI_ISteamUtils_GetConnectedUniverse", nullptr},
    {"SteamAPI_ISteamUtils_GetServerRealTime", nullptr},
    {"SteamAPI_ISteamUtils_GetIPCountry", nullptr},
    {"SteamAPI_ISteamUtils_GetImageSize", kParams_SteamUtils009_5},
    {"SteamAPI_ISteamUtils_GetImageRGBA", kParams_SteamUtils009_6},
    {"SteamAPI_ISteamUtils_GetCSERIPPort", kParams_SteamUtils009_7},
    {"SteamAPI_ISteamUtils_GetCurrentBatteryPower", nullptr},
    {"SteamAPI_ISteamUtils_GetAppID", nullptr},
    {"SteamAPI_ISteamUtils_SetOverlayNotificationPosition", kParams_SteamUtils009_10},
    {"SteamAPI_ISteamUtils_IsAPICallCompleted", kParams_SteamUtils009_11},
    {"SteamAPI_ISteamUtils_GetAPICallFailureReason", kParams_SteamUtils009_12},
    {"SteamAPI_ISteamUtils_GetAPICallResult", kParams_SteamUtils009_13},
    {"ISteamUtils::RunFrame", nullptr},
    {"SteamAPI_ISteamUtils_GetIPCCallCount", nullptr},
    {"SteamAPI_ISteamUtils_SetWarningMessageHook", kParams_SteamUtils009_16},
    {"SteamAPI_ISteamUtils_IsOverlayEnabled", nullptr},
    {"SteamAPI_ISteamUtils_BOverlayNeedsPresent", nullptr},
    {"SteamAPI_ISteamUtils_CheckFileSignature", kParams_SteamUtils009_19},
    {"SteamAPI_ISteamUtils_ShowGamepadTextInput", kParams_SteamUtils009_20},
    {"SteamAPI_ISteamUtils_GetEnteredGamepadTextLength", nullptr},
    {"SteamAPI_ISteamUtils_GetEnteredGamepadTextInput", kParams_SteamUtils009_22},
    {"SteamAPI_ISteamUtils_GetSteamUILanguage", nullptr},
    {"SteamAPI_ISteamUtils_IsSteamRunningInVR", nullptr},
    {"SteamAPI_ISteamUtils_SetOverlayNotificationInset", kParams_SteamUtils009_25},
    {"SteamAPI_ISteamUtils_IsSteamInBigPictureMode", nullptr},
    {"SteamAPI_ISteamUtils_StartVRDashboard", nullptr},
    {"SteamAPI_ISteamUtils_IsVRHeadsetStreamingEnabled", nullptr},
    {"SteamAPI_ISteamUtils_SetVRHeadsetStreamingEnabled", kParams_SteamUtils009_29},
    {"SteamAPI_ISteamUtils_IsSteamChinaLauncher", nullptr},
    {"SteamAPI_ISteamUtils_InitFilterText", nullptr},
    {"SteamAPI_ISteamUtils_FilterText", kParams_SteamUtils009_32},
    {"SteamAPI_ISteamUtils_GetIPv6ConnectivityState", kParams_SteamUtils009_33},
};

class Version_SteamUtils009 {
public:
    virtual std::uint32_t GetSecondsSinceAppActive() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamUtils009[0]);
    }
    virtual std::uint32_t GetSecondsSinceComputerActive() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamUtils009[1]);
    }
    virtual std::int32_t GetConnectedUniverse() {
        return steambridge::slot<std::int32_t>(kSlots_SteamUtils009[2]);
    }
    virtual std::uint32_t GetServerRealTime() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamUtils009[3]);
    }
    virtual const char* GetIPCountry() {
        return steambridge::slot<const char*>(kSlots_SteamUtils009[4]);
    }
    virtual bool GetImageSize(std::int32_t iImage, std::uint32_t* pnWidth, std::uint32_t* pnHeight) {
        return steambridge::slot<bool>(kSlots_SteamUtils009[5], iImage, pnWidth, pnHeight);
    }
    virtual bool GetImageRGBA(std::int32_t iImage, std::uint8_t* pubDest, std::int32_t nDestBufferSize) {
        return steambridge::slot<bool>(kSlots_SteamUtils009[6], iImage, pubDest, nDestBufferSize);
    }
    virtual bool GetCSERIPPort(std::uint32_t* unIP, std::uint16_t* usPort) {
        return steambridge::slot<bool>(kSlots_SteamUtils009[7], unIP, usPort);
    }
    virtual std::uint8_t GetCurrentBatteryPower() {
        return steambridge::slot<std::uint8_t>(kSlots_SteamUtils009[8]);
    }
    virtual std::uint32_t GetAppID() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamUtils009[9]);
    }
    virtual void SetOverlayNotificationPosition(std::int32_t eNotificationPosition) {
        steambridge::slot<void>(kSlots_SteamUtils009[10], eNotificationPosition);
    }
    virtual bool IsAPICallCompleted(std::uint64_t hSteamAPICall, bool* pbFailed) {
        return steambridge::slot<bool>(kSlots_SteamUtils009[11], hSteamAPICall, pbFailed);
    }
    virtual std::int32_t GetAPICallFailureReason(std::uint64_t hSteamAPICall) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUtils009[12], hSteamAPICall);
    }
    virtual bool GetAPICallResult(std::uint64_t hSteamAPICall, void* pCallback, std::int32_t cubCallback, std::int32_t iCallbackExpected, bool* pbFailed) {
        return steambridge::slot<bool>(kSlots_SteamUtils009[13], hSteamAPICall, pCallback, cubCallback, iCallbackExpected, pbFailed);
    }
    virtual void RunFrame() {
        steambridge::slot<void>(kSlots_SteamUtils009[14]);
    }
    virtual std::uint32_t GetIPCCallCount() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamUtils009[15]);
    }
    virtual void SetWarningMessageHook(void* pFunction) {
        steambridge::slot<void>(kSlots_SteamUtils009[16], pFunction);
    }
    virtual bool IsOverlayEnabled() {
        return steambridge::slot<bool>(kSlots_SteamUtils009[17]);
    }
    virtual bool BOverlayNeedsPresent() {
        return steambridge::slot<bool>(kSlots_SteamUtils009[18]);
    }
    virtual std::uint64_t CheckFileSignature(const char* szFileName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamUtils009[19], szFileName);
    }
    virtual bool ShowGamepadTextInput(std::int32_t eInputMode, std::int32_t eLineInputMode, const char* pchDescription, std::uint32_t unCharMax, const char* pchExistingText) {
        return steambridge::slot<bool>(kSlots_SteamUtils009[20], eInputMode, eLineInputMode, pchDescription, unCharMax, pchExistingText);
    }
    virtual std::uint32_t GetEnteredGamepadTextLength() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamUtils009[21]);
    }
    virtual bool GetEnteredGamepadTextInput(void* pchText, std::uint32_t cchText) {
        return steambridge::slot<bool>(kSlots_SteamUtils009[22], pchText, cchText);
    }
    virtual const char* GetSteamUILanguage() {
        return steambridge::slot<const char*>(kSlots_SteamUtils009[23]);
    }
    virtual bool IsSteamRunningInVR() {
        return steambridge::slot<bool>(kSlots_SteamUtils009[24]);
    }
    virtual void SetOverlayNotificationInset(std::int32_t nHorizontalInset, std::int32_t nVerticalInset) {
        steambridge::slot<void>(kSlots_SteamUtils009[25], nHorizontalInset, nVerticalInset);
    }
    virtual bool IsSteamInBigPictureMode() {
        return steambridge::slot<bool>(kSlots_SteamUtils009[26]);
    }
    virtual void StartVRDashboard() {
        steambridge::slot<void>(kSlots_SteamUtils009[27]);
    }
    virtual bool IsVRHeadsetStreamingEnabled() {
        return steambridge::slot<bool>(kSlots_SteamUtils009[28]);
    }
    virtual void SetVRHeadsetStreamingEnabled(bool bEnabled) {
        steambridge::slot<void>(kSlots_SteamUtils009[29], bEnabled);
    }
    virtual bool IsSteamChinaLauncher() {
        return steambridge::slot<bool>(kSlots_SteamUtils009[30]);
    }
    virtual bool InitFilterText() {
        return steambridge::slot<bool>(kSlots_SteamUtils009[31]);
    }
    virtual std::int32_t FilterText(void* pchOutFilteredText, std::uint32_t nByteSizeOutFilteredText, const char* pchInputMessage, bool bLegalOnly) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUtils009[32], pchOutFilteredText, nByteSizeOutFilteredText, pchInputMessage, bLegalOnly);
    }
    virtual std::int32_t GetIPv6ConnectivityState(std::int32_t eProtocol) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUtils009[33], eProtocol);
    }
};

Version_SteamUtils009 g_SteamUtils009;

const char* const kParams_SteamUtils010_5[] = {
    "iImage",
    "pnWidth",
    "pnHeight",
};
const char* const kParams_SteamUtils010_6[] = {
    "iImage",
    "pubDest",
    "nDestBufferSize",
};
const char* const kParams_SteamUtils010_7[] = {
    "unIP",
    "usPort",
};
const char* const kParams_SteamUtils010_10[] = {
    "eNotificationPosition",
};
const char* const kParams_SteamUtils010_11[] = {
    "hSteamAPICall",
    "pbFailed",
};
const char* const kParams_SteamUtils010_12[] = {
    "hSteamAPICall",
};
const char* const kParams_SteamUtils010_13[] = {
    "hSteamAPICall",
    "pCallback",
    "cubCallback",
    "iCallbackExpected",
    "pbFailed",
};
const char* const kParams_SteamUtils010_16[] = {
    "pFunction",
};
const char* const kParams_SteamUtils010_19[] = {
    "szFileName",
};
const char* const kParams_SteamUtils010_20[] = {
    "eInputMode",
    "eLineInputMode",
    "pchDescription",
    "unCharMax",
    "pchExistingText",
};
const char* const kParams_SteamUtils010_22[] = {
    "pchText",
    "cchText",
};
const char* const kParams_SteamUtils010_25[] = {
    "nHorizontalInset",
    "nVerticalInset",
};
const char* const kParams_SteamUtils010_29[] = {
    "bEnabled",
};
const char* const kParams_SteamUtils010_31[] = {
    "unFilterOptions",
};
const char* const kParams_SteamUtils010_32[] = {
    "eContext",
    "sourceSteamID",
    "pchInputMessage",
    "pchOutFilteredText",
    "nByteSizeOutFilteredText",
};
const char* const kParams_SteamUtils010_33[] = {
    "eProtocol",
};
const char* const kParams_SteamUtils010_35[] = {
    "eKeyboardMode",
    "nTextFieldXPosition",
    "nTextFieldYPosition",
    "nTextFieldWidth",
    "nTextFieldHeight",
};
const char* const kParams_SteamUtils010_36[] = {
    "bLauncherMode",
};
// ISteamUtils SteamUtils010
const steambridge::SlotInfo kSlots_SteamUtils010[] = {
    {"SteamAPI_ISteamUtils_GetSecondsSinceAppActive", nullptr},
    {"SteamAPI_ISteamUtils_GetSecondsSinceComputerActive", nullptr},
    {"SteamAPI_ISteamUtils_GetConnectedUniverse", nullptr},
    {"SteamAPI_ISteamUtils_GetServerRealTime", nullptr},
    {"SteamAPI_ISteamUtils_GetIPCountry", nullptr},
    {"SteamAPI_ISteamUtils_GetImageSize", kParams_SteamUtils010_5},
    {"SteamAPI_ISteamUtils_GetImageRGBA", kParams_SteamUtils010_6},
    {"ISteamUtils::GetCSERIPPort", kParams_SteamUtils010_7},
    {"SteamAPI_ISteamUtils_GetCurrentBatteryPower", nullptr},
    {"SteamAPI_ISteamUtils_GetAppID", nullptr},
    {"SteamAPI_ISteamUtils_SetOverlayNotificationPosition", kParams_SteamUtils010_10},
    {"SteamAPI_ISteamUtils_IsAPICallCompleted", kParams_SteamUtils010_11},
    {"SteamAPI_ISteamUtils_GetAPICallFailureReason", kParams_SteamUtils010_12},
    {"SteamAPI_ISteamUtils_GetAPICallResult", kParams_SteamUtils010_13},
    {"ISteamUtils::RunFrame", nullptr},
    {"SteamAPI_ISteamUtils_GetIPCCallCount", nullptr},
    {"SteamAPI_ISteamUtils_SetWarningMessageHook", kParams_SteamUtils010_16},
    {"SteamAPI_ISteamUtils_IsOverlayEnabled", nullptr},
    {"SteamAPI_ISteamUtils_BOverlayNeedsPresent", nullptr},
    {"SteamAPI_ISteamUtils_CheckFileSignature", kParams_SteamUtils010_19},
    {"SteamAPI_ISteamUtils_ShowGamepadTextInput", kParams_SteamUtils010_20},
    {"SteamAPI_ISteamUtils_GetEnteredGamepadTextLength", nullptr},
    {"SteamAPI_ISteamUtils_GetEnteredGamepadTextInput", kParams_SteamUtils010_22},
    {"SteamAPI_ISteamUtils_GetSteamUILanguage", nullptr},
    {"SteamAPI_ISteamUtils_IsSteamRunningInVR", nullptr},
    {"SteamAPI_ISteamUtils_SetOverlayNotificationInset", kParams_SteamUtils010_25},
    {"SteamAPI_ISteamUtils_IsSteamInBigPictureMode", nullptr},
    {"SteamAPI_ISteamUtils_StartVRDashboard", nullptr},
    {"SteamAPI_ISteamUtils_IsVRHeadsetStreamingEnabled", nullptr},
    {"SteamAPI_ISteamUtils_SetVRHeadsetStreamingEnabled", kParams_SteamUtils010_29},
    {"SteamAPI_ISteamUtils_IsSteamChinaLauncher", nullptr},
    {"SteamAPI_ISteamUtils_InitFilterText", kParams_SteamUtils010_31},
    {"SteamAPI_ISteamUtils_FilterText", kParams_SteamUtils010_32},
    {"SteamAPI_ISteamUtils_GetIPv6ConnectivityState", kParams_SteamUtils010_33},
    {"SteamAPI_ISteamUtils_IsSteamRunningOnSteamDeck", nullptr},
    {"SteamAPI_ISteamUtils_ShowFloatingGamepadTextInput", kParams_SteamUtils010_35},
    {"SteamAPI_ISteamUtils_SetGameLauncherMode", kParams_SteamUtils010_36},
    {"SteamAPI_ISteamUtils_DismissFloatingGamepadTextInput", nullptr},
};

class Version_SteamUtils010 {
public:
    virtual std::uint32_t GetSecondsSinceAppActive() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamUtils010[0]);
    }
    virtual std::uint32_t GetSecondsSinceComputerActive() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamUtils010[1]);
    }
    virtual std::int32_t GetConnectedUniverse() {
        return steambridge::slot<std::int32_t>(kSlots_SteamUtils010[2]);
    }
    virtual std::uint32_t GetServerRealTime() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamUtils010[3]);
    }
    virtual const char* GetIPCountry() {
        return steambridge::slot<const char*>(kSlots_SteamUtils010[4]);
    }
    virtual bool GetImageSize(std::int32_t iImage, std::uint32_t* pnWidth, std::uint32_t* pnHeight) {
        return steambridge::slot<bool>(kSlots_SteamUtils010[5], iImage, pnWidth, pnHeight);
    }
    virtual bool GetImageRGBA(std::int32_t iImage, std::uint8_t* pubDest, std::int32_t nDestBufferSize) {
        return steambridge::slot<bool>(kSlots_SteamUtils010[6], iImage, pubDest, nDestBufferSize);
    }
    virtual bool GetCSERIPPort(std::uint32_t* unIP, std::uint16_t* usPort) {
        return steambridge::slot<bool>(kSlots_SteamUtils010[7], unIP, usPort);
    }
    virtual std::uint8_t GetCurrentBatteryPower() {
        return steambridge::slot<std::uint8_t>(kSlots_SteamUtils010[8]);
    }
    virtual std::uint32_t GetAppID() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamUtils010[9]);
    }
    virtual void SetOverlayNotificationPosition(std::int32_t eNotificationPosition) {
        steambridge::slot<void>(kSlots_SteamUtils010[10], eNotificationPosition);
    }
    virtual bool IsAPICallCompleted(std::uint64_t hSteamAPICall, bool* pbFailed) {
        return steambridge::slot<bool>(kSlots_SteamUtils010[11], hSteamAPICall, pbFailed);
    }
    virtual std::int32_t GetAPICallFailureReason(std::uint64_t hSteamAPICall) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUtils010[12], hSteamAPICall);
    }
    virtual bool GetAPICallResult(std::uint64_t hSteamAPICall, void* pCallback, std::int32_t cubCallback, std::int32_t iCallbackExpected, bool* pbFailed) {
        return steambridge::slot<bool>(kSlots_SteamUtils010[13], hSteamAPICall, pCallback, cubCallback, iCallbackExpected, pbFailed);
    }
    virtual void RunFrame() {
        steambridge::slot<void>(kSlots_SteamUtils010[14]);
    }
    virtual std::uint32_t GetIPCCallCount() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamUtils010[15]);
    }
    virtual void SetWarningMessageHook(void* pFunction) {
        steambridge::slot<void>(kSlots_SteamUtils010[16], pFunction);
    }
    virtual bool IsOverlayEnabled() {
        return steambridge::slot<bool>(kSlots_SteamUtils010[17]);
    }
    virtual bool BOverlayNeedsPresent() {
        return steambridge::slot<bool>(kSlots_SteamUtils010[18]);
    }
    virtual std::uint64_t CheckFileSignature(const char* szFileName) {
        return steambridge::slot<std::uint64_t>(kSlots_SteamUtils010[19], szFileName);
    }
    virtual bool ShowGamepadTextInput(std::int32_t eInputMode, std::int32_t eLineInputMode, const char* pchDescription, std::uint32_t unCharMax, const char* pchExistingText) {
        return steambridge::slot<bool>(kSlots_SteamUtils010[20], eInputMode, eLineInputMode, pchDescription, unCharMax, pchExistingText);
    }
    virtual std::uint32_t GetEnteredGamepadTextLength() {
        return steambridge::slot<std::uint32_t>(kSlots_SteamUtils010[21]);
    }
    virtual bool GetEnteredGamepadTextInput(void* pchText, std::uint32_t cchText) {
        return steambridge::slot<bool>(kSlots_SteamUtils010[22], pchText, cchText);
    }
    virtual const char* GetSteamUILanguage() {
        return steambridge::slot<const char*>(kSlots_SteamUtils010[23]);
    }
    virtual bool IsSteamRunningInVR() {
        return steambridge::slot<bool>(kSlots_SteamUtils010[24]);
    }
    virtual void SetOverlayNotificationInset(std::int32_t nHorizontalInset, std::int32_t nVerticalInset) {
        steambridge::slot<void>(kSlots_SteamUtils010[25], nHorizontalInset, nVerticalInset);
    }
    virtual bool IsSteamInBigPictureMode() {
        return steambridge::slot<bool>(kSlots_SteamUtils010[26]);
    }
    virtual void StartVRDashboard() {
        steambridge::slot<void>(kSlots_SteamUtils010[27]);
    }
    virtual bool IsVRHeadsetStreamingEnabled() {
        return steambridge::slot<bool>(kSlots_SteamUtils010[28]);
    }
    virtual void SetVRHeadsetStreamingEnabled(bool bEnabled) {
        steambridge::slot<void>(kSlots_SteamUtils010[29], bEnabled);
    }
    virtual bool IsSteamChinaLauncher() {
        return steambridge::slot<bool>(kSlots_SteamUtils010[30]);
    }
    virtual bool InitFilterText(std::uint32_t unFilterOptions) {
        return steambridge::slot<bool>(kSlots_SteamUtils010[31], unFilterOptions);
    }
    virtual std::int32_t FilterText(std::int32_t eContext, CSteamID sourceSteamID, const char* pchInputMessage, void* pchOutFilteredText, std::uint32_t nByteSizeOutFilteredText) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUtils010[32], eContext, sourceSteamID, pchInputMessage, pchOutFilteredText, nByteSizeOutFilteredText);
    }
    virtual std::int32_t GetIPv6ConnectivityState(std::int32_t eProtocol) {
        return steambridge::slot<std::int32_t>(kSlots_SteamUtils010[33], eProtocol);
    }
    virtual bool IsSteamRunningOnSteamDeck() {
        return steambridge::slot<bool>(kSlots_SteamUtils010[34]);
    }
    virtual bool ShowFloatingGamepadTextInput(std::int32_t eKeyboardMode, std::int32_t nTextFieldXPosition, std::int32_t nTextFieldYPosition, std::int32_t nTextFieldWidth, std::int32_t nTextFieldHeight) {
        return steambridge::slot<bool>(kSlots_SteamUtils010[35], eKeyboardMode, nTextFieldXPosition, nTextFieldYPosition, nTextFieldWidth, nTextFieldHeight);
    }
    virtual void SetGameLauncherMode(bool bLauncherMode) {
        steambridge::slot<void>(kSlots_SteamUtils010[36], bLauncherMode);
    }
    virtual bool DismissFloatingGamepadTextInput() {
        return steambridge::slot<bool>(kSlots_SteamUtils010[37]);
    }
};

Version_SteamUtils010 g_SteamUtils010;

const char* const kParams_STEAMVIDEO_INTERFACE_V002_0[] = {
    "unVideoAppID",
};
const char* const kParams_STEAMVIDEO_INTERFACE_V002_1[] = {
    "pnNumViewers",
};
const char* const kParams_STEAMVIDEO_INTERFACE_V002_2[] = {
    "unVideoAppID",
};
const char* const kParams_STEAMVIDEO_INTERFACE_V002_3[] = {
    "unVideoAppID",
    "pchBuffer",
    "pnBufferSize",
};
// ISteamVideo STEAMVIDEO_INTERFACE_V002
const steambridge::SlotInfo kSlots_STEAMVIDEO_INTERFACE_V002[] = {
    {"SteamAPI_ISteamVideo_GetVideoURL", kParams_STEAMVIDEO_INTERFACE_V002_0},
    {"SteamAPI_ISteamVideo_IsBroadcasting", kParams_STEAMVIDEO_INTERFACE_V002_1},
    {"SteamAPI_ISteamVideo_GetOPFSettings", kParams_STEAMVIDEO_INTERFACE_V002_2},
    {"SteamAPI_ISteamVideo_GetOPFStringForApp", kParams_STEAMVIDEO_INTERFACE_V002_3},
};

class Version_STEAMVIDEO_INTERFACE_V002 {
public:
    virtual void GetVideoURL(std::uint32_t unVideoAppID) {
        steambridge::slot<void>(kSlots_STEAMVIDEO_INTERFACE_V002[0], unVideoAppID);
    }
    virtual bool IsBroadcasting(std::int32_t* pnNumViewers) {
        return steambridge::slot<bool>(kSlots_STEAMVIDEO_INTERFACE_V002[1], pnNumViewers);
    }
    virtual void GetOPFSettings(std::uint32_t unVideoAppID) {
        steambridge::slot<void>(kSlots_STEAMVIDEO_INTERFACE_V002[2], unVideoAppID);
    }
    virtual bool GetOPFStringForApp(std::uint32_t unVideoAppID, void* pchBuffer, std::int32_t* pnBufferSize) {
        return steambridge::slot<bool>(kSlots_STEAMVIDEO_INTERFACE_V002[3], unVideoAppID, pchBuffer, pnBufferSize);
    }
};

Version_STEAMVIDEO_INTERFACE_V002 g_STEAMVIDEO_INTERFACE_V002;

const steambridge::InterfaceVersion kVersions[] = {
    {"STEAMAPPLIST_INTERFACE_VERSION001", &g_STEAMAPPLIST_INTERFACE_VERSION001},
    {"STEAMAPPS_INTERFACE_VERSION008", &g_STEAMAPPS_INTERFACE_VERSION008},
    {"SteamClient017", &g_SteamClient017},
    {"SteamClient019", &g_SteamClient019},
    {"SteamClient020", &g_SteamClient020},
    {"SteamController005", &g_SteamController005},
    {"SteamController007", &g_SteamController007},
    {"SteamController008", &g_SteamController008},
    {"SteamFriends015", &g_SteamFriends015},
    {"SteamFriends017", &g_SteamFriends017},
    {"SteamMatchGameSearch001", &g_SteamMatchGameSearch001},
    {"SteamGameServer012", &g_SteamGameServer012},
    {"SteamGameServer013", &g_SteamGameServer013},
    {"SteamGameServer014", &g_SteamGameServer014},
    {"SteamGameServer015", &g_SteamGameServer015},
    {"SteamGameServerStats001", &g_SteamGameServerStats001},
    {"STEAMHTMLSURFACE_INTERFACE_VERSION_004", &g_STEAMHTMLSURFACE_INTERFACE_VERSION_004},
    {"STEAMHTMLSURFACE_INTERFACE_VERSION_005", &g_STEAMHTMLSURFACE_INTERFACE_VERSION_005},
    {"STEAMHTTP_INTERFACE_VERSION002", &g_STEAMHTTP_INTERFACE_VERSION002},
    {"STEAMHTTP_INTERFACE_VERSION003", &g_STEAMHTTP_INTERFACE_VERSION003},
    {"SteamInput001", &g_SteamInput001},
    {"SteamInput002", &g_SteamInput002},
    {"SteamInput006", &g_SteamInput006},
    {"STEAMINVENTORY_INTERFACE_V002", &g_STEAMINVENTORY_INTERFACE_V002},
    {"STEAMINVENTORY_INTERFACE_V003", &g_STEAMINVENTORY_INTERFACE_V003},
    {"SteamMatchMaking009", &g_SteamMatchMaking009},
    {"SteamMatchMakingServers002", &g_SteamMatchMakingServers002},
    {"STEAMMUSIC_INTERFACE_VERSION001", &g_STEAMMUSIC_INTERFACE_VERSION001},
    {"STEAMMUSICREMOTE_INTERFACE_VERSION001", &g_STEAMMUSICREMOTE_INTERFACE_VERSION001},
    {"SteamNetworking005", &g_SteamNetworking005},
    {"SteamNetworking006", &g_SteamNetworking006},
    {"SteamNetworkingMessages002", &g_SteamNetworkingMessages002},
    {"SteamNetworkingSockets009", &g_SteamNetworkingSockets009},
    {"SteamNetworkingSockets012", &g_SteamNetworkingSockets012},
    {"SteamNetworkingUtils003", &g_SteamNetworkingUtils003},
    {"SteamNetworkingUtils004", &g_SteamNetworkingUtils004},
    {"STEAMPARENTALSETTINGS_INTERFACE_VERSION001", &g_STEAMPARENTALSETTINGS_INTERFACE_VERSION001},
    {"SteamParties002", &g_SteamParties002},
    {"STEAMREMOTEPLAY_INTERFACE_VERSION001", &g_STEAMREMOTEPLAY_INTERFACE_VERSION001},
    {"STEAMREMOTESTORAGE_INTERFACE_VERSION014", &g_STEAMREMOTESTORAGE_INTERFACE_VERSION014},
    {"STEAMREMOTESTORAGE_INTERFACE_VERSION016", &g_STEAMREMOTESTORAGE_INTERFACE_VERSION016},
    {"STEAMSCREENSHOTS_INTERFACE_VERSION003", &g_STEAMSCREENSHOTS_INTERFACE_VERSION003},
    {"STEAMUGC_INTERFACE_VERSION010", &g_STEAMUGC_INTERFACE_VERSION010},
    {"STEAMUGC_INTERFACE_VERSION013", &g_STEAMUGC_INTERFACE_VERSION013},
    {"STEAMUGC_INTERFACE_VERSION014", &g_STEAMUGC_INTERFACE_VERSION014},
    {"STEAMUGC_INTERFACE_VERSION015", &g_STEAMUGC_INTERFACE_VERSION015},
    {"STEAMUGC_INTERFACE_VERSION016", &g_STEAMUGC_INTERFACE_VERSION016},
    {"STEAMUGC_INTERFACE_VERSION017", &g_STEAMUGC_INTERFACE_VERSION017},
    {"STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001", &g_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001},
    {"SteamUser019", &g_SteamUser019},
    {"SteamUser020", &g_SteamUser020},
    {"SteamUser021", &g_SteamUser021},
    {"SteamUser023", &g_SteamUser023},
    {"STEAMUSERSTATS_INTERFACE_VERSION011", &g_STEAMUSERSTATS_INTERFACE_VERSION011},
    {"STEAMUSERSTATS_INTERFACE_VERSION012", &g_STEAMUSERSTATS_INTERFACE_VERSION012},
    {"SteamUtils009", &g_SteamUtils009},
    {"SteamUtils010", &g_SteamUtils010},
    {"STEAMVIDEO_INTERFACE_V002", &g_STEAMVIDEO_INTERFACE_V002},
};

}  // namespace

// The object to hand back for a version string, or null when this stub has
// none - a game that asked for something newer than any SDK this knows.
void* interface_object(const char* version) noexcept {
    if (version == nullptr) {
        return nullptr;
    }
    for (const InterfaceVersion& entry : kVersions) {
        if (std::strcmp(entry.version, version) == 0) {
            return entry.object;
        }
    }
    return nullptr;
}

const InterfaceVersion* interface_versions(std::size_t& count) noexcept {
    count = sizeof(kVersions) / sizeof(kVersions[0]);
    return kVersions;
}

}  // namespace steambridge
