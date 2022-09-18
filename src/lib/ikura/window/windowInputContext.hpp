#pragma once

namespace ikura {
struct Keyboard {};

struct Mouse {};

class WindowInputContext {
  public:
    Keyboard keyboard;
    Mouse mouse;
};
} // namespace ikura