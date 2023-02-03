#include "init.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

#include <easylogging++.h>
#include <libshaderc_util/io_shaderc.h>
#include <shaderc/shaderc.hpp>

#include "./ikuraConfig.h"

#include "./common/resourceDirectory.hpp"
#include "./misc/shaderCodes.hpp"

#define VERSION_INFO_FILE_NAME "version_info.txt"

namespace ikura {
void compileGlslCodeAndOutput(shaderc::Compiler &compiler,
                              std::filesystem::path outputPath,
                              const char *code, size_t codeSize,
                              shaderc_shader_kind kind, const char *name) {
    // compile ----------
    auto compilationResult =
        compiler.CompileGlslToSpv(code, codeSize, kind, "vertex_shader");

    // error check
    if (compilationResult.GetCompilationStatus() !=
        shaderc_compilation_status_success) {
        std::string msg;
        msg += "Failed to compile shader code.\n";
        msg += compilationResult.GetErrorMessage();
        throw std::runtime_error(msg);
    }

    shaderc_util::string_piece compilationOutput(
        reinterpret_cast<const char *>(compilationResult.cbegin()),
        reinterpret_cast<const char *>(compilationResult.cend()));

    // output ----------
    // prepare stream
    std::ofstream potentialOutputStream;
    auto outputStream = shaderc_util::GetOutputStream(
        outputPath.c_str(), &potentialOutputStream, &std::cerr);

    // error check
    if (!outputStream || outputStream->fail()) {
        throw std::runtime_error("Failed to open shader output stream "
                                 "(provided by libshaderc_util).");
    }

    outputStream->write(compilationOutput.data(), compilationOutput.size());
}

std::string getCurrentVersionString() {
    std::string versionInfoStr;
    versionInfoStr += std::to_string(IKURA_VERSION_MAJOR);
    versionInfoStr += ".";
    versionInfoStr += std::to_string(IKURA_VERSION_MINOR);
    versionInfoStr += ".";
    versionInfoStr += std::to_string(IKURA_VERSION_PATCH);

    return versionInfoStr;
}

// creates vertex / fragment shader file (.spv) to shader resource directory
void createShaderFiles(std::filesystem::path shaderResourceDirPath) {
    shaderc::Compiler spvCompiler;

    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Generating " << shaderResourceDirPath / "vert.spv"
        << " ...";

    compileGlslCodeAndOutput(
        spvCompiler,                                /* compiler */
        shaderResourceDirPath / "vert.spv",         /* output path */
        VERTEX_SHADER_CODE.c_str(),                 /* shader code */
        VERTEX_SHADER_CODE.size(),                  /* shader code size */
        shaderc_shader_kind::shaderc_vertex_shader, /* kind */
        "vertex_shader"                             /* shader name */
    );

    VLOG(VLOG_LV_3_PROCESS_TRACKING)
        << "Generating " << shaderResourceDirPath / "frag.spv"
        << " ...";

    compileGlslCodeAndOutput(
        spvCompiler,                                  /* compiler */
        shaderResourceDirPath / "frag.spv",           /* output path */
        FRAGMENT_SHADER_CODE.c_str(),                 /* shader code */
        FRAGMENT_SHADER_CODE.size(),                  /* shader code size */
        shaderc_shader_kind::shaderc_fragment_shader, /* kind */
        "fragment_shader"                             /* shader name */
    );
}

// Overwrite shader file if (version is difference) or (shader files not exist)
void overwriteShaderFilesIfNeeded(std::filesystem::path shaderDir) {
    // if version info file NOT exists
    if (!std::filesystem::exists(shaderDir / VERSION_INFO_FILE_NAME)) {
        createShaderFiles(shaderDir);
        return;
    }

    // if any shader files are NOT exist
    if (!std::filesystem::exists(shaderDir / "vert.spv") ||
        !std::filesystem::exists(shaderDir / "frag.spv")) {
        createShaderFiles(shaderDir);
        return;
    }

    // if recorded version is differ from current version
    std::ifstream versionFileIn(shaderDir / VERSION_INFO_FILE_NAME);
    std::string recordedVersionString;
    versionFileIn >> recordedVersionString;

    if (recordedVersionString != getCurrentVersionString()) {
        createShaderFiles(shaderDir);
        return;
    }
}

void createDirectoryIfNotExists(std::filesystem::path directoryPath) {
    if (!std::filesystem::exists(directoryPath)) {
        VLOG(VLOG_LV_3_PROCESS_TRACKING)
            << directoryPath << " is not exists. Creating directory...";

        std::filesystem::create_directory(directoryPath);
    }
}

void init() {
    VLOG(VLOG_LV_3_PROCESS_TRACKING) << "Initializing ikura...";

    // create resource directories if not exists
    createDirectoryIfNotExists(createResourceDirectoryPath(""));
    createDirectoryIfNotExists(createResourceDirectoryPath("shaders"));

    auto shaderDir = createResourceDirectoryPath("shaders");

    // create .spv files if not exists
    overwriteShaderFilesIfNeeded(shaderDir);

    // record current version to shader resource directory
    std::ofstream versionFileOut(shaderDir / VERSION_INFO_FILE_NAME);
    versionFileOut << getCurrentVersionString();

    VLOG(VLOG_LV_3_PROCESS_TRACKING) << "ikura has been initialized.";
}
} // namespace ikura