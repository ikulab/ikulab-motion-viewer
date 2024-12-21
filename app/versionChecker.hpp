//
// Created by Caffeine on 24/12/22.
//

#pragma once

#include <easylogging++.h>

#include "./resourceDirectory.hpp"

#ifdef IS_WINDOWS
#include <windows.h>
#endif

#ifdef IS_WINDOWS

static void spawnVersionCheckerProcess() {
    auto versionCheckerExePath = getResourceDirectory() / "imv_version_checker.exe";
    versionCheckerExePath.make_preferred();

    auto versionInfoDirPath = getResourceDirectory();
    versionInfoDirPath.make_preferred();

    ShellExecute(nullptr, "open",
                 reinterpret_cast<LPCSTR>(versionCheckerExePath.string().c_str()),
                 reinterpret_cast<LPCSTR>(versionInfoDirPath.string().c_str()),
                 nullptr,
                 SW_HIDE);
    if (const auto result = GetLastError(); result < 32) {
        LOG(ERROR) << "Failed to launch version-checker.exe: " << result;
    }

}

#endif

#ifdef IS_MACOS

static void spawnVersionCheckerProcess() {
    //TODO
}

#endif
