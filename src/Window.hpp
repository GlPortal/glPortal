#ifndef __WINDOW_HPP
#define __WINDOW_HPP

#define DEFAULT_WIDTH 1680
#define DEFAULT_HEIGHT 1050

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include "Player.hpp"
#include "Map.hpp"
#include "Box.hpp"
#include <vector>

class Window {
public:
void setup(int *, char **);
int getWidth();
int getHeight();
void setSize(int, int);
private:
int width, height;
};

#endif
