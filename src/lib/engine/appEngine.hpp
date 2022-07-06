#pragma once

#include <vector>
#include <memory>

#include "../window/window.hpp"

class AppEngine {
	std::vector<std::unique_ptr<Window>> windows;
};