#include <vulkan/vulkan.h>

#include <stdexcept>
#include <iostream>
#include <cstdlib>

extern "C" {
#include <logc/log.h>
}

#include "app.hpp"

int main() {
	App app;

	try {
		app.init();
	}
	catch (const std::runtime_error& e) {
		log_error(e.what());
		return EXIT_FAILURE;
	}
	catch (const std::exception& e) {
		std::cerr << "Caught an Exception :" << std::endl;
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}