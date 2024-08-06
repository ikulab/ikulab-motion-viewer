#include "app.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <easylogging++.h>

#include "./resourceDirectory.hpp"
#include "./util/popupUtils.hpp"
#include "./util/errorUtils.hpp"

INITIALIZE_EASYLOGGINGPP

void initEasyloggingpp() {
    el::Configurations conf;
    conf.setToDefault();

#ifdef __APPLE__
    auto logFilePath =
        getHomeDirectory() / "Library" / "Logs" / "ikulab-motion-viewer.log";
    conf.set(el::Level::Global, el::ConfigurationType::Filename, logFilePath);
#endif

    el::Loggers::reconfigureAllLoggers(conf);
}

int main(int argc, const char **argv) {
    initEasyloggingpp();

    if (argc > 1) {
        int vLevel = std::stoi(argv[1]);
        el::Loggers::setVerboseLevel(vLevel);
    }

    el::Loggers::setVerboseLevel(9);

    LOG(ERROR) << "test";

    try {
        App app;
        std::cout << "Hello Ikura!!" << std::endl;
        app.run();
    } catch (const std::runtime_error &e) {
        std::string msg;
        msg += "Caught a Runtime Error: ";
        msg += e.what();

        notifyErrorAndExit(msg);
    } catch (const std::exception &e) {
        std::string msg;
        msg += "Caught a Fatal Error: ";
        msg += e.what();

        notifyErrorAndExit(msg);
    }

    return EXIT_SUCCESS;
}
