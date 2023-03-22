#include "./bvhExporter.hpp"

#include <cstring>
#include <fstream>

#include "./bvhParser.hpp"

void exportLoopRangeToBvhFile(const Animator &animator,
                              std::filesystem::path destFile) {

    // vector<vector<Motion>>
    auto motions = animator.getMotions();
    std::ofstream exportFile(destFile);

    // TODO: export process
}
