#include "Screen.hpp"

namespace glPortal {

Screen::Screen(std::string title, std::string text):
  title(title), text(text), enabled(false), alpha(1),
  textColor(0, 0, 0, 1), backgroundColor(1, 1, 1, 1) {
}
} /* namespace glPortal */
