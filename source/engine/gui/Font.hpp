#ifndef __FONT_HPP
#define __FONT_HPP

#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/freeglut.h>
#include "Window.hpp"

class Font {
public:
  Font();
  void drawStringToPosition(char[] string, x, y);
};

#endif
