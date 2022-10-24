#pragma once
#include <cstdlib>
#include <string>
#include <filesystem>

std::filesystem::path getResourceDirectory();

#ifdef __linux__
std::filesystem::path getResourceDirectory() {
    std::filesystem::path homeDir(getenv("HOME"));
    auto resourceDir = homeDir / ".local" / "share" / "ikulab-motion-viewer";
    return resourceDir;
}
#endif