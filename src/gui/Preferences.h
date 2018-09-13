#pragma once

#include <json.hpp>
#include <SDL2/SDL.h>
#include <fstream>
#include <string>

using json = nlohmann::json;

#define kYonPrefTag "shortYon"
#define kMouseSensitivityTag "mouseSensitivity"
#define kLatencyToleranceTag "latencyTolerance"
#define kHullTypeTag "hull"

// TODO: split this into separate prefs
#define kServerOptionsTag "_serverOptions"

// UDP stuff
#define kDefaultUDPPort "udpDefaultPort"
#define kUDPResendPrefTag "udpResend"
#define kUDPConnectionSpeedTag "udpConnectionSpeed"

// Keyboard mapping
#define kKeyboardMappingTag "keyboard"

#define kPlayerNameTag "playerName"

// GL stuff
#define kMultiSamplesTag "multiSamples"


// Key names are from https://wiki.libsdl.org/SDL_Scancode
static json defaultPrefs = {{kYonPrefTag, 0},
    {kMouseSensitivityTag, 0},
    {kLatencyToleranceTag, 1},
    {kHullTypeTag, 0}, // 0 = light, 1 = medium, 2 = heavy
    {kServerOptionsTag, 129}, // 1 = allow load, 128 = auto latency
    {kDefaultUDPPort, 19567},
    {kUDPResendPrefTag, 2},
    {kUDPConnectionSpeedTag, 3},
    {kKeyboardMappingTag,
        {{"forward", {"W", "Up"}},
            {"backward", {"S", "Down"}},
            {"left", {"A", "Left"}},
            {"right", {"D", "Right"}},
            {"jump", {"Space", "B", "N", "M", ","}},
            {"abort", "Escape"},
            {"loadMissile", "Q"},
            {"loadGrenade", "E"},
            {"fire", "E"},
            {"boost", "Left Shift"},
            {"aimForward", "2"},
            {"scoutView", "Tab"},
            {"scoutControl", {"2", "Up", "Down", "Left", "Right"}},
            {"chatMode", "Return"}}},
    {kPlayerNameTag, "Unnamed Player"},
    {kMultiSamplesTag, 0}};



static std::string PrefPath() {
    char *prefPath = SDL_GetPrefPath("Avaraline", "Avara");
    std::string jsonPath = std::string(prefPath) + "prefs.json";
    SDL_free(prefPath);
    return jsonPath;
}

static json ReadPrefs() {
    json prefs;
    std::ifstream in(PrefPath());

    if (in.fail()) {
        // No prefs file, use defaults.
        prefs = defaultPrefs;
    } else {
        in >> prefs;
        for (json::iterator it = defaultPrefs.begin(); it != defaultPrefs.end(); ++it) {
            if (prefs.find(it.key()) == prefs.end()) {
                // A new key was added to defaultPrefs, add it to prefs.
                prefs[it.key()] = it.value();
            }
        }
    }

    return prefs;
}

static void WritePrefs(json prefs) {
    std::ofstream out(PrefPath());
    out << std::setw(4) << prefs << std::endl;
}
