#include "Screen.hpp"

namespace glPortal {

Screen::Screen(std::string title, std::string text) {
  this->title = title;
  this->text  = text;
  this->enabled = true;
  this->alpha = 1;
}
} /* namespace glPortal */

