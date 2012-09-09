#include "Map.hpp"

Map::Map() {
	// Left and right ground
	walls.push_back(Box(0,0,0, 5,-5,10));
	walls.push_back(Box(10,0,0, 15,-5,10));

	// Outer walls
	walls.push_back(Box(-1,0,0, 0,10,10));
	walls.push_back(Box(15,0,0, 16,10,10));
	walls.push_back(Box(0,-5,0, 15,10,-1));
	walls.push_back(Box(0,-5,10, 15,10,11));

	// Roof
	walls.push_back(Box(0,10,0, 15,11,10));

	// Toxic waste
	acid.push_back(Box(5,-1,0, 10,-5,10));
}

void Map::draw(GLuint *textures) {
	// Draw walls
	std::vector<Box>::iterator it;
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glBegin(GL_QUADS);
	for(it = walls.begin(); it < walls.end(); it++) {
		drawBox(it);
	}
	glEnd();

	// Draw acid waste
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);
	for(it = acid.begin(); it < acid.end(); it++) {
		drawBox(it);
	}
	glEnd();
}

void Map::drawBox(std::vector<Box>::iterator it) {
	float dx = (it->x2 - it->x1)*0.5f;
	float dy = (it->y2 - it->y1)*0.5f;
	float dz = (it->z2 - it->z1)*0.5f;

	// Top
	glTexCoord2f(0.f, 0.f); glVertex3f(it->x1, it->y2, it->z1);
	glTexCoord2f(0.f,  dz); glVertex3f(it->x1, it->y2, it->z2);
	glTexCoord2f( dx,  dz); glVertex3f(it->x2, it->y2, it->z2);
	glTexCoord2f( dx, 0.f); glVertex3f(it->x2, it->y2, it->z1);

	// Bottom
	glTexCoord2f( dx, 0.f); glVertex3f(it->x2, it->y1, it->z1);
	glTexCoord2f( dx,  dz); glVertex3f(it->x2, it->y1, it->z2);
	glTexCoord2f(0.f,  dz); glVertex3f(it->x1, it->y1, it->z2);
	glTexCoord2f(0.f, 0.f); glVertex3f(it->x1, it->y1, it->z1);

	// Front
	glTexCoord2f( dx, 0.f); glVertex3f(it->x2, it->y1, it->z2);
	glTexCoord2f( dx,  dy); glVertex3f(it->x2, it->y2, it->z2);
	glTexCoord2f(0.f,  dy); glVertex3f(it->x1, it->y2, it->z2);
	glTexCoord2f(0.f, 0.f); glVertex3f(it->x1, it->y1, it->z2);

	// Back
	glTexCoord2f(0.f, 0.f); glVertex3f(it->x2, it->y1, it->z1);
	glTexCoord2f( dx, 0.f); glVertex3f(it->x1, it->y1, it->z1);
	glTexCoord2f( dx,  dy); glVertex3f(it->x1, it->y2, it->z1);
	glTexCoord2f(0.f,  dy); glVertex3f(it->x2, it->y2, it->z1);

	// Left
	glTexCoord2f(0.f,  dz); glVertex3f(it->x1, it->y1, it->z2);
	glTexCoord2f( dy,  dz); glVertex3f(it->x1, it->y2, it->z2);
	glTexCoord2f( dy, 0.f); glVertex3f(it->x1, it->y2, it->z1);
	glTexCoord2f(0.f, 0.f); glVertex3f(it->x1, it->y1, it->z1);

	// Right
	glTexCoord2f(0.f, 0.f); glVertex3f(it->x2, it->y1, it->z1);
	glTexCoord2f( dy, 0.f); glVertex3f(it->x2, it->y2, it->z1);
	glTexCoord2f( dy,  dz); glVertex3f(it->x2, it->y2, it->z2);
	glTexCoord2f(0.f,  dz); glVertex3f(it->x2, it->y1, it->z2);
}
