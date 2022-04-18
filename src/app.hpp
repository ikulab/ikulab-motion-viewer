#pragma once

#include <memory>

#include "base.hpp"

class App {
	std::unique_ptr<Base> base;

	void init();

public:
	App() {
		init();
	}

	~App() {
		// TODO: clean up Base object
	}

	void run();
};