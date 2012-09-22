#ifndef __MAP_HPP
#define __MAP_HPP

#include <vector>
#include <GL/glut.h>
#include "Box.hpp"
#include "Portal.hpp"

class Map {
public:
	void load(const char *filename);
	void draw(GLuint *textures);
	void drawFromPortal(GLuint *textures, Portal& portal);
	bool collidesWithWall(Box &bbox);
	bool pointInWall(float x, float y, float z, Box *box);

protected:
	void drawBox(std::vector<Box>::iterator it);
	std::vector<Box> *getWalls();
	std::vector<Box> *getAcid();

	std::vector<Box> walls;
	std::vector<Box> acid;
	GLfloat lightpos[4];
};

#endif
