#pragma once

#include <filesystem>

void exportLoopRangeToBvhFile(std::filesystem::path sourceFile,
                              std::filesystem::path destFile,
                              uint32_t loopStartIndex, uint32_t loopEndIndex);