#include <vulkan/vulkan.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <easylogging++.h>
// INITIALIZE_EASYLOGGINGPP

#include "app.hpp"

int main(int argc, const char **argv) {
    el::Configurations conf("./easylogging.conf");
    el::Loggers::reconfigureAllLoggers(conf);

    if (argc > 1) {
        int vLevel = std::stoi(argv[1]);
        el::Loggers::setVerboseLevel(vLevel);
    }

    try {
        App app;
        std::cout << "Hello Ikura!!" << std::endl;
        app.run();
    } catch (const std::runtime_error &e) {
        LOG(FATAL) << e.what();
        return EXIT_FAILURE;
    } catch (const std::exception &e) {
        std::cerr << "Caught an Exception :" << std::endl;
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}