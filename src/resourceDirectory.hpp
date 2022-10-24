#pragma once
#include <cstdlib>
#include <string>

std::string getResourceDirectory();
// relPath starts without '/'
std::string createAbsPathFromResourceDirectory(const char *relPath);

#ifdef __linux__
std::string getResourceDirectory() {
    std::string homeDir(getenv("HOME"));
    std::string resourceDir = homeDir + "/.local/share/ikulab-motion-viewer";
    return resourceDir;
}

std::string createAbsPathFromResourceDirectory(const char *relPath) {
    std::string resourceDir = getResourceDirectory();
    std::string absPath = resourceDir + "/" + relPath;
    return absPath;
}
#endif