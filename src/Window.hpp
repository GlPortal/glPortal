#ifndef __WINDOW_HPP
#define __WINDOW_HPP

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

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
private:
void setSize(int, int);
int width, height;
};

#endif
