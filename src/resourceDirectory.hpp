#pragma once
#include <cstdlib>
#include <filesystem>
#include <string>

std::filesystem::path getResourceDirectory();

#ifdef __linux__
std::filesystem::path getResourceDirectory() {
    std::filesystem::path resourceDir = "/usr/local/share/ikulab-motion-viewer";

    return resourceDir;
}
#endif