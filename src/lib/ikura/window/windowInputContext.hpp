#pragma once

struct Keyboard {};

struct Mouse {};

class WindowInputContext {
public:
	Keyboard keyboard;
	Mouse mouse;
};