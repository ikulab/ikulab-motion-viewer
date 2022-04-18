#include <memory>

#include "app.hpp"
#include "base.hpp"

void App::init() {
	base = std::make_unique<Base>();
}

void App::run() {
	while (!base->windowShouldClose()) {
		base->pollWindowEvent();
		base->drawFrame();
	}
}