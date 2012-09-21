#include "Map.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>

void Map::load(const char *filename) {
	std::ifstream ifs(filename, std::ifstream::in);
	std::string line, temp;

	// Read map file line by line
	while(ifs.good()) {
		std::getline(ifs, line);
		std::stringstream ss(line);
		if(line.length() > 0) {
			ss >> temp;
			float values[6];
			switch(line[0]) {
				// Wall definition
				case 'w':
					for(int i = 0; i < 6; i++) {
						ss >> temp;
						values[i] = (float)atof(temp.c_str());
					}
					walls.push_back(Box(values, BT_WALL));
					break;
				// Tiles definition
				case 't':
					for(int i = 0; i < 6; i++) {
						ss >> temp;
						values[i] = (float)atof(temp.c_str());
					}
					walls.push_back(Box(values, BT_TILES));
					break;
				// Acid pool definition
				case 'a':
					for(int i = 0; i < 6; i++) {
						ss >> temp;
						values[i] = (float)atof(temp.c_str());
					}
					acid.push_back(Box(values, BT_ACID));
					break;
				// Light position
				case 'l':
					for(int i = 0; i < 3; i++) {
						ss >> temp;
						lightpos[i] = (GLfloat)atof(temp.c_str());
					}
					lightpos[3] = 1.f; // Set as positioned light
					break;
			}
		}
	}

	ifs.close();
}

void Map::draw(GLuint *textures) {
	// Update light position
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	// Draw walls
	BOX_TYPE current_type = BT_NONE;
	std::vector<Box>::iterator it;

	for(it = walls.begin(); it < walls.end(); it++) {
		if(it->type != current_type) {
			current_type = it->type;
			glBindTexture(GL_TEXTURE_2D, textures[it->type]);
		}
		drawBox(it);
	}

	// Draw acid waste
	glBindTexture(GL_TEXTURE_2D, textures[BT_ACID]);
	for(it = acid.begin(); it < acid.end(); it++) {
		drawBox(it);
	}
}

void Map::drawFromPortal(GLuint *textures, Portal& portal) {
	// Update light position
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	// Draw walls
	std::vector<Box>::iterator it;
	BOX_TYPE current_type = BT_NONE;

	for(it = walls.begin(); it < walls.end(); it++) {
		// Horribly slow bounds check, but smaller code
		if(portal.dir == PD_FRONT && it->z2 > portal.z
		|| portal.dir == PD_BACK  && it->z1 < portal.z
		|| portal.dir == PD_RIGHT && it->x2 > portal.x
		|| portal.dir == PD_LEFT  && it->x1 < portal.x) {
			if(it->type != current_type) {
				current_type = it->type;
				glBindTexture(GL_TEXTURE_2D, textures[it->type]);
			}
			drawBox(it);
		}
	}

	// Draw acid waste
	glBindTexture(GL_TEXTURE_2D, textures[BT_ACID]);
	for(it = acid.begin(); it < acid.end(); it++) {
		if(portal.dir == PD_FRONT && it->z2 > portal.z
		|| portal.dir == PD_BACK && it->z1 < portal.z
		|| portal.dir == PD_RIGHT && it->x2 > portal.x
		|| portal.dir == PD_LEFT && it->x1 < portal.x) {
			drawBox(it);
		}
	}
}

void Map::drawBox(std::vector<Box>::iterator it) {
	float dx = (it->x2 - it->x1)*0.5f;
	float dy = (it->y2 - it->y1)*0.5f;
	float dz = (it->z2 - it->z1)*0.5f;

	glBegin(GL_QUADS);
	// Top
	glNormal3f(0,1,0);
	glTexCoord2f(0.f, 0.f); glVertex3f(it->x1, it->y2, it->z1);
	glTexCoord2f(0.f,  dz); glVertex3f(it->x1, it->y2, it->z2);
	glTexCoord2f( dx,  dz); glVertex3f(it->x2, it->y2, it->z2);
	glTexCoord2f( dx, 0.f); glVertex3f(it->x2, it->y2, it->z1);

	// Bottom
	glNormal3f(0,-1,0);
	glTexCoord2f( dx, 0.f); glVertex3f(it->x2, it->y1, it->z1);
	glTexCoord2f( dx,  dz); glVertex3f(it->x2, it->y1, it->z2);
	glTexCoord2f(0.f,  dz); glVertex3f(it->x1, it->y1, it->z2);
	glTexCoord2f(0.f, 0.f); glVertex3f(it->x1, it->y1, it->z1);

	// Front
	glNormal3f(0,0,1);
	glTexCoord2f( dx, 0.f); glVertex3f(it->x2, it->y1, it->z2);
	glTexCoord2f( dx,  dy); glVertex3f(it->x2, it->y2, it->z2);
	glTexCoord2f(0.f,  dy); glVertex3f(it->x1, it->y2, it->z2);
	glTexCoord2f(0.f, 0.f); glVertex3f(it->x1, it->y1, it->z2);

	// Back
	glNormal3f(0,0,-1);
	glTexCoord2f(0.f, 0.f); glVertex3f(it->x2, it->y1, it->z1);
	glTexCoord2f( dx, 0.f); glVertex3f(it->x1, it->y1, it->z1);
	glTexCoord2f( dx,  dy); glVertex3f(it->x1, it->y2, it->z1);
	glTexCoord2f(0.f,  dy); glVertex3f(it->x2, it->y2, it->z1);

	// Left
	glNormal3f(-1,0,0);
	glTexCoord2f(0.f,  dz); glVertex3f(it->x1, it->y1, it->z2);
	glTexCoord2f( dy,  dz); glVertex3f(it->x1, it->y2, it->z2);
	glTexCoord2f( dy, 0.f); glVertex3f(it->x1, it->y2, it->z1);
	glTexCoord2f(0.f, 0.f); glVertex3f(it->x1, it->y1, it->z1);

	// Right
	glNormal3f(1,0,0);
	glTexCoord2f(0.f, 0.f); glVertex3f(it->x2, it->y1, it->z1);
	glTexCoord2f( dy, 0.f); glVertex3f(it->x2, it->y2, it->z1);
	glTexCoord2f( dy,  dz); glVertex3f(it->x2, it->y2, it->z2);
	glTexCoord2f(0.f,  dz); glVertex3f(it->x2, it->y1, it->z2);

	glEnd();
}

bool Map::collidesWithWall(Box &bbox) {
	std::vector<Box>::iterator it;
	for(it = walls.begin(); it < walls.end(); it++) {
		if(bbox.collide(*it)) return true;
	}
	return false;
}

std::vector<Box>* Map::getWalls() {
	return &walls;
}

std::vector<Box> *Map::getAcid() {
	return &acid;
}
