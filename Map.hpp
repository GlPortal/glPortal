#ifndef __MAP_HPP
#define __MAP_HPP

#include <vector>
#include <GL/glut.h>
#include "Box.hpp"

class Map {
public:
	Map();
	void draw(GLuint *textures);

protected:
	void drawBox(std::vector<Box>::iterator it);
	std::vector<Box> walls;
	std::vector<Box> acid;
};

#endif
