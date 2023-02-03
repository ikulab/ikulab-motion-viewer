#include "resourceDirectory.hpp"

namespace ikura {

#ifdef __linux__
std::filesystem::path
createResourceDirectoryPath(std::filesystem::path subPath) {
    std::filesystem::path homeDir = getenv("HOME");
    std::filesystem::path resourceDir = homeDir / ".local" / "share" / "ikura";
    std::filesystem::path destinationPath = resourceDir / subPath;

    return destinationPath;
}
#endif

#ifdef __APPLE__
std::filesystem::path
createResourceDirectoryPath(std::filesystem::path subPath) {
    std::filesystem::path homeDir = getenv("HOME");
    std::filesystem::path resourceDir = homeDir / ".local" / "share" / "ikura";
    std::filesystem::path destinationPath = resourceDir / subPath;

    return destinationPath;
}
#endif

} // namespace ikura