#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <engine/core/diag/Logger.hpp>

namespace glPortal {

class Console : public Logger {
public:
  void render();
};

} /* namespace glPortal */
#endif /* CONSOLE_HPP */