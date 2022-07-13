#include <vulkan/vulkan.h>

#include <stdexcept>
#include <iostream>
#include <cstdlib>

#include <easylogging++.h>
INITIALIZE_EASYLOGGINGPP

#include "app.hpp"

int main() {
	el::Configurations conf("./easylogging.conf");
	el::Loggers::reconfigureAllLoggers(conf);
	el::Loggers::setVerboseLevel(4);

	App app;

	try {
		app.init();
	}
	catch (const std::runtime_error& e) {
		LOG(FATAL) << e.what();
		return EXIT_FAILURE;
	}
	catch (const std::exception& e) {
		std::cerr << "Caught an Exception :" << std::endl;
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}