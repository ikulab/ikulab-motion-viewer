#include "./resourceDirectory.hpp"

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <mach-o/dyld.h>
#elif IS_WINDOWS
#include <windows.h>
#endif

#include <tinyfiledialogs.h>

// ------------------------------------------------------------
// getResourceDirectory
// ------------------------------------------------------------

#ifdef RESOURCE_DIR

std::filesystem::path getResourceDirectory() {
    std::filesystem::path resourceDir = RESOURCE_DIR;

    return resourceDir;
}

#else

#ifdef __linux__

std::filesystem::path getResourceDirectory() {
    std::filesystem::path resourceDir = "/usr/local/share/ikulab-motion-viewer";

    return resourceDir;
}

#elif __APPLE__

/**
 * @brief Check if the application is launched from a bundle.
 * i.e. if the application launched by double-clicking .app file from Finder.
 * 
 * @return true if the application is launched from a bundle.
 
 */
bool launchedFromBundle() {
    char result[PATH_MAX];
    uint32_t size = PATH_MAX;
    if (_NSGetExecutablePath(result, &size) == 0) {
        auto resultString = std::string(result);
        return resultString.find(".app/Contents/MacOS/") != std::string::npos;
    }

    return false;
}

/**
 * @brief Get the resource directory from the bundle.
 * i.g. /path/to/ikulab-motion-viewer.app/Contents/Resources
 * 
 * @return path to Resources directory in the bundle.
 */
std::filesystem::path getBundleResourceDirectory() {
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);

    char path[PATH_MAX];
    if (CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path,
                                         PATH_MAX)) {
        CFRelease(resourcesURL);
        return path;
    }
    CFRelease(resourcesURL);
    return "";
}

std::filesystem::path getResourceDirectory() {
    if (launchedFromBundle()) {
        return getBundleResourceDirectory();
    } else {
        return "/usr/local/share/ikulab-motion-viewer";
    }
}

#elif IS_WINDOWS

std::filesystem::path getResourceDirectory() {
    char buffer[MAX_PATH];
    GetModuleFileName(nullptr, buffer, MAX_PATH);
    const std::filesystem::path path = buffer;
    std::filesystem::path resourceDir = path.parent_path();

    return resourceDir;
}

#endif

#endif

// ------------------------------------------------------------
// getHomeDirectory
// ------------------------------------------------------------

#ifdef __linux__

std::filesystem::path getHomeDirectory() { return getenv("HOME"); }

#elif __APPLE__

std::filesystem::path getHomeDirectory() { return getenv("HOME"); }

#elif IS_WINDOWS

std::filesystem::path getHomeDirectory() {

    std::filesystem::path homeDrive = getenv("HOMEDRIVE");
    std::filesystem::path homePath = getenv("HOMEPATH");

    return homeDrive / homePath;
}
#endif