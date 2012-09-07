#include "Map.hpp"
#include <GL/glut.h>

Map::Map() {
	walls.push_back(Box(-5,0,-5, 5,-1,5));
}

void Map::draw() {
	std::vector<Box>::iterator it;

	glBegin(GL_QUADS);
	for(it = walls.begin(); it < walls.end(); it++) {
		glVertex3f(it->x1, it->y1, it->z1);
		glVertex3f(it->x1, it->y1, it->z2);
		glVertex3f(it->x2, it->y1, it->z2);
		glVertex3f(it->x2, it->y1, it->z1);
	}
	glEnd();
}
