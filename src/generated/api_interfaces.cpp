// ============================================================================
//  GENERATED FILE - do not edit by hand.
//
//  Source:   gen/steam_interfaces.json (58 interface versions, 2115 slots,
//            937 distinct calls)
//  Regenerate: steammock_codegen
// ============================================================================

#include "bridge/synth.hpp"

#include <cstring>

namespace steammock {
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

    static Arg arg(CGameID value) noexcept {
        return wire_uint(value.m_gameid);
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

    static Arg arg(CSteamID value) noexcept {
        return wire_uint(value.m_steamid);
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

    static Arg arg(const ControllerAnalogActionData_t&) noexcept {
        return wire_null();
    }
    static ControllerAnalogActionData_t from(const Json&) noexcept {
        return ControllerAnalogActionData_t{};
    }
    static ControllerAnalogActionData_t fallback() noexcept { return ControllerAnalogActionData_t{}; }
};

template <> struct Kind<ControllerDigitalActionData_t> {
    static constexpr bool out() noexcept { return false; }

    static Arg arg(const ControllerDigitalActionData_t&) noexcept {
        return wire_null();
    }
    static ControllerDigitalActionData_t from(const Json&) noexcept {
        return ControllerDigitalActionData_t{};
    }
    static ControllerDigitalActionData_t fallback() noexcept { return ControllerDigitalActionData_t{}; }
};

template <> struct Kind<ControllerMotionData_t> {
    static constexpr bool out() noexcept { return false; }

    static Arg arg(const ControllerMotionData_t&) noexcept {
        return wire_null();
    }
    static ControllerMotionData_t from(const Json&) noexcept {
        return ControllerMotionData_t{};
    }
    static ControllerMotionData_t fallback() noexcept { return ControllerMotionData_t{}; }
};

template <> struct Kind<InputAnalogActionData_t> {
    static constexpr bool out() noexcept { return false; }

    static Arg arg(const InputAnalogActionData_t&) noexcept {
        return wire_null();
    }
    static InputAnalogActionData_t from(const Json&) noexcept {
        return InputAnalogActionData_t{};
    }
    static InputAnalogActionData_t fallback() noexcept { return InputAnalogActionData_t{}; }
};

template <> struct Kind<InputDigitalActionData_t> {
    static constexpr bool out() noexcept { return false; }

    static Arg arg(const InputDigitalActionData_t&) noexcept {
        return wire_null();
    }
    static InputDigitalActionData_t from(const Json&) noexcept {
        return InputDigitalActionData_t{};
    }
    static InputDigitalActionData_t fallback() noexcept { return InputDigitalActionData_t{}; }
};

template <> struct Kind<InputMotionData_t> {
    static constexpr bool out() noexcept { return false; }

    static Arg arg(const InputMotionData_t&) noexcept {
        return wire_null();
    }
    static InputMotionData_t from(const Json&) noexcept {
        return InputMotionData_t{};
    }
    static InputMotionData_t fallback() noexcept { return InputMotionData_t{}; }
};

template <> struct Kind<SteamIPAddress_t> {
    static constexpr bool out() noexcept { return false; }

    static Arg arg(const SteamIPAddress_t&) noexcept {
        return wire_null();
    }
    static SteamIPAddress_t from(const Json&) noexcept {
        return SteamIPAddress_t{};
    }
    static SteamIPAddress_t fallback() noexcept { return SteamIPAddress_t{}; }
};

template <> struct Kind<SteamPartyBeaconLocation_t> {
    static constexpr bool out() noexcept { return false; }

    static Arg arg(const SteamPartyBeaconLocation_t&) noexcept {
        return wire_null();
    }
    static SteamPartyBeaconLocation_t from(const Json&) noexcept {
        return SteamPartyBeaconLocation_t{};
    }
    static SteamPartyBeaconLocation_t fallback() noexcept { return SteamPartyBeaconLocation_t{}; }
};

namespace {

// ---------------------------------------------------------------------------
//  The argument names the calls carry.
// ---------------------------------------------------------------------------
//  One list per distinct set of names, so a call declared in six versions names
//  its arguments once and all six point here. A call that takes none has no
//  list, which is what the null in its descriptor says.

const char* const kParams_0[] = {"pvecAppID", "unMaxAppIDs"};
const char* const kParams_1[] = {"nAppID", "pchName", "cchNameMax"};
const char* const kParams_2[] = {"nAppID", "pchDirectory", "cchNameMax"};
const char* const kParams_3[] = {"nAppID"};
const char* const kParams_4[] = {"appID"};
const char* const kParams_5[] = {"iDLC", "pAppID", "pbAvailable", "pchName", "cchNameBufferSize"};
const char* const kParams_6[] = {"pchName", "cchNameBufferSize"};
const char* const kParams_7[] = {"bMissingFilesOnly"};
const char* const kParams_8[] = {"appID", "pvecDepots", "cMaxDepots"};
const char* const kParams_9[] = {"appID", "pchFolder", "cchFolderBufferSize"};
const char* const kParams_10[] = {"pchKey"};
const char* const kParams_11[] = {"nAppID", "punBytesDownloaded", "punBytesTotal"};
const char* const kParams_12[] = {"pszFileName"};
const char* const kParams_13[] = {"pszCommandLine", "cubCommandLine"};
const char* const kParams_14[] = {"punSecondsAllowed", "punSecondsPlayed"};
const char* const kParams_15[] = {"hSteamPipe"};
const char* const kParams_16[] = {"phSteamPipe", "eAccountType"};
const char* const kParams_17[] = {"hSteamPipe", "hUser"};
const char* const kParams_18[] = {"hSteamUser", "hSteamPipe", "pchVersion"};
const char* const kParams_19[] = {"unIP", "usPort"};
const char* const kParams_20[] = {"hSteamPipe", "pchVersion"};
const char* const kParams_21[] = {"hSteamuser", "hSteamPipe", "pchVersion"};
const char* const kParams_22[] = {"pFunction"};
const char* const kParams_23[] = {"arg0"};
const char* const kParams_24[] = {"func"};
const char* const kParams_25[] = {"handlesOut"};
const char* const kParams_26[] = {"controllerHandle"};
const char* const kParams_27[] = {"pszActionSetName"};
const char* const kParams_28[] = {"controllerHandle", "actionSetHandle"};
const char* const kParams_29[] = {"pszActionName"};
const char* const kParams_30[] = {"controllerHandle", "digitalActionHandle"};
const char* const kParams_31[] = {
    "controllerHandle",
    "actionSetHandle",
    "digitalActionHandle",
    "originsOut",
};
const char* const kParams_32[] = {"controllerHandle", "analogActionHandle"};
const char* const kParams_33[] = {
    "controllerHandle",
    "actionSetHandle",
    "analogActionHandle",
    "originsOut",
};
const char* const kParams_34[] = {"controllerHandle", "eAction"};
const char* const kParams_35[] = {"controllerHandle", "eTargetPad", "usDurationMicroSec"};
const char* const kParams_36[] = {
    "controllerHandle",
    "eTargetPad",
    "usDurationMicroSec",
    "usOffMicroSec",
    "unRepeat",
    "nFlags",
};
const char* const kParams_37[] = {"controllerHandle", "usLeftSpeed", "usRightSpeed"};
const char* const kParams_38[] = {"controllerHandle", "nColorR", "nColorG", "nColorB", "nFlags"};
const char* const kParams_39[] = {"ulControllerHandle"};
const char* const kParams_40[] = {"nIndex"};
const char* const kParams_41[] = {
    "controllerHandle",
    "digitalActionHandle",
    "flScale",
    "flXPosition",
    "flYPosition",
};
const char* const kParams_42[] = {
    "controllerHandle",
    "analogActionHandle",
    "flScale",
    "flXPosition",
    "flYPosition",
};
const char* const kParams_43[] = {"eOrigin"};
const char* const kParams_44[] = {"controllerHandle", "actionSetLayerHandle"};
const char* const kParams_45[] = {"controllerHandle", "handlesOut"};
const char* const kParams_46[] = {"controllerHandle", "eOrigin"};
const char* const kParams_47[] = {"eDestinationInputType", "eSourceOrigin"};
const char* const kParams_48[] = {"controllerHandle", "pMajor", "pMinor"};
const char* const kParams_49[] = {"pchPersonaName"};
const char* const kParams_50[] = {"iFriendFlags"};
const char* const kParams_51[] = {"iFriend", "iFriendFlags"};
const char* const kParams_52[] = {"steamIDFriend"};
const char* const kParams_53[] = {"steamIDFriend", "pFriendGameInfo"};
const char* const kParams_54[] = {"steamIDFriend", "iPersonaName"};
const char* const kParams_55[] = {"steamIDPlayer"};
const char* const kParams_56[] = {"iFG"};
const char* const kParams_57[] = {"friendsGroupID"};
const char* const kParams_58[] = {"friendsGroupID", "pOutSteamIDMembers", "nMembersCount"};
const char* const kParams_59[] = {"steamIDFriend", "iFriendFlags"};
const char* const kParams_60[] = {"iClan"};
const char* const kParams_61[] = {"steamIDClan"};
const char* const kParams_62[] = {"steamIDClan", "pnOnline", "pnInGame", "pnChatting"};
const char* const kParams_63[] = {"psteamIDClans", "cClansToRequest"};
const char* const kParams_64[] = {"steamIDSource"};
const char* const kParams_65[] = {"steamIDSource", "iFriend"};
const char* const kParams_66[] = {"steamIDUser", "steamIDSource"};
const char* const kParams_67[] = {"steamIDUser", "bSpeaking"};
const char* const kParams_68[] = {"pchDialog"};
const char* const kParams_69[] = {"pchDialog", "steamID"};
const char* const kParams_70[] = {"pchURL"};
const char* const kParams_71[] = {"nAppID", "eFlag"};
const char* const kParams_72[] = {"steamIDUserPlayedWith"};
const char* const kParams_73[] = {"steamIDLobby"};
const char* const kParams_74[] = {"steamIDUser", "bRequireNameOnly"};
const char* const kParams_75[] = {"steamIDClan", "iOfficer"};
const char* const kParams_76[] = {"pchKey", "pchValue"};
const char* const kParams_77[] = {"steamIDFriend", "pchKey"};
const char* const kParams_78[] = {"steamIDFriend", "iKey"};
const char* const kParams_79[] = {"steamIDFriend", "pchConnectString"};
const char* const kParams_80[] = {"iCoplayFriend"};
const char* const kParams_81[] = {"steamIDClan", "iUser"};
const char* const kParams_82[] = {"steamIDClanChat", "pchText"};
const char* const kParams_83[] = {
    "steamIDClanChat",
    "iMessage",
    "prgchText",
    "cchTextMax",
    "peChatEntryType",
    "psteamidChatter",
};
const char* const kParams_84[] = {"steamIDClanChat", "steamIDUser"};
const char* const kParams_85[] = {"steamIDClanChat"};
const char* const kParams_86[] = {"bInterceptEnabled"};
const char* const kParams_87[] = {"steamIDFriend", "pchMsgToSend"};
const char* const kParams_88[] = {
    "steamIDFriend",
    "iMessageID",
    "pvData",
    "cubData",
    "peChatEntryType",
};
const char* const kParams_89[] = {"steamID"};
const char* const kParams_90[] = {"unStartIndex"};
const char* const kParams_91[] = {"pchURL", "eMode"};
const char* const kParams_92[] = {"pchProtocol"};
const char* const kParams_93[] = {"pchConnectString"};
const char* const kParams_94[] = {"steamID", "itemType"};
const char* const kParams_95[] = {"steamID", "itemType", "prop"};
const char* const kParams_96[] = {"pchKeyToFind", "pchValuesToFind"};
const char* const kParams_97[] = {"steamIDLobby", "nPlayerMin", "nPlayerMax"};
const char* const kParams_98[] = {"nPlayerMin", "nPlayerMax"};
const char* const kParams_99[] = {"steamIDHost", "pchConnectionDetails", "cubConnectionDetails"};
const char* const kParams_100[] = {"pchConnectionDetails", "cubConnectionDetails"};
const char* const kParams_101[] = {"nPlayerMin", "nPlayerMax", "nMaxTeamSize"};
const char* const kParams_102[] = {"ullUniqueGameID"};
const char* const kParams_103[] = {"ullUniqueGameID", "steamIDPlayer", "EPlayerResult"};
const char* const kParams_104[] = {
    "unIP",
    "usGamePort",
    "usQueryPort",
    "unFlags",
    "nGameAppId",
    "pchVersionString",
};
const char* const kParams_105[] = {"pszProduct"};
const char* const kParams_106[] = {"pszGameDescription"};
const char* const kParams_107[] = {"pszModDir"};
const char* const kParams_108[] = {"bDedicated"};
const char* const kParams_109[] = {"pszToken"};
const char* const kParams_110[] = {"cPlayersMax"};
const char* const kParams_111[] = {"cBotplayers"};
const char* const kParams_112[] = {"pszServerName"};
const char* const kParams_113[] = {"pszMapName"};
const char* const kParams_114[] = {"bPasswordProtected"};
const char* const kParams_115[] = {"unSpectatorPort"};
const char* const kParams_116[] = {"pszSpectatorServerName"};
const char* const kParams_117[] = {"pKey", "pValue"};
const char* const kParams_118[] = {"pchGameTags"};
const char* const kParams_119[] = {"pchGameData"};
const char* const kParams_120[] = {"pszRegion"};
const char* const kParams_121[] = {"unIPClient", "pvAuthBlob", "cubAuthBlobSize", "pSteamIDUser"};
const char* const kParams_122[] = {"steamIDUser"};
const char* const kParams_123[] = {"steamIDUser", "pchPlayerName", "uScore"};
const char* const kParams_124[] = {"pTicket", "cbMaxTicket", "pcbTicket"};
const char* const kParams_125[] = {"pAuthTicket", "cbAuthTicket", "steamID"};
const char* const kParams_126[] = {"hAuthTicket"};
const char* const kParams_127[] = {"steamID", "appID"};
const char* const kParams_128[] = {"steamIDUser", "steamIDGroup"};
const char* const kParams_129[] = {"pData", "cbData", "srcIP", "srcPort"};
const char* const kParams_130[] = {"pOut", "cbMaxOut", "pNetAdr", "pPort"};
const char* const kParams_131[] = {"bActive"};
const char* const kParams_132[] = {"iHeartbeatInterval"};
const char* const kParams_133[] = {"steamIDNewPlayer"};
const char* const kParams_134[] = {"pTicket", "cbMaxTicket", "pcbTicket", "pSnid"};
const char* const kParams_135[] = {"steamIDUser", "pchName", "pData"};
const char* const kParams_136[] = {"steamIDUser", "pchName", "pbAchieved"};
const char* const kParams_137[] = {"steamIDUser", "pchName", "nData"};
const char* const kParams_138[] = {"steamIDUser", "pchName", "fData"};
const char* const kParams_139[] = {
    "steamIDUser",
    "pchName",
    "flCountThisSession",
    "dSessionLength",
};
const char* const kParams_140[] = {"steamIDUser", "pchName"};
const char* const kParams_141[] = {"pchUserAgent", "pchUserCSS"};
const char* const kParams_142[] = {"unBrowserHandle"};
const char* const kParams_143[] = {"unBrowserHandle", "pchURL", "pchPostData"};
const char* const kParams_144[] = {"unBrowserHandle", "unWidth", "unHeight"};
const char* const kParams_145[] = {"unBrowserHandle", "pchKey", "pchValue"};
const char* const kParams_146[] = {"unBrowserHandle", "pchScript"};
const char* const kParams_147[] = {"unBrowserHandle", "eMouseButton"};
const char* const kParams_148[] = {"unBrowserHandle", "x", "y"};
const char* const kParams_149[] = {"unBrowserHandle", "nDelta"};
const char* const kParams_150[] = {"unBrowserHandle", "nNativeKeyCode", "eHTMLKeyModifiers"};
const char* const kParams_151[] = {"unBrowserHandle", "cUnicodeChar", "eHTMLKeyModifiers"};
const char* const kParams_152[] = {"unBrowserHandle", "nAbsolutePixelScroll"};
const char* const kParams_153[] = {"unBrowserHandle", "bHasKeyFocus"};
const char* const kParams_154[] = {
    "unBrowserHandle",
    "pchSearchStr",
    "bCurrentlyInFind",
    "bReverse",
};
const char* const kParams_155[] = {
    "pchHostname",
    "pchKey",
    "pchValue",
    "pchPath",
    "nExpires",
    "bSecure",
    "bHTTPOnly",
};
const char* const kParams_156[] = {"unBrowserHandle", "flZoom", "nPointX", "nPointY"};
const char* const kParams_157[] = {"unBrowserHandle", "bBackgroundMode"};
const char* const kParams_158[] = {"unBrowserHandle", "flDPIScaling"};
const char* const kParams_159[] = {"unBrowserHandle", "bAllowed"};
const char* const kParams_160[] = {"unBrowserHandle", "bResult"};
const char* const kParams_161[] = {"unBrowserHandle", "pchSelectedFiles"};
const char* const kParams_162[] = {
    "unBrowserHandle",
    "nNativeKeyCode",
    "eHTMLKeyModifiers",
    "bIsSystemKey",
};
const char* const kParams_163[] = {"eHTTPRequestMethod", "pchAbsoluteURL"};
const char* const kParams_164[] = {"hRequest", "ulContextValue"};
const char* const kParams_165[] = {"hRequest", "unTimeoutSeconds"};
const char* const kParams_166[] = {"hRequest", "pchHeaderName", "pchHeaderValue"};
const char* const kParams_167[] = {"hRequest", "pchParamName", "pchParamValue"};
const char* const kParams_168[] = {"hRequest", "pCallHandle"};
const char* const kParams_169[] = {"hRequest"};
const char* const kParams_170[] = {"hRequest", "pchHeaderName", "unResponseHeaderSize"};
const char* const kParams_171[] = {
    "hRequest",
    "pchHeaderName",
    "pHeaderValueBuffer",
    "unBufferSize",
};
const char* const kParams_172[] = {"hRequest", "unBodySize"};
const char* const kParams_173[] = {"hRequest", "pBodyDataBuffer", "unBufferSize"};
const char* const kParams_174[] = {"hRequest", "cOffset", "pBodyDataBuffer", "unBufferSize"};
const char* const kParams_175[] = {"hRequest", "pflPercentOut"};
const char* const kParams_176[] = {"hRequest", "pchContentType", "pubBody", "unBodyLen"};
const char* const kParams_177[] = {"bAllowResponsesToModify"};
const char* const kParams_178[] = {"hCookieContainer"};
const char* const kParams_179[] = {"hCookieContainer", "pchHost", "pchUrl", "pchCookie"};
const char* const kParams_180[] = {"hRequest", "hCookieContainer"};
const char* const kParams_181[] = {"hRequest", "pchUserAgentInfo"};
const char* const kParams_182[] = {"hRequest", "bRequireVerifiedCertificate"};
const char* const kParams_183[] = {"hRequest", "unMilliseconds"};
const char* const kParams_184[] = {"hRequest", "pbWasTimedOut"};
const char* const kParams_185[] = {"inputHandle", "actionSetHandle"};
const char* const kParams_186[] = {"inputHandle"};
const char* const kParams_187[] = {"inputHandle", "actionSetLayerHandle"};
const char* const kParams_188[] = {"inputHandle", "handlesOut"};
const char* const kParams_189[] = {"inputHandle", "digitalActionHandle"};
const char* const kParams_190[] = {
    "inputHandle",
    "actionSetHandle",
    "digitalActionHandle",
    "originsOut",
};
const char* const kParams_191[] = {"inputHandle", "analogActionHandle"};
const char* const kParams_192[] = {
    "inputHandle",
    "actionSetHandle",
    "analogActionHandle",
    "originsOut",
};
const char* const kParams_193[] = {"inputHandle", "eAction"};
const char* const kParams_194[] = {"inputHandle", "usLeftSpeed", "usRightSpeed"};
const char* const kParams_195[] = {"inputHandle", "nColorR", "nColorG", "nColorB", "nFlags"};
const char* const kParams_196[] = {"inputHandle", "eTargetPad", "usDurationMicroSec"};
const char* const kParams_197[] = {
    "inputHandle",
    "eTargetPad",
    "usDurationMicroSec",
    "usOffMicroSec",
    "unRepeat",
    "nFlags",
};
const char* const kParams_198[] = {"ulinputHandle"};
const char* const kParams_199[] = {"inputHandle", "eOrigin"};
const char* const kParams_200[] = {"inputHandle", "pMajor", "pMinor"};
const char* const kParams_201[] = {"bExplicitlyCallRunFrame"};
const char* const kParams_202[] = {"pchInputActionManifestAbsolutePath"};
const char* const kParams_203[] = {"bReservedValue"};
const char* const kParams_204[] = {"bWaitForever", "unTimeout"};
const char* const kParams_205[] = {"pCallback"};
const char* const kParams_206[] = {"eActionHandle"};
const char* const kParams_207[] = {"eOrigin", "eSize", "unFlags"};
const char* const kParams_208[] = {"eOrigin", "unFlags"};
const char* const kParams_209[] = {
    "inputHandle",
    "usLeftSpeed",
    "usRightSpeed",
    "usLeftTriggerSpeed",
    "usRightTriggerSpeed",
};
const char* const kParams_210[] = {
    "inputHandle",
    "eHapticLocation",
    "nIntensity",
    "nGainDB",
    "nOtherIntensity",
    "nOtherGainDB",
};
const char* const kParams_211[] = {"inputHandle", "pParam"};
const char* const kParams_212[] = {"resultHandle"};
const char* const kParams_213[] = {"resultHandle", "pOutItemsArray", "punOutItemsArraySize"};
const char* const kParams_214[] = {
    "resultHandle",
    "unItemIndex",
    "pchPropertyName",
    "pchValueBuffer",
    "punValueBufferSizeOut",
};
const char* const kParams_215[] = {"resultHandle", "steamIDExpected"};
const char* const kParams_216[] = {"pResultHandle"};
const char* const kParams_217[] = {"pResultHandle", "pInstanceIDs", "unCountInstanceIDs"};
const char* const kParams_218[] = {"resultHandle", "pOutBuffer", "punOutBufferSize"};
const char* const kParams_219[] = {
    "pOutResultHandle",
    "pBuffer",
    "unBufferSize",
    "bRESERVED_MUST_BE_FALSE",
};
const char* const kParams_220[] = {
    "pResultHandle",
    "pArrayItemDefs",
    "punArrayQuantity",
    "unArrayLength",
};
const char* const kParams_221[] = {"pResultHandle", "itemDef"};
const char* const kParams_222[] = {"pResultHandle", "pArrayItemDefs", "unArrayLength"};
const char* const kParams_223[] = {"pResultHandle", "itemConsume", "unQuantity"};
const char* const kParams_224[] = {
    "pResultHandle",
    "pArrayGenerate",
    "punArrayGenerateQuantity",
    "unArrayGenerateLength",
    "pArrayDestroy",
    "punArrayDestroyQuantity",
    "unArrayDestroyLength",
};
const char* const kParams_225[] = {"pResultHandle", "itemIdSource", "unQuantity", "itemIdDest"};
const char* const kParams_226[] = {"pResultHandle", "dropListDefinition"};
const char* const kParams_227[] = {
    "pResultHandle",
    "steamIDTradePartner",
    "pArrayGive",
    "pArrayGiveQuantity",
    "nArrayGiveLength",
    "pArrayGet",
    "pArrayGetQuantity",
    "nArrayGetLength",
};
const char* const kParams_228[] = {"pItemDefIDs", "punItemDefIDsArraySize"};
const char* const kParams_229[] = {
    "iDefinition",
    "pchPropertyName",
    "pchValueBuffer",
    "punValueBufferSizeOut",
};
const char* const kParams_230[] = {"steamID", "pItemDefIDs", "punItemDefIDsArraySize"};
const char* const kParams_231[] = {"pArrayItemDefs", "punArrayQuantity", "unArrayLength"};
const char* const kParams_232[] = {
    "pArrayItemDefs",
    "pCurrentPrices",
    "pBasePrices",
    "unArrayLength",
};
const char* const kParams_233[] = {"iDefinition", "pCurrentPrice", "pBasePrice"};
const char* const kParams_234[] = {"handle", "nItemID", "pchPropertyName"};
const char* const kParams_235[] = {"handle", "nItemID", "pchPropertyName", "pchPropertyValue"};
const char* const kParams_236[] = {"handle", "nItemID", "pchPropertyName", "bValue"};
const char* const kParams_237[] = {"handle", "nItemID", "pchPropertyName", "nValue"};
const char* const kParams_238[] = {"handle", "nItemID", "pchPropertyName", "flValue"};
const char* const kParams_239[] = {"handle", "pResultHandle"};
const char* const kParams_240[] = {"pResultHandle", "pchItemToken"};
const char* const kParams_241[] = {
    "iGame",
    "pnAppID",
    "pnIP",
    "pnConnPort",
    "pnQueryPort",
    "punFlags",
    "pRTime32LastPlayedOnServer",
};
const char* const kParams_242[] = {
    "nAppID",
    "nIP",
    "nConnPort",
    "nQueryPort",
    "unFlags",
    "rTime32LastPlayedOnServer",
};
const char* const kParams_243[] = {"nAppID", "nIP", "nConnPort", "nQueryPort", "unFlags"};
const char* const kParams_244[] = {"pchKeyToMatch", "pchValueToMatch", "eComparisonType"};
const char* const kParams_245[] = {"pchKeyToMatch", "nValueToMatch", "eComparisonType"};
const char* const kParams_246[] = {"pchKeyToMatch", "nValueToBeCloseTo"};
const char* const kParams_247[] = {"nSlotsAvailable"};
const char* const kParams_248[] = {"eLobbyDistanceFilter"};
const char* const kParams_249[] = {"cMaxResults"};
const char* const kParams_250[] = {"iLobby"};
const char* const kParams_251[] = {"eLobbyType", "cMaxMembers"};
const char* const kParams_252[] = {"steamIDLobby", "steamIDInvitee"};
const char* const kParams_253[] = {"steamIDLobby", "iMember"};
const char* const kParams_254[] = {"steamIDLobby", "pchKey"};
const char* const kParams_255[] = {"steamIDLobby", "pchKey", "pchValue"};
const char* const kParams_256[] = {
    "steamIDLobby",
    "iLobbyData",
    "pchKey",
    "cchKeyBufferSize",
    "pchValue",
    "cchValueBufferSize",
};
const char* const kParams_257[] = {"steamIDLobby", "steamIDUser", "pchKey"};
const char* const kParams_258[] = {"steamIDLobby", "pvMsgBody", "cubMsgBody"};
const char* const kParams_259[] = {
    "steamIDLobby",
    "iChatID",
    "pSteamIDUser",
    "pvData",
    "cubData",
    "peChatEntryType",
};
const char* const kParams_260[] = {
    "steamIDLobby",
    "unGameServerIP",
    "unGameServerPort",
    "steamIDGameServer",
};
const char* const kParams_261[] = {
    "steamIDLobby",
    "punGameServerIP",
    "punGameServerPort",
    "psteamIDGameServer",
};
const char* const kParams_262[] = {"steamIDLobby", "cMaxMembers"};
const char* const kParams_263[] = {"steamIDLobby", "eLobbyType"};
const char* const kParams_264[] = {"steamIDLobby", "bLobbyJoinable"};
const char* const kParams_265[] = {"steamIDLobby", "steamIDNewOwner"};
const char* const kParams_266[] = {"steamIDLobby", "steamIDLobbyDependent"};
const char* const kParams_267[] = {"iApp", "ppchFilters", "nFilters", "pRequestServersResponse"};
const char* const kParams_268[] = {"iApp", "pRequestServersResponse"};
const char* const kParams_269[] = {"hServerListRequest"};
const char* const kParams_270[] = {"hRequest", "iServer"};
const char* const kParams_271[] = {"unIP", "usPort", "pRequestServersResponse"};
const char* const kParams_272[] = {"hServerQuery"};
const char* const kParams_273[] = {"flVolume"};
const char* const kParams_274[] = {"pchName"};
const char* const kParams_275[] = {"bValue"};
const char* const kParams_276[] = {"pchDisplayName"};
const char* const kParams_277[] = {"pvBuffer", "cbBufferLength"};
const char* const kParams_278[] = {"nStatus"};
const char* const kParams_279[] = {"flValue"};
const char* const kParams_280[] = {"bAvailable"};
const char* const kParams_281[] = {"pchText"};
const char* const kParams_282[] = {"nValue"};
const char* const kParams_283[] = {"nID", "nPosition", "pchEntryText"};
const char* const kParams_284[] = {"nID"};
const char* const kParams_285[] = {
    "steamIDRemote",
    "pubData",
    "cubData",
    "eP2PSendType",
    "nChannel",
};
const char* const kParams_286[] = {"pcubMsgSize", "nChannel"};
const char* const kParams_287[] = {
    "pubDest",
    "cubDest",
    "pcubMsgSize",
    "psteamIDRemote",
    "nChannel",
};
const char* const kParams_288[] = {"steamIDRemote"};
const char* const kParams_289[] = {"steamIDRemote", "nChannel"};
const char* const kParams_290[] = {"steamIDRemote", "pConnectionState"};
const char* const kParams_291[] = {"bAllow"};
const char* const kParams_292[] = {"nVirtualP2PPort", "nIP", "nPort", "bAllowUseOfPacketRelay"};
const char* const kParams_293[] = {
    "steamIDTarget",
    "nVirtualPort",
    "nTimeoutSec",
    "bAllowUseOfPacketRelay",
};
const char* const kParams_294[] = {"nIP", "nPort", "nTimeoutSec"};
const char* const kParams_295[] = {"hSocket", "bNotifyRemoteEnd"};
const char* const kParams_296[] = {"hSocket", "pubData", "cubData", "bReliable"};
const char* const kParams_297[] = {"hSocket", "pcubMsgSize"};
const char* const kParams_298[] = {"hSocket", "pubDest", "cubDest", "pcubMsgSize"};
const char* const kParams_299[] = {"hListenSocket", "pcubMsgSize", "phSocket"};
const char* const kParams_300[] = {
    "hListenSocket",
    "pubDest",
    "cubDest",
    "pcubMsgSize",
    "phSocket",
};
const char* const kParams_301[] = {
    "hSocket",
    "pSteamIDRemote",
    "peSocketStatus",
    "punIPRemote",
    "punPortRemote",
};
const char* const kParams_302[] = {"hListenSocket", "pnIP", "pnPort"};
const char* const kParams_303[] = {"hSocket"};
const char* const kParams_304[] = {
    "identityRemote",
    "pubData",
    "cubData",
    "nSendFlags",
    "nRemoteChannel",
};
const char* const kParams_305[] = {"nLocalChannel", "ppOutMessages", "nMaxMessages"};
const char* const kParams_306[] = {"identityRemote"};
const char* const kParams_307[] = {"identityRemote", "nLocalChannel"};
const char* const kParams_308[] = {"identityRemote", "pConnectionInfo", "pQuickStatus"};
const char* const kParams_309[] = {"localAddress", "nOptions", "pOptions"};
const char* const kParams_310[] = {"address", "nOptions", "pOptions"};
const char* const kParams_311[] = {"nLocalVirtualPort", "nOptions", "pOptions"};
const char* const kParams_312[] = {"identityRemote", "nRemoteVirtualPort", "nOptions", "pOptions"};
const char* const kParams_313[] = {"hConn"};
const char* const kParams_314[] = {"hPeer", "nReason", "pszDebug", "bEnableLinger"};
const char* const kParams_315[] = {"hPeer", "nUserData"};
const char* const kParams_316[] = {"hPeer"};
const char* const kParams_317[] = {"hPeer", "pszName"};
const char* const kParams_318[] = {"hPeer", "pszName", "nMaxLen"};
const char* const kParams_319[] = {"hConn", "pData", "cbData", "nSendFlags", "pOutMessageNumber"};
const char* const kParams_320[] = {"nMessages", "pMessages", "pOutMessageNumberOrResult"};
const char* const kParams_321[] = {"hConn", "ppOutMessages", "nMaxMessages"};
const char* const kParams_322[] = {"hConn", "pInfo"};
const char* const kParams_323[] = {"hConn", "pStats"};
const char* const kParams_324[] = {"hConn", "pszBuf", "cbBuf"};
const char* const kParams_325[] = {"hSocket", "address"};
const char* const kParams_326[] = {
    "pOutConnection1",
    "pOutConnection2",
    "bUseNetworkLoopback",
    "pIdentity1",
    "pIdentity2",
};
const char* const kParams_327[] = {"pIdentity"};
const char* const kParams_328[] = {"pDetails"};
const char* const kParams_329[] = {"hPollGroup"};
const char* const kParams_330[] = {"hConn", "hPollGroup"};
const char* const kParams_331[] = {"hPollGroup", "ppOutMessages", "nMaxMessages"};
const char* const kParams_332[] = {"pvTicket", "cbTicket", "pOutParsedTicket"};
const char* const kParams_333[] = {"identityGameServer", "nRemoteVirtualPort", "pOutParsedTicket"};
const char* const kParams_334[] = {"identityTarget", "nRemoteVirtualPort", "nOptions", "pOptions"};
const char* const kParams_335[] = {"pRouting"};
const char* const kParams_336[] = {"pLoginInfo", "pcbSignedBlob", "pBlob"};
const char* const kParams_337[] = {
    "pSignaling",
    "pPeerIdentity",
    "nRemoteVirtualPort",
    "nOptions",
    "pOptions",
};
const char* const kParams_338[] = {"pMsg", "cbMsg", "pContext"};
const char* const kParams_339[] = {"pcbBlob", "pBlob", "errMsg"};
const char* const kParams_340[] = {"pCertificate", "cbCertificate", "errMsg"};
const char* const kParams_341[] = {"hConn", "pStatus", "nLanes", "pLanes"};
const char* const kParams_342[] = {"hConn", "nNumLanes", "pLanePriorities", "pLaneWeights"};
const char* const kParams_343[] = {"nNumPorts"};
const char* const kParams_344[] = {"idxFirstPort", "pInfo"};
const char* const kParams_345[] = {"idxFakePort", "nOptions", "pOptions"};
const char* const kParams_346[] = {"hConn", "pOutAddr"};
const char* const kParams_347[] = {"idxFakeServerPort"};
const char* const kParams_348[] = {"cbAllocateBuffer"};
const char* const kParams_349[] = {"result"};
const char* const kParams_350[] = {"location1", "location2"};
const char* const kParams_351[] = {"remoteLocation"};
const char* const kParams_352[] = {"location", "pszBuf", "cchBufSize"};
const char* const kParams_353[] = {"pszString", "result"};
const char* const kParams_354[] = {"flMaxAgeSeconds"};
const char* const kParams_355[] = {"popID", "pViaRelayPoP"};
const char* const kParams_356[] = {"popID"};
const char* const kParams_357[] = {"list", "nListSz"};
const char* const kParams_358[] = {"eDetailLevel", "pfnFunc"};
const char* const kParams_359[] = {"eValue", "eScopeType", "scopeObj", "eDataType", "pArg"};
const char* const kParams_360[] = {
    "eValue",
    "eScopeType",
    "scopeObj",
    "pOutDataType",
    "pResult",
    "cbResult",
};
const char* const kParams_361[] = {
    "eValue",
    "pOutName",
    "pOutDataType",
    "pOutScope",
    "pOutNextValue",
};
const char* const kParams_362[] = {"addr", "buf", "cbBuf", "bWithPort"};
const char* const kParams_363[] = {"pAddr", "pszStr"};
const char* const kParams_364[] = {"identity", "buf", "cbBuf"};
const char* const kParams_365[] = {"pIdentity", "pszStr"};
const char* const kParams_366[] = {"nIPv4"};
const char* const kParams_367[] = {"fakeIP", "pOutRealIdentity"};
const char* const kParams_368[] = {"eValue", "pOutDataType", "pOutScope"};
const char* const kParams_369[] = {"eCurrent", "bEnumerateDevVars"};
const char* const kParams_370[] = {"addr"};
const char* const kParams_371[] = {"eFeature"};
const char* const kParams_372[] = {"unIndex"};
const char* const kParams_373[] = {
    "ulBeaconID",
    "pSteamIDBeaconOwner",
    "pLocation",
    "pchMetadata",
    "cchMetadata",
};
const char* const kParams_374[] = {"ulBeaconID"};
const char* const kParams_375[] = {"puNumLocations"};
const char* const kParams_376[] = {"pLocationList", "uMaxNumLocations"};
const char* const kParams_377[] = {
    "unOpenSlots",
    "pBeaconLocation",
    "pchConnectString",
    "pchMetadata",
};
const char* const kParams_378[] = {"ulBeacon", "steamIDUser"};
const char* const kParams_379[] = {"ulBeacon", "unOpenSlots"};
const char* const kParams_380[] = {"ulBeacon"};
const char* const kParams_381[] = {
    "BeaconLocation",
    "eData",
    "pchDataStringOut",
    "cchDataStringOut",
};
const char* const kParams_382[] = {"iSessionIndex"};
const char* const kParams_383[] = {"unSessionID"};
const char* const kParams_384[] = {"unSessionID", "pnResolutionX", "pnResolutionY"};
const char* const kParams_385[] = {"pchFile", "pvData", "cubData"};
const char* const kParams_386[] = {"pchFile", "pvData", "cubDataToRead"};
const char* const kParams_387[] = {"pchFile", "nOffset", "cubToRead"};
const char* const kParams_388[] = {"hReadCall", "pvBuffer", "cubToRead"};
const char* const kParams_389[] = {"pchFile"};
const char* const kParams_390[] = {"pchFile", "eRemoteStoragePlatform"};
const char* const kParams_391[] = {"writeHandle", "pvData", "cubData"};
const char* const kParams_392[] = {"writeHandle"};
const char* const kParams_393[] = {"iFile", "pnFileSizeInBytes"};
const char* const kParams_394[] = {"pnTotalBytes", "puAvailableBytes"};
const char* const kParams_395[] = {"bEnabled"};
const char* const kParams_396[] = {"hContent", "unPriority"};
const char* const kParams_397[] = {"hContent", "pnBytesDownloaded", "pnBytesExpected"};
const char* const kParams_398[] = {
    "hContent",
    "pnAppID",
    "ppchName",
    "pnFileSizeInBytes",
    "pSteamIDOwner",
};
const char* const kParams_399[] = {"hContent", "pvData", "cubDataToRead", "cOffset", "eAction"};
const char* const kParams_400[] = {"iCachedContent"};
const char* const kParams_401[] = {
    "pchFile",
    "pchPreviewFile",
    "nConsumerAppId",
    "pchTitle",
    "pchDescription",
    "eVisibility",
    "pTags",
    "eWorkshopFileType",
};
const char* const kParams_402[] = {"unPublishedFileId"};
const char* const kParams_403[] = {"updateHandle", "pchFile"};
const char* const kParams_404[] = {"updateHandle", "pchPreviewFile"};
const char* const kParams_405[] = {"updateHandle", "pchTitle"};
const char* const kParams_406[] = {"updateHandle", "pchDescription"};
const char* const kParams_407[] = {"updateHandle", "eVisibility"};
const char* const kParams_408[] = {"updateHandle", "pTags"};
const char* const kParams_409[] = {"updateHandle"};
const char* const kParams_410[] = {"unPublishedFileId", "unMaxSecondsOld"};
const char* const kParams_411[] = {"updateHandle", "pchChangeDescription"};
const char* const kParams_412[] = {"unPublishedFileId", "bVoteUp"};
const char* const kParams_413[] = {"steamId", "unStartIndex", "pRequiredTags", "pExcludedTags"};
const char* const kParams_414[] = {
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
const char* const kParams_415[] = {"unPublishedFileId", "eAction"};
const char* const kParams_416[] = {"eAction", "unStartIndex"};
const char* const kParams_417[] = {
    "eEnumerationType",
    "unStartIndex",
    "unCount",
    "unDays",
    "pTags",
    "pUserTags",
};
const char* const kParams_418[] = {"hContent", "pchLocation", "unPriority"};
const char* const kParams_419[] = {"iFile", "pEChangeType", "pEFilePathType"};
const char* const kParams_420[] = {"pubRGB", "cubRGB", "nWidth", "nHeight"};
const char* const kParams_421[] = {"pchFilename", "pchThumbnailFilename", "nWidth", "nHeight"};
const char* const kParams_422[] = {"bHook"};
const char* const kParams_423[] = {"hScreenshot", "pchLocation"};
const char* const kParams_424[] = {"hScreenshot", "steamID"};
const char* const kParams_425[] = {"hScreenshot", "unPublishedFileID"};
const char* const kParams_426[] = {"eType", "pchFilename", "pchVRFilename"};
const char* const kParams_427[] = {
    "unAccountID",
    "eListType",
    "eMatchingUGCType",
    "eSortOrder",
    "nCreatorAppID",
    "nConsumerAppID",
    "unPage",
};
const char* const kParams_428[] = {
    "eQueryType",
    "eMatchingeMatchingUGCTypeFileType",
    "nCreatorAppID",
    "nConsumerAppID",
    "unPage",
};
const char* const kParams_429[] = {"pvecPublishedFileID", "unNumPublishedFileIDs"};
const char* const kParams_430[] = {"handle"};
const char* const kParams_431[] = {"handle", "index", "pDetails"};
const char* const kParams_432[] = {"handle", "index", "pchURL", "cchURLSize"};
const char* const kParams_433[] = {"handle", "index", "pchMetadata", "cchMetadatasize"};
const char* const kParams_434[] = {"handle", "index", "pvecPublishedFileID", "cMaxEntries"};
const char* const kParams_435[] = {"handle", "index", "eStatType", "pStatValue"};
const char* const kParams_436[] = {"handle", "index"};
const char* const kParams_437[] = {
    "handle",
    "index",
    "previewIndex",
    "pchURLOrVideoID",
    "cchURLSize",
    "pchOriginalFileName",
    "cchOriginalFileNameSize",
    "pPreviewType",
};
const char* const kParams_438[] = {
    "handle",
    "index",
    "keyValueTagIndex",
    "pchKey",
    "cchKeySize",
    "pchValue",
    "cchValueSize",
};
const char* const kParams_439[] = {"handle", "pTagName"};
const char* const kParams_440[] = {"handle", "bReturnOnlyIDs"};
const char* const kParams_441[] = {"handle", "bReturnKeyValueTags"};
const char* const kParams_442[] = {"handle", "bReturnLongDescription"};
const char* const kParams_443[] = {"handle", "bReturnMetadata"};
const char* const kParams_444[] = {"handle", "bReturnChildren"};
const char* const kParams_445[] = {"handle", "bReturnAdditionalPreviews"};
const char* const kParams_446[] = {"handle", "bReturnTotalOnly"};
const char* const kParams_447[] = {"handle", "unDays"};
const char* const kParams_448[] = {"handle", "pchLanguage"};
const char* const kParams_449[] = {"handle", "unMaxAgeSeconds"};
const char* const kParams_450[] = {"handle", "pMatchCloudFileName"};
const char* const kParams_451[] = {"handle", "bMatchAnyTag"};
const char* const kParams_452[] = {"handle", "pSearchText"};
const char* const kParams_453[] = {"handle", "pKey", "pValue"};
const char* const kParams_454[] = {"nPublishedFileID", "unMaxAgeSeconds"};
const char* const kParams_455[] = {"nConsumerAppId", "eFileType"};
const char* const kParams_456[] = {"nConsumerAppId", "nPublishedFileID"};
const char* const kParams_457[] = {"handle", "pchTitle"};
const char* const kParams_458[] = {"handle", "pchDescription"};
const char* const kParams_459[] = {"handle", "pchMetaData"};
const char* const kParams_460[] = {"handle", "eVisibility"};
const char* const kParams_461[] = {"handle", "pszContentFolder"};
const char* const kParams_462[] = {"handle", "pszPreviewFile"};
const char* const kParams_463[] = {"handle", "pchKey"};
const char* const kParams_464[] = {"handle", "pchKey", "pchValue"};
const char* const kParams_465[] = {"handle", "pszPreviewFile", "type"};
const char* const kParams_466[] = {"handle", "pszVideoID"};
const char* const kParams_467[] = {"handle", "index", "pszPreviewFile"};
const char* const kParams_468[] = {"handle", "index", "pszVideoID"};
const char* const kParams_469[] = {"handle", "pchChangeNote"};
const char* const kParams_470[] = {"handle", "punBytesProcessed", "punBytesTotal"};
const char* const kParams_471[] = {"nPublishedFileID", "bVoteUp"};
const char* const kParams_472[] = {"nPublishedFileID"};
const char* const kParams_473[] = {"nAppId", "nPublishedFileID"};
const char* const kParams_474[] = {"pvecPublishedFileID", "cMaxEntries"};
const char* const kParams_475[] = {
    "nPublishedFileID",
    "punSizeOnDisk",
    "pchFolder",
    "cchFolderSize",
    "punTimeStamp",
};
const char* const kParams_476[] = {"nPublishedFileID", "punBytesDownloaded", "punBytesTotal"};
const char* const kParams_477[] = {"nPublishedFileID", "bHighPriority"};
const char* const kParams_478[] = {"unWorkshopDepotID", "pszFolder"};
const char* const kParams_479[] = {"bSuspend"};
const char* const kParams_480[] = {"nParentPublishedFileID", "nChildPublishedFileID"};
const char* const kParams_481[] = {"nPublishedFileID", "nAppID"};
const char* const kParams_482[] = {
    "eQueryType",
    "eMatchingeMatchingUGCTypeFileType",
    "nCreatorAppID",
    "nConsumerAppID",
    "pchCursor",
};
const char* const kParams_483[] = {"handle", "index", "pchKey", "pchValue", "cchValueSize"};
const char* const kParams_484[] = {"handle", "bAllowLegacyUpload"};
const char* const kParams_485[] = {"handle", "pTagGroups"};
const char* const kParams_486[] = {"handle", "index", "indexTag", "pchValue", "cchValueSize"};
const char* const kParams_487[] = {"handle", "rtStart", "rtEnd"};
const char* const kParams_488[] = {"handle", "index", "pvecDescriptors", "cMaxEntries"};
const char* const kParams_489[] = {"handle", "descid"};
const char* const kParams_490[] = {
    "pchServiceMethod",
    "pRequestBuffer",
    "unRequestBufferSize",
    "unContext",
};
const char* const kParams_491[] = {"hHandle", "punResponseSize", "peResult"};
const char* const kParams_492[] = {
    "hHandle",
    "pResponseBuffer",
    "unResponseBufferSize",
    "bAutoRelease",
};
const char* const kParams_493[] = {"hHandle"};
const char* const kParams_494[] = {
    "pchServiceNotification",
    "pNotificationBuffer",
    "unNotificationBufferSize",
};
const char* const kParams_495[] = {
    "pAuthBlob",
    "cbMaxAuthBlob",
    "steamIDGameServer",
    "unIPServer",
    "usPortServer",
    "bSecure",
};
const char* const kParams_496[] = {"unIPServer", "usPortServer"};
const char* const kParams_497[] = {"gameID", "eAppUsageEvent", "pchExtraInfo"};
const char* const kParams_498[] = {"pchBuffer", "cubBuffer"};
const char* const kParams_499[] = {
    "pcbCompressed",
    "pcbUncompressed_Deprecated",
    "nUncompressedVoiceDesiredSampleRate_Deprecated",
};
const char* const kParams_500[] = {
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
const char* const kParams_501[] = {
    "pCompressed",
    "cbCompressed",
    "pDestBuffer",
    "cbDestBufferSize",
    "nBytesWritten",
    "nDesiredSampleRate",
};
const char* const kParams_502[] = {"steamIDGameServer", "unIPServer", "usPortServer"};
const char* const kParams_503[] = {"pDataToInclude", "cbDataToInclude"};
const char* const kParams_504[] = {"nSeries", "bFoil"};
const char* const kParams_505[] = {"pchRedirectURL"};
const char* const kParams_506[] = {"eNewState"};
const char* const kParams_507[] = {
    "pTicket",
    "cbMaxTicket",
    "pcbTicket",
    "pSteamNetworkingIdentity",
};
const char* const kParams_508[] = {"pchIdentity"};
const char* const kParams_509[] = {"pchName", "pData"};
const char* const kParams_510[] = {"pchName", "nData"};
const char* const kParams_511[] = {"pchName", "fData"};
const char* const kParams_512[] = {"pchName", "flCountThisSession", "dSessionLength"};
const char* const kParams_513[] = {"pchName", "pbAchieved"};
const char* const kParams_514[] = {"pchName", "pbAchieved", "punUnlockTime"};
const char* const kParams_515[] = {"pchName", "pchKey"};
const char* const kParams_516[] = {"pchName", "nCurProgress", "nMaxProgress"};
const char* const kParams_517[] = {"iAchievement"};
const char* const kParams_518[] = {"steamIDUser", "pchName", "pbAchieved", "punUnlockTime"};
const char* const kParams_519[] = {"bAchievementsToo"};
const char* const kParams_520[] = {
    "pchLeaderboardName",
    "eLeaderboardSortMethod",
    "eLeaderboardDisplayType",
};
const char* const kParams_521[] = {"pchLeaderboardName"};
const char* const kParams_522[] = {"hSteamLeaderboard"};
const char* const kParams_523[] = {
    "hSteamLeaderboard",
    "eLeaderboardDataRequest",
    "nRangeStart",
    "nRangeEnd",
};
const char* const kParams_524[] = {"hSteamLeaderboard", "prgUsers", "cUsers"};
const char* const kParams_525[] = {
    "hSteamLeaderboardEntries",
    "index",
    "pLeaderboardEntry",
    "pDetails",
    "cDetailsMax",
};
const char* const kParams_526[] = {
    "hSteamLeaderboard",
    "eLeaderboardUploadScoreMethod",
    "nScore",
    "pScoreDetails",
    "cScoreDetailsCount",
};
const char* const kParams_527[] = {"hSteamLeaderboard", "hUGC"};
const char* const kParams_528[] = {"pchName", "unNameBufLen", "pflPercent", "pbAchieved"};
const char* const kParams_529[] = {
    "iIteratorPrevious",
    "pchName",
    "unNameBufLen",
    "pflPercent",
    "pbAchieved",
};
const char* const kParams_530[] = {"pchName", "pflPercent"};
const char* const kParams_531[] = {"nHistoryDays"};
const char* const kParams_532[] = {"pchStatName", "pData"};
const char* const kParams_533[] = {"pchStatName", "pData", "cubData"};
const char* const kParams_534[] = {"pchName", "pnMinProgress", "pnMaxProgress"};
const char* const kParams_535[] = {"pchName", "pfMinProgress", "pfMaxProgress"};
const char* const kParams_536[] = {"iImage", "pnWidth", "pnHeight"};
const char* const kParams_537[] = {"iImage", "pubDest", "nDestBufferSize"};
const char* const kParams_538[] = {"eNotificationPosition"};
const char* const kParams_539[] = {"hSteamAPICall", "pbFailed"};
const char* const kParams_540[] = {"hSteamAPICall"};
const char* const kParams_541[] = {
    "hSteamAPICall",
    "pCallback",
    "cubCallback",
    "iCallbackExpected",
    "pbFailed",
};
const char* const kParams_542[] = {"szFileName"};
const char* const kParams_543[] = {
    "eInputMode",
    "eLineInputMode",
    "pchDescription",
    "unCharMax",
    "pchExistingText",
};
const char* const kParams_544[] = {"pchText", "cchText"};
const char* const kParams_545[] = {"nHorizontalInset", "nVerticalInset"};
const char* const kParams_546[] = {
    "pchOutFilteredText",
    "nByteSizeOutFilteredText",
    "pchInputMessage",
    "bLegalOnly",
};
const char* const kParams_547[] = {"eProtocol"};
const char* const kParams_548[] = {"unFilterOptions"};
const char* const kParams_549[] = {
    "eContext",
    "sourceSteamID",
    "pchInputMessage",
    "pchOutFilteredText",
    "nByteSizeOutFilteredText",
};
const char* const kParams_550[] = {
    "eKeyboardMode",
    "nTextFieldXPosition",
    "nTextFieldYPosition",
    "nTextFieldWidth",
    "nTextFieldHeight",
};
const char* const kParams_551[] = {"bLauncherMode"};
const char* const kParams_552[] = {"unVideoAppID"};
const char* const kParams_553[] = {"pnNumViewers"};
const char* const kParams_554[] = {"unVideoAppID", "pchBuffer", "pnBufferSize"};

// ---------------------------------------------------------------------------
//  The calls themselves.
// ---------------------------------------------------------------------------
//  One entry per distinct call: the name it travels under, and the argument
//  names it carries. Two versions that declare the same call with the same
//  arguments are the same call, and each slot body below names one of these -
//  which is the whole of what a slot knows about itself.

const steammock::SlotInfo kCall_0 = {"SteamAPI_ISteamAppList_GetNumInstalledApps", nullptr};
const steammock::SlotInfo kCall_1 = {"SteamAPI_ISteamAppList_GetInstalledApps", kParams_0};
const steammock::SlotInfo kCall_2 = {"SteamAPI_ISteamAppList_GetAppName", kParams_1};
const steammock::SlotInfo kCall_3 = {"SteamAPI_ISteamAppList_GetAppInstallDir", kParams_2};
const steammock::SlotInfo kCall_4 = {"SteamAPI_ISteamAppList_GetAppBuildId", kParams_3};
const steammock::SlotInfo kCall_5 = {"SteamAPI_ISteamApps_BIsSubscribed", nullptr};
const steammock::SlotInfo kCall_6 = {"SteamAPI_ISteamApps_BIsLowViolence", nullptr};
const steammock::SlotInfo kCall_7 = {"SteamAPI_ISteamApps_BIsCybercafe", nullptr};
const steammock::SlotInfo kCall_8 = {"SteamAPI_ISteamApps_BIsVACBanned", nullptr};
const steammock::SlotInfo kCall_9 = {"SteamAPI_ISteamApps_GetCurrentGameLanguage", nullptr};
const steammock::SlotInfo kCall_10 = {"SteamAPI_ISteamApps_GetAvailableGameLanguages", nullptr};
const steammock::SlotInfo kCall_11 = {"SteamAPI_ISteamApps_BIsSubscribedApp", kParams_4};
const steammock::SlotInfo kCall_12 = {"SteamAPI_ISteamApps_BIsDlcInstalled", kParams_4};
const steammock::SlotInfo kCall_13 = {"SteamAPI_ISteamApps_GetEarliestPurchaseUnixTime", kParams_3};
const steammock::SlotInfo kCall_14 = {"SteamAPI_ISteamApps_BIsSubscribedFromFreeWeekend", nullptr};
const steammock::SlotInfo kCall_15 = {"SteamAPI_ISteamApps_GetDLCCount", nullptr};
const steammock::SlotInfo kCall_16 = {"SteamAPI_ISteamApps_BGetDLCDataByIndex", kParams_5};
const steammock::SlotInfo kCall_17 = {"SteamAPI_ISteamApps_InstallDLC", kParams_3};
const steammock::SlotInfo kCall_18 = {"SteamAPI_ISteamApps_UninstallDLC", kParams_3};
const steammock::SlotInfo kCall_19 = {"SteamAPI_ISteamApps_RequestAppProofOfPurchaseKey", kParams_3};
const steammock::SlotInfo kCall_20 = {"SteamAPI_ISteamApps_GetCurrentBetaName", kParams_6};
const steammock::SlotInfo kCall_21 = {"SteamAPI_ISteamApps_MarkContentCorrupt", kParams_7};
const steammock::SlotInfo kCall_22 = {"SteamAPI_ISteamApps_GetInstalledDepots", kParams_8};
const steammock::SlotInfo kCall_23 = {"SteamAPI_ISteamApps_GetAppInstallDir", kParams_9};
const steammock::SlotInfo kCall_24 = {"SteamAPI_ISteamApps_BIsAppInstalled", kParams_4};
const steammock::SlotInfo kCall_25 = {"SteamAPI_ISteamApps_GetAppOwner", nullptr};
const steammock::SlotInfo kCall_26 = {"SteamAPI_ISteamApps_GetLaunchQueryParam", kParams_10};
const steammock::SlotInfo kCall_27 = {"SteamAPI_ISteamApps_GetDlcDownloadProgress", kParams_11};
const steammock::SlotInfo kCall_28 = {"SteamAPI_ISteamApps_GetAppBuildId", nullptr};
const steammock::SlotInfo kCall_29 = {"SteamAPI_ISteamApps_RequestAllProofOfPurchaseKeys", nullptr};
const steammock::SlotInfo kCall_30 = {"SteamAPI_ISteamApps_GetFileDetails", kParams_12};
const steammock::SlotInfo kCall_31 = {"SteamAPI_ISteamApps_GetLaunchCommandLine", kParams_13};
const steammock::SlotInfo kCall_32 = {"SteamAPI_ISteamApps_BIsSubscribedFromFamilySharing", nullptr};
const steammock::SlotInfo kCall_33 = {"SteamAPI_ISteamApps_BIsTimedTrial", kParams_14};
const steammock::SlotInfo kCall_34 = {"SteamAPI_ISteamApps_SetDlcContext", kParams_3};
const steammock::SlotInfo kCall_35 = {"SteamAPI_ISteamClient_CreateSteamPipe", nullptr};
const steammock::SlotInfo kCall_36 = {"SteamAPI_ISteamClient_BReleaseSteamPipe", kParams_15};
const steammock::SlotInfo kCall_37 = {"SteamAPI_ISteamClient_ConnectToGlobalUser", kParams_15};
const steammock::SlotInfo kCall_38 = {"SteamAPI_ISteamClient_CreateLocalUser", kParams_16};
const steammock::SlotInfo kCall_39 = {"SteamAPI_ISteamClient_ReleaseUser", kParams_17};
const steammock::SlotInfo kCall_40 = {"SteamAPI_ISteamClient_GetISteamUser", kParams_18};
const steammock::SlotInfo kCall_41 = {"SteamAPI_ISteamClient_GetISteamGameServer", kParams_18};
const steammock::SlotInfo kCall_42 = {"SteamAPI_ISteamClient_SetLocalIPBinding", kParams_19};
const steammock::SlotInfo kCall_43 = {"SteamAPI_ISteamClient_GetISteamFriends", kParams_18};
const steammock::SlotInfo kCall_44 = {"SteamAPI_ISteamClient_GetISteamUtils", kParams_20};
const steammock::SlotInfo kCall_45 = {"SteamAPI_ISteamClient_GetISteamMatchmaking", kParams_18};
const steammock::SlotInfo kCall_46 = {"SteamAPI_ISteamClient_GetISteamMatchmakingServers", kParams_18};
const steammock::SlotInfo kCall_47 = {"SteamAPI_ISteamClient_GetISteamGenericInterface", kParams_18};
const steammock::SlotInfo kCall_48 = {"SteamAPI_ISteamClient_GetISteamUserStats", kParams_18};
const steammock::SlotInfo kCall_49 = {"SteamAPI_ISteamClient_GetISteamGameServerStats", kParams_21};
const steammock::SlotInfo kCall_50 = {"SteamAPI_ISteamClient_GetISteamApps", kParams_18};
const steammock::SlotInfo kCall_51 = {"SteamAPI_ISteamClient_GetISteamNetworking", kParams_18};
const steammock::SlotInfo kCall_52 = {"SteamAPI_ISteamClient_GetISteamRemoteStorage", kParams_21};
const steammock::SlotInfo kCall_53 = {"SteamAPI_ISteamClient_GetISteamScreenshots", kParams_21};
const steammock::SlotInfo kCall_54 = {"ISteamClient::RunFrame", nullptr};
const steammock::SlotInfo kCall_55 = {"SteamAPI_ISteamClient_GetIPCCallCount", nullptr};
const steammock::SlotInfo kCall_56 = {"SteamAPI_ISteamClient_SetWarningMessageHook", kParams_22};
const steammock::SlotInfo kCall_57 = {"SteamAPI_ISteamClient_BShutdownIfAllPipesClosed", nullptr};
const steammock::SlotInfo kCall_58 = {"SteamAPI_ISteamClient_GetISteamHTTP", kParams_21};
const steammock::SlotInfo kCall_59 = {"SteamAPI_ISteamClient_GetISteamUnifiedMessages", kParams_21};
const steammock::SlotInfo kCall_60 = {"SteamAPI_ISteamClient_GetISteamController", kParams_18};
const steammock::SlotInfo kCall_61 = {"SteamAPI_ISteamClient_GetISteamUGC", kParams_18};
const steammock::SlotInfo kCall_62 = {"SteamAPI_ISteamClient_GetISteamAppList", kParams_18};
const steammock::SlotInfo kCall_63 = {"SteamAPI_ISteamClient_GetISteamMusic", kParams_21};
const steammock::SlotInfo kCall_64 = {"SteamAPI_ISteamClient_GetISteamMusicRemote", kParams_21};
const steammock::SlotInfo kCall_65 = {"SteamAPI_ISteamClient_GetISteamHTMLSurface", kParams_21};
const steammock::SlotInfo kCall_66 = {"ISteamClient::DEPRECATED_Set_SteamAPI_CPostAPIResultInProcess", kParams_23};
const steammock::SlotInfo kCall_67 = {"ISteamClient::DEPRECATED_Remove_SteamAPI_CPostAPIResultInProcess", kParams_23};
const steammock::SlotInfo kCall_68 = {"ISteamClient::Set_SteamAPI_CCheckCallbackRegisteredInProcess", kParams_24};
const steammock::SlotInfo kCall_69 = {"SteamAPI_ISteamClient_GetISteamInventory", kParams_21};
const steammock::SlotInfo kCall_70 = {"SteamAPI_ISteamClient_GetISteamVideo", kParams_21};
const steammock::SlotInfo kCall_71 = {"SteamAPI_ISteamClient_GetISteamParentalSettings", kParams_21};
const steammock::SlotInfo kCall_72 = {"SteamAPI_ISteamClient_GetISteamGameSearch", kParams_21};
const steammock::SlotInfo kCall_73 = {"ISteamClient::DEPRECATED_GetISteamUnifiedMessages", kParams_21};
const steammock::SlotInfo kCall_74 = {"SteamAPI_ISteamClient_GetISteamInput", kParams_18};
const steammock::SlotInfo kCall_75 = {"SteamAPI_ISteamClient_GetISteamParties", kParams_18};
const steammock::SlotInfo kCall_76 = {"SteamAPI_ISteamClient_GetISteamRemotePlay", kParams_18};
const steammock::SlotInfo kCall_77 = {"ISteamClient::DestroyAllInterfaces", nullptr};
const steammock::SlotInfo kCall_78 = {"SteamAPI_ISteamController_Init", nullptr};
const steammock::SlotInfo kCall_79 = {"SteamAPI_ISteamController_Shutdown", nullptr};
const steammock::SlotInfo kCall_80 = {"SteamAPI_ISteamController_RunFrame", nullptr};
const steammock::SlotInfo kCall_81 = {"SteamAPI_ISteamController_GetConnectedControllers", kParams_25};
const steammock::SlotInfo kCall_82 = {"SteamAPI_ISteamController_ShowBindingPanel", kParams_26};
const steammock::SlotInfo kCall_83 = {"SteamAPI_ISteamController_GetActionSetHandle", kParams_27};
const steammock::SlotInfo kCall_84 = {"SteamAPI_ISteamController_ActivateActionSet", kParams_28};
const steammock::SlotInfo kCall_85 = {"SteamAPI_ISteamController_GetCurrentActionSet", kParams_26};
const steammock::SlotInfo kCall_86 = {"SteamAPI_ISteamController_GetDigitalActionHandle", kParams_29};
const steammock::SlotInfo kCall_87 = {"SteamAPI_ISteamController_GetDigitalActionData", kParams_30};
const steammock::SlotInfo kCall_88 = {"SteamAPI_ISteamController_GetDigitalActionOrigins", kParams_31};
const steammock::SlotInfo kCall_89 = {"SteamAPI_ISteamController_GetAnalogActionHandle", kParams_29};
const steammock::SlotInfo kCall_90 = {"SteamAPI_ISteamController_GetAnalogActionData", kParams_32};
const steammock::SlotInfo kCall_91 = {"SteamAPI_ISteamController_GetAnalogActionOrigins", kParams_33};
const steammock::SlotInfo kCall_92 = {"SteamAPI_ISteamController_StopAnalogActionMomentum", kParams_34};
const steammock::SlotInfo kCall_93 = {"SteamAPI_ISteamController_TriggerHapticPulse", kParams_35};
const steammock::SlotInfo kCall_94 = {"SteamAPI_ISteamController_TriggerRepeatedHapticPulse", kParams_36};
const steammock::SlotInfo kCall_95 = {"SteamAPI_ISteamController_TriggerVibration", kParams_37};
const steammock::SlotInfo kCall_96 = {"SteamAPI_ISteamController_SetLEDColor", kParams_38};
const steammock::SlotInfo kCall_97 = {"SteamAPI_ISteamController_GetGamepadIndexForController", kParams_39};
const steammock::SlotInfo kCall_98 = {"SteamAPI_ISteamController_GetControllerForGamepadIndex", kParams_40};
const steammock::SlotInfo kCall_99 = {"SteamAPI_ISteamController_GetMotionData", kParams_26};
const steammock::SlotInfo kCall_100 = {"SteamAPI_ISteamController_ShowDigitalActionOrigins", kParams_41};
const steammock::SlotInfo kCall_101 = {"SteamAPI_ISteamController_ShowAnalogActionOrigins", kParams_42};
const steammock::SlotInfo kCall_102 = {"SteamAPI_ISteamController_GetStringForActionOrigin", kParams_43};
const steammock::SlotInfo kCall_103 = {"SteamAPI_ISteamController_GetGlyphForActionOrigin", kParams_43};
const steammock::SlotInfo kCall_104 = {"SteamAPI_ISteamController_ActivateActionSetLayer", kParams_44};
const steammock::SlotInfo kCall_105 = {"SteamAPI_ISteamController_DeactivateActionSetLayer", kParams_44};
const steammock::SlotInfo kCall_106 = {"SteamAPI_ISteamController_DeactivateAllActionSetLayers", kParams_26};
const steammock::SlotInfo kCall_107 = {"SteamAPI_ISteamController_GetActiveActionSetLayers", kParams_45};
const steammock::SlotInfo kCall_108 = {"SteamAPI_ISteamController_GetInputTypeForHandle", kParams_26};
const steammock::SlotInfo kCall_109 = {"SteamAPI_ISteamController_GetStringForXboxOrigin", kParams_43};
const steammock::SlotInfo kCall_110 = {"SteamAPI_ISteamController_GetGlyphForXboxOrigin", kParams_43};
const steammock::SlotInfo kCall_111 = {"SteamAPI_ISteamController_GetActionOriginFromXboxOrigin", kParams_46};
const steammock::SlotInfo kCall_112 = {"SteamAPI_ISteamController_TranslateActionOrigin", kParams_47};
const steammock::SlotInfo kCall_113 = {"SteamAPI_ISteamController_GetControllerBindingRevision", kParams_48};
const steammock::SlotInfo kCall_114 = {"SteamAPI_ISteamFriends_GetPersonaName", nullptr};
const steammock::SlotInfo kCall_115 = {"SteamAPI_ISteamFriends_SetPersonaName", kParams_49};
const steammock::SlotInfo kCall_116 = {"SteamAPI_ISteamFriends_GetPersonaState", nullptr};
const steammock::SlotInfo kCall_117 = {"SteamAPI_ISteamFriends_GetFriendCount", kParams_50};
const steammock::SlotInfo kCall_118 = {"SteamAPI_ISteamFriends_GetFriendByIndex", kParams_51};
const steammock::SlotInfo kCall_119 = {"SteamAPI_ISteamFriends_GetFriendRelationship", kParams_52};
const steammock::SlotInfo kCall_120 = {"SteamAPI_ISteamFriends_GetFriendPersonaState", kParams_52};
const steammock::SlotInfo kCall_121 = {"SteamAPI_ISteamFriends_GetFriendPersonaName", kParams_52};
const steammock::SlotInfo kCall_122 = {"SteamAPI_ISteamFriends_GetFriendGamePlayed", kParams_53};
const steammock::SlotInfo kCall_123 = {"SteamAPI_ISteamFriends_GetFriendPersonaNameHistory", kParams_54};
const steammock::SlotInfo kCall_124 = {"SteamAPI_ISteamFriends_GetFriendSteamLevel", kParams_52};
const steammock::SlotInfo kCall_125 = {"SteamAPI_ISteamFriends_GetPlayerNickname", kParams_55};
const steammock::SlotInfo kCall_126 = {"SteamAPI_ISteamFriends_GetFriendsGroupCount", nullptr};
const steammock::SlotInfo kCall_127 = {"SteamAPI_ISteamFriends_GetFriendsGroupIDByIndex", kParams_56};
const steammock::SlotInfo kCall_128 = {"SteamAPI_ISteamFriends_GetFriendsGroupName", kParams_57};
const steammock::SlotInfo kCall_129 = {"SteamAPI_ISteamFriends_GetFriendsGroupMembersCount", kParams_57};
const steammock::SlotInfo kCall_130 = {"SteamAPI_ISteamFriends_GetFriendsGroupMembersList", kParams_58};
const steammock::SlotInfo kCall_131 = {"SteamAPI_ISteamFriends_HasFriend", kParams_59};
const steammock::SlotInfo kCall_132 = {"SteamAPI_ISteamFriends_GetClanCount", nullptr};
const steammock::SlotInfo kCall_133 = {"SteamAPI_ISteamFriends_GetClanByIndex", kParams_60};
const steammock::SlotInfo kCall_134 = {"SteamAPI_ISteamFriends_GetClanName", kParams_61};
const steammock::SlotInfo kCall_135 = {"SteamAPI_ISteamFriends_GetClanTag", kParams_61};
const steammock::SlotInfo kCall_136 = {"SteamAPI_ISteamFriends_GetClanActivityCounts", kParams_62};
const steammock::SlotInfo kCall_137 = {"SteamAPI_ISteamFriends_DownloadClanActivityCounts", kParams_63};
const steammock::SlotInfo kCall_138 = {"SteamAPI_ISteamFriends_GetFriendCountFromSource", kParams_64};
const steammock::SlotInfo kCall_139 = {"SteamAPI_ISteamFriends_GetFriendFromSourceByIndex", kParams_65};
const steammock::SlotInfo kCall_140 = {"SteamAPI_ISteamFriends_IsUserInSource", kParams_66};
const steammock::SlotInfo kCall_141 = {"SteamAPI_ISteamFriends_SetInGameVoiceSpeaking", kParams_67};
const steammock::SlotInfo kCall_142 = {"SteamAPI_ISteamFriends_ActivateGameOverlay", kParams_68};
const steammock::SlotInfo kCall_143 = {"SteamAPI_ISteamFriends_ActivateGameOverlayToUser", kParams_69};
const steammock::SlotInfo kCall_144 = {"SteamAPI_ISteamFriends_ActivateGameOverlayToWebPage", kParams_70};
const steammock::SlotInfo kCall_145 = {"SteamAPI_ISteamFriends_ActivateGameOverlayToStore", kParams_71};
const steammock::SlotInfo kCall_146 = {"SteamAPI_ISteamFriends_SetPlayedWith", kParams_72};
const steammock::SlotInfo kCall_147 = {"SteamAPI_ISteamFriends_ActivateGameOverlayInviteDialog", kParams_73};
const steammock::SlotInfo kCall_148 = {"SteamAPI_ISteamFriends_GetSmallFriendAvatar", kParams_52};
const steammock::SlotInfo kCall_149 = {"SteamAPI_ISteamFriends_GetMediumFriendAvatar", kParams_52};
const steammock::SlotInfo kCall_150 = {"SteamAPI_ISteamFriends_GetLargeFriendAvatar", kParams_52};
const steammock::SlotInfo kCall_151 = {"SteamAPI_ISteamFriends_RequestUserInformation", kParams_74};
const steammock::SlotInfo kCall_152 = {"SteamAPI_ISteamFriends_RequestClanOfficerList", kParams_61};
const steammock::SlotInfo kCall_153 = {"SteamAPI_ISteamFriends_GetClanOwner", kParams_61};
const steammock::SlotInfo kCall_154 = {"SteamAPI_ISteamFriends_GetClanOfficerCount", kParams_61};
const steammock::SlotInfo kCall_155 = {"SteamAPI_ISteamFriends_GetClanOfficerByIndex", kParams_75};
const steammock::SlotInfo kCall_156 = {"SteamAPI_ISteamFriends_GetUserRestrictions", nullptr};
const steammock::SlotInfo kCall_157 = {"SteamAPI_ISteamFriends_SetRichPresence", kParams_76};
const steammock::SlotInfo kCall_158 = {"SteamAPI_ISteamFriends_ClearRichPresence", nullptr};
const steammock::SlotInfo kCall_159 = {"SteamAPI_ISteamFriends_GetFriendRichPresence", kParams_77};
const steammock::SlotInfo kCall_160 = {"SteamAPI_ISteamFriends_GetFriendRichPresenceKeyCount", kParams_52};
const steammock::SlotInfo kCall_161 = {"SteamAPI_ISteamFriends_GetFriendRichPresenceKeyByIndex", kParams_78};
const steammock::SlotInfo kCall_162 = {"SteamAPI_ISteamFriends_RequestFriendRichPresence", kParams_52};
const steammock::SlotInfo kCall_163 = {"SteamAPI_ISteamFriends_InviteUserToGame", kParams_79};
const steammock::SlotInfo kCall_164 = {"SteamAPI_ISteamFriends_GetCoplayFriendCount", nullptr};
const steammock::SlotInfo kCall_165 = {"SteamAPI_ISteamFriends_GetCoplayFriend", kParams_80};
const steammock::SlotInfo kCall_166 = {"SteamAPI_ISteamFriends_GetFriendCoplayTime", kParams_52};
const steammock::SlotInfo kCall_167 = {"SteamAPI_ISteamFriends_GetFriendCoplayGame", kParams_52};
const steammock::SlotInfo kCall_168 = {"SteamAPI_ISteamFriends_JoinClanChatRoom", kParams_61};
const steammock::SlotInfo kCall_169 = {"SteamAPI_ISteamFriends_LeaveClanChatRoom", kParams_61};
const steammock::SlotInfo kCall_170 = {"SteamAPI_ISteamFriends_GetClanChatMemberCount", kParams_61};
const steammock::SlotInfo kCall_171 = {"SteamAPI_ISteamFriends_GetChatMemberByIndex", kParams_81};
const steammock::SlotInfo kCall_172 = {"SteamAPI_ISteamFriends_SendClanChatMessage", kParams_82};
const steammock::SlotInfo kCall_173 = {"SteamAPI_ISteamFriends_GetClanChatMessage", kParams_83};
const steammock::SlotInfo kCall_174 = {"SteamAPI_ISteamFriends_IsClanChatAdmin", kParams_84};
const steammock::SlotInfo kCall_175 = {"SteamAPI_ISteamFriends_IsClanChatWindowOpenInSteam", kParams_85};
const steammock::SlotInfo kCall_176 = {"SteamAPI_ISteamFriends_OpenClanChatWindowInSteam", kParams_85};
const steammock::SlotInfo kCall_177 = {"SteamAPI_ISteamFriends_CloseClanChatWindowInSteam", kParams_85};
const steammock::SlotInfo kCall_178 = {"SteamAPI_ISteamFriends_SetListenForFriendsMessages", kParams_86};
const steammock::SlotInfo kCall_179 = {"SteamAPI_ISteamFriends_ReplyToFriendMessage", kParams_87};
const steammock::SlotInfo kCall_180 = {"SteamAPI_ISteamFriends_GetFriendMessage", kParams_88};
const steammock::SlotInfo kCall_181 = {"SteamAPI_ISteamFriends_GetFollowerCount", kParams_89};
const steammock::SlotInfo kCall_182 = {"SteamAPI_ISteamFriends_IsFollowing", kParams_89};
const steammock::SlotInfo kCall_183 = {"SteamAPI_ISteamFriends_EnumerateFollowingList", kParams_90};
const steammock::SlotInfo kCall_184 = {"SteamAPI_ISteamFriends_ActivateGameOverlayToWebPage", kParams_91};
const steammock::SlotInfo kCall_185 = {"SteamAPI_ISteamFriends_IsClanPublic", kParams_61};
const steammock::SlotInfo kCall_186 = {"SteamAPI_ISteamFriends_IsClanOfficialGameGroup", kParams_61};
const steammock::SlotInfo kCall_187 = {"SteamAPI_ISteamFriends_GetNumChatsWithUnreadPriorityMessages", nullptr};
const steammock::SlotInfo kCall_188 = {"SteamAPI_ISteamFriends_ActivateGameOverlayRemotePlayTogetherInviteDialog", kParams_73};
const steammock::SlotInfo kCall_189 = {"SteamAPI_ISteamFriends_RegisterProtocolInOverlayBrowser", kParams_92};
const steammock::SlotInfo kCall_190 = {"SteamAPI_ISteamFriends_ActivateGameOverlayInviteDialogConnectString", kParams_93};
const steammock::SlotInfo kCall_191 = {"SteamAPI_ISteamFriends_RequestEquippedProfileItems", kParams_89};
const steammock::SlotInfo kCall_192 = {"SteamAPI_ISteamFriends_BHasEquippedProfileItem", kParams_94};
const steammock::SlotInfo kCall_193 = {"SteamAPI_ISteamFriends_GetProfileItemPropertyString", kParams_95};
const steammock::SlotInfo kCall_194 = {"SteamAPI_ISteamFriends_GetProfileItemPropertyUint", kParams_95};
const steammock::SlotInfo kCall_195 = {"SteamAPI_ISteamGameSearch_AddGameSearchParams", kParams_96};
const steammock::SlotInfo kCall_196 = {"SteamAPI_ISteamGameSearch_SearchForGameWithLobby", kParams_97};
const steammock::SlotInfo kCall_197 = {"SteamAPI_ISteamGameSearch_SearchForGameSolo", kParams_98};
const steammock::SlotInfo kCall_198 = {"SteamAPI_ISteamGameSearch_AcceptGame", nullptr};
const steammock::SlotInfo kCall_199 = {"SteamAPI_ISteamGameSearch_DeclineGame", nullptr};
const steammock::SlotInfo kCall_200 = {"SteamAPI_ISteamGameSearch_RetrieveConnectionDetails", kParams_99};
const steammock::SlotInfo kCall_201 = {"SteamAPI_ISteamGameSearch_EndGameSearch", nullptr};
const steammock::SlotInfo kCall_202 = {"SteamAPI_ISteamGameSearch_SetGameHostParams", kParams_76};
const steammock::SlotInfo kCall_203 = {"SteamAPI_ISteamGameSearch_SetConnectionDetails", kParams_100};
const steammock::SlotInfo kCall_204 = {"SteamAPI_ISteamGameSearch_RequestPlayersForGame", kParams_101};
const steammock::SlotInfo kCall_205 = {"SteamAPI_ISteamGameSearch_HostConfirmGameStart", kParams_102};
const steammock::SlotInfo kCall_206 = {"SteamAPI_ISteamGameSearch_CancelRequestPlayersForGame", nullptr};
const steammock::SlotInfo kCall_207 = {"SteamAPI_ISteamGameSearch_SubmitPlayerResult", kParams_103};
const steammock::SlotInfo kCall_208 = {"SteamAPI_ISteamGameSearch_EndGame", kParams_102};
const steammock::SlotInfo kCall_209 = {"SteamAPI_ISteamGameServer_InitGameServer", kParams_104};
const steammock::SlotInfo kCall_210 = {"SteamAPI_ISteamGameServer_SetProduct", kParams_105};
const steammock::SlotInfo kCall_211 = {"SteamAPI_ISteamGameServer_SetGameDescription", kParams_106};
const steammock::SlotInfo kCall_212 = {"SteamAPI_ISteamGameServer_SetModDir", kParams_107};
const steammock::SlotInfo kCall_213 = {"SteamAPI_ISteamGameServer_SetDedicatedServer", kParams_108};
const steammock::SlotInfo kCall_214 = {"SteamAPI_ISteamGameServer_LogOn", kParams_109};
const steammock::SlotInfo kCall_215 = {"SteamAPI_ISteamGameServer_LogOnAnonymous", nullptr};
const steammock::SlotInfo kCall_216 = {"SteamAPI_ISteamGameServer_LogOff", nullptr};
const steammock::SlotInfo kCall_217 = {"SteamAPI_ISteamGameServer_BLoggedOn", nullptr};
const steammock::SlotInfo kCall_218 = {"SteamAPI_ISteamGameServer_BSecure", nullptr};
const steammock::SlotInfo kCall_219 = {"SteamAPI_ISteamGameServer_GetSteamID", nullptr};
const steammock::SlotInfo kCall_220 = {"SteamAPI_ISteamGameServer_WasRestartRequested", nullptr};
const steammock::SlotInfo kCall_221 = {"SteamAPI_ISteamGameServer_SetMaxPlayerCount", kParams_110};
const steammock::SlotInfo kCall_222 = {"SteamAPI_ISteamGameServer_SetBotPlayerCount", kParams_111};
const steammock::SlotInfo kCall_223 = {"SteamAPI_ISteamGameServer_SetServerName", kParams_112};
const steammock::SlotInfo kCall_224 = {"SteamAPI_ISteamGameServer_SetMapName", kParams_113};
const steammock::SlotInfo kCall_225 = {"SteamAPI_ISteamGameServer_SetPasswordProtected", kParams_114};
const steammock::SlotInfo kCall_226 = {"SteamAPI_ISteamGameServer_SetSpectatorPort", kParams_115};
const steammock::SlotInfo kCall_227 = {"SteamAPI_ISteamGameServer_SetSpectatorServerName", kParams_116};
const steammock::SlotInfo kCall_228 = {"SteamAPI_ISteamGameServer_ClearAllKeyValues", nullptr};
const steammock::SlotInfo kCall_229 = {"SteamAPI_ISteamGameServer_SetKeyValue", kParams_117};
const steammock::SlotInfo kCall_230 = {"SteamAPI_ISteamGameServer_SetGameTags", kParams_118};
const steammock::SlotInfo kCall_231 = {"SteamAPI_ISteamGameServer_SetGameData", kParams_119};
const steammock::SlotInfo kCall_232 = {"SteamAPI_ISteamGameServer_SetRegion", kParams_120};
const steammock::SlotInfo kCall_233 = {"SteamAPI_ISteamGameServer_SendUserConnectAndAuthenticate", kParams_121};
const steammock::SlotInfo kCall_234 = {"SteamAPI_ISteamGameServer_CreateUnauthenticatedUserConnection", nullptr};
const steammock::SlotInfo kCall_235 = {"SteamAPI_ISteamGameServer_SendUserDisconnect", kParams_122};
const steammock::SlotInfo kCall_236 = {"SteamAPI_ISteamGameServer_BUpdateUserData", kParams_123};
const steammock::SlotInfo kCall_237 = {"SteamAPI_ISteamGameServer_GetAuthSessionTicket", kParams_124};
const steammock::SlotInfo kCall_238 = {"SteamAPI_ISteamGameServer_BeginAuthSession", kParams_125};
const steammock::SlotInfo kCall_239 = {"SteamAPI_ISteamGameServer_EndAuthSession", kParams_89};
const steammock::SlotInfo kCall_240 = {"SteamAPI_ISteamGameServer_CancelAuthTicket", kParams_126};
const steammock::SlotInfo kCall_241 = {"SteamAPI_ISteamGameServer_UserHasLicenseForApp", kParams_127};
const steammock::SlotInfo kCall_242 = {"SteamAPI_ISteamGameServer_RequestUserGroupStatus", kParams_128};
const steammock::SlotInfo kCall_243 = {"SteamAPI_ISteamGameServer_GetGameplayStats", nullptr};
const steammock::SlotInfo kCall_244 = {"SteamAPI_ISteamGameServer_GetServerReputation", nullptr};
const steammock::SlotInfo kCall_245 = {"SteamAPI_ISteamGameServer_GetPublicIP", nullptr};
const steammock::SlotInfo kCall_246 = {"SteamAPI_ISteamGameServer_HandleIncomingPacket", kParams_129};
const steammock::SlotInfo kCall_247 = {"SteamAPI_ISteamGameServer_GetNextOutgoingPacket", kParams_130};
const steammock::SlotInfo kCall_248 = {"SteamAPI_ISteamGameServer_EnableHeartbeats", kParams_131};
const steammock::SlotInfo kCall_249 = {"SteamAPI_ISteamGameServer_SetHeartbeatInterval", kParams_132};
const steammock::SlotInfo kCall_250 = {"SteamAPI_ISteamGameServer_ForceHeartbeat", nullptr};
const steammock::SlotInfo kCall_251 = {"SteamAPI_ISteamGameServer_AssociateWithClan", kParams_61};
const steammock::SlotInfo kCall_252 = {"SteamAPI_ISteamGameServer_ComputeNewPlayerCompatibility", kParams_133};
const steammock::SlotInfo kCall_253 = {"ISteamGameServer::InitGameServer", kParams_104};
const steammock::SlotInfo kCall_254 = {"SteamAPI_ISteamGameServer_SetAdvertiseServerActive", kParams_131};
const steammock::SlotInfo kCall_255 = {"SteamAPI_ISteamGameServer_SendUserConnectAndAuthenticate_DEPRECATED", kParams_121};
const steammock::SlotInfo kCall_256 = {"SteamAPI_ISteamGameServer_SendUserDisconnect_DEPRECATED", kParams_122};
const steammock::SlotInfo kCall_257 = {"ISteamGameServer::SetMasterServerHeartbeatInterval_DEPRECATED", kParams_132};
const steammock::SlotInfo kCall_258 = {"ISteamGameServer::ForceMasterServerHeartbeat_DEPRECATED", nullptr};
const steammock::SlotInfo kCall_259 = {"SteamAPI_ISteamGameServer_GetAuthSessionTicket", kParams_134};
const steammock::SlotInfo kCall_260 = {"SteamAPI_ISteamGameServerStats_RequestUserStats", kParams_122};
const steammock::SlotInfo kCall_261 = {"SteamAPI_ISteamGameServerStats_GetUserStatInt32", kParams_135};
const steammock::SlotInfo kCall_262 = {"SteamAPI_ISteamGameServerStats_GetUserStatFloat", kParams_135};
const steammock::SlotInfo kCall_263 = {"SteamAPI_ISteamGameServerStats_GetUserAchievement", kParams_136};
const steammock::SlotInfo kCall_264 = {"SteamAPI_ISteamGameServerStats_SetUserStatInt32", kParams_137};
const steammock::SlotInfo kCall_265 = {"SteamAPI_ISteamGameServerStats_SetUserStatFloat", kParams_138};
const steammock::SlotInfo kCall_266 = {"SteamAPI_ISteamGameServerStats_UpdateUserAvgRateStat", kParams_139};
const steammock::SlotInfo kCall_267 = {"SteamAPI_ISteamGameServerStats_SetUserAchievement", kParams_140};
const steammock::SlotInfo kCall_268 = {"SteamAPI_ISteamGameServerStats_ClearUserAchievement", kParams_140};
const steammock::SlotInfo kCall_269 = {"SteamAPI_ISteamGameServerStats_StoreUserStats", kParams_122};
const steammock::SlotInfo kCall_270 = {"SteamAPI_ISteamHTMLSurface_Init", nullptr};
const steammock::SlotInfo kCall_271 = {"SteamAPI_ISteamHTMLSurface_Shutdown", nullptr};
const steammock::SlotInfo kCall_272 = {"SteamAPI_ISteamHTMLSurface_CreateBrowser", kParams_141};
const steammock::SlotInfo kCall_273 = {"SteamAPI_ISteamHTMLSurface_RemoveBrowser", kParams_142};
const steammock::SlotInfo kCall_274 = {"SteamAPI_ISteamHTMLSurface_LoadURL", kParams_143};
const steammock::SlotInfo kCall_275 = {"SteamAPI_ISteamHTMLSurface_SetSize", kParams_144};
const steammock::SlotInfo kCall_276 = {"SteamAPI_ISteamHTMLSurface_StopLoad", kParams_142};
const steammock::SlotInfo kCall_277 = {"SteamAPI_ISteamHTMLSurface_Reload", kParams_142};
const steammock::SlotInfo kCall_278 = {"SteamAPI_ISteamHTMLSurface_GoBack", kParams_142};
const steammock::SlotInfo kCall_279 = {"SteamAPI_ISteamHTMLSurface_GoForward", kParams_142};
const steammock::SlotInfo kCall_280 = {"SteamAPI_ISteamHTMLSurface_AddHeader", kParams_145};
const steammock::SlotInfo kCall_281 = {"SteamAPI_ISteamHTMLSurface_ExecuteJavascript", kParams_146};
const steammock::SlotInfo kCall_282 = {"SteamAPI_ISteamHTMLSurface_MouseUp", kParams_147};
const steammock::SlotInfo kCall_283 = {"SteamAPI_ISteamHTMLSurface_MouseDown", kParams_147};
const steammock::SlotInfo kCall_284 = {"SteamAPI_ISteamHTMLSurface_MouseDoubleClick", kParams_147};
const steammock::SlotInfo kCall_285 = {"SteamAPI_ISteamHTMLSurface_MouseMove", kParams_148};
const steammock::SlotInfo kCall_286 = {"SteamAPI_ISteamHTMLSurface_MouseWheel", kParams_149};
const steammock::SlotInfo kCall_287 = {"SteamAPI_ISteamHTMLSurface_KeyDown", kParams_150};
const steammock::SlotInfo kCall_288 = {"SteamAPI_ISteamHTMLSurface_KeyUp", kParams_150};
const steammock::SlotInfo kCall_289 = {"SteamAPI_ISteamHTMLSurface_KeyChar", kParams_151};
const steammock::SlotInfo kCall_290 = {"SteamAPI_ISteamHTMLSurface_SetHorizontalScroll", kParams_152};
const steammock::SlotInfo kCall_291 = {"SteamAPI_ISteamHTMLSurface_SetVerticalScroll", kParams_152};
const steammock::SlotInfo kCall_292 = {"SteamAPI_ISteamHTMLSurface_SetKeyFocus", kParams_153};
const steammock::SlotInfo kCall_293 = {"SteamAPI_ISteamHTMLSurface_ViewSource", kParams_142};
const steammock::SlotInfo kCall_294 = {"SteamAPI_ISteamHTMLSurface_CopyToClipboard", kParams_142};
const steammock::SlotInfo kCall_295 = {"SteamAPI_ISteamHTMLSurface_PasteFromClipboard", kParams_142};
const steammock::SlotInfo kCall_296 = {"SteamAPI_ISteamHTMLSurface_Find", kParams_154};
const steammock::SlotInfo kCall_297 = {"SteamAPI_ISteamHTMLSurface_StopFind", kParams_142};
const steammock::SlotInfo kCall_298 = {"SteamAPI_ISteamHTMLSurface_GetLinkAtPosition", kParams_148};
const steammock::SlotInfo kCall_299 = {"SteamAPI_ISteamHTMLSurface_SetCookie", kParams_155};
const steammock::SlotInfo kCall_300 = {"SteamAPI_ISteamHTMLSurface_SetPageScaleFactor", kParams_156};
const steammock::SlotInfo kCall_301 = {"SteamAPI_ISteamHTMLSurface_SetBackgroundMode", kParams_157};
const steammock::SlotInfo kCall_302 = {"SteamAPI_ISteamHTMLSurface_SetDPIScalingFactor", kParams_158};
const steammock::SlotInfo kCall_303 = {"SteamAPI_ISteamHTMLSurface_AllowStartRequest", kParams_159};
const steammock::SlotInfo kCall_304 = {"SteamAPI_ISteamHTMLSurface_JSDialogResponse", kParams_160};
const steammock::SlotInfo kCall_305 = {"ISteamHTMLSurface::FileLoadDialogResponse", kParams_161};
const steammock::SlotInfo kCall_306 = {"SteamAPI_ISteamHTMLSurface_KeyDown", kParams_162};
const steammock::SlotInfo kCall_307 = {"SteamAPI_ISteamHTMLSurface_OpenDeveloperTools", kParams_142};
const steammock::SlotInfo kCall_308 = {"SteamAPI_ISteamHTMLSurface_FileLoadDialogResponse", kParams_161};
const steammock::SlotInfo kCall_309 = {"SteamAPI_ISteamHTTP_CreateHTTPRequest", kParams_163};
const steammock::SlotInfo kCall_310 = {"SteamAPI_ISteamHTTP_SetHTTPRequestContextValue", kParams_164};
const steammock::SlotInfo kCall_311 = {"SteamAPI_ISteamHTTP_SetHTTPRequestNetworkActivityTimeout", kParams_165};
const steammock::SlotInfo kCall_312 = {"SteamAPI_ISteamHTTP_SetHTTPRequestHeaderValue", kParams_166};
const steammock::SlotInfo kCall_313 = {"SteamAPI_ISteamHTTP_SetHTTPRequestGetOrPostParameter", kParams_167};
const steammock::SlotInfo kCall_314 = {"SteamAPI_ISteamHTTP_SendHTTPRequest", kParams_168};
const steammock::SlotInfo kCall_315 = {"SteamAPI_ISteamHTTP_SendHTTPRequestAndStreamResponse", kParams_168};
const steammock::SlotInfo kCall_316 = {"SteamAPI_ISteamHTTP_DeferHTTPRequest", kParams_169};
const steammock::SlotInfo kCall_317 = {"SteamAPI_ISteamHTTP_PrioritizeHTTPRequest", kParams_169};
const steammock::SlotInfo kCall_318 = {"SteamAPI_ISteamHTTP_GetHTTPResponseHeaderSize", kParams_170};
const steammock::SlotInfo kCall_319 = {"SteamAPI_ISteamHTTP_GetHTTPResponseHeaderValue", kParams_171};
const steammock::SlotInfo kCall_320 = {"SteamAPI_ISteamHTTP_GetHTTPResponseBodySize", kParams_172};
const steammock::SlotInfo kCall_321 = {"SteamAPI_ISteamHTTP_GetHTTPResponseBodyData", kParams_173};
const steammock::SlotInfo kCall_322 = {"SteamAPI_ISteamHTTP_GetHTTPStreamingResponseBodyData", kParams_174};
const steammock::SlotInfo kCall_323 = {"SteamAPI_ISteamHTTP_ReleaseHTTPRequest", kParams_169};
const steammock::SlotInfo kCall_324 = {"SteamAPI_ISteamHTTP_GetHTTPDownloadProgressPct", kParams_175};
const steammock::SlotInfo kCall_325 = {"SteamAPI_ISteamHTTP_SetHTTPRequestRawPostBody", kParams_176};
const steammock::SlotInfo kCall_326 = {"SteamAPI_ISteamHTTP_CreateCookieContainer", kParams_177};
const steammock::SlotInfo kCall_327 = {"SteamAPI_ISteamHTTP_ReleaseCookieContainer", kParams_178};
const steammock::SlotInfo kCall_328 = {"SteamAPI_ISteamHTTP_SetCookie", kParams_179};
const steammock::SlotInfo kCall_329 = {"SteamAPI_ISteamHTTP_SetHTTPRequestCookieContainer", kParams_180};
const steammock::SlotInfo kCall_330 = {"SteamAPI_ISteamHTTP_SetHTTPRequestUserAgentInfo", kParams_181};
const steammock::SlotInfo kCall_331 = {"SteamAPI_ISteamHTTP_SetHTTPRequestRequiresVerifiedCertificate", kParams_182};
const steammock::SlotInfo kCall_332 = {"SteamAPI_ISteamHTTP_SetHTTPRequestAbsoluteTimeoutMS", kParams_183};
const steammock::SlotInfo kCall_333 = {"SteamAPI_ISteamHTTP_GetHTTPRequestWasTimedOut", kParams_184};
const steammock::SlotInfo kCall_334 = {"SteamAPI_ISteamInput_Init", nullptr};
const steammock::SlotInfo kCall_335 = {"SteamAPI_ISteamInput_Shutdown", nullptr};
const steammock::SlotInfo kCall_336 = {"SteamAPI_ISteamInput_RunFrame", nullptr};
const steammock::SlotInfo kCall_337 = {"SteamAPI_ISteamInput_GetConnectedControllers", kParams_25};
const steammock::SlotInfo kCall_338 = {"SteamAPI_ISteamInput_GetActionSetHandle", kParams_27};
const steammock::SlotInfo kCall_339 = {"SteamAPI_ISteamInput_ActivateActionSet", kParams_185};
const steammock::SlotInfo kCall_340 = {"SteamAPI_ISteamInput_GetCurrentActionSet", kParams_186};
const steammock::SlotInfo kCall_341 = {"SteamAPI_ISteamInput_ActivateActionSetLayer", kParams_187};
const steammock::SlotInfo kCall_342 = {"SteamAPI_ISteamInput_DeactivateActionSetLayer", kParams_187};
const steammock::SlotInfo kCall_343 = {"SteamAPI_ISteamInput_DeactivateAllActionSetLayers", kParams_186};
const steammock::SlotInfo kCall_344 = {"SteamAPI_ISteamInput_GetActiveActionSetLayers", kParams_188};
const steammock::SlotInfo kCall_345 = {"SteamAPI_ISteamInput_GetDigitalActionHandle", kParams_29};
const steammock::SlotInfo kCall_346 = {"SteamAPI_ISteamInput_GetDigitalActionData", kParams_189};
const steammock::SlotInfo kCall_347 = {"SteamAPI_ISteamInput_GetDigitalActionOrigins", kParams_190};
const steammock::SlotInfo kCall_348 = {"SteamAPI_ISteamInput_GetAnalogActionHandle", kParams_29};
const steammock::SlotInfo kCall_349 = {"SteamAPI_ISteamInput_GetAnalogActionData", kParams_191};
const steammock::SlotInfo kCall_350 = {"SteamAPI_ISteamInput_GetAnalogActionOrigins", kParams_192};
const steammock::SlotInfo kCall_351 = {"SteamAPI_ISteamInput_GetGlyphForActionOrigin", kParams_43};
const steammock::SlotInfo kCall_352 = {"SteamAPI_ISteamInput_GetStringForActionOrigin", kParams_43};
const steammock::SlotInfo kCall_353 = {"SteamAPI_ISteamInput_StopAnalogActionMomentum", kParams_193};
const steammock::SlotInfo kCall_354 = {"SteamAPI_ISteamInput_GetMotionData", kParams_186};
const steammock::SlotInfo kCall_355 = {"SteamAPI_ISteamInput_TriggerVibration", kParams_194};
const steammock::SlotInfo kCall_356 = {"SteamAPI_ISteamInput_SetLEDColor", kParams_195};
const steammock::SlotInfo kCall_357 = {"SteamAPI_ISteamInput_TriggerHapticPulse", kParams_196};
const steammock::SlotInfo kCall_358 = {"SteamAPI_ISteamInput_TriggerRepeatedHapticPulse", kParams_197};
const steammock::SlotInfo kCall_359 = {"SteamAPI_ISteamInput_ShowBindingPanel", kParams_186};
const steammock::SlotInfo kCall_360 = {"SteamAPI_ISteamInput_GetInputTypeForHandle", kParams_186};
const steammock::SlotInfo kCall_361 = {"SteamAPI_ISteamInput_GetControllerForGamepadIndex", kParams_40};
const steammock::SlotInfo kCall_362 = {"SteamAPI_ISteamInput_GetGamepadIndexForController", kParams_198};
const steammock::SlotInfo kCall_363 = {"SteamAPI_ISteamInput_GetStringForXboxOrigin", kParams_43};
const steammock::SlotInfo kCall_364 = {"SteamAPI_ISteamInput_GetGlyphForXboxOrigin", kParams_43};
const steammock::SlotInfo kCall_365 = {"SteamAPI_ISteamInput_GetActionOriginFromXboxOrigin", kParams_199};
const steammock::SlotInfo kCall_366 = {"SteamAPI_ISteamInput_TranslateActionOrigin", kParams_47};
const steammock::SlotInfo kCall_367 = {"SteamAPI_ISteamInput_GetDeviceBindingRevision", kParams_200};
const steammock::SlotInfo kCall_368 = {"SteamAPI_ISteamInput_GetRemotePlaySessionID", kParams_186};
const steammock::SlotInfo kCall_369 = {"SteamAPI_ISteamInput_Init", kParams_201};
const steammock::SlotInfo kCall_370 = {"SteamAPI_ISteamInput_SetInputActionManifestFilePath", kParams_202};
const steammock::SlotInfo kCall_371 = {"SteamAPI_ISteamInput_RunFrame", kParams_203};
const steammock::SlotInfo kCall_372 = {"SteamAPI_ISteamInput_BWaitForData", kParams_204};
const steammock::SlotInfo kCall_373 = {"SteamAPI_ISteamInput_BNewDataAvailable", nullptr};
const steammock::SlotInfo kCall_374 = {"SteamAPI_ISteamInput_EnableDeviceCallbacks", nullptr};
const steammock::SlotInfo kCall_375 = {"SteamAPI_ISteamInput_EnableActionEventCallbacks", kParams_205};
const steammock::SlotInfo kCall_376 = {"SteamAPI_ISteamInput_GetStringForDigitalActionName", kParams_206};
const steammock::SlotInfo kCall_377 = {"SteamAPI_ISteamInput_GetGlyphPNGForActionOrigin", kParams_207};
const steammock::SlotInfo kCall_378 = {"SteamAPI_ISteamInput_GetGlyphSVGForActionOrigin", kParams_208};
const steammock::SlotInfo kCall_379 = {"SteamAPI_ISteamInput_GetGlyphForActionOrigin_Legacy", kParams_43};
const steammock::SlotInfo kCall_380 = {"SteamAPI_ISteamInput_GetStringForAnalogActionName", kParams_206};
const steammock::SlotInfo kCall_381 = {"SteamAPI_ISteamInput_TriggerVibrationExtended", kParams_209};
const steammock::SlotInfo kCall_382 = {"SteamAPI_ISteamInput_TriggerSimpleHapticEvent", kParams_210};
const steammock::SlotInfo kCall_383 = {"SteamAPI_ISteamInput_Legacy_TriggerHapticPulse", kParams_196};
const steammock::SlotInfo kCall_384 = {"SteamAPI_ISteamInput_Legacy_TriggerRepeatedHapticPulse", kParams_197};
const steammock::SlotInfo kCall_385 = {"SteamAPI_ISteamInput_GetSessionInputConfigurationSettings", nullptr};
const steammock::SlotInfo kCall_386 = {"SteamAPI_ISteamInput_SetDualSenseTriggerEffect", kParams_211};
const steammock::SlotInfo kCall_387 = {"SteamAPI_ISteamInventory_GetResultStatus", kParams_212};
const steammock::SlotInfo kCall_388 = {"SteamAPI_ISteamInventory_GetResultItems", kParams_213};
const steammock::SlotInfo kCall_389 = {"SteamAPI_ISteamInventory_GetResultItemProperty", kParams_214};
const steammock::SlotInfo kCall_390 = {"SteamAPI_ISteamInventory_GetResultTimestamp", kParams_212};
const steammock::SlotInfo kCall_391 = {"SteamAPI_ISteamInventory_CheckResultSteamID", kParams_215};
const steammock::SlotInfo kCall_392 = {"SteamAPI_ISteamInventory_DestroyResult", kParams_212};
const steammock::SlotInfo kCall_393 = {"SteamAPI_ISteamInventory_GetAllItems", kParams_216};
const steammock::SlotInfo kCall_394 = {"SteamAPI_ISteamInventory_GetItemsByID", kParams_217};
const steammock::SlotInfo kCall_395 = {"SteamAPI_ISteamInventory_SerializeResult", kParams_218};
const steammock::SlotInfo kCall_396 = {"SteamAPI_ISteamInventory_DeserializeResult", kParams_219};
const steammock::SlotInfo kCall_397 = {"SteamAPI_ISteamInventory_GenerateItems", kParams_220};
const steammock::SlotInfo kCall_398 = {"SteamAPI_ISteamInventory_GrantPromoItems", kParams_216};
const steammock::SlotInfo kCall_399 = {"SteamAPI_ISteamInventory_AddPromoItem", kParams_221};
const steammock::SlotInfo kCall_400 = {"SteamAPI_ISteamInventory_AddPromoItems", kParams_222};
const steammock::SlotInfo kCall_401 = {"SteamAPI_ISteamInventory_ConsumeItem", kParams_223};
const steammock::SlotInfo kCall_402 = {"SteamAPI_ISteamInventory_ExchangeItems", kParams_224};
const steammock::SlotInfo kCall_403 = {"SteamAPI_ISteamInventory_TransferItemQuantity", kParams_225};
const steammock::SlotInfo kCall_404 = {"SteamAPI_ISteamInventory_SendItemDropHeartbeat", nullptr};
const steammock::SlotInfo kCall_405 = {"SteamAPI_ISteamInventory_TriggerItemDrop", kParams_226};
const steammock::SlotInfo kCall_406 = {"SteamAPI_ISteamInventory_TradeItems", kParams_227};
const steammock::SlotInfo kCall_407 = {"SteamAPI_ISteamInventory_LoadItemDefinitions", nullptr};
const steammock::SlotInfo kCall_408 = {"SteamAPI_ISteamInventory_GetItemDefinitionIDs", kParams_228};
const steammock::SlotInfo kCall_409 = {"SteamAPI_ISteamInventory_GetItemDefinitionProperty", kParams_229};
const steammock::SlotInfo kCall_410 = {"SteamAPI_ISteamInventory_RequestEligiblePromoItemDefinitionsIDs", kParams_89};
const steammock::SlotInfo kCall_411 = {"SteamAPI_ISteamInventory_GetEligiblePromoItemDefinitionIDs", kParams_230};
const steammock::SlotInfo kCall_412 = {"SteamAPI_ISteamInventory_StartPurchase", kParams_231};
const steammock::SlotInfo kCall_413 = {"SteamAPI_ISteamInventory_RequestPrices", nullptr};
const steammock::SlotInfo kCall_414 = {"SteamAPI_ISteamInventory_GetNumItemsWithPrices", nullptr};
const steammock::SlotInfo kCall_415 = {"SteamAPI_ISteamInventory_GetItemsWithPrices", kParams_232};
const steammock::SlotInfo kCall_416 = {"SteamAPI_ISteamInventory_GetItemPrice", kParams_233};
const steammock::SlotInfo kCall_417 = {"SteamAPI_ISteamInventory_StartUpdateProperties", nullptr};
const steammock::SlotInfo kCall_418 = {"SteamAPI_ISteamInventory_RemoveProperty", kParams_234};
const steammock::SlotInfo kCall_419 = {"SteamAPI_ISteamInventory_SetPropertyString", kParams_235};
const steammock::SlotInfo kCall_420 = {"SteamAPI_ISteamInventory_SetPropertyBool", kParams_236};
const steammock::SlotInfo kCall_421 = {"SteamAPI_ISteamInventory_SetPropertyInt64", kParams_237};
const steammock::SlotInfo kCall_422 = {"SteamAPI_ISteamInventory_SetPropertyFloat", kParams_238};
const steammock::SlotInfo kCall_423 = {"SteamAPI_ISteamInventory_SubmitUpdateProperties", kParams_239};
const steammock::SlotInfo kCall_424 = {"SteamAPI_ISteamInventory_InspectItem", kParams_240};
const steammock::SlotInfo kCall_425 = {"SteamAPI_ISteamMatchmaking_GetFavoriteGameCount", nullptr};
const steammock::SlotInfo kCall_426 = {"SteamAPI_ISteamMatchmaking_GetFavoriteGame", kParams_241};
const steammock::SlotInfo kCall_427 = {"SteamAPI_ISteamMatchmaking_AddFavoriteGame", kParams_242};
const steammock::SlotInfo kCall_428 = {"SteamAPI_ISteamMatchmaking_RemoveFavoriteGame", kParams_243};
const steammock::SlotInfo kCall_429 = {"SteamAPI_ISteamMatchmaking_RequestLobbyList", nullptr};
const steammock::SlotInfo kCall_430 = {"SteamAPI_ISteamMatchmaking_AddRequestLobbyListStringFilter", kParams_244};
const steammock::SlotInfo kCall_431 = {"SteamAPI_ISteamMatchmaking_AddRequestLobbyListNumericalFilter", kParams_245};
const steammock::SlotInfo kCall_432 = {"SteamAPI_ISteamMatchmaking_AddRequestLobbyListNearValueFilter", kParams_246};
const steammock::SlotInfo kCall_433 = {"SteamAPI_ISteamMatchmaking_AddRequestLobbyListFilterSlotsAvailable", kParams_247};
const steammock::SlotInfo kCall_434 = {"SteamAPI_ISteamMatchmaking_AddRequestLobbyListDistanceFilter", kParams_248};
const steammock::SlotInfo kCall_435 = {"SteamAPI_ISteamMatchmaking_AddRequestLobbyListResultCountFilter", kParams_249};
const steammock::SlotInfo kCall_436 = {"SteamAPI_ISteamMatchmaking_AddRequestLobbyListCompatibleMembersFilter", kParams_73};
const steammock::SlotInfo kCall_437 = {"SteamAPI_ISteamMatchmaking_GetLobbyByIndex", kParams_250};
const steammock::SlotInfo kCall_438 = {"SteamAPI_ISteamMatchmaking_CreateLobby", kParams_251};
const steammock::SlotInfo kCall_439 = {"SteamAPI_ISteamMatchmaking_JoinLobby", kParams_73};
const steammock::SlotInfo kCall_440 = {"SteamAPI_ISteamMatchmaking_LeaveLobby", kParams_73};
const steammock::SlotInfo kCall_441 = {"SteamAPI_ISteamMatchmaking_InviteUserToLobby", kParams_252};
const steammock::SlotInfo kCall_442 = {"SteamAPI_ISteamMatchmaking_GetNumLobbyMembers", kParams_73};
const steammock::SlotInfo kCall_443 = {"SteamAPI_ISteamMatchmaking_GetLobbyMemberByIndex", kParams_253};
const steammock::SlotInfo kCall_444 = {"SteamAPI_ISteamMatchmaking_GetLobbyData", kParams_254};
const steammock::SlotInfo kCall_445 = {"SteamAPI_ISteamMatchmaking_SetLobbyData", kParams_255};
const steammock::SlotInfo kCall_446 = {"SteamAPI_ISteamMatchmaking_GetLobbyDataCount", kParams_73};
const steammock::SlotInfo kCall_447 = {"SteamAPI_ISteamMatchmaking_GetLobbyDataByIndex", kParams_256};
const steammock::SlotInfo kCall_448 = {"SteamAPI_ISteamMatchmaking_DeleteLobbyData", kParams_254};
const steammock::SlotInfo kCall_449 = {"SteamAPI_ISteamMatchmaking_GetLobbyMemberData", kParams_257};
const steammock::SlotInfo kCall_450 = {"SteamAPI_ISteamMatchmaking_SetLobbyMemberData", kParams_255};
const steammock::SlotInfo kCall_451 = {"SteamAPI_ISteamMatchmaking_SendLobbyChatMsg", kParams_258};
const steammock::SlotInfo kCall_452 = {"SteamAPI_ISteamMatchmaking_GetLobbyChatEntry", kParams_259};
const steammock::SlotInfo kCall_453 = {"SteamAPI_ISteamMatchmaking_RequestLobbyData", kParams_73};
const steammock::SlotInfo kCall_454 = {"SteamAPI_ISteamMatchmaking_SetLobbyGameServer", kParams_260};
const steammock::SlotInfo kCall_455 = {"SteamAPI_ISteamMatchmaking_GetLobbyGameServer", kParams_261};
const steammock::SlotInfo kCall_456 = {"SteamAPI_ISteamMatchmaking_SetLobbyMemberLimit", kParams_262};
const steammock::SlotInfo kCall_457 = {"SteamAPI_ISteamMatchmaking_GetLobbyMemberLimit", kParams_73};
const steammock::SlotInfo kCall_458 = {"SteamAPI_ISteamMatchmaking_SetLobbyType", kParams_263};
const steammock::SlotInfo kCall_459 = {"SteamAPI_ISteamMatchmaking_SetLobbyJoinable", kParams_264};
const steammock::SlotInfo kCall_460 = {"SteamAPI_ISteamMatchmaking_GetLobbyOwner", kParams_73};
const steammock::SlotInfo kCall_461 = {"SteamAPI_ISteamMatchmaking_SetLobbyOwner", kParams_265};
const steammock::SlotInfo kCall_462 = {"SteamAPI_ISteamMatchmaking_SetLinkedLobby", kParams_266};
const steammock::SlotInfo kCall_463 = {"SteamAPI_ISteamMatchmakingServers_RequestInternetServerList", kParams_267};
const steammock::SlotInfo kCall_464 = {"SteamAPI_ISteamMatchmakingServers_RequestLANServerList", kParams_268};
const steammock::SlotInfo kCall_465 = {"SteamAPI_ISteamMatchmakingServers_RequestFriendsServerList", kParams_267};
const steammock::SlotInfo kCall_466 = {"SteamAPI_ISteamMatchmakingServers_RequestFavoritesServerList", kParams_267};
const steammock::SlotInfo kCall_467 = {"SteamAPI_ISteamMatchmakingServers_RequestHistoryServerList", kParams_267};
const steammock::SlotInfo kCall_468 = {"SteamAPI_ISteamMatchmakingServers_RequestSpectatorServerList", kParams_267};
const steammock::SlotInfo kCall_469 = {"SteamAPI_ISteamMatchmakingServers_ReleaseRequest", kParams_269};
const steammock::SlotInfo kCall_470 = {"SteamAPI_ISteamMatchmakingServers_GetServerDetails", kParams_270};
const steammock::SlotInfo kCall_471 = {"SteamAPI_ISteamMatchmakingServers_CancelQuery", kParams_169};
const steammock::SlotInfo kCall_472 = {"SteamAPI_ISteamMatchmakingServers_RefreshQuery", kParams_169};
const steammock::SlotInfo kCall_473 = {"SteamAPI_ISteamMatchmakingServers_IsRefreshing", kParams_169};
const steammock::SlotInfo kCall_474 = {"SteamAPI_ISteamMatchmakingServers_GetServerCount", kParams_169};
const steammock::SlotInfo kCall_475 = {"SteamAPI_ISteamMatchmakingServers_RefreshServer", kParams_270};
const steammock::SlotInfo kCall_476 = {"SteamAPI_ISteamMatchmakingServers_PingServer", kParams_271};
const steammock::SlotInfo kCall_477 = {"SteamAPI_ISteamMatchmakingServers_PlayerDetails", kParams_271};
const steammock::SlotInfo kCall_478 = {"SteamAPI_ISteamMatchmakingServers_ServerRules", kParams_271};
const steammock::SlotInfo kCall_479 = {"SteamAPI_ISteamMatchmakingServers_CancelServerQuery", kParams_272};
const steammock::SlotInfo kCall_480 = {"SteamAPI_ISteamMusic_BIsEnabled", nullptr};
const steammock::SlotInfo kCall_481 = {"SteamAPI_ISteamMusic_BIsPlaying", nullptr};
const steammock::SlotInfo kCall_482 = {"SteamAPI_ISteamMusic_GetPlaybackStatus", nullptr};
const steammock::SlotInfo kCall_483 = {"SteamAPI_ISteamMusic_Play", nullptr};
const steammock::SlotInfo kCall_484 = {"SteamAPI_ISteamMusic_Pause", nullptr};
const steammock::SlotInfo kCall_485 = {"SteamAPI_ISteamMusic_PlayPrevious", nullptr};
const steammock::SlotInfo kCall_486 = {"SteamAPI_ISteamMusic_PlayNext", nullptr};
const steammock::SlotInfo kCall_487 = {"SteamAPI_ISteamMusic_SetVolume", kParams_273};
const steammock::SlotInfo kCall_488 = {"SteamAPI_ISteamMusic_GetVolume", nullptr};
const steammock::SlotInfo kCall_489 = {"SteamAPI_ISteamMusicRemote_RegisterSteamMusicRemote", kParams_274};
const steammock::SlotInfo kCall_490 = {"SteamAPI_ISteamMusicRemote_DeregisterSteamMusicRemote", nullptr};
const steammock::SlotInfo kCall_491 = {"SteamAPI_ISteamMusicRemote_BIsCurrentMusicRemote", nullptr};
const steammock::SlotInfo kCall_492 = {"SteamAPI_ISteamMusicRemote_BActivationSuccess", kParams_275};
const steammock::SlotInfo kCall_493 = {"SteamAPI_ISteamMusicRemote_SetDisplayName", kParams_276};
const steammock::SlotInfo kCall_494 = {"SteamAPI_ISteamMusicRemote_SetPNGIcon_64x64", kParams_277};
const steammock::SlotInfo kCall_495 = {"SteamAPI_ISteamMusicRemote_EnablePlayPrevious", kParams_275};
const steammock::SlotInfo kCall_496 = {"SteamAPI_ISteamMusicRemote_EnablePlayNext", kParams_275};
const steammock::SlotInfo kCall_497 = {"SteamAPI_ISteamMusicRemote_EnableShuffled", kParams_275};
const steammock::SlotInfo kCall_498 = {"SteamAPI_ISteamMusicRemote_EnableLooped", kParams_275};
const steammock::SlotInfo kCall_499 = {"SteamAPI_ISteamMusicRemote_EnableQueue", kParams_275};
const steammock::SlotInfo kCall_500 = {"SteamAPI_ISteamMusicRemote_EnablePlaylists", kParams_275};
const steammock::SlotInfo kCall_501 = {"SteamAPI_ISteamMusicRemote_UpdatePlaybackStatus", kParams_278};
const steammock::SlotInfo kCall_502 = {"SteamAPI_ISteamMusicRemote_UpdateShuffled", kParams_275};
const steammock::SlotInfo kCall_503 = {"SteamAPI_ISteamMusicRemote_UpdateLooped", kParams_275};
const steammock::SlotInfo kCall_504 = {"SteamAPI_ISteamMusicRemote_UpdateVolume", kParams_279};
const steammock::SlotInfo kCall_505 = {"SteamAPI_ISteamMusicRemote_CurrentEntryWillChange", nullptr};
const steammock::SlotInfo kCall_506 = {"SteamAPI_ISteamMusicRemote_CurrentEntryIsAvailable", kParams_280};
const steammock::SlotInfo kCall_507 = {"SteamAPI_ISteamMusicRemote_UpdateCurrentEntryText", kParams_281};
const steammock::SlotInfo kCall_508 = {"SteamAPI_ISteamMusicRemote_UpdateCurrentEntryElapsedSeconds", kParams_282};
const steammock::SlotInfo kCall_509 = {"SteamAPI_ISteamMusicRemote_UpdateCurrentEntryCoverArt", kParams_277};
const steammock::SlotInfo kCall_510 = {"SteamAPI_ISteamMusicRemote_CurrentEntryDidChange", nullptr};
const steammock::SlotInfo kCall_511 = {"SteamAPI_ISteamMusicRemote_QueueWillChange", nullptr};
const steammock::SlotInfo kCall_512 = {"SteamAPI_ISteamMusicRemote_ResetQueueEntries", nullptr};
const steammock::SlotInfo kCall_513 = {"SteamAPI_ISteamMusicRemote_SetQueueEntry", kParams_283};
const steammock::SlotInfo kCall_514 = {"SteamAPI_ISteamMusicRemote_SetCurrentQueueEntry", kParams_284};
const steammock::SlotInfo kCall_515 = {"SteamAPI_ISteamMusicRemote_QueueDidChange", nullptr};
const steammock::SlotInfo kCall_516 = {"SteamAPI_ISteamMusicRemote_PlaylistWillChange", nullptr};
const steammock::SlotInfo kCall_517 = {"SteamAPI_ISteamMusicRemote_ResetPlaylistEntries", nullptr};
const steammock::SlotInfo kCall_518 = {"SteamAPI_ISteamMusicRemote_SetPlaylistEntry", kParams_283};
const steammock::SlotInfo kCall_519 = {"SteamAPI_ISteamMusicRemote_SetCurrentPlaylistEntry", kParams_284};
const steammock::SlotInfo kCall_520 = {"SteamAPI_ISteamMusicRemote_PlaylistDidChange", nullptr};
const steammock::SlotInfo kCall_521 = {"SteamAPI_ISteamNetworking_SendP2PPacket", kParams_285};
const steammock::SlotInfo kCall_522 = {"SteamAPI_ISteamNetworking_IsP2PPacketAvailable", kParams_286};
const steammock::SlotInfo kCall_523 = {"SteamAPI_ISteamNetworking_ReadP2PPacket", kParams_287};
const steammock::SlotInfo kCall_524 = {"SteamAPI_ISteamNetworking_AcceptP2PSessionWithUser", kParams_288};
const steammock::SlotInfo kCall_525 = {"SteamAPI_ISteamNetworking_CloseP2PSessionWithUser", kParams_288};
const steammock::SlotInfo kCall_526 = {"SteamAPI_ISteamNetworking_CloseP2PChannelWithUser", kParams_289};
const steammock::SlotInfo kCall_527 = {"SteamAPI_ISteamNetworking_GetP2PSessionState", kParams_290};
const steammock::SlotInfo kCall_528 = {"SteamAPI_ISteamNetworking_AllowP2PPacketRelay", kParams_291};
const steammock::SlotInfo kCall_529 = {"SteamAPI_ISteamNetworking_CreateListenSocket", kParams_292};
const steammock::SlotInfo kCall_530 = {"SteamAPI_ISteamNetworking_CreateP2PConnectionSocket", kParams_293};
const steammock::SlotInfo kCall_531 = {"SteamAPI_ISteamNetworking_CreateConnectionSocket", kParams_294};
const steammock::SlotInfo kCall_532 = {"SteamAPI_ISteamNetworking_DestroySocket", kParams_295};
const steammock::SlotInfo kCall_533 = {"SteamAPI_ISteamNetworking_DestroyListenSocket", kParams_295};
const steammock::SlotInfo kCall_534 = {"SteamAPI_ISteamNetworking_SendDataOnSocket", kParams_296};
const steammock::SlotInfo kCall_535 = {"SteamAPI_ISteamNetworking_IsDataAvailableOnSocket", kParams_297};
const steammock::SlotInfo kCall_536 = {"SteamAPI_ISteamNetworking_RetrieveDataFromSocket", kParams_298};
const steammock::SlotInfo kCall_537 = {"SteamAPI_ISteamNetworking_IsDataAvailable", kParams_299};
const steammock::SlotInfo kCall_538 = {"SteamAPI_ISteamNetworking_RetrieveData", kParams_300};
const steammock::SlotInfo kCall_539 = {"SteamAPI_ISteamNetworking_GetSocketInfo", kParams_301};
const steammock::SlotInfo kCall_540 = {"SteamAPI_ISteamNetworking_GetListenSocketInfo", kParams_302};
const steammock::SlotInfo kCall_541 = {"SteamAPI_ISteamNetworking_GetSocketConnectionType", kParams_303};
const steammock::SlotInfo kCall_542 = {"SteamAPI_ISteamNetworking_GetMaxPacketSize", kParams_303};
const steammock::SlotInfo kCall_543 = {"SteamAPI_ISteamNetworkingMessages_SendMessageToUser", kParams_304};
const steammock::SlotInfo kCall_544 = {"SteamAPI_ISteamNetworkingMessages_ReceiveMessagesOnChannel", kParams_305};
const steammock::SlotInfo kCall_545 = {"SteamAPI_ISteamNetworkingMessages_AcceptSessionWithUser", kParams_306};
const steammock::SlotInfo kCall_546 = {"SteamAPI_ISteamNetworkingMessages_CloseSessionWithUser", kParams_306};
const steammock::SlotInfo kCall_547 = {"SteamAPI_ISteamNetworkingMessages_CloseChannelWithUser", kParams_307};
const steammock::SlotInfo kCall_548 = {"SteamAPI_ISteamNetworkingMessages_GetSessionConnectionInfo", kParams_308};
const steammock::SlotInfo kCall_549 = {"SteamAPI_ISteamNetworkingSockets_CreateListenSocketIP", kParams_309};
const steammock::SlotInfo kCall_550 = {"SteamAPI_ISteamNetworkingSockets_ConnectByIPAddress", kParams_310};
const steammock::SlotInfo kCall_551 = {"SteamAPI_ISteamNetworkingSockets_CreateListenSocketP2P", kParams_311};
const steammock::SlotInfo kCall_552 = {"SteamAPI_ISteamNetworkingSockets_ConnectP2P", kParams_312};
const steammock::SlotInfo kCall_553 = {"SteamAPI_ISteamNetworkingSockets_AcceptConnection", kParams_313};
const steammock::SlotInfo kCall_554 = {"SteamAPI_ISteamNetworkingSockets_CloseConnection", kParams_314};
const steammock::SlotInfo kCall_555 = {"SteamAPI_ISteamNetworkingSockets_CloseListenSocket", kParams_303};
const steammock::SlotInfo kCall_556 = {"SteamAPI_ISteamNetworkingSockets_SetConnectionUserData", kParams_315};
const steammock::SlotInfo kCall_557 = {"SteamAPI_ISteamNetworkingSockets_GetConnectionUserData", kParams_316};
const steammock::SlotInfo kCall_558 = {"SteamAPI_ISteamNetworkingSockets_SetConnectionName", kParams_317};
const steammock::SlotInfo kCall_559 = {"SteamAPI_ISteamNetworkingSockets_GetConnectionName", kParams_318};
const steammock::SlotInfo kCall_560 = {"SteamAPI_ISteamNetworkingSockets_SendMessageToConnection", kParams_319};
const steammock::SlotInfo kCall_561 = {"SteamAPI_ISteamNetworkingSockets_SendMessages", kParams_320};
const steammock::SlotInfo kCall_562 = {"SteamAPI_ISteamNetworkingSockets_FlushMessagesOnConnection", kParams_313};
const steammock::SlotInfo kCall_563 = {"SteamAPI_ISteamNetworkingSockets_ReceiveMessagesOnConnection", kParams_321};
const steammock::SlotInfo kCall_564 = {"SteamAPI_ISteamNetworkingSockets_GetConnectionInfo", kParams_322};
const steammock::SlotInfo kCall_565 = {"SteamAPI_ISteamNetworkingSockets_GetQuickConnectionStatus", kParams_323};
const steammock::SlotInfo kCall_566 = {"SteamAPI_ISteamNetworkingSockets_GetDetailedConnectionStatus", kParams_324};
const steammock::SlotInfo kCall_567 = {"SteamAPI_ISteamNetworkingSockets_GetListenSocketAddress", kParams_325};
const steammock::SlotInfo kCall_568 = {"SteamAPI_ISteamNetworkingSockets_CreateSocketPair", kParams_326};
const steammock::SlotInfo kCall_569 = {"SteamAPI_ISteamNetworkingSockets_GetIdentity", kParams_327};
const steammock::SlotInfo kCall_570 = {"SteamAPI_ISteamNetworkingSockets_InitAuthentication", nullptr};
const steammock::SlotInfo kCall_571 = {"SteamAPI_ISteamNetworkingSockets_GetAuthenticationStatus", kParams_328};
const steammock::SlotInfo kCall_572 = {"SteamAPI_ISteamNetworkingSockets_CreatePollGroup", nullptr};
const steammock::SlotInfo kCall_573 = {"SteamAPI_ISteamNetworkingSockets_DestroyPollGroup", kParams_329};
const steammock::SlotInfo kCall_574 = {"SteamAPI_ISteamNetworkingSockets_SetConnectionPollGroup", kParams_330};
const steammock::SlotInfo kCall_575 = {"SteamAPI_ISteamNetworkingSockets_ReceiveMessagesOnPollGroup", kParams_331};
const steammock::SlotInfo kCall_576 = {"SteamAPI_ISteamNetworkingSockets_ReceivedRelayAuthTicket", kParams_332};
const steammock::SlotInfo kCall_577 = {"SteamAPI_ISteamNetworkingSockets_FindRelayAuthTicketForServer", kParams_333};
const steammock::SlotInfo kCall_578 = {"SteamAPI_ISteamNetworkingSockets_ConnectToHostedDedicatedServer", kParams_334};
const steammock::SlotInfo kCall_579 = {"SteamAPI_ISteamNetworkingSockets_GetHostedDedicatedServerPort", nullptr};
const steammock::SlotInfo kCall_580 = {"SteamAPI_ISteamNetworkingSockets_GetHostedDedicatedServerPOPID", nullptr};
const steammock::SlotInfo kCall_581 = {"SteamAPI_ISteamNetworkingSockets_GetHostedDedicatedServerAddress", kParams_335};
const steammock::SlotInfo kCall_582 = {"SteamAPI_ISteamNetworkingSockets_CreateHostedDedicatedServerListenSocket", kParams_311};
const steammock::SlotInfo kCall_583 = {"SteamAPI_ISteamNetworkingSockets_GetGameCoordinatorServerLogin", kParams_336};
const steammock::SlotInfo kCall_584 = {"SteamAPI_ISteamNetworkingSockets_ConnectP2PCustomSignaling", kParams_337};
const steammock::SlotInfo kCall_585 = {"SteamAPI_ISteamNetworkingSockets_ReceivedP2PCustomSignal", kParams_338};
const steammock::SlotInfo kCall_586 = {"SteamAPI_ISteamNetworkingSockets_GetCertificateRequest", kParams_339};
const steammock::SlotInfo kCall_587 = {"SteamAPI_ISteamNetworkingSockets_SetCertificate", kParams_340};
const steammock::SlotInfo kCall_588 = {"SteamAPI_ISteamNetworkingSockets_RunCallbacks", nullptr};
const steammock::SlotInfo kCall_589 = {"SteamAPI_ISteamNetworkingSockets_GetConnectionRealTimeStatus", kParams_341};
const steammock::SlotInfo kCall_590 = {"SteamAPI_ISteamNetworkingSockets_ConfigureConnectionLanes", kParams_342};
const steammock::SlotInfo kCall_591 = {"SteamAPI_ISteamNetworkingSockets_ResetIdentity", kParams_327};
const steammock::SlotInfo kCall_592 = {"SteamAPI_ISteamNetworkingSockets_BeginAsyncRequestFakeIP", kParams_343};
const steammock::SlotInfo kCall_593 = {"SteamAPI_ISteamNetworkingSockets_GetFakeIP", kParams_344};
const steammock::SlotInfo kCall_594 = {"SteamAPI_ISteamNetworkingSockets_CreateListenSocketP2PFakeIP", kParams_345};
const steammock::SlotInfo kCall_595 = {"SteamAPI_ISteamNetworkingSockets_GetRemoteFakeIPForConnection", kParams_346};
const steammock::SlotInfo kCall_596 = {"SteamAPI_ISteamNetworkingSockets_CreateFakeUDPPort", kParams_347};
const steammock::SlotInfo kCall_597 = {"SteamAPI_ISteamNetworkingUtils_AllocateMessage", kParams_348};
const steammock::SlotInfo kCall_598 = {"SteamAPI_ISteamNetworkingUtils_GetRelayNetworkStatus", kParams_328};
const steammock::SlotInfo kCall_599 = {"SteamAPI_ISteamNetworkingUtils_GetLocalPingLocation", kParams_349};
const steammock::SlotInfo kCall_600 = {"SteamAPI_ISteamNetworkingUtils_EstimatePingTimeBetweenTwoLocations", kParams_350};
const steammock::SlotInfo kCall_601 = {"SteamAPI_ISteamNetworkingUtils_EstimatePingTimeFromLocalHost", kParams_351};
const steammock::SlotInfo kCall_602 = {"SteamAPI_ISteamNetworkingUtils_ConvertPingLocationToString", kParams_352};
const steammock::SlotInfo kCall_603 = {"SteamAPI_ISteamNetworkingUtils_ParsePingLocationString", kParams_353};
const steammock::SlotInfo kCall_604 = {"SteamAPI_ISteamNetworkingUtils_CheckPingDataUpToDate", kParams_354};
const steammock::SlotInfo kCall_605 = {"SteamAPI_ISteamNetworkingUtils_GetPingToDataCenter", kParams_355};
const steammock::SlotInfo kCall_606 = {"SteamAPI_ISteamNetworkingUtils_GetDirectPingToPOP", kParams_356};
const steammock::SlotInfo kCall_607 = {"SteamAPI_ISteamNetworkingUtils_GetPOPCount", nullptr};
const steammock::SlotInfo kCall_608 = {"SteamAPI_ISteamNetworkingUtils_GetPOPList", kParams_357};
const steammock::SlotInfo kCall_609 = {"SteamAPI_ISteamNetworkingUtils_GetLocalTimestamp", nullptr};
const steammock::SlotInfo kCall_610 = {"SteamAPI_ISteamNetworkingUtils_SetDebugOutputFunction", kParams_358};
const steammock::SlotInfo kCall_611 = {"SteamAPI_ISteamNetworkingUtils_SetConfigValue", kParams_359};
const steammock::SlotInfo kCall_612 = {"SteamAPI_ISteamNetworkingUtils_GetConfigValue", kParams_360};
const steammock::SlotInfo kCall_613 = {"SteamAPI_ISteamNetworkingUtils_GetConfigValueInfo", kParams_361};
const steammock::SlotInfo kCall_614 = {"SteamAPI_ISteamNetworkingUtils_GetFirstConfigValue", nullptr};
const steammock::SlotInfo kCall_615 = {"SteamAPI_ISteamNetworkingUtils_SteamNetworkingIPAddr_ToString", kParams_362};
const steammock::SlotInfo kCall_616 = {"SteamAPI_ISteamNetworkingUtils_SteamNetworkingIPAddr_ParseString", kParams_363};
const steammock::SlotInfo kCall_617 = {"SteamAPI_ISteamNetworkingUtils_SteamNetworkingIdentity_ToString", kParams_364};
const steammock::SlotInfo kCall_618 = {"SteamAPI_ISteamNetworkingUtils_SteamNetworkingIdentity_ParseString", kParams_365};
const steammock::SlotInfo kCall_619 = {"SteamAPI_ISteamNetworkingUtils_GetIPv4FakeIPType", kParams_366};
const steammock::SlotInfo kCall_620 = {"SteamAPI_ISteamNetworkingUtils_GetRealIdentityForFakeIP", kParams_367};
const steammock::SlotInfo kCall_621 = {"SteamAPI_ISteamNetworkingUtils_GetConfigValueInfo", kParams_368};
const steammock::SlotInfo kCall_622 = {"SteamAPI_ISteamNetworkingUtils_IterateGenericEditableConfigValues", kParams_369};
const steammock::SlotInfo kCall_623 = {"SteamAPI_ISteamNetworkingUtils_SteamNetworkingIPAddr_GetFakeIPType", kParams_370};
const steammock::SlotInfo kCall_624 = {"SteamAPI_ISteamParentalSettings_BIsParentalLockEnabled", nullptr};
const steammock::SlotInfo kCall_625 = {"SteamAPI_ISteamParentalSettings_BIsParentalLockLocked", nullptr};
const steammock::SlotInfo kCall_626 = {"SteamAPI_ISteamParentalSettings_BIsAppBlocked", kParams_3};
const steammock::SlotInfo kCall_627 = {"SteamAPI_ISteamParentalSettings_BIsAppInBlockList", kParams_3};
const steammock::SlotInfo kCall_628 = {"SteamAPI_ISteamParentalSettings_BIsFeatureBlocked", kParams_371};
const steammock::SlotInfo kCall_629 = {"SteamAPI_ISteamParentalSettings_BIsFeatureInBlockList", kParams_371};
const steammock::SlotInfo kCall_630 = {"SteamAPI_ISteamParties_GetNumActiveBeacons", nullptr};
const steammock::SlotInfo kCall_631 = {"SteamAPI_ISteamParties_GetBeaconByIndex", kParams_372};
const steammock::SlotInfo kCall_632 = {"SteamAPI_ISteamParties_GetBeaconDetails", kParams_373};
const steammock::SlotInfo kCall_633 = {"SteamAPI_ISteamParties_JoinParty", kParams_374};
const steammock::SlotInfo kCall_634 = {"SteamAPI_ISteamParties_GetNumAvailableBeaconLocations", kParams_375};
const steammock::SlotInfo kCall_635 = {"SteamAPI_ISteamParties_GetAvailableBeaconLocations", kParams_376};
const steammock::SlotInfo kCall_636 = {"SteamAPI_ISteamParties_CreateBeacon", kParams_377};
const steammock::SlotInfo kCall_637 = {"SteamAPI_ISteamParties_OnReservationCompleted", kParams_378};
const steammock::SlotInfo kCall_638 = {"SteamAPI_ISteamParties_CancelReservation", kParams_378};
const steammock::SlotInfo kCall_639 = {"SteamAPI_ISteamParties_ChangeNumOpenSlots", kParams_379};
const steammock::SlotInfo kCall_640 = {"SteamAPI_ISteamParties_DestroyBeacon", kParams_380};
const steammock::SlotInfo kCall_641 = {"SteamAPI_ISteamParties_GetBeaconLocationData", kParams_381};
const steammock::SlotInfo kCall_642 = {"SteamAPI_ISteamRemotePlay_GetSessionCount", nullptr};
const steammock::SlotInfo kCall_643 = {"SteamAPI_ISteamRemotePlay_GetSessionID", kParams_382};
const steammock::SlotInfo kCall_644 = {"SteamAPI_ISteamRemotePlay_GetSessionSteamID", kParams_383};
const steammock::SlotInfo kCall_645 = {"SteamAPI_ISteamRemotePlay_GetSessionClientName", kParams_383};
const steammock::SlotInfo kCall_646 = {"SteamAPI_ISteamRemotePlay_GetSessionClientFormFactor", kParams_383};
const steammock::SlotInfo kCall_647 = {"SteamAPI_ISteamRemotePlay_BGetSessionClientResolution", kParams_384};
const steammock::SlotInfo kCall_648 = {"SteamAPI_ISteamRemotePlay_BSendRemotePlayTogetherInvite", kParams_52};
const steammock::SlotInfo kCall_649 = {"SteamAPI_ISteamRemoteStorage_FileWrite", kParams_385};
const steammock::SlotInfo kCall_650 = {"SteamAPI_ISteamRemoteStorage_FileRead", kParams_386};
const steammock::SlotInfo kCall_651 = {"SteamAPI_ISteamRemoteStorage_FileWriteAsync", kParams_385};
const steammock::SlotInfo kCall_652 = {"SteamAPI_ISteamRemoteStorage_FileReadAsync", kParams_387};
const steammock::SlotInfo kCall_653 = {"SteamAPI_ISteamRemoteStorage_FileReadAsyncComplete", kParams_388};
const steammock::SlotInfo kCall_654 = {"SteamAPI_ISteamRemoteStorage_FileForget", kParams_389};
const steammock::SlotInfo kCall_655 = {"SteamAPI_ISteamRemoteStorage_FileDelete", kParams_389};
const steammock::SlotInfo kCall_656 = {"SteamAPI_ISteamRemoteStorage_FileShare", kParams_389};
const steammock::SlotInfo kCall_657 = {"SteamAPI_ISteamRemoteStorage_SetSyncPlatforms", kParams_390};
const steammock::SlotInfo kCall_658 = {"SteamAPI_ISteamRemoteStorage_FileWriteStreamOpen", kParams_389};
const steammock::SlotInfo kCall_659 = {"SteamAPI_ISteamRemoteStorage_FileWriteStreamWriteChunk", kParams_391};
const steammock::SlotInfo kCall_660 = {"SteamAPI_ISteamRemoteStorage_FileWriteStreamClose", kParams_392};
const steammock::SlotInfo kCall_661 = {"SteamAPI_ISteamRemoteStorage_FileWriteStreamCancel", kParams_392};
const steammock::SlotInfo kCall_662 = {"SteamAPI_ISteamRemoteStorage_FileExists", kParams_389};
const steammock::SlotInfo kCall_663 = {"SteamAPI_ISteamRemoteStorage_FilePersisted", kParams_389};
const steammock::SlotInfo kCall_664 = {"SteamAPI_ISteamRemoteStorage_GetFileSize", kParams_389};
const steammock::SlotInfo kCall_665 = {"SteamAPI_ISteamRemoteStorage_GetFileTimestamp", kParams_389};
const steammock::SlotInfo kCall_666 = {"SteamAPI_ISteamRemoteStorage_GetSyncPlatforms", kParams_389};
const steammock::SlotInfo kCall_667 = {"SteamAPI_ISteamRemoteStorage_GetFileCount", nullptr};
const steammock::SlotInfo kCall_668 = {"SteamAPI_ISteamRemoteStorage_GetFileNameAndSize", kParams_393};
const steammock::SlotInfo kCall_669 = {"SteamAPI_ISteamRemoteStorage_GetQuota", kParams_394};
const steammock::SlotInfo kCall_670 = {"SteamAPI_ISteamRemoteStorage_IsCloudEnabledForAccount", nullptr};
const steammock::SlotInfo kCall_671 = {"SteamAPI_ISteamRemoteStorage_IsCloudEnabledForApp", nullptr};
const steammock::SlotInfo kCall_672 = {"SteamAPI_ISteamRemoteStorage_SetCloudEnabledForApp", kParams_395};
const steammock::SlotInfo kCall_673 = {"SteamAPI_ISteamRemoteStorage_UGCDownload", kParams_396};
const steammock::SlotInfo kCall_674 = {"SteamAPI_ISteamRemoteStorage_GetUGCDownloadProgress", kParams_397};
const steammock::SlotInfo kCall_675 = {"SteamAPI_ISteamRemoteStorage_GetUGCDetails", kParams_398};
const steammock::SlotInfo kCall_676 = {"SteamAPI_ISteamRemoteStorage_UGCRead", kParams_399};
const steammock::SlotInfo kCall_677 = {"SteamAPI_ISteamRemoteStorage_GetCachedUGCCount", nullptr};
const steammock::SlotInfo kCall_678 = {"SteamAPI_ISteamRemoteStorage_GetCachedUGCHandle", kParams_400};
const steammock::SlotInfo kCall_679 = {"SteamAPI_ISteamRemoteStorage_PublishWorkshopFile", kParams_401};
const steammock::SlotInfo kCall_680 = {"SteamAPI_ISteamRemoteStorage_CreatePublishedFileUpdateRequest", kParams_402};
const steammock::SlotInfo kCall_681 = {"SteamAPI_ISteamRemoteStorage_UpdatePublishedFileFile", kParams_403};
const steammock::SlotInfo kCall_682 = {"SteamAPI_ISteamRemoteStorage_UpdatePublishedFilePreviewFile", kParams_404};
const steammock::SlotInfo kCall_683 = {"SteamAPI_ISteamRemoteStorage_UpdatePublishedFileTitle", kParams_405};
const steammock::SlotInfo kCall_684 = {"SteamAPI_ISteamRemoteStorage_UpdatePublishedFileDescription", kParams_406};
const steammock::SlotInfo kCall_685 = {"SteamAPI_ISteamRemoteStorage_UpdatePublishedFileVisibility", kParams_407};
const steammock::SlotInfo kCall_686 = {"SteamAPI_ISteamRemoteStorage_UpdatePublishedFileTags", kParams_408};
const steammock::SlotInfo kCall_687 = {"SteamAPI_ISteamRemoteStorage_CommitPublishedFileUpdate", kParams_409};
const steammock::SlotInfo kCall_688 = {"SteamAPI_ISteamRemoteStorage_GetPublishedFileDetails", kParams_410};
const steammock::SlotInfo kCall_689 = {"SteamAPI_ISteamRemoteStorage_DeletePublishedFile", kParams_402};
const steammock::SlotInfo kCall_690 = {"SteamAPI_ISteamRemoteStorage_EnumerateUserPublishedFiles", kParams_90};
const steammock::SlotInfo kCall_691 = {"SteamAPI_ISteamRemoteStorage_SubscribePublishedFile", kParams_402};
const steammock::SlotInfo kCall_692 = {"SteamAPI_ISteamRemoteStorage_EnumerateUserSubscribedFiles", kParams_90};
const steammock::SlotInfo kCall_693 = {"SteamAPI_ISteamRemoteStorage_UnsubscribePublishedFile", kParams_402};
const steammock::SlotInfo kCall_694 = {"SteamAPI_ISteamRemoteStorage_UpdatePublishedFileSetChangeDescription", kParams_411};
const steammock::SlotInfo kCall_695 = {"SteamAPI_ISteamRemoteStorage_GetPublishedItemVoteDetails", kParams_402};
const steammock::SlotInfo kCall_696 = {"SteamAPI_ISteamRemoteStorage_UpdateUserPublishedItemVote", kParams_412};
const steammock::SlotInfo kCall_697 = {"SteamAPI_ISteamRemoteStorage_GetUserPublishedItemVoteDetails", kParams_402};
const steammock::SlotInfo kCall_698 = {"SteamAPI_ISteamRemoteStorage_EnumerateUserSharedWorkshopFiles", kParams_413};
const steammock::SlotInfo kCall_699 = {"SteamAPI_ISteamRemoteStorage_PublishVideo", kParams_414};
const steammock::SlotInfo kCall_700 = {"SteamAPI_ISteamRemoteStorage_SetUserPublishedFileAction", kParams_415};
const steammock::SlotInfo kCall_701 = {"SteamAPI_ISteamRemoteStorage_EnumeratePublishedFilesByUserAction", kParams_416};
const steammock::SlotInfo kCall_702 = {"SteamAPI_ISteamRemoteStorage_EnumeratePublishedWorkshopFiles", kParams_417};
const steammock::SlotInfo kCall_703 = {"SteamAPI_ISteamRemoteStorage_UGCDownloadToLocation", kParams_418};
const steammock::SlotInfo kCall_704 = {"SteamAPI_ISteamRemoteStorage_GetLocalFileChangeCount", nullptr};
const steammock::SlotInfo kCall_705 = {"SteamAPI_ISteamRemoteStorage_GetLocalFileChange", kParams_419};
const steammock::SlotInfo kCall_706 = {"SteamAPI_ISteamRemoteStorage_BeginFileWriteBatch", nullptr};
const steammock::SlotInfo kCall_707 = {"SteamAPI_ISteamRemoteStorage_EndFileWriteBatch", nullptr};
const steammock::SlotInfo kCall_708 = {"SteamAPI_ISteamScreenshots_WriteScreenshot", kParams_420};
const steammock::SlotInfo kCall_709 = {"SteamAPI_ISteamScreenshots_AddScreenshotToLibrary", kParams_421};
const steammock::SlotInfo kCall_710 = {"SteamAPI_ISteamScreenshots_TriggerScreenshot", nullptr};
const steammock::SlotInfo kCall_711 = {"SteamAPI_ISteamScreenshots_HookScreenshots", kParams_422};
const steammock::SlotInfo kCall_712 = {"SteamAPI_ISteamScreenshots_SetLocation", kParams_423};
const steammock::SlotInfo kCall_713 = {"SteamAPI_ISteamScreenshots_TagUser", kParams_424};
const steammock::SlotInfo kCall_714 = {"SteamAPI_ISteamScreenshots_TagPublishedFile", kParams_425};
const steammock::SlotInfo kCall_715 = {"SteamAPI_ISteamScreenshots_IsScreenshotsHooked", nullptr};
const steammock::SlotInfo kCall_716 = {"SteamAPI_ISteamScreenshots_AddVRScreenshotToLibrary", kParams_426};
const steammock::SlotInfo kCall_717 = {"SteamAPI_ISteamUGC_CreateQueryUserUGCRequest", kParams_427};
const steammock::SlotInfo kCall_718 = {"SteamAPI_ISteamUGC_CreateQueryAllUGCRequestPage", kParams_428};
const steammock::SlotInfo kCall_719 = {"SteamAPI_ISteamUGC_CreateQueryUGCDetailsRequest", kParams_429};
const steammock::SlotInfo kCall_720 = {"SteamAPI_ISteamUGC_SendQueryUGCRequest", kParams_430};
const steammock::SlotInfo kCall_721 = {"SteamAPI_ISteamUGC_GetQueryUGCResult", kParams_431};
const steammock::SlotInfo kCall_722 = {"SteamAPI_ISteamUGC_GetQueryUGCPreviewURL", kParams_432};
const steammock::SlotInfo kCall_723 = {"SteamAPI_ISteamUGC_GetQueryUGCMetadata", kParams_433};
const steammock::SlotInfo kCall_724 = {"SteamAPI_ISteamUGC_GetQueryUGCChildren", kParams_434};
const steammock::SlotInfo kCall_725 = {"SteamAPI_ISteamUGC_GetQueryUGCStatistic", kParams_435};
const steammock::SlotInfo kCall_726 = {"SteamAPI_ISteamUGC_GetQueryUGCNumAdditionalPreviews", kParams_436};
const steammock::SlotInfo kCall_727 = {"SteamAPI_ISteamUGC_GetQueryUGCAdditionalPreview", kParams_437};
const steammock::SlotInfo kCall_728 = {"SteamAPI_ISteamUGC_GetQueryUGCNumKeyValueTags", kParams_436};
const steammock::SlotInfo kCall_729 = {"SteamAPI_ISteamUGC_GetQueryUGCKeyValueTag", kParams_438};
const steammock::SlotInfo kCall_730 = {"SteamAPI_ISteamUGC_ReleaseQueryUGCRequest", kParams_430};
const steammock::SlotInfo kCall_731 = {"SteamAPI_ISteamUGC_AddRequiredTag", kParams_439};
const steammock::SlotInfo kCall_732 = {"SteamAPI_ISteamUGC_AddExcludedTag", kParams_439};
const steammock::SlotInfo kCall_733 = {"SteamAPI_ISteamUGC_SetReturnOnlyIDs", kParams_440};
const steammock::SlotInfo kCall_734 = {"SteamAPI_ISteamUGC_SetReturnKeyValueTags", kParams_441};
const steammock::SlotInfo kCall_735 = {"SteamAPI_ISteamUGC_SetReturnLongDescription", kParams_442};
const steammock::SlotInfo kCall_736 = {"SteamAPI_ISteamUGC_SetReturnMetadata", kParams_443};
const steammock::SlotInfo kCall_737 = {"SteamAPI_ISteamUGC_SetReturnChildren", kParams_444};
const steammock::SlotInfo kCall_738 = {"SteamAPI_ISteamUGC_SetReturnAdditionalPreviews", kParams_445};
const steammock::SlotInfo kCall_739 = {"SteamAPI_ISteamUGC_SetReturnTotalOnly", kParams_446};
const steammock::SlotInfo kCall_740 = {"SteamAPI_ISteamUGC_SetReturnPlaytimeStats", kParams_447};
const steammock::SlotInfo kCall_741 = {"SteamAPI_ISteamUGC_SetLanguage", kParams_448};
const steammock::SlotInfo kCall_742 = {"SteamAPI_ISteamUGC_SetAllowCachedResponse", kParams_449};
const steammock::SlotInfo kCall_743 = {"SteamAPI_ISteamUGC_SetCloudFileNameFilter", kParams_450};
const steammock::SlotInfo kCall_744 = {"SteamAPI_ISteamUGC_SetMatchAnyTag", kParams_451};
const steammock::SlotInfo kCall_745 = {"SteamAPI_ISteamUGC_SetSearchText", kParams_452};
const steammock::SlotInfo kCall_746 = {"SteamAPI_ISteamUGC_SetRankedByTrendDays", kParams_447};
const steammock::SlotInfo kCall_747 = {"SteamAPI_ISteamUGC_AddRequiredKeyValueTag", kParams_453};
const steammock::SlotInfo kCall_748 = {"SteamAPI_ISteamUGC_RequestUGCDetails", kParams_454};
const steammock::SlotInfo kCall_749 = {"SteamAPI_ISteamUGC_CreateItem", kParams_455};
const steammock::SlotInfo kCall_750 = {"SteamAPI_ISteamUGC_StartItemUpdate", kParams_456};
const steammock::SlotInfo kCall_751 = {"SteamAPI_ISteamUGC_SetItemTitle", kParams_457};
const steammock::SlotInfo kCall_752 = {"SteamAPI_ISteamUGC_SetItemDescription", kParams_458};
const steammock::SlotInfo kCall_753 = {"SteamAPI_ISteamUGC_SetItemUpdateLanguage", kParams_448};
const steammock::SlotInfo kCall_754 = {"SteamAPI_ISteamUGC_SetItemMetadata", kParams_459};
const steammock::SlotInfo kCall_755 = {"SteamAPI_ISteamUGC_SetItemVisibility", kParams_460};
const steammock::SlotInfo kCall_756 = {"SteamAPI_ISteamUGC_SetItemTags", kParams_408};
const steammock::SlotInfo kCall_757 = {"SteamAPI_ISteamUGC_SetItemContent", kParams_461};
const steammock::SlotInfo kCall_758 = {"SteamAPI_ISteamUGC_SetItemPreview", kParams_462};
const steammock::SlotInfo kCall_759 = {"SteamAPI_ISteamUGC_RemoveItemKeyValueTags", kParams_463};
const steammock::SlotInfo kCall_760 = {"SteamAPI_ISteamUGC_AddItemKeyValueTag", kParams_464};
const steammock::SlotInfo kCall_761 = {"SteamAPI_ISteamUGC_AddItemPreviewFile", kParams_465};
const steammock::SlotInfo kCall_762 = {"SteamAPI_ISteamUGC_AddItemPreviewVideo", kParams_466};
const steammock::SlotInfo kCall_763 = {"SteamAPI_ISteamUGC_UpdateItemPreviewFile", kParams_467};
const steammock::SlotInfo kCall_764 = {"SteamAPI_ISteamUGC_UpdateItemPreviewVideo", kParams_468};
const steammock::SlotInfo kCall_765 = {"SteamAPI_ISteamUGC_RemoveItemPreview", kParams_436};
const steammock::SlotInfo kCall_766 = {"SteamAPI_ISteamUGC_SubmitItemUpdate", kParams_469};
const steammock::SlotInfo kCall_767 = {"SteamAPI_ISteamUGC_GetItemUpdateProgress", kParams_470};
const steammock::SlotInfo kCall_768 = {"SteamAPI_ISteamUGC_SetUserItemVote", kParams_471};
const steammock::SlotInfo kCall_769 = {"SteamAPI_ISteamUGC_GetUserItemVote", kParams_472};
const steammock::SlotInfo kCall_770 = {"SteamAPI_ISteamUGC_AddItemToFavorites", kParams_473};
const steammock::SlotInfo kCall_771 = {"SteamAPI_ISteamUGC_RemoveItemFromFavorites", kParams_473};
const steammock::SlotInfo kCall_772 = {"SteamAPI_ISteamUGC_SubscribeItem", kParams_472};
const steammock::SlotInfo kCall_773 = {"SteamAPI_ISteamUGC_UnsubscribeItem", kParams_472};
const steammock::SlotInfo kCall_774 = {"SteamAPI_ISteamUGC_GetNumSubscribedItems", nullptr};
const steammock::SlotInfo kCall_775 = {"SteamAPI_ISteamUGC_GetSubscribedItems", kParams_474};
const steammock::SlotInfo kCall_776 = {"SteamAPI_ISteamUGC_GetItemState", kParams_472};
const steammock::SlotInfo kCall_777 = {"SteamAPI_ISteamUGC_GetItemInstallInfo", kParams_475};
const steammock::SlotInfo kCall_778 = {"SteamAPI_ISteamUGC_GetItemDownloadInfo", kParams_476};
const steammock::SlotInfo kCall_779 = {"SteamAPI_ISteamUGC_DownloadItem", kParams_477};
const steammock::SlotInfo kCall_780 = {"SteamAPI_ISteamUGC_BInitWorkshopForGameServer", kParams_478};
const steammock::SlotInfo kCall_781 = {"SteamAPI_ISteamUGC_SuspendDownloads", kParams_479};
const steammock::SlotInfo kCall_782 = {"SteamAPI_ISteamUGC_StartPlaytimeTracking", kParams_429};
const steammock::SlotInfo kCall_783 = {"SteamAPI_ISteamUGC_StopPlaytimeTracking", kParams_429};
const steammock::SlotInfo kCall_784 = {"SteamAPI_ISteamUGC_StopPlaytimeTrackingForAllItems", nullptr};
const steammock::SlotInfo kCall_785 = {"SteamAPI_ISteamUGC_AddDependency", kParams_480};
const steammock::SlotInfo kCall_786 = {"SteamAPI_ISteamUGC_RemoveDependency", kParams_480};
const steammock::SlotInfo kCall_787 = {"SteamAPI_ISteamUGC_AddAppDependency", kParams_481};
const steammock::SlotInfo kCall_788 = {"SteamAPI_ISteamUGC_RemoveAppDependency", kParams_481};
const steammock::SlotInfo kCall_789 = {"SteamAPI_ISteamUGC_GetAppDependencies", kParams_472};
const steammock::SlotInfo kCall_790 = {"SteamAPI_ISteamUGC_DeleteItem", kParams_472};
const steammock::SlotInfo kCall_791 = {"SteamAPI_ISteamUGC_CreateQueryAllUGCRequestCursor", kParams_482};
const steammock::SlotInfo kCall_792 = {"SteamAPI_ISteamUGC_GetQueryFirstUGCKeyValueTag", kParams_483};
const steammock::SlotInfo kCall_793 = {"SteamAPI_ISteamUGC_SetAllowLegacyUpload", kParams_484};
const steammock::SlotInfo kCall_794 = {"SteamAPI_ISteamUGC_RemoveAllItemKeyValueTags", kParams_430};
const steammock::SlotInfo kCall_795 = {"SteamAPI_ISteamUGC_AddRequiredTagGroup", kParams_485};
const steammock::SlotInfo kCall_796 = {"SteamAPI_ISteamUGC_GetQueryUGCNumTags", kParams_436};
const steammock::SlotInfo kCall_797 = {"SteamAPI_ISteamUGC_GetQueryUGCTag", kParams_486};
const steammock::SlotInfo kCall_798 = {"SteamAPI_ISteamUGC_GetQueryUGCTagDisplayName", kParams_486};
const steammock::SlotInfo kCall_799 = {"SteamAPI_ISteamUGC_SetTimeCreatedDateRange", kParams_487};
const steammock::SlotInfo kCall_800 = {"SteamAPI_ISteamUGC_SetTimeUpdatedDateRange", kParams_487};
const steammock::SlotInfo kCall_801 = {"SteamAPI_ISteamUGC_ShowWorkshopEULA", nullptr};
const steammock::SlotInfo kCall_802 = {"SteamAPI_ISteamUGC_GetWorkshopEULAStatus", nullptr};
const steammock::SlotInfo kCall_803 = {"SteamAPI_ISteamUGC_GetQueryUGCContentDescriptors", kParams_488};
const steammock::SlotInfo kCall_804 = {"SteamAPI_ISteamUGC_AddContentDescriptor", kParams_489};
const steammock::SlotInfo kCall_805 = {"SteamAPI_ISteamUGC_RemoveContentDescriptor", kParams_489};
const steammock::SlotInfo kCall_806 = {"SteamAPI_ISteamUnifiedMessages_SendMethod", kParams_490};
const steammock::SlotInfo kCall_807 = {"SteamAPI_ISteamUnifiedMessages_GetMethodResponseInfo", kParams_491};
const steammock::SlotInfo kCall_808 = {"SteamAPI_ISteamUnifiedMessages_GetMethodResponseData", kParams_492};
const steammock::SlotInfo kCall_809 = {"SteamAPI_ISteamUnifiedMessages_ReleaseMethod", kParams_493};
const steammock::SlotInfo kCall_810 = {"SteamAPI_ISteamUnifiedMessages_SendNotification", kParams_494};
const steammock::SlotInfo kCall_811 = {"SteamAPI_ISteamUser_GetHSteamUser", nullptr};
const steammock::SlotInfo kCall_812 = {"SteamAPI_ISteamUser_BLoggedOn", nullptr};
const steammock::SlotInfo kCall_813 = {"SteamAPI_ISteamUser_GetSteamID", nullptr};
const steammock::SlotInfo kCall_814 = {"SteamAPI_ISteamUser_InitiateGameConnection", kParams_495};
const steammock::SlotInfo kCall_815 = {"SteamAPI_ISteamUser_TerminateGameConnection", kParams_496};
const steammock::SlotInfo kCall_816 = {"SteamAPI_ISteamUser_TrackAppUsageEvent", kParams_497};
const steammock::SlotInfo kCall_817 = {"SteamAPI_ISteamUser_GetUserDataFolder", kParams_498};
const steammock::SlotInfo kCall_818 = {"SteamAPI_ISteamUser_StartVoiceRecording", nullptr};
const steammock::SlotInfo kCall_819 = {"SteamAPI_ISteamUser_StopVoiceRecording", nullptr};
const steammock::SlotInfo kCall_820 = {"SteamAPI_ISteamUser_GetAvailableVoice", kParams_499};
const steammock::SlotInfo kCall_821 = {"SteamAPI_ISteamUser_GetVoice", kParams_500};
const steammock::SlotInfo kCall_822 = {"SteamAPI_ISteamUser_DecompressVoice", kParams_501};
const steammock::SlotInfo kCall_823 = {"SteamAPI_ISteamUser_GetVoiceOptimalSampleRate", nullptr};
const steammock::SlotInfo kCall_824 = {"SteamAPI_ISteamUser_GetAuthSessionTicket", kParams_124};
const steammock::SlotInfo kCall_825 = {"SteamAPI_ISteamUser_BeginAuthSession", kParams_125};
const steammock::SlotInfo kCall_826 = {"SteamAPI_ISteamUser_EndAuthSession", kParams_89};
const steammock::SlotInfo kCall_827 = {"SteamAPI_ISteamUser_CancelAuthTicket", kParams_126};
const steammock::SlotInfo kCall_828 = {"SteamAPI_ISteamUser_UserHasLicenseForApp", kParams_127};
const steammock::SlotInfo kCall_829 = {"SteamAPI_ISteamUser_BIsBehindNAT", nullptr};
const steammock::SlotInfo kCall_830 = {"SteamAPI_ISteamUser_AdvertiseGame", kParams_502};
const steammock::SlotInfo kCall_831 = {"SteamAPI_ISteamUser_RequestEncryptedAppTicket", kParams_503};
const steammock::SlotInfo kCall_832 = {"SteamAPI_ISteamUser_GetEncryptedAppTicket", kParams_124};
const steammock::SlotInfo kCall_833 = {"SteamAPI_ISteamUser_GetGameBadgeLevel", kParams_504};
const steammock::SlotInfo kCall_834 = {"SteamAPI_ISteamUser_GetPlayerSteamLevel", nullptr};
const steammock::SlotInfo kCall_835 = {"SteamAPI_ISteamUser_RequestStoreAuthURL", kParams_505};
const steammock::SlotInfo kCall_836 = {"SteamAPI_ISteamUser_BIsPhoneVerified", nullptr};
const steammock::SlotInfo kCall_837 = {"SteamAPI_ISteamUser_BIsTwoFactorEnabled", nullptr};
const steammock::SlotInfo kCall_838 = {"SteamAPI_ISteamUser_BIsPhoneIdentifying", nullptr};
const steammock::SlotInfo kCall_839 = {"SteamAPI_ISteamUser_BIsPhoneRequiringVerification", nullptr};
const steammock::SlotInfo kCall_840 = {"SteamAPI_ISteamUser_GetMarketEligibility", nullptr};
const steammock::SlotInfo kCall_841 = {"SteamAPI_ISteamUser_GetDurationControl", nullptr};
const steammock::SlotInfo kCall_842 = {"SteamAPI_ISteamUser_InitiateGameConnection_DEPRECATED", kParams_495};
const steammock::SlotInfo kCall_843 = {"SteamAPI_ISteamUser_TerminateGameConnection_DEPRECATED", kParams_496};
const steammock::SlotInfo kCall_844 = {"SteamAPI_ISteamUser_BSetDurationControlOnlineState", kParams_506};
const steammock::SlotInfo kCall_845 = {"SteamAPI_ISteamUser_GetAuthSessionTicket", kParams_507};
const steammock::SlotInfo kCall_846 = {"SteamAPI_ISteamUser_GetAuthTicketForWebApi", kParams_508};
const steammock::SlotInfo kCall_847 = {"SteamAPI_ISteamUserStats_RequestCurrentStats", nullptr};
const steammock::SlotInfo kCall_848 = {"SteamAPI_ISteamUserStats_GetStatInt32", kParams_509};
const steammock::SlotInfo kCall_849 = {"SteamAPI_ISteamUserStats_GetStatFloat", kParams_509};
const steammock::SlotInfo kCall_850 = {"SteamAPI_ISteamUserStats_SetStatInt32", kParams_510};
const steammock::SlotInfo kCall_851 = {"SteamAPI_ISteamUserStats_SetStatFloat", kParams_511};
const steammock::SlotInfo kCall_852 = {"SteamAPI_ISteamUserStats_UpdateAvgRateStat", kParams_512};
const steammock::SlotInfo kCall_853 = {"SteamAPI_ISteamUserStats_GetAchievement", kParams_513};
const steammock::SlotInfo kCall_854 = {"SteamAPI_ISteamUserStats_SetAchievement", kParams_274};
const steammock::SlotInfo kCall_855 = {"SteamAPI_ISteamUserStats_ClearAchievement", kParams_274};
const steammock::SlotInfo kCall_856 = {"SteamAPI_ISteamUserStats_GetAchievementAndUnlockTime", kParams_514};
const steammock::SlotInfo kCall_857 = {"SteamAPI_ISteamUserStats_StoreStats", nullptr};
const steammock::SlotInfo kCall_858 = {"SteamAPI_ISteamUserStats_GetAchievementIcon", kParams_274};
const steammock::SlotInfo kCall_859 = {"SteamAPI_ISteamUserStats_GetAchievementDisplayAttribute", kParams_515};
const steammock::SlotInfo kCall_860 = {"SteamAPI_ISteamUserStats_IndicateAchievementProgress", kParams_516};
const steammock::SlotInfo kCall_861 = {"SteamAPI_ISteamUserStats_GetNumAchievements", nullptr};
const steammock::SlotInfo kCall_862 = {"SteamAPI_ISteamUserStats_GetAchievementName", kParams_517};
const steammock::SlotInfo kCall_863 = {"SteamAPI_ISteamUserStats_RequestUserStats", kParams_122};
const steammock::SlotInfo kCall_864 = {"SteamAPI_ISteamUserStats_GetUserStatInt32", kParams_135};
const steammock::SlotInfo kCall_865 = {"SteamAPI_ISteamUserStats_GetUserStatFloat", kParams_135};
const steammock::SlotInfo kCall_866 = {"SteamAPI_ISteamUserStats_GetUserAchievement", kParams_136};
const steammock::SlotInfo kCall_867 = {"SteamAPI_ISteamUserStats_GetUserAchievementAndUnlockTime", kParams_518};
const steammock::SlotInfo kCall_868 = {"SteamAPI_ISteamUserStats_ResetAllStats", kParams_519};
const steammock::SlotInfo kCall_869 = {"SteamAPI_ISteamUserStats_FindOrCreateLeaderboard", kParams_520};
const steammock::SlotInfo kCall_870 = {"SteamAPI_ISteamUserStats_FindLeaderboard", kParams_521};
const steammock::SlotInfo kCall_871 = {"SteamAPI_ISteamUserStats_GetLeaderboardName", kParams_522};
const steammock::SlotInfo kCall_872 = {"SteamAPI_ISteamUserStats_GetLeaderboardEntryCount", kParams_522};
const steammock::SlotInfo kCall_873 = {"SteamAPI_ISteamUserStats_GetLeaderboardSortMethod", kParams_522};
const steammock::SlotInfo kCall_874 = {"SteamAPI_ISteamUserStats_GetLeaderboardDisplayType", kParams_522};
const steammock::SlotInfo kCall_875 = {"SteamAPI_ISteamUserStats_DownloadLeaderboardEntries", kParams_523};
const steammock::SlotInfo kCall_876 = {"SteamAPI_ISteamUserStats_DownloadLeaderboardEntriesForUsers", kParams_524};
const steammock::SlotInfo kCall_877 = {"SteamAPI_ISteamUserStats_GetDownloadedLeaderboardEntry", kParams_525};
const steammock::SlotInfo kCall_878 = {"SteamAPI_ISteamUserStats_UploadLeaderboardScore", kParams_526};
const steammock::SlotInfo kCall_879 = {"SteamAPI_ISteamUserStats_AttachLeaderboardUGC", kParams_527};
const steammock::SlotInfo kCall_880 = {"SteamAPI_ISteamUserStats_GetNumberOfCurrentPlayers", nullptr};
const steammock::SlotInfo kCall_881 = {"SteamAPI_ISteamUserStats_RequestGlobalAchievementPercentages", nullptr};
const steammock::SlotInfo kCall_882 = {"SteamAPI_ISteamUserStats_GetMostAchievedAchievementInfo", kParams_528};
const steammock::SlotInfo kCall_883 = {"SteamAPI_ISteamUserStats_GetNextMostAchievedAchievementInfo", kParams_529};
const steammock::SlotInfo kCall_884 = {"SteamAPI_ISteamUserStats_GetAchievementAchievedPercent", kParams_530};
const steammock::SlotInfo kCall_885 = {"SteamAPI_ISteamUserStats_RequestGlobalStats", kParams_531};
const steammock::SlotInfo kCall_886 = {"SteamAPI_ISteamUserStats_GetGlobalStatInt64", kParams_532};
const steammock::SlotInfo kCall_887 = {"SteamAPI_ISteamUserStats_GetGlobalStatDouble", kParams_532};
const steammock::SlotInfo kCall_888 = {"SteamAPI_ISteamUserStats_GetGlobalStatHistoryInt64", kParams_533};
const steammock::SlotInfo kCall_889 = {"SteamAPI_ISteamUserStats_GetGlobalStatHistoryDouble", kParams_533};
const steammock::SlotInfo kCall_890 = {"SteamAPI_ISteamUserStats_GetAchievementProgressLimitsInt32", kParams_534};
const steammock::SlotInfo kCall_891 = {"SteamAPI_ISteamUserStats_GetAchievementProgressLimitsFloat", kParams_535};
const steammock::SlotInfo kCall_892 = {"SteamAPI_ISteamUtils_GetSecondsSinceAppActive", nullptr};
const steammock::SlotInfo kCall_893 = {"SteamAPI_ISteamUtils_GetSecondsSinceComputerActive", nullptr};
const steammock::SlotInfo kCall_894 = {"SteamAPI_ISteamUtils_GetConnectedUniverse", nullptr};
const steammock::SlotInfo kCall_895 = {"SteamAPI_ISteamUtils_GetServerRealTime", nullptr};
const steammock::SlotInfo kCall_896 = {"SteamAPI_ISteamUtils_GetIPCountry", nullptr};
const steammock::SlotInfo kCall_897 = {"SteamAPI_ISteamUtils_GetImageSize", kParams_536};
const steammock::SlotInfo kCall_898 = {"SteamAPI_ISteamUtils_GetImageRGBA", kParams_537};
const steammock::SlotInfo kCall_899 = {"SteamAPI_ISteamUtils_GetCSERIPPort", kParams_19};
const steammock::SlotInfo kCall_900 = {"SteamAPI_ISteamUtils_GetCurrentBatteryPower", nullptr};
const steammock::SlotInfo kCall_901 = {"SteamAPI_ISteamUtils_GetAppID", nullptr};
const steammock::SlotInfo kCall_902 = {"SteamAPI_ISteamUtils_SetOverlayNotificationPosition", kParams_538};
const steammock::SlotInfo kCall_903 = {"SteamAPI_ISteamUtils_IsAPICallCompleted", kParams_539};
const steammock::SlotInfo kCall_904 = {"SteamAPI_ISteamUtils_GetAPICallFailureReason", kParams_540};
const steammock::SlotInfo kCall_905 = {"SteamAPI_ISteamUtils_GetAPICallResult", kParams_541};
const steammock::SlotInfo kCall_906 = {"ISteamUtils::RunFrame", nullptr};
const steammock::SlotInfo kCall_907 = {"SteamAPI_ISteamUtils_GetIPCCallCount", nullptr};
const steammock::SlotInfo kCall_908 = {"SteamAPI_ISteamUtils_SetWarningMessageHook", kParams_22};
const steammock::SlotInfo kCall_909 = {"SteamAPI_ISteamUtils_IsOverlayEnabled", nullptr};
const steammock::SlotInfo kCall_910 = {"SteamAPI_ISteamUtils_BOverlayNeedsPresent", nullptr};
const steammock::SlotInfo kCall_911 = {"SteamAPI_ISteamUtils_CheckFileSignature", kParams_542};
const steammock::SlotInfo kCall_912 = {"SteamAPI_ISteamUtils_ShowGamepadTextInput", kParams_543};
const steammock::SlotInfo kCall_913 = {"SteamAPI_ISteamUtils_GetEnteredGamepadTextLength", nullptr};
const steammock::SlotInfo kCall_914 = {"SteamAPI_ISteamUtils_GetEnteredGamepadTextInput", kParams_544};
const steammock::SlotInfo kCall_915 = {"SteamAPI_ISteamUtils_GetSteamUILanguage", nullptr};
const steammock::SlotInfo kCall_916 = {"SteamAPI_ISteamUtils_IsSteamRunningInVR", nullptr};
const steammock::SlotInfo kCall_917 = {"SteamAPI_ISteamUtils_SetOverlayNotificationInset", kParams_545};
const steammock::SlotInfo kCall_918 = {"SteamAPI_ISteamUtils_IsSteamInBigPictureMode", nullptr};
const steammock::SlotInfo kCall_919 = {"SteamAPI_ISteamUtils_StartVRDashboard", nullptr};
const steammock::SlotInfo kCall_920 = {"SteamAPI_ISteamUtils_IsVRHeadsetStreamingEnabled", nullptr};
const steammock::SlotInfo kCall_921 = {"SteamAPI_ISteamUtils_SetVRHeadsetStreamingEnabled", kParams_395};
const steammock::SlotInfo kCall_922 = {"SteamAPI_ISteamUtils_IsSteamChinaLauncher", nullptr};
const steammock::SlotInfo kCall_923 = {"SteamAPI_ISteamUtils_InitFilterText", nullptr};
const steammock::SlotInfo kCall_924 = {"SteamAPI_ISteamUtils_FilterText", kParams_546};
const steammock::SlotInfo kCall_925 = {"SteamAPI_ISteamUtils_GetIPv6ConnectivityState", kParams_547};
const steammock::SlotInfo kCall_926 = {"ISteamUtils::GetCSERIPPort", kParams_19};
const steammock::SlotInfo kCall_927 = {"SteamAPI_ISteamUtils_InitFilterText", kParams_548};
const steammock::SlotInfo kCall_928 = {"SteamAPI_ISteamUtils_FilterText", kParams_549};
const steammock::SlotInfo kCall_929 = {"SteamAPI_ISteamUtils_IsSteamRunningOnSteamDeck", nullptr};
const steammock::SlotInfo kCall_930 = {"SteamAPI_ISteamUtils_ShowFloatingGamepadTextInput", kParams_550};
const steammock::SlotInfo kCall_931 = {"SteamAPI_ISteamUtils_SetGameLauncherMode", kParams_551};
const steammock::SlotInfo kCall_932 = {"SteamAPI_ISteamUtils_DismissFloatingGamepadTextInput", nullptr};
const steammock::SlotInfo kCall_933 = {"SteamAPI_ISteamVideo_GetVideoURL", kParams_552};
const steammock::SlotInfo kCall_934 = {"SteamAPI_ISteamVideo_IsBroadcasting", kParams_553};
const steammock::SlotInfo kCall_935 = {"SteamAPI_ISteamVideo_GetOPFSettings", kParams_552};
const steammock::SlotInfo kCall_936 = {"SteamAPI_ISteamVideo_GetOPFStringForApp", kParams_554};

// ISteamAppList STEAMAPPLIST_INTERFACE_VERSION001
class Version_STEAMAPPLIST_INTERFACE_VERSION001 {
public:
    virtual std::uint32_t GetNumInstalledApps() { return steammock::slot<std::uint32_t>(kCall_0); }
    virtual std::uint32_t GetInstalledApps(std::uint32_t* pvecAppID, std::uint32_t unMaxAppIDs) {
        return steammock::slot<std::uint32_t>(kCall_1, pvecAppID, unMaxAppIDs);
    }
    virtual std::int32_t GetAppName(std::uint32_t nAppID, void* pchName, std::int32_t cchNameMax) {
        return steammock::slot<std::int32_t>(kCall_2, nAppID, pchName, cchNameMax);
    }
    virtual std::int32_t GetAppInstallDir(std::uint32_t nAppID, void* pchDirectory, std::int32_t cchNameMax) {
        return steammock::slot<std::int32_t>(kCall_3, nAppID, pchDirectory, cchNameMax);
    }
    virtual std::int32_t GetAppBuildId(std::uint32_t nAppID) {
        return steammock::slot<std::int32_t>(kCall_4, nAppID);
    }
};

Version_STEAMAPPLIST_INTERFACE_VERSION001 g_STEAMAPPLIST_INTERFACE_VERSION001;

// ISteamApps STEAMAPPS_INTERFACE_VERSION008
class Version_STEAMAPPS_INTERFACE_VERSION008 {
public:
    virtual bool BIsSubscribed() { return steammock::slot<bool>(kCall_5); }
    virtual bool BIsLowViolence() { return steammock::slot<bool>(kCall_6); }
    virtual bool BIsCybercafe() { return steammock::slot<bool>(kCall_7); }
    virtual bool BIsVACBanned() { return steammock::slot<bool>(kCall_8); }
    virtual const char* GetCurrentGameLanguage() { return steammock::slot<const char*>(kCall_9); }
    virtual const char* GetAvailableGameLanguages() {
        return steammock::slot<const char*>(kCall_10);
    }
    virtual bool BIsSubscribedApp(std::uint32_t appID) {
        return steammock::slot<bool>(kCall_11, appID);
    }
    virtual bool BIsDlcInstalled(std::uint32_t appID) {
        return steammock::slot<bool>(kCall_12, appID);
    }
    virtual std::uint32_t GetEarliestPurchaseUnixTime(std::uint32_t nAppID) {
        return steammock::slot<std::uint32_t>(kCall_13, nAppID);
    }
    virtual bool BIsSubscribedFromFreeWeekend() { return steammock::slot<bool>(kCall_14); }
    virtual std::int32_t GetDLCCount() { return steammock::slot<std::int32_t>(kCall_15); }
    virtual bool BGetDLCDataByIndex(std::int32_t iDLC, std::uint32_t* pAppID, bool* pbAvailable, void* pchName, std::int32_t cchNameBufferSize) {
        return steammock::slot<bool>(kCall_16, iDLC, pAppID, pbAvailable, pchName, cchNameBufferSize);
    }
    virtual void InstallDLC(std::uint32_t nAppID) { steammock::slot<void>(kCall_17, nAppID); }
    virtual void UninstallDLC(std::uint32_t nAppID) { steammock::slot<void>(kCall_18, nAppID); }
    virtual void RequestAppProofOfPurchaseKey(std::uint32_t nAppID) {
        steammock::slot<void>(kCall_19, nAppID);
    }
    virtual bool GetCurrentBetaName(void* pchName, std::int32_t cchNameBufferSize) {
        return steammock::slot<bool>(kCall_20, pchName, cchNameBufferSize);
    }
    virtual bool MarkContentCorrupt(bool bMissingFilesOnly) {
        return steammock::slot<bool>(kCall_21, bMissingFilesOnly);
    }
    virtual std::uint32_t GetInstalledDepots(std::uint32_t appID, std::uint32_t* pvecDepots, std::uint32_t cMaxDepots) {
        return steammock::slot<std::uint32_t>(kCall_22, appID, pvecDepots, cMaxDepots);
    }
    virtual std::uint32_t GetAppInstallDir(std::uint32_t appID, void* pchFolder, std::uint32_t cchFolderBufferSize) {
        return steammock::slot<std::uint32_t>(kCall_23, appID, pchFolder, cchFolderBufferSize);
    }
    virtual bool BIsAppInstalled(std::uint32_t appID) {
        return steammock::slot<bool>(kCall_24, appID);
    }
    virtual CSteamID GetAppOwner() { return steammock::slot<CSteamID>(kCall_25); }
    virtual const char* GetLaunchQueryParam(const char* pchKey) {
        return steammock::slot<const char*>(kCall_26, pchKey);
    }
    virtual bool GetDlcDownloadProgress(std::uint32_t nAppID, std::uint64_t* punBytesDownloaded, std::uint64_t* punBytesTotal) {
        return steammock::slot<bool>(kCall_27, nAppID, punBytesDownloaded, punBytesTotal);
    }
    virtual std::int32_t GetAppBuildId() { return steammock::slot<std::int32_t>(kCall_28); }
    virtual void RequestAllProofOfPurchaseKeys() { steammock::slot<void>(kCall_29); }
    virtual std::uint64_t GetFileDetails(const char* pszFileName) {
        return steammock::slot<std::uint64_t>(kCall_30, pszFileName);
    }
    virtual std::int32_t GetLaunchCommandLine(void* pszCommandLine, std::int32_t cubCommandLine) {
        return steammock::slot<std::int32_t>(kCall_31, pszCommandLine, cubCommandLine);
    }
    virtual bool BIsSubscribedFromFamilySharing() { return steammock::slot<bool>(kCall_32); }
    virtual bool BIsTimedTrial(std::uint32_t* punSecondsAllowed, std::uint32_t* punSecondsPlayed) {
        return steammock::slot<bool>(kCall_33, punSecondsAllowed, punSecondsPlayed);
    }
    virtual bool SetDlcContext(std::uint32_t nAppID) {
        return steammock::slot<bool>(kCall_34, nAppID);
    }
};

Version_STEAMAPPS_INTERFACE_VERSION008 g_STEAMAPPS_INTERFACE_VERSION008;

// ISteamClient SteamClient017
class Version_SteamClient017 {
public:
    virtual std::int32_t CreateSteamPipe() { return steammock::slot<std::int32_t>(kCall_35); }
    virtual bool BReleaseSteamPipe(std::int32_t hSteamPipe) {
        return steammock::slot<bool>(kCall_36, hSteamPipe);
    }
    virtual std::int32_t ConnectToGlobalUser(std::int32_t hSteamPipe) {
        return steammock::slot<std::int32_t>(kCall_37, hSteamPipe);
    }
    virtual std::int32_t CreateLocalUser(std::int32_t* phSteamPipe, std::int32_t eAccountType) {
        return steammock::slot<std::int32_t>(kCall_38, phSteamPipe, eAccountType);
    }
    virtual void ReleaseUser(std::int32_t hSteamPipe, std::int32_t hUser) {
        steammock::slot<void>(kCall_39, hSteamPipe, hUser);
    }
    virtual void* GetISteamUser(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_40, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGameServer(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_41, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void SetLocalIPBinding(std::uint32_t unIP, std::uint16_t usPort) {
        steammock::slot<void>(kCall_42, unIP, usPort);
    }
    virtual void* GetISteamFriends(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_43, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamUtils(std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_44, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMatchmaking(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_45, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMatchmakingServers(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_46, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGenericInterface(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_47, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamUserStats(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_48, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGameServerStats(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_49, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamApps(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_50, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamNetworking(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_51, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamRemoteStorage(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_52, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamScreenshots(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_53, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void RunFrame() { steammock::slot<void>(kCall_54); }
    virtual std::uint32_t GetIPCCallCount() { return steammock::slot<std::uint32_t>(kCall_55); }
    virtual void SetWarningMessageHook(void* pFunction) {
        steammock::slot<void>(kCall_56, pFunction);
    }
    virtual bool BShutdownIfAllPipesClosed() { return steammock::slot<bool>(kCall_57); }
    virtual void* GetISteamHTTP(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_58, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamUnifiedMessages(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_59, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamController(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_60, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamUGC(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_61, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamAppList(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_62, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMusic(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_63, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMusicRemote(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_64, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamHTMLSurface(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_65, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void DEPRECATED_Set_SteamAPI_CPostAPIResultInProcess(void* arg0) {
        steammock::slot<void>(kCall_66, arg0);
    }
    virtual void DEPRECATED_Remove_SteamAPI_CPostAPIResultInProcess(void* arg0) {
        steammock::slot<void>(kCall_67, arg0);
    }
    virtual void Set_SteamAPI_CCheckCallbackRegisteredInProcess(void* func) {
        steammock::slot<void>(kCall_68, func);
    }
    virtual void* GetISteamInventory(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_69, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamVideo(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_70, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamParentalSettings(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_71, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
};

Version_SteamClient017 g_SteamClient017;

// ISteamClient SteamClient019
class Version_SteamClient019 {
public:
    virtual std::int32_t CreateSteamPipe() { return steammock::slot<std::int32_t>(kCall_35); }
    virtual bool BReleaseSteamPipe(std::int32_t hSteamPipe) {
        return steammock::slot<bool>(kCall_36, hSteamPipe);
    }
    virtual std::int32_t ConnectToGlobalUser(std::int32_t hSteamPipe) {
        return steammock::slot<std::int32_t>(kCall_37, hSteamPipe);
    }
    virtual std::int32_t CreateLocalUser(std::int32_t* phSteamPipe, std::int32_t eAccountType) {
        return steammock::slot<std::int32_t>(kCall_38, phSteamPipe, eAccountType);
    }
    virtual void ReleaseUser(std::int32_t hSteamPipe, std::int32_t hUser) {
        steammock::slot<void>(kCall_39, hSteamPipe, hUser);
    }
    virtual void* GetISteamUser(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_40, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGameServer(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_41, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void SetLocalIPBinding(std::uint32_t unIP, std::uint16_t usPort) {
        steammock::slot<void>(kCall_42, unIP, usPort);
    }
    virtual void* GetISteamFriends(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_43, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamUtils(std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_44, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMatchmaking(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_45, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMatchmakingServers(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_46, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGenericInterface(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_47, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamUserStats(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_48, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGameServerStats(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_49, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamApps(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_50, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamNetworking(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_51, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamRemoteStorage(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_52, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamScreenshots(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_53, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGameSearch(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_72, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void RunFrame() { steammock::slot<void>(kCall_54); }
    virtual std::uint32_t GetIPCCallCount() { return steammock::slot<std::uint32_t>(kCall_55); }
    virtual void SetWarningMessageHook(void* pFunction) {
        steammock::slot<void>(kCall_56, pFunction);
    }
    virtual bool BShutdownIfAllPipesClosed() { return steammock::slot<bool>(kCall_57); }
    virtual void* GetISteamHTTP(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_58, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* DEPRECATED_GetISteamUnifiedMessages(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_73, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamController(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_60, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamUGC(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_61, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamAppList(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_62, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMusic(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_63, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMusicRemote(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_64, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamHTMLSurface(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_65, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void DEPRECATED_Set_SteamAPI_CPostAPIResultInProcess(void* arg0) {
        steammock::slot<void>(kCall_66, arg0);
    }
    virtual void DEPRECATED_Remove_SteamAPI_CPostAPIResultInProcess(void* arg0) {
        steammock::slot<void>(kCall_67, arg0);
    }
    virtual void Set_SteamAPI_CCheckCallbackRegisteredInProcess(void* func) {
        steammock::slot<void>(kCall_68, func);
    }
    virtual void* GetISteamInventory(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_69, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamVideo(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_70, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamParentalSettings(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_71, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamInput(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_74, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamParties(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_75, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamRemotePlay(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_76, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
};

Version_SteamClient019 g_SteamClient019;

// ISteamClient SteamClient020
class Version_SteamClient020 {
public:
    virtual std::int32_t CreateSteamPipe() { return steammock::slot<std::int32_t>(kCall_35); }
    virtual bool BReleaseSteamPipe(std::int32_t hSteamPipe) {
        return steammock::slot<bool>(kCall_36, hSteamPipe);
    }
    virtual std::int32_t ConnectToGlobalUser(std::int32_t hSteamPipe) {
        return steammock::slot<std::int32_t>(kCall_37, hSteamPipe);
    }
    virtual std::int32_t CreateLocalUser(std::int32_t* phSteamPipe, std::int32_t eAccountType) {
        return steammock::slot<std::int32_t>(kCall_38, phSteamPipe, eAccountType);
    }
    virtual void ReleaseUser(std::int32_t hSteamPipe, std::int32_t hUser) {
        steammock::slot<void>(kCall_39, hSteamPipe, hUser);
    }
    virtual void* GetISteamUser(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_40, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGameServer(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_41, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void SetLocalIPBinding(void* unIP, std::uint16_t usPort) {
        steammock::slot<void>(kCall_42, unIP, usPort);
    }
    virtual void* GetISteamFriends(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_43, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamUtils(std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_44, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMatchmaking(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_45, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMatchmakingServers(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_46, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGenericInterface(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_47, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamUserStats(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_48, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGameServerStats(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_49, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamApps(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_50, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamNetworking(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_51, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamRemoteStorage(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_52, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamScreenshots(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_53, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamGameSearch(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_72, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void RunFrame() { steammock::slot<void>(kCall_54); }
    virtual std::uint32_t GetIPCCallCount() { return steammock::slot<std::uint32_t>(kCall_55); }
    virtual void SetWarningMessageHook(void* pFunction) {
        steammock::slot<void>(kCall_56, pFunction);
    }
    virtual bool BShutdownIfAllPipesClosed() { return steammock::slot<bool>(kCall_57); }
    virtual void* GetISteamHTTP(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_58, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* DEPRECATED_GetISteamUnifiedMessages(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_73, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamController(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_60, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamUGC(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_61, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamAppList(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_62, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMusic(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_63, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamMusicRemote(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_64, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamHTMLSurface(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_65, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void DEPRECATED_Set_SteamAPI_CPostAPIResultInProcess(void* arg0) {
        steammock::slot<void>(kCall_66, arg0);
    }
    virtual void DEPRECATED_Remove_SteamAPI_CPostAPIResultInProcess(void* arg0) {
        steammock::slot<void>(kCall_67, arg0);
    }
    virtual void Set_SteamAPI_CCheckCallbackRegisteredInProcess(void* func) {
        steammock::slot<void>(kCall_68, func);
    }
    virtual void* GetISteamInventory(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_69, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamVideo(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_70, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamParentalSettings(std::int32_t hSteamuser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_71, hSteamuser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamInput(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_74, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamParties(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_75, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void* GetISteamRemotePlay(std::int32_t hSteamUser, std::int32_t hSteamPipe, const char* pchVersion) {
        void* result = steammock::slot<void*>(kCall_76, hSteamUser, hSteamPipe, pchVersion);
        if (result == nullptr) {
            result = steammock::interface_object(pchVersion);
        }
        return result;
    }
    virtual void DestroyAllInterfaces() { steammock::slot<void>(kCall_77); }
};

Version_SteamClient020 g_SteamClient020;

// ISteamController SteamController005
class Version_SteamController005 {
public:
    virtual bool Init() { return steammock::slot<bool>(kCall_78); }
    virtual bool Shutdown() { return steammock::slot<bool>(kCall_79); }
    virtual void RunFrame() { steammock::slot<void>(kCall_80); }
    virtual std::int32_t GetConnectedControllers(std::uint64_t* handlesOut) {
        return steammock::slot<std::int32_t>(kCall_81, handlesOut);
    }
    virtual bool ShowBindingPanel(std::uint64_t controllerHandle) {
        return steammock::slot<bool>(kCall_82, controllerHandle);
    }
    virtual std::uint64_t GetActionSetHandle(const char* pszActionSetName) {
        return steammock::slot<std::uint64_t>(kCall_83, pszActionSetName);
    }
    virtual void ActivateActionSet(std::uint64_t controllerHandle, std::uint64_t actionSetHandle) {
        steammock::slot<void>(kCall_84, controllerHandle, actionSetHandle);
    }
    virtual std::uint64_t GetCurrentActionSet(std::uint64_t controllerHandle) {
        return steammock::slot<std::uint64_t>(kCall_85, controllerHandle);
    }
    virtual std::uint64_t GetDigitalActionHandle(const char* pszActionName) {
        return steammock::slot<std::uint64_t>(kCall_86, pszActionName);
    }
    virtual ControllerDigitalActionData_t GetDigitalActionData(std::uint64_t controllerHandle, std::uint64_t digitalActionHandle) {
        return steammock::slot<ControllerDigitalActionData_t>(kCall_87, controllerHandle, digitalActionHandle);
    }
    virtual std::int32_t GetDigitalActionOrigins(std::uint64_t controllerHandle, std::uint64_t actionSetHandle, std::uint64_t digitalActionHandle, std::int32_t* originsOut) {
        return steammock::slot<std::int32_t>(kCall_88, controllerHandle, actionSetHandle, digitalActionHandle, originsOut);
    }
    virtual std::uint64_t GetAnalogActionHandle(const char* pszActionName) {
        return steammock::slot<std::uint64_t>(kCall_89, pszActionName);
    }
    virtual ControllerAnalogActionData_t GetAnalogActionData(std::uint64_t controllerHandle, std::uint64_t analogActionHandle) {
        return steammock::slot<ControllerAnalogActionData_t>(kCall_90, controllerHandle, analogActionHandle);
    }
    virtual std::int32_t GetAnalogActionOrigins(std::uint64_t controllerHandle, std::uint64_t actionSetHandle, std::uint64_t analogActionHandle, std::int32_t* originsOut) {
        return steammock::slot<std::int32_t>(kCall_91, controllerHandle, actionSetHandle, analogActionHandle, originsOut);
    }
    virtual void StopAnalogActionMomentum(std::uint64_t controllerHandle, std::uint64_t eAction) {
        steammock::slot<void>(kCall_92, controllerHandle, eAction);
    }
    virtual void TriggerHapticPulse(std::uint64_t controllerHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec) {
        steammock::slot<void>(kCall_93, controllerHandle, eTargetPad, usDurationMicroSec);
    }
    virtual void TriggerRepeatedHapticPulse(std::uint64_t controllerHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec, std::uint16_t usOffMicroSec, std::uint16_t unRepeat, std::uint32_t nFlags) {
        steammock::slot<void>(kCall_94, controllerHandle, eTargetPad, usDurationMicroSec, usOffMicroSec, unRepeat, nFlags);
    }
    virtual void TriggerVibration(std::uint64_t controllerHandle, std::uint16_t usLeftSpeed, std::uint16_t usRightSpeed) {
        steammock::slot<void>(kCall_95, controllerHandle, usLeftSpeed, usRightSpeed);
    }
    virtual void SetLEDColor(std::uint64_t controllerHandle, std::uint8_t nColorR, std::uint8_t nColorG, std::uint8_t nColorB, std::uint32_t nFlags) {
        steammock::slot<void>(kCall_96, controllerHandle, nColorR, nColorG, nColorB, nFlags);
    }
    virtual std::int32_t GetGamepadIndexForController(std::uint64_t ulControllerHandle) {
        return steammock::slot<std::int32_t>(kCall_97, ulControllerHandle);
    }
    virtual std::uint64_t GetControllerForGamepadIndex(std::int32_t nIndex) {
        return steammock::slot<std::uint64_t>(kCall_98, nIndex);
    }
    virtual ControllerMotionData_t GetMotionData(std::uint64_t controllerHandle) {
        return steammock::slot<ControllerMotionData_t>(kCall_99, controllerHandle);
    }
    virtual bool ShowDigitalActionOrigins(std::uint64_t controllerHandle, std::uint64_t digitalActionHandle, float flScale, float flXPosition, float flYPosition) {
        return steammock::slot<bool>(kCall_100, controllerHandle, digitalActionHandle, flScale, flXPosition, flYPosition);
    }
    virtual bool ShowAnalogActionOrigins(std::uint64_t controllerHandle, std::uint64_t analogActionHandle, float flScale, float flXPosition, float flYPosition) {
        return steammock::slot<bool>(kCall_101, controllerHandle, analogActionHandle, flScale, flXPosition, flYPosition);
    }
    virtual const char* GetStringForActionOrigin(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_102, eOrigin);
    }
    virtual const char* GetGlyphForActionOrigin(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_103, eOrigin);
    }
};

Version_SteamController005 g_SteamController005;

// ISteamController SteamController007
class Version_SteamController007 {
public:
    virtual bool Init() { return steammock::slot<bool>(kCall_78); }
    virtual bool Shutdown() { return steammock::slot<bool>(kCall_79); }
    virtual void RunFrame() { steammock::slot<void>(kCall_80); }
    virtual std::int32_t GetConnectedControllers(std::uint64_t* handlesOut) {
        return steammock::slot<std::int32_t>(kCall_81, handlesOut);
    }
    virtual std::uint64_t GetActionSetHandle(const char* pszActionSetName) {
        return steammock::slot<std::uint64_t>(kCall_83, pszActionSetName);
    }
    virtual void ActivateActionSet(std::uint64_t controllerHandle, std::uint64_t actionSetHandle) {
        steammock::slot<void>(kCall_84, controllerHandle, actionSetHandle);
    }
    virtual std::uint64_t GetCurrentActionSet(std::uint64_t controllerHandle) {
        return steammock::slot<std::uint64_t>(kCall_85, controllerHandle);
    }
    virtual void ActivateActionSetLayer(std::uint64_t controllerHandle, std::uint64_t actionSetLayerHandle) {
        steammock::slot<void>(kCall_104, controllerHandle, actionSetLayerHandle);
    }
    virtual void DeactivateActionSetLayer(std::uint64_t controllerHandle, std::uint64_t actionSetLayerHandle) {
        steammock::slot<void>(kCall_105, controllerHandle, actionSetLayerHandle);
    }
    virtual void DeactivateAllActionSetLayers(std::uint64_t controllerHandle) {
        steammock::slot<void>(kCall_106, controllerHandle);
    }
    virtual std::int32_t GetActiveActionSetLayers(std::uint64_t controllerHandle, std::uint64_t* handlesOut) {
        return steammock::slot<std::int32_t>(kCall_107, controllerHandle, handlesOut);
    }
    virtual std::uint64_t GetDigitalActionHandle(const char* pszActionName) {
        return steammock::slot<std::uint64_t>(kCall_86, pszActionName);
    }
    virtual ControllerDigitalActionData_t GetDigitalActionData(std::uint64_t controllerHandle, std::uint64_t digitalActionHandle) {
        return steammock::slot<ControllerDigitalActionData_t>(kCall_87, controllerHandle, digitalActionHandle);
    }
    virtual std::int32_t GetDigitalActionOrigins(std::uint64_t controllerHandle, std::uint64_t actionSetHandle, std::uint64_t digitalActionHandle, std::int32_t* originsOut) {
        return steammock::slot<std::int32_t>(kCall_88, controllerHandle, actionSetHandle, digitalActionHandle, originsOut);
    }
    virtual std::uint64_t GetAnalogActionHandle(const char* pszActionName) {
        return steammock::slot<std::uint64_t>(kCall_89, pszActionName);
    }
    virtual ControllerAnalogActionData_t GetAnalogActionData(std::uint64_t controllerHandle, std::uint64_t analogActionHandle) {
        return steammock::slot<ControllerAnalogActionData_t>(kCall_90, controllerHandle, analogActionHandle);
    }
    virtual std::int32_t GetAnalogActionOrigins(std::uint64_t controllerHandle, std::uint64_t actionSetHandle, std::uint64_t analogActionHandle, std::int32_t* originsOut) {
        return steammock::slot<std::int32_t>(kCall_91, controllerHandle, actionSetHandle, analogActionHandle, originsOut);
    }
    virtual const char* GetGlyphForActionOrigin(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_103, eOrigin);
    }
    virtual const char* GetStringForActionOrigin(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_102, eOrigin);
    }
    virtual void StopAnalogActionMomentum(std::uint64_t controllerHandle, std::uint64_t eAction) {
        steammock::slot<void>(kCall_92, controllerHandle, eAction);
    }
    virtual ControllerMotionData_t GetMotionData(std::uint64_t controllerHandle) {
        return steammock::slot<ControllerMotionData_t>(kCall_99, controllerHandle);
    }
    virtual void TriggerHapticPulse(std::uint64_t controllerHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec) {
        steammock::slot<void>(kCall_93, controllerHandle, eTargetPad, usDurationMicroSec);
    }
    virtual void TriggerRepeatedHapticPulse(std::uint64_t controllerHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec, std::uint16_t usOffMicroSec, std::uint16_t unRepeat, std::uint32_t nFlags) {
        steammock::slot<void>(kCall_94, controllerHandle, eTargetPad, usDurationMicroSec, usOffMicroSec, unRepeat, nFlags);
    }
    virtual void TriggerVibration(std::uint64_t controllerHandle, std::uint16_t usLeftSpeed, std::uint16_t usRightSpeed) {
        steammock::slot<void>(kCall_95, controllerHandle, usLeftSpeed, usRightSpeed);
    }
    virtual void SetLEDColor(std::uint64_t controllerHandle, std::uint8_t nColorR, std::uint8_t nColorG, std::uint8_t nColorB, std::uint32_t nFlags) {
        steammock::slot<void>(kCall_96, controllerHandle, nColorR, nColorG, nColorB, nFlags);
    }
    virtual bool ShowBindingPanel(std::uint64_t controllerHandle) {
        return steammock::slot<bool>(kCall_82, controllerHandle);
    }
    virtual std::int32_t GetInputTypeForHandle(std::uint64_t controllerHandle) {
        return steammock::slot<std::int32_t>(kCall_108, controllerHandle);
    }
    virtual std::uint64_t GetControllerForGamepadIndex(std::int32_t nIndex) {
        return steammock::slot<std::uint64_t>(kCall_98, nIndex);
    }
    virtual std::int32_t GetGamepadIndexForController(std::uint64_t ulControllerHandle) {
        return steammock::slot<std::int32_t>(kCall_97, ulControllerHandle);
    }
    virtual const char* GetStringForXboxOrigin(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_109, eOrigin);
    }
    virtual const char* GetGlyphForXboxOrigin(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_110, eOrigin);
    }
    virtual std::int32_t GetActionOriginFromXboxOrigin(std::uint64_t controllerHandle, std::int32_t eOrigin) {
        return steammock::slot<std::int32_t>(kCall_111, controllerHandle, eOrigin);
    }
    virtual std::int32_t TranslateActionOrigin(std::int32_t eDestinationInputType, std::int32_t eSourceOrigin) {
        return steammock::slot<std::int32_t>(kCall_112, eDestinationInputType, eSourceOrigin);
    }
    virtual bool GetControllerBindingRevision(std::uint64_t controllerHandle, std::int32_t* pMajor, std::int32_t* pMinor) {
        return steammock::slot<bool>(kCall_113, controllerHandle, pMajor, pMinor);
    }
};

Version_SteamController007 g_SteamController007;

// ISteamController SteamController008
class Version_SteamController008 {
public:
    virtual bool Init() { return steammock::slot<bool>(kCall_78); }
    virtual bool Shutdown() { return steammock::slot<bool>(kCall_79); }
    virtual void RunFrame() { steammock::slot<void>(kCall_80); }
    virtual std::int32_t GetConnectedControllers(std::uint64_t* handlesOut) {
        return steammock::slot<std::int32_t>(kCall_81, handlesOut);
    }
    virtual std::uint64_t GetActionSetHandle(const char* pszActionSetName) {
        return steammock::slot<std::uint64_t>(kCall_83, pszActionSetName);
    }
    virtual void ActivateActionSet(std::uint64_t controllerHandle, std::uint64_t actionSetHandle) {
        steammock::slot<void>(kCall_84, controllerHandle, actionSetHandle);
    }
    virtual std::uint64_t GetCurrentActionSet(std::uint64_t controllerHandle) {
        return steammock::slot<std::uint64_t>(kCall_85, controllerHandle);
    }
    virtual void ActivateActionSetLayer(std::uint64_t controllerHandle, std::uint64_t actionSetLayerHandle) {
        steammock::slot<void>(kCall_104, controllerHandle, actionSetLayerHandle);
    }
    virtual void DeactivateActionSetLayer(std::uint64_t controllerHandle, std::uint64_t actionSetLayerHandle) {
        steammock::slot<void>(kCall_105, controllerHandle, actionSetLayerHandle);
    }
    virtual void DeactivateAllActionSetLayers(std::uint64_t controllerHandle) {
        steammock::slot<void>(kCall_106, controllerHandle);
    }
    virtual std::int32_t GetActiveActionSetLayers(std::uint64_t controllerHandle, std::uint64_t* handlesOut) {
        return steammock::slot<std::int32_t>(kCall_107, controllerHandle, handlesOut);
    }
    virtual std::uint64_t GetDigitalActionHandle(const char* pszActionName) {
        return steammock::slot<std::uint64_t>(kCall_86, pszActionName);
    }
    virtual ControllerDigitalActionData_t GetDigitalActionData(std::uint64_t controllerHandle, std::uint64_t digitalActionHandle) {
        return steammock::slot<ControllerDigitalActionData_t>(kCall_87, controllerHandle, digitalActionHandle);
    }
    virtual std::int32_t GetDigitalActionOrigins(std::uint64_t controllerHandle, std::uint64_t actionSetHandle, std::uint64_t digitalActionHandle, std::int32_t* originsOut) {
        return steammock::slot<std::int32_t>(kCall_88, controllerHandle, actionSetHandle, digitalActionHandle, originsOut);
    }
    virtual std::uint64_t GetAnalogActionHandle(const char* pszActionName) {
        return steammock::slot<std::uint64_t>(kCall_89, pszActionName);
    }
    virtual ControllerAnalogActionData_t GetAnalogActionData(std::uint64_t controllerHandle, std::uint64_t analogActionHandle) {
        return steammock::slot<ControllerAnalogActionData_t>(kCall_90, controllerHandle, analogActionHandle);
    }
    virtual std::int32_t GetAnalogActionOrigins(std::uint64_t controllerHandle, std::uint64_t actionSetHandle, std::uint64_t analogActionHandle, std::int32_t* originsOut) {
        return steammock::slot<std::int32_t>(kCall_91, controllerHandle, actionSetHandle, analogActionHandle, originsOut);
    }
    virtual const char* GetGlyphForActionOrigin(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_103, eOrigin);
    }
    virtual const char* GetStringForActionOrigin(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_102, eOrigin);
    }
    virtual void StopAnalogActionMomentum(std::uint64_t controllerHandle, std::uint64_t eAction) {
        steammock::slot<void>(kCall_92, controllerHandle, eAction);
    }
    virtual ControllerMotionData_t GetMotionData(std::uint64_t controllerHandle) {
        return steammock::slot<ControllerMotionData_t>(kCall_99, controllerHandle);
    }
    virtual void TriggerHapticPulse(std::uint64_t controllerHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec) {
        steammock::slot<void>(kCall_93, controllerHandle, eTargetPad, usDurationMicroSec);
    }
    virtual void TriggerRepeatedHapticPulse(std::uint64_t controllerHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec, std::uint16_t usOffMicroSec, std::uint16_t unRepeat, std::uint32_t nFlags) {
        steammock::slot<void>(kCall_94, controllerHandle, eTargetPad, usDurationMicroSec, usOffMicroSec, unRepeat, nFlags);
    }
    virtual void TriggerVibration(std::uint64_t controllerHandle, std::uint16_t usLeftSpeed, std::uint16_t usRightSpeed) {
        steammock::slot<void>(kCall_95, controllerHandle, usLeftSpeed, usRightSpeed);
    }
    virtual void SetLEDColor(std::uint64_t controllerHandle, std::uint8_t nColorR, std::uint8_t nColorG, std::uint8_t nColorB, std::uint32_t nFlags) {
        steammock::slot<void>(kCall_96, controllerHandle, nColorR, nColorG, nColorB, nFlags);
    }
    virtual bool ShowBindingPanel(std::uint64_t controllerHandle) {
        return steammock::slot<bool>(kCall_82, controllerHandle);
    }
    virtual std::int32_t GetInputTypeForHandle(std::uint64_t controllerHandle) {
        return steammock::slot<std::int32_t>(kCall_108, controllerHandle);
    }
    virtual std::uint64_t GetControllerForGamepadIndex(std::int32_t nIndex) {
        return steammock::slot<std::uint64_t>(kCall_98, nIndex);
    }
    virtual std::int32_t GetGamepadIndexForController(std::uint64_t ulControllerHandle) {
        return steammock::slot<std::int32_t>(kCall_97, ulControllerHandle);
    }
    virtual const char* GetStringForXboxOrigin(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_109, eOrigin);
    }
    virtual const char* GetGlyphForXboxOrigin(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_110, eOrigin);
    }
    virtual std::int32_t GetActionOriginFromXboxOrigin(std::uint64_t controllerHandle, std::int32_t eOrigin) {
        return steammock::slot<std::int32_t>(kCall_111, controllerHandle, eOrigin);
    }
    virtual std::int32_t TranslateActionOrigin(std::int32_t eDestinationInputType, std::int32_t eSourceOrigin) {
        return steammock::slot<std::int32_t>(kCall_112, eDestinationInputType, eSourceOrigin);
    }
    virtual bool GetControllerBindingRevision(std::uint64_t controllerHandle, std::int32_t* pMajor, std::int32_t* pMinor) {
        return steammock::slot<bool>(kCall_113, controllerHandle, pMajor, pMinor);
    }
};

Version_SteamController008 g_SteamController008;

// ISteamFriends SteamFriends015
class Version_SteamFriends015 {
public:
    virtual const char* GetPersonaName() { return steammock::slot<const char*>(kCall_114); }
    virtual std::uint64_t SetPersonaName(const char* pchPersonaName) {
        return steammock::slot<std::uint64_t>(kCall_115, pchPersonaName);
    }
    virtual std::int32_t GetPersonaState() { return steammock::slot<std::int32_t>(kCall_116); }
    virtual std::int32_t GetFriendCount(std::int32_t iFriendFlags) {
        return steammock::slot<std::int32_t>(kCall_117, iFriendFlags);
    }
    virtual CSteamID GetFriendByIndex(std::int32_t iFriend, std::int32_t iFriendFlags) {
        return steammock::slot<CSteamID>(kCall_118, iFriend, iFriendFlags);
    }
    virtual std::int32_t GetFriendRelationship(CSteamID steamIDFriend) {
        return steammock::slot<std::int32_t>(kCall_119, steamIDFriend);
    }
    virtual std::int32_t GetFriendPersonaState(CSteamID steamIDFriend) {
        return steammock::slot<std::int32_t>(kCall_120, steamIDFriend);
    }
    virtual const char* GetFriendPersonaName(CSteamID steamIDFriend) {
        return steammock::slot<const char*>(kCall_121, steamIDFriend);
    }
    virtual bool GetFriendGamePlayed(CSteamID steamIDFriend, void* pFriendGameInfo) {
        return steammock::slot<bool>(kCall_122, steamIDFriend, pFriendGameInfo);
    }
    virtual const char* GetFriendPersonaNameHistory(CSteamID steamIDFriend, std::int32_t iPersonaName) {
        return steammock::slot<const char*>(kCall_123, steamIDFriend, iPersonaName);
    }
    virtual std::int32_t GetFriendSteamLevel(CSteamID steamIDFriend) {
        return steammock::slot<std::int32_t>(kCall_124, steamIDFriend);
    }
    virtual const char* GetPlayerNickname(CSteamID steamIDPlayer) {
        return steammock::slot<const char*>(kCall_125, steamIDPlayer);
    }
    virtual std::int32_t GetFriendsGroupCount() { return steammock::slot<std::int32_t>(kCall_126); }
    virtual std::int16_t GetFriendsGroupIDByIndex(std::int32_t iFG) {
        return steammock::slot<std::int16_t>(kCall_127, iFG);
    }
    virtual const char* GetFriendsGroupName(std::int16_t friendsGroupID) {
        return steammock::slot<const char*>(kCall_128, friendsGroupID);
    }
    virtual std::int32_t GetFriendsGroupMembersCount(std::int16_t friendsGroupID) {
        return steammock::slot<std::int32_t>(kCall_129, friendsGroupID);
    }
    virtual void GetFriendsGroupMembersList(std::int16_t friendsGroupID, CSteamID* pOutSteamIDMembers, std::int32_t nMembersCount) {
        steammock::slot<void>(kCall_130, friendsGroupID, pOutSteamIDMembers, nMembersCount);
    }
    virtual bool HasFriend(CSteamID steamIDFriend, std::int32_t iFriendFlags) {
        return steammock::slot<bool>(kCall_131, steamIDFriend, iFriendFlags);
    }
    virtual std::int32_t GetClanCount() { return steammock::slot<std::int32_t>(kCall_132); }
    virtual CSteamID GetClanByIndex(std::int32_t iClan) {
        return steammock::slot<CSteamID>(kCall_133, iClan);
    }
    virtual const char* GetClanName(CSteamID steamIDClan) {
        return steammock::slot<const char*>(kCall_134, steamIDClan);
    }
    virtual const char* GetClanTag(CSteamID steamIDClan) {
        return steammock::slot<const char*>(kCall_135, steamIDClan);
    }
    virtual bool GetClanActivityCounts(CSteamID steamIDClan, std::int32_t* pnOnline, std::int32_t* pnInGame, std::int32_t* pnChatting) {
        return steammock::slot<bool>(kCall_136, steamIDClan, pnOnline, pnInGame, pnChatting);
    }
    virtual std::uint64_t DownloadClanActivityCounts(CSteamID* psteamIDClans, std::int32_t cClansToRequest) {
        return steammock::slot<std::uint64_t>(kCall_137, psteamIDClans, cClansToRequest);
    }
    virtual std::int32_t GetFriendCountFromSource(CSteamID steamIDSource) {
        return steammock::slot<std::int32_t>(kCall_138, steamIDSource);
    }
    virtual CSteamID GetFriendFromSourceByIndex(CSteamID steamIDSource, std::int32_t iFriend) {
        return steammock::slot<CSteamID>(kCall_139, steamIDSource, iFriend);
    }
    virtual bool IsUserInSource(CSteamID steamIDUser, CSteamID steamIDSource) {
        return steammock::slot<bool>(kCall_140, steamIDUser, steamIDSource);
    }
    virtual void SetInGameVoiceSpeaking(CSteamID steamIDUser, bool bSpeaking) {
        steammock::slot<void>(kCall_141, steamIDUser, bSpeaking);
    }
    virtual void ActivateGameOverlay(const char* pchDialog) {
        steammock::slot<void>(kCall_142, pchDialog);
    }
    virtual void ActivateGameOverlayToUser(const char* pchDialog, CSteamID steamID) {
        steammock::slot<void>(kCall_143, pchDialog, steamID);
    }
    virtual void ActivateGameOverlayToWebPage(const char* pchURL) {
        steammock::slot<void>(kCall_144, pchURL);
    }
    virtual void ActivateGameOverlayToStore(std::uint32_t nAppID, std::int32_t eFlag) {
        steammock::slot<void>(kCall_145, nAppID, eFlag);
    }
    virtual void SetPlayedWith(CSteamID steamIDUserPlayedWith) {
        steammock::slot<void>(kCall_146, steamIDUserPlayedWith);
    }
    virtual void ActivateGameOverlayInviteDialog(CSteamID steamIDLobby) {
        steammock::slot<void>(kCall_147, steamIDLobby);
    }
    virtual std::int32_t GetSmallFriendAvatar(CSteamID steamIDFriend) {
        return steammock::slot<std::int32_t>(kCall_148, steamIDFriend);
    }
    virtual std::int32_t GetMediumFriendAvatar(CSteamID steamIDFriend) {
        return steammock::slot<std::int32_t>(kCall_149, steamIDFriend);
    }
    virtual std::int32_t GetLargeFriendAvatar(CSteamID steamIDFriend) {
        return steammock::slot<std::int32_t>(kCall_150, steamIDFriend);
    }
    virtual bool RequestUserInformation(CSteamID steamIDUser, bool bRequireNameOnly) {
        return steammock::slot<bool>(kCall_151, steamIDUser, bRequireNameOnly);
    }
    virtual std::uint64_t RequestClanOfficerList(CSteamID steamIDClan) {
        return steammock::slot<std::uint64_t>(kCall_152, steamIDClan);
    }
    virtual CSteamID GetClanOwner(CSteamID steamIDClan) {
        return steammock::slot<CSteamID>(kCall_153, steamIDClan);
    }
    virtual std::int32_t GetClanOfficerCount(CSteamID steamIDClan) {
        return steammock::slot<std::int32_t>(kCall_154, steamIDClan);
    }
    virtual CSteamID GetClanOfficerByIndex(CSteamID steamIDClan, std::int32_t iOfficer) {
        return steammock::slot<CSteamID>(kCall_155, steamIDClan, iOfficer);
    }
    virtual std::uint32_t GetUserRestrictions() {
        return steammock::slot<std::uint32_t>(kCall_156);
    }
    virtual bool SetRichPresence(const char* pchKey, const char* pchValue) {
        return steammock::slot<bool>(kCall_157, pchKey, pchValue);
    }
    virtual void ClearRichPresence() { steammock::slot<void>(kCall_158); }
    virtual const char* GetFriendRichPresence(CSteamID steamIDFriend, const char* pchKey) {
        return steammock::slot<const char*>(kCall_159, steamIDFriend, pchKey);
    }
    virtual std::int32_t GetFriendRichPresenceKeyCount(CSteamID steamIDFriend) {
        return steammock::slot<std::int32_t>(kCall_160, steamIDFriend);
    }
    virtual const char* GetFriendRichPresenceKeyByIndex(CSteamID steamIDFriend, std::int32_t iKey) {
        return steammock::slot<const char*>(kCall_161, steamIDFriend, iKey);
    }
    virtual void RequestFriendRichPresence(CSteamID steamIDFriend) {
        steammock::slot<void>(kCall_162, steamIDFriend);
    }
    virtual bool InviteUserToGame(CSteamID steamIDFriend, const char* pchConnectString) {
        return steammock::slot<bool>(kCall_163, steamIDFriend, pchConnectString);
    }
    virtual std::int32_t GetCoplayFriendCount() { return steammock::slot<std::int32_t>(kCall_164); }
    virtual CSteamID GetCoplayFriend(std::int32_t iCoplayFriend) {
        return steammock::slot<CSteamID>(kCall_165, iCoplayFriend);
    }
    virtual std::int32_t GetFriendCoplayTime(CSteamID steamIDFriend) {
        return steammock::slot<std::int32_t>(kCall_166, steamIDFriend);
    }
    virtual std::uint32_t GetFriendCoplayGame(CSteamID steamIDFriend) {
        return steammock::slot<std::uint32_t>(kCall_167, steamIDFriend);
    }
    virtual std::uint64_t JoinClanChatRoom(CSteamID steamIDClan) {
        return steammock::slot<std::uint64_t>(kCall_168, steamIDClan);
    }
    virtual bool LeaveClanChatRoom(CSteamID steamIDClan) {
        return steammock::slot<bool>(kCall_169, steamIDClan);
    }
    virtual std::int32_t GetClanChatMemberCount(CSteamID steamIDClan) {
        return steammock::slot<std::int32_t>(kCall_170, steamIDClan);
    }
    virtual CSteamID GetChatMemberByIndex(CSteamID steamIDClan, std::int32_t iUser) {
        return steammock::slot<CSteamID>(kCall_171, steamIDClan, iUser);
    }
    virtual bool SendClanChatMessage(CSteamID steamIDClanChat, const char* pchText) {
        return steammock::slot<bool>(kCall_172, steamIDClanChat, pchText);
    }
    virtual std::int32_t GetClanChatMessage(CSteamID steamIDClanChat, std::int32_t iMessage, void* prgchText, std::int32_t cchTextMax, std::int32_t* peChatEntryType, CSteamID* psteamidChatter) {
        return steammock::slot<std::int32_t>(kCall_173, steamIDClanChat, iMessage, prgchText, cchTextMax, peChatEntryType, psteamidChatter);
    }
    virtual bool IsClanChatAdmin(CSteamID steamIDClanChat, CSteamID steamIDUser) {
        return steammock::slot<bool>(kCall_174, steamIDClanChat, steamIDUser);
    }
    virtual bool IsClanChatWindowOpenInSteam(CSteamID steamIDClanChat) {
        return steammock::slot<bool>(kCall_175, steamIDClanChat);
    }
    virtual bool OpenClanChatWindowInSteam(CSteamID steamIDClanChat) {
        return steammock::slot<bool>(kCall_176, steamIDClanChat);
    }
    virtual bool CloseClanChatWindowInSteam(CSteamID steamIDClanChat) {
        return steammock::slot<bool>(kCall_177, steamIDClanChat);
    }
    virtual bool SetListenForFriendsMessages(bool bInterceptEnabled) {
        return steammock::slot<bool>(kCall_178, bInterceptEnabled);
    }
    virtual bool ReplyToFriendMessage(CSteamID steamIDFriend, const char* pchMsgToSend) {
        return steammock::slot<bool>(kCall_179, steamIDFriend, pchMsgToSend);
    }
    virtual std::int32_t GetFriendMessage(CSteamID steamIDFriend, std::int32_t iMessageID, void* pvData, std::int32_t cubData, std::int32_t* peChatEntryType) {
        return steammock::slot<std::int32_t>(kCall_180, steamIDFriend, iMessageID, pvData, cubData, peChatEntryType);
    }
    virtual std::uint64_t GetFollowerCount(CSteamID steamID) {
        return steammock::slot<std::uint64_t>(kCall_181, steamID);
    }
    virtual std::uint64_t IsFollowing(CSteamID steamID) {
        return steammock::slot<std::uint64_t>(kCall_182, steamID);
    }
    virtual std::uint64_t EnumerateFollowingList(std::uint32_t unStartIndex) {
        return steammock::slot<std::uint64_t>(kCall_183, unStartIndex);
    }
};

Version_SteamFriends015 g_SteamFriends015;

// ISteamFriends SteamFriends017
class Version_SteamFriends017 {
public:
    virtual const char* GetPersonaName() { return steammock::slot<const char*>(kCall_114); }
    virtual std::uint64_t SetPersonaName(const char* pchPersonaName) {
        return steammock::slot<std::uint64_t>(kCall_115, pchPersonaName);
    }
    virtual std::int32_t GetPersonaState() { return steammock::slot<std::int32_t>(kCall_116); }
    virtual std::int32_t GetFriendCount(std::int32_t iFriendFlags) {
        return steammock::slot<std::int32_t>(kCall_117, iFriendFlags);
    }
    virtual CSteamID GetFriendByIndex(std::int32_t iFriend, std::int32_t iFriendFlags) {
        return steammock::slot<CSteamID>(kCall_118, iFriend, iFriendFlags);
    }
    virtual std::int32_t GetFriendRelationship(CSteamID steamIDFriend) {
        return steammock::slot<std::int32_t>(kCall_119, steamIDFriend);
    }
    virtual std::int32_t GetFriendPersonaState(CSteamID steamIDFriend) {
        return steammock::slot<std::int32_t>(kCall_120, steamIDFriend);
    }
    virtual const char* GetFriendPersonaName(CSteamID steamIDFriend) {
        return steammock::slot<const char*>(kCall_121, steamIDFriend);
    }
    virtual bool GetFriendGamePlayed(CSteamID steamIDFriend, void* pFriendGameInfo) {
        return steammock::slot<bool>(kCall_122, steamIDFriend, pFriendGameInfo);
    }
    virtual const char* GetFriendPersonaNameHistory(CSteamID steamIDFriend, std::int32_t iPersonaName) {
        return steammock::slot<const char*>(kCall_123, steamIDFriend, iPersonaName);
    }
    virtual std::int32_t GetFriendSteamLevel(CSteamID steamIDFriend) {
        return steammock::slot<std::int32_t>(kCall_124, steamIDFriend);
    }
    virtual const char* GetPlayerNickname(CSteamID steamIDPlayer) {
        return steammock::slot<const char*>(kCall_125, steamIDPlayer);
    }
    virtual std::int32_t GetFriendsGroupCount() { return steammock::slot<std::int32_t>(kCall_126); }
    virtual std::int16_t GetFriendsGroupIDByIndex(std::int32_t iFG) {
        return steammock::slot<std::int16_t>(kCall_127, iFG);
    }
    virtual const char* GetFriendsGroupName(std::int16_t friendsGroupID) {
        return steammock::slot<const char*>(kCall_128, friendsGroupID);
    }
    virtual std::int32_t GetFriendsGroupMembersCount(std::int16_t friendsGroupID) {
        return steammock::slot<std::int32_t>(kCall_129, friendsGroupID);
    }
    virtual void GetFriendsGroupMembersList(std::int16_t friendsGroupID, CSteamID* pOutSteamIDMembers, std::int32_t nMembersCount) {
        steammock::slot<void>(kCall_130, friendsGroupID, pOutSteamIDMembers, nMembersCount);
    }
    virtual bool HasFriend(CSteamID steamIDFriend, std::int32_t iFriendFlags) {
        return steammock::slot<bool>(kCall_131, steamIDFriend, iFriendFlags);
    }
    virtual std::int32_t GetClanCount() { return steammock::slot<std::int32_t>(kCall_132); }
    virtual CSteamID GetClanByIndex(std::int32_t iClan) {
        return steammock::slot<CSteamID>(kCall_133, iClan);
    }
    virtual const char* GetClanName(CSteamID steamIDClan) {
        return steammock::slot<const char*>(kCall_134, steamIDClan);
    }
    virtual const char* GetClanTag(CSteamID steamIDClan) {
        return steammock::slot<const char*>(kCall_135, steamIDClan);
    }
    virtual bool GetClanActivityCounts(CSteamID steamIDClan, std::int32_t* pnOnline, std::int32_t* pnInGame, std::int32_t* pnChatting) {
        return steammock::slot<bool>(kCall_136, steamIDClan, pnOnline, pnInGame, pnChatting);
    }
    virtual std::uint64_t DownloadClanActivityCounts(CSteamID* psteamIDClans, std::int32_t cClansToRequest) {
        return steammock::slot<std::uint64_t>(kCall_137, psteamIDClans, cClansToRequest);
    }
    virtual std::int32_t GetFriendCountFromSource(CSteamID steamIDSource) {
        return steammock::slot<std::int32_t>(kCall_138, steamIDSource);
    }
    virtual CSteamID GetFriendFromSourceByIndex(CSteamID steamIDSource, std::int32_t iFriend) {
        return steammock::slot<CSteamID>(kCall_139, steamIDSource, iFriend);
    }
    virtual bool IsUserInSource(CSteamID steamIDUser, CSteamID steamIDSource) {
        return steammock::slot<bool>(kCall_140, steamIDUser, steamIDSource);
    }
    virtual void SetInGameVoiceSpeaking(CSteamID steamIDUser, bool bSpeaking) {
        steammock::slot<void>(kCall_141, steamIDUser, bSpeaking);
    }
    virtual void ActivateGameOverlay(const char* pchDialog) {
        steammock::slot<void>(kCall_142, pchDialog);
    }
    virtual void ActivateGameOverlayToUser(const char* pchDialog, CSteamID steamID) {
        steammock::slot<void>(kCall_143, pchDialog, steamID);
    }
    virtual void ActivateGameOverlayToWebPage(const char* pchURL, std::int32_t eMode) {
        steammock::slot<void>(kCall_184, pchURL, eMode);
    }
    virtual void ActivateGameOverlayToStore(std::uint32_t nAppID, std::int32_t eFlag) {
        steammock::slot<void>(kCall_145, nAppID, eFlag);
    }
    virtual void SetPlayedWith(CSteamID steamIDUserPlayedWith) {
        steammock::slot<void>(kCall_146, steamIDUserPlayedWith);
    }
    virtual void ActivateGameOverlayInviteDialog(CSteamID steamIDLobby) {
        steammock::slot<void>(kCall_147, steamIDLobby);
    }
    virtual std::int32_t GetSmallFriendAvatar(CSteamID steamIDFriend) {
        return steammock::slot<std::int32_t>(kCall_148, steamIDFriend);
    }
    virtual std::int32_t GetMediumFriendAvatar(CSteamID steamIDFriend) {
        return steammock::slot<std::int32_t>(kCall_149, steamIDFriend);
    }
    virtual std::int32_t GetLargeFriendAvatar(CSteamID steamIDFriend) {
        return steammock::slot<std::int32_t>(kCall_150, steamIDFriend);
    }
    virtual bool RequestUserInformation(CSteamID steamIDUser, bool bRequireNameOnly) {
        return steammock::slot<bool>(kCall_151, steamIDUser, bRequireNameOnly);
    }
    virtual std::uint64_t RequestClanOfficerList(CSteamID steamIDClan) {
        return steammock::slot<std::uint64_t>(kCall_152, steamIDClan);
    }
    virtual CSteamID GetClanOwner(CSteamID steamIDClan) {
        return steammock::slot<CSteamID>(kCall_153, steamIDClan);
    }
    virtual std::int32_t GetClanOfficerCount(CSteamID steamIDClan) {
        return steammock::slot<std::int32_t>(kCall_154, steamIDClan);
    }
    virtual CSteamID GetClanOfficerByIndex(CSteamID steamIDClan, std::int32_t iOfficer) {
        return steammock::slot<CSteamID>(kCall_155, steamIDClan, iOfficer);
    }
    virtual std::uint32_t GetUserRestrictions() {
        return steammock::slot<std::uint32_t>(kCall_156);
    }
    virtual bool SetRichPresence(const char* pchKey, const char* pchValue) {
        return steammock::slot<bool>(kCall_157, pchKey, pchValue);
    }
    virtual void ClearRichPresence() { steammock::slot<void>(kCall_158); }
    virtual const char* GetFriendRichPresence(CSteamID steamIDFriend, const char* pchKey) {
        return steammock::slot<const char*>(kCall_159, steamIDFriend, pchKey);
    }
    virtual std::int32_t GetFriendRichPresenceKeyCount(CSteamID steamIDFriend) {
        return steammock::slot<std::int32_t>(kCall_160, steamIDFriend);
    }
    virtual const char* GetFriendRichPresenceKeyByIndex(CSteamID steamIDFriend, std::int32_t iKey) {
        return steammock::slot<const char*>(kCall_161, steamIDFriend, iKey);
    }
    virtual void RequestFriendRichPresence(CSteamID steamIDFriend) {
        steammock::slot<void>(kCall_162, steamIDFriend);
    }
    virtual bool InviteUserToGame(CSteamID steamIDFriend, const char* pchConnectString) {
        return steammock::slot<bool>(kCall_163, steamIDFriend, pchConnectString);
    }
    virtual std::int32_t GetCoplayFriendCount() { return steammock::slot<std::int32_t>(kCall_164); }
    virtual CSteamID GetCoplayFriend(std::int32_t iCoplayFriend) {
        return steammock::slot<CSteamID>(kCall_165, iCoplayFriend);
    }
    virtual std::int32_t GetFriendCoplayTime(CSteamID steamIDFriend) {
        return steammock::slot<std::int32_t>(kCall_166, steamIDFriend);
    }
    virtual std::uint32_t GetFriendCoplayGame(CSteamID steamIDFriend) {
        return steammock::slot<std::uint32_t>(kCall_167, steamIDFriend);
    }
    virtual std::uint64_t JoinClanChatRoom(CSteamID steamIDClan) {
        return steammock::slot<std::uint64_t>(kCall_168, steamIDClan);
    }
    virtual bool LeaveClanChatRoom(CSteamID steamIDClan) {
        return steammock::slot<bool>(kCall_169, steamIDClan);
    }
    virtual std::int32_t GetClanChatMemberCount(CSteamID steamIDClan) {
        return steammock::slot<std::int32_t>(kCall_170, steamIDClan);
    }
    virtual CSteamID GetChatMemberByIndex(CSteamID steamIDClan, std::int32_t iUser) {
        return steammock::slot<CSteamID>(kCall_171, steamIDClan, iUser);
    }
    virtual bool SendClanChatMessage(CSteamID steamIDClanChat, const char* pchText) {
        return steammock::slot<bool>(kCall_172, steamIDClanChat, pchText);
    }
    virtual std::int32_t GetClanChatMessage(CSteamID steamIDClanChat, std::int32_t iMessage, void* prgchText, std::int32_t cchTextMax, std::int32_t* peChatEntryType, CSteamID* psteamidChatter) {
        return steammock::slot<std::int32_t>(kCall_173, steamIDClanChat, iMessage, prgchText, cchTextMax, peChatEntryType, psteamidChatter);
    }
    virtual bool IsClanChatAdmin(CSteamID steamIDClanChat, CSteamID steamIDUser) {
        return steammock::slot<bool>(kCall_174, steamIDClanChat, steamIDUser);
    }
    virtual bool IsClanChatWindowOpenInSteam(CSteamID steamIDClanChat) {
        return steammock::slot<bool>(kCall_175, steamIDClanChat);
    }
    virtual bool OpenClanChatWindowInSteam(CSteamID steamIDClanChat) {
        return steammock::slot<bool>(kCall_176, steamIDClanChat);
    }
    virtual bool CloseClanChatWindowInSteam(CSteamID steamIDClanChat) {
        return steammock::slot<bool>(kCall_177, steamIDClanChat);
    }
    virtual bool SetListenForFriendsMessages(bool bInterceptEnabled) {
        return steammock::slot<bool>(kCall_178, bInterceptEnabled);
    }
    virtual bool ReplyToFriendMessage(CSteamID steamIDFriend, const char* pchMsgToSend) {
        return steammock::slot<bool>(kCall_179, steamIDFriend, pchMsgToSend);
    }
    virtual std::int32_t GetFriendMessage(CSteamID steamIDFriend, std::int32_t iMessageID, void* pvData, std::int32_t cubData, std::int32_t* peChatEntryType) {
        return steammock::slot<std::int32_t>(kCall_180, steamIDFriend, iMessageID, pvData, cubData, peChatEntryType);
    }
    virtual std::uint64_t GetFollowerCount(CSteamID steamID) {
        return steammock::slot<std::uint64_t>(kCall_181, steamID);
    }
    virtual std::uint64_t IsFollowing(CSteamID steamID) {
        return steammock::slot<std::uint64_t>(kCall_182, steamID);
    }
    virtual std::uint64_t EnumerateFollowingList(std::uint32_t unStartIndex) {
        return steammock::slot<std::uint64_t>(kCall_183, unStartIndex);
    }
    virtual bool IsClanPublic(CSteamID steamIDClan) {
        return steammock::slot<bool>(kCall_185, steamIDClan);
    }
    virtual bool IsClanOfficialGameGroup(CSteamID steamIDClan) {
        return steammock::slot<bool>(kCall_186, steamIDClan);
    }
    virtual std::int32_t GetNumChatsWithUnreadPriorityMessages() {
        return steammock::slot<std::int32_t>(kCall_187);
    }
    virtual void ActivateGameOverlayRemotePlayTogetherInviteDialog(CSteamID steamIDLobby) {
        steammock::slot<void>(kCall_188, steamIDLobby);
    }
    virtual bool RegisterProtocolInOverlayBrowser(const char* pchProtocol) {
        return steammock::slot<bool>(kCall_189, pchProtocol);
    }
    virtual void ActivateGameOverlayInviteDialogConnectString(const char* pchConnectString) {
        steammock::slot<void>(kCall_190, pchConnectString);
    }
    virtual std::uint64_t RequestEquippedProfileItems(CSteamID steamID) {
        return steammock::slot<std::uint64_t>(kCall_191, steamID);
    }
    virtual bool BHasEquippedProfileItem(CSteamID steamID, std::int32_t itemType) {
        return steammock::slot<bool>(kCall_192, steamID, itemType);
    }
    virtual const char* GetProfileItemPropertyString(CSteamID steamID, std::int32_t itemType, std::int32_t prop) {
        return steammock::slot<const char*>(kCall_193, steamID, itemType, prop);
    }
    virtual std::uint32_t GetProfileItemPropertyUint(CSteamID steamID, std::int32_t itemType, std::int32_t prop) {
        return steammock::slot<std::uint32_t>(kCall_194, steamID, itemType, prop);
    }
};

Version_SteamFriends017 g_SteamFriends017;

// ISteamGameSearch SteamMatchGameSearch001
class Version_SteamMatchGameSearch001 {
public:
    virtual std::int32_t AddGameSearchParams(const char* pchKeyToFind, const char* pchValuesToFind) {
        return steammock::slot<std::int32_t>(kCall_195, pchKeyToFind, pchValuesToFind);
    }
    virtual std::int32_t SearchForGameWithLobby(CSteamID steamIDLobby, std::int32_t nPlayerMin, std::int32_t nPlayerMax) {
        return steammock::slot<std::int32_t>(kCall_196, steamIDLobby, nPlayerMin, nPlayerMax);
    }
    virtual std::int32_t SearchForGameSolo(std::int32_t nPlayerMin, std::int32_t nPlayerMax) {
        return steammock::slot<std::int32_t>(kCall_197, nPlayerMin, nPlayerMax);
    }
    virtual std::int32_t AcceptGame() { return steammock::slot<std::int32_t>(kCall_198); }
    virtual std::int32_t DeclineGame() { return steammock::slot<std::int32_t>(kCall_199); }
    virtual std::int32_t RetrieveConnectionDetails(CSteamID steamIDHost, void* pchConnectionDetails, std::int32_t cubConnectionDetails) {
        return steammock::slot<std::int32_t>(kCall_200, steamIDHost, pchConnectionDetails, cubConnectionDetails);
    }
    virtual std::int32_t EndGameSearch() { return steammock::slot<std::int32_t>(kCall_201); }
    virtual std::int32_t SetGameHostParams(const char* pchKey, const char* pchValue) {
        return steammock::slot<std::int32_t>(kCall_202, pchKey, pchValue);
    }
    virtual std::int32_t SetConnectionDetails(const char* pchConnectionDetails, std::int32_t cubConnectionDetails) {
        return steammock::slot<std::int32_t>(kCall_203, pchConnectionDetails, cubConnectionDetails);
    }
    virtual std::int32_t RequestPlayersForGame(std::int32_t nPlayerMin, std::int32_t nPlayerMax, std::int32_t nMaxTeamSize) {
        return steammock::slot<std::int32_t>(kCall_204, nPlayerMin, nPlayerMax, nMaxTeamSize);
    }
    virtual std::int32_t HostConfirmGameStart(std::uint64_t ullUniqueGameID) {
        return steammock::slot<std::int32_t>(kCall_205, ullUniqueGameID);
    }
    virtual std::int32_t CancelRequestPlayersForGame() {
        return steammock::slot<std::int32_t>(kCall_206);
    }
    virtual std::int32_t SubmitPlayerResult(std::uint64_t ullUniqueGameID, CSteamID steamIDPlayer, std::int32_t EPlayerResult) {
        return steammock::slot<std::int32_t>(kCall_207, ullUniqueGameID, steamIDPlayer, EPlayerResult);
    }
    virtual std::int32_t EndGame(std::uint64_t ullUniqueGameID) {
        return steammock::slot<std::int32_t>(kCall_208, ullUniqueGameID);
    }
};

Version_SteamMatchGameSearch001 g_SteamMatchGameSearch001;

// ISteamGameServer SteamGameServer012
class Version_SteamGameServer012 {
public:
    virtual bool InitGameServer(std::uint32_t unIP, std::uint16_t usGamePort, std::uint16_t usQueryPort, std::uint32_t unFlags, std::uint32_t nGameAppId, const char* pchVersionString) {
        return steammock::slot<bool>(kCall_209, unIP, usGamePort, usQueryPort, unFlags, nGameAppId, pchVersionString);
    }
    virtual void SetProduct(const char* pszProduct) {
        steammock::slot<void>(kCall_210, pszProduct);
    }
    virtual void SetGameDescription(const char* pszGameDescription) {
        steammock::slot<void>(kCall_211, pszGameDescription);
    }
    virtual void SetModDir(const char* pszModDir) { steammock::slot<void>(kCall_212, pszModDir); }
    virtual void SetDedicatedServer(bool bDedicated) {
        steammock::slot<void>(kCall_213, bDedicated);
    }
    virtual void LogOn(const char* pszToken) { steammock::slot<void>(kCall_214, pszToken); }
    virtual void LogOnAnonymous() { steammock::slot<void>(kCall_215); }
    virtual void LogOff() { steammock::slot<void>(kCall_216); }
    virtual bool BLoggedOn() { return steammock::slot<bool>(kCall_217); }
    virtual bool BSecure() { return steammock::slot<bool>(kCall_218); }
    virtual CSteamID GetSteamID() { return steammock::slot<CSteamID>(kCall_219); }
    virtual bool WasRestartRequested() { return steammock::slot<bool>(kCall_220); }
    virtual void SetMaxPlayerCount(std::int32_t cPlayersMax) {
        steammock::slot<void>(kCall_221, cPlayersMax);
    }
    virtual void SetBotPlayerCount(std::int32_t cBotplayers) {
        steammock::slot<void>(kCall_222, cBotplayers);
    }
    virtual void SetServerName(const char* pszServerName) {
        steammock::slot<void>(kCall_223, pszServerName);
    }
    virtual void SetMapName(const char* pszMapName) {
        steammock::slot<void>(kCall_224, pszMapName);
    }
    virtual void SetPasswordProtected(bool bPasswordProtected) {
        steammock::slot<void>(kCall_225, bPasswordProtected);
    }
    virtual void SetSpectatorPort(std::uint16_t unSpectatorPort) {
        steammock::slot<void>(kCall_226, unSpectatorPort);
    }
    virtual void SetSpectatorServerName(const char* pszSpectatorServerName) {
        steammock::slot<void>(kCall_227, pszSpectatorServerName);
    }
    virtual void ClearAllKeyValues() { steammock::slot<void>(kCall_228); }
    virtual void SetKeyValue(const char* pKey, const char* pValue) {
        steammock::slot<void>(kCall_229, pKey, pValue);
    }
    virtual void SetGameTags(const char* pchGameTags) {
        steammock::slot<void>(kCall_230, pchGameTags);
    }
    virtual void SetGameData(const char* pchGameData) {
        steammock::slot<void>(kCall_231, pchGameData);
    }
    virtual void SetRegion(const char* pszRegion) { steammock::slot<void>(kCall_232, pszRegion); }
    virtual bool SendUserConnectAndAuthenticate(std::uint32_t unIPClient, void* pvAuthBlob, std::uint32_t cubAuthBlobSize, CSteamID* pSteamIDUser) {
        return steammock::slot<bool>(kCall_233, unIPClient, pvAuthBlob, cubAuthBlobSize, pSteamIDUser);
    }
    virtual CSteamID CreateUnauthenticatedUserConnection() {
        return steammock::slot<CSteamID>(kCall_234);
    }
    virtual void SendUserDisconnect(CSteamID steamIDUser) {
        steammock::slot<void>(kCall_235, steamIDUser);
    }
    virtual bool BUpdateUserData(CSteamID steamIDUser, const char* pchPlayerName, std::uint32_t uScore) {
        return steammock::slot<bool>(kCall_236, steamIDUser, pchPlayerName, uScore);
    }
    virtual std::uint32_t GetAuthSessionTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket) {
        return steammock::slot<std::uint32_t>(kCall_237, pTicket, cbMaxTicket, pcbTicket);
    }
    virtual std::int32_t BeginAuthSession(void* pAuthTicket, std::int32_t cbAuthTicket, CSteamID steamID) {
        return steammock::slot<std::int32_t>(kCall_238, pAuthTicket, cbAuthTicket, steamID);
    }
    virtual void EndAuthSession(CSteamID steamID) { steammock::slot<void>(kCall_239, steamID); }
    virtual void CancelAuthTicket(std::uint32_t hAuthTicket) {
        steammock::slot<void>(kCall_240, hAuthTicket);
    }
    virtual std::int32_t UserHasLicenseForApp(CSteamID steamID, std::uint32_t appID) {
        return steammock::slot<std::int32_t>(kCall_241, steamID, appID);
    }
    virtual bool RequestUserGroupStatus(CSteamID steamIDUser, CSteamID steamIDGroup) {
        return steammock::slot<bool>(kCall_242, steamIDUser, steamIDGroup);
    }
    virtual void GetGameplayStats() { steammock::slot<void>(kCall_243); }
    virtual std::uint64_t GetServerReputation() {
        return steammock::slot<std::uint64_t>(kCall_244);
    }
    virtual std::uint32_t GetPublicIP() { return steammock::slot<std::uint32_t>(kCall_245); }
    virtual bool HandleIncomingPacket(void* pData, std::int32_t cbData, std::uint32_t srcIP, std::uint16_t srcPort) {
        return steammock::slot<bool>(kCall_246, pData, cbData, srcIP, srcPort);
    }
    virtual std::int32_t GetNextOutgoingPacket(void* pOut, std::int32_t cbMaxOut, std::uint32_t* pNetAdr, std::uint16_t* pPort) {
        return steammock::slot<std::int32_t>(kCall_247, pOut, cbMaxOut, pNetAdr, pPort);
    }
    virtual void EnableHeartbeats(bool bActive) { steammock::slot<void>(kCall_248, bActive); }
    virtual void SetHeartbeatInterval(std::int32_t iHeartbeatInterval) {
        steammock::slot<void>(kCall_249, iHeartbeatInterval);
    }
    virtual void ForceHeartbeat() { steammock::slot<void>(kCall_250); }
    virtual std::uint64_t AssociateWithClan(CSteamID steamIDClan) {
        return steammock::slot<std::uint64_t>(kCall_251, steamIDClan);
    }
    virtual std::uint64_t ComputeNewPlayerCompatibility(CSteamID steamIDNewPlayer) {
        return steammock::slot<std::uint64_t>(kCall_252, steamIDNewPlayer);
    }
};

Version_SteamGameServer012 g_SteamGameServer012;

// ISteamGameServer SteamGameServer013
class Version_SteamGameServer013 {
public:
    virtual bool InitGameServer(std::uint32_t unIP, std::uint16_t usGamePort, std::uint16_t usQueryPort, std::uint32_t unFlags, std::uint32_t nGameAppId, const char* pchVersionString) {
        return steammock::slot<bool>(kCall_253, unIP, usGamePort, usQueryPort, unFlags, nGameAppId, pchVersionString);
    }
    virtual void SetProduct(const char* pszProduct) {
        steammock::slot<void>(kCall_210, pszProduct);
    }
    virtual void SetGameDescription(const char* pszGameDescription) {
        steammock::slot<void>(kCall_211, pszGameDescription);
    }
    virtual void SetModDir(const char* pszModDir) { steammock::slot<void>(kCall_212, pszModDir); }
    virtual void SetDedicatedServer(bool bDedicated) {
        steammock::slot<void>(kCall_213, bDedicated);
    }
    virtual void LogOn(const char* pszToken) { steammock::slot<void>(kCall_214, pszToken); }
    virtual void LogOnAnonymous() { steammock::slot<void>(kCall_215); }
    virtual void LogOff() { steammock::slot<void>(kCall_216); }
    virtual bool BLoggedOn() { return steammock::slot<bool>(kCall_217); }
    virtual bool BSecure() { return steammock::slot<bool>(kCall_218); }
    virtual CSteamID GetSteamID() { return steammock::slot<CSteamID>(kCall_219); }
    virtual bool WasRestartRequested() { return steammock::slot<bool>(kCall_220); }
    virtual void SetMaxPlayerCount(std::int32_t cPlayersMax) {
        steammock::slot<void>(kCall_221, cPlayersMax);
    }
    virtual void SetBotPlayerCount(std::int32_t cBotplayers) {
        steammock::slot<void>(kCall_222, cBotplayers);
    }
    virtual void SetServerName(const char* pszServerName) {
        steammock::slot<void>(kCall_223, pszServerName);
    }
    virtual void SetMapName(const char* pszMapName) {
        steammock::slot<void>(kCall_224, pszMapName);
    }
    virtual void SetPasswordProtected(bool bPasswordProtected) {
        steammock::slot<void>(kCall_225, bPasswordProtected);
    }
    virtual void SetSpectatorPort(std::uint16_t unSpectatorPort) {
        steammock::slot<void>(kCall_226, unSpectatorPort);
    }
    virtual void SetSpectatorServerName(const char* pszSpectatorServerName) {
        steammock::slot<void>(kCall_227, pszSpectatorServerName);
    }
    virtual void ClearAllKeyValues() { steammock::slot<void>(kCall_228); }
    virtual void SetKeyValue(const char* pKey, const char* pValue) {
        steammock::slot<void>(kCall_229, pKey, pValue);
    }
    virtual void SetGameTags(const char* pchGameTags) {
        steammock::slot<void>(kCall_230, pchGameTags);
    }
    virtual void SetGameData(const char* pchGameData) {
        steammock::slot<void>(kCall_231, pchGameData);
    }
    virtual void SetRegion(const char* pszRegion) { steammock::slot<void>(kCall_232, pszRegion); }
    virtual bool SendUserConnectAndAuthenticate(std::uint32_t unIPClient, void* pvAuthBlob, std::uint32_t cubAuthBlobSize, CSteamID* pSteamIDUser) {
        return steammock::slot<bool>(kCall_233, unIPClient, pvAuthBlob, cubAuthBlobSize, pSteamIDUser);
    }
    virtual CSteamID CreateUnauthenticatedUserConnection() {
        return steammock::slot<CSteamID>(kCall_234);
    }
    virtual void SendUserDisconnect(CSteamID steamIDUser) {
        steammock::slot<void>(kCall_235, steamIDUser);
    }
    virtual bool BUpdateUserData(CSteamID steamIDUser, const char* pchPlayerName, std::uint32_t uScore) {
        return steammock::slot<bool>(kCall_236, steamIDUser, pchPlayerName, uScore);
    }
    virtual std::uint32_t GetAuthSessionTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket) {
        return steammock::slot<std::uint32_t>(kCall_237, pTicket, cbMaxTicket, pcbTicket);
    }
    virtual std::int32_t BeginAuthSession(void* pAuthTicket, std::int32_t cbAuthTicket, CSteamID steamID) {
        return steammock::slot<std::int32_t>(kCall_238, pAuthTicket, cbAuthTicket, steamID);
    }
    virtual void EndAuthSession(CSteamID steamID) { steammock::slot<void>(kCall_239, steamID); }
    virtual void CancelAuthTicket(std::uint32_t hAuthTicket) {
        steammock::slot<void>(kCall_240, hAuthTicket);
    }
    virtual std::int32_t UserHasLicenseForApp(CSteamID steamID, std::uint32_t appID) {
        return steammock::slot<std::int32_t>(kCall_241, steamID, appID);
    }
    virtual bool RequestUserGroupStatus(CSteamID steamIDUser, CSteamID steamIDGroup) {
        return steammock::slot<bool>(kCall_242, steamIDUser, steamIDGroup);
    }
    virtual void GetGameplayStats() { steammock::slot<void>(kCall_243); }
    virtual std::uint64_t GetServerReputation() {
        return steammock::slot<std::uint64_t>(kCall_244);
    }
    virtual SteamIPAddress_t GetPublicIP() { return steammock::slot<SteamIPAddress_t>(kCall_245); }
    virtual bool HandleIncomingPacket(void* pData, std::int32_t cbData, std::uint32_t srcIP, std::uint16_t srcPort) {
        return steammock::slot<bool>(kCall_246, pData, cbData, srcIP, srcPort);
    }
    virtual std::int32_t GetNextOutgoingPacket(void* pOut, std::int32_t cbMaxOut, std::uint32_t* pNetAdr, std::uint16_t* pPort) {
        return steammock::slot<std::int32_t>(kCall_247, pOut, cbMaxOut, pNetAdr, pPort);
    }
    virtual void EnableHeartbeats(bool bActive) { steammock::slot<void>(kCall_248, bActive); }
    virtual void SetHeartbeatInterval(std::int32_t iHeartbeatInterval) {
        steammock::slot<void>(kCall_249, iHeartbeatInterval);
    }
    virtual void ForceHeartbeat() { steammock::slot<void>(kCall_250); }
    virtual std::uint64_t AssociateWithClan(CSteamID steamIDClan) {
        return steammock::slot<std::uint64_t>(kCall_251, steamIDClan);
    }
    virtual std::uint64_t ComputeNewPlayerCompatibility(CSteamID steamIDNewPlayer) {
        return steammock::slot<std::uint64_t>(kCall_252, steamIDNewPlayer);
    }
};

Version_SteamGameServer013 g_SteamGameServer013;

// ISteamGameServer SteamGameServer014
class Version_SteamGameServer014 {
public:
    virtual bool InitGameServer(std::uint32_t unIP, std::uint16_t usGamePort, std::uint16_t usQueryPort, std::uint32_t unFlags, std::uint32_t nGameAppId, const char* pchVersionString) {
        return steammock::slot<bool>(kCall_253, unIP, usGamePort, usQueryPort, unFlags, nGameAppId, pchVersionString);
    }
    virtual void SetProduct(const char* pszProduct) {
        steammock::slot<void>(kCall_210, pszProduct);
    }
    virtual void SetGameDescription(const char* pszGameDescription) {
        steammock::slot<void>(kCall_211, pszGameDescription);
    }
    virtual void SetModDir(const char* pszModDir) { steammock::slot<void>(kCall_212, pszModDir); }
    virtual void SetDedicatedServer(bool bDedicated) {
        steammock::slot<void>(kCall_213, bDedicated);
    }
    virtual void LogOn(const char* pszToken) { steammock::slot<void>(kCall_214, pszToken); }
    virtual void LogOnAnonymous() { steammock::slot<void>(kCall_215); }
    virtual void LogOff() { steammock::slot<void>(kCall_216); }
    virtual bool BLoggedOn() { return steammock::slot<bool>(kCall_217); }
    virtual bool BSecure() { return steammock::slot<bool>(kCall_218); }
    virtual CSteamID GetSteamID() { return steammock::slot<CSteamID>(kCall_219); }
    virtual bool WasRestartRequested() { return steammock::slot<bool>(kCall_220); }
    virtual void SetMaxPlayerCount(std::int32_t cPlayersMax) {
        steammock::slot<void>(kCall_221, cPlayersMax);
    }
    virtual void SetBotPlayerCount(std::int32_t cBotplayers) {
        steammock::slot<void>(kCall_222, cBotplayers);
    }
    virtual void SetServerName(const char* pszServerName) {
        steammock::slot<void>(kCall_223, pszServerName);
    }
    virtual void SetMapName(const char* pszMapName) {
        steammock::slot<void>(kCall_224, pszMapName);
    }
    virtual void SetPasswordProtected(bool bPasswordProtected) {
        steammock::slot<void>(kCall_225, bPasswordProtected);
    }
    virtual void SetSpectatorPort(std::uint16_t unSpectatorPort) {
        steammock::slot<void>(kCall_226, unSpectatorPort);
    }
    virtual void SetSpectatorServerName(const char* pszSpectatorServerName) {
        steammock::slot<void>(kCall_227, pszSpectatorServerName);
    }
    virtual void ClearAllKeyValues() { steammock::slot<void>(kCall_228); }
    virtual void SetKeyValue(const char* pKey, const char* pValue) {
        steammock::slot<void>(kCall_229, pKey, pValue);
    }
    virtual void SetGameTags(const char* pchGameTags) {
        steammock::slot<void>(kCall_230, pchGameTags);
    }
    virtual void SetGameData(const char* pchGameData) {
        steammock::slot<void>(kCall_231, pchGameData);
    }
    virtual void SetRegion(const char* pszRegion) { steammock::slot<void>(kCall_232, pszRegion); }
    virtual void SetAdvertiseServerActive(bool bActive) {
        steammock::slot<void>(kCall_254, bActive);
    }
    virtual std::uint32_t GetAuthSessionTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket) {
        return steammock::slot<std::uint32_t>(kCall_237, pTicket, cbMaxTicket, pcbTicket);
    }
    virtual std::int32_t BeginAuthSession(void* pAuthTicket, std::int32_t cbAuthTicket, CSteamID steamID) {
        return steammock::slot<std::int32_t>(kCall_238, pAuthTicket, cbAuthTicket, steamID);
    }
    virtual void EndAuthSession(CSteamID steamID) { steammock::slot<void>(kCall_239, steamID); }
    virtual void CancelAuthTicket(std::uint32_t hAuthTicket) {
        steammock::slot<void>(kCall_240, hAuthTicket);
    }
    virtual std::int32_t UserHasLicenseForApp(CSteamID steamID, std::uint32_t appID) {
        return steammock::slot<std::int32_t>(kCall_241, steamID, appID);
    }
    virtual bool RequestUserGroupStatus(CSteamID steamIDUser, CSteamID steamIDGroup) {
        return steammock::slot<bool>(kCall_242, steamIDUser, steamIDGroup);
    }
    virtual void GetGameplayStats() { steammock::slot<void>(kCall_243); }
    virtual std::uint64_t GetServerReputation() {
        return steammock::slot<std::uint64_t>(kCall_244);
    }
    virtual SteamIPAddress_t GetPublicIP() { return steammock::slot<SteamIPAddress_t>(kCall_245); }
    virtual bool HandleIncomingPacket(void* pData, std::int32_t cbData, std::uint32_t srcIP, std::uint16_t srcPort) {
        return steammock::slot<bool>(kCall_246, pData, cbData, srcIP, srcPort);
    }
    virtual std::int32_t GetNextOutgoingPacket(void* pOut, std::int32_t cbMaxOut, std::uint32_t* pNetAdr, std::uint16_t* pPort) {
        return steammock::slot<std::int32_t>(kCall_247, pOut, cbMaxOut, pNetAdr, pPort);
    }
    virtual std::uint64_t AssociateWithClan(CSteamID steamIDClan) {
        return steammock::slot<std::uint64_t>(kCall_251, steamIDClan);
    }
    virtual std::uint64_t ComputeNewPlayerCompatibility(CSteamID steamIDNewPlayer) {
        return steammock::slot<std::uint64_t>(kCall_252, steamIDNewPlayer);
    }
    virtual bool SendUserConnectAndAuthenticate_DEPRECATED(std::uint32_t unIPClient, void* pvAuthBlob, std::uint32_t cubAuthBlobSize, CSteamID* pSteamIDUser) {
        return steammock::slot<bool>(kCall_255, unIPClient, pvAuthBlob, cubAuthBlobSize, pSteamIDUser);
    }
    virtual CSteamID CreateUnauthenticatedUserConnection() {
        return steammock::slot<CSteamID>(kCall_234);
    }
    virtual void SendUserDisconnect_DEPRECATED(CSteamID steamIDUser) {
        steammock::slot<void>(kCall_256, steamIDUser);
    }
    virtual bool BUpdateUserData(CSteamID steamIDUser, const char* pchPlayerName, std::uint32_t uScore) {
        return steammock::slot<bool>(kCall_236, steamIDUser, pchPlayerName, uScore);
    }
    virtual void SetMasterServerHeartbeatInterval_DEPRECATED(std::int32_t iHeartbeatInterval) {
        steammock::slot<void>(kCall_257, iHeartbeatInterval);
    }
    virtual void ForceMasterServerHeartbeat_DEPRECATED() { steammock::slot<void>(kCall_258); }
};

Version_SteamGameServer014 g_SteamGameServer014;

// ISteamGameServer SteamGameServer015
class Version_SteamGameServer015 {
public:
    virtual bool InitGameServer(std::uint32_t unIP, std::uint16_t usGamePort, std::uint16_t usQueryPort, std::uint32_t unFlags, std::uint32_t nGameAppId, const char* pchVersionString) {
        return steammock::slot<bool>(kCall_253, unIP, usGamePort, usQueryPort, unFlags, nGameAppId, pchVersionString);
    }
    virtual void SetProduct(const char* pszProduct) {
        steammock::slot<void>(kCall_210, pszProduct);
    }
    virtual void SetGameDescription(const char* pszGameDescription) {
        steammock::slot<void>(kCall_211, pszGameDescription);
    }
    virtual void SetModDir(const char* pszModDir) { steammock::slot<void>(kCall_212, pszModDir); }
    virtual void SetDedicatedServer(bool bDedicated) {
        steammock::slot<void>(kCall_213, bDedicated);
    }
    virtual void LogOn(const char* pszToken) { steammock::slot<void>(kCall_214, pszToken); }
    virtual void LogOnAnonymous() { steammock::slot<void>(kCall_215); }
    virtual void LogOff() { steammock::slot<void>(kCall_216); }
    virtual bool BLoggedOn() { return steammock::slot<bool>(kCall_217); }
    virtual bool BSecure() { return steammock::slot<bool>(kCall_218); }
    virtual CSteamID GetSteamID() { return steammock::slot<CSteamID>(kCall_219); }
    virtual bool WasRestartRequested() { return steammock::slot<bool>(kCall_220); }
    virtual void SetMaxPlayerCount(std::int32_t cPlayersMax) {
        steammock::slot<void>(kCall_221, cPlayersMax);
    }
    virtual void SetBotPlayerCount(std::int32_t cBotplayers) {
        steammock::slot<void>(kCall_222, cBotplayers);
    }
    virtual void SetServerName(const char* pszServerName) {
        steammock::slot<void>(kCall_223, pszServerName);
    }
    virtual void SetMapName(const char* pszMapName) {
        steammock::slot<void>(kCall_224, pszMapName);
    }
    virtual void SetPasswordProtected(bool bPasswordProtected) {
        steammock::slot<void>(kCall_225, bPasswordProtected);
    }
    virtual void SetSpectatorPort(std::uint16_t unSpectatorPort) {
        steammock::slot<void>(kCall_226, unSpectatorPort);
    }
    virtual void SetSpectatorServerName(const char* pszSpectatorServerName) {
        steammock::slot<void>(kCall_227, pszSpectatorServerName);
    }
    virtual void ClearAllKeyValues() { steammock::slot<void>(kCall_228); }
    virtual void SetKeyValue(const char* pKey, const char* pValue) {
        steammock::slot<void>(kCall_229, pKey, pValue);
    }
    virtual void SetGameTags(const char* pchGameTags) {
        steammock::slot<void>(kCall_230, pchGameTags);
    }
    virtual void SetGameData(const char* pchGameData) {
        steammock::slot<void>(kCall_231, pchGameData);
    }
    virtual void SetRegion(const char* pszRegion) { steammock::slot<void>(kCall_232, pszRegion); }
    virtual void SetAdvertiseServerActive(bool bActive) {
        steammock::slot<void>(kCall_254, bActive);
    }
    virtual std::uint32_t GetAuthSessionTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket, void* pSnid) {
        return steammock::slot<std::uint32_t>(kCall_259, pTicket, cbMaxTicket, pcbTicket, pSnid);
    }
    virtual std::int32_t BeginAuthSession(void* pAuthTicket, std::int32_t cbAuthTicket, CSteamID steamID) {
        return steammock::slot<std::int32_t>(kCall_238, pAuthTicket, cbAuthTicket, steamID);
    }
    virtual void EndAuthSession(CSteamID steamID) { steammock::slot<void>(kCall_239, steamID); }
    virtual void CancelAuthTicket(std::uint32_t hAuthTicket) {
        steammock::slot<void>(kCall_240, hAuthTicket);
    }
    virtual std::int32_t UserHasLicenseForApp(CSteamID steamID, std::uint32_t appID) {
        return steammock::slot<std::int32_t>(kCall_241, steamID, appID);
    }
    virtual bool RequestUserGroupStatus(CSteamID steamIDUser, CSteamID steamIDGroup) {
        return steammock::slot<bool>(kCall_242, steamIDUser, steamIDGroup);
    }
    virtual void GetGameplayStats() { steammock::slot<void>(kCall_243); }
    virtual std::uint64_t GetServerReputation() {
        return steammock::slot<std::uint64_t>(kCall_244);
    }
    virtual SteamIPAddress_t GetPublicIP() { return steammock::slot<SteamIPAddress_t>(kCall_245); }
    virtual bool HandleIncomingPacket(void* pData, std::int32_t cbData, std::uint32_t srcIP, std::uint16_t srcPort) {
        return steammock::slot<bool>(kCall_246, pData, cbData, srcIP, srcPort);
    }
    virtual std::int32_t GetNextOutgoingPacket(void* pOut, std::int32_t cbMaxOut, std::uint32_t* pNetAdr, std::uint16_t* pPort) {
        return steammock::slot<std::int32_t>(kCall_247, pOut, cbMaxOut, pNetAdr, pPort);
    }
    virtual std::uint64_t AssociateWithClan(CSteamID steamIDClan) {
        return steammock::slot<std::uint64_t>(kCall_251, steamIDClan);
    }
    virtual std::uint64_t ComputeNewPlayerCompatibility(CSteamID steamIDNewPlayer) {
        return steammock::slot<std::uint64_t>(kCall_252, steamIDNewPlayer);
    }
    virtual bool SendUserConnectAndAuthenticate_DEPRECATED(std::uint32_t unIPClient, void* pvAuthBlob, std::uint32_t cubAuthBlobSize, CSteamID* pSteamIDUser) {
        return steammock::slot<bool>(kCall_255, unIPClient, pvAuthBlob, cubAuthBlobSize, pSteamIDUser);
    }
    virtual CSteamID CreateUnauthenticatedUserConnection() {
        return steammock::slot<CSteamID>(kCall_234);
    }
    virtual void SendUserDisconnect_DEPRECATED(CSteamID steamIDUser) {
        steammock::slot<void>(kCall_256, steamIDUser);
    }
    virtual bool BUpdateUserData(CSteamID steamIDUser, const char* pchPlayerName, std::uint32_t uScore) {
        return steammock::slot<bool>(kCall_236, steamIDUser, pchPlayerName, uScore);
    }
    virtual void SetMasterServerHeartbeatInterval_DEPRECATED(std::int32_t iHeartbeatInterval) {
        steammock::slot<void>(kCall_257, iHeartbeatInterval);
    }
    virtual void ForceMasterServerHeartbeat_DEPRECATED() { steammock::slot<void>(kCall_258); }
};

Version_SteamGameServer015 g_SteamGameServer015;

// ISteamGameServerStats SteamGameServerStats001
class Version_SteamGameServerStats001 {
public:
    virtual std::uint64_t RequestUserStats(CSteamID steamIDUser) {
        return steammock::slot<std::uint64_t>(kCall_260, steamIDUser);
    }
    virtual bool GetUserStat(CSteamID steamIDUser, const char* pchName, std::int32_t* pData) {
        return steammock::slot<bool>(kCall_261, steamIDUser, pchName, pData);
    }
    virtual bool GetUserStat(CSteamID steamIDUser, const char* pchName, float* pData) {
        return steammock::slot<bool>(kCall_262, steamIDUser, pchName, pData);
    }
    virtual bool GetUserAchievement(CSteamID steamIDUser, const char* pchName, bool* pbAchieved) {
        return steammock::slot<bool>(kCall_263, steamIDUser, pchName, pbAchieved);
    }
    virtual bool SetUserStat(CSteamID steamIDUser, const char* pchName, std::int32_t nData) {
        return steammock::slot<bool>(kCall_264, steamIDUser, pchName, nData);
    }
    virtual bool SetUserStat(CSteamID steamIDUser, const char* pchName, float fData) {
        return steammock::slot<bool>(kCall_265, steamIDUser, pchName, fData);
    }
    virtual bool UpdateUserAvgRateStat(CSteamID steamIDUser, const char* pchName, float flCountThisSession, double dSessionLength) {
        return steammock::slot<bool>(kCall_266, steamIDUser, pchName, flCountThisSession, dSessionLength);
    }
    virtual bool SetUserAchievement(CSteamID steamIDUser, const char* pchName) {
        return steammock::slot<bool>(kCall_267, steamIDUser, pchName);
    }
    virtual bool ClearUserAchievement(CSteamID steamIDUser, const char* pchName) {
        return steammock::slot<bool>(kCall_268, steamIDUser, pchName);
    }
    virtual std::uint64_t StoreUserStats(CSteamID steamIDUser) {
        return steammock::slot<std::uint64_t>(kCall_269, steamIDUser);
    }
};

Version_SteamGameServerStats001 g_SteamGameServerStats001;

// ISteamHTMLSurface STEAMHTMLSURFACE_INTERFACE_VERSION_004
class Version_STEAMHTMLSURFACE_INTERFACE_VERSION_004 {
public:
    virtual ~Version_STEAMHTMLSURFACE_INTERFACE_VERSION_004() {}
    virtual bool Init() { return steammock::slot<bool>(kCall_270); }
    virtual bool Shutdown() { return steammock::slot<bool>(kCall_271); }
    virtual std::uint64_t CreateBrowser(const char* pchUserAgent, const char* pchUserCSS) {
        return steammock::slot<std::uint64_t>(kCall_272, pchUserAgent, pchUserCSS);
    }
    virtual void RemoveBrowser(std::uint32_t unBrowserHandle) {
        steammock::slot<void>(kCall_273, unBrowserHandle);
    }
    virtual void LoadURL(std::uint32_t unBrowserHandle, const char* pchURL, const char* pchPostData) {
        steammock::slot<void>(kCall_274, unBrowserHandle, pchURL, pchPostData);
    }
    virtual void SetSize(std::uint32_t unBrowserHandle, std::uint32_t unWidth, std::uint32_t unHeight) {
        steammock::slot<void>(kCall_275, unBrowserHandle, unWidth, unHeight);
    }
    virtual void StopLoad(std::uint32_t unBrowserHandle) {
        steammock::slot<void>(kCall_276, unBrowserHandle);
    }
    virtual void Reload(std::uint32_t unBrowserHandle) {
        steammock::slot<void>(kCall_277, unBrowserHandle);
    }
    virtual void GoBack(std::uint32_t unBrowserHandle) {
        steammock::slot<void>(kCall_278, unBrowserHandle);
    }
    virtual void GoForward(std::uint32_t unBrowserHandle) {
        steammock::slot<void>(kCall_279, unBrowserHandle);
    }
    virtual void AddHeader(std::uint32_t unBrowserHandle, const char* pchKey, const char* pchValue) {
        steammock::slot<void>(kCall_280, unBrowserHandle, pchKey, pchValue);
    }
    virtual void ExecuteJavascript(std::uint32_t unBrowserHandle, const char* pchScript) {
        steammock::slot<void>(kCall_281, unBrowserHandle, pchScript);
    }
    virtual void MouseUp(std::uint32_t unBrowserHandle, std::int32_t eMouseButton) {
        steammock::slot<void>(kCall_282, unBrowserHandle, eMouseButton);
    }
    virtual void MouseDown(std::uint32_t unBrowserHandle, std::int32_t eMouseButton) {
        steammock::slot<void>(kCall_283, unBrowserHandle, eMouseButton);
    }
    virtual void MouseDoubleClick(std::uint32_t unBrowserHandle, std::int32_t eMouseButton) {
        steammock::slot<void>(kCall_284, unBrowserHandle, eMouseButton);
    }
    virtual void MouseMove(std::uint32_t unBrowserHandle, std::int32_t x, std::int32_t y) {
        steammock::slot<void>(kCall_285, unBrowserHandle, x, y);
    }
    virtual void MouseWheel(std::uint32_t unBrowserHandle, std::int32_t nDelta) {
        steammock::slot<void>(kCall_286, unBrowserHandle, nDelta);
    }
    virtual void KeyDown(std::uint32_t unBrowserHandle, std::uint32_t nNativeKeyCode, std::int32_t eHTMLKeyModifiers) {
        steammock::slot<void>(kCall_287, unBrowserHandle, nNativeKeyCode, eHTMLKeyModifiers);
    }
    virtual void KeyUp(std::uint32_t unBrowserHandle, std::uint32_t nNativeKeyCode, std::int32_t eHTMLKeyModifiers) {
        steammock::slot<void>(kCall_288, unBrowserHandle, nNativeKeyCode, eHTMLKeyModifiers);
    }
    virtual void KeyChar(std::uint32_t unBrowserHandle, std::uint32_t cUnicodeChar, std::int32_t eHTMLKeyModifiers) {
        steammock::slot<void>(kCall_289, unBrowserHandle, cUnicodeChar, eHTMLKeyModifiers);
    }
    virtual void SetHorizontalScroll(std::uint32_t unBrowserHandle, std::uint32_t nAbsolutePixelScroll) {
        steammock::slot<void>(kCall_290, unBrowserHandle, nAbsolutePixelScroll);
    }
    virtual void SetVerticalScroll(std::uint32_t unBrowserHandle, std::uint32_t nAbsolutePixelScroll) {
        steammock::slot<void>(kCall_291, unBrowserHandle, nAbsolutePixelScroll);
    }
    virtual void SetKeyFocus(std::uint32_t unBrowserHandle, bool bHasKeyFocus) {
        steammock::slot<void>(kCall_292, unBrowserHandle, bHasKeyFocus);
    }
    virtual void ViewSource(std::uint32_t unBrowserHandle) {
        steammock::slot<void>(kCall_293, unBrowserHandle);
    }
    virtual void CopyToClipboard(std::uint32_t unBrowserHandle) {
        steammock::slot<void>(kCall_294, unBrowserHandle);
    }
    virtual void PasteFromClipboard(std::uint32_t unBrowserHandle) {
        steammock::slot<void>(kCall_295, unBrowserHandle);
    }
    virtual void Find(std::uint32_t unBrowserHandle, const char* pchSearchStr, bool bCurrentlyInFind, bool bReverse) {
        steammock::slot<void>(kCall_296, unBrowserHandle, pchSearchStr, bCurrentlyInFind, bReverse);
    }
    virtual void StopFind(std::uint32_t unBrowserHandle) {
        steammock::slot<void>(kCall_297, unBrowserHandle);
    }
    virtual void GetLinkAtPosition(std::uint32_t unBrowserHandle, std::int32_t x, std::int32_t y) {
        steammock::slot<void>(kCall_298, unBrowserHandle, x, y);
    }
    virtual void SetCookie(const char* pchHostname, const char* pchKey, const char* pchValue, const char* pchPath, std::uint32_t nExpires, bool bSecure, bool bHTTPOnly) {
        steammock::slot<void>(kCall_299, pchHostname, pchKey, pchValue, pchPath, nExpires, bSecure, bHTTPOnly);
    }
    virtual void SetPageScaleFactor(std::uint32_t unBrowserHandle, float flZoom, std::int32_t nPointX, std::int32_t nPointY) {
        steammock::slot<void>(kCall_300, unBrowserHandle, flZoom, nPointX, nPointY);
    }
    virtual void SetBackgroundMode(std::uint32_t unBrowserHandle, bool bBackgroundMode) {
        steammock::slot<void>(kCall_301, unBrowserHandle, bBackgroundMode);
    }
    virtual void SetDPIScalingFactor(std::uint32_t unBrowserHandle, float flDPIScaling) {
        steammock::slot<void>(kCall_302, unBrowserHandle, flDPIScaling);
    }
    virtual void AllowStartRequest(std::uint32_t unBrowserHandle, bool bAllowed) {
        steammock::slot<void>(kCall_303, unBrowserHandle, bAllowed);
    }
    virtual void JSDialogResponse(std::uint32_t unBrowserHandle, bool bResult) {
        steammock::slot<void>(kCall_304, unBrowserHandle, bResult);
    }
    virtual void FileLoadDialogResponse(std::uint32_t unBrowserHandle, void* pchSelectedFiles) {
        steammock::slot<void>(kCall_305, unBrowserHandle, pchSelectedFiles);
    }
};

Version_STEAMHTMLSURFACE_INTERFACE_VERSION_004 g_STEAMHTMLSURFACE_INTERFACE_VERSION_004;

// ISteamHTMLSurface STEAMHTMLSURFACE_INTERFACE_VERSION_005
class Version_STEAMHTMLSURFACE_INTERFACE_VERSION_005 {
public:
    virtual ~Version_STEAMHTMLSURFACE_INTERFACE_VERSION_005() {}
    virtual bool Init() { return steammock::slot<bool>(kCall_270); }
    virtual bool Shutdown() { return steammock::slot<bool>(kCall_271); }
    virtual std::uint64_t CreateBrowser(const char* pchUserAgent, const char* pchUserCSS) {
        return steammock::slot<std::uint64_t>(kCall_272, pchUserAgent, pchUserCSS);
    }
    virtual void RemoveBrowser(std::uint32_t unBrowserHandle) {
        steammock::slot<void>(kCall_273, unBrowserHandle);
    }
    virtual void LoadURL(std::uint32_t unBrowserHandle, const char* pchURL, const char* pchPostData) {
        steammock::slot<void>(kCall_274, unBrowserHandle, pchURL, pchPostData);
    }
    virtual void SetSize(std::uint32_t unBrowserHandle, std::uint32_t unWidth, std::uint32_t unHeight) {
        steammock::slot<void>(kCall_275, unBrowserHandle, unWidth, unHeight);
    }
    virtual void StopLoad(std::uint32_t unBrowserHandle) {
        steammock::slot<void>(kCall_276, unBrowserHandle);
    }
    virtual void Reload(std::uint32_t unBrowserHandle) {
        steammock::slot<void>(kCall_277, unBrowserHandle);
    }
    virtual void GoBack(std::uint32_t unBrowserHandle) {
        steammock::slot<void>(kCall_278, unBrowserHandle);
    }
    virtual void GoForward(std::uint32_t unBrowserHandle) {
        steammock::slot<void>(kCall_279, unBrowserHandle);
    }
    virtual void AddHeader(std::uint32_t unBrowserHandle, const char* pchKey, const char* pchValue) {
        steammock::slot<void>(kCall_280, unBrowserHandle, pchKey, pchValue);
    }
    virtual void ExecuteJavascript(std::uint32_t unBrowserHandle, const char* pchScript) {
        steammock::slot<void>(kCall_281, unBrowserHandle, pchScript);
    }
    virtual void MouseUp(std::uint32_t unBrowserHandle, std::int32_t eMouseButton) {
        steammock::slot<void>(kCall_282, unBrowserHandle, eMouseButton);
    }
    virtual void MouseDown(std::uint32_t unBrowserHandle, std::int32_t eMouseButton) {
        steammock::slot<void>(kCall_283, unBrowserHandle, eMouseButton);
    }
    virtual void MouseDoubleClick(std::uint32_t unBrowserHandle, std::int32_t eMouseButton) {
        steammock::slot<void>(kCall_284, unBrowserHandle, eMouseButton);
    }
    virtual void MouseMove(std::uint32_t unBrowserHandle, std::int32_t x, std::int32_t y) {
        steammock::slot<void>(kCall_285, unBrowserHandle, x, y);
    }
    virtual void MouseWheel(std::uint32_t unBrowserHandle, std::int32_t nDelta) {
        steammock::slot<void>(kCall_286, unBrowserHandle, nDelta);
    }
    virtual void KeyDown(std::uint32_t unBrowserHandle, std::uint32_t nNativeKeyCode, std::int32_t eHTMLKeyModifiers, bool bIsSystemKey) {
        steammock::slot<void>(kCall_306, unBrowserHandle, nNativeKeyCode, eHTMLKeyModifiers, bIsSystemKey);
    }
    virtual void KeyUp(std::uint32_t unBrowserHandle, std::uint32_t nNativeKeyCode, std::int32_t eHTMLKeyModifiers) {
        steammock::slot<void>(kCall_288, unBrowserHandle, nNativeKeyCode, eHTMLKeyModifiers);
    }
    virtual void KeyChar(std::uint32_t unBrowserHandle, std::uint32_t cUnicodeChar, std::int32_t eHTMLKeyModifiers) {
        steammock::slot<void>(kCall_289, unBrowserHandle, cUnicodeChar, eHTMLKeyModifiers);
    }
    virtual void SetHorizontalScroll(std::uint32_t unBrowserHandle, std::uint32_t nAbsolutePixelScroll) {
        steammock::slot<void>(kCall_290, unBrowserHandle, nAbsolutePixelScroll);
    }
    virtual void SetVerticalScroll(std::uint32_t unBrowserHandle, std::uint32_t nAbsolutePixelScroll) {
        steammock::slot<void>(kCall_291, unBrowserHandle, nAbsolutePixelScroll);
    }
    virtual void SetKeyFocus(std::uint32_t unBrowserHandle, bool bHasKeyFocus) {
        steammock::slot<void>(kCall_292, unBrowserHandle, bHasKeyFocus);
    }
    virtual void ViewSource(std::uint32_t unBrowserHandle) {
        steammock::slot<void>(kCall_293, unBrowserHandle);
    }
    virtual void CopyToClipboard(std::uint32_t unBrowserHandle) {
        steammock::slot<void>(kCall_294, unBrowserHandle);
    }
    virtual void PasteFromClipboard(std::uint32_t unBrowserHandle) {
        steammock::slot<void>(kCall_295, unBrowserHandle);
    }
    virtual void Find(std::uint32_t unBrowserHandle, const char* pchSearchStr, bool bCurrentlyInFind, bool bReverse) {
        steammock::slot<void>(kCall_296, unBrowserHandle, pchSearchStr, bCurrentlyInFind, bReverse);
    }
    virtual void StopFind(std::uint32_t unBrowserHandle) {
        steammock::slot<void>(kCall_297, unBrowserHandle);
    }
    virtual void GetLinkAtPosition(std::uint32_t unBrowserHandle, std::int32_t x, std::int32_t y) {
        steammock::slot<void>(kCall_298, unBrowserHandle, x, y);
    }
    virtual void SetCookie(const char* pchHostname, const char* pchKey, const char* pchValue, const char* pchPath, std::uint32_t nExpires, bool bSecure, bool bHTTPOnly) {
        steammock::slot<void>(kCall_299, pchHostname, pchKey, pchValue, pchPath, nExpires, bSecure, bHTTPOnly);
    }
    virtual void SetPageScaleFactor(std::uint32_t unBrowserHandle, float flZoom, std::int32_t nPointX, std::int32_t nPointY) {
        steammock::slot<void>(kCall_300, unBrowserHandle, flZoom, nPointX, nPointY);
    }
    virtual void SetBackgroundMode(std::uint32_t unBrowserHandle, bool bBackgroundMode) {
        steammock::slot<void>(kCall_301, unBrowserHandle, bBackgroundMode);
    }
    virtual void SetDPIScalingFactor(std::uint32_t unBrowserHandle, float flDPIScaling) {
        steammock::slot<void>(kCall_302, unBrowserHandle, flDPIScaling);
    }
    virtual void OpenDeveloperTools(std::uint32_t unBrowserHandle) {
        steammock::slot<void>(kCall_307, unBrowserHandle);
    }
    virtual void AllowStartRequest(std::uint32_t unBrowserHandle, bool bAllowed) {
        steammock::slot<void>(kCall_303, unBrowserHandle, bAllowed);
    }
    virtual void JSDialogResponse(std::uint32_t unBrowserHandle, bool bResult) {
        steammock::slot<void>(kCall_304, unBrowserHandle, bResult);
    }
    virtual void FileLoadDialogResponse(std::uint32_t unBrowserHandle, void* pchSelectedFiles) {
        steammock::slot<void>(kCall_308, unBrowserHandle, pchSelectedFiles);
    }
};

Version_STEAMHTMLSURFACE_INTERFACE_VERSION_005 g_STEAMHTMLSURFACE_INTERFACE_VERSION_005;

// ISteamHTTP STEAMHTTP_INTERFACE_VERSION002
class Version_STEAMHTTP_INTERFACE_VERSION002 {
public:
    virtual std::uint32_t CreateHTTPRequest(std::int32_t eHTTPRequestMethod, const char* pchAbsoluteURL) {
        return steammock::slot<std::uint32_t>(kCall_309, eHTTPRequestMethod, pchAbsoluteURL);
    }
    virtual bool SetHTTPRequestContextValue(std::uint32_t hRequest, std::uint64_t ulContextValue) {
        return steammock::slot<bool>(kCall_310, hRequest, ulContextValue);
    }
    virtual bool SetHTTPRequestNetworkActivityTimeout(std::uint32_t hRequest, std::uint32_t unTimeoutSeconds) {
        return steammock::slot<bool>(kCall_311, hRequest, unTimeoutSeconds);
    }
    virtual bool SetHTTPRequestHeaderValue(std::uint32_t hRequest, const char* pchHeaderName, const char* pchHeaderValue) {
        return steammock::slot<bool>(kCall_312, hRequest, pchHeaderName, pchHeaderValue);
    }
    virtual bool SetHTTPRequestGetOrPostParameter(std::uint32_t hRequest, const char* pchParamName, const char* pchParamValue) {
        return steammock::slot<bool>(kCall_313, hRequest, pchParamName, pchParamValue);
    }
    virtual bool SendHTTPRequest(std::uint32_t hRequest, std::uint64_t* pCallHandle) {
        return steammock::slot<bool>(kCall_314, hRequest, pCallHandle);
    }
    virtual bool SendHTTPRequestAndStreamResponse(std::uint32_t hRequest, std::uint64_t* pCallHandle) {
        return steammock::slot<bool>(kCall_315, hRequest, pCallHandle);
    }
    virtual bool DeferHTTPRequest(std::uint32_t hRequest) {
        return steammock::slot<bool>(kCall_316, hRequest);
    }
    virtual bool PrioritizeHTTPRequest(std::uint32_t hRequest) {
        return steammock::slot<bool>(kCall_317, hRequest);
    }
    virtual bool GetHTTPResponseHeaderSize(std::uint32_t hRequest, const char* pchHeaderName, std::uint32_t* unResponseHeaderSize) {
        return steammock::slot<bool>(kCall_318, hRequest, pchHeaderName, unResponseHeaderSize);
    }
    virtual bool GetHTTPResponseHeaderValue(std::uint32_t hRequest, const char* pchHeaderName, std::uint8_t* pHeaderValueBuffer, std::uint32_t unBufferSize) {
        return steammock::slot<bool>(kCall_319, hRequest, pchHeaderName, pHeaderValueBuffer, unBufferSize);
    }
    virtual bool GetHTTPResponseBodySize(std::uint32_t hRequest, std::uint32_t* unBodySize) {
        return steammock::slot<bool>(kCall_320, hRequest, unBodySize);
    }
    virtual bool GetHTTPResponseBodyData(std::uint32_t hRequest, std::uint8_t* pBodyDataBuffer, std::uint32_t unBufferSize) {
        return steammock::slot<bool>(kCall_321, hRequest, pBodyDataBuffer, unBufferSize);
    }
    virtual bool GetHTTPStreamingResponseBodyData(std::uint32_t hRequest, std::uint32_t cOffset, std::uint8_t* pBodyDataBuffer, std::uint32_t unBufferSize) {
        return steammock::slot<bool>(kCall_322, hRequest, cOffset, pBodyDataBuffer, unBufferSize);
    }
    virtual bool ReleaseHTTPRequest(std::uint32_t hRequest) {
        return steammock::slot<bool>(kCall_323, hRequest);
    }
    virtual bool GetHTTPDownloadProgressPct(std::uint32_t hRequest, float* pflPercentOut) {
        return steammock::slot<bool>(kCall_324, hRequest, pflPercentOut);
    }
    virtual bool SetHTTPRequestRawPostBody(std::uint32_t hRequest, const char* pchContentType, std::uint8_t* pubBody, std::uint32_t unBodyLen) {
        return steammock::slot<bool>(kCall_325, hRequest, pchContentType, pubBody, unBodyLen);
    }
    virtual std::uint32_t CreateCookieContainer(bool bAllowResponsesToModify) {
        return steammock::slot<std::uint32_t>(kCall_326, bAllowResponsesToModify);
    }
    virtual bool ReleaseCookieContainer(std::uint32_t hCookieContainer) {
        return steammock::slot<bool>(kCall_327, hCookieContainer);
    }
    virtual bool SetCookie(std::uint32_t hCookieContainer, const char* pchHost, const char* pchUrl, const char* pchCookie) {
        return steammock::slot<bool>(kCall_328, hCookieContainer, pchHost, pchUrl, pchCookie);
    }
    virtual bool SetHTTPRequestCookieContainer(std::uint32_t hRequest, std::uint32_t hCookieContainer) {
        return steammock::slot<bool>(kCall_329, hRequest, hCookieContainer);
    }
    virtual bool SetHTTPRequestUserAgentInfo(std::uint32_t hRequest, const char* pchUserAgentInfo) {
        return steammock::slot<bool>(kCall_330, hRequest, pchUserAgentInfo);
    }
    virtual bool SetHTTPRequestRequiresVerifiedCertificate(std::uint32_t hRequest, bool bRequireVerifiedCertificate) {
        return steammock::slot<bool>(kCall_331, hRequest, bRequireVerifiedCertificate);
    }
    virtual bool SetHTTPRequestAbsoluteTimeoutMS(std::uint32_t hRequest, std::uint32_t unMilliseconds) {
        return steammock::slot<bool>(kCall_332, hRequest, unMilliseconds);
    }
    virtual bool GetHTTPRequestWasTimedOut(std::uint32_t hRequest, bool* pbWasTimedOut) {
        return steammock::slot<bool>(kCall_333, hRequest, pbWasTimedOut);
    }
};

Version_STEAMHTTP_INTERFACE_VERSION002 g_STEAMHTTP_INTERFACE_VERSION002;

// ISteamHTTP STEAMHTTP_INTERFACE_VERSION003
class Version_STEAMHTTP_INTERFACE_VERSION003 {
public:
    virtual std::uint32_t CreateHTTPRequest(std::int32_t eHTTPRequestMethod, const char* pchAbsoluteURL) {
        return steammock::slot<std::uint32_t>(kCall_309, eHTTPRequestMethod, pchAbsoluteURL);
    }
    virtual bool SetHTTPRequestContextValue(std::uint32_t hRequest, std::uint64_t ulContextValue) {
        return steammock::slot<bool>(kCall_310, hRequest, ulContextValue);
    }
    virtual bool SetHTTPRequestNetworkActivityTimeout(std::uint32_t hRequest, std::uint32_t unTimeoutSeconds) {
        return steammock::slot<bool>(kCall_311, hRequest, unTimeoutSeconds);
    }
    virtual bool SetHTTPRequestHeaderValue(std::uint32_t hRequest, const char* pchHeaderName, const char* pchHeaderValue) {
        return steammock::slot<bool>(kCall_312, hRequest, pchHeaderName, pchHeaderValue);
    }
    virtual bool SetHTTPRequestGetOrPostParameter(std::uint32_t hRequest, const char* pchParamName, const char* pchParamValue) {
        return steammock::slot<bool>(kCall_313, hRequest, pchParamName, pchParamValue);
    }
    virtual bool SendHTTPRequest(std::uint32_t hRequest, std::uint64_t* pCallHandle) {
        return steammock::slot<bool>(kCall_314, hRequest, pCallHandle);
    }
    virtual bool SendHTTPRequestAndStreamResponse(std::uint32_t hRequest, std::uint64_t* pCallHandle) {
        return steammock::slot<bool>(kCall_315, hRequest, pCallHandle);
    }
    virtual bool DeferHTTPRequest(std::uint32_t hRequest) {
        return steammock::slot<bool>(kCall_316, hRequest);
    }
    virtual bool PrioritizeHTTPRequest(std::uint32_t hRequest) {
        return steammock::slot<bool>(kCall_317, hRequest);
    }
    virtual bool GetHTTPResponseHeaderSize(std::uint32_t hRequest, const char* pchHeaderName, std::uint32_t* unResponseHeaderSize) {
        return steammock::slot<bool>(kCall_318, hRequest, pchHeaderName, unResponseHeaderSize);
    }
    virtual bool GetHTTPResponseHeaderValue(std::uint32_t hRequest, const char* pchHeaderName, std::uint8_t* pHeaderValueBuffer, std::uint32_t unBufferSize) {
        return steammock::slot<bool>(kCall_319, hRequest, pchHeaderName, pHeaderValueBuffer, unBufferSize);
    }
    virtual bool GetHTTPResponseBodySize(std::uint32_t hRequest, std::uint32_t* unBodySize) {
        return steammock::slot<bool>(kCall_320, hRequest, unBodySize);
    }
    virtual bool GetHTTPResponseBodyData(std::uint32_t hRequest, std::uint8_t* pBodyDataBuffer, std::uint32_t unBufferSize) {
        return steammock::slot<bool>(kCall_321, hRequest, pBodyDataBuffer, unBufferSize);
    }
    virtual bool GetHTTPStreamingResponseBodyData(std::uint32_t hRequest, std::uint32_t cOffset, std::uint8_t* pBodyDataBuffer, std::uint32_t unBufferSize) {
        return steammock::slot<bool>(kCall_322, hRequest, cOffset, pBodyDataBuffer, unBufferSize);
    }
    virtual bool ReleaseHTTPRequest(std::uint32_t hRequest) {
        return steammock::slot<bool>(kCall_323, hRequest);
    }
    virtual bool GetHTTPDownloadProgressPct(std::uint32_t hRequest, float* pflPercentOut) {
        return steammock::slot<bool>(kCall_324, hRequest, pflPercentOut);
    }
    virtual bool SetHTTPRequestRawPostBody(std::uint32_t hRequest, const char* pchContentType, std::uint8_t* pubBody, std::uint32_t unBodyLen) {
        return steammock::slot<bool>(kCall_325, hRequest, pchContentType, pubBody, unBodyLen);
    }
    virtual std::uint32_t CreateCookieContainer(bool bAllowResponsesToModify) {
        return steammock::slot<std::uint32_t>(kCall_326, bAllowResponsesToModify);
    }
    virtual bool ReleaseCookieContainer(std::uint32_t hCookieContainer) {
        return steammock::slot<bool>(kCall_327, hCookieContainer);
    }
    virtual bool SetCookie(std::uint32_t hCookieContainer, const char* pchHost, const char* pchUrl, const char* pchCookie) {
        return steammock::slot<bool>(kCall_328, hCookieContainer, pchHost, pchUrl, pchCookie);
    }
    virtual bool SetHTTPRequestCookieContainer(std::uint32_t hRequest, std::uint32_t hCookieContainer) {
        return steammock::slot<bool>(kCall_329, hRequest, hCookieContainer);
    }
    virtual bool SetHTTPRequestUserAgentInfo(std::uint32_t hRequest, const char* pchUserAgentInfo) {
        return steammock::slot<bool>(kCall_330, hRequest, pchUserAgentInfo);
    }
    virtual bool SetHTTPRequestRequiresVerifiedCertificate(std::uint32_t hRequest, bool bRequireVerifiedCertificate) {
        return steammock::slot<bool>(kCall_331, hRequest, bRequireVerifiedCertificate);
    }
    virtual bool SetHTTPRequestAbsoluteTimeoutMS(std::uint32_t hRequest, std::uint32_t unMilliseconds) {
        return steammock::slot<bool>(kCall_332, hRequest, unMilliseconds);
    }
    virtual bool GetHTTPRequestWasTimedOut(std::uint32_t hRequest, bool* pbWasTimedOut) {
        return steammock::slot<bool>(kCall_333, hRequest, pbWasTimedOut);
    }
};

Version_STEAMHTTP_INTERFACE_VERSION003 g_STEAMHTTP_INTERFACE_VERSION003;

// ISteamInput SteamInput001
class Version_SteamInput001 {
public:
    virtual bool Init() { return steammock::slot<bool>(kCall_334); }
    virtual bool Shutdown() { return steammock::slot<bool>(kCall_335); }
    virtual void RunFrame() { steammock::slot<void>(kCall_336); }
    virtual std::int32_t GetConnectedControllers(std::uint64_t* handlesOut) {
        return steammock::slot<std::int32_t>(kCall_337, handlesOut);
    }
    virtual std::uint64_t GetActionSetHandle(const char* pszActionSetName) {
        return steammock::slot<std::uint64_t>(kCall_338, pszActionSetName);
    }
    virtual void ActivateActionSet(std::uint64_t inputHandle, std::uint64_t actionSetHandle) {
        steammock::slot<void>(kCall_339, inputHandle, actionSetHandle);
    }
    virtual std::uint64_t GetCurrentActionSet(std::uint64_t inputHandle) {
        return steammock::slot<std::uint64_t>(kCall_340, inputHandle);
    }
    virtual void ActivateActionSetLayer(std::uint64_t inputHandle, std::uint64_t actionSetLayerHandle) {
        steammock::slot<void>(kCall_341, inputHandle, actionSetLayerHandle);
    }
    virtual void DeactivateActionSetLayer(std::uint64_t inputHandle, std::uint64_t actionSetLayerHandle) {
        steammock::slot<void>(kCall_342, inputHandle, actionSetLayerHandle);
    }
    virtual void DeactivateAllActionSetLayers(std::uint64_t inputHandle) {
        steammock::slot<void>(kCall_343, inputHandle);
    }
    virtual std::int32_t GetActiveActionSetLayers(std::uint64_t inputHandle, std::uint64_t* handlesOut) {
        return steammock::slot<std::int32_t>(kCall_344, inputHandle, handlesOut);
    }
    virtual std::uint64_t GetDigitalActionHandle(const char* pszActionName) {
        return steammock::slot<std::uint64_t>(kCall_345, pszActionName);
    }
    virtual InputDigitalActionData_t GetDigitalActionData(std::uint64_t inputHandle, std::uint64_t digitalActionHandle) {
        return steammock::slot<InputDigitalActionData_t>(kCall_346, inputHandle, digitalActionHandle);
    }
    virtual std::int32_t GetDigitalActionOrigins(std::uint64_t inputHandle, std::uint64_t actionSetHandle, std::uint64_t digitalActionHandle, std::int32_t* originsOut) {
        return steammock::slot<std::int32_t>(kCall_347, inputHandle, actionSetHandle, digitalActionHandle, originsOut);
    }
    virtual std::uint64_t GetAnalogActionHandle(const char* pszActionName) {
        return steammock::slot<std::uint64_t>(kCall_348, pszActionName);
    }
    virtual InputAnalogActionData_t GetAnalogActionData(std::uint64_t inputHandle, std::uint64_t analogActionHandle) {
        return steammock::slot<InputAnalogActionData_t>(kCall_349, inputHandle, analogActionHandle);
    }
    virtual std::int32_t GetAnalogActionOrigins(std::uint64_t inputHandle, std::uint64_t actionSetHandle, std::uint64_t analogActionHandle, std::int32_t* originsOut) {
        return steammock::slot<std::int32_t>(kCall_350, inputHandle, actionSetHandle, analogActionHandle, originsOut);
    }
    virtual const char* GetGlyphForActionOrigin(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_351, eOrigin);
    }
    virtual const char* GetStringForActionOrigin(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_352, eOrigin);
    }
    virtual void StopAnalogActionMomentum(std::uint64_t inputHandle, std::uint64_t eAction) {
        steammock::slot<void>(kCall_353, inputHandle, eAction);
    }
    virtual InputMotionData_t GetMotionData(std::uint64_t inputHandle) {
        return steammock::slot<InputMotionData_t>(kCall_354, inputHandle);
    }
    virtual void TriggerVibration(std::uint64_t inputHandle, std::uint16_t usLeftSpeed, std::uint16_t usRightSpeed) {
        steammock::slot<void>(kCall_355, inputHandle, usLeftSpeed, usRightSpeed);
    }
    virtual void SetLEDColor(std::uint64_t inputHandle, std::uint8_t nColorR, std::uint8_t nColorG, std::uint8_t nColorB, std::uint32_t nFlags) {
        steammock::slot<void>(kCall_356, inputHandle, nColorR, nColorG, nColorB, nFlags);
    }
    virtual void TriggerHapticPulse(std::uint64_t inputHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec) {
        steammock::slot<void>(kCall_357, inputHandle, eTargetPad, usDurationMicroSec);
    }
    virtual void TriggerRepeatedHapticPulse(std::uint64_t inputHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec, std::uint16_t usOffMicroSec, std::uint16_t unRepeat, std::uint32_t nFlags) {
        steammock::slot<void>(kCall_358, inputHandle, eTargetPad, usDurationMicroSec, usOffMicroSec, unRepeat, nFlags);
    }
    virtual bool ShowBindingPanel(std::uint64_t inputHandle) {
        return steammock::slot<bool>(kCall_359, inputHandle);
    }
    virtual std::int32_t GetInputTypeForHandle(std::uint64_t inputHandle) {
        return steammock::slot<std::int32_t>(kCall_360, inputHandle);
    }
    virtual std::uint64_t GetControllerForGamepadIndex(std::int32_t nIndex) {
        return steammock::slot<std::uint64_t>(kCall_361, nIndex);
    }
    virtual std::int32_t GetGamepadIndexForController(std::uint64_t ulinputHandle) {
        return steammock::slot<std::int32_t>(kCall_362, ulinputHandle);
    }
    virtual const char* GetStringForXboxOrigin(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_363, eOrigin);
    }
    virtual const char* GetGlyphForXboxOrigin(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_364, eOrigin);
    }
    virtual std::int32_t GetActionOriginFromXboxOrigin(std::uint64_t inputHandle, std::int32_t eOrigin) {
        return steammock::slot<std::int32_t>(kCall_365, inputHandle, eOrigin);
    }
    virtual std::int32_t TranslateActionOrigin(std::int32_t eDestinationInputType, std::int32_t eSourceOrigin) {
        return steammock::slot<std::int32_t>(kCall_366, eDestinationInputType, eSourceOrigin);
    }
    virtual bool GetDeviceBindingRevision(std::uint64_t inputHandle, std::int32_t* pMajor, std::int32_t* pMinor) {
        return steammock::slot<bool>(kCall_367, inputHandle, pMajor, pMinor);
    }
    virtual std::uint32_t GetRemotePlaySessionID(std::uint64_t inputHandle) {
        return steammock::slot<std::uint32_t>(kCall_368, inputHandle);
    }
};

Version_SteamInput001 g_SteamInput001;

// ISteamInput SteamInput002
class Version_SteamInput002 {
public:
    virtual bool Init() { return steammock::slot<bool>(kCall_334); }
    virtual bool Shutdown() { return steammock::slot<bool>(kCall_335); }
    virtual void RunFrame() { steammock::slot<void>(kCall_336); }
    virtual std::int32_t GetConnectedControllers(std::uint64_t* handlesOut) {
        return steammock::slot<std::int32_t>(kCall_337, handlesOut);
    }
    virtual std::uint64_t GetActionSetHandle(const char* pszActionSetName) {
        return steammock::slot<std::uint64_t>(kCall_338, pszActionSetName);
    }
    virtual void ActivateActionSet(std::uint64_t inputHandle, std::uint64_t actionSetHandle) {
        steammock::slot<void>(kCall_339, inputHandle, actionSetHandle);
    }
    virtual std::uint64_t GetCurrentActionSet(std::uint64_t inputHandle) {
        return steammock::slot<std::uint64_t>(kCall_340, inputHandle);
    }
    virtual void ActivateActionSetLayer(std::uint64_t inputHandle, std::uint64_t actionSetLayerHandle) {
        steammock::slot<void>(kCall_341, inputHandle, actionSetLayerHandle);
    }
    virtual void DeactivateActionSetLayer(std::uint64_t inputHandle, std::uint64_t actionSetLayerHandle) {
        steammock::slot<void>(kCall_342, inputHandle, actionSetLayerHandle);
    }
    virtual void DeactivateAllActionSetLayers(std::uint64_t inputHandle) {
        steammock::slot<void>(kCall_343, inputHandle);
    }
    virtual std::int32_t GetActiveActionSetLayers(std::uint64_t inputHandle, std::uint64_t* handlesOut) {
        return steammock::slot<std::int32_t>(kCall_344, inputHandle, handlesOut);
    }
    virtual std::uint64_t GetDigitalActionHandle(const char* pszActionName) {
        return steammock::slot<std::uint64_t>(kCall_345, pszActionName);
    }
    virtual InputDigitalActionData_t GetDigitalActionData(std::uint64_t inputHandle, std::uint64_t digitalActionHandle) {
        return steammock::slot<InputDigitalActionData_t>(kCall_346, inputHandle, digitalActionHandle);
    }
    virtual std::int32_t GetDigitalActionOrigins(std::uint64_t inputHandle, std::uint64_t actionSetHandle, std::uint64_t digitalActionHandle, std::int32_t* originsOut) {
        return steammock::slot<std::int32_t>(kCall_347, inputHandle, actionSetHandle, digitalActionHandle, originsOut);
    }
    virtual std::uint64_t GetAnalogActionHandle(const char* pszActionName) {
        return steammock::slot<std::uint64_t>(kCall_348, pszActionName);
    }
    virtual InputAnalogActionData_t GetAnalogActionData(std::uint64_t inputHandle, std::uint64_t analogActionHandle) {
        return steammock::slot<InputAnalogActionData_t>(kCall_349, inputHandle, analogActionHandle);
    }
    virtual std::int32_t GetAnalogActionOrigins(std::uint64_t inputHandle, std::uint64_t actionSetHandle, std::uint64_t analogActionHandle, std::int32_t* originsOut) {
        return steammock::slot<std::int32_t>(kCall_350, inputHandle, actionSetHandle, analogActionHandle, originsOut);
    }
    virtual const char* GetGlyphForActionOrigin(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_351, eOrigin);
    }
    virtual const char* GetStringForActionOrigin(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_352, eOrigin);
    }
    virtual void StopAnalogActionMomentum(std::uint64_t inputHandle, std::uint64_t eAction) {
        steammock::slot<void>(kCall_353, inputHandle, eAction);
    }
    virtual InputMotionData_t GetMotionData(std::uint64_t inputHandle) {
        return steammock::slot<InputMotionData_t>(kCall_354, inputHandle);
    }
    virtual void TriggerVibration(std::uint64_t inputHandle, std::uint16_t usLeftSpeed, std::uint16_t usRightSpeed) {
        steammock::slot<void>(kCall_355, inputHandle, usLeftSpeed, usRightSpeed);
    }
    virtual void SetLEDColor(std::uint64_t inputHandle, std::uint8_t nColorR, std::uint8_t nColorG, std::uint8_t nColorB, std::uint32_t nFlags) {
        steammock::slot<void>(kCall_356, inputHandle, nColorR, nColorG, nColorB, nFlags);
    }
    virtual void TriggerHapticPulse(std::uint64_t inputHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec) {
        steammock::slot<void>(kCall_357, inputHandle, eTargetPad, usDurationMicroSec);
    }
    virtual void TriggerRepeatedHapticPulse(std::uint64_t inputHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec, std::uint16_t usOffMicroSec, std::uint16_t unRepeat, std::uint32_t nFlags) {
        steammock::slot<void>(kCall_358, inputHandle, eTargetPad, usDurationMicroSec, usOffMicroSec, unRepeat, nFlags);
    }
    virtual bool ShowBindingPanel(std::uint64_t inputHandle) {
        return steammock::slot<bool>(kCall_359, inputHandle);
    }
    virtual std::int32_t GetInputTypeForHandle(std::uint64_t inputHandle) {
        return steammock::slot<std::int32_t>(kCall_360, inputHandle);
    }
    virtual std::uint64_t GetControllerForGamepadIndex(std::int32_t nIndex) {
        return steammock::slot<std::uint64_t>(kCall_361, nIndex);
    }
    virtual std::int32_t GetGamepadIndexForController(std::uint64_t ulinputHandle) {
        return steammock::slot<std::int32_t>(kCall_362, ulinputHandle);
    }
    virtual const char* GetStringForXboxOrigin(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_363, eOrigin);
    }
    virtual const char* GetGlyphForXboxOrigin(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_364, eOrigin);
    }
    virtual std::int32_t GetActionOriginFromXboxOrigin(std::uint64_t inputHandle, std::int32_t eOrigin) {
        return steammock::slot<std::int32_t>(kCall_365, inputHandle, eOrigin);
    }
    virtual std::int32_t TranslateActionOrigin(std::int32_t eDestinationInputType, std::int32_t eSourceOrigin) {
        return steammock::slot<std::int32_t>(kCall_366, eDestinationInputType, eSourceOrigin);
    }
    virtual bool GetDeviceBindingRevision(std::uint64_t inputHandle, std::int32_t* pMajor, std::int32_t* pMinor) {
        return steammock::slot<bool>(kCall_367, inputHandle, pMajor, pMinor);
    }
    virtual std::uint32_t GetRemotePlaySessionID(std::uint64_t inputHandle) {
        return steammock::slot<std::uint32_t>(kCall_368, inputHandle);
    }
};

Version_SteamInput002 g_SteamInput002;

// ISteamInput SteamInput006
class Version_SteamInput006 {
public:
    virtual bool Init(bool bExplicitlyCallRunFrame) {
        return steammock::slot<bool>(kCall_369, bExplicitlyCallRunFrame);
    }
    virtual bool Shutdown() { return steammock::slot<bool>(kCall_335); }
    virtual bool SetInputActionManifestFilePath(const char* pchInputActionManifestAbsolutePath) {
        return steammock::slot<bool>(kCall_370, pchInputActionManifestAbsolutePath);
    }
    virtual void RunFrame(bool bReservedValue) { steammock::slot<void>(kCall_371, bReservedValue); }
    virtual bool BWaitForData(bool bWaitForever, std::uint32_t unTimeout) {
        return steammock::slot<bool>(kCall_372, bWaitForever, unTimeout);
    }
    virtual bool BNewDataAvailable() { return steammock::slot<bool>(kCall_373); }
    virtual std::int32_t GetConnectedControllers(std::uint64_t* handlesOut) {
        return steammock::slot<std::int32_t>(kCall_337, handlesOut);
    }
    virtual void EnableDeviceCallbacks() { steammock::slot<void>(kCall_374); }
    virtual void EnableActionEventCallbacks(void* pCallback) {
        steammock::slot<void>(kCall_375, pCallback);
    }
    virtual std::uint64_t GetActionSetHandle(const char* pszActionSetName) {
        return steammock::slot<std::uint64_t>(kCall_338, pszActionSetName);
    }
    virtual void ActivateActionSet(std::uint64_t inputHandle, std::uint64_t actionSetHandle) {
        steammock::slot<void>(kCall_339, inputHandle, actionSetHandle);
    }
    virtual std::uint64_t GetCurrentActionSet(std::uint64_t inputHandle) {
        return steammock::slot<std::uint64_t>(kCall_340, inputHandle);
    }
    virtual void ActivateActionSetLayer(std::uint64_t inputHandle, std::uint64_t actionSetLayerHandle) {
        steammock::slot<void>(kCall_341, inputHandle, actionSetLayerHandle);
    }
    virtual void DeactivateActionSetLayer(std::uint64_t inputHandle, std::uint64_t actionSetLayerHandle) {
        steammock::slot<void>(kCall_342, inputHandle, actionSetLayerHandle);
    }
    virtual void DeactivateAllActionSetLayers(std::uint64_t inputHandle) {
        steammock::slot<void>(kCall_343, inputHandle);
    }
    virtual std::int32_t GetActiveActionSetLayers(std::uint64_t inputHandle, std::uint64_t* handlesOut) {
        return steammock::slot<std::int32_t>(kCall_344, inputHandle, handlesOut);
    }
    virtual std::uint64_t GetDigitalActionHandle(const char* pszActionName) {
        return steammock::slot<std::uint64_t>(kCall_345, pszActionName);
    }
    virtual InputDigitalActionData_t GetDigitalActionData(std::uint64_t inputHandle, std::uint64_t digitalActionHandle) {
        return steammock::slot<InputDigitalActionData_t>(kCall_346, inputHandle, digitalActionHandle);
    }
    virtual std::int32_t GetDigitalActionOrigins(std::uint64_t inputHandle, std::uint64_t actionSetHandle, std::uint64_t digitalActionHandle, std::int32_t* originsOut) {
        return steammock::slot<std::int32_t>(kCall_347, inputHandle, actionSetHandle, digitalActionHandle, originsOut);
    }
    virtual const char* GetStringForDigitalActionName(std::uint64_t eActionHandle) {
        return steammock::slot<const char*>(kCall_376, eActionHandle);
    }
    virtual std::uint64_t GetAnalogActionHandle(const char* pszActionName) {
        return steammock::slot<std::uint64_t>(kCall_348, pszActionName);
    }
    virtual InputAnalogActionData_t GetAnalogActionData(std::uint64_t inputHandle, std::uint64_t analogActionHandle) {
        return steammock::slot<InputAnalogActionData_t>(kCall_349, inputHandle, analogActionHandle);
    }
    virtual std::int32_t GetAnalogActionOrigins(std::uint64_t inputHandle, std::uint64_t actionSetHandle, std::uint64_t analogActionHandle, std::int32_t* originsOut) {
        return steammock::slot<std::int32_t>(kCall_350, inputHandle, actionSetHandle, analogActionHandle, originsOut);
    }
    virtual const char* GetGlyphPNGForActionOrigin(std::int32_t eOrigin, std::int32_t eSize, std::uint32_t unFlags) {
        return steammock::slot<const char*>(kCall_377, eOrigin, eSize, unFlags);
    }
    virtual const char* GetGlyphSVGForActionOrigin(std::int32_t eOrigin, std::uint32_t unFlags) {
        return steammock::slot<const char*>(kCall_378, eOrigin, unFlags);
    }
    virtual const char* GetGlyphForActionOrigin_Legacy(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_379, eOrigin);
    }
    virtual const char* GetStringForActionOrigin(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_352, eOrigin);
    }
    virtual const char* GetStringForAnalogActionName(std::uint64_t eActionHandle) {
        return steammock::slot<const char*>(kCall_380, eActionHandle);
    }
    virtual void StopAnalogActionMomentum(std::uint64_t inputHandle, std::uint64_t eAction) {
        steammock::slot<void>(kCall_353, inputHandle, eAction);
    }
    virtual InputMotionData_t GetMotionData(std::uint64_t inputHandle) {
        return steammock::slot<InputMotionData_t>(kCall_354, inputHandle);
    }
    virtual void TriggerVibration(std::uint64_t inputHandle, std::uint16_t usLeftSpeed, std::uint16_t usRightSpeed) {
        steammock::slot<void>(kCall_355, inputHandle, usLeftSpeed, usRightSpeed);
    }
    virtual void TriggerVibrationExtended(std::uint64_t inputHandle, std::uint16_t usLeftSpeed, std::uint16_t usRightSpeed, std::uint16_t usLeftTriggerSpeed, std::uint16_t usRightTriggerSpeed) {
        steammock::slot<void>(kCall_381, inputHandle, usLeftSpeed, usRightSpeed, usLeftTriggerSpeed, usRightTriggerSpeed);
    }
    virtual void TriggerSimpleHapticEvent(std::uint64_t inputHandle, std::int32_t eHapticLocation, std::uint8_t nIntensity, std::int8_t nGainDB, std::uint8_t nOtherIntensity, std::int8_t nOtherGainDB) {
        steammock::slot<void>(kCall_382, inputHandle, eHapticLocation, nIntensity, nGainDB, nOtherIntensity, nOtherGainDB);
    }
    virtual void SetLEDColor(std::uint64_t inputHandle, std::uint8_t nColorR, std::uint8_t nColorG, std::uint8_t nColorB, std::uint32_t nFlags) {
        steammock::slot<void>(kCall_356, inputHandle, nColorR, nColorG, nColorB, nFlags);
    }
    virtual void Legacy_TriggerHapticPulse(std::uint64_t inputHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec) {
        steammock::slot<void>(kCall_383, inputHandle, eTargetPad, usDurationMicroSec);
    }
    virtual void Legacy_TriggerRepeatedHapticPulse(std::uint64_t inputHandle, std::int32_t eTargetPad, std::uint16_t usDurationMicroSec, std::uint16_t usOffMicroSec, std::uint16_t unRepeat, std::uint32_t nFlags) {
        steammock::slot<void>(kCall_384, inputHandle, eTargetPad, usDurationMicroSec, usOffMicroSec, unRepeat, nFlags);
    }
    virtual bool ShowBindingPanel(std::uint64_t inputHandle) {
        return steammock::slot<bool>(kCall_359, inputHandle);
    }
    virtual std::int32_t GetInputTypeForHandle(std::uint64_t inputHandle) {
        return steammock::slot<std::int32_t>(kCall_360, inputHandle);
    }
    virtual std::uint64_t GetControllerForGamepadIndex(std::int32_t nIndex) {
        return steammock::slot<std::uint64_t>(kCall_361, nIndex);
    }
    virtual std::int32_t GetGamepadIndexForController(std::uint64_t ulinputHandle) {
        return steammock::slot<std::int32_t>(kCall_362, ulinputHandle);
    }
    virtual const char* GetStringForXboxOrigin(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_363, eOrigin);
    }
    virtual const char* GetGlyphForXboxOrigin(std::int32_t eOrigin) {
        return steammock::slot<const char*>(kCall_364, eOrigin);
    }
    virtual std::int32_t GetActionOriginFromXboxOrigin(std::uint64_t inputHandle, std::int32_t eOrigin) {
        return steammock::slot<std::int32_t>(kCall_365, inputHandle, eOrigin);
    }
    virtual std::int32_t TranslateActionOrigin(std::int32_t eDestinationInputType, std::int32_t eSourceOrigin) {
        return steammock::slot<std::int32_t>(kCall_366, eDestinationInputType, eSourceOrigin);
    }
    virtual bool GetDeviceBindingRevision(std::uint64_t inputHandle, std::int32_t* pMajor, std::int32_t* pMinor) {
        return steammock::slot<bool>(kCall_367, inputHandle, pMajor, pMinor);
    }
    virtual std::uint32_t GetRemotePlaySessionID(std::uint64_t inputHandle) {
        return steammock::slot<std::uint32_t>(kCall_368, inputHandle);
    }
    virtual std::uint16_t GetSessionInputConfigurationSettings() {
        return steammock::slot<std::uint16_t>(kCall_385);
    }
    virtual void SetDualSenseTriggerEffect(std::uint64_t inputHandle, void* pParam) {
        steammock::slot<void>(kCall_386, inputHandle, pParam);
    }
};

Version_SteamInput006 g_SteamInput006;

// ISteamInventory STEAMINVENTORY_INTERFACE_V002
class Version_STEAMINVENTORY_INTERFACE_V002 {
public:
    virtual std::int32_t GetResultStatus(std::int32_t resultHandle) {
        return steammock::slot<std::int32_t>(kCall_387, resultHandle);
    }
    virtual bool GetResultItems(std::int32_t resultHandle, void* pOutItemsArray, std::uint32_t* punOutItemsArraySize) {
        return steammock::slot<bool>(kCall_388, resultHandle, pOutItemsArray, punOutItemsArraySize);
    }
    virtual bool GetResultItemProperty(std::int32_t resultHandle, std::uint32_t unItemIndex, const char* pchPropertyName, void* pchValueBuffer, std::uint32_t* punValueBufferSizeOut) {
        return steammock::slot<bool>(kCall_389, resultHandle, unItemIndex, pchPropertyName, pchValueBuffer, punValueBufferSizeOut);
    }
    virtual std::uint32_t GetResultTimestamp(std::int32_t resultHandle) {
        return steammock::slot<std::uint32_t>(kCall_390, resultHandle);
    }
    virtual bool CheckResultSteamID(std::int32_t resultHandle, CSteamID steamIDExpected) {
        return steammock::slot<bool>(kCall_391, resultHandle, steamIDExpected);
    }
    virtual void DestroyResult(std::int32_t resultHandle) {
        steammock::slot<void>(kCall_392, resultHandle);
    }
    virtual bool GetAllItems(std::int32_t* pResultHandle) {
        return steammock::slot<bool>(kCall_393, pResultHandle);
    }
    virtual bool GetItemsByID(std::int32_t* pResultHandle, void* pInstanceIDs, std::uint32_t unCountInstanceIDs) {
        return steammock::slot<bool>(kCall_394, pResultHandle, pInstanceIDs, unCountInstanceIDs);
    }
    virtual bool SerializeResult(std::int32_t resultHandle, void* pOutBuffer, std::uint32_t* punOutBufferSize) {
        return steammock::slot<bool>(kCall_395, resultHandle, pOutBuffer, punOutBufferSize);
    }
    virtual bool DeserializeResult(std::int32_t* pOutResultHandle, void* pBuffer, std::uint32_t unBufferSize, bool bRESERVED_MUST_BE_FALSE) {
        return steammock::slot<bool>(kCall_396, pOutResultHandle, pBuffer, unBufferSize, bRESERVED_MUST_BE_FALSE);
    }
    virtual bool GenerateItems(std::int32_t* pResultHandle, void* pArrayItemDefs, void* punArrayQuantity, std::uint32_t unArrayLength) {
        return steammock::slot<bool>(kCall_397, pResultHandle, pArrayItemDefs, punArrayQuantity, unArrayLength);
    }
    virtual bool GrantPromoItems(std::int32_t* pResultHandle) {
        return steammock::slot<bool>(kCall_398, pResultHandle);
    }
    virtual bool AddPromoItem(std::int32_t* pResultHandle, std::int32_t itemDef) {
        return steammock::slot<bool>(kCall_399, pResultHandle, itemDef);
    }
    virtual bool AddPromoItems(std::int32_t* pResultHandle, void* pArrayItemDefs, std::uint32_t unArrayLength) {
        return steammock::slot<bool>(kCall_400, pResultHandle, pArrayItemDefs, unArrayLength);
    }
    virtual bool ConsumeItem(std::int32_t* pResultHandle, std::uint64_t itemConsume, std::uint32_t unQuantity) {
        return steammock::slot<bool>(kCall_401, pResultHandle, itemConsume, unQuantity);
    }
    virtual bool ExchangeItems(std::int32_t* pResultHandle, void* pArrayGenerate, void* punArrayGenerateQuantity, std::uint32_t unArrayGenerateLength, void* pArrayDestroy, void* punArrayDestroyQuantity, std::uint32_t unArrayDestroyLength) {
        return steammock::slot<bool>(kCall_402, pResultHandle, pArrayGenerate, punArrayGenerateQuantity, unArrayGenerateLength, pArrayDestroy, punArrayDestroyQuantity, unArrayDestroyLength);
    }
    virtual bool TransferItemQuantity(std::int32_t* pResultHandle, std::uint64_t itemIdSource, std::uint32_t unQuantity, std::uint64_t itemIdDest) {
        return steammock::slot<bool>(kCall_403, pResultHandle, itemIdSource, unQuantity, itemIdDest);
    }
    virtual void SendItemDropHeartbeat() { steammock::slot<void>(kCall_404); }
    virtual bool TriggerItemDrop(std::int32_t* pResultHandle, std::int32_t dropListDefinition) {
        return steammock::slot<bool>(kCall_405, pResultHandle, dropListDefinition);
    }
    virtual bool TradeItems(std::int32_t* pResultHandle, CSteamID steamIDTradePartner, void* pArrayGive, void* pArrayGiveQuantity, std::uint32_t nArrayGiveLength, void* pArrayGet, void* pArrayGetQuantity, std::uint32_t nArrayGetLength) {
        return steammock::slot<bool>(kCall_406, pResultHandle, steamIDTradePartner, pArrayGive, pArrayGiveQuantity, nArrayGiveLength, pArrayGet, pArrayGetQuantity, nArrayGetLength);
    }
    virtual bool LoadItemDefinitions() { return steammock::slot<bool>(kCall_407); }
    virtual bool GetItemDefinitionIDs(std::int32_t* pItemDefIDs, std::uint32_t* punItemDefIDsArraySize) {
        return steammock::slot<bool>(kCall_408, pItemDefIDs, punItemDefIDsArraySize);
    }
    virtual bool GetItemDefinitionProperty(std::int32_t iDefinition, const char* pchPropertyName, void* pchValueBuffer, std::uint32_t* punValueBufferSizeOut) {
        return steammock::slot<bool>(kCall_409, iDefinition, pchPropertyName, pchValueBuffer, punValueBufferSizeOut);
    }
    virtual std::uint64_t RequestEligiblePromoItemDefinitionsIDs(CSteamID steamID) {
        return steammock::slot<std::uint64_t>(kCall_410, steamID);
    }
    virtual bool GetEligiblePromoItemDefinitionIDs(CSteamID steamID, std::int32_t* pItemDefIDs, std::uint32_t* punItemDefIDsArraySize) {
        return steammock::slot<bool>(kCall_411, steamID, pItemDefIDs, punItemDefIDsArraySize);
    }
};

Version_STEAMINVENTORY_INTERFACE_V002 g_STEAMINVENTORY_INTERFACE_V002;

// ISteamInventory STEAMINVENTORY_INTERFACE_V003
class Version_STEAMINVENTORY_INTERFACE_V003 {
public:
    virtual std::int32_t GetResultStatus(std::int32_t resultHandle) {
        return steammock::slot<std::int32_t>(kCall_387, resultHandle);
    }
    virtual bool GetResultItems(std::int32_t resultHandle, void* pOutItemsArray, std::uint32_t* punOutItemsArraySize) {
        return steammock::slot<bool>(kCall_388, resultHandle, pOutItemsArray, punOutItemsArraySize);
    }
    virtual bool GetResultItemProperty(std::int32_t resultHandle, std::uint32_t unItemIndex, const char* pchPropertyName, void* pchValueBuffer, std::uint32_t* punValueBufferSizeOut) {
        return steammock::slot<bool>(kCall_389, resultHandle, unItemIndex, pchPropertyName, pchValueBuffer, punValueBufferSizeOut);
    }
    virtual std::uint32_t GetResultTimestamp(std::int32_t resultHandle) {
        return steammock::slot<std::uint32_t>(kCall_390, resultHandle);
    }
    virtual bool CheckResultSteamID(std::int32_t resultHandle, CSteamID steamIDExpected) {
        return steammock::slot<bool>(kCall_391, resultHandle, steamIDExpected);
    }
    virtual void DestroyResult(std::int32_t resultHandle) {
        steammock::slot<void>(kCall_392, resultHandle);
    }
    virtual bool GetAllItems(std::int32_t* pResultHandle) {
        return steammock::slot<bool>(kCall_393, pResultHandle);
    }
    virtual bool GetItemsByID(std::int32_t* pResultHandle, void* pInstanceIDs, std::uint32_t unCountInstanceIDs) {
        return steammock::slot<bool>(kCall_394, pResultHandle, pInstanceIDs, unCountInstanceIDs);
    }
    virtual bool SerializeResult(std::int32_t resultHandle, void* pOutBuffer, std::uint32_t* punOutBufferSize) {
        return steammock::slot<bool>(kCall_395, resultHandle, pOutBuffer, punOutBufferSize);
    }
    virtual bool DeserializeResult(std::int32_t* pOutResultHandle, void* pBuffer, std::uint32_t unBufferSize, bool bRESERVED_MUST_BE_FALSE) {
        return steammock::slot<bool>(kCall_396, pOutResultHandle, pBuffer, unBufferSize, bRESERVED_MUST_BE_FALSE);
    }
    virtual bool GenerateItems(std::int32_t* pResultHandle, void* pArrayItemDefs, void* punArrayQuantity, std::uint32_t unArrayLength) {
        return steammock::slot<bool>(kCall_397, pResultHandle, pArrayItemDefs, punArrayQuantity, unArrayLength);
    }
    virtual bool GrantPromoItems(std::int32_t* pResultHandle) {
        return steammock::slot<bool>(kCall_398, pResultHandle);
    }
    virtual bool AddPromoItem(std::int32_t* pResultHandle, std::int32_t itemDef) {
        return steammock::slot<bool>(kCall_399, pResultHandle, itemDef);
    }
    virtual bool AddPromoItems(std::int32_t* pResultHandle, void* pArrayItemDefs, std::uint32_t unArrayLength) {
        return steammock::slot<bool>(kCall_400, pResultHandle, pArrayItemDefs, unArrayLength);
    }
    virtual bool ConsumeItem(std::int32_t* pResultHandle, std::uint64_t itemConsume, std::uint32_t unQuantity) {
        return steammock::slot<bool>(kCall_401, pResultHandle, itemConsume, unQuantity);
    }
    virtual bool ExchangeItems(std::int32_t* pResultHandle, void* pArrayGenerate, void* punArrayGenerateQuantity, std::uint32_t unArrayGenerateLength, void* pArrayDestroy, void* punArrayDestroyQuantity, std::uint32_t unArrayDestroyLength) {
        return steammock::slot<bool>(kCall_402, pResultHandle, pArrayGenerate, punArrayGenerateQuantity, unArrayGenerateLength, pArrayDestroy, punArrayDestroyQuantity, unArrayDestroyLength);
    }
    virtual bool TransferItemQuantity(std::int32_t* pResultHandle, std::uint64_t itemIdSource, std::uint32_t unQuantity, std::uint64_t itemIdDest) {
        return steammock::slot<bool>(kCall_403, pResultHandle, itemIdSource, unQuantity, itemIdDest);
    }
    virtual void SendItemDropHeartbeat() { steammock::slot<void>(kCall_404); }
    virtual bool TriggerItemDrop(std::int32_t* pResultHandle, std::int32_t dropListDefinition) {
        return steammock::slot<bool>(kCall_405, pResultHandle, dropListDefinition);
    }
    virtual bool TradeItems(std::int32_t* pResultHandle, CSteamID steamIDTradePartner, void* pArrayGive, void* pArrayGiveQuantity, std::uint32_t nArrayGiveLength, void* pArrayGet, void* pArrayGetQuantity, std::uint32_t nArrayGetLength) {
        return steammock::slot<bool>(kCall_406, pResultHandle, steamIDTradePartner, pArrayGive, pArrayGiveQuantity, nArrayGiveLength, pArrayGet, pArrayGetQuantity, nArrayGetLength);
    }
    virtual bool LoadItemDefinitions() { return steammock::slot<bool>(kCall_407); }
    virtual bool GetItemDefinitionIDs(std::int32_t* pItemDefIDs, std::uint32_t* punItemDefIDsArraySize) {
        return steammock::slot<bool>(kCall_408, pItemDefIDs, punItemDefIDsArraySize);
    }
    virtual bool GetItemDefinitionProperty(std::int32_t iDefinition, const char* pchPropertyName, void* pchValueBuffer, std::uint32_t* punValueBufferSizeOut) {
        return steammock::slot<bool>(kCall_409, iDefinition, pchPropertyName, pchValueBuffer, punValueBufferSizeOut);
    }
    virtual std::uint64_t RequestEligiblePromoItemDefinitionsIDs(CSteamID steamID) {
        return steammock::slot<std::uint64_t>(kCall_410, steamID);
    }
    virtual bool GetEligiblePromoItemDefinitionIDs(CSteamID steamID, std::int32_t* pItemDefIDs, std::uint32_t* punItemDefIDsArraySize) {
        return steammock::slot<bool>(kCall_411, steamID, pItemDefIDs, punItemDefIDsArraySize);
    }
    virtual std::uint64_t StartPurchase(void* pArrayItemDefs, void* punArrayQuantity, std::uint32_t unArrayLength) {
        return steammock::slot<std::uint64_t>(kCall_412, pArrayItemDefs, punArrayQuantity, unArrayLength);
    }
    virtual std::uint64_t RequestPrices() { return steammock::slot<std::uint64_t>(kCall_413); }
    virtual std::uint32_t GetNumItemsWithPrices() {
        return steammock::slot<std::uint32_t>(kCall_414);
    }
    virtual bool GetItemsWithPrices(std::int32_t* pArrayItemDefs, std::uint64_t* pCurrentPrices, std::uint64_t* pBasePrices, std::uint32_t unArrayLength) {
        return steammock::slot<bool>(kCall_415, pArrayItemDefs, pCurrentPrices, pBasePrices, unArrayLength);
    }
    virtual bool GetItemPrice(std::int32_t iDefinition, std::uint64_t* pCurrentPrice, std::uint64_t* pBasePrice) {
        return steammock::slot<bool>(kCall_416, iDefinition, pCurrentPrice, pBasePrice);
    }
    virtual std::uint64_t StartUpdateProperties() {
        return steammock::slot<std::uint64_t>(kCall_417);
    }
    virtual bool RemoveProperty(std::uint64_t handle, std::uint64_t nItemID, const char* pchPropertyName) {
        return steammock::slot<bool>(kCall_418, handle, nItemID, pchPropertyName);
    }
    virtual bool SetProperty(std::uint64_t handle, std::uint64_t nItemID, const char* pchPropertyName, const char* pchPropertyValue) {
        return steammock::slot<bool>(kCall_419, handle, nItemID, pchPropertyName, pchPropertyValue);
    }
    virtual bool SetProperty(std::uint64_t handle, std::uint64_t nItemID, const char* pchPropertyName, bool bValue) {
        return steammock::slot<bool>(kCall_420, handle, nItemID, pchPropertyName, bValue);
    }
    virtual bool SetProperty(std::uint64_t handle, std::uint64_t nItemID, const char* pchPropertyName, std::int64_t nValue) {
        return steammock::slot<bool>(kCall_421, handle, nItemID, pchPropertyName, nValue);
    }
    virtual bool SetProperty(std::uint64_t handle, std::uint64_t nItemID, const char* pchPropertyName, float flValue) {
        return steammock::slot<bool>(kCall_422, handle, nItemID, pchPropertyName, flValue);
    }
    virtual bool SubmitUpdateProperties(std::uint64_t handle, std::int32_t* pResultHandle) {
        return steammock::slot<bool>(kCall_423, handle, pResultHandle);
    }
    virtual bool InspectItem(std::int32_t* pResultHandle, const char* pchItemToken) {
        return steammock::slot<bool>(kCall_424, pResultHandle, pchItemToken);
    }
};

Version_STEAMINVENTORY_INTERFACE_V003 g_STEAMINVENTORY_INTERFACE_V003;

// ISteamMatchmaking SteamMatchMaking009
class Version_SteamMatchMaking009 {
public:
    virtual std::int32_t GetFavoriteGameCount() { return steammock::slot<std::int32_t>(kCall_425); }
    virtual bool GetFavoriteGame(std::int32_t iGame, std::uint32_t* pnAppID, std::uint32_t* pnIP, std::uint16_t* pnConnPort, std::uint16_t* pnQueryPort, std::uint32_t* punFlags, std::uint32_t* pRTime32LastPlayedOnServer) {
        return steammock::slot<bool>(kCall_426, iGame, pnAppID, pnIP, pnConnPort, pnQueryPort, punFlags, pRTime32LastPlayedOnServer);
    }
    virtual std::int32_t AddFavoriteGame(std::uint32_t nAppID, std::uint32_t nIP, std::uint16_t nConnPort, std::uint16_t nQueryPort, std::uint32_t unFlags, std::uint32_t rTime32LastPlayedOnServer) {
        return steammock::slot<std::int32_t>(kCall_427, nAppID, nIP, nConnPort, nQueryPort, unFlags, rTime32LastPlayedOnServer);
    }
    virtual bool RemoveFavoriteGame(std::uint32_t nAppID, std::uint32_t nIP, std::uint16_t nConnPort, std::uint16_t nQueryPort, std::uint32_t unFlags) {
        return steammock::slot<bool>(kCall_428, nAppID, nIP, nConnPort, nQueryPort, unFlags);
    }
    virtual std::uint64_t RequestLobbyList() { return steammock::slot<std::uint64_t>(kCall_429); }
    virtual void AddRequestLobbyListStringFilter(const char* pchKeyToMatch, const char* pchValueToMatch, std::int32_t eComparisonType) {
        steammock::slot<void>(kCall_430, pchKeyToMatch, pchValueToMatch, eComparisonType);
    }
    virtual void AddRequestLobbyListNumericalFilter(const char* pchKeyToMatch, std::int32_t nValueToMatch, std::int32_t eComparisonType) {
        steammock::slot<void>(kCall_431, pchKeyToMatch, nValueToMatch, eComparisonType);
    }
    virtual void AddRequestLobbyListNearValueFilter(const char* pchKeyToMatch, std::int32_t nValueToBeCloseTo) {
        steammock::slot<void>(kCall_432, pchKeyToMatch, nValueToBeCloseTo);
    }
    virtual void AddRequestLobbyListFilterSlotsAvailable(std::int32_t nSlotsAvailable) {
        steammock::slot<void>(kCall_433, nSlotsAvailable);
    }
    virtual void AddRequestLobbyListDistanceFilter(std::int32_t eLobbyDistanceFilter) {
        steammock::slot<void>(kCall_434, eLobbyDistanceFilter);
    }
    virtual void AddRequestLobbyListResultCountFilter(std::int32_t cMaxResults) {
        steammock::slot<void>(kCall_435, cMaxResults);
    }
    virtual void AddRequestLobbyListCompatibleMembersFilter(CSteamID steamIDLobby) {
        steammock::slot<void>(kCall_436, steamIDLobby);
    }
    virtual CSteamID GetLobbyByIndex(std::int32_t iLobby) {
        return steammock::slot<CSteamID>(kCall_437, iLobby);
    }
    virtual std::uint64_t CreateLobby(std::int32_t eLobbyType, std::int32_t cMaxMembers) {
        return steammock::slot<std::uint64_t>(kCall_438, eLobbyType, cMaxMembers);
    }
    virtual std::uint64_t JoinLobby(CSteamID steamIDLobby) {
        return steammock::slot<std::uint64_t>(kCall_439, steamIDLobby);
    }
    virtual void LeaveLobby(CSteamID steamIDLobby) {
        steammock::slot<void>(kCall_440, steamIDLobby);
    }
    virtual bool InviteUserToLobby(CSteamID steamIDLobby, CSteamID steamIDInvitee) {
        return steammock::slot<bool>(kCall_441, steamIDLobby, steamIDInvitee);
    }
    virtual std::int32_t GetNumLobbyMembers(CSteamID steamIDLobby) {
        return steammock::slot<std::int32_t>(kCall_442, steamIDLobby);
    }
    virtual CSteamID GetLobbyMemberByIndex(CSteamID steamIDLobby, std::int32_t iMember) {
        return steammock::slot<CSteamID>(kCall_443, steamIDLobby, iMember);
    }
    virtual const char* GetLobbyData(CSteamID steamIDLobby, const char* pchKey) {
        return steammock::slot<const char*>(kCall_444, steamIDLobby, pchKey);
    }
    virtual bool SetLobbyData(CSteamID steamIDLobby, const char* pchKey, const char* pchValue) {
        return steammock::slot<bool>(kCall_445, steamIDLobby, pchKey, pchValue);
    }
    virtual std::int32_t GetLobbyDataCount(CSteamID steamIDLobby) {
        return steammock::slot<std::int32_t>(kCall_446, steamIDLobby);
    }
    virtual bool GetLobbyDataByIndex(CSteamID steamIDLobby, std::int32_t iLobbyData, void* pchKey, std::int32_t cchKeyBufferSize, void* pchValue, std::int32_t cchValueBufferSize) {
        return steammock::slot<bool>(kCall_447, steamIDLobby, iLobbyData, pchKey, cchKeyBufferSize, pchValue, cchValueBufferSize);
    }
    virtual bool DeleteLobbyData(CSteamID steamIDLobby, const char* pchKey) {
        return steammock::slot<bool>(kCall_448, steamIDLobby, pchKey);
    }
    virtual const char* GetLobbyMemberData(CSteamID steamIDLobby, CSteamID steamIDUser, const char* pchKey) {
        return steammock::slot<const char*>(kCall_449, steamIDLobby, steamIDUser, pchKey);
    }
    virtual void SetLobbyMemberData(CSteamID steamIDLobby, const char* pchKey, const char* pchValue) {
        steammock::slot<void>(kCall_450, steamIDLobby, pchKey, pchValue);
    }
    virtual bool SendLobbyChatMsg(CSteamID steamIDLobby, void* pvMsgBody, std::int32_t cubMsgBody) {
        return steammock::slot<bool>(kCall_451, steamIDLobby, pvMsgBody, cubMsgBody);
    }
    virtual std::int32_t GetLobbyChatEntry(CSteamID steamIDLobby, std::int32_t iChatID, CSteamID* pSteamIDUser, void* pvData, std::int32_t cubData, std::int32_t* peChatEntryType) {
        return steammock::slot<std::int32_t>(kCall_452, steamIDLobby, iChatID, pSteamIDUser, pvData, cubData, peChatEntryType);
    }
    virtual bool RequestLobbyData(CSteamID steamIDLobby) {
        return steammock::slot<bool>(kCall_453, steamIDLobby);
    }
    virtual void SetLobbyGameServer(CSteamID steamIDLobby, std::uint32_t unGameServerIP, std::uint16_t unGameServerPort, CSteamID steamIDGameServer) {
        steammock::slot<void>(kCall_454, steamIDLobby, unGameServerIP, unGameServerPort, steamIDGameServer);
    }
    virtual bool GetLobbyGameServer(CSteamID steamIDLobby, std::uint32_t* punGameServerIP, std::uint16_t* punGameServerPort, CSteamID* psteamIDGameServer) {
        return steammock::slot<bool>(kCall_455, steamIDLobby, punGameServerIP, punGameServerPort, psteamIDGameServer);
    }
    virtual bool SetLobbyMemberLimit(CSteamID steamIDLobby, std::int32_t cMaxMembers) {
        return steammock::slot<bool>(kCall_456, steamIDLobby, cMaxMembers);
    }
    virtual std::int32_t GetLobbyMemberLimit(CSteamID steamIDLobby) {
        return steammock::slot<std::int32_t>(kCall_457, steamIDLobby);
    }
    virtual bool SetLobbyType(CSteamID steamIDLobby, std::int32_t eLobbyType) {
        return steammock::slot<bool>(kCall_458, steamIDLobby, eLobbyType);
    }
    virtual bool SetLobbyJoinable(CSteamID steamIDLobby, bool bLobbyJoinable) {
        return steammock::slot<bool>(kCall_459, steamIDLobby, bLobbyJoinable);
    }
    virtual CSteamID GetLobbyOwner(CSteamID steamIDLobby) {
        return steammock::slot<CSteamID>(kCall_460, steamIDLobby);
    }
    virtual bool SetLobbyOwner(CSteamID steamIDLobby, CSteamID steamIDNewOwner) {
        return steammock::slot<bool>(kCall_461, steamIDLobby, steamIDNewOwner);
    }
    virtual bool SetLinkedLobby(CSteamID steamIDLobby, CSteamID steamIDLobbyDependent) {
        return steammock::slot<bool>(kCall_462, steamIDLobby, steamIDLobbyDependent);
    }
};

Version_SteamMatchMaking009 g_SteamMatchMaking009;

// ISteamMatchmakingServers SteamMatchMakingServers002
class Version_SteamMatchMakingServers002 {
public:
    virtual void* RequestInternetServerList(std::uint32_t iApp, void* ppchFilters, std::uint32_t nFilters, void* pRequestServersResponse) {
        return steammock::slot<void*>(kCall_463, iApp, ppchFilters, nFilters, pRequestServersResponse);
    }
    virtual void* RequestLANServerList(std::uint32_t iApp, void* pRequestServersResponse) {
        return steammock::slot<void*>(kCall_464, iApp, pRequestServersResponse);
    }
    virtual void* RequestFriendsServerList(std::uint32_t iApp, void* ppchFilters, std::uint32_t nFilters, void* pRequestServersResponse) {
        return steammock::slot<void*>(kCall_465, iApp, ppchFilters, nFilters, pRequestServersResponse);
    }
    virtual void* RequestFavoritesServerList(std::uint32_t iApp, void* ppchFilters, std::uint32_t nFilters, void* pRequestServersResponse) {
        return steammock::slot<void*>(kCall_466, iApp, ppchFilters, nFilters, pRequestServersResponse);
    }
    virtual void* RequestHistoryServerList(std::uint32_t iApp, void* ppchFilters, std::uint32_t nFilters, void* pRequestServersResponse) {
        return steammock::slot<void*>(kCall_467, iApp, ppchFilters, nFilters, pRequestServersResponse);
    }
    virtual void* RequestSpectatorServerList(std::uint32_t iApp, void* ppchFilters, std::uint32_t nFilters, void* pRequestServersResponse) {
        return steammock::slot<void*>(kCall_468, iApp, ppchFilters, nFilters, pRequestServersResponse);
    }
    virtual void ReleaseRequest(void* hServerListRequest) {
        steammock::slot<void>(kCall_469, hServerListRequest);
    }
    virtual void* GetServerDetails(void* hRequest, std::int32_t iServer) {
        return steammock::slot<void*>(kCall_470, hRequest, iServer);
    }
    virtual void CancelQuery(void* hRequest) { steammock::slot<void>(kCall_471, hRequest); }
    virtual void RefreshQuery(void* hRequest) { steammock::slot<void>(kCall_472, hRequest); }
    virtual bool IsRefreshing(void* hRequest) { return steammock::slot<bool>(kCall_473, hRequest); }
    virtual std::int32_t GetServerCount(void* hRequest) {
        return steammock::slot<std::int32_t>(kCall_474, hRequest);
    }
    virtual void RefreshServer(void* hRequest, std::int32_t iServer) {
        steammock::slot<void>(kCall_475, hRequest, iServer);
    }
    virtual std::int32_t PingServer(std::uint32_t unIP, std::uint16_t usPort, void* pRequestServersResponse) {
        return steammock::slot<std::int32_t>(kCall_476, unIP, usPort, pRequestServersResponse);
    }
    virtual std::int32_t PlayerDetails(std::uint32_t unIP, std::uint16_t usPort, void* pRequestServersResponse) {
        return steammock::slot<std::int32_t>(kCall_477, unIP, usPort, pRequestServersResponse);
    }
    virtual std::int32_t ServerRules(std::uint32_t unIP, std::uint16_t usPort, void* pRequestServersResponse) {
        return steammock::slot<std::int32_t>(kCall_478, unIP, usPort, pRequestServersResponse);
    }
    virtual void CancelServerQuery(std::int32_t hServerQuery) {
        steammock::slot<void>(kCall_479, hServerQuery);
    }
};

Version_SteamMatchMakingServers002 g_SteamMatchMakingServers002;

// ISteamMusic STEAMMUSIC_INTERFACE_VERSION001
class Version_STEAMMUSIC_INTERFACE_VERSION001 {
public:
    virtual bool BIsEnabled() { return steammock::slot<bool>(kCall_480); }
    virtual bool BIsPlaying() { return steammock::slot<bool>(kCall_481); }
    virtual std::int32_t GetPlaybackStatus() { return steammock::slot<std::int32_t>(kCall_482); }
    virtual void Play() { steammock::slot<void>(kCall_483); }
    virtual void Pause() { steammock::slot<void>(kCall_484); }
    virtual void PlayPrevious() { steammock::slot<void>(kCall_485); }
    virtual void PlayNext() { steammock::slot<void>(kCall_486); }
    virtual void SetVolume(float flVolume) { steammock::slot<void>(kCall_487, flVolume); }
    virtual float GetVolume() { return steammock::slot<float>(kCall_488); }
};

Version_STEAMMUSIC_INTERFACE_VERSION001 g_STEAMMUSIC_INTERFACE_VERSION001;

// ISteamMusicRemote STEAMMUSICREMOTE_INTERFACE_VERSION001
class Version_STEAMMUSICREMOTE_INTERFACE_VERSION001 {
public:
    virtual bool RegisterSteamMusicRemote(const char* pchName) {
        return steammock::slot<bool>(kCall_489, pchName);
    }
    virtual bool DeregisterSteamMusicRemote() { return steammock::slot<bool>(kCall_490); }
    virtual bool BIsCurrentMusicRemote() { return steammock::slot<bool>(kCall_491); }
    virtual bool BActivationSuccess(bool bValue) {
        return steammock::slot<bool>(kCall_492, bValue);
    }
    virtual bool SetDisplayName(const char* pchDisplayName) {
        return steammock::slot<bool>(kCall_493, pchDisplayName);
    }
    virtual bool SetPNGIcon_64x64(void* pvBuffer, std::uint32_t cbBufferLength) {
        return steammock::slot<bool>(kCall_494, pvBuffer, cbBufferLength);
    }
    virtual bool EnablePlayPrevious(bool bValue) {
        return steammock::slot<bool>(kCall_495, bValue);
    }
    virtual bool EnablePlayNext(bool bValue) { return steammock::slot<bool>(kCall_496, bValue); }
    virtual bool EnableShuffled(bool bValue) { return steammock::slot<bool>(kCall_497, bValue); }
    virtual bool EnableLooped(bool bValue) { return steammock::slot<bool>(kCall_498, bValue); }
    virtual bool EnableQueue(bool bValue) { return steammock::slot<bool>(kCall_499, bValue); }
    virtual bool EnablePlaylists(bool bValue) { return steammock::slot<bool>(kCall_500, bValue); }
    virtual bool UpdatePlaybackStatus(std::int32_t nStatus) {
        return steammock::slot<bool>(kCall_501, nStatus);
    }
    virtual bool UpdateShuffled(bool bValue) { return steammock::slot<bool>(kCall_502, bValue); }
    virtual bool UpdateLooped(bool bValue) { return steammock::slot<bool>(kCall_503, bValue); }
    virtual bool UpdateVolume(float flValue) { return steammock::slot<bool>(kCall_504, flValue); }
    virtual bool CurrentEntryWillChange() { return steammock::slot<bool>(kCall_505); }
    virtual bool CurrentEntryIsAvailable(bool bAvailable) {
        return steammock::slot<bool>(kCall_506, bAvailable);
    }
    virtual bool UpdateCurrentEntryText(const char* pchText) {
        return steammock::slot<bool>(kCall_507, pchText);
    }
    virtual bool UpdateCurrentEntryElapsedSeconds(std::int32_t nValue) {
        return steammock::slot<bool>(kCall_508, nValue);
    }
    virtual bool UpdateCurrentEntryCoverArt(void* pvBuffer, std::uint32_t cbBufferLength) {
        return steammock::slot<bool>(kCall_509, pvBuffer, cbBufferLength);
    }
    virtual bool CurrentEntryDidChange() { return steammock::slot<bool>(kCall_510); }
    virtual bool QueueWillChange() { return steammock::slot<bool>(kCall_511); }
    virtual bool ResetQueueEntries() { return steammock::slot<bool>(kCall_512); }
    virtual bool SetQueueEntry(std::int32_t nID, std::int32_t nPosition, const char* pchEntryText) {
        return steammock::slot<bool>(kCall_513, nID, nPosition, pchEntryText);
    }
    virtual bool SetCurrentQueueEntry(std::int32_t nID) {
        return steammock::slot<bool>(kCall_514, nID);
    }
    virtual bool QueueDidChange() { return steammock::slot<bool>(kCall_515); }
    virtual bool PlaylistWillChange() { return steammock::slot<bool>(kCall_516); }
    virtual bool ResetPlaylistEntries() { return steammock::slot<bool>(kCall_517); }
    virtual bool SetPlaylistEntry(std::int32_t nID, std::int32_t nPosition, const char* pchEntryText) {
        return steammock::slot<bool>(kCall_518, nID, nPosition, pchEntryText);
    }
    virtual bool SetCurrentPlaylistEntry(std::int32_t nID) {
        return steammock::slot<bool>(kCall_519, nID);
    }
    virtual bool PlaylistDidChange() { return steammock::slot<bool>(kCall_520); }
};

Version_STEAMMUSICREMOTE_INTERFACE_VERSION001 g_STEAMMUSICREMOTE_INTERFACE_VERSION001;

// ISteamNetworking SteamNetworking005
class Version_SteamNetworking005 {
public:
    virtual bool SendP2PPacket(CSteamID steamIDRemote, void* pubData, std::uint32_t cubData, std::int32_t eP2PSendType, std::int32_t nChannel) {
        return steammock::slot<bool>(kCall_521, steamIDRemote, pubData, cubData, eP2PSendType, nChannel);
    }
    virtual bool IsP2PPacketAvailable(std::uint32_t* pcubMsgSize, std::int32_t nChannel) {
        return steammock::slot<bool>(kCall_522, pcubMsgSize, nChannel);
    }
    virtual bool ReadP2PPacket(void* pubDest, std::uint32_t cubDest, std::uint32_t* pcubMsgSize, CSteamID* psteamIDRemote, std::int32_t nChannel) {
        return steammock::slot<bool>(kCall_523, pubDest, cubDest, pcubMsgSize, psteamIDRemote, nChannel);
    }
    virtual bool AcceptP2PSessionWithUser(CSteamID steamIDRemote) {
        return steammock::slot<bool>(kCall_524, steamIDRemote);
    }
    virtual bool CloseP2PSessionWithUser(CSteamID steamIDRemote) {
        return steammock::slot<bool>(kCall_525, steamIDRemote);
    }
    virtual bool CloseP2PChannelWithUser(CSteamID steamIDRemote, std::int32_t nChannel) {
        return steammock::slot<bool>(kCall_526, steamIDRemote, nChannel);
    }
    virtual bool GetP2PSessionState(CSteamID steamIDRemote, void* pConnectionState) {
        return steammock::slot<bool>(kCall_527, steamIDRemote, pConnectionState);
    }
    virtual bool AllowP2PPacketRelay(bool bAllow) {
        return steammock::slot<bool>(kCall_528, bAllow);
    }
    virtual std::uint32_t CreateListenSocket(std::int32_t nVirtualP2PPort, std::uint32_t nIP, std::uint16_t nPort, bool bAllowUseOfPacketRelay) {
        return steammock::slot<std::uint32_t>(kCall_529, nVirtualP2PPort, nIP, nPort, bAllowUseOfPacketRelay);
    }
    virtual std::uint32_t CreateP2PConnectionSocket(CSteamID steamIDTarget, std::int32_t nVirtualPort, std::int32_t nTimeoutSec, bool bAllowUseOfPacketRelay) {
        return steammock::slot<std::uint32_t>(kCall_530, steamIDTarget, nVirtualPort, nTimeoutSec, bAllowUseOfPacketRelay);
    }
    virtual std::uint32_t CreateConnectionSocket(std::uint32_t nIP, std::uint16_t nPort, std::int32_t nTimeoutSec) {
        return steammock::slot<std::uint32_t>(kCall_531, nIP, nPort, nTimeoutSec);
    }
    virtual bool DestroySocket(std::uint32_t hSocket, bool bNotifyRemoteEnd) {
        return steammock::slot<bool>(kCall_532, hSocket, bNotifyRemoteEnd);
    }
    virtual bool DestroyListenSocket(std::uint32_t hSocket, bool bNotifyRemoteEnd) {
        return steammock::slot<bool>(kCall_533, hSocket, bNotifyRemoteEnd);
    }
    virtual bool SendDataOnSocket(std::uint32_t hSocket, void* pubData, std::uint32_t cubData, bool bReliable) {
        return steammock::slot<bool>(kCall_534, hSocket, pubData, cubData, bReliable);
    }
    virtual bool IsDataAvailableOnSocket(std::uint32_t hSocket, std::uint32_t* pcubMsgSize) {
        return steammock::slot<bool>(kCall_535, hSocket, pcubMsgSize);
    }
    virtual bool RetrieveDataFromSocket(std::uint32_t hSocket, void* pubDest, std::uint32_t cubDest, std::uint32_t* pcubMsgSize) {
        return steammock::slot<bool>(kCall_536, hSocket, pubDest, cubDest, pcubMsgSize);
    }
    virtual bool IsDataAvailable(std::uint32_t hListenSocket, std::uint32_t* pcubMsgSize, std::uint32_t* phSocket) {
        return steammock::slot<bool>(kCall_537, hListenSocket, pcubMsgSize, phSocket);
    }
    virtual bool RetrieveData(std::uint32_t hListenSocket, void* pubDest, std::uint32_t cubDest, std::uint32_t* pcubMsgSize, std::uint32_t* phSocket) {
        return steammock::slot<bool>(kCall_538, hListenSocket, pubDest, cubDest, pcubMsgSize, phSocket);
    }
    virtual bool GetSocketInfo(std::uint32_t hSocket, CSteamID* pSteamIDRemote, std::int32_t* peSocketStatus, std::uint32_t* punIPRemote, std::uint16_t* punPortRemote) {
        return steammock::slot<bool>(kCall_539, hSocket, pSteamIDRemote, peSocketStatus, punIPRemote, punPortRemote);
    }
    virtual bool GetListenSocketInfo(std::uint32_t hListenSocket, std::uint32_t* pnIP, std::uint16_t* pnPort) {
        return steammock::slot<bool>(kCall_540, hListenSocket, pnIP, pnPort);
    }
    virtual std::int32_t GetSocketConnectionType(std::uint32_t hSocket) {
        return steammock::slot<std::int32_t>(kCall_541, hSocket);
    }
    virtual std::int32_t GetMaxPacketSize(std::uint32_t hSocket) {
        return steammock::slot<std::int32_t>(kCall_542, hSocket);
    }
};

Version_SteamNetworking005 g_SteamNetworking005;

// ISteamNetworking SteamNetworking006
class Version_SteamNetworking006 {
public:
    virtual bool SendP2PPacket(CSteamID steamIDRemote, void* pubData, std::uint32_t cubData, std::int32_t eP2PSendType, std::int32_t nChannel) {
        return steammock::slot<bool>(kCall_521, steamIDRemote, pubData, cubData, eP2PSendType, nChannel);
    }
    virtual bool IsP2PPacketAvailable(std::uint32_t* pcubMsgSize, std::int32_t nChannel) {
        return steammock::slot<bool>(kCall_522, pcubMsgSize, nChannel);
    }
    virtual bool ReadP2PPacket(void* pubDest, std::uint32_t cubDest, std::uint32_t* pcubMsgSize, CSteamID* psteamIDRemote, std::int32_t nChannel) {
        return steammock::slot<bool>(kCall_523, pubDest, cubDest, pcubMsgSize, psteamIDRemote, nChannel);
    }
    virtual bool AcceptP2PSessionWithUser(CSteamID steamIDRemote) {
        return steammock::slot<bool>(kCall_524, steamIDRemote);
    }
    virtual bool CloseP2PSessionWithUser(CSteamID steamIDRemote) {
        return steammock::slot<bool>(kCall_525, steamIDRemote);
    }
    virtual bool CloseP2PChannelWithUser(CSteamID steamIDRemote, std::int32_t nChannel) {
        return steammock::slot<bool>(kCall_526, steamIDRemote, nChannel);
    }
    virtual bool GetP2PSessionState(CSteamID steamIDRemote, void* pConnectionState) {
        return steammock::slot<bool>(kCall_527, steamIDRemote, pConnectionState);
    }
    virtual bool AllowP2PPacketRelay(bool bAllow) {
        return steammock::slot<bool>(kCall_528, bAllow);
    }
    virtual std::uint32_t CreateListenSocket(std::int32_t nVirtualP2PPort, SteamIPAddress_t nIP, std::uint16_t nPort, bool bAllowUseOfPacketRelay) {
        return steammock::slot<std::uint32_t>(kCall_529, nVirtualP2PPort, nIP, nPort, bAllowUseOfPacketRelay);
    }
    virtual std::uint32_t CreateP2PConnectionSocket(CSteamID steamIDTarget, std::int32_t nVirtualPort, std::int32_t nTimeoutSec, bool bAllowUseOfPacketRelay) {
        return steammock::slot<std::uint32_t>(kCall_530, steamIDTarget, nVirtualPort, nTimeoutSec, bAllowUseOfPacketRelay);
    }
    virtual std::uint32_t CreateConnectionSocket(SteamIPAddress_t nIP, std::uint16_t nPort, std::int32_t nTimeoutSec) {
        return steammock::slot<std::uint32_t>(kCall_531, nIP, nPort, nTimeoutSec);
    }
    virtual bool DestroySocket(std::uint32_t hSocket, bool bNotifyRemoteEnd) {
        return steammock::slot<bool>(kCall_532, hSocket, bNotifyRemoteEnd);
    }
    virtual bool DestroyListenSocket(std::uint32_t hSocket, bool bNotifyRemoteEnd) {
        return steammock::slot<bool>(kCall_533, hSocket, bNotifyRemoteEnd);
    }
    virtual bool SendDataOnSocket(std::uint32_t hSocket, void* pubData, std::uint32_t cubData, bool bReliable) {
        return steammock::slot<bool>(kCall_534, hSocket, pubData, cubData, bReliable);
    }
    virtual bool IsDataAvailableOnSocket(std::uint32_t hSocket, std::uint32_t* pcubMsgSize) {
        return steammock::slot<bool>(kCall_535, hSocket, pcubMsgSize);
    }
    virtual bool RetrieveDataFromSocket(std::uint32_t hSocket, void* pubDest, std::uint32_t cubDest, std::uint32_t* pcubMsgSize) {
        return steammock::slot<bool>(kCall_536, hSocket, pubDest, cubDest, pcubMsgSize);
    }
    virtual bool IsDataAvailable(std::uint32_t hListenSocket, std::uint32_t* pcubMsgSize, std::uint32_t* phSocket) {
        return steammock::slot<bool>(kCall_537, hListenSocket, pcubMsgSize, phSocket);
    }
    virtual bool RetrieveData(std::uint32_t hListenSocket, void* pubDest, std::uint32_t cubDest, std::uint32_t* pcubMsgSize, std::uint32_t* phSocket) {
        return steammock::slot<bool>(kCall_538, hListenSocket, pubDest, cubDest, pcubMsgSize, phSocket);
    }
    virtual bool GetSocketInfo(std::uint32_t hSocket, CSteamID* pSteamIDRemote, std::int32_t* peSocketStatus, void* punIPRemote, std::uint16_t* punPortRemote) {
        return steammock::slot<bool>(kCall_539, hSocket, pSteamIDRemote, peSocketStatus, punIPRemote, punPortRemote);
    }
    virtual bool GetListenSocketInfo(std::uint32_t hListenSocket, void* pnIP, std::uint16_t* pnPort) {
        return steammock::slot<bool>(kCall_540, hListenSocket, pnIP, pnPort);
    }
    virtual std::int32_t GetSocketConnectionType(std::uint32_t hSocket) {
        return steammock::slot<std::int32_t>(kCall_541, hSocket);
    }
    virtual std::int32_t GetMaxPacketSize(std::uint32_t hSocket) {
        return steammock::slot<std::int32_t>(kCall_542, hSocket);
    }
};

Version_SteamNetworking006 g_SteamNetworking006;

// ISteamNetworkingMessages SteamNetworkingMessages002
class Version_SteamNetworkingMessages002 {
public:
    virtual std::int32_t SendMessageToUser(void* identityRemote, void* pubData, std::uint32_t cubData, std::int32_t nSendFlags, std::int32_t nRemoteChannel) {
        return steammock::slot<std::int32_t>(kCall_543, identityRemote, pubData, cubData, nSendFlags, nRemoteChannel);
    }
    virtual std::int32_t ReceiveMessagesOnChannel(std::int32_t nLocalChannel, void* ppOutMessages, std::int32_t nMaxMessages) {
        return steammock::slot<std::int32_t>(kCall_544, nLocalChannel, ppOutMessages, nMaxMessages);
    }
    virtual bool AcceptSessionWithUser(void* identityRemote) {
        return steammock::slot<bool>(kCall_545, identityRemote);
    }
    virtual bool CloseSessionWithUser(void* identityRemote) {
        return steammock::slot<bool>(kCall_546, identityRemote);
    }
    virtual bool CloseChannelWithUser(void* identityRemote, std::int32_t nLocalChannel) {
        return steammock::slot<bool>(kCall_547, identityRemote, nLocalChannel);
    }
    virtual std::int32_t GetSessionConnectionInfo(void* identityRemote, void* pConnectionInfo, void* pQuickStatus) {
        return steammock::slot<std::int32_t>(kCall_548, identityRemote, pConnectionInfo, pQuickStatus);
    }
};

Version_SteamNetworkingMessages002 g_SteamNetworkingMessages002;

// ISteamNetworkingSockets SteamNetworkingSockets009
class Version_SteamNetworkingSockets009 {
public:
    virtual std::uint32_t CreateListenSocketIP(void* localAddress, std::int32_t nOptions, void* pOptions) {
        return steammock::slot<std::uint32_t>(kCall_549, localAddress, nOptions, pOptions);
    }
    virtual std::uint32_t ConnectByIPAddress(void* address, std::int32_t nOptions, void* pOptions) {
        return steammock::slot<std::uint32_t>(kCall_550, address, nOptions, pOptions);
    }
    virtual std::uint32_t CreateListenSocketP2P(std::int32_t nLocalVirtualPort, std::int32_t nOptions, void* pOptions) {
        return steammock::slot<std::uint32_t>(kCall_551, nLocalVirtualPort, nOptions, pOptions);
    }
    virtual std::uint32_t ConnectP2P(void* identityRemote, std::int32_t nRemoteVirtualPort, std::int32_t nOptions, void* pOptions) {
        return steammock::slot<std::uint32_t>(kCall_552, identityRemote, nRemoteVirtualPort, nOptions, pOptions);
    }
    virtual std::int32_t AcceptConnection(std::uint32_t hConn) {
        return steammock::slot<std::int32_t>(kCall_553, hConn);
    }
    virtual bool CloseConnection(std::uint32_t hPeer, std::int32_t nReason, const char* pszDebug, bool bEnableLinger) {
        return steammock::slot<bool>(kCall_554, hPeer, nReason, pszDebug, bEnableLinger);
    }
    virtual bool CloseListenSocket(std::uint32_t hSocket) {
        return steammock::slot<bool>(kCall_555, hSocket);
    }
    virtual bool SetConnectionUserData(std::uint32_t hPeer, std::int64_t nUserData) {
        return steammock::slot<bool>(kCall_556, hPeer, nUserData);
    }
    virtual std::int64_t GetConnectionUserData(std::uint32_t hPeer) {
        return steammock::slot<std::int64_t>(kCall_557, hPeer);
    }
    virtual void SetConnectionName(std::uint32_t hPeer, const char* pszName) {
        steammock::slot<void>(kCall_558, hPeer, pszName);
    }
    virtual bool GetConnectionName(std::uint32_t hPeer, void* pszName, std::int32_t nMaxLen) {
        return steammock::slot<bool>(kCall_559, hPeer, pszName, nMaxLen);
    }
    virtual std::int32_t SendMessageToConnection(std::uint32_t hConn, void* pData, std::uint32_t cbData, std::int32_t nSendFlags, std::int64_t* pOutMessageNumber) {
        return steammock::slot<std::int32_t>(kCall_560, hConn, pData, cbData, nSendFlags, pOutMessageNumber);
    }
    virtual void SendMessages(std::int32_t nMessages, void* pMessages, std::int64_t* pOutMessageNumberOrResult) {
        steammock::slot<void>(kCall_561, nMessages, pMessages, pOutMessageNumberOrResult);
    }
    virtual std::int32_t FlushMessagesOnConnection(std::uint32_t hConn) {
        return steammock::slot<std::int32_t>(kCall_562, hConn);
    }
    virtual std::int32_t ReceiveMessagesOnConnection(std::uint32_t hConn, void* ppOutMessages, std::int32_t nMaxMessages) {
        return steammock::slot<std::int32_t>(kCall_563, hConn, ppOutMessages, nMaxMessages);
    }
    virtual bool GetConnectionInfo(std::uint32_t hConn, void* pInfo) {
        return steammock::slot<bool>(kCall_564, hConn, pInfo);
    }
    virtual bool GetQuickConnectionStatus(std::uint32_t hConn, void* pStats) {
        return steammock::slot<bool>(kCall_565, hConn, pStats);
    }
    virtual std::int32_t GetDetailedConnectionStatus(std::uint32_t hConn, void* pszBuf, std::int32_t cbBuf) {
        return steammock::slot<std::int32_t>(kCall_566, hConn, pszBuf, cbBuf);
    }
    virtual bool GetListenSocketAddress(std::uint32_t hSocket, void* address) {
        return steammock::slot<bool>(kCall_567, hSocket, address);
    }
    virtual bool CreateSocketPair(std::uint32_t* pOutConnection1, std::uint32_t* pOutConnection2, bool bUseNetworkLoopback, void* pIdentity1, void* pIdentity2) {
        return steammock::slot<bool>(kCall_568, pOutConnection1, pOutConnection2, bUseNetworkLoopback, pIdentity1, pIdentity2);
    }
    virtual bool GetIdentity(void* pIdentity) {
        return steammock::slot<bool>(kCall_569, pIdentity);
    }
    virtual std::int32_t InitAuthentication() { return steammock::slot<std::int32_t>(kCall_570); }
    virtual std::int32_t GetAuthenticationStatus(void* pDetails) {
        return steammock::slot<std::int32_t>(kCall_571, pDetails);
    }
    virtual std::uint32_t CreatePollGroup() { return steammock::slot<std::uint32_t>(kCall_572); }
    virtual bool DestroyPollGroup(std::uint32_t hPollGroup) {
        return steammock::slot<bool>(kCall_573, hPollGroup);
    }
    virtual bool SetConnectionPollGroup(std::uint32_t hConn, std::uint32_t hPollGroup) {
        return steammock::slot<bool>(kCall_574, hConn, hPollGroup);
    }
    virtual std::int32_t ReceiveMessagesOnPollGroup(std::uint32_t hPollGroup, void* ppOutMessages, std::int32_t nMaxMessages) {
        return steammock::slot<std::int32_t>(kCall_575, hPollGroup, ppOutMessages, nMaxMessages);
    }
    virtual bool ReceivedRelayAuthTicket(void* pvTicket, std::int32_t cbTicket, void* pOutParsedTicket) {
        return steammock::slot<bool>(kCall_576, pvTicket, cbTicket, pOutParsedTicket);
    }
    virtual std::int32_t FindRelayAuthTicketForServer(void* identityGameServer, std::int32_t nRemoteVirtualPort, void* pOutParsedTicket) {
        return steammock::slot<std::int32_t>(kCall_577, identityGameServer, nRemoteVirtualPort, pOutParsedTicket);
    }
    virtual std::uint32_t ConnectToHostedDedicatedServer(void* identityTarget, std::int32_t nRemoteVirtualPort, std::int32_t nOptions, void* pOptions) {
        return steammock::slot<std::uint32_t>(kCall_578, identityTarget, nRemoteVirtualPort, nOptions, pOptions);
    }
    virtual std::uint16_t GetHostedDedicatedServerPort() {
        return steammock::slot<std::uint16_t>(kCall_579);
    }
    virtual std::uint32_t GetHostedDedicatedServerPOPID() {
        return steammock::slot<std::uint32_t>(kCall_580);
    }
    virtual std::int32_t GetHostedDedicatedServerAddress(void* pRouting) {
        return steammock::slot<std::int32_t>(kCall_581, pRouting);
    }
    virtual std::uint32_t CreateHostedDedicatedServerListenSocket(std::int32_t nLocalVirtualPort, std::int32_t nOptions, void* pOptions) {
        return steammock::slot<std::uint32_t>(kCall_582, nLocalVirtualPort, nOptions, pOptions);
    }
    virtual std::int32_t GetGameCoordinatorServerLogin(void* pLoginInfo, std::int32_t* pcbSignedBlob, void* pBlob) {
        return steammock::slot<std::int32_t>(kCall_583, pLoginInfo, pcbSignedBlob, pBlob);
    }
    virtual std::uint32_t ConnectP2PCustomSignaling(void* pSignaling, void* pPeerIdentity, std::int32_t nRemoteVirtualPort, std::int32_t nOptions, void* pOptions) {
        return steammock::slot<std::uint32_t>(kCall_584, pSignaling, pPeerIdentity, nRemoteVirtualPort, nOptions, pOptions);
    }
    virtual bool ReceivedP2PCustomSignal(void* pMsg, std::int32_t cbMsg, void* pContext) {
        return steammock::slot<bool>(kCall_585, pMsg, cbMsg, pContext);
    }
    virtual bool GetCertificateRequest(std::int32_t* pcbBlob, void* pBlob, void* errMsg) {
        return steammock::slot<bool>(kCall_586, pcbBlob, pBlob, errMsg);
    }
    virtual bool SetCertificate(void* pCertificate, std::int32_t cbCertificate, void* errMsg) {
        return steammock::slot<bool>(kCall_587, pCertificate, cbCertificate, errMsg);
    }
    virtual void RunCallbacks() { steammock::slot<void>(kCall_588); }
};

Version_SteamNetworkingSockets009 g_SteamNetworkingSockets009;

// ISteamNetworkingSockets SteamNetworkingSockets012
class Version_SteamNetworkingSockets012 {
public:
    virtual std::uint32_t CreateListenSocketIP(void* localAddress, std::int32_t nOptions, void* pOptions) {
        return steammock::slot<std::uint32_t>(kCall_549, localAddress, nOptions, pOptions);
    }
    virtual std::uint32_t ConnectByIPAddress(void* address, std::int32_t nOptions, void* pOptions) {
        return steammock::slot<std::uint32_t>(kCall_550, address, nOptions, pOptions);
    }
    virtual std::uint32_t CreateListenSocketP2P(std::int32_t nLocalVirtualPort, std::int32_t nOptions, void* pOptions) {
        return steammock::slot<std::uint32_t>(kCall_551, nLocalVirtualPort, nOptions, pOptions);
    }
    virtual std::uint32_t ConnectP2P(void* identityRemote, std::int32_t nRemoteVirtualPort, std::int32_t nOptions, void* pOptions) {
        return steammock::slot<std::uint32_t>(kCall_552, identityRemote, nRemoteVirtualPort, nOptions, pOptions);
    }
    virtual std::int32_t AcceptConnection(std::uint32_t hConn) {
        return steammock::slot<std::int32_t>(kCall_553, hConn);
    }
    virtual bool CloseConnection(std::uint32_t hPeer, std::int32_t nReason, const char* pszDebug, bool bEnableLinger) {
        return steammock::slot<bool>(kCall_554, hPeer, nReason, pszDebug, bEnableLinger);
    }
    virtual bool CloseListenSocket(std::uint32_t hSocket) {
        return steammock::slot<bool>(kCall_555, hSocket);
    }
    virtual bool SetConnectionUserData(std::uint32_t hPeer, std::int64_t nUserData) {
        return steammock::slot<bool>(kCall_556, hPeer, nUserData);
    }
    virtual std::int64_t GetConnectionUserData(std::uint32_t hPeer) {
        return steammock::slot<std::int64_t>(kCall_557, hPeer);
    }
    virtual void SetConnectionName(std::uint32_t hPeer, const char* pszName) {
        steammock::slot<void>(kCall_558, hPeer, pszName);
    }
    virtual bool GetConnectionName(std::uint32_t hPeer, void* pszName, std::int32_t nMaxLen) {
        return steammock::slot<bool>(kCall_559, hPeer, pszName, nMaxLen);
    }
    virtual std::int32_t SendMessageToConnection(std::uint32_t hConn, void* pData, std::uint32_t cbData, std::int32_t nSendFlags, std::int64_t* pOutMessageNumber) {
        return steammock::slot<std::int32_t>(kCall_560, hConn, pData, cbData, nSendFlags, pOutMessageNumber);
    }
    virtual void SendMessages(std::int32_t nMessages, void* pMessages, std::int64_t* pOutMessageNumberOrResult) {
        steammock::slot<void>(kCall_561, nMessages, pMessages, pOutMessageNumberOrResult);
    }
    virtual std::int32_t FlushMessagesOnConnection(std::uint32_t hConn) {
        return steammock::slot<std::int32_t>(kCall_562, hConn);
    }
    virtual std::int32_t ReceiveMessagesOnConnection(std::uint32_t hConn, void* ppOutMessages, std::int32_t nMaxMessages) {
        return steammock::slot<std::int32_t>(kCall_563, hConn, ppOutMessages, nMaxMessages);
    }
    virtual bool GetConnectionInfo(std::uint32_t hConn, void* pInfo) {
        return steammock::slot<bool>(kCall_564, hConn, pInfo);
    }
    virtual std::int32_t GetConnectionRealTimeStatus(std::uint32_t hConn, void* pStatus, std::int32_t nLanes, void* pLanes) {
        return steammock::slot<std::int32_t>(kCall_589, hConn, pStatus, nLanes, pLanes);
    }
    virtual std::int32_t GetDetailedConnectionStatus(std::uint32_t hConn, void* pszBuf, std::int32_t cbBuf) {
        return steammock::slot<std::int32_t>(kCall_566, hConn, pszBuf, cbBuf);
    }
    virtual bool GetListenSocketAddress(std::uint32_t hSocket, void* address) {
        return steammock::slot<bool>(kCall_567, hSocket, address);
    }
    virtual bool CreateSocketPair(std::uint32_t* pOutConnection1, std::uint32_t* pOutConnection2, bool bUseNetworkLoopback, void* pIdentity1, void* pIdentity2) {
        return steammock::slot<bool>(kCall_568, pOutConnection1, pOutConnection2, bUseNetworkLoopback, pIdentity1, pIdentity2);
    }
    virtual std::int32_t ConfigureConnectionLanes(std::uint32_t hConn, std::int32_t nNumLanes, void* pLanePriorities, void* pLaneWeights) {
        return steammock::slot<std::int32_t>(kCall_590, hConn, nNumLanes, pLanePriorities, pLaneWeights);
    }
    virtual bool GetIdentity(void* pIdentity) {
        return steammock::slot<bool>(kCall_569, pIdentity);
    }
    virtual std::int32_t InitAuthentication() { return steammock::slot<std::int32_t>(kCall_570); }
    virtual std::int32_t GetAuthenticationStatus(void* pDetails) {
        return steammock::slot<std::int32_t>(kCall_571, pDetails);
    }
    virtual std::uint32_t CreatePollGroup() { return steammock::slot<std::uint32_t>(kCall_572); }
    virtual bool DestroyPollGroup(std::uint32_t hPollGroup) {
        return steammock::slot<bool>(kCall_573, hPollGroup);
    }
    virtual bool SetConnectionPollGroup(std::uint32_t hConn, std::uint32_t hPollGroup) {
        return steammock::slot<bool>(kCall_574, hConn, hPollGroup);
    }
    virtual std::int32_t ReceiveMessagesOnPollGroup(std::uint32_t hPollGroup, void* ppOutMessages, std::int32_t nMaxMessages) {
        return steammock::slot<std::int32_t>(kCall_575, hPollGroup, ppOutMessages, nMaxMessages);
    }
    virtual bool ReceivedRelayAuthTicket(void* pvTicket, std::int32_t cbTicket, void* pOutParsedTicket) {
        return steammock::slot<bool>(kCall_576, pvTicket, cbTicket, pOutParsedTicket);
    }
    virtual std::int32_t FindRelayAuthTicketForServer(void* identityGameServer, std::int32_t nRemoteVirtualPort, void* pOutParsedTicket) {
        return steammock::slot<std::int32_t>(kCall_577, identityGameServer, nRemoteVirtualPort, pOutParsedTicket);
    }
    virtual std::uint32_t ConnectToHostedDedicatedServer(void* identityTarget, std::int32_t nRemoteVirtualPort, std::int32_t nOptions, void* pOptions) {
        return steammock::slot<std::uint32_t>(kCall_578, identityTarget, nRemoteVirtualPort, nOptions, pOptions);
    }
    virtual std::uint16_t GetHostedDedicatedServerPort() {
        return steammock::slot<std::uint16_t>(kCall_579);
    }
    virtual std::uint32_t GetHostedDedicatedServerPOPID() {
        return steammock::slot<std::uint32_t>(kCall_580);
    }
    virtual std::int32_t GetHostedDedicatedServerAddress(void* pRouting) {
        return steammock::slot<std::int32_t>(kCall_581, pRouting);
    }
    virtual std::uint32_t CreateHostedDedicatedServerListenSocket(std::int32_t nLocalVirtualPort, std::int32_t nOptions, void* pOptions) {
        return steammock::slot<std::uint32_t>(kCall_582, nLocalVirtualPort, nOptions, pOptions);
    }
    virtual std::int32_t GetGameCoordinatorServerLogin(void* pLoginInfo, std::int32_t* pcbSignedBlob, void* pBlob) {
        return steammock::slot<std::int32_t>(kCall_583, pLoginInfo, pcbSignedBlob, pBlob);
    }
    virtual std::uint32_t ConnectP2PCustomSignaling(void* pSignaling, void* pPeerIdentity, std::int32_t nRemoteVirtualPort, std::int32_t nOptions, void* pOptions) {
        return steammock::slot<std::uint32_t>(kCall_584, pSignaling, pPeerIdentity, nRemoteVirtualPort, nOptions, pOptions);
    }
    virtual bool ReceivedP2PCustomSignal(void* pMsg, std::int32_t cbMsg, void* pContext) {
        return steammock::slot<bool>(kCall_585, pMsg, cbMsg, pContext);
    }
    virtual bool GetCertificateRequest(std::int32_t* pcbBlob, void* pBlob, void* errMsg) {
        return steammock::slot<bool>(kCall_586, pcbBlob, pBlob, errMsg);
    }
    virtual bool SetCertificate(void* pCertificate, std::int32_t cbCertificate, void* errMsg) {
        return steammock::slot<bool>(kCall_587, pCertificate, cbCertificate, errMsg);
    }
    virtual void ResetIdentity(void* pIdentity) { steammock::slot<void>(kCall_591, pIdentity); }
    virtual void RunCallbacks() { steammock::slot<void>(kCall_588); }
    virtual bool BeginAsyncRequestFakeIP(std::int32_t nNumPorts) {
        return steammock::slot<bool>(kCall_592, nNumPorts);
    }
    virtual void GetFakeIP(std::int32_t idxFirstPort, void* pInfo) {
        steammock::slot<void>(kCall_593, idxFirstPort, pInfo);
    }
    virtual std::uint32_t CreateListenSocketP2PFakeIP(std::int32_t idxFakePort, std::int32_t nOptions, void* pOptions) {
        return steammock::slot<std::uint32_t>(kCall_594, idxFakePort, nOptions, pOptions);
    }
    virtual std::int32_t GetRemoteFakeIPForConnection(std::uint32_t hConn, void* pOutAddr) {
        return steammock::slot<std::int32_t>(kCall_595, hConn, pOutAddr);
    }
    virtual void* CreateFakeUDPPort(std::int32_t idxFakeServerPort) {
        return steammock::slot<void*>(kCall_596, idxFakeServerPort);
    }
};

Version_SteamNetworkingSockets012 g_SteamNetworkingSockets012;

// ISteamNetworkingUtils SteamNetworkingUtils003
class Version_SteamNetworkingUtils003 {
public:
    virtual void* AllocateMessage(std::int32_t cbAllocateBuffer) {
        return steammock::slot<void*>(kCall_597, cbAllocateBuffer);
    }
    virtual std::int32_t GetRelayNetworkStatus(void* pDetails) {
        return steammock::slot<std::int32_t>(kCall_598, pDetails);
    }
    virtual float GetLocalPingLocation(void* result) {
        return steammock::slot<float>(kCall_599, result);
    }
    virtual std::int32_t EstimatePingTimeBetweenTwoLocations(void* location1, void* location2) {
        return steammock::slot<std::int32_t>(kCall_600, location1, location2);
    }
    virtual std::int32_t EstimatePingTimeFromLocalHost(void* remoteLocation) {
        return steammock::slot<std::int32_t>(kCall_601, remoteLocation);
    }
    virtual void ConvertPingLocationToString(void* location, void* pszBuf, std::int32_t cchBufSize) {
        steammock::slot<void>(kCall_602, location, pszBuf, cchBufSize);
    }
    virtual bool ParsePingLocationString(const char* pszString, void* result) {
        return steammock::slot<bool>(kCall_603, pszString, result);
    }
    virtual bool CheckPingDataUpToDate(float flMaxAgeSeconds) {
        return steammock::slot<bool>(kCall_604, flMaxAgeSeconds);
    }
    virtual std::int32_t GetPingToDataCenter(std::uint32_t popID, std::uint32_t* pViaRelayPoP) {
        return steammock::slot<std::int32_t>(kCall_605, popID, pViaRelayPoP);
    }
    virtual std::int32_t GetDirectPingToPOP(std::uint32_t popID) {
        return steammock::slot<std::int32_t>(kCall_606, popID);
    }
    virtual std::int32_t GetPOPCount() { return steammock::slot<std::int32_t>(kCall_607); }
    virtual std::int32_t GetPOPList(std::uint32_t* list, std::int32_t nListSz) {
        return steammock::slot<std::int32_t>(kCall_608, list, nListSz);
    }
    virtual std::int64_t GetLocalTimestamp() { return steammock::slot<std::int64_t>(kCall_609); }
    virtual void SetDebugOutputFunction(std::int32_t eDetailLevel, void* pfnFunc) {
        steammock::slot<void>(kCall_610, eDetailLevel, pfnFunc);
    }
    virtual bool SetConfigValue(std::int32_t eValue, std::int32_t eScopeType, std::intptr_t scopeObj, std::int32_t eDataType, void* pArg) {
        return steammock::slot<bool>(kCall_611, eValue, eScopeType, scopeObj, eDataType, pArg);
    }
    virtual std::int32_t GetConfigValue(std::int32_t eValue, std::int32_t eScopeType, std::intptr_t scopeObj, std::int32_t* pOutDataType, void* pResult, std::size_t* cbResult) {
        return steammock::slot<std::int32_t>(kCall_612, eValue, eScopeType, scopeObj, pOutDataType, pResult, cbResult);
    }
    virtual bool GetConfigValueInfo(std::int32_t eValue, void* pOutName, std::int32_t* pOutDataType, std::int32_t* pOutScope, std::int32_t* pOutNextValue) {
        return steammock::slot<bool>(kCall_613, eValue, pOutName, pOutDataType, pOutScope, pOutNextValue);
    }
    virtual std::int32_t GetFirstConfigValue() { return steammock::slot<std::int32_t>(kCall_614); }
    virtual void SteamNetworkingIPAddr_ToString(void* addr, void* buf, std::size_t cbBuf, bool bWithPort) {
        steammock::slot<void>(kCall_615, addr, buf, cbBuf, bWithPort);
    }
    virtual bool SteamNetworkingIPAddr_ParseString(void* pAddr, const char* pszStr) {
        return steammock::slot<bool>(kCall_616, pAddr, pszStr);
    }
    virtual void SteamNetworkingIdentity_ToString(void* identity, void* buf, std::size_t cbBuf) {
        steammock::slot<void>(kCall_617, identity, buf, cbBuf);
    }
    virtual bool SteamNetworkingIdentity_ParseString(void* pIdentity, const char* pszStr) {
        return steammock::slot<bool>(kCall_618, pIdentity, pszStr);
    }
};

Version_SteamNetworkingUtils003 g_SteamNetworkingUtils003;

// ISteamNetworkingUtils SteamNetworkingUtils004
class Version_SteamNetworkingUtils004 {
public:
    virtual void* AllocateMessage(std::int32_t cbAllocateBuffer) {
        return steammock::slot<void*>(kCall_597, cbAllocateBuffer);
    }
    virtual std::int32_t GetRelayNetworkStatus(void* pDetails) {
        return steammock::slot<std::int32_t>(kCall_598, pDetails);
    }
    virtual float GetLocalPingLocation(void* result) {
        return steammock::slot<float>(kCall_599, result);
    }
    virtual std::int32_t EstimatePingTimeBetweenTwoLocations(void* location1, void* location2) {
        return steammock::slot<std::int32_t>(kCall_600, location1, location2);
    }
    virtual std::int32_t EstimatePingTimeFromLocalHost(void* remoteLocation) {
        return steammock::slot<std::int32_t>(kCall_601, remoteLocation);
    }
    virtual void ConvertPingLocationToString(void* location, void* pszBuf, std::int32_t cchBufSize) {
        steammock::slot<void>(kCall_602, location, pszBuf, cchBufSize);
    }
    virtual bool ParsePingLocationString(const char* pszString, void* result) {
        return steammock::slot<bool>(kCall_603, pszString, result);
    }
    virtual bool CheckPingDataUpToDate(float flMaxAgeSeconds) {
        return steammock::slot<bool>(kCall_604, flMaxAgeSeconds);
    }
    virtual std::int32_t GetPingToDataCenter(std::uint32_t popID, std::uint32_t* pViaRelayPoP) {
        return steammock::slot<std::int32_t>(kCall_605, popID, pViaRelayPoP);
    }
    virtual std::int32_t GetDirectPingToPOP(std::uint32_t popID) {
        return steammock::slot<std::int32_t>(kCall_606, popID);
    }
    virtual std::int32_t GetPOPCount() { return steammock::slot<std::int32_t>(kCall_607); }
    virtual std::int32_t GetPOPList(std::uint32_t* list, std::int32_t nListSz) {
        return steammock::slot<std::int32_t>(kCall_608, list, nListSz);
    }
    virtual std::int64_t GetLocalTimestamp() { return steammock::slot<std::int64_t>(kCall_609); }
    virtual void SetDebugOutputFunction(std::int32_t eDetailLevel, void* pfnFunc) {
        steammock::slot<void>(kCall_610, eDetailLevel, pfnFunc);
    }
    virtual std::int32_t GetIPv4FakeIPType(std::uint32_t nIPv4) {
        return steammock::slot<std::int32_t>(kCall_619, nIPv4);
    }
    virtual std::int32_t GetRealIdentityForFakeIP(void* fakeIP, void* pOutRealIdentity) {
        return steammock::slot<std::int32_t>(kCall_620, fakeIP, pOutRealIdentity);
    }
    virtual bool SetConfigValue(std::int32_t eValue, std::int32_t eScopeType, std::intptr_t scopeObj, std::int32_t eDataType, void* pArg) {
        return steammock::slot<bool>(kCall_611, eValue, eScopeType, scopeObj, eDataType, pArg);
    }
    virtual std::int32_t GetConfigValue(std::int32_t eValue, std::int32_t eScopeType, std::intptr_t scopeObj, std::int32_t* pOutDataType, void* pResult, std::size_t* cbResult) {
        return steammock::slot<std::int32_t>(kCall_612, eValue, eScopeType, scopeObj, pOutDataType, pResult, cbResult);
    }
    virtual const char* GetConfigValueInfo(std::int32_t eValue, std::int32_t* pOutDataType, std::int32_t* pOutScope) {
        return steammock::slot<const char*>(kCall_621, eValue, pOutDataType, pOutScope);
    }
    virtual std::int32_t IterateGenericEditableConfigValues(std::int32_t eCurrent, bool bEnumerateDevVars) {
        return steammock::slot<std::int32_t>(kCall_622, eCurrent, bEnumerateDevVars);
    }
    virtual void SteamNetworkingIPAddr_ToString(void* addr, void* buf, std::size_t cbBuf, bool bWithPort) {
        steammock::slot<void>(kCall_615, addr, buf, cbBuf, bWithPort);
    }
    virtual bool SteamNetworkingIPAddr_ParseString(void* pAddr, const char* pszStr) {
        return steammock::slot<bool>(kCall_616, pAddr, pszStr);
    }
    virtual std::int32_t SteamNetworkingIPAddr_GetFakeIPType(void* addr) {
        return steammock::slot<std::int32_t>(kCall_623, addr);
    }
    virtual void SteamNetworkingIdentity_ToString(void* identity, void* buf, std::size_t cbBuf) {
        steammock::slot<void>(kCall_617, identity, buf, cbBuf);
    }
    virtual bool SteamNetworkingIdentity_ParseString(void* pIdentity, const char* pszStr) {
        return steammock::slot<bool>(kCall_618, pIdentity, pszStr);
    }
};

Version_SteamNetworkingUtils004 g_SteamNetworkingUtils004;

// ISteamParentalSettings STEAMPARENTALSETTINGS_INTERFACE_VERSION001
class Version_STEAMPARENTALSETTINGS_INTERFACE_VERSION001 {
public:
    virtual bool BIsParentalLockEnabled() { return steammock::slot<bool>(kCall_624); }
    virtual bool BIsParentalLockLocked() { return steammock::slot<bool>(kCall_625); }
    virtual bool BIsAppBlocked(std::uint32_t nAppID) {
        return steammock::slot<bool>(kCall_626, nAppID);
    }
    virtual bool BIsAppInBlockList(std::uint32_t nAppID) {
        return steammock::slot<bool>(kCall_627, nAppID);
    }
    virtual bool BIsFeatureBlocked(std::int32_t eFeature) {
        return steammock::slot<bool>(kCall_628, eFeature);
    }
    virtual bool BIsFeatureInBlockList(std::int32_t eFeature) {
        return steammock::slot<bool>(kCall_629, eFeature);
    }
};

Version_STEAMPARENTALSETTINGS_INTERFACE_VERSION001 g_STEAMPARENTALSETTINGS_INTERFACE_VERSION001;

// ISteamParties SteamParties002
class Version_SteamParties002 {
public:
    virtual std::uint32_t GetNumActiveBeacons() {
        return steammock::slot<std::uint32_t>(kCall_630);
    }
    virtual std::uint64_t GetBeaconByIndex(std::uint32_t unIndex) {
        return steammock::slot<std::uint64_t>(kCall_631, unIndex);
    }
    virtual bool GetBeaconDetails(std::uint64_t ulBeaconID, CSteamID* pSteamIDBeaconOwner, void* pLocation, void* pchMetadata, std::int32_t cchMetadata) {
        return steammock::slot<bool>(kCall_632, ulBeaconID, pSteamIDBeaconOwner, pLocation, pchMetadata, cchMetadata);
    }
    virtual std::uint64_t JoinParty(std::uint64_t ulBeaconID) {
        return steammock::slot<std::uint64_t>(kCall_633, ulBeaconID);
    }
    virtual bool GetNumAvailableBeaconLocations(std::uint32_t* puNumLocations) {
        return steammock::slot<bool>(kCall_634, puNumLocations);
    }
    virtual bool GetAvailableBeaconLocations(void* pLocationList, std::uint32_t uMaxNumLocations) {
        return steammock::slot<bool>(kCall_635, pLocationList, uMaxNumLocations);
    }
    virtual std::uint64_t CreateBeacon(std::uint32_t unOpenSlots, void* pBeaconLocation, const char* pchConnectString, const char* pchMetadata) {
        return steammock::slot<std::uint64_t>(kCall_636, unOpenSlots, pBeaconLocation, pchConnectString, pchMetadata);
    }
    virtual void OnReservationCompleted(std::uint64_t ulBeacon, CSteamID steamIDUser) {
        steammock::slot<void>(kCall_637, ulBeacon, steamIDUser);
    }
    virtual void CancelReservation(std::uint64_t ulBeacon, CSteamID steamIDUser) {
        steammock::slot<void>(kCall_638, ulBeacon, steamIDUser);
    }
    virtual std::uint64_t ChangeNumOpenSlots(std::uint64_t ulBeacon, std::uint32_t unOpenSlots) {
        return steammock::slot<std::uint64_t>(kCall_639, ulBeacon, unOpenSlots);
    }
    virtual bool DestroyBeacon(std::uint64_t ulBeacon) {
        return steammock::slot<bool>(kCall_640, ulBeacon);
    }
    virtual bool GetBeaconLocationData(SteamPartyBeaconLocation_t BeaconLocation, std::int32_t eData, void* pchDataStringOut, std::int32_t cchDataStringOut) {
        return steammock::slot<bool>(kCall_641, BeaconLocation, eData, pchDataStringOut, cchDataStringOut);
    }
};

Version_SteamParties002 g_SteamParties002;

// ISteamRemotePlay STEAMREMOTEPLAY_INTERFACE_VERSION001
class Version_STEAMREMOTEPLAY_INTERFACE_VERSION001 {
public:
    virtual std::uint32_t GetSessionCount() { return steammock::slot<std::uint32_t>(kCall_642); }
    virtual std::uint32_t GetSessionID(std::int32_t iSessionIndex) {
        return steammock::slot<std::uint32_t>(kCall_643, iSessionIndex);
    }
    virtual CSteamID GetSessionSteamID(std::uint32_t unSessionID) {
        return steammock::slot<CSteamID>(kCall_644, unSessionID);
    }
    virtual const char* GetSessionClientName(std::uint32_t unSessionID) {
        return steammock::slot<const char*>(kCall_645, unSessionID);
    }
    virtual std::int32_t GetSessionClientFormFactor(std::uint32_t unSessionID) {
        return steammock::slot<std::int32_t>(kCall_646, unSessionID);
    }
    virtual bool BGetSessionClientResolution(std::uint32_t unSessionID, std::int32_t* pnResolutionX, std::int32_t* pnResolutionY) {
        return steammock::slot<bool>(kCall_647, unSessionID, pnResolutionX, pnResolutionY);
    }
    virtual bool BSendRemotePlayTogetherInvite(CSteamID steamIDFriend) {
        return steammock::slot<bool>(kCall_648, steamIDFriend);
    }
};

Version_STEAMREMOTEPLAY_INTERFACE_VERSION001 g_STEAMREMOTEPLAY_INTERFACE_VERSION001;

// ISteamRemoteStorage STEAMREMOTESTORAGE_INTERFACE_VERSION014
class Version_STEAMREMOTESTORAGE_INTERFACE_VERSION014 {
public:
    virtual bool FileWrite(const char* pchFile, void* pvData, std::int32_t cubData) {
        return steammock::slot<bool>(kCall_649, pchFile, pvData, cubData);
    }
    virtual std::int32_t FileRead(const char* pchFile, void* pvData, std::int32_t cubDataToRead) {
        return steammock::slot<std::int32_t>(kCall_650, pchFile, pvData, cubDataToRead);
    }
    virtual std::uint64_t FileWriteAsync(const char* pchFile, void* pvData, std::uint32_t cubData) {
        return steammock::slot<std::uint64_t>(kCall_651, pchFile, pvData, cubData);
    }
    virtual std::uint64_t FileReadAsync(const char* pchFile, std::uint32_t nOffset, std::uint32_t cubToRead) {
        return steammock::slot<std::uint64_t>(kCall_652, pchFile, nOffset, cubToRead);
    }
    virtual bool FileReadAsyncComplete(std::uint64_t hReadCall, void* pvBuffer, std::uint32_t cubToRead) {
        return steammock::slot<bool>(kCall_653, hReadCall, pvBuffer, cubToRead);
    }
    virtual bool FileForget(const char* pchFile) {
        return steammock::slot<bool>(kCall_654, pchFile);
    }
    virtual bool FileDelete(const char* pchFile) {
        return steammock::slot<bool>(kCall_655, pchFile);
    }
    virtual std::uint64_t FileShare(const char* pchFile) {
        return steammock::slot<std::uint64_t>(kCall_656, pchFile);
    }
    virtual bool SetSyncPlatforms(const char* pchFile, std::int32_t eRemoteStoragePlatform) {
        return steammock::slot<bool>(kCall_657, pchFile, eRemoteStoragePlatform);
    }
    virtual std::uint64_t FileWriteStreamOpen(const char* pchFile) {
        return steammock::slot<std::uint64_t>(kCall_658, pchFile);
    }
    virtual bool FileWriteStreamWriteChunk(std::uint64_t writeHandle, void* pvData, std::int32_t cubData) {
        return steammock::slot<bool>(kCall_659, writeHandle, pvData, cubData);
    }
    virtual bool FileWriteStreamClose(std::uint64_t writeHandle) {
        return steammock::slot<bool>(kCall_660, writeHandle);
    }
    virtual bool FileWriteStreamCancel(std::uint64_t writeHandle) {
        return steammock::slot<bool>(kCall_661, writeHandle);
    }
    virtual bool FileExists(const char* pchFile) {
        return steammock::slot<bool>(kCall_662, pchFile);
    }
    virtual bool FilePersisted(const char* pchFile) {
        return steammock::slot<bool>(kCall_663, pchFile);
    }
    virtual std::int32_t GetFileSize(const char* pchFile) {
        return steammock::slot<std::int32_t>(kCall_664, pchFile);
    }
    virtual std::int64_t GetFileTimestamp(const char* pchFile) {
        return steammock::slot<std::int64_t>(kCall_665, pchFile);
    }
    virtual std::int32_t GetSyncPlatforms(const char* pchFile) {
        return steammock::slot<std::int32_t>(kCall_666, pchFile);
    }
    virtual std::int32_t GetFileCount() { return steammock::slot<std::int32_t>(kCall_667); }
    virtual const char* GetFileNameAndSize(std::int32_t iFile, std::int32_t* pnFileSizeInBytes) {
        return steammock::slot<const char*>(kCall_668, iFile, pnFileSizeInBytes);
    }
    virtual bool GetQuota(std::uint64_t* pnTotalBytes, std::uint64_t* puAvailableBytes) {
        return steammock::slot<bool>(kCall_669, pnTotalBytes, puAvailableBytes);
    }
    virtual bool IsCloudEnabledForAccount() { return steammock::slot<bool>(kCall_670); }
    virtual bool IsCloudEnabledForApp() { return steammock::slot<bool>(kCall_671); }
    virtual void SetCloudEnabledForApp(bool bEnabled) {
        steammock::slot<void>(kCall_672, bEnabled);
    }
    virtual std::uint64_t UGCDownload(std::uint64_t hContent, std::uint32_t unPriority) {
        return steammock::slot<std::uint64_t>(kCall_673, hContent, unPriority);
    }
    virtual bool GetUGCDownloadProgress(std::uint64_t hContent, std::int32_t* pnBytesDownloaded, std::int32_t* pnBytesExpected) {
        return steammock::slot<bool>(kCall_674, hContent, pnBytesDownloaded, pnBytesExpected);
    }
    virtual bool GetUGCDetails(std::uint64_t hContent, std::uint32_t* pnAppID, void* ppchName, std::int32_t* pnFileSizeInBytes, CSteamID* pSteamIDOwner) {
        return steammock::slot<bool>(kCall_675, hContent, pnAppID, ppchName, pnFileSizeInBytes, pSteamIDOwner);
    }
    virtual std::int32_t UGCRead(std::uint64_t hContent, void* pvData, std::int32_t cubDataToRead, std::uint32_t cOffset, std::int32_t eAction) {
        return steammock::slot<std::int32_t>(kCall_676, hContent, pvData, cubDataToRead, cOffset, eAction);
    }
    virtual std::int32_t GetCachedUGCCount() { return steammock::slot<std::int32_t>(kCall_677); }
    virtual std::uint64_t GetCachedUGCHandle(std::int32_t iCachedContent) {
        return steammock::slot<std::uint64_t>(kCall_678, iCachedContent);
    }
    virtual std::uint64_t PublishWorkshopFile(const char* pchFile, const char* pchPreviewFile, std::uint32_t nConsumerAppId, const char* pchTitle, const char* pchDescription, std::int32_t eVisibility, void* pTags, std::int32_t eWorkshopFileType) {
        return steammock::slot<std::uint64_t>(kCall_679, pchFile, pchPreviewFile, nConsumerAppId, pchTitle, pchDescription, eVisibility, pTags, eWorkshopFileType);
    }
    virtual std::uint64_t CreatePublishedFileUpdateRequest(std::uint64_t unPublishedFileId) {
        return steammock::slot<std::uint64_t>(kCall_680, unPublishedFileId);
    }
    virtual bool UpdatePublishedFileFile(std::uint64_t updateHandle, const char* pchFile) {
        return steammock::slot<bool>(kCall_681, updateHandle, pchFile);
    }
    virtual bool UpdatePublishedFilePreviewFile(std::uint64_t updateHandle, const char* pchPreviewFile) {
        return steammock::slot<bool>(kCall_682, updateHandle, pchPreviewFile);
    }
    virtual bool UpdatePublishedFileTitle(std::uint64_t updateHandle, const char* pchTitle) {
        return steammock::slot<bool>(kCall_683, updateHandle, pchTitle);
    }
    virtual bool UpdatePublishedFileDescription(std::uint64_t updateHandle, const char* pchDescription) {
        return steammock::slot<bool>(kCall_684, updateHandle, pchDescription);
    }
    virtual bool UpdatePublishedFileVisibility(std::uint64_t updateHandle, std::int32_t eVisibility) {
        return steammock::slot<bool>(kCall_685, updateHandle, eVisibility);
    }
    virtual bool UpdatePublishedFileTags(std::uint64_t updateHandle, void* pTags) {
        return steammock::slot<bool>(kCall_686, updateHandle, pTags);
    }
    virtual std::uint64_t CommitPublishedFileUpdate(std::uint64_t updateHandle) {
        return steammock::slot<std::uint64_t>(kCall_687, updateHandle);
    }
    virtual std::uint64_t GetPublishedFileDetails(std::uint64_t unPublishedFileId, std::uint32_t unMaxSecondsOld) {
        return steammock::slot<std::uint64_t>(kCall_688, unPublishedFileId, unMaxSecondsOld);
    }
    virtual std::uint64_t DeletePublishedFile(std::uint64_t unPublishedFileId) {
        return steammock::slot<std::uint64_t>(kCall_689, unPublishedFileId);
    }
    virtual std::uint64_t EnumerateUserPublishedFiles(std::uint32_t unStartIndex) {
        return steammock::slot<std::uint64_t>(kCall_690, unStartIndex);
    }
    virtual std::uint64_t SubscribePublishedFile(std::uint64_t unPublishedFileId) {
        return steammock::slot<std::uint64_t>(kCall_691, unPublishedFileId);
    }
    virtual std::uint64_t EnumerateUserSubscribedFiles(std::uint32_t unStartIndex) {
        return steammock::slot<std::uint64_t>(kCall_692, unStartIndex);
    }
    virtual std::uint64_t UnsubscribePublishedFile(std::uint64_t unPublishedFileId) {
        return steammock::slot<std::uint64_t>(kCall_693, unPublishedFileId);
    }
    virtual bool UpdatePublishedFileSetChangeDescription(std::uint64_t updateHandle, const char* pchChangeDescription) {
        return steammock::slot<bool>(kCall_694, updateHandle, pchChangeDescription);
    }
    virtual std::uint64_t GetPublishedItemVoteDetails(std::uint64_t unPublishedFileId) {
        return steammock::slot<std::uint64_t>(kCall_695, unPublishedFileId);
    }
    virtual std::uint64_t UpdateUserPublishedItemVote(std::uint64_t unPublishedFileId, bool bVoteUp) {
        return steammock::slot<std::uint64_t>(kCall_696, unPublishedFileId, bVoteUp);
    }
    virtual std::uint64_t GetUserPublishedItemVoteDetails(std::uint64_t unPublishedFileId) {
        return steammock::slot<std::uint64_t>(kCall_697, unPublishedFileId);
    }
    virtual std::uint64_t EnumerateUserSharedWorkshopFiles(CSteamID steamId, std::uint32_t unStartIndex, void* pRequiredTags, void* pExcludedTags) {
        return steammock::slot<std::uint64_t>(kCall_698, steamId, unStartIndex, pRequiredTags, pExcludedTags);
    }
    virtual std::uint64_t PublishVideo(std::int32_t eVideoProvider, const char* pchVideoAccount, const char* pchVideoIdentifier, const char* pchPreviewFile, std::uint32_t nConsumerAppId, const char* pchTitle, const char* pchDescription, std::int32_t eVisibility, void* pTags) {
        return steammock::slot<std::uint64_t>(kCall_699, eVideoProvider, pchVideoAccount, pchVideoIdentifier, pchPreviewFile, nConsumerAppId, pchTitle, pchDescription, eVisibility, pTags);
    }
    virtual std::uint64_t SetUserPublishedFileAction(std::uint64_t unPublishedFileId, std::int32_t eAction) {
        return steammock::slot<std::uint64_t>(kCall_700, unPublishedFileId, eAction);
    }
    virtual std::uint64_t EnumeratePublishedFilesByUserAction(std::int32_t eAction, std::uint32_t unStartIndex) {
        return steammock::slot<std::uint64_t>(kCall_701, eAction, unStartIndex);
    }
    virtual std::uint64_t EnumeratePublishedWorkshopFiles(std::int32_t eEnumerationType, std::uint32_t unStartIndex, std::uint32_t unCount, std::uint32_t unDays, void* pTags, void* pUserTags) {
        return steammock::slot<std::uint64_t>(kCall_702, eEnumerationType, unStartIndex, unCount, unDays, pTags, pUserTags);
    }
    virtual std::uint64_t UGCDownloadToLocation(std::uint64_t hContent, const char* pchLocation, std::uint32_t unPriority) {
        return steammock::slot<std::uint64_t>(kCall_703, hContent, pchLocation, unPriority);
    }
};

Version_STEAMREMOTESTORAGE_INTERFACE_VERSION014 g_STEAMREMOTESTORAGE_INTERFACE_VERSION014;

// ISteamRemoteStorage STEAMREMOTESTORAGE_INTERFACE_VERSION016
class Version_STEAMREMOTESTORAGE_INTERFACE_VERSION016 {
public:
    virtual bool FileWrite(const char* pchFile, void* pvData, std::int32_t cubData) {
        return steammock::slot<bool>(kCall_649, pchFile, pvData, cubData);
    }
    virtual std::int32_t FileRead(const char* pchFile, void* pvData, std::int32_t cubDataToRead) {
        return steammock::slot<std::int32_t>(kCall_650, pchFile, pvData, cubDataToRead);
    }
    virtual std::uint64_t FileWriteAsync(const char* pchFile, void* pvData, std::uint32_t cubData) {
        return steammock::slot<std::uint64_t>(kCall_651, pchFile, pvData, cubData);
    }
    virtual std::uint64_t FileReadAsync(const char* pchFile, std::uint32_t nOffset, std::uint32_t cubToRead) {
        return steammock::slot<std::uint64_t>(kCall_652, pchFile, nOffset, cubToRead);
    }
    virtual bool FileReadAsyncComplete(std::uint64_t hReadCall, void* pvBuffer, std::uint32_t cubToRead) {
        return steammock::slot<bool>(kCall_653, hReadCall, pvBuffer, cubToRead);
    }
    virtual bool FileForget(const char* pchFile) {
        return steammock::slot<bool>(kCall_654, pchFile);
    }
    virtual bool FileDelete(const char* pchFile) {
        return steammock::slot<bool>(kCall_655, pchFile);
    }
    virtual std::uint64_t FileShare(const char* pchFile) {
        return steammock::slot<std::uint64_t>(kCall_656, pchFile);
    }
    virtual bool SetSyncPlatforms(const char* pchFile, std::int32_t eRemoteStoragePlatform) {
        return steammock::slot<bool>(kCall_657, pchFile, eRemoteStoragePlatform);
    }
    virtual std::uint64_t FileWriteStreamOpen(const char* pchFile) {
        return steammock::slot<std::uint64_t>(kCall_658, pchFile);
    }
    virtual bool FileWriteStreamWriteChunk(std::uint64_t writeHandle, void* pvData, std::int32_t cubData) {
        return steammock::slot<bool>(kCall_659, writeHandle, pvData, cubData);
    }
    virtual bool FileWriteStreamClose(std::uint64_t writeHandle) {
        return steammock::slot<bool>(kCall_660, writeHandle);
    }
    virtual bool FileWriteStreamCancel(std::uint64_t writeHandle) {
        return steammock::slot<bool>(kCall_661, writeHandle);
    }
    virtual bool FileExists(const char* pchFile) {
        return steammock::slot<bool>(kCall_662, pchFile);
    }
    virtual bool FilePersisted(const char* pchFile) {
        return steammock::slot<bool>(kCall_663, pchFile);
    }
    virtual std::int32_t GetFileSize(const char* pchFile) {
        return steammock::slot<std::int32_t>(kCall_664, pchFile);
    }
    virtual std::int64_t GetFileTimestamp(const char* pchFile) {
        return steammock::slot<std::int64_t>(kCall_665, pchFile);
    }
    virtual std::int32_t GetSyncPlatforms(const char* pchFile) {
        return steammock::slot<std::int32_t>(kCall_666, pchFile);
    }
    virtual std::int32_t GetFileCount() { return steammock::slot<std::int32_t>(kCall_667); }
    virtual const char* GetFileNameAndSize(std::int32_t iFile, std::int32_t* pnFileSizeInBytes) {
        return steammock::slot<const char*>(kCall_668, iFile, pnFileSizeInBytes);
    }
    virtual bool GetQuota(std::uint64_t* pnTotalBytes, std::uint64_t* puAvailableBytes) {
        return steammock::slot<bool>(kCall_669, pnTotalBytes, puAvailableBytes);
    }
    virtual bool IsCloudEnabledForAccount() { return steammock::slot<bool>(kCall_670); }
    virtual bool IsCloudEnabledForApp() { return steammock::slot<bool>(kCall_671); }
    virtual void SetCloudEnabledForApp(bool bEnabled) {
        steammock::slot<void>(kCall_672, bEnabled);
    }
    virtual std::uint64_t UGCDownload(std::uint64_t hContent, std::uint32_t unPriority) {
        return steammock::slot<std::uint64_t>(kCall_673, hContent, unPriority);
    }
    virtual bool GetUGCDownloadProgress(std::uint64_t hContent, std::int32_t* pnBytesDownloaded, std::int32_t* pnBytesExpected) {
        return steammock::slot<bool>(kCall_674, hContent, pnBytesDownloaded, pnBytesExpected);
    }
    virtual bool GetUGCDetails(std::uint64_t hContent, std::uint32_t* pnAppID, void* ppchName, std::int32_t* pnFileSizeInBytes, CSteamID* pSteamIDOwner) {
        return steammock::slot<bool>(kCall_675, hContent, pnAppID, ppchName, pnFileSizeInBytes, pSteamIDOwner);
    }
    virtual std::int32_t UGCRead(std::uint64_t hContent, void* pvData, std::int32_t cubDataToRead, std::uint32_t cOffset, std::int32_t eAction) {
        return steammock::slot<std::int32_t>(kCall_676, hContent, pvData, cubDataToRead, cOffset, eAction);
    }
    virtual std::int32_t GetCachedUGCCount() { return steammock::slot<std::int32_t>(kCall_677); }
    virtual std::uint64_t GetCachedUGCHandle(std::int32_t iCachedContent) {
        return steammock::slot<std::uint64_t>(kCall_678, iCachedContent);
    }
    virtual std::uint64_t PublishWorkshopFile(const char* pchFile, const char* pchPreviewFile, std::uint32_t nConsumerAppId, const char* pchTitle, const char* pchDescription, std::int32_t eVisibility, void* pTags, std::int32_t eWorkshopFileType) {
        return steammock::slot<std::uint64_t>(kCall_679, pchFile, pchPreviewFile, nConsumerAppId, pchTitle, pchDescription, eVisibility, pTags, eWorkshopFileType);
    }
    virtual std::uint64_t CreatePublishedFileUpdateRequest(std::uint64_t unPublishedFileId) {
        return steammock::slot<std::uint64_t>(kCall_680, unPublishedFileId);
    }
    virtual bool UpdatePublishedFileFile(std::uint64_t updateHandle, const char* pchFile) {
        return steammock::slot<bool>(kCall_681, updateHandle, pchFile);
    }
    virtual bool UpdatePublishedFilePreviewFile(std::uint64_t updateHandle, const char* pchPreviewFile) {
        return steammock::slot<bool>(kCall_682, updateHandle, pchPreviewFile);
    }
    virtual bool UpdatePublishedFileTitle(std::uint64_t updateHandle, const char* pchTitle) {
        return steammock::slot<bool>(kCall_683, updateHandle, pchTitle);
    }
    virtual bool UpdatePublishedFileDescription(std::uint64_t updateHandle, const char* pchDescription) {
        return steammock::slot<bool>(kCall_684, updateHandle, pchDescription);
    }
    virtual bool UpdatePublishedFileVisibility(std::uint64_t updateHandle, std::int32_t eVisibility) {
        return steammock::slot<bool>(kCall_685, updateHandle, eVisibility);
    }
    virtual bool UpdatePublishedFileTags(std::uint64_t updateHandle, void* pTags) {
        return steammock::slot<bool>(kCall_686, updateHandle, pTags);
    }
    virtual std::uint64_t CommitPublishedFileUpdate(std::uint64_t updateHandle) {
        return steammock::slot<std::uint64_t>(kCall_687, updateHandle);
    }
    virtual std::uint64_t GetPublishedFileDetails(std::uint64_t unPublishedFileId, std::uint32_t unMaxSecondsOld) {
        return steammock::slot<std::uint64_t>(kCall_688, unPublishedFileId, unMaxSecondsOld);
    }
    virtual std::uint64_t DeletePublishedFile(std::uint64_t unPublishedFileId) {
        return steammock::slot<std::uint64_t>(kCall_689, unPublishedFileId);
    }
    virtual std::uint64_t EnumerateUserPublishedFiles(std::uint32_t unStartIndex) {
        return steammock::slot<std::uint64_t>(kCall_690, unStartIndex);
    }
    virtual std::uint64_t SubscribePublishedFile(std::uint64_t unPublishedFileId) {
        return steammock::slot<std::uint64_t>(kCall_691, unPublishedFileId);
    }
    virtual std::uint64_t EnumerateUserSubscribedFiles(std::uint32_t unStartIndex) {
        return steammock::slot<std::uint64_t>(kCall_692, unStartIndex);
    }
    virtual std::uint64_t UnsubscribePublishedFile(std::uint64_t unPublishedFileId) {
        return steammock::slot<std::uint64_t>(kCall_693, unPublishedFileId);
    }
    virtual bool UpdatePublishedFileSetChangeDescription(std::uint64_t updateHandle, const char* pchChangeDescription) {
        return steammock::slot<bool>(kCall_694, updateHandle, pchChangeDescription);
    }
    virtual std::uint64_t GetPublishedItemVoteDetails(std::uint64_t unPublishedFileId) {
        return steammock::slot<std::uint64_t>(kCall_695, unPublishedFileId);
    }
    virtual std::uint64_t UpdateUserPublishedItemVote(std::uint64_t unPublishedFileId, bool bVoteUp) {
        return steammock::slot<std::uint64_t>(kCall_696, unPublishedFileId, bVoteUp);
    }
    virtual std::uint64_t GetUserPublishedItemVoteDetails(std::uint64_t unPublishedFileId) {
        return steammock::slot<std::uint64_t>(kCall_697, unPublishedFileId);
    }
    virtual std::uint64_t EnumerateUserSharedWorkshopFiles(CSteamID steamId, std::uint32_t unStartIndex, void* pRequiredTags, void* pExcludedTags) {
        return steammock::slot<std::uint64_t>(kCall_698, steamId, unStartIndex, pRequiredTags, pExcludedTags);
    }
    virtual std::uint64_t PublishVideo(std::int32_t eVideoProvider, const char* pchVideoAccount, const char* pchVideoIdentifier, const char* pchPreviewFile, std::uint32_t nConsumerAppId, const char* pchTitle, const char* pchDescription, std::int32_t eVisibility, void* pTags) {
        return steammock::slot<std::uint64_t>(kCall_699, eVideoProvider, pchVideoAccount, pchVideoIdentifier, pchPreviewFile, nConsumerAppId, pchTitle, pchDescription, eVisibility, pTags);
    }
    virtual std::uint64_t SetUserPublishedFileAction(std::uint64_t unPublishedFileId, std::int32_t eAction) {
        return steammock::slot<std::uint64_t>(kCall_700, unPublishedFileId, eAction);
    }
    virtual std::uint64_t EnumeratePublishedFilesByUserAction(std::int32_t eAction, std::uint32_t unStartIndex) {
        return steammock::slot<std::uint64_t>(kCall_701, eAction, unStartIndex);
    }
    virtual std::uint64_t EnumeratePublishedWorkshopFiles(std::int32_t eEnumerationType, std::uint32_t unStartIndex, std::uint32_t unCount, std::uint32_t unDays, void* pTags, void* pUserTags) {
        return steammock::slot<std::uint64_t>(kCall_702, eEnumerationType, unStartIndex, unCount, unDays, pTags, pUserTags);
    }
    virtual std::uint64_t UGCDownloadToLocation(std::uint64_t hContent, const char* pchLocation, std::uint32_t unPriority) {
        return steammock::slot<std::uint64_t>(kCall_703, hContent, pchLocation, unPriority);
    }
    virtual std::int32_t GetLocalFileChangeCount() {
        return steammock::slot<std::int32_t>(kCall_704);
    }
    virtual const char* GetLocalFileChange(std::int32_t iFile, std::int32_t* pEChangeType, std::int32_t* pEFilePathType) {
        return steammock::slot<const char*>(kCall_705, iFile, pEChangeType, pEFilePathType);
    }
    virtual bool BeginFileWriteBatch() { return steammock::slot<bool>(kCall_706); }
    virtual bool EndFileWriteBatch() { return steammock::slot<bool>(kCall_707); }
};

Version_STEAMREMOTESTORAGE_INTERFACE_VERSION016 g_STEAMREMOTESTORAGE_INTERFACE_VERSION016;

// ISteamScreenshots STEAMSCREENSHOTS_INTERFACE_VERSION003
class Version_STEAMSCREENSHOTS_INTERFACE_VERSION003 {
public:
    virtual std::uint32_t WriteScreenshot(void* pubRGB, std::uint32_t cubRGB, std::int32_t nWidth, std::int32_t nHeight) {
        return steammock::slot<std::uint32_t>(kCall_708, pubRGB, cubRGB, nWidth, nHeight);
    }
    virtual std::uint32_t AddScreenshotToLibrary(const char* pchFilename, const char* pchThumbnailFilename, std::int32_t nWidth, std::int32_t nHeight) {
        return steammock::slot<std::uint32_t>(kCall_709, pchFilename, pchThumbnailFilename, nWidth, nHeight);
    }
    virtual void TriggerScreenshot() { steammock::slot<void>(kCall_710); }
    virtual void HookScreenshots(bool bHook) { steammock::slot<void>(kCall_711, bHook); }
    virtual bool SetLocation(std::uint32_t hScreenshot, const char* pchLocation) {
        return steammock::slot<bool>(kCall_712, hScreenshot, pchLocation);
    }
    virtual bool TagUser(std::uint32_t hScreenshot, CSteamID steamID) {
        return steammock::slot<bool>(kCall_713, hScreenshot, steamID);
    }
    virtual bool TagPublishedFile(std::uint32_t hScreenshot, std::uint64_t unPublishedFileID) {
        return steammock::slot<bool>(kCall_714, hScreenshot, unPublishedFileID);
    }
    virtual bool IsScreenshotsHooked() { return steammock::slot<bool>(kCall_715); }
    virtual std::uint32_t AddVRScreenshotToLibrary(std::int32_t eType, const char* pchFilename, const char* pchVRFilename) {
        return steammock::slot<std::uint32_t>(kCall_716, eType, pchFilename, pchVRFilename);
    }
};

Version_STEAMSCREENSHOTS_INTERFACE_VERSION003 g_STEAMSCREENSHOTS_INTERFACE_VERSION003;

// ISteamUGC STEAMUGC_INTERFACE_VERSION010
class Version_STEAMUGC_INTERFACE_VERSION010 {
public:
    virtual std::uint64_t CreateQueryUserUGCRequest(std::uint32_t unAccountID, std::int32_t eListType, std::int32_t eMatchingUGCType, std::int32_t eSortOrder, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steammock::slot<std::uint64_t>(kCall_717, unAccountID, eListType, eMatchingUGCType, eSortOrder, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steammock::slot<std::uint64_t>(kCall_718, eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryUGCDetailsRequest(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steammock::slot<std::uint64_t>(kCall_719, pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t SendQueryUGCRequest(std::uint64_t handle) {
        return steammock::slot<std::uint64_t>(kCall_720, handle);
    }
    virtual bool GetQueryUGCResult(std::uint64_t handle, std::uint32_t index, void* pDetails) {
        return steammock::slot<bool>(kCall_721, handle, index, pDetails);
    }
    virtual bool GetQueryUGCPreviewURL(std::uint64_t handle, std::uint32_t index, void* pchURL, std::uint32_t cchURLSize) {
        return steammock::slot<bool>(kCall_722, handle, index, pchURL, cchURLSize);
    }
    virtual bool GetQueryUGCMetadata(std::uint64_t handle, std::uint32_t index, void* pchMetadata, std::uint32_t cchMetadatasize) {
        return steammock::slot<bool>(kCall_723, handle, index, pchMetadata, cchMetadatasize);
    }
    virtual bool GetQueryUGCChildren(std::uint64_t handle, std::uint32_t index, std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steammock::slot<bool>(kCall_724, handle, index, pvecPublishedFileID, cMaxEntries);
    }
    virtual bool GetQueryUGCStatistic(std::uint64_t handle, std::uint32_t index, std::int32_t eStatType, std::uint64_t* pStatValue) {
        return steammock::slot<bool>(kCall_725, handle, index, eStatType, pStatValue);
    }
    virtual std::uint32_t GetQueryUGCNumAdditionalPreviews(std::uint64_t handle, std::uint32_t index) {
        return steammock::slot<std::uint32_t>(kCall_726, handle, index);
    }
    virtual bool GetQueryUGCAdditionalPreview(std::uint64_t handle, std::uint32_t index, std::uint32_t previewIndex, void* pchURLOrVideoID, std::uint32_t cchURLSize, void* pchOriginalFileName, std::uint32_t cchOriginalFileNameSize, std::int32_t* pPreviewType) {
        return steammock::slot<bool>(kCall_727, handle, index, previewIndex, pchURLOrVideoID, cchURLSize, pchOriginalFileName, cchOriginalFileNameSize, pPreviewType);
    }
    virtual std::uint32_t GetQueryUGCNumKeyValueTags(std::uint64_t handle, std::uint32_t index) {
        return steammock::slot<std::uint32_t>(kCall_728, handle, index);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, std::uint32_t keyValueTagIndex, void* pchKey, std::uint32_t cchKeySize, void* pchValue, std::uint32_t cchValueSize) {
        return steammock::slot<bool>(kCall_729, handle, index, keyValueTagIndex, pchKey, cchKeySize, pchValue, cchValueSize);
    }
    virtual bool ReleaseQueryUGCRequest(std::uint64_t handle) {
        return steammock::slot<bool>(kCall_730, handle);
    }
    virtual bool AddRequiredTag(std::uint64_t handle, const char* pTagName) {
        return steammock::slot<bool>(kCall_731, handle, pTagName);
    }
    virtual bool AddExcludedTag(std::uint64_t handle, const char* pTagName) {
        return steammock::slot<bool>(kCall_732, handle, pTagName);
    }
    virtual bool SetReturnOnlyIDs(std::uint64_t handle, bool bReturnOnlyIDs) {
        return steammock::slot<bool>(kCall_733, handle, bReturnOnlyIDs);
    }
    virtual bool SetReturnKeyValueTags(std::uint64_t handle, bool bReturnKeyValueTags) {
        return steammock::slot<bool>(kCall_734, handle, bReturnKeyValueTags);
    }
    virtual bool SetReturnLongDescription(std::uint64_t handle, bool bReturnLongDescription) {
        return steammock::slot<bool>(kCall_735, handle, bReturnLongDescription);
    }
    virtual bool SetReturnMetadata(std::uint64_t handle, bool bReturnMetadata) {
        return steammock::slot<bool>(kCall_736, handle, bReturnMetadata);
    }
    virtual bool SetReturnChildren(std::uint64_t handle, bool bReturnChildren) {
        return steammock::slot<bool>(kCall_737, handle, bReturnChildren);
    }
    virtual bool SetReturnAdditionalPreviews(std::uint64_t handle, bool bReturnAdditionalPreviews) {
        return steammock::slot<bool>(kCall_738, handle, bReturnAdditionalPreviews);
    }
    virtual bool SetReturnTotalOnly(std::uint64_t handle, bool bReturnTotalOnly) {
        return steammock::slot<bool>(kCall_739, handle, bReturnTotalOnly);
    }
    virtual bool SetReturnPlaytimeStats(std::uint64_t handle, std::uint32_t unDays) {
        return steammock::slot<bool>(kCall_740, handle, unDays);
    }
    virtual bool SetLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steammock::slot<bool>(kCall_741, handle, pchLanguage);
    }
    virtual bool SetAllowCachedResponse(std::uint64_t handle, std::uint32_t unMaxAgeSeconds) {
        return steammock::slot<bool>(kCall_742, handle, unMaxAgeSeconds);
    }
    virtual bool SetCloudFileNameFilter(std::uint64_t handle, const char* pMatchCloudFileName) {
        return steammock::slot<bool>(kCall_743, handle, pMatchCloudFileName);
    }
    virtual bool SetMatchAnyTag(std::uint64_t handle, bool bMatchAnyTag) {
        return steammock::slot<bool>(kCall_744, handle, bMatchAnyTag);
    }
    virtual bool SetSearchText(std::uint64_t handle, const char* pSearchText) {
        return steammock::slot<bool>(kCall_745, handle, pSearchText);
    }
    virtual bool SetRankedByTrendDays(std::uint64_t handle, std::uint32_t unDays) {
        return steammock::slot<bool>(kCall_746, handle, unDays);
    }
    virtual bool AddRequiredKeyValueTag(std::uint64_t handle, const char* pKey, const char* pValue) {
        return steammock::slot<bool>(kCall_747, handle, pKey, pValue);
    }
    virtual std::uint64_t RequestUGCDetails(std::uint64_t nPublishedFileID, std::uint32_t unMaxAgeSeconds) {
        return steammock::slot<std::uint64_t>(kCall_748, nPublishedFileID, unMaxAgeSeconds);
    }
    virtual std::uint64_t CreateItem(std::uint32_t nConsumerAppId, std::int32_t eFileType) {
        return steammock::slot<std::uint64_t>(kCall_749, nConsumerAppId, eFileType);
    }
    virtual std::uint64_t StartItemUpdate(std::uint32_t nConsumerAppId, std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_750, nConsumerAppId, nPublishedFileID);
    }
    virtual bool SetItemTitle(std::uint64_t handle, const char* pchTitle) {
        return steammock::slot<bool>(kCall_751, handle, pchTitle);
    }
    virtual bool SetItemDescription(std::uint64_t handle, const char* pchDescription) {
        return steammock::slot<bool>(kCall_752, handle, pchDescription);
    }
    virtual bool SetItemUpdateLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steammock::slot<bool>(kCall_753, handle, pchLanguage);
    }
    virtual bool SetItemMetadata(std::uint64_t handle, const char* pchMetaData) {
        return steammock::slot<bool>(kCall_754, handle, pchMetaData);
    }
    virtual bool SetItemVisibility(std::uint64_t handle, std::int32_t eVisibility) {
        return steammock::slot<bool>(kCall_755, handle, eVisibility);
    }
    virtual bool SetItemTags(std::uint64_t updateHandle, void* pTags) {
        return steammock::slot<bool>(kCall_756, updateHandle, pTags);
    }
    virtual bool SetItemContent(std::uint64_t handle, const char* pszContentFolder) {
        return steammock::slot<bool>(kCall_757, handle, pszContentFolder);
    }
    virtual bool SetItemPreview(std::uint64_t handle, const char* pszPreviewFile) {
        return steammock::slot<bool>(kCall_758, handle, pszPreviewFile);
    }
    virtual bool RemoveItemKeyValueTags(std::uint64_t handle, const char* pchKey) {
        return steammock::slot<bool>(kCall_759, handle, pchKey);
    }
    virtual bool AddItemKeyValueTag(std::uint64_t handle, const char* pchKey, const char* pchValue) {
        return steammock::slot<bool>(kCall_760, handle, pchKey, pchValue);
    }
    virtual bool AddItemPreviewFile(std::uint64_t handle, const char* pszPreviewFile, std::int32_t type) {
        return steammock::slot<bool>(kCall_761, handle, pszPreviewFile, type);
    }
    virtual bool AddItemPreviewVideo(std::uint64_t handle, const char* pszVideoID) {
        return steammock::slot<bool>(kCall_762, handle, pszVideoID);
    }
    virtual bool UpdateItemPreviewFile(std::uint64_t handle, std::uint32_t index, const char* pszPreviewFile) {
        return steammock::slot<bool>(kCall_763, handle, index, pszPreviewFile);
    }
    virtual bool UpdateItemPreviewVideo(std::uint64_t handle, std::uint32_t index, const char* pszVideoID) {
        return steammock::slot<bool>(kCall_764, handle, index, pszVideoID);
    }
    virtual bool RemoveItemPreview(std::uint64_t handle, std::uint32_t index) {
        return steammock::slot<bool>(kCall_765, handle, index);
    }
    virtual std::uint64_t SubmitItemUpdate(std::uint64_t handle, const char* pchChangeNote) {
        return steammock::slot<std::uint64_t>(kCall_766, handle, pchChangeNote);
    }
    virtual std::int32_t GetItemUpdateProgress(std::uint64_t handle, std::uint64_t* punBytesProcessed, std::uint64_t* punBytesTotal) {
        return steammock::slot<std::int32_t>(kCall_767, handle, punBytesProcessed, punBytesTotal);
    }
    virtual std::uint64_t SetUserItemVote(std::uint64_t nPublishedFileID, bool bVoteUp) {
        return steammock::slot<std::uint64_t>(kCall_768, nPublishedFileID, bVoteUp);
    }
    virtual std::uint64_t GetUserItemVote(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_769, nPublishedFileID);
    }
    virtual std::uint64_t AddItemToFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_770, nAppId, nPublishedFileID);
    }
    virtual std::uint64_t RemoveItemFromFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_771, nAppId, nPublishedFileID);
    }
    virtual std::uint64_t SubscribeItem(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_772, nPublishedFileID);
    }
    virtual std::uint64_t UnsubscribeItem(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_773, nPublishedFileID);
    }
    virtual std::uint32_t GetNumSubscribedItems() {
        return steammock::slot<std::uint32_t>(kCall_774);
    }
    virtual std::uint32_t GetSubscribedItems(std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steammock::slot<std::uint32_t>(kCall_775, pvecPublishedFileID, cMaxEntries);
    }
    virtual std::uint32_t GetItemState(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint32_t>(kCall_776, nPublishedFileID);
    }
    virtual bool GetItemInstallInfo(std::uint64_t nPublishedFileID, std::uint64_t* punSizeOnDisk, void* pchFolder, std::uint32_t cchFolderSize, std::uint32_t* punTimeStamp) {
        return steammock::slot<bool>(kCall_777, nPublishedFileID, punSizeOnDisk, pchFolder, cchFolderSize, punTimeStamp);
    }
    virtual bool GetItemDownloadInfo(std::uint64_t nPublishedFileID, std::uint64_t* punBytesDownloaded, std::uint64_t* punBytesTotal) {
        return steammock::slot<bool>(kCall_778, nPublishedFileID, punBytesDownloaded, punBytesTotal);
    }
    virtual bool DownloadItem(std::uint64_t nPublishedFileID, bool bHighPriority) {
        return steammock::slot<bool>(kCall_779, nPublishedFileID, bHighPriority);
    }
    virtual bool BInitWorkshopForGameServer(std::uint32_t unWorkshopDepotID, const char* pszFolder) {
        return steammock::slot<bool>(kCall_780, unWorkshopDepotID, pszFolder);
    }
    virtual void SuspendDownloads(bool bSuspend) { steammock::slot<void>(kCall_781, bSuspend); }
    virtual std::uint64_t StartPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steammock::slot<std::uint64_t>(kCall_782, pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steammock::slot<std::uint64_t>(kCall_783, pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTrackingForAllItems() {
        return steammock::slot<std::uint64_t>(kCall_784);
    }
    virtual std::uint64_t AddDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_785, nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t RemoveDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_786, nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t AddAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steammock::slot<std::uint64_t>(kCall_787, nPublishedFileID, nAppID);
    }
    virtual std::uint64_t RemoveAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steammock::slot<std::uint64_t>(kCall_788, nPublishedFileID, nAppID);
    }
    virtual std::uint64_t GetAppDependencies(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_789, nPublishedFileID);
    }
    virtual std::uint64_t DeleteItem(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_790, nPublishedFileID);
    }
};

Version_STEAMUGC_INTERFACE_VERSION010 g_STEAMUGC_INTERFACE_VERSION010;

// ISteamUGC STEAMUGC_INTERFACE_VERSION013
class Version_STEAMUGC_INTERFACE_VERSION013 {
public:
    virtual std::uint64_t CreateQueryUserUGCRequest(std::uint32_t unAccountID, std::int32_t eListType, std::int32_t eMatchingUGCType, std::int32_t eSortOrder, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steammock::slot<std::uint64_t>(kCall_717, unAccountID, eListType, eMatchingUGCType, eSortOrder, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steammock::slot<std::uint64_t>(kCall_718, eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, const char* pchCursor) {
        return steammock::slot<std::uint64_t>(kCall_791, eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, pchCursor);
    }
    virtual std::uint64_t CreateQueryUGCDetailsRequest(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steammock::slot<std::uint64_t>(kCall_719, pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t SendQueryUGCRequest(std::uint64_t handle) {
        return steammock::slot<std::uint64_t>(kCall_720, handle);
    }
    virtual bool GetQueryUGCResult(std::uint64_t handle, std::uint32_t index, void* pDetails) {
        return steammock::slot<bool>(kCall_721, handle, index, pDetails);
    }
    virtual bool GetQueryUGCPreviewURL(std::uint64_t handle, std::uint32_t index, void* pchURL, std::uint32_t cchURLSize) {
        return steammock::slot<bool>(kCall_722, handle, index, pchURL, cchURLSize);
    }
    virtual bool GetQueryUGCMetadata(std::uint64_t handle, std::uint32_t index, void* pchMetadata, std::uint32_t cchMetadatasize) {
        return steammock::slot<bool>(kCall_723, handle, index, pchMetadata, cchMetadatasize);
    }
    virtual bool GetQueryUGCChildren(std::uint64_t handle, std::uint32_t index, std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steammock::slot<bool>(kCall_724, handle, index, pvecPublishedFileID, cMaxEntries);
    }
    virtual bool GetQueryUGCStatistic(std::uint64_t handle, std::uint32_t index, std::int32_t eStatType, std::uint64_t* pStatValue) {
        return steammock::slot<bool>(kCall_725, handle, index, eStatType, pStatValue);
    }
    virtual std::uint32_t GetQueryUGCNumAdditionalPreviews(std::uint64_t handle, std::uint32_t index) {
        return steammock::slot<std::uint32_t>(kCall_726, handle, index);
    }
    virtual bool GetQueryUGCAdditionalPreview(std::uint64_t handle, std::uint32_t index, std::uint32_t previewIndex, void* pchURLOrVideoID, std::uint32_t cchURLSize, void* pchOriginalFileName, std::uint32_t cchOriginalFileNameSize, std::int32_t* pPreviewType) {
        return steammock::slot<bool>(kCall_727, handle, index, previewIndex, pchURLOrVideoID, cchURLSize, pchOriginalFileName, cchOriginalFileNameSize, pPreviewType);
    }
    virtual std::uint32_t GetQueryUGCNumKeyValueTags(std::uint64_t handle, std::uint32_t index) {
        return steammock::slot<std::uint32_t>(kCall_728, handle, index);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, std::uint32_t keyValueTagIndex, void* pchKey, std::uint32_t cchKeySize, void* pchValue, std::uint32_t cchValueSize) {
        return steammock::slot<bool>(kCall_729, handle, index, keyValueTagIndex, pchKey, cchKeySize, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, const char* pchKey, void* pchValue, std::uint32_t cchValueSize) {
        return steammock::slot<bool>(kCall_792, handle, index, pchKey, pchValue, cchValueSize);
    }
    virtual bool ReleaseQueryUGCRequest(std::uint64_t handle) {
        return steammock::slot<bool>(kCall_730, handle);
    }
    virtual bool AddRequiredTag(std::uint64_t handle, const char* pTagName) {
        return steammock::slot<bool>(kCall_731, handle, pTagName);
    }
    virtual bool AddExcludedTag(std::uint64_t handle, const char* pTagName) {
        return steammock::slot<bool>(kCall_732, handle, pTagName);
    }
    virtual bool SetReturnOnlyIDs(std::uint64_t handle, bool bReturnOnlyIDs) {
        return steammock::slot<bool>(kCall_733, handle, bReturnOnlyIDs);
    }
    virtual bool SetReturnKeyValueTags(std::uint64_t handle, bool bReturnKeyValueTags) {
        return steammock::slot<bool>(kCall_734, handle, bReturnKeyValueTags);
    }
    virtual bool SetReturnLongDescription(std::uint64_t handle, bool bReturnLongDescription) {
        return steammock::slot<bool>(kCall_735, handle, bReturnLongDescription);
    }
    virtual bool SetReturnMetadata(std::uint64_t handle, bool bReturnMetadata) {
        return steammock::slot<bool>(kCall_736, handle, bReturnMetadata);
    }
    virtual bool SetReturnChildren(std::uint64_t handle, bool bReturnChildren) {
        return steammock::slot<bool>(kCall_737, handle, bReturnChildren);
    }
    virtual bool SetReturnAdditionalPreviews(std::uint64_t handle, bool bReturnAdditionalPreviews) {
        return steammock::slot<bool>(kCall_738, handle, bReturnAdditionalPreviews);
    }
    virtual bool SetReturnTotalOnly(std::uint64_t handle, bool bReturnTotalOnly) {
        return steammock::slot<bool>(kCall_739, handle, bReturnTotalOnly);
    }
    virtual bool SetReturnPlaytimeStats(std::uint64_t handle, std::uint32_t unDays) {
        return steammock::slot<bool>(kCall_740, handle, unDays);
    }
    virtual bool SetLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steammock::slot<bool>(kCall_741, handle, pchLanguage);
    }
    virtual bool SetAllowCachedResponse(std::uint64_t handle, std::uint32_t unMaxAgeSeconds) {
        return steammock::slot<bool>(kCall_742, handle, unMaxAgeSeconds);
    }
    virtual bool SetCloudFileNameFilter(std::uint64_t handle, const char* pMatchCloudFileName) {
        return steammock::slot<bool>(kCall_743, handle, pMatchCloudFileName);
    }
    virtual bool SetMatchAnyTag(std::uint64_t handle, bool bMatchAnyTag) {
        return steammock::slot<bool>(kCall_744, handle, bMatchAnyTag);
    }
    virtual bool SetSearchText(std::uint64_t handle, const char* pSearchText) {
        return steammock::slot<bool>(kCall_745, handle, pSearchText);
    }
    virtual bool SetRankedByTrendDays(std::uint64_t handle, std::uint32_t unDays) {
        return steammock::slot<bool>(kCall_746, handle, unDays);
    }
    virtual bool AddRequiredKeyValueTag(std::uint64_t handle, const char* pKey, const char* pValue) {
        return steammock::slot<bool>(kCall_747, handle, pKey, pValue);
    }
    virtual std::uint64_t RequestUGCDetails(std::uint64_t nPublishedFileID, std::uint32_t unMaxAgeSeconds) {
        return steammock::slot<std::uint64_t>(kCall_748, nPublishedFileID, unMaxAgeSeconds);
    }
    virtual std::uint64_t CreateItem(std::uint32_t nConsumerAppId, std::int32_t eFileType) {
        return steammock::slot<std::uint64_t>(kCall_749, nConsumerAppId, eFileType);
    }
    virtual std::uint64_t StartItemUpdate(std::uint32_t nConsumerAppId, std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_750, nConsumerAppId, nPublishedFileID);
    }
    virtual bool SetItemTitle(std::uint64_t handle, const char* pchTitle) {
        return steammock::slot<bool>(kCall_751, handle, pchTitle);
    }
    virtual bool SetItemDescription(std::uint64_t handle, const char* pchDescription) {
        return steammock::slot<bool>(kCall_752, handle, pchDescription);
    }
    virtual bool SetItemUpdateLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steammock::slot<bool>(kCall_753, handle, pchLanguage);
    }
    virtual bool SetItemMetadata(std::uint64_t handle, const char* pchMetaData) {
        return steammock::slot<bool>(kCall_754, handle, pchMetaData);
    }
    virtual bool SetItemVisibility(std::uint64_t handle, std::int32_t eVisibility) {
        return steammock::slot<bool>(kCall_755, handle, eVisibility);
    }
    virtual bool SetItemTags(std::uint64_t updateHandle, void* pTags) {
        return steammock::slot<bool>(kCall_756, updateHandle, pTags);
    }
    virtual bool SetItemContent(std::uint64_t handle, const char* pszContentFolder) {
        return steammock::slot<bool>(kCall_757, handle, pszContentFolder);
    }
    virtual bool SetItemPreview(std::uint64_t handle, const char* pszPreviewFile) {
        return steammock::slot<bool>(kCall_758, handle, pszPreviewFile);
    }
    virtual bool SetAllowLegacyUpload(std::uint64_t handle, bool bAllowLegacyUpload) {
        return steammock::slot<bool>(kCall_793, handle, bAllowLegacyUpload);
    }
    virtual bool RemoveAllItemKeyValueTags(std::uint64_t handle) {
        return steammock::slot<bool>(kCall_794, handle);
    }
    virtual bool RemoveItemKeyValueTags(std::uint64_t handle, const char* pchKey) {
        return steammock::slot<bool>(kCall_759, handle, pchKey);
    }
    virtual bool AddItemKeyValueTag(std::uint64_t handle, const char* pchKey, const char* pchValue) {
        return steammock::slot<bool>(kCall_760, handle, pchKey, pchValue);
    }
    virtual bool AddItemPreviewFile(std::uint64_t handle, const char* pszPreviewFile, std::int32_t type) {
        return steammock::slot<bool>(kCall_761, handle, pszPreviewFile, type);
    }
    virtual bool AddItemPreviewVideo(std::uint64_t handle, const char* pszVideoID) {
        return steammock::slot<bool>(kCall_762, handle, pszVideoID);
    }
    virtual bool UpdateItemPreviewFile(std::uint64_t handle, std::uint32_t index, const char* pszPreviewFile) {
        return steammock::slot<bool>(kCall_763, handle, index, pszPreviewFile);
    }
    virtual bool UpdateItemPreviewVideo(std::uint64_t handle, std::uint32_t index, const char* pszVideoID) {
        return steammock::slot<bool>(kCall_764, handle, index, pszVideoID);
    }
    virtual bool RemoveItemPreview(std::uint64_t handle, std::uint32_t index) {
        return steammock::slot<bool>(kCall_765, handle, index);
    }
    virtual std::uint64_t SubmitItemUpdate(std::uint64_t handle, const char* pchChangeNote) {
        return steammock::slot<std::uint64_t>(kCall_766, handle, pchChangeNote);
    }
    virtual std::int32_t GetItemUpdateProgress(std::uint64_t handle, std::uint64_t* punBytesProcessed, std::uint64_t* punBytesTotal) {
        return steammock::slot<std::int32_t>(kCall_767, handle, punBytesProcessed, punBytesTotal);
    }
    virtual std::uint64_t SetUserItemVote(std::uint64_t nPublishedFileID, bool bVoteUp) {
        return steammock::slot<std::uint64_t>(kCall_768, nPublishedFileID, bVoteUp);
    }
    virtual std::uint64_t GetUserItemVote(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_769, nPublishedFileID);
    }
    virtual std::uint64_t AddItemToFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_770, nAppId, nPublishedFileID);
    }
    virtual std::uint64_t RemoveItemFromFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_771, nAppId, nPublishedFileID);
    }
    virtual std::uint64_t SubscribeItem(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_772, nPublishedFileID);
    }
    virtual std::uint64_t UnsubscribeItem(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_773, nPublishedFileID);
    }
    virtual std::uint32_t GetNumSubscribedItems() {
        return steammock::slot<std::uint32_t>(kCall_774);
    }
    virtual std::uint32_t GetSubscribedItems(std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steammock::slot<std::uint32_t>(kCall_775, pvecPublishedFileID, cMaxEntries);
    }
    virtual std::uint32_t GetItemState(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint32_t>(kCall_776, nPublishedFileID);
    }
    virtual bool GetItemInstallInfo(std::uint64_t nPublishedFileID, std::uint64_t* punSizeOnDisk, void* pchFolder, std::uint32_t cchFolderSize, std::uint32_t* punTimeStamp) {
        return steammock::slot<bool>(kCall_777, nPublishedFileID, punSizeOnDisk, pchFolder, cchFolderSize, punTimeStamp);
    }
    virtual bool GetItemDownloadInfo(std::uint64_t nPublishedFileID, std::uint64_t* punBytesDownloaded, std::uint64_t* punBytesTotal) {
        return steammock::slot<bool>(kCall_778, nPublishedFileID, punBytesDownloaded, punBytesTotal);
    }
    virtual bool DownloadItem(std::uint64_t nPublishedFileID, bool bHighPriority) {
        return steammock::slot<bool>(kCall_779, nPublishedFileID, bHighPriority);
    }
    virtual bool BInitWorkshopForGameServer(std::uint32_t unWorkshopDepotID, const char* pszFolder) {
        return steammock::slot<bool>(kCall_780, unWorkshopDepotID, pszFolder);
    }
    virtual void SuspendDownloads(bool bSuspend) { steammock::slot<void>(kCall_781, bSuspend); }
    virtual std::uint64_t StartPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steammock::slot<std::uint64_t>(kCall_782, pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steammock::slot<std::uint64_t>(kCall_783, pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTrackingForAllItems() {
        return steammock::slot<std::uint64_t>(kCall_784);
    }
    virtual std::uint64_t AddDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_785, nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t RemoveDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_786, nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t AddAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steammock::slot<std::uint64_t>(kCall_787, nPublishedFileID, nAppID);
    }
    virtual std::uint64_t RemoveAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steammock::slot<std::uint64_t>(kCall_788, nPublishedFileID, nAppID);
    }
    virtual std::uint64_t GetAppDependencies(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_789, nPublishedFileID);
    }
    virtual std::uint64_t DeleteItem(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_790, nPublishedFileID);
    }
};

Version_STEAMUGC_INTERFACE_VERSION013 g_STEAMUGC_INTERFACE_VERSION013;

// ISteamUGC STEAMUGC_INTERFACE_VERSION014
class Version_STEAMUGC_INTERFACE_VERSION014 {
public:
    virtual std::uint64_t CreateQueryUserUGCRequest(std::uint32_t unAccountID, std::int32_t eListType, std::int32_t eMatchingUGCType, std::int32_t eSortOrder, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steammock::slot<std::uint64_t>(kCall_717, unAccountID, eListType, eMatchingUGCType, eSortOrder, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steammock::slot<std::uint64_t>(kCall_718, eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, const char* pchCursor) {
        return steammock::slot<std::uint64_t>(kCall_791, eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, pchCursor);
    }
    virtual std::uint64_t CreateQueryUGCDetailsRequest(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steammock::slot<std::uint64_t>(kCall_719, pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t SendQueryUGCRequest(std::uint64_t handle) {
        return steammock::slot<std::uint64_t>(kCall_720, handle);
    }
    virtual bool GetQueryUGCResult(std::uint64_t handle, std::uint32_t index, void* pDetails) {
        return steammock::slot<bool>(kCall_721, handle, index, pDetails);
    }
    virtual bool GetQueryUGCPreviewURL(std::uint64_t handle, std::uint32_t index, void* pchURL, std::uint32_t cchURLSize) {
        return steammock::slot<bool>(kCall_722, handle, index, pchURL, cchURLSize);
    }
    virtual bool GetQueryUGCMetadata(std::uint64_t handle, std::uint32_t index, void* pchMetadata, std::uint32_t cchMetadatasize) {
        return steammock::slot<bool>(kCall_723, handle, index, pchMetadata, cchMetadatasize);
    }
    virtual bool GetQueryUGCChildren(std::uint64_t handle, std::uint32_t index, std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steammock::slot<bool>(kCall_724, handle, index, pvecPublishedFileID, cMaxEntries);
    }
    virtual bool GetQueryUGCStatistic(std::uint64_t handle, std::uint32_t index, std::int32_t eStatType, std::uint64_t* pStatValue) {
        return steammock::slot<bool>(kCall_725, handle, index, eStatType, pStatValue);
    }
    virtual std::uint32_t GetQueryUGCNumAdditionalPreviews(std::uint64_t handle, std::uint32_t index) {
        return steammock::slot<std::uint32_t>(kCall_726, handle, index);
    }
    virtual bool GetQueryUGCAdditionalPreview(std::uint64_t handle, std::uint32_t index, std::uint32_t previewIndex, void* pchURLOrVideoID, std::uint32_t cchURLSize, void* pchOriginalFileName, std::uint32_t cchOriginalFileNameSize, std::int32_t* pPreviewType) {
        return steammock::slot<bool>(kCall_727, handle, index, previewIndex, pchURLOrVideoID, cchURLSize, pchOriginalFileName, cchOriginalFileNameSize, pPreviewType);
    }
    virtual std::uint32_t GetQueryUGCNumKeyValueTags(std::uint64_t handle, std::uint32_t index) {
        return steammock::slot<std::uint32_t>(kCall_728, handle, index);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, std::uint32_t keyValueTagIndex, void* pchKey, std::uint32_t cchKeySize, void* pchValue, std::uint32_t cchValueSize) {
        return steammock::slot<bool>(kCall_729, handle, index, keyValueTagIndex, pchKey, cchKeySize, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, const char* pchKey, void* pchValue, std::uint32_t cchValueSize) {
        return steammock::slot<bool>(kCall_792, handle, index, pchKey, pchValue, cchValueSize);
    }
    virtual bool ReleaseQueryUGCRequest(std::uint64_t handle) {
        return steammock::slot<bool>(kCall_730, handle);
    }
    virtual bool AddRequiredTag(std::uint64_t handle, const char* pTagName) {
        return steammock::slot<bool>(kCall_731, handle, pTagName);
    }
    virtual bool AddRequiredTagGroup(std::uint64_t handle, void* pTagGroups) {
        return steammock::slot<bool>(kCall_795, handle, pTagGroups);
    }
    virtual bool AddExcludedTag(std::uint64_t handle, const char* pTagName) {
        return steammock::slot<bool>(kCall_732, handle, pTagName);
    }
    virtual bool SetReturnOnlyIDs(std::uint64_t handle, bool bReturnOnlyIDs) {
        return steammock::slot<bool>(kCall_733, handle, bReturnOnlyIDs);
    }
    virtual bool SetReturnKeyValueTags(std::uint64_t handle, bool bReturnKeyValueTags) {
        return steammock::slot<bool>(kCall_734, handle, bReturnKeyValueTags);
    }
    virtual bool SetReturnLongDescription(std::uint64_t handle, bool bReturnLongDescription) {
        return steammock::slot<bool>(kCall_735, handle, bReturnLongDescription);
    }
    virtual bool SetReturnMetadata(std::uint64_t handle, bool bReturnMetadata) {
        return steammock::slot<bool>(kCall_736, handle, bReturnMetadata);
    }
    virtual bool SetReturnChildren(std::uint64_t handle, bool bReturnChildren) {
        return steammock::slot<bool>(kCall_737, handle, bReturnChildren);
    }
    virtual bool SetReturnAdditionalPreviews(std::uint64_t handle, bool bReturnAdditionalPreviews) {
        return steammock::slot<bool>(kCall_738, handle, bReturnAdditionalPreviews);
    }
    virtual bool SetReturnTotalOnly(std::uint64_t handle, bool bReturnTotalOnly) {
        return steammock::slot<bool>(kCall_739, handle, bReturnTotalOnly);
    }
    virtual bool SetReturnPlaytimeStats(std::uint64_t handle, std::uint32_t unDays) {
        return steammock::slot<bool>(kCall_740, handle, unDays);
    }
    virtual bool SetLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steammock::slot<bool>(kCall_741, handle, pchLanguage);
    }
    virtual bool SetAllowCachedResponse(std::uint64_t handle, std::uint32_t unMaxAgeSeconds) {
        return steammock::slot<bool>(kCall_742, handle, unMaxAgeSeconds);
    }
    virtual bool SetCloudFileNameFilter(std::uint64_t handle, const char* pMatchCloudFileName) {
        return steammock::slot<bool>(kCall_743, handle, pMatchCloudFileName);
    }
    virtual bool SetMatchAnyTag(std::uint64_t handle, bool bMatchAnyTag) {
        return steammock::slot<bool>(kCall_744, handle, bMatchAnyTag);
    }
    virtual bool SetSearchText(std::uint64_t handle, const char* pSearchText) {
        return steammock::slot<bool>(kCall_745, handle, pSearchText);
    }
    virtual bool SetRankedByTrendDays(std::uint64_t handle, std::uint32_t unDays) {
        return steammock::slot<bool>(kCall_746, handle, unDays);
    }
    virtual bool AddRequiredKeyValueTag(std::uint64_t handle, const char* pKey, const char* pValue) {
        return steammock::slot<bool>(kCall_747, handle, pKey, pValue);
    }
    virtual std::uint64_t RequestUGCDetails(std::uint64_t nPublishedFileID, std::uint32_t unMaxAgeSeconds) {
        return steammock::slot<std::uint64_t>(kCall_748, nPublishedFileID, unMaxAgeSeconds);
    }
    virtual std::uint64_t CreateItem(std::uint32_t nConsumerAppId, std::int32_t eFileType) {
        return steammock::slot<std::uint64_t>(kCall_749, nConsumerAppId, eFileType);
    }
    virtual std::uint64_t StartItemUpdate(std::uint32_t nConsumerAppId, std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_750, nConsumerAppId, nPublishedFileID);
    }
    virtual bool SetItemTitle(std::uint64_t handle, const char* pchTitle) {
        return steammock::slot<bool>(kCall_751, handle, pchTitle);
    }
    virtual bool SetItemDescription(std::uint64_t handle, const char* pchDescription) {
        return steammock::slot<bool>(kCall_752, handle, pchDescription);
    }
    virtual bool SetItemUpdateLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steammock::slot<bool>(kCall_753, handle, pchLanguage);
    }
    virtual bool SetItemMetadata(std::uint64_t handle, const char* pchMetaData) {
        return steammock::slot<bool>(kCall_754, handle, pchMetaData);
    }
    virtual bool SetItemVisibility(std::uint64_t handle, std::int32_t eVisibility) {
        return steammock::slot<bool>(kCall_755, handle, eVisibility);
    }
    virtual bool SetItemTags(std::uint64_t updateHandle, void* pTags) {
        return steammock::slot<bool>(kCall_756, updateHandle, pTags);
    }
    virtual bool SetItemContent(std::uint64_t handle, const char* pszContentFolder) {
        return steammock::slot<bool>(kCall_757, handle, pszContentFolder);
    }
    virtual bool SetItemPreview(std::uint64_t handle, const char* pszPreviewFile) {
        return steammock::slot<bool>(kCall_758, handle, pszPreviewFile);
    }
    virtual bool SetAllowLegacyUpload(std::uint64_t handle, bool bAllowLegacyUpload) {
        return steammock::slot<bool>(kCall_793, handle, bAllowLegacyUpload);
    }
    virtual bool RemoveAllItemKeyValueTags(std::uint64_t handle) {
        return steammock::slot<bool>(kCall_794, handle);
    }
    virtual bool RemoveItemKeyValueTags(std::uint64_t handle, const char* pchKey) {
        return steammock::slot<bool>(kCall_759, handle, pchKey);
    }
    virtual bool AddItemKeyValueTag(std::uint64_t handle, const char* pchKey, const char* pchValue) {
        return steammock::slot<bool>(kCall_760, handle, pchKey, pchValue);
    }
    virtual bool AddItemPreviewFile(std::uint64_t handle, const char* pszPreviewFile, std::int32_t type) {
        return steammock::slot<bool>(kCall_761, handle, pszPreviewFile, type);
    }
    virtual bool AddItemPreviewVideo(std::uint64_t handle, const char* pszVideoID) {
        return steammock::slot<bool>(kCall_762, handle, pszVideoID);
    }
    virtual bool UpdateItemPreviewFile(std::uint64_t handle, std::uint32_t index, const char* pszPreviewFile) {
        return steammock::slot<bool>(kCall_763, handle, index, pszPreviewFile);
    }
    virtual bool UpdateItemPreviewVideo(std::uint64_t handle, std::uint32_t index, const char* pszVideoID) {
        return steammock::slot<bool>(kCall_764, handle, index, pszVideoID);
    }
    virtual bool RemoveItemPreview(std::uint64_t handle, std::uint32_t index) {
        return steammock::slot<bool>(kCall_765, handle, index);
    }
    virtual std::uint64_t SubmitItemUpdate(std::uint64_t handle, const char* pchChangeNote) {
        return steammock::slot<std::uint64_t>(kCall_766, handle, pchChangeNote);
    }
    virtual std::int32_t GetItemUpdateProgress(std::uint64_t handle, std::uint64_t* punBytesProcessed, std::uint64_t* punBytesTotal) {
        return steammock::slot<std::int32_t>(kCall_767, handle, punBytesProcessed, punBytesTotal);
    }
    virtual std::uint64_t SetUserItemVote(std::uint64_t nPublishedFileID, bool bVoteUp) {
        return steammock::slot<std::uint64_t>(kCall_768, nPublishedFileID, bVoteUp);
    }
    virtual std::uint64_t GetUserItemVote(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_769, nPublishedFileID);
    }
    virtual std::uint64_t AddItemToFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_770, nAppId, nPublishedFileID);
    }
    virtual std::uint64_t RemoveItemFromFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_771, nAppId, nPublishedFileID);
    }
    virtual std::uint64_t SubscribeItem(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_772, nPublishedFileID);
    }
    virtual std::uint64_t UnsubscribeItem(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_773, nPublishedFileID);
    }
    virtual std::uint32_t GetNumSubscribedItems() {
        return steammock::slot<std::uint32_t>(kCall_774);
    }
    virtual std::uint32_t GetSubscribedItems(std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steammock::slot<std::uint32_t>(kCall_775, pvecPublishedFileID, cMaxEntries);
    }
    virtual std::uint32_t GetItemState(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint32_t>(kCall_776, nPublishedFileID);
    }
    virtual bool GetItemInstallInfo(std::uint64_t nPublishedFileID, std::uint64_t* punSizeOnDisk, void* pchFolder, std::uint32_t cchFolderSize, std::uint32_t* punTimeStamp) {
        return steammock::slot<bool>(kCall_777, nPublishedFileID, punSizeOnDisk, pchFolder, cchFolderSize, punTimeStamp);
    }
    virtual bool GetItemDownloadInfo(std::uint64_t nPublishedFileID, std::uint64_t* punBytesDownloaded, std::uint64_t* punBytesTotal) {
        return steammock::slot<bool>(kCall_778, nPublishedFileID, punBytesDownloaded, punBytesTotal);
    }
    virtual bool DownloadItem(std::uint64_t nPublishedFileID, bool bHighPriority) {
        return steammock::slot<bool>(kCall_779, nPublishedFileID, bHighPriority);
    }
    virtual bool BInitWorkshopForGameServer(std::uint32_t unWorkshopDepotID, const char* pszFolder) {
        return steammock::slot<bool>(kCall_780, unWorkshopDepotID, pszFolder);
    }
    virtual void SuspendDownloads(bool bSuspend) { steammock::slot<void>(kCall_781, bSuspend); }
    virtual std::uint64_t StartPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steammock::slot<std::uint64_t>(kCall_782, pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steammock::slot<std::uint64_t>(kCall_783, pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTrackingForAllItems() {
        return steammock::slot<std::uint64_t>(kCall_784);
    }
    virtual std::uint64_t AddDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_785, nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t RemoveDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_786, nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t AddAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steammock::slot<std::uint64_t>(kCall_787, nPublishedFileID, nAppID);
    }
    virtual std::uint64_t RemoveAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steammock::slot<std::uint64_t>(kCall_788, nPublishedFileID, nAppID);
    }
    virtual std::uint64_t GetAppDependencies(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_789, nPublishedFileID);
    }
    virtual std::uint64_t DeleteItem(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_790, nPublishedFileID);
    }
};

Version_STEAMUGC_INTERFACE_VERSION014 g_STEAMUGC_INTERFACE_VERSION014;

// ISteamUGC STEAMUGC_INTERFACE_VERSION015
class Version_STEAMUGC_INTERFACE_VERSION015 {
public:
    virtual std::uint64_t CreateQueryUserUGCRequest(std::uint32_t unAccountID, std::int32_t eListType, std::int32_t eMatchingUGCType, std::int32_t eSortOrder, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steammock::slot<std::uint64_t>(kCall_717, unAccountID, eListType, eMatchingUGCType, eSortOrder, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steammock::slot<std::uint64_t>(kCall_718, eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, const char* pchCursor) {
        return steammock::slot<std::uint64_t>(kCall_791, eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, pchCursor);
    }
    virtual std::uint64_t CreateQueryUGCDetailsRequest(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steammock::slot<std::uint64_t>(kCall_719, pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t SendQueryUGCRequest(std::uint64_t handle) {
        return steammock::slot<std::uint64_t>(kCall_720, handle);
    }
    virtual bool GetQueryUGCResult(std::uint64_t handle, std::uint32_t index, void* pDetails) {
        return steammock::slot<bool>(kCall_721, handle, index, pDetails);
    }
    virtual std::uint32_t GetQueryUGCNumTags(std::uint64_t handle, std::uint32_t index) {
        return steammock::slot<std::uint32_t>(kCall_796, handle, index);
    }
    virtual bool GetQueryUGCTag(std::uint64_t handle, std::uint32_t index, std::uint32_t indexTag, void* pchValue, std::uint32_t cchValueSize) {
        return steammock::slot<bool>(kCall_797, handle, index, indexTag, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCTagDisplayName(std::uint64_t handle, std::uint32_t index, std::uint32_t indexTag, void* pchValue, std::uint32_t cchValueSize) {
        return steammock::slot<bool>(kCall_798, handle, index, indexTag, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCPreviewURL(std::uint64_t handle, std::uint32_t index, void* pchURL, std::uint32_t cchURLSize) {
        return steammock::slot<bool>(kCall_722, handle, index, pchURL, cchURLSize);
    }
    virtual bool GetQueryUGCMetadata(std::uint64_t handle, std::uint32_t index, void* pchMetadata, std::uint32_t cchMetadatasize) {
        return steammock::slot<bool>(kCall_723, handle, index, pchMetadata, cchMetadatasize);
    }
    virtual bool GetQueryUGCChildren(std::uint64_t handle, std::uint32_t index, std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steammock::slot<bool>(kCall_724, handle, index, pvecPublishedFileID, cMaxEntries);
    }
    virtual bool GetQueryUGCStatistic(std::uint64_t handle, std::uint32_t index, std::int32_t eStatType, std::uint64_t* pStatValue) {
        return steammock::slot<bool>(kCall_725, handle, index, eStatType, pStatValue);
    }
    virtual std::uint32_t GetQueryUGCNumAdditionalPreviews(std::uint64_t handle, std::uint32_t index) {
        return steammock::slot<std::uint32_t>(kCall_726, handle, index);
    }
    virtual bool GetQueryUGCAdditionalPreview(std::uint64_t handle, std::uint32_t index, std::uint32_t previewIndex, void* pchURLOrVideoID, std::uint32_t cchURLSize, void* pchOriginalFileName, std::uint32_t cchOriginalFileNameSize, std::int32_t* pPreviewType) {
        return steammock::slot<bool>(kCall_727, handle, index, previewIndex, pchURLOrVideoID, cchURLSize, pchOriginalFileName, cchOriginalFileNameSize, pPreviewType);
    }
    virtual std::uint32_t GetQueryUGCNumKeyValueTags(std::uint64_t handle, std::uint32_t index) {
        return steammock::slot<std::uint32_t>(kCall_728, handle, index);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, std::uint32_t keyValueTagIndex, void* pchKey, std::uint32_t cchKeySize, void* pchValue, std::uint32_t cchValueSize) {
        return steammock::slot<bool>(kCall_729, handle, index, keyValueTagIndex, pchKey, cchKeySize, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, const char* pchKey, void* pchValue, std::uint32_t cchValueSize) {
        return steammock::slot<bool>(kCall_792, handle, index, pchKey, pchValue, cchValueSize);
    }
    virtual bool ReleaseQueryUGCRequest(std::uint64_t handle) {
        return steammock::slot<bool>(kCall_730, handle);
    }
    virtual bool AddRequiredTag(std::uint64_t handle, const char* pTagName) {
        return steammock::slot<bool>(kCall_731, handle, pTagName);
    }
    virtual bool AddRequiredTagGroup(std::uint64_t handle, void* pTagGroups) {
        return steammock::slot<bool>(kCall_795, handle, pTagGroups);
    }
    virtual bool AddExcludedTag(std::uint64_t handle, const char* pTagName) {
        return steammock::slot<bool>(kCall_732, handle, pTagName);
    }
    virtual bool SetReturnOnlyIDs(std::uint64_t handle, bool bReturnOnlyIDs) {
        return steammock::slot<bool>(kCall_733, handle, bReturnOnlyIDs);
    }
    virtual bool SetReturnKeyValueTags(std::uint64_t handle, bool bReturnKeyValueTags) {
        return steammock::slot<bool>(kCall_734, handle, bReturnKeyValueTags);
    }
    virtual bool SetReturnLongDescription(std::uint64_t handle, bool bReturnLongDescription) {
        return steammock::slot<bool>(kCall_735, handle, bReturnLongDescription);
    }
    virtual bool SetReturnMetadata(std::uint64_t handle, bool bReturnMetadata) {
        return steammock::slot<bool>(kCall_736, handle, bReturnMetadata);
    }
    virtual bool SetReturnChildren(std::uint64_t handle, bool bReturnChildren) {
        return steammock::slot<bool>(kCall_737, handle, bReturnChildren);
    }
    virtual bool SetReturnAdditionalPreviews(std::uint64_t handle, bool bReturnAdditionalPreviews) {
        return steammock::slot<bool>(kCall_738, handle, bReturnAdditionalPreviews);
    }
    virtual bool SetReturnTotalOnly(std::uint64_t handle, bool bReturnTotalOnly) {
        return steammock::slot<bool>(kCall_739, handle, bReturnTotalOnly);
    }
    virtual bool SetReturnPlaytimeStats(std::uint64_t handle, std::uint32_t unDays) {
        return steammock::slot<bool>(kCall_740, handle, unDays);
    }
    virtual bool SetLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steammock::slot<bool>(kCall_741, handle, pchLanguage);
    }
    virtual bool SetAllowCachedResponse(std::uint64_t handle, std::uint32_t unMaxAgeSeconds) {
        return steammock::slot<bool>(kCall_742, handle, unMaxAgeSeconds);
    }
    virtual bool SetCloudFileNameFilter(std::uint64_t handle, const char* pMatchCloudFileName) {
        return steammock::slot<bool>(kCall_743, handle, pMatchCloudFileName);
    }
    virtual bool SetMatchAnyTag(std::uint64_t handle, bool bMatchAnyTag) {
        return steammock::slot<bool>(kCall_744, handle, bMatchAnyTag);
    }
    virtual bool SetSearchText(std::uint64_t handle, const char* pSearchText) {
        return steammock::slot<bool>(kCall_745, handle, pSearchText);
    }
    virtual bool SetRankedByTrendDays(std::uint64_t handle, std::uint32_t unDays) {
        return steammock::slot<bool>(kCall_746, handle, unDays);
    }
    virtual bool AddRequiredKeyValueTag(std::uint64_t handle, const char* pKey, const char* pValue) {
        return steammock::slot<bool>(kCall_747, handle, pKey, pValue);
    }
    virtual std::uint64_t RequestUGCDetails(std::uint64_t nPublishedFileID, std::uint32_t unMaxAgeSeconds) {
        return steammock::slot<std::uint64_t>(kCall_748, nPublishedFileID, unMaxAgeSeconds);
    }
    virtual std::uint64_t CreateItem(std::uint32_t nConsumerAppId, std::int32_t eFileType) {
        return steammock::slot<std::uint64_t>(kCall_749, nConsumerAppId, eFileType);
    }
    virtual std::uint64_t StartItemUpdate(std::uint32_t nConsumerAppId, std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_750, nConsumerAppId, nPublishedFileID);
    }
    virtual bool SetItemTitle(std::uint64_t handle, const char* pchTitle) {
        return steammock::slot<bool>(kCall_751, handle, pchTitle);
    }
    virtual bool SetItemDescription(std::uint64_t handle, const char* pchDescription) {
        return steammock::slot<bool>(kCall_752, handle, pchDescription);
    }
    virtual bool SetItemUpdateLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steammock::slot<bool>(kCall_753, handle, pchLanguage);
    }
    virtual bool SetItemMetadata(std::uint64_t handle, const char* pchMetaData) {
        return steammock::slot<bool>(kCall_754, handle, pchMetaData);
    }
    virtual bool SetItemVisibility(std::uint64_t handle, std::int32_t eVisibility) {
        return steammock::slot<bool>(kCall_755, handle, eVisibility);
    }
    virtual bool SetItemTags(std::uint64_t updateHandle, void* pTags) {
        return steammock::slot<bool>(kCall_756, updateHandle, pTags);
    }
    virtual bool SetItemContent(std::uint64_t handle, const char* pszContentFolder) {
        return steammock::slot<bool>(kCall_757, handle, pszContentFolder);
    }
    virtual bool SetItemPreview(std::uint64_t handle, const char* pszPreviewFile) {
        return steammock::slot<bool>(kCall_758, handle, pszPreviewFile);
    }
    virtual bool SetAllowLegacyUpload(std::uint64_t handle, bool bAllowLegacyUpload) {
        return steammock::slot<bool>(kCall_793, handle, bAllowLegacyUpload);
    }
    virtual bool RemoveAllItemKeyValueTags(std::uint64_t handle) {
        return steammock::slot<bool>(kCall_794, handle);
    }
    virtual bool RemoveItemKeyValueTags(std::uint64_t handle, const char* pchKey) {
        return steammock::slot<bool>(kCall_759, handle, pchKey);
    }
    virtual bool AddItemKeyValueTag(std::uint64_t handle, const char* pchKey, const char* pchValue) {
        return steammock::slot<bool>(kCall_760, handle, pchKey, pchValue);
    }
    virtual bool AddItemPreviewFile(std::uint64_t handle, const char* pszPreviewFile, std::int32_t type) {
        return steammock::slot<bool>(kCall_761, handle, pszPreviewFile, type);
    }
    virtual bool AddItemPreviewVideo(std::uint64_t handle, const char* pszVideoID) {
        return steammock::slot<bool>(kCall_762, handle, pszVideoID);
    }
    virtual bool UpdateItemPreviewFile(std::uint64_t handle, std::uint32_t index, const char* pszPreviewFile) {
        return steammock::slot<bool>(kCall_763, handle, index, pszPreviewFile);
    }
    virtual bool UpdateItemPreviewVideo(std::uint64_t handle, std::uint32_t index, const char* pszVideoID) {
        return steammock::slot<bool>(kCall_764, handle, index, pszVideoID);
    }
    virtual bool RemoveItemPreview(std::uint64_t handle, std::uint32_t index) {
        return steammock::slot<bool>(kCall_765, handle, index);
    }
    virtual std::uint64_t SubmitItemUpdate(std::uint64_t handle, const char* pchChangeNote) {
        return steammock::slot<std::uint64_t>(kCall_766, handle, pchChangeNote);
    }
    virtual std::int32_t GetItemUpdateProgress(std::uint64_t handle, std::uint64_t* punBytesProcessed, std::uint64_t* punBytesTotal) {
        return steammock::slot<std::int32_t>(kCall_767, handle, punBytesProcessed, punBytesTotal);
    }
    virtual std::uint64_t SetUserItemVote(std::uint64_t nPublishedFileID, bool bVoteUp) {
        return steammock::slot<std::uint64_t>(kCall_768, nPublishedFileID, bVoteUp);
    }
    virtual std::uint64_t GetUserItemVote(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_769, nPublishedFileID);
    }
    virtual std::uint64_t AddItemToFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_770, nAppId, nPublishedFileID);
    }
    virtual std::uint64_t RemoveItemFromFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_771, nAppId, nPublishedFileID);
    }
    virtual std::uint64_t SubscribeItem(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_772, nPublishedFileID);
    }
    virtual std::uint64_t UnsubscribeItem(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_773, nPublishedFileID);
    }
    virtual std::uint32_t GetNumSubscribedItems() {
        return steammock::slot<std::uint32_t>(kCall_774);
    }
    virtual std::uint32_t GetSubscribedItems(std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steammock::slot<std::uint32_t>(kCall_775, pvecPublishedFileID, cMaxEntries);
    }
    virtual std::uint32_t GetItemState(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint32_t>(kCall_776, nPublishedFileID);
    }
    virtual bool GetItemInstallInfo(std::uint64_t nPublishedFileID, std::uint64_t* punSizeOnDisk, void* pchFolder, std::uint32_t cchFolderSize, std::uint32_t* punTimeStamp) {
        return steammock::slot<bool>(kCall_777, nPublishedFileID, punSizeOnDisk, pchFolder, cchFolderSize, punTimeStamp);
    }
    virtual bool GetItemDownloadInfo(std::uint64_t nPublishedFileID, std::uint64_t* punBytesDownloaded, std::uint64_t* punBytesTotal) {
        return steammock::slot<bool>(kCall_778, nPublishedFileID, punBytesDownloaded, punBytesTotal);
    }
    virtual bool DownloadItem(std::uint64_t nPublishedFileID, bool bHighPriority) {
        return steammock::slot<bool>(kCall_779, nPublishedFileID, bHighPriority);
    }
    virtual bool BInitWorkshopForGameServer(std::uint32_t unWorkshopDepotID, const char* pszFolder) {
        return steammock::slot<bool>(kCall_780, unWorkshopDepotID, pszFolder);
    }
    virtual void SuspendDownloads(bool bSuspend) { steammock::slot<void>(kCall_781, bSuspend); }
    virtual std::uint64_t StartPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steammock::slot<std::uint64_t>(kCall_782, pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steammock::slot<std::uint64_t>(kCall_783, pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTrackingForAllItems() {
        return steammock::slot<std::uint64_t>(kCall_784);
    }
    virtual std::uint64_t AddDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_785, nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t RemoveDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_786, nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t AddAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steammock::slot<std::uint64_t>(kCall_787, nPublishedFileID, nAppID);
    }
    virtual std::uint64_t RemoveAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steammock::slot<std::uint64_t>(kCall_788, nPublishedFileID, nAppID);
    }
    virtual std::uint64_t GetAppDependencies(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_789, nPublishedFileID);
    }
    virtual std::uint64_t DeleteItem(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_790, nPublishedFileID);
    }
};

Version_STEAMUGC_INTERFACE_VERSION015 g_STEAMUGC_INTERFACE_VERSION015;

// ISteamUGC STEAMUGC_INTERFACE_VERSION016
class Version_STEAMUGC_INTERFACE_VERSION016 {
public:
    virtual std::uint64_t CreateQueryUserUGCRequest(std::uint32_t unAccountID, std::int32_t eListType, std::int32_t eMatchingUGCType, std::int32_t eSortOrder, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steammock::slot<std::uint64_t>(kCall_717, unAccountID, eListType, eMatchingUGCType, eSortOrder, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steammock::slot<std::uint64_t>(kCall_718, eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, const char* pchCursor) {
        return steammock::slot<std::uint64_t>(kCall_791, eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, pchCursor);
    }
    virtual std::uint64_t CreateQueryUGCDetailsRequest(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steammock::slot<std::uint64_t>(kCall_719, pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t SendQueryUGCRequest(std::uint64_t handle) {
        return steammock::slot<std::uint64_t>(kCall_720, handle);
    }
    virtual bool GetQueryUGCResult(std::uint64_t handle, std::uint32_t index, void* pDetails) {
        return steammock::slot<bool>(kCall_721, handle, index, pDetails);
    }
    virtual std::uint32_t GetQueryUGCNumTags(std::uint64_t handle, std::uint32_t index) {
        return steammock::slot<std::uint32_t>(kCall_796, handle, index);
    }
    virtual bool GetQueryUGCTag(std::uint64_t handle, std::uint32_t index, std::uint32_t indexTag, void* pchValue, std::uint32_t cchValueSize) {
        return steammock::slot<bool>(kCall_797, handle, index, indexTag, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCTagDisplayName(std::uint64_t handle, std::uint32_t index, std::uint32_t indexTag, void* pchValue, std::uint32_t cchValueSize) {
        return steammock::slot<bool>(kCall_798, handle, index, indexTag, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCPreviewURL(std::uint64_t handle, std::uint32_t index, void* pchURL, std::uint32_t cchURLSize) {
        return steammock::slot<bool>(kCall_722, handle, index, pchURL, cchURLSize);
    }
    virtual bool GetQueryUGCMetadata(std::uint64_t handle, std::uint32_t index, void* pchMetadata, std::uint32_t cchMetadatasize) {
        return steammock::slot<bool>(kCall_723, handle, index, pchMetadata, cchMetadatasize);
    }
    virtual bool GetQueryUGCChildren(std::uint64_t handle, std::uint32_t index, std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steammock::slot<bool>(kCall_724, handle, index, pvecPublishedFileID, cMaxEntries);
    }
    virtual bool GetQueryUGCStatistic(std::uint64_t handle, std::uint32_t index, std::int32_t eStatType, std::uint64_t* pStatValue) {
        return steammock::slot<bool>(kCall_725, handle, index, eStatType, pStatValue);
    }
    virtual std::uint32_t GetQueryUGCNumAdditionalPreviews(std::uint64_t handle, std::uint32_t index) {
        return steammock::slot<std::uint32_t>(kCall_726, handle, index);
    }
    virtual bool GetQueryUGCAdditionalPreview(std::uint64_t handle, std::uint32_t index, std::uint32_t previewIndex, void* pchURLOrVideoID, std::uint32_t cchURLSize, void* pchOriginalFileName, std::uint32_t cchOriginalFileNameSize, std::int32_t* pPreviewType) {
        return steammock::slot<bool>(kCall_727, handle, index, previewIndex, pchURLOrVideoID, cchURLSize, pchOriginalFileName, cchOriginalFileNameSize, pPreviewType);
    }
    virtual std::uint32_t GetQueryUGCNumKeyValueTags(std::uint64_t handle, std::uint32_t index) {
        return steammock::slot<std::uint32_t>(kCall_728, handle, index);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, std::uint32_t keyValueTagIndex, void* pchKey, std::uint32_t cchKeySize, void* pchValue, std::uint32_t cchValueSize) {
        return steammock::slot<bool>(kCall_729, handle, index, keyValueTagIndex, pchKey, cchKeySize, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, const char* pchKey, void* pchValue, std::uint32_t cchValueSize) {
        return steammock::slot<bool>(kCall_792, handle, index, pchKey, pchValue, cchValueSize);
    }
    virtual bool ReleaseQueryUGCRequest(std::uint64_t handle) {
        return steammock::slot<bool>(kCall_730, handle);
    }
    virtual bool AddRequiredTag(std::uint64_t handle, const char* pTagName) {
        return steammock::slot<bool>(kCall_731, handle, pTagName);
    }
    virtual bool AddRequiredTagGroup(std::uint64_t handle, void* pTagGroups) {
        return steammock::slot<bool>(kCall_795, handle, pTagGroups);
    }
    virtual bool AddExcludedTag(std::uint64_t handle, const char* pTagName) {
        return steammock::slot<bool>(kCall_732, handle, pTagName);
    }
    virtual bool SetReturnOnlyIDs(std::uint64_t handle, bool bReturnOnlyIDs) {
        return steammock::slot<bool>(kCall_733, handle, bReturnOnlyIDs);
    }
    virtual bool SetReturnKeyValueTags(std::uint64_t handle, bool bReturnKeyValueTags) {
        return steammock::slot<bool>(kCall_734, handle, bReturnKeyValueTags);
    }
    virtual bool SetReturnLongDescription(std::uint64_t handle, bool bReturnLongDescription) {
        return steammock::slot<bool>(kCall_735, handle, bReturnLongDescription);
    }
    virtual bool SetReturnMetadata(std::uint64_t handle, bool bReturnMetadata) {
        return steammock::slot<bool>(kCall_736, handle, bReturnMetadata);
    }
    virtual bool SetReturnChildren(std::uint64_t handle, bool bReturnChildren) {
        return steammock::slot<bool>(kCall_737, handle, bReturnChildren);
    }
    virtual bool SetReturnAdditionalPreviews(std::uint64_t handle, bool bReturnAdditionalPreviews) {
        return steammock::slot<bool>(kCall_738, handle, bReturnAdditionalPreviews);
    }
    virtual bool SetReturnTotalOnly(std::uint64_t handle, bool bReturnTotalOnly) {
        return steammock::slot<bool>(kCall_739, handle, bReturnTotalOnly);
    }
    virtual bool SetReturnPlaytimeStats(std::uint64_t handle, std::uint32_t unDays) {
        return steammock::slot<bool>(kCall_740, handle, unDays);
    }
    virtual bool SetLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steammock::slot<bool>(kCall_741, handle, pchLanguage);
    }
    virtual bool SetAllowCachedResponse(std::uint64_t handle, std::uint32_t unMaxAgeSeconds) {
        return steammock::slot<bool>(kCall_742, handle, unMaxAgeSeconds);
    }
    virtual bool SetCloudFileNameFilter(std::uint64_t handle, const char* pMatchCloudFileName) {
        return steammock::slot<bool>(kCall_743, handle, pMatchCloudFileName);
    }
    virtual bool SetMatchAnyTag(std::uint64_t handle, bool bMatchAnyTag) {
        return steammock::slot<bool>(kCall_744, handle, bMatchAnyTag);
    }
    virtual bool SetSearchText(std::uint64_t handle, const char* pSearchText) {
        return steammock::slot<bool>(kCall_745, handle, pSearchText);
    }
    virtual bool SetRankedByTrendDays(std::uint64_t handle, std::uint32_t unDays) {
        return steammock::slot<bool>(kCall_746, handle, unDays);
    }
    virtual bool SetTimeCreatedDateRange(std::uint64_t handle, std::uint32_t rtStart, std::uint32_t rtEnd) {
        return steammock::slot<bool>(kCall_799, handle, rtStart, rtEnd);
    }
    virtual bool SetTimeUpdatedDateRange(std::uint64_t handle, std::uint32_t rtStart, std::uint32_t rtEnd) {
        return steammock::slot<bool>(kCall_800, handle, rtStart, rtEnd);
    }
    virtual bool AddRequiredKeyValueTag(std::uint64_t handle, const char* pKey, const char* pValue) {
        return steammock::slot<bool>(kCall_747, handle, pKey, pValue);
    }
    virtual std::uint64_t RequestUGCDetails(std::uint64_t nPublishedFileID, std::uint32_t unMaxAgeSeconds) {
        return steammock::slot<std::uint64_t>(kCall_748, nPublishedFileID, unMaxAgeSeconds);
    }
    virtual std::uint64_t CreateItem(std::uint32_t nConsumerAppId, std::int32_t eFileType) {
        return steammock::slot<std::uint64_t>(kCall_749, nConsumerAppId, eFileType);
    }
    virtual std::uint64_t StartItemUpdate(std::uint32_t nConsumerAppId, std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_750, nConsumerAppId, nPublishedFileID);
    }
    virtual bool SetItemTitle(std::uint64_t handle, const char* pchTitle) {
        return steammock::slot<bool>(kCall_751, handle, pchTitle);
    }
    virtual bool SetItemDescription(std::uint64_t handle, const char* pchDescription) {
        return steammock::slot<bool>(kCall_752, handle, pchDescription);
    }
    virtual bool SetItemUpdateLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steammock::slot<bool>(kCall_753, handle, pchLanguage);
    }
    virtual bool SetItemMetadata(std::uint64_t handle, const char* pchMetaData) {
        return steammock::slot<bool>(kCall_754, handle, pchMetaData);
    }
    virtual bool SetItemVisibility(std::uint64_t handle, std::int32_t eVisibility) {
        return steammock::slot<bool>(kCall_755, handle, eVisibility);
    }
    virtual bool SetItemTags(std::uint64_t updateHandle, void* pTags) {
        return steammock::slot<bool>(kCall_756, updateHandle, pTags);
    }
    virtual bool SetItemContent(std::uint64_t handle, const char* pszContentFolder) {
        return steammock::slot<bool>(kCall_757, handle, pszContentFolder);
    }
    virtual bool SetItemPreview(std::uint64_t handle, const char* pszPreviewFile) {
        return steammock::slot<bool>(kCall_758, handle, pszPreviewFile);
    }
    virtual bool SetAllowLegacyUpload(std::uint64_t handle, bool bAllowLegacyUpload) {
        return steammock::slot<bool>(kCall_793, handle, bAllowLegacyUpload);
    }
    virtual bool RemoveAllItemKeyValueTags(std::uint64_t handle) {
        return steammock::slot<bool>(kCall_794, handle);
    }
    virtual bool RemoveItemKeyValueTags(std::uint64_t handle, const char* pchKey) {
        return steammock::slot<bool>(kCall_759, handle, pchKey);
    }
    virtual bool AddItemKeyValueTag(std::uint64_t handle, const char* pchKey, const char* pchValue) {
        return steammock::slot<bool>(kCall_760, handle, pchKey, pchValue);
    }
    virtual bool AddItemPreviewFile(std::uint64_t handle, const char* pszPreviewFile, std::int32_t type) {
        return steammock::slot<bool>(kCall_761, handle, pszPreviewFile, type);
    }
    virtual bool AddItemPreviewVideo(std::uint64_t handle, const char* pszVideoID) {
        return steammock::slot<bool>(kCall_762, handle, pszVideoID);
    }
    virtual bool UpdateItemPreviewFile(std::uint64_t handle, std::uint32_t index, const char* pszPreviewFile) {
        return steammock::slot<bool>(kCall_763, handle, index, pszPreviewFile);
    }
    virtual bool UpdateItemPreviewVideo(std::uint64_t handle, std::uint32_t index, const char* pszVideoID) {
        return steammock::slot<bool>(kCall_764, handle, index, pszVideoID);
    }
    virtual bool RemoveItemPreview(std::uint64_t handle, std::uint32_t index) {
        return steammock::slot<bool>(kCall_765, handle, index);
    }
    virtual std::uint64_t SubmitItemUpdate(std::uint64_t handle, const char* pchChangeNote) {
        return steammock::slot<std::uint64_t>(kCall_766, handle, pchChangeNote);
    }
    virtual std::int32_t GetItemUpdateProgress(std::uint64_t handle, std::uint64_t* punBytesProcessed, std::uint64_t* punBytesTotal) {
        return steammock::slot<std::int32_t>(kCall_767, handle, punBytesProcessed, punBytesTotal);
    }
    virtual std::uint64_t SetUserItemVote(std::uint64_t nPublishedFileID, bool bVoteUp) {
        return steammock::slot<std::uint64_t>(kCall_768, nPublishedFileID, bVoteUp);
    }
    virtual std::uint64_t GetUserItemVote(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_769, nPublishedFileID);
    }
    virtual std::uint64_t AddItemToFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_770, nAppId, nPublishedFileID);
    }
    virtual std::uint64_t RemoveItemFromFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_771, nAppId, nPublishedFileID);
    }
    virtual std::uint64_t SubscribeItem(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_772, nPublishedFileID);
    }
    virtual std::uint64_t UnsubscribeItem(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_773, nPublishedFileID);
    }
    virtual std::uint32_t GetNumSubscribedItems() {
        return steammock::slot<std::uint32_t>(kCall_774);
    }
    virtual std::uint32_t GetSubscribedItems(std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steammock::slot<std::uint32_t>(kCall_775, pvecPublishedFileID, cMaxEntries);
    }
    virtual std::uint32_t GetItemState(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint32_t>(kCall_776, nPublishedFileID);
    }
    virtual bool GetItemInstallInfo(std::uint64_t nPublishedFileID, std::uint64_t* punSizeOnDisk, void* pchFolder, std::uint32_t cchFolderSize, std::uint32_t* punTimeStamp) {
        return steammock::slot<bool>(kCall_777, nPublishedFileID, punSizeOnDisk, pchFolder, cchFolderSize, punTimeStamp);
    }
    virtual bool GetItemDownloadInfo(std::uint64_t nPublishedFileID, std::uint64_t* punBytesDownloaded, std::uint64_t* punBytesTotal) {
        return steammock::slot<bool>(kCall_778, nPublishedFileID, punBytesDownloaded, punBytesTotal);
    }
    virtual bool DownloadItem(std::uint64_t nPublishedFileID, bool bHighPriority) {
        return steammock::slot<bool>(kCall_779, nPublishedFileID, bHighPriority);
    }
    virtual bool BInitWorkshopForGameServer(std::uint32_t unWorkshopDepotID, const char* pszFolder) {
        return steammock::slot<bool>(kCall_780, unWorkshopDepotID, pszFolder);
    }
    virtual void SuspendDownloads(bool bSuspend) { steammock::slot<void>(kCall_781, bSuspend); }
    virtual std::uint64_t StartPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steammock::slot<std::uint64_t>(kCall_782, pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steammock::slot<std::uint64_t>(kCall_783, pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTrackingForAllItems() {
        return steammock::slot<std::uint64_t>(kCall_784);
    }
    virtual std::uint64_t AddDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_785, nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t RemoveDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_786, nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t AddAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steammock::slot<std::uint64_t>(kCall_787, nPublishedFileID, nAppID);
    }
    virtual std::uint64_t RemoveAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steammock::slot<std::uint64_t>(kCall_788, nPublishedFileID, nAppID);
    }
    virtual std::uint64_t GetAppDependencies(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_789, nPublishedFileID);
    }
    virtual std::uint64_t DeleteItem(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_790, nPublishedFileID);
    }
    virtual bool ShowWorkshopEULA() { return steammock::slot<bool>(kCall_801); }
    virtual std::uint64_t GetWorkshopEULAStatus() {
        return steammock::slot<std::uint64_t>(kCall_802);
    }
};

Version_STEAMUGC_INTERFACE_VERSION016 g_STEAMUGC_INTERFACE_VERSION016;

// ISteamUGC STEAMUGC_INTERFACE_VERSION017
class Version_STEAMUGC_INTERFACE_VERSION017 {
public:
    virtual std::uint64_t CreateQueryUserUGCRequest(std::uint32_t unAccountID, std::int32_t eListType, std::int32_t eMatchingUGCType, std::int32_t eSortOrder, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steammock::slot<std::uint64_t>(kCall_717, unAccountID, eListType, eMatchingUGCType, eSortOrder, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, std::uint32_t unPage) {
        return steammock::slot<std::uint64_t>(kCall_718, eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, unPage);
    }
    virtual std::uint64_t CreateQueryAllUGCRequest(std::int32_t eQueryType, std::int32_t eMatchingeMatchingUGCTypeFileType, std::uint32_t nCreatorAppID, std::uint32_t nConsumerAppID, const char* pchCursor) {
        return steammock::slot<std::uint64_t>(kCall_791, eQueryType, eMatchingeMatchingUGCTypeFileType, nCreatorAppID, nConsumerAppID, pchCursor);
    }
    virtual std::uint64_t CreateQueryUGCDetailsRequest(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steammock::slot<std::uint64_t>(kCall_719, pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t SendQueryUGCRequest(std::uint64_t handle) {
        return steammock::slot<std::uint64_t>(kCall_720, handle);
    }
    virtual bool GetQueryUGCResult(std::uint64_t handle, std::uint32_t index, void* pDetails) {
        return steammock::slot<bool>(kCall_721, handle, index, pDetails);
    }
    virtual std::uint32_t GetQueryUGCNumTags(std::uint64_t handle, std::uint32_t index) {
        return steammock::slot<std::uint32_t>(kCall_796, handle, index);
    }
    virtual bool GetQueryUGCTag(std::uint64_t handle, std::uint32_t index, std::uint32_t indexTag, void* pchValue, std::uint32_t cchValueSize) {
        return steammock::slot<bool>(kCall_797, handle, index, indexTag, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCTagDisplayName(std::uint64_t handle, std::uint32_t index, std::uint32_t indexTag, void* pchValue, std::uint32_t cchValueSize) {
        return steammock::slot<bool>(kCall_798, handle, index, indexTag, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCPreviewURL(std::uint64_t handle, std::uint32_t index, void* pchURL, std::uint32_t cchURLSize) {
        return steammock::slot<bool>(kCall_722, handle, index, pchURL, cchURLSize);
    }
    virtual bool GetQueryUGCMetadata(std::uint64_t handle, std::uint32_t index, void* pchMetadata, std::uint32_t cchMetadatasize) {
        return steammock::slot<bool>(kCall_723, handle, index, pchMetadata, cchMetadatasize);
    }
    virtual bool GetQueryUGCChildren(std::uint64_t handle, std::uint32_t index, std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steammock::slot<bool>(kCall_724, handle, index, pvecPublishedFileID, cMaxEntries);
    }
    virtual bool GetQueryUGCStatistic(std::uint64_t handle, std::uint32_t index, std::int32_t eStatType, std::uint64_t* pStatValue) {
        return steammock::slot<bool>(kCall_725, handle, index, eStatType, pStatValue);
    }
    virtual std::uint32_t GetQueryUGCNumAdditionalPreviews(std::uint64_t handle, std::uint32_t index) {
        return steammock::slot<std::uint32_t>(kCall_726, handle, index);
    }
    virtual bool GetQueryUGCAdditionalPreview(std::uint64_t handle, std::uint32_t index, std::uint32_t previewIndex, void* pchURLOrVideoID, std::uint32_t cchURLSize, void* pchOriginalFileName, std::uint32_t cchOriginalFileNameSize, std::int32_t* pPreviewType) {
        return steammock::slot<bool>(kCall_727, handle, index, previewIndex, pchURLOrVideoID, cchURLSize, pchOriginalFileName, cchOriginalFileNameSize, pPreviewType);
    }
    virtual std::uint32_t GetQueryUGCNumKeyValueTags(std::uint64_t handle, std::uint32_t index) {
        return steammock::slot<std::uint32_t>(kCall_728, handle, index);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, std::uint32_t keyValueTagIndex, void* pchKey, std::uint32_t cchKeySize, void* pchValue, std::uint32_t cchValueSize) {
        return steammock::slot<bool>(kCall_729, handle, index, keyValueTagIndex, pchKey, cchKeySize, pchValue, cchValueSize);
    }
    virtual bool GetQueryUGCKeyValueTag(std::uint64_t handle, std::uint32_t index, const char* pchKey, void* pchValue, std::uint32_t cchValueSize) {
        return steammock::slot<bool>(kCall_792, handle, index, pchKey, pchValue, cchValueSize);
    }
    virtual std::uint32_t GetQueryUGCContentDescriptors(std::uint64_t handle, std::uint32_t index, std::int32_t* pvecDescriptors, std::uint32_t cMaxEntries) {
        return steammock::slot<std::uint32_t>(kCall_803, handle, index, pvecDescriptors, cMaxEntries);
    }
    virtual bool ReleaseQueryUGCRequest(std::uint64_t handle) {
        return steammock::slot<bool>(kCall_730, handle);
    }
    virtual bool AddRequiredTag(std::uint64_t handle, const char* pTagName) {
        return steammock::slot<bool>(kCall_731, handle, pTagName);
    }
    virtual bool AddRequiredTagGroup(std::uint64_t handle, void* pTagGroups) {
        return steammock::slot<bool>(kCall_795, handle, pTagGroups);
    }
    virtual bool AddExcludedTag(std::uint64_t handle, const char* pTagName) {
        return steammock::slot<bool>(kCall_732, handle, pTagName);
    }
    virtual bool SetReturnOnlyIDs(std::uint64_t handle, bool bReturnOnlyIDs) {
        return steammock::slot<bool>(kCall_733, handle, bReturnOnlyIDs);
    }
    virtual bool SetReturnKeyValueTags(std::uint64_t handle, bool bReturnKeyValueTags) {
        return steammock::slot<bool>(kCall_734, handle, bReturnKeyValueTags);
    }
    virtual bool SetReturnLongDescription(std::uint64_t handle, bool bReturnLongDescription) {
        return steammock::slot<bool>(kCall_735, handle, bReturnLongDescription);
    }
    virtual bool SetReturnMetadata(std::uint64_t handle, bool bReturnMetadata) {
        return steammock::slot<bool>(kCall_736, handle, bReturnMetadata);
    }
    virtual bool SetReturnChildren(std::uint64_t handle, bool bReturnChildren) {
        return steammock::slot<bool>(kCall_737, handle, bReturnChildren);
    }
    virtual bool SetReturnAdditionalPreviews(std::uint64_t handle, bool bReturnAdditionalPreviews) {
        return steammock::slot<bool>(kCall_738, handle, bReturnAdditionalPreviews);
    }
    virtual bool SetReturnTotalOnly(std::uint64_t handle, bool bReturnTotalOnly) {
        return steammock::slot<bool>(kCall_739, handle, bReturnTotalOnly);
    }
    virtual bool SetReturnPlaytimeStats(std::uint64_t handle, std::uint32_t unDays) {
        return steammock::slot<bool>(kCall_740, handle, unDays);
    }
    virtual bool SetLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steammock::slot<bool>(kCall_741, handle, pchLanguage);
    }
    virtual bool SetAllowCachedResponse(std::uint64_t handle, std::uint32_t unMaxAgeSeconds) {
        return steammock::slot<bool>(kCall_742, handle, unMaxAgeSeconds);
    }
    virtual bool SetCloudFileNameFilter(std::uint64_t handle, const char* pMatchCloudFileName) {
        return steammock::slot<bool>(kCall_743, handle, pMatchCloudFileName);
    }
    virtual bool SetMatchAnyTag(std::uint64_t handle, bool bMatchAnyTag) {
        return steammock::slot<bool>(kCall_744, handle, bMatchAnyTag);
    }
    virtual bool SetSearchText(std::uint64_t handle, const char* pSearchText) {
        return steammock::slot<bool>(kCall_745, handle, pSearchText);
    }
    virtual bool SetRankedByTrendDays(std::uint64_t handle, std::uint32_t unDays) {
        return steammock::slot<bool>(kCall_746, handle, unDays);
    }
    virtual bool SetTimeCreatedDateRange(std::uint64_t handle, std::uint32_t rtStart, std::uint32_t rtEnd) {
        return steammock::slot<bool>(kCall_799, handle, rtStart, rtEnd);
    }
    virtual bool SetTimeUpdatedDateRange(std::uint64_t handle, std::uint32_t rtStart, std::uint32_t rtEnd) {
        return steammock::slot<bool>(kCall_800, handle, rtStart, rtEnd);
    }
    virtual bool AddRequiredKeyValueTag(std::uint64_t handle, const char* pKey, const char* pValue) {
        return steammock::slot<bool>(kCall_747, handle, pKey, pValue);
    }
    virtual std::uint64_t RequestUGCDetails(std::uint64_t nPublishedFileID, std::uint32_t unMaxAgeSeconds) {
        return steammock::slot<std::uint64_t>(kCall_748, nPublishedFileID, unMaxAgeSeconds);
    }
    virtual std::uint64_t CreateItem(std::uint32_t nConsumerAppId, std::int32_t eFileType) {
        return steammock::slot<std::uint64_t>(kCall_749, nConsumerAppId, eFileType);
    }
    virtual std::uint64_t StartItemUpdate(std::uint32_t nConsumerAppId, std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_750, nConsumerAppId, nPublishedFileID);
    }
    virtual bool SetItemTitle(std::uint64_t handle, const char* pchTitle) {
        return steammock::slot<bool>(kCall_751, handle, pchTitle);
    }
    virtual bool SetItemDescription(std::uint64_t handle, const char* pchDescription) {
        return steammock::slot<bool>(kCall_752, handle, pchDescription);
    }
    virtual bool SetItemUpdateLanguage(std::uint64_t handle, const char* pchLanguage) {
        return steammock::slot<bool>(kCall_753, handle, pchLanguage);
    }
    virtual bool SetItemMetadata(std::uint64_t handle, const char* pchMetaData) {
        return steammock::slot<bool>(kCall_754, handle, pchMetaData);
    }
    virtual bool SetItemVisibility(std::uint64_t handle, std::int32_t eVisibility) {
        return steammock::slot<bool>(kCall_755, handle, eVisibility);
    }
    virtual bool SetItemTags(std::uint64_t updateHandle, void* pTags) {
        return steammock::slot<bool>(kCall_756, updateHandle, pTags);
    }
    virtual bool SetItemContent(std::uint64_t handle, const char* pszContentFolder) {
        return steammock::slot<bool>(kCall_757, handle, pszContentFolder);
    }
    virtual bool SetItemPreview(std::uint64_t handle, const char* pszPreviewFile) {
        return steammock::slot<bool>(kCall_758, handle, pszPreviewFile);
    }
    virtual bool SetAllowLegacyUpload(std::uint64_t handle, bool bAllowLegacyUpload) {
        return steammock::slot<bool>(kCall_793, handle, bAllowLegacyUpload);
    }
    virtual bool RemoveAllItemKeyValueTags(std::uint64_t handle) {
        return steammock::slot<bool>(kCall_794, handle);
    }
    virtual bool RemoveItemKeyValueTags(std::uint64_t handle, const char* pchKey) {
        return steammock::slot<bool>(kCall_759, handle, pchKey);
    }
    virtual bool AddItemKeyValueTag(std::uint64_t handle, const char* pchKey, const char* pchValue) {
        return steammock::slot<bool>(kCall_760, handle, pchKey, pchValue);
    }
    virtual bool AddItemPreviewFile(std::uint64_t handle, const char* pszPreviewFile, std::int32_t type) {
        return steammock::slot<bool>(kCall_761, handle, pszPreviewFile, type);
    }
    virtual bool AddItemPreviewVideo(std::uint64_t handle, const char* pszVideoID) {
        return steammock::slot<bool>(kCall_762, handle, pszVideoID);
    }
    virtual bool UpdateItemPreviewFile(std::uint64_t handle, std::uint32_t index, const char* pszPreviewFile) {
        return steammock::slot<bool>(kCall_763, handle, index, pszPreviewFile);
    }
    virtual bool UpdateItemPreviewVideo(std::uint64_t handle, std::uint32_t index, const char* pszVideoID) {
        return steammock::slot<bool>(kCall_764, handle, index, pszVideoID);
    }
    virtual bool RemoveItemPreview(std::uint64_t handle, std::uint32_t index) {
        return steammock::slot<bool>(kCall_765, handle, index);
    }
    virtual bool AddContentDescriptor(std::uint64_t handle, std::int32_t descid) {
        return steammock::slot<bool>(kCall_804, handle, descid);
    }
    virtual bool RemoveContentDescriptor(std::uint64_t handle, std::int32_t descid) {
        return steammock::slot<bool>(kCall_805, handle, descid);
    }
    virtual std::uint64_t SubmitItemUpdate(std::uint64_t handle, const char* pchChangeNote) {
        return steammock::slot<std::uint64_t>(kCall_766, handle, pchChangeNote);
    }
    virtual std::int32_t GetItemUpdateProgress(std::uint64_t handle, std::uint64_t* punBytesProcessed, std::uint64_t* punBytesTotal) {
        return steammock::slot<std::int32_t>(kCall_767, handle, punBytesProcessed, punBytesTotal);
    }
    virtual std::uint64_t SetUserItemVote(std::uint64_t nPublishedFileID, bool bVoteUp) {
        return steammock::slot<std::uint64_t>(kCall_768, nPublishedFileID, bVoteUp);
    }
    virtual std::uint64_t GetUserItemVote(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_769, nPublishedFileID);
    }
    virtual std::uint64_t AddItemToFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_770, nAppId, nPublishedFileID);
    }
    virtual std::uint64_t RemoveItemFromFavorites(std::uint32_t nAppId, std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_771, nAppId, nPublishedFileID);
    }
    virtual std::uint64_t SubscribeItem(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_772, nPublishedFileID);
    }
    virtual std::uint64_t UnsubscribeItem(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_773, nPublishedFileID);
    }
    virtual std::uint32_t GetNumSubscribedItems() {
        return steammock::slot<std::uint32_t>(kCall_774);
    }
    virtual std::uint32_t GetSubscribedItems(std::uint64_t* pvecPublishedFileID, std::uint32_t cMaxEntries) {
        return steammock::slot<std::uint32_t>(kCall_775, pvecPublishedFileID, cMaxEntries);
    }
    virtual std::uint32_t GetItemState(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint32_t>(kCall_776, nPublishedFileID);
    }
    virtual bool GetItemInstallInfo(std::uint64_t nPublishedFileID, std::uint64_t* punSizeOnDisk, void* pchFolder, std::uint32_t cchFolderSize, std::uint32_t* punTimeStamp) {
        return steammock::slot<bool>(kCall_777, nPublishedFileID, punSizeOnDisk, pchFolder, cchFolderSize, punTimeStamp);
    }
    virtual bool GetItemDownloadInfo(std::uint64_t nPublishedFileID, std::uint64_t* punBytesDownloaded, std::uint64_t* punBytesTotal) {
        return steammock::slot<bool>(kCall_778, nPublishedFileID, punBytesDownloaded, punBytesTotal);
    }
    virtual bool DownloadItem(std::uint64_t nPublishedFileID, bool bHighPriority) {
        return steammock::slot<bool>(kCall_779, nPublishedFileID, bHighPriority);
    }
    virtual bool BInitWorkshopForGameServer(std::uint32_t unWorkshopDepotID, const char* pszFolder) {
        return steammock::slot<bool>(kCall_780, unWorkshopDepotID, pszFolder);
    }
    virtual void SuspendDownloads(bool bSuspend) { steammock::slot<void>(kCall_781, bSuspend); }
    virtual std::uint64_t StartPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steammock::slot<std::uint64_t>(kCall_782, pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTracking(std::uint64_t* pvecPublishedFileID, std::uint32_t unNumPublishedFileIDs) {
        return steammock::slot<std::uint64_t>(kCall_783, pvecPublishedFileID, unNumPublishedFileIDs);
    }
    virtual std::uint64_t StopPlaytimeTrackingForAllItems() {
        return steammock::slot<std::uint64_t>(kCall_784);
    }
    virtual std::uint64_t AddDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_785, nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t RemoveDependency(std::uint64_t nParentPublishedFileID, std::uint64_t nChildPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_786, nParentPublishedFileID, nChildPublishedFileID);
    }
    virtual std::uint64_t AddAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steammock::slot<std::uint64_t>(kCall_787, nPublishedFileID, nAppID);
    }
    virtual std::uint64_t RemoveAppDependency(std::uint64_t nPublishedFileID, std::uint32_t nAppID) {
        return steammock::slot<std::uint64_t>(kCall_788, nPublishedFileID, nAppID);
    }
    virtual std::uint64_t GetAppDependencies(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_789, nPublishedFileID);
    }
    virtual std::uint64_t DeleteItem(std::uint64_t nPublishedFileID) {
        return steammock::slot<std::uint64_t>(kCall_790, nPublishedFileID);
    }
    virtual bool ShowWorkshopEULA() { return steammock::slot<bool>(kCall_801); }
    virtual std::uint64_t GetWorkshopEULAStatus() {
        return steammock::slot<std::uint64_t>(kCall_802);
    }
};

Version_STEAMUGC_INTERFACE_VERSION017 g_STEAMUGC_INTERFACE_VERSION017;

// ISteamUnifiedMessages STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001
class Version_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001 {
public:
    virtual std::uint64_t SendMethod(const char* pchServiceMethod, void* pRequestBuffer, std::uint32_t unRequestBufferSize, std::uint64_t unContext) {
        return steammock::slot<std::uint64_t>(kCall_806, pchServiceMethod, pRequestBuffer, unRequestBufferSize, unContext);
    }
    virtual bool GetMethodResponseInfo(std::uint64_t hHandle, std::uint32_t* punResponseSize, std::int32_t* peResult) {
        return steammock::slot<bool>(kCall_807, hHandle, punResponseSize, peResult);
    }
    virtual bool GetMethodResponseData(std::uint64_t hHandle, void* pResponseBuffer, std::uint32_t unResponseBufferSize, bool bAutoRelease) {
        return steammock::slot<bool>(kCall_808, hHandle, pResponseBuffer, unResponseBufferSize, bAutoRelease);
    }
    virtual bool ReleaseMethod(std::uint64_t hHandle) {
        return steammock::slot<bool>(kCall_809, hHandle);
    }
    virtual bool SendNotification(const char* pchServiceNotification, void* pNotificationBuffer, std::uint32_t unNotificationBufferSize) {
        return steammock::slot<bool>(kCall_810, pchServiceNotification, pNotificationBuffer, unNotificationBufferSize);
    }
};

Version_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001 g_STEAMUNIFIEDMESSAGES_INTERFACE_VERSION001;

// ISteamUser SteamUser019
class Version_SteamUser019 {
public:
    virtual std::int32_t GetHSteamUser() { return steammock::slot<std::int32_t>(kCall_811); }
    virtual bool BLoggedOn() { return steammock::slot<bool>(kCall_812); }
    virtual CSteamID GetSteamID() { return steammock::slot<CSteamID>(kCall_813); }
    virtual std::int32_t InitiateGameConnection(void* pAuthBlob, std::int32_t cbMaxAuthBlob, CSteamID steamIDGameServer, std::uint32_t unIPServer, std::uint16_t usPortServer, bool bSecure) {
        return steammock::slot<std::int32_t>(kCall_814, pAuthBlob, cbMaxAuthBlob, steamIDGameServer, unIPServer, usPortServer, bSecure);
    }
    virtual void TerminateGameConnection(std::uint32_t unIPServer, std::uint16_t usPortServer) {
        steammock::slot<void>(kCall_815, unIPServer, usPortServer);
    }
    virtual void TrackAppUsageEvent(CGameID gameID, std::int32_t eAppUsageEvent, const char* pchExtraInfo) {
        steammock::slot<void>(kCall_816, gameID, eAppUsageEvent, pchExtraInfo);
    }
    virtual bool GetUserDataFolder(void* pchBuffer, std::int32_t cubBuffer) {
        return steammock::slot<bool>(kCall_817, pchBuffer, cubBuffer);
    }
    virtual void StartVoiceRecording() { steammock::slot<void>(kCall_818); }
    virtual void StopVoiceRecording() { steammock::slot<void>(kCall_819); }
    virtual std::int32_t GetAvailableVoice(std::uint32_t* pcbCompressed, std::uint32_t* pcbUncompressed_Deprecated, std::uint32_t nUncompressedVoiceDesiredSampleRate_Deprecated) {
        return steammock::slot<std::int32_t>(kCall_820, pcbCompressed, pcbUncompressed_Deprecated, nUncompressedVoiceDesiredSampleRate_Deprecated);
    }
    virtual std::int32_t GetVoice(bool bWantCompressed, void* pDestBuffer, std::uint32_t cbDestBufferSize, std::uint32_t* nBytesWritten, bool bWantUncompressed_Deprecated, void* pUncompressedDestBuffer_Deprecated, std::uint32_t cbUncompressedDestBufferSize_Deprecated, std::uint32_t* nUncompressBytesWritten_Deprecated, std::uint32_t nUncompressedVoiceDesiredSampleRate_Deprecated) {
        return steammock::slot<std::int32_t>(kCall_821, bWantCompressed, pDestBuffer, cbDestBufferSize, nBytesWritten, bWantUncompressed_Deprecated, pUncompressedDestBuffer_Deprecated, cbUncompressedDestBufferSize_Deprecated, nUncompressBytesWritten_Deprecated, nUncompressedVoiceDesiredSampleRate_Deprecated);
    }
    virtual std::int32_t DecompressVoice(void* pCompressed, std::uint32_t cbCompressed, void* pDestBuffer, std::uint32_t cbDestBufferSize, std::uint32_t* nBytesWritten, std::uint32_t nDesiredSampleRate) {
        return steammock::slot<std::int32_t>(kCall_822, pCompressed, cbCompressed, pDestBuffer, cbDestBufferSize, nBytesWritten, nDesiredSampleRate);
    }
    virtual std::uint32_t GetVoiceOptimalSampleRate() {
        return steammock::slot<std::uint32_t>(kCall_823);
    }
    virtual std::uint32_t GetAuthSessionTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket) {
        return steammock::slot<std::uint32_t>(kCall_824, pTicket, cbMaxTicket, pcbTicket);
    }
    virtual std::int32_t BeginAuthSession(void* pAuthTicket, std::int32_t cbAuthTicket, CSteamID steamID) {
        return steammock::slot<std::int32_t>(kCall_825, pAuthTicket, cbAuthTicket, steamID);
    }
    virtual void EndAuthSession(CSteamID steamID) { steammock::slot<void>(kCall_826, steamID); }
    virtual void CancelAuthTicket(std::uint32_t hAuthTicket) {
        steammock::slot<void>(kCall_827, hAuthTicket);
    }
    virtual std::int32_t UserHasLicenseForApp(CSteamID steamID, std::uint32_t appID) {
        return steammock::slot<std::int32_t>(kCall_828, steamID, appID);
    }
    virtual bool BIsBehindNAT() { return steammock::slot<bool>(kCall_829); }
    virtual void AdvertiseGame(CSteamID steamIDGameServer, std::uint32_t unIPServer, std::uint16_t usPortServer) {
        steammock::slot<void>(kCall_830, steamIDGameServer, unIPServer, usPortServer);
    }
    virtual std::uint64_t RequestEncryptedAppTicket(void* pDataToInclude, std::int32_t cbDataToInclude) {
        return steammock::slot<std::uint64_t>(kCall_831, pDataToInclude, cbDataToInclude);
    }
    virtual bool GetEncryptedAppTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket) {
        return steammock::slot<bool>(kCall_832, pTicket, cbMaxTicket, pcbTicket);
    }
    virtual std::int32_t GetGameBadgeLevel(std::int32_t nSeries, bool bFoil) {
        return steammock::slot<std::int32_t>(kCall_833, nSeries, bFoil);
    }
    virtual std::int32_t GetPlayerSteamLevel() { return steammock::slot<std::int32_t>(kCall_834); }
    virtual std::uint64_t RequestStoreAuthURL(const char* pchRedirectURL) {
        return steammock::slot<std::uint64_t>(kCall_835, pchRedirectURL);
    }
    virtual bool BIsPhoneVerified() { return steammock::slot<bool>(kCall_836); }
    virtual bool BIsTwoFactorEnabled() { return steammock::slot<bool>(kCall_837); }
    virtual bool BIsPhoneIdentifying() { return steammock::slot<bool>(kCall_838); }
    virtual bool BIsPhoneRequiringVerification() { return steammock::slot<bool>(kCall_839); }
};

Version_SteamUser019 g_SteamUser019;

// ISteamUser SteamUser020
class Version_SteamUser020 {
public:
    virtual std::int32_t GetHSteamUser() { return steammock::slot<std::int32_t>(kCall_811); }
    virtual bool BLoggedOn() { return steammock::slot<bool>(kCall_812); }
    virtual CSteamID GetSteamID() { return steammock::slot<CSteamID>(kCall_813); }
    virtual std::int32_t InitiateGameConnection(void* pAuthBlob, std::int32_t cbMaxAuthBlob, CSteamID steamIDGameServer, std::uint32_t unIPServer, std::uint16_t usPortServer, bool bSecure) {
        return steammock::slot<std::int32_t>(kCall_814, pAuthBlob, cbMaxAuthBlob, steamIDGameServer, unIPServer, usPortServer, bSecure);
    }
    virtual void TerminateGameConnection(std::uint32_t unIPServer, std::uint16_t usPortServer) {
        steammock::slot<void>(kCall_815, unIPServer, usPortServer);
    }
    virtual void TrackAppUsageEvent(CGameID gameID, std::int32_t eAppUsageEvent, const char* pchExtraInfo) {
        steammock::slot<void>(kCall_816, gameID, eAppUsageEvent, pchExtraInfo);
    }
    virtual bool GetUserDataFolder(void* pchBuffer, std::int32_t cubBuffer) {
        return steammock::slot<bool>(kCall_817, pchBuffer, cubBuffer);
    }
    virtual void StartVoiceRecording() { steammock::slot<void>(kCall_818); }
    virtual void StopVoiceRecording() { steammock::slot<void>(kCall_819); }
    virtual std::int32_t GetAvailableVoice(std::uint32_t* pcbCompressed, std::uint32_t* pcbUncompressed_Deprecated, std::uint32_t nUncompressedVoiceDesiredSampleRate_Deprecated) {
        return steammock::slot<std::int32_t>(kCall_820, pcbCompressed, pcbUncompressed_Deprecated, nUncompressedVoiceDesiredSampleRate_Deprecated);
    }
    virtual std::int32_t GetVoice(bool bWantCompressed, void* pDestBuffer, std::uint32_t cbDestBufferSize, std::uint32_t* nBytesWritten, bool bWantUncompressed_Deprecated, void* pUncompressedDestBuffer_Deprecated, std::uint32_t cbUncompressedDestBufferSize_Deprecated, std::uint32_t* nUncompressBytesWritten_Deprecated, std::uint32_t nUncompressedVoiceDesiredSampleRate_Deprecated) {
        return steammock::slot<std::int32_t>(kCall_821, bWantCompressed, pDestBuffer, cbDestBufferSize, nBytesWritten, bWantUncompressed_Deprecated, pUncompressedDestBuffer_Deprecated, cbUncompressedDestBufferSize_Deprecated, nUncompressBytesWritten_Deprecated, nUncompressedVoiceDesiredSampleRate_Deprecated);
    }
    virtual std::int32_t DecompressVoice(void* pCompressed, std::uint32_t cbCompressed, void* pDestBuffer, std::uint32_t cbDestBufferSize, std::uint32_t* nBytesWritten, std::uint32_t nDesiredSampleRate) {
        return steammock::slot<std::int32_t>(kCall_822, pCompressed, cbCompressed, pDestBuffer, cbDestBufferSize, nBytesWritten, nDesiredSampleRate);
    }
    virtual std::uint32_t GetVoiceOptimalSampleRate() {
        return steammock::slot<std::uint32_t>(kCall_823);
    }
    virtual std::uint32_t GetAuthSessionTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket) {
        return steammock::slot<std::uint32_t>(kCall_824, pTicket, cbMaxTicket, pcbTicket);
    }
    virtual std::int32_t BeginAuthSession(void* pAuthTicket, std::int32_t cbAuthTicket, CSteamID steamID) {
        return steammock::slot<std::int32_t>(kCall_825, pAuthTicket, cbAuthTicket, steamID);
    }
    virtual void EndAuthSession(CSteamID steamID) { steammock::slot<void>(kCall_826, steamID); }
    virtual void CancelAuthTicket(std::uint32_t hAuthTicket) {
        steammock::slot<void>(kCall_827, hAuthTicket);
    }
    virtual std::int32_t UserHasLicenseForApp(CSteamID steamID, std::uint32_t appID) {
        return steammock::slot<std::int32_t>(kCall_828, steamID, appID);
    }
    virtual bool BIsBehindNAT() { return steammock::slot<bool>(kCall_829); }
    virtual void AdvertiseGame(CSteamID steamIDGameServer, std::uint32_t unIPServer, std::uint16_t usPortServer) {
        steammock::slot<void>(kCall_830, steamIDGameServer, unIPServer, usPortServer);
    }
    virtual std::uint64_t RequestEncryptedAppTicket(void* pDataToInclude, std::int32_t cbDataToInclude) {
        return steammock::slot<std::uint64_t>(kCall_831, pDataToInclude, cbDataToInclude);
    }
    virtual bool GetEncryptedAppTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket) {
        return steammock::slot<bool>(kCall_832, pTicket, cbMaxTicket, pcbTicket);
    }
    virtual std::int32_t GetGameBadgeLevel(std::int32_t nSeries, bool bFoil) {
        return steammock::slot<std::int32_t>(kCall_833, nSeries, bFoil);
    }
    virtual std::int32_t GetPlayerSteamLevel() { return steammock::slot<std::int32_t>(kCall_834); }
    virtual std::uint64_t RequestStoreAuthURL(const char* pchRedirectURL) {
        return steammock::slot<std::uint64_t>(kCall_835, pchRedirectURL);
    }
    virtual bool BIsPhoneVerified() { return steammock::slot<bool>(kCall_836); }
    virtual bool BIsTwoFactorEnabled() { return steammock::slot<bool>(kCall_837); }
    virtual bool BIsPhoneIdentifying() { return steammock::slot<bool>(kCall_838); }
    virtual bool BIsPhoneRequiringVerification() { return steammock::slot<bool>(kCall_839); }
    virtual std::uint64_t GetMarketEligibility() {
        return steammock::slot<std::uint64_t>(kCall_840);
    }
    virtual std::uint64_t GetDurationControl() { return steammock::slot<std::uint64_t>(kCall_841); }
};

Version_SteamUser020 g_SteamUser020;

// ISteamUser SteamUser021
class Version_SteamUser021 {
public:
    virtual std::int32_t GetHSteamUser() { return steammock::slot<std::int32_t>(kCall_811); }
    virtual bool BLoggedOn() { return steammock::slot<bool>(kCall_812); }
    virtual CSteamID GetSteamID() { return steammock::slot<CSteamID>(kCall_813); }
    virtual std::int32_t InitiateGameConnection_DEPRECATED(void* pAuthBlob, std::int32_t cbMaxAuthBlob, CSteamID steamIDGameServer, std::uint32_t unIPServer, std::uint16_t usPortServer, bool bSecure) {
        return steammock::slot<std::int32_t>(kCall_842, pAuthBlob, cbMaxAuthBlob, steamIDGameServer, unIPServer, usPortServer, bSecure);
    }
    virtual void TerminateGameConnection_DEPRECATED(std::uint32_t unIPServer, std::uint16_t usPortServer) {
        steammock::slot<void>(kCall_843, unIPServer, usPortServer);
    }
    virtual void TrackAppUsageEvent(CGameID gameID, std::int32_t eAppUsageEvent, const char* pchExtraInfo) {
        steammock::slot<void>(kCall_816, gameID, eAppUsageEvent, pchExtraInfo);
    }
    virtual bool GetUserDataFolder(void* pchBuffer, std::int32_t cubBuffer) {
        return steammock::slot<bool>(kCall_817, pchBuffer, cubBuffer);
    }
    virtual void StartVoiceRecording() { steammock::slot<void>(kCall_818); }
    virtual void StopVoiceRecording() { steammock::slot<void>(kCall_819); }
    virtual std::int32_t GetAvailableVoice(std::uint32_t* pcbCompressed, std::uint32_t* pcbUncompressed_Deprecated, std::uint32_t nUncompressedVoiceDesiredSampleRate_Deprecated) {
        return steammock::slot<std::int32_t>(kCall_820, pcbCompressed, pcbUncompressed_Deprecated, nUncompressedVoiceDesiredSampleRate_Deprecated);
    }
    virtual std::int32_t GetVoice(bool bWantCompressed, void* pDestBuffer, std::uint32_t cbDestBufferSize, std::uint32_t* nBytesWritten, bool bWantUncompressed_Deprecated, void* pUncompressedDestBuffer_Deprecated, std::uint32_t cbUncompressedDestBufferSize_Deprecated, std::uint32_t* nUncompressBytesWritten_Deprecated, std::uint32_t nUncompressedVoiceDesiredSampleRate_Deprecated) {
        return steammock::slot<std::int32_t>(kCall_821, bWantCompressed, pDestBuffer, cbDestBufferSize, nBytesWritten, bWantUncompressed_Deprecated, pUncompressedDestBuffer_Deprecated, cbUncompressedDestBufferSize_Deprecated, nUncompressBytesWritten_Deprecated, nUncompressedVoiceDesiredSampleRate_Deprecated);
    }
    virtual std::int32_t DecompressVoice(void* pCompressed, std::uint32_t cbCompressed, void* pDestBuffer, std::uint32_t cbDestBufferSize, std::uint32_t* nBytesWritten, std::uint32_t nDesiredSampleRate) {
        return steammock::slot<std::int32_t>(kCall_822, pCompressed, cbCompressed, pDestBuffer, cbDestBufferSize, nBytesWritten, nDesiredSampleRate);
    }
    virtual std::uint32_t GetVoiceOptimalSampleRate() {
        return steammock::slot<std::uint32_t>(kCall_823);
    }
    virtual std::uint32_t GetAuthSessionTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket) {
        return steammock::slot<std::uint32_t>(kCall_824, pTicket, cbMaxTicket, pcbTicket);
    }
    virtual std::int32_t BeginAuthSession(void* pAuthTicket, std::int32_t cbAuthTicket, CSteamID steamID) {
        return steammock::slot<std::int32_t>(kCall_825, pAuthTicket, cbAuthTicket, steamID);
    }
    virtual void EndAuthSession(CSteamID steamID) { steammock::slot<void>(kCall_826, steamID); }
    virtual void CancelAuthTicket(std::uint32_t hAuthTicket) {
        steammock::slot<void>(kCall_827, hAuthTicket);
    }
    virtual std::int32_t UserHasLicenseForApp(CSteamID steamID, std::uint32_t appID) {
        return steammock::slot<std::int32_t>(kCall_828, steamID, appID);
    }
    virtual bool BIsBehindNAT() { return steammock::slot<bool>(kCall_829); }
    virtual void AdvertiseGame(CSteamID steamIDGameServer, std::uint32_t unIPServer, std::uint16_t usPortServer) {
        steammock::slot<void>(kCall_830, steamIDGameServer, unIPServer, usPortServer);
    }
    virtual std::uint64_t RequestEncryptedAppTicket(void* pDataToInclude, std::int32_t cbDataToInclude) {
        return steammock::slot<std::uint64_t>(kCall_831, pDataToInclude, cbDataToInclude);
    }
    virtual bool GetEncryptedAppTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket) {
        return steammock::slot<bool>(kCall_832, pTicket, cbMaxTicket, pcbTicket);
    }
    virtual std::int32_t GetGameBadgeLevel(std::int32_t nSeries, bool bFoil) {
        return steammock::slot<std::int32_t>(kCall_833, nSeries, bFoil);
    }
    virtual std::int32_t GetPlayerSteamLevel() { return steammock::slot<std::int32_t>(kCall_834); }
    virtual std::uint64_t RequestStoreAuthURL(const char* pchRedirectURL) {
        return steammock::slot<std::uint64_t>(kCall_835, pchRedirectURL);
    }
    virtual bool BIsPhoneVerified() { return steammock::slot<bool>(kCall_836); }
    virtual bool BIsTwoFactorEnabled() { return steammock::slot<bool>(kCall_837); }
    virtual bool BIsPhoneIdentifying() { return steammock::slot<bool>(kCall_838); }
    virtual bool BIsPhoneRequiringVerification() { return steammock::slot<bool>(kCall_839); }
    virtual std::uint64_t GetMarketEligibility() {
        return steammock::slot<std::uint64_t>(kCall_840);
    }
    virtual std::uint64_t GetDurationControl() { return steammock::slot<std::uint64_t>(kCall_841); }
    virtual bool BSetDurationControlOnlineState(std::int32_t eNewState) {
        return steammock::slot<bool>(kCall_844, eNewState);
    }
};

Version_SteamUser021 g_SteamUser021;

// ISteamUser SteamUser023
class Version_SteamUser023 {
public:
    virtual std::int32_t GetHSteamUser() { return steammock::slot<std::int32_t>(kCall_811); }
    virtual bool BLoggedOn() { return steammock::slot<bool>(kCall_812); }
    virtual CSteamID GetSteamID() { return steammock::slot<CSteamID>(kCall_813); }
    virtual std::int32_t InitiateGameConnection_DEPRECATED(void* pAuthBlob, std::int32_t cbMaxAuthBlob, CSteamID steamIDGameServer, std::uint32_t unIPServer, std::uint16_t usPortServer, bool bSecure) {
        return steammock::slot<std::int32_t>(kCall_842, pAuthBlob, cbMaxAuthBlob, steamIDGameServer, unIPServer, usPortServer, bSecure);
    }
    virtual void TerminateGameConnection_DEPRECATED(std::uint32_t unIPServer, std::uint16_t usPortServer) {
        steammock::slot<void>(kCall_843, unIPServer, usPortServer);
    }
    virtual void TrackAppUsageEvent(CGameID gameID, std::int32_t eAppUsageEvent, const char* pchExtraInfo) {
        steammock::slot<void>(kCall_816, gameID, eAppUsageEvent, pchExtraInfo);
    }
    virtual bool GetUserDataFolder(void* pchBuffer, std::int32_t cubBuffer) {
        return steammock::slot<bool>(kCall_817, pchBuffer, cubBuffer);
    }
    virtual void StartVoiceRecording() { steammock::slot<void>(kCall_818); }
    virtual void StopVoiceRecording() { steammock::slot<void>(kCall_819); }
    virtual std::int32_t GetAvailableVoice(std::uint32_t* pcbCompressed, std::uint32_t* pcbUncompressed_Deprecated, std::uint32_t nUncompressedVoiceDesiredSampleRate_Deprecated) {
        return steammock::slot<std::int32_t>(kCall_820, pcbCompressed, pcbUncompressed_Deprecated, nUncompressedVoiceDesiredSampleRate_Deprecated);
    }
    virtual std::int32_t GetVoice(bool bWantCompressed, void* pDestBuffer, std::uint32_t cbDestBufferSize, std::uint32_t* nBytesWritten, bool bWantUncompressed_Deprecated, void* pUncompressedDestBuffer_Deprecated, std::uint32_t cbUncompressedDestBufferSize_Deprecated, std::uint32_t* nUncompressBytesWritten_Deprecated, std::uint32_t nUncompressedVoiceDesiredSampleRate_Deprecated) {
        return steammock::slot<std::int32_t>(kCall_821, bWantCompressed, pDestBuffer, cbDestBufferSize, nBytesWritten, bWantUncompressed_Deprecated, pUncompressedDestBuffer_Deprecated, cbUncompressedDestBufferSize_Deprecated, nUncompressBytesWritten_Deprecated, nUncompressedVoiceDesiredSampleRate_Deprecated);
    }
    virtual std::int32_t DecompressVoice(void* pCompressed, std::uint32_t cbCompressed, void* pDestBuffer, std::uint32_t cbDestBufferSize, std::uint32_t* nBytesWritten, std::uint32_t nDesiredSampleRate) {
        return steammock::slot<std::int32_t>(kCall_822, pCompressed, cbCompressed, pDestBuffer, cbDestBufferSize, nBytesWritten, nDesiredSampleRate);
    }
    virtual std::uint32_t GetVoiceOptimalSampleRate() {
        return steammock::slot<std::uint32_t>(kCall_823);
    }
    virtual std::uint32_t GetAuthSessionTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket, void* pSteamNetworkingIdentity) {
        return steammock::slot<std::uint32_t>(kCall_845, pTicket, cbMaxTicket, pcbTicket, pSteamNetworkingIdentity);
    }
    virtual std::uint32_t GetAuthTicketForWebApi(const char* pchIdentity) {
        return steammock::slot<std::uint32_t>(kCall_846, pchIdentity);
    }
    virtual std::int32_t BeginAuthSession(void* pAuthTicket, std::int32_t cbAuthTicket, CSteamID steamID) {
        return steammock::slot<std::int32_t>(kCall_825, pAuthTicket, cbAuthTicket, steamID);
    }
    virtual void EndAuthSession(CSteamID steamID) { steammock::slot<void>(kCall_826, steamID); }
    virtual void CancelAuthTicket(std::uint32_t hAuthTicket) {
        steammock::slot<void>(kCall_827, hAuthTicket);
    }
    virtual std::int32_t UserHasLicenseForApp(CSteamID steamID, std::uint32_t appID) {
        return steammock::slot<std::int32_t>(kCall_828, steamID, appID);
    }
    virtual bool BIsBehindNAT() { return steammock::slot<bool>(kCall_829); }
    virtual void AdvertiseGame(CSteamID steamIDGameServer, std::uint32_t unIPServer, std::uint16_t usPortServer) {
        steammock::slot<void>(kCall_830, steamIDGameServer, unIPServer, usPortServer);
    }
    virtual std::uint64_t RequestEncryptedAppTicket(void* pDataToInclude, std::int32_t cbDataToInclude) {
        return steammock::slot<std::uint64_t>(kCall_831, pDataToInclude, cbDataToInclude);
    }
    virtual bool GetEncryptedAppTicket(void* pTicket, std::int32_t cbMaxTicket, std::uint32_t* pcbTicket) {
        return steammock::slot<bool>(kCall_832, pTicket, cbMaxTicket, pcbTicket);
    }
    virtual std::int32_t GetGameBadgeLevel(std::int32_t nSeries, bool bFoil) {
        return steammock::slot<std::int32_t>(kCall_833, nSeries, bFoil);
    }
    virtual std::int32_t GetPlayerSteamLevel() { return steammock::slot<std::int32_t>(kCall_834); }
    virtual std::uint64_t RequestStoreAuthURL(const char* pchRedirectURL) {
        return steammock::slot<std::uint64_t>(kCall_835, pchRedirectURL);
    }
    virtual bool BIsPhoneVerified() { return steammock::slot<bool>(kCall_836); }
    virtual bool BIsTwoFactorEnabled() { return steammock::slot<bool>(kCall_837); }
    virtual bool BIsPhoneIdentifying() { return steammock::slot<bool>(kCall_838); }
    virtual bool BIsPhoneRequiringVerification() { return steammock::slot<bool>(kCall_839); }
    virtual std::uint64_t GetMarketEligibility() {
        return steammock::slot<std::uint64_t>(kCall_840);
    }
    virtual std::uint64_t GetDurationControl() { return steammock::slot<std::uint64_t>(kCall_841); }
    virtual bool BSetDurationControlOnlineState(std::int32_t eNewState) {
        return steammock::slot<bool>(kCall_844, eNewState);
    }
};

Version_SteamUser023 g_SteamUser023;

// ISteamUserStats STEAMUSERSTATS_INTERFACE_VERSION011
class Version_STEAMUSERSTATS_INTERFACE_VERSION011 {
public:
    virtual bool RequestCurrentStats() { return steammock::slot<bool>(kCall_847); }
    virtual bool GetStat(const char* pchName, std::int32_t* pData) {
        return steammock::slot<bool>(kCall_848, pchName, pData);
    }
    virtual bool GetStat(const char* pchName, float* pData) {
        return steammock::slot<bool>(kCall_849, pchName, pData);
    }
    virtual bool SetStat(const char* pchName, std::int32_t nData) {
        return steammock::slot<bool>(kCall_850, pchName, nData);
    }
    virtual bool SetStat(const char* pchName, float fData) {
        return steammock::slot<bool>(kCall_851, pchName, fData);
    }
    virtual bool UpdateAvgRateStat(const char* pchName, float flCountThisSession, double dSessionLength) {
        return steammock::slot<bool>(kCall_852, pchName, flCountThisSession, dSessionLength);
    }
    virtual bool GetAchievement(const char* pchName, bool* pbAchieved) {
        return steammock::slot<bool>(kCall_853, pchName, pbAchieved);
    }
    virtual bool SetAchievement(const char* pchName) {
        return steammock::slot<bool>(kCall_854, pchName);
    }
    virtual bool ClearAchievement(const char* pchName) {
        return steammock::slot<bool>(kCall_855, pchName);
    }
    virtual bool GetAchievementAndUnlockTime(const char* pchName, bool* pbAchieved, std::uint32_t* punUnlockTime) {
        return steammock::slot<bool>(kCall_856, pchName, pbAchieved, punUnlockTime);
    }
    virtual bool StoreStats() { return steammock::slot<bool>(kCall_857); }
    virtual std::int32_t GetAchievementIcon(const char* pchName) {
        return steammock::slot<std::int32_t>(kCall_858, pchName);
    }
    virtual const char* GetAchievementDisplayAttribute(const char* pchName, const char* pchKey) {
        return steammock::slot<const char*>(kCall_859, pchName, pchKey);
    }
    virtual bool IndicateAchievementProgress(const char* pchName, std::uint32_t nCurProgress, std::uint32_t nMaxProgress) {
        return steammock::slot<bool>(kCall_860, pchName, nCurProgress, nMaxProgress);
    }
    virtual std::uint32_t GetNumAchievements() { return steammock::slot<std::uint32_t>(kCall_861); }
    virtual const char* GetAchievementName(std::uint32_t iAchievement) {
        return steammock::slot<const char*>(kCall_862, iAchievement);
    }
    virtual std::uint64_t RequestUserStats(CSteamID steamIDUser) {
        return steammock::slot<std::uint64_t>(kCall_863, steamIDUser);
    }
    virtual bool GetUserStat(CSteamID steamIDUser, const char* pchName, std::int32_t* pData) {
        return steammock::slot<bool>(kCall_864, steamIDUser, pchName, pData);
    }
    virtual bool GetUserStat(CSteamID steamIDUser, const char* pchName, float* pData) {
        return steammock::slot<bool>(kCall_865, steamIDUser, pchName, pData);
    }
    virtual bool GetUserAchievement(CSteamID steamIDUser, const char* pchName, bool* pbAchieved) {
        return steammock::slot<bool>(kCall_866, steamIDUser, pchName, pbAchieved);
    }
    virtual bool GetUserAchievementAndUnlockTime(CSteamID steamIDUser, const char* pchName, bool* pbAchieved, std::uint32_t* punUnlockTime) {
        return steammock::slot<bool>(kCall_867, steamIDUser, pchName, pbAchieved, punUnlockTime);
    }
    virtual bool ResetAllStats(bool bAchievementsToo) {
        return steammock::slot<bool>(kCall_868, bAchievementsToo);
    }
    virtual std::uint64_t FindOrCreateLeaderboard(const char* pchLeaderboardName, std::int32_t eLeaderboardSortMethod, std::int32_t eLeaderboardDisplayType) {
        return steammock::slot<std::uint64_t>(kCall_869, pchLeaderboardName, eLeaderboardSortMethod, eLeaderboardDisplayType);
    }
    virtual std::uint64_t FindLeaderboard(const char* pchLeaderboardName) {
        return steammock::slot<std::uint64_t>(kCall_870, pchLeaderboardName);
    }
    virtual const char* GetLeaderboardName(std::uint64_t hSteamLeaderboard) {
        return steammock::slot<const char*>(kCall_871, hSteamLeaderboard);
    }
    virtual std::int32_t GetLeaderboardEntryCount(std::uint64_t hSteamLeaderboard) {
        return steammock::slot<std::int32_t>(kCall_872, hSteamLeaderboard);
    }
    virtual std::int32_t GetLeaderboardSortMethod(std::uint64_t hSteamLeaderboard) {
        return steammock::slot<std::int32_t>(kCall_873, hSteamLeaderboard);
    }
    virtual std::int32_t GetLeaderboardDisplayType(std::uint64_t hSteamLeaderboard) {
        return steammock::slot<std::int32_t>(kCall_874, hSteamLeaderboard);
    }
    virtual std::uint64_t DownloadLeaderboardEntries(std::uint64_t hSteamLeaderboard, std::int32_t eLeaderboardDataRequest, std::int32_t nRangeStart, std::int32_t nRangeEnd) {
        return steammock::slot<std::uint64_t>(kCall_875, hSteamLeaderboard, eLeaderboardDataRequest, nRangeStart, nRangeEnd);
    }
    virtual std::uint64_t DownloadLeaderboardEntriesForUsers(std::uint64_t hSteamLeaderboard, CSteamID* prgUsers, std::int32_t cUsers) {
        return steammock::slot<std::uint64_t>(kCall_876, hSteamLeaderboard, prgUsers, cUsers);
    }
    virtual bool GetDownloadedLeaderboardEntry(std::uint64_t hSteamLeaderboardEntries, std::int32_t index, void* pLeaderboardEntry, std::int32_t* pDetails, std::int32_t cDetailsMax) {
        return steammock::slot<bool>(kCall_877, hSteamLeaderboardEntries, index, pLeaderboardEntry, pDetails, cDetailsMax);
    }
    virtual std::uint64_t UploadLeaderboardScore(std::uint64_t hSteamLeaderboard, std::int32_t eLeaderboardUploadScoreMethod, std::int32_t nScore, void* pScoreDetails, std::int32_t cScoreDetailsCount) {
        return steammock::slot<std::uint64_t>(kCall_878, hSteamLeaderboard, eLeaderboardUploadScoreMethod, nScore, pScoreDetails, cScoreDetailsCount);
    }
    virtual std::uint64_t AttachLeaderboardUGC(std::uint64_t hSteamLeaderboard, std::uint64_t hUGC) {
        return steammock::slot<std::uint64_t>(kCall_879, hSteamLeaderboard, hUGC);
    }
    virtual std::uint64_t GetNumberOfCurrentPlayers() {
        return steammock::slot<std::uint64_t>(kCall_880);
    }
    virtual std::uint64_t RequestGlobalAchievementPercentages() {
        return steammock::slot<std::uint64_t>(kCall_881);
    }
    virtual std::int32_t GetMostAchievedAchievementInfo(void* pchName, std::uint32_t unNameBufLen, float* pflPercent, bool* pbAchieved) {
        return steammock::slot<std::int32_t>(kCall_882, pchName, unNameBufLen, pflPercent, pbAchieved);
    }
    virtual std::int32_t GetNextMostAchievedAchievementInfo(std::int32_t iIteratorPrevious, void* pchName, std::uint32_t unNameBufLen, float* pflPercent, bool* pbAchieved) {
        return steammock::slot<std::int32_t>(kCall_883, iIteratorPrevious, pchName, unNameBufLen, pflPercent, pbAchieved);
    }
    virtual bool GetAchievementAchievedPercent(const char* pchName, float* pflPercent) {
        return steammock::slot<bool>(kCall_884, pchName, pflPercent);
    }
    virtual std::uint64_t RequestGlobalStats(std::int32_t nHistoryDays) {
        return steammock::slot<std::uint64_t>(kCall_885, nHistoryDays);
    }
    virtual bool GetGlobalStat(const char* pchStatName, std::int64_t* pData) {
        return steammock::slot<bool>(kCall_886, pchStatName, pData);
    }
    virtual bool GetGlobalStat(const char* pchStatName, double* pData) {
        return steammock::slot<bool>(kCall_887, pchStatName, pData);
    }
    virtual std::int32_t GetGlobalStatHistory(const char* pchStatName, std::int64_t* pData, std::uint32_t cubData) {
        return steammock::slot<std::int32_t>(kCall_888, pchStatName, pData, cubData);
    }
    virtual std::int32_t GetGlobalStatHistory(const char* pchStatName, double* pData, std::uint32_t cubData) {
        return steammock::slot<std::int32_t>(kCall_889, pchStatName, pData, cubData);
    }
};

Version_STEAMUSERSTATS_INTERFACE_VERSION011 g_STEAMUSERSTATS_INTERFACE_VERSION011;

// ISteamUserStats STEAMUSERSTATS_INTERFACE_VERSION012
class Version_STEAMUSERSTATS_INTERFACE_VERSION012 {
public:
    virtual bool RequestCurrentStats() { return steammock::slot<bool>(kCall_847); }
    virtual bool GetStat(const char* pchName, std::int32_t* pData) {
        return steammock::slot<bool>(kCall_848, pchName, pData);
    }
    virtual bool GetStat(const char* pchName, float* pData) {
        return steammock::slot<bool>(kCall_849, pchName, pData);
    }
    virtual bool SetStat(const char* pchName, std::int32_t nData) {
        return steammock::slot<bool>(kCall_850, pchName, nData);
    }
    virtual bool SetStat(const char* pchName, float fData) {
        return steammock::slot<bool>(kCall_851, pchName, fData);
    }
    virtual bool UpdateAvgRateStat(const char* pchName, float flCountThisSession, double dSessionLength) {
        return steammock::slot<bool>(kCall_852, pchName, flCountThisSession, dSessionLength);
    }
    virtual bool GetAchievement(const char* pchName, bool* pbAchieved) {
        return steammock::slot<bool>(kCall_853, pchName, pbAchieved);
    }
    virtual bool SetAchievement(const char* pchName) {
        return steammock::slot<bool>(kCall_854, pchName);
    }
    virtual bool ClearAchievement(const char* pchName) {
        return steammock::slot<bool>(kCall_855, pchName);
    }
    virtual bool GetAchievementAndUnlockTime(const char* pchName, bool* pbAchieved, std::uint32_t* punUnlockTime) {
        return steammock::slot<bool>(kCall_856, pchName, pbAchieved, punUnlockTime);
    }
    virtual bool StoreStats() { return steammock::slot<bool>(kCall_857); }
    virtual std::int32_t GetAchievementIcon(const char* pchName) {
        return steammock::slot<std::int32_t>(kCall_858, pchName);
    }
    virtual const char* GetAchievementDisplayAttribute(const char* pchName, const char* pchKey) {
        return steammock::slot<const char*>(kCall_859, pchName, pchKey);
    }
    virtual bool IndicateAchievementProgress(const char* pchName, std::uint32_t nCurProgress, std::uint32_t nMaxProgress) {
        return steammock::slot<bool>(kCall_860, pchName, nCurProgress, nMaxProgress);
    }
    virtual std::uint32_t GetNumAchievements() { return steammock::slot<std::uint32_t>(kCall_861); }
    virtual const char* GetAchievementName(std::uint32_t iAchievement) {
        return steammock::slot<const char*>(kCall_862, iAchievement);
    }
    virtual std::uint64_t RequestUserStats(CSteamID steamIDUser) {
        return steammock::slot<std::uint64_t>(kCall_863, steamIDUser);
    }
    virtual bool GetUserStat(CSteamID steamIDUser, const char* pchName, std::int32_t* pData) {
        return steammock::slot<bool>(kCall_864, steamIDUser, pchName, pData);
    }
    virtual bool GetUserStat(CSteamID steamIDUser, const char* pchName, float* pData) {
        return steammock::slot<bool>(kCall_865, steamIDUser, pchName, pData);
    }
    virtual bool GetUserAchievement(CSteamID steamIDUser, const char* pchName, bool* pbAchieved) {
        return steammock::slot<bool>(kCall_866, steamIDUser, pchName, pbAchieved);
    }
    virtual bool GetUserAchievementAndUnlockTime(CSteamID steamIDUser, const char* pchName, bool* pbAchieved, std::uint32_t* punUnlockTime) {
        return steammock::slot<bool>(kCall_867, steamIDUser, pchName, pbAchieved, punUnlockTime);
    }
    virtual bool ResetAllStats(bool bAchievementsToo) {
        return steammock::slot<bool>(kCall_868, bAchievementsToo);
    }
    virtual std::uint64_t FindOrCreateLeaderboard(const char* pchLeaderboardName, std::int32_t eLeaderboardSortMethod, std::int32_t eLeaderboardDisplayType) {
        return steammock::slot<std::uint64_t>(kCall_869, pchLeaderboardName, eLeaderboardSortMethod, eLeaderboardDisplayType);
    }
    virtual std::uint64_t FindLeaderboard(const char* pchLeaderboardName) {
        return steammock::slot<std::uint64_t>(kCall_870, pchLeaderboardName);
    }
    virtual const char* GetLeaderboardName(std::uint64_t hSteamLeaderboard) {
        return steammock::slot<const char*>(kCall_871, hSteamLeaderboard);
    }
    virtual std::int32_t GetLeaderboardEntryCount(std::uint64_t hSteamLeaderboard) {
        return steammock::slot<std::int32_t>(kCall_872, hSteamLeaderboard);
    }
    virtual std::int32_t GetLeaderboardSortMethod(std::uint64_t hSteamLeaderboard) {
        return steammock::slot<std::int32_t>(kCall_873, hSteamLeaderboard);
    }
    virtual std::int32_t GetLeaderboardDisplayType(std::uint64_t hSteamLeaderboard) {
        return steammock::slot<std::int32_t>(kCall_874, hSteamLeaderboard);
    }
    virtual std::uint64_t DownloadLeaderboardEntries(std::uint64_t hSteamLeaderboard, std::int32_t eLeaderboardDataRequest, std::int32_t nRangeStart, std::int32_t nRangeEnd) {
        return steammock::slot<std::uint64_t>(kCall_875, hSteamLeaderboard, eLeaderboardDataRequest, nRangeStart, nRangeEnd);
    }
    virtual std::uint64_t DownloadLeaderboardEntriesForUsers(std::uint64_t hSteamLeaderboard, CSteamID* prgUsers, std::int32_t cUsers) {
        return steammock::slot<std::uint64_t>(kCall_876, hSteamLeaderboard, prgUsers, cUsers);
    }
    virtual bool GetDownloadedLeaderboardEntry(std::uint64_t hSteamLeaderboardEntries, std::int32_t index, void* pLeaderboardEntry, std::int32_t* pDetails, std::int32_t cDetailsMax) {
        return steammock::slot<bool>(kCall_877, hSteamLeaderboardEntries, index, pLeaderboardEntry, pDetails, cDetailsMax);
    }
    virtual std::uint64_t UploadLeaderboardScore(std::uint64_t hSteamLeaderboard, std::int32_t eLeaderboardUploadScoreMethod, std::int32_t nScore, void* pScoreDetails, std::int32_t cScoreDetailsCount) {
        return steammock::slot<std::uint64_t>(kCall_878, hSteamLeaderboard, eLeaderboardUploadScoreMethod, nScore, pScoreDetails, cScoreDetailsCount);
    }
    virtual std::uint64_t AttachLeaderboardUGC(std::uint64_t hSteamLeaderboard, std::uint64_t hUGC) {
        return steammock::slot<std::uint64_t>(kCall_879, hSteamLeaderboard, hUGC);
    }
    virtual std::uint64_t GetNumberOfCurrentPlayers() {
        return steammock::slot<std::uint64_t>(kCall_880);
    }
    virtual std::uint64_t RequestGlobalAchievementPercentages() {
        return steammock::slot<std::uint64_t>(kCall_881);
    }
    virtual std::int32_t GetMostAchievedAchievementInfo(void* pchName, std::uint32_t unNameBufLen, float* pflPercent, bool* pbAchieved) {
        return steammock::slot<std::int32_t>(kCall_882, pchName, unNameBufLen, pflPercent, pbAchieved);
    }
    virtual std::int32_t GetNextMostAchievedAchievementInfo(std::int32_t iIteratorPrevious, void* pchName, std::uint32_t unNameBufLen, float* pflPercent, bool* pbAchieved) {
        return steammock::slot<std::int32_t>(kCall_883, iIteratorPrevious, pchName, unNameBufLen, pflPercent, pbAchieved);
    }
    virtual bool GetAchievementAchievedPercent(const char* pchName, float* pflPercent) {
        return steammock::slot<bool>(kCall_884, pchName, pflPercent);
    }
    virtual std::uint64_t RequestGlobalStats(std::int32_t nHistoryDays) {
        return steammock::slot<std::uint64_t>(kCall_885, nHistoryDays);
    }
    virtual bool GetGlobalStat(const char* pchStatName, std::int64_t* pData) {
        return steammock::slot<bool>(kCall_886, pchStatName, pData);
    }
    virtual bool GetGlobalStat(const char* pchStatName, double* pData) {
        return steammock::slot<bool>(kCall_887, pchStatName, pData);
    }
    virtual std::int32_t GetGlobalStatHistory(const char* pchStatName, std::int64_t* pData, std::uint32_t cubData) {
        return steammock::slot<std::int32_t>(kCall_888, pchStatName, pData, cubData);
    }
    virtual std::int32_t GetGlobalStatHistory(const char* pchStatName, double* pData, std::uint32_t cubData) {
        return steammock::slot<std::int32_t>(kCall_889, pchStatName, pData, cubData);
    }
    virtual bool GetAchievementProgressLimits(const char* pchName, std::int32_t* pnMinProgress, std::int32_t* pnMaxProgress) {
        return steammock::slot<bool>(kCall_890, pchName, pnMinProgress, pnMaxProgress);
    }
    virtual bool GetAchievementProgressLimits(const char* pchName, float* pfMinProgress, float* pfMaxProgress) {
        return steammock::slot<bool>(kCall_891, pchName, pfMinProgress, pfMaxProgress);
    }
};

Version_STEAMUSERSTATS_INTERFACE_VERSION012 g_STEAMUSERSTATS_INTERFACE_VERSION012;

// ISteamUtils SteamUtils009
class Version_SteamUtils009 {
public:
    virtual std::uint32_t GetSecondsSinceAppActive() {
        return steammock::slot<std::uint32_t>(kCall_892);
    }
    virtual std::uint32_t GetSecondsSinceComputerActive() {
        return steammock::slot<std::uint32_t>(kCall_893);
    }
    virtual std::int32_t GetConnectedUniverse() { return steammock::slot<std::int32_t>(kCall_894); }
    virtual std::uint32_t GetServerRealTime() { return steammock::slot<std::uint32_t>(kCall_895); }
    virtual const char* GetIPCountry() { return steammock::slot<const char*>(kCall_896); }
    virtual bool GetImageSize(std::int32_t iImage, std::uint32_t* pnWidth, std::uint32_t* pnHeight) {
        return steammock::slot<bool>(kCall_897, iImage, pnWidth, pnHeight);
    }
    virtual bool GetImageRGBA(std::int32_t iImage, std::uint8_t* pubDest, std::int32_t nDestBufferSize) {
        return steammock::slot<bool>(kCall_898, iImage, pubDest, nDestBufferSize);
    }
    virtual bool GetCSERIPPort(std::uint32_t* unIP, std::uint16_t* usPort) {
        return steammock::slot<bool>(kCall_899, unIP, usPort);
    }
    virtual std::uint8_t GetCurrentBatteryPower() {
        return steammock::slot<std::uint8_t>(kCall_900);
    }
    virtual std::uint32_t GetAppID() { return steammock::slot<std::uint32_t>(kCall_901); }
    virtual void SetOverlayNotificationPosition(std::int32_t eNotificationPosition) {
        steammock::slot<void>(kCall_902, eNotificationPosition);
    }
    virtual bool IsAPICallCompleted(std::uint64_t hSteamAPICall, bool* pbFailed) {
        return steammock::slot<bool>(kCall_903, hSteamAPICall, pbFailed);
    }
    virtual std::int32_t GetAPICallFailureReason(std::uint64_t hSteamAPICall) {
        return steammock::slot<std::int32_t>(kCall_904, hSteamAPICall);
    }
    virtual bool GetAPICallResult(std::uint64_t hSteamAPICall, void* pCallback, std::int32_t cubCallback, std::int32_t iCallbackExpected, bool* pbFailed) {
        return steammock::slot<bool>(kCall_905, hSteamAPICall, pCallback, cubCallback, iCallbackExpected, pbFailed);
    }
    virtual void RunFrame() { steammock::slot<void>(kCall_906); }
    virtual std::uint32_t GetIPCCallCount() { return steammock::slot<std::uint32_t>(kCall_907); }
    virtual void SetWarningMessageHook(void* pFunction) {
        steammock::slot<void>(kCall_908, pFunction);
    }
    virtual bool IsOverlayEnabled() { return steammock::slot<bool>(kCall_909); }
    virtual bool BOverlayNeedsPresent() { return steammock::slot<bool>(kCall_910); }
    virtual std::uint64_t CheckFileSignature(const char* szFileName) {
        return steammock::slot<std::uint64_t>(kCall_911, szFileName);
    }
    virtual bool ShowGamepadTextInput(std::int32_t eInputMode, std::int32_t eLineInputMode, const char* pchDescription, std::uint32_t unCharMax, const char* pchExistingText) {
        return steammock::slot<bool>(kCall_912, eInputMode, eLineInputMode, pchDescription, unCharMax, pchExistingText);
    }
    virtual std::uint32_t GetEnteredGamepadTextLength() {
        return steammock::slot<std::uint32_t>(kCall_913);
    }
    virtual bool GetEnteredGamepadTextInput(void* pchText, std::uint32_t cchText) {
        return steammock::slot<bool>(kCall_914, pchText, cchText);
    }
    virtual const char* GetSteamUILanguage() { return steammock::slot<const char*>(kCall_915); }
    virtual bool IsSteamRunningInVR() { return steammock::slot<bool>(kCall_916); }
    virtual void SetOverlayNotificationInset(std::int32_t nHorizontalInset, std::int32_t nVerticalInset) {
        steammock::slot<void>(kCall_917, nHorizontalInset, nVerticalInset);
    }
    virtual bool IsSteamInBigPictureMode() { return steammock::slot<bool>(kCall_918); }
    virtual void StartVRDashboard() { steammock::slot<void>(kCall_919); }
    virtual bool IsVRHeadsetStreamingEnabled() { return steammock::slot<bool>(kCall_920); }
    virtual void SetVRHeadsetStreamingEnabled(bool bEnabled) {
        steammock::slot<void>(kCall_921, bEnabled);
    }
    virtual bool IsSteamChinaLauncher() { return steammock::slot<bool>(kCall_922); }
    virtual bool InitFilterText() { return steammock::slot<bool>(kCall_923); }
    virtual std::int32_t FilterText(void* pchOutFilteredText, std::uint32_t nByteSizeOutFilteredText, const char* pchInputMessage, bool bLegalOnly) {
        return steammock::slot<std::int32_t>(kCall_924, pchOutFilteredText, nByteSizeOutFilteredText, pchInputMessage, bLegalOnly);
    }
    virtual std::int32_t GetIPv6ConnectivityState(std::int32_t eProtocol) {
        return steammock::slot<std::int32_t>(kCall_925, eProtocol);
    }
};

Version_SteamUtils009 g_SteamUtils009;

// ISteamUtils SteamUtils010
class Version_SteamUtils010 {
public:
    virtual std::uint32_t GetSecondsSinceAppActive() {
        return steammock::slot<std::uint32_t>(kCall_892);
    }
    virtual std::uint32_t GetSecondsSinceComputerActive() {
        return steammock::slot<std::uint32_t>(kCall_893);
    }
    virtual std::int32_t GetConnectedUniverse() { return steammock::slot<std::int32_t>(kCall_894); }
    virtual std::uint32_t GetServerRealTime() { return steammock::slot<std::uint32_t>(kCall_895); }
    virtual const char* GetIPCountry() { return steammock::slot<const char*>(kCall_896); }
    virtual bool GetImageSize(std::int32_t iImage, std::uint32_t* pnWidth, std::uint32_t* pnHeight) {
        return steammock::slot<bool>(kCall_897, iImage, pnWidth, pnHeight);
    }
    virtual bool GetImageRGBA(std::int32_t iImage, std::uint8_t* pubDest, std::int32_t nDestBufferSize) {
        return steammock::slot<bool>(kCall_898, iImage, pubDest, nDestBufferSize);
    }
    virtual bool GetCSERIPPort(std::uint32_t* unIP, std::uint16_t* usPort) {
        return steammock::slot<bool>(kCall_926, unIP, usPort);
    }
    virtual std::uint8_t GetCurrentBatteryPower() {
        return steammock::slot<std::uint8_t>(kCall_900);
    }
    virtual std::uint32_t GetAppID() { return steammock::slot<std::uint32_t>(kCall_901); }
    virtual void SetOverlayNotificationPosition(std::int32_t eNotificationPosition) {
        steammock::slot<void>(kCall_902, eNotificationPosition);
    }
    virtual bool IsAPICallCompleted(std::uint64_t hSteamAPICall, bool* pbFailed) {
        return steammock::slot<bool>(kCall_903, hSteamAPICall, pbFailed);
    }
    virtual std::int32_t GetAPICallFailureReason(std::uint64_t hSteamAPICall) {
        return steammock::slot<std::int32_t>(kCall_904, hSteamAPICall);
    }
    virtual bool GetAPICallResult(std::uint64_t hSteamAPICall, void* pCallback, std::int32_t cubCallback, std::int32_t iCallbackExpected, bool* pbFailed) {
        return steammock::slot<bool>(kCall_905, hSteamAPICall, pCallback, cubCallback, iCallbackExpected, pbFailed);
    }
    virtual void RunFrame() { steammock::slot<void>(kCall_906); }
    virtual std::uint32_t GetIPCCallCount() { return steammock::slot<std::uint32_t>(kCall_907); }
    virtual void SetWarningMessageHook(void* pFunction) {
        steammock::slot<void>(kCall_908, pFunction);
    }
    virtual bool IsOverlayEnabled() { return steammock::slot<bool>(kCall_909); }
    virtual bool BOverlayNeedsPresent() { return steammock::slot<bool>(kCall_910); }
    virtual std::uint64_t CheckFileSignature(const char* szFileName) {
        return steammock::slot<std::uint64_t>(kCall_911, szFileName);
    }
    virtual bool ShowGamepadTextInput(std::int32_t eInputMode, std::int32_t eLineInputMode, const char* pchDescription, std::uint32_t unCharMax, const char* pchExistingText) {
        return steammock::slot<bool>(kCall_912, eInputMode, eLineInputMode, pchDescription, unCharMax, pchExistingText);
    }
    virtual std::uint32_t GetEnteredGamepadTextLength() {
        return steammock::slot<std::uint32_t>(kCall_913);
    }
    virtual bool GetEnteredGamepadTextInput(void* pchText, std::uint32_t cchText) {
        return steammock::slot<bool>(kCall_914, pchText, cchText);
    }
    virtual const char* GetSteamUILanguage() { return steammock::slot<const char*>(kCall_915); }
    virtual bool IsSteamRunningInVR() { return steammock::slot<bool>(kCall_916); }
    virtual void SetOverlayNotificationInset(std::int32_t nHorizontalInset, std::int32_t nVerticalInset) {
        steammock::slot<void>(kCall_917, nHorizontalInset, nVerticalInset);
    }
    virtual bool IsSteamInBigPictureMode() { return steammock::slot<bool>(kCall_918); }
    virtual void StartVRDashboard() { steammock::slot<void>(kCall_919); }
    virtual bool IsVRHeadsetStreamingEnabled() { return steammock::slot<bool>(kCall_920); }
    virtual void SetVRHeadsetStreamingEnabled(bool bEnabled) {
        steammock::slot<void>(kCall_921, bEnabled);
    }
    virtual bool IsSteamChinaLauncher() { return steammock::slot<bool>(kCall_922); }
    virtual bool InitFilterText(std::uint32_t unFilterOptions) {
        return steammock::slot<bool>(kCall_927, unFilterOptions);
    }
    virtual std::int32_t FilterText(std::int32_t eContext, CSteamID sourceSteamID, const char* pchInputMessage, void* pchOutFilteredText, std::uint32_t nByteSizeOutFilteredText) {
        return steammock::slot<std::int32_t>(kCall_928, eContext, sourceSteamID, pchInputMessage, pchOutFilteredText, nByteSizeOutFilteredText);
    }
    virtual std::int32_t GetIPv6ConnectivityState(std::int32_t eProtocol) {
        return steammock::slot<std::int32_t>(kCall_925, eProtocol);
    }
    virtual bool IsSteamRunningOnSteamDeck() { return steammock::slot<bool>(kCall_929); }
    virtual bool ShowFloatingGamepadTextInput(std::int32_t eKeyboardMode, std::int32_t nTextFieldXPosition, std::int32_t nTextFieldYPosition, std::int32_t nTextFieldWidth, std::int32_t nTextFieldHeight) {
        return steammock::slot<bool>(kCall_930, eKeyboardMode, nTextFieldXPosition, nTextFieldYPosition, nTextFieldWidth, nTextFieldHeight);
    }
    virtual void SetGameLauncherMode(bool bLauncherMode) {
        steammock::slot<void>(kCall_931, bLauncherMode);
    }
    virtual bool DismissFloatingGamepadTextInput() { return steammock::slot<bool>(kCall_932); }
};

Version_SteamUtils010 g_SteamUtils010;

// ISteamVideo STEAMVIDEO_INTERFACE_V002
class Version_STEAMVIDEO_INTERFACE_V002 {
public:
    virtual void GetVideoURL(std::uint32_t unVideoAppID) {
        steammock::slot<void>(kCall_933, unVideoAppID);
    }
    virtual bool IsBroadcasting(std::int32_t* pnNumViewers) {
        return steammock::slot<bool>(kCall_934, pnNumViewers);
    }
    virtual void GetOPFSettings(std::uint32_t unVideoAppID) {
        steammock::slot<void>(kCall_935, unVideoAppID);
    }
    virtual bool GetOPFStringForApp(std::uint32_t unVideoAppID, void* pchBuffer, std::int32_t* pnBufferSize) {
        return steammock::slot<bool>(kCall_936, unVideoAppID, pchBuffer, pnBufferSize);
    }
};

Version_STEAMVIDEO_INTERFACE_V002 g_STEAMVIDEO_INTERFACE_V002;

const steammock::InterfaceVersion kVersions[] = {
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

}  // namespace steammock
