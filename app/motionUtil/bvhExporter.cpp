#include "./bvhExporter.hpp"

#include <cstring>
#include <fstream>

#include "./bvhParser.hpp"

void exportLoopRangeToBvhFile(std::filesystem::path sourceFile,
                              std::filesystem::path destFile,
                              uint32_t loopStartIndex, uint32_t loopEndIndex) {

    std::ifstream origFile(sourceFile);
    std::ofstream exportFile(destFile);

    char buf[10000];
    while (!origFile.eof()) {
        origFile.getline(buf, 10000);
        exportFile << buf << "\n";

        if (!std::strcmp(buf, "MOTION")) {
            break;
        }
    }

    // Frames: <num of frames>
    origFile.getline(buf, 10000);
    exportFile << TOKEN_FRAMES;
    exportFile << " " << loopEndIndex - loopStartIndex + 1 << "\n";
    // Frame Time: <frame time>
    origFile.getline(buf, 10000);
    exportFile << buf << "\n";

    int index = 0;
    while (!origFile.eof()) {
        origFile.getline(buf, 10000);
        if (loopStartIndex <= index && index <= loopEndIndex) {
            exportFile << buf << "\n";
        }
        index++;
    }
}