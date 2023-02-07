#include "./resourceDirectory.hpp"

#ifdef __linux__
std::filesystem::path getResourceDirectory() {
    std::filesystem::path resourceDir = "/usr/local/share/ikulab-motion-viewer";

    return resourceDir;
}
#endif

#ifdef __APPLE__
std::filesystem::path getResourceDirectory() {
    std::filesystem::path resourceDir = "/usr/local/share/ikulab-motion-viewer";

    return resourceDir;
}
#endif

#ifdef IS_WINDOWS
std::filesystem::path getResourceDirectory() {
    std::filesystem::path homeDrive = getenv("HOMEDRIVE");
    std::filesystem::path homePath = getenv("HOMEPATH");
    std::filesystem::path resourceDir = homeDrive / homePath / ".ikulab-motion-viewer";

    return resourceDir;
}
#endif