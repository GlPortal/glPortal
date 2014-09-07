#include "Text.hpp"

namespace glPortal {
  Text::Text(std::string text, Vector3f position){
    this->text = text;
    this->position = position;
    this->font = FontLoader::getFont("Adobe");
    this->size = 1;
  }
} /* namespace glPortal */
