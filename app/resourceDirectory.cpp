#include "./resourceDirectory.hpp"

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <mach-o/dyld.h>
#endif

#include <tinyfiledialogs.h>

#ifdef __linux__
std::filesystem::path getResourceDirectory() {
    std::filesystem::path resourceDir = "/usr/local/share/ikulab-motion-viewer";

    return resourceDir;
}

std::filesystem::path getHomeDirectory() { return getenv("HOME"); }
#endif

#ifdef __APPLE__
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

std::filesystem::path getHomeDirectory() { return getenv("HOME"); }
#endif

#ifdef IS_WINDOWS
std::filesystem::path getResourceDirectory() {
    std::filesystem::path homeDrive = getenv("HOMEDRIVE");
    std::filesystem::path homePath = getenv("HOMEPATH");
    std::filesystem::path resourceDir = homeDrive / homePath / ".ikulab-motion-viewer";

    return resourceDir;
}

std::filesystem::path getHomeDirectory() {

    std::filesystem::path homeDrive = getenv("HOMEDRIVE");
    std::filesystem::path homePath = getenv("HOMEPATH");
    
    return homeDrive / homePath;
}
#endif