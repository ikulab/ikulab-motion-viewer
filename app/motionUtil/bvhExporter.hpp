#pragma once

#include <filesystem>

#include "./animator.hpp"

void exportLoopRangeToBvhFile(const Animator &animator,
                              std::filesystem::path destFile);
