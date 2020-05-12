#include "nelf/Version.h"

#include <sstream>
#include <string>

const char* elfGetProgramName() { return "NeoELF"; }

const char* elfGetPlatform()
{
#ifdef WIN32
    return "windows";
#elif defined __APPLE__
    return "macosx";
#elif defined __linux__
    return "linux";
#else
    return "unknown";
#endif
}

int elfGetVersionMajor() { return 0; }

int elfGetVersionMinor() { return 1; }

int elfGetVersionPatch() { return 0; }

const char* elfGetVersionRelease() { return "alpha1"; }

static std::string buildVersion()
{
    std::stringstream ss;
    // Version core
    ss << elfGetVersionMajor() << "." << elfGetVersionMinor() << "." << elfGetVersionPatch();

    // Additional (pre-)release info
    if (elfGetVersionRelease() != nullptr && strlen(elfGetVersionRelease()) > 0)
    {
        ss << "-" << elfGetVersionRelease();
    }
    ss << "-" << elfGetPlatform();

    return ss.str();
}

const char* elfGetVersion()
{
    static std::string version = buildVersion();
    return version.c_str();
}