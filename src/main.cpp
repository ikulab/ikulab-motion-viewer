#include <vulkan/vulkan.h>

#include <stdexcept>
#include <iostream>
#include <cstdlib>

#include "app.hpp"
#include "animator.hpp"

int main() {
	Animator anim;
	anim.initFromBVH("./models/swing.bvh");

	return 0;

	App app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << "caught an exception :" << std::endl;
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}