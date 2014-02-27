#ifndef __SHOT_HPP
#define __SHOT_HPP

#include <GL/freeglut.h>
#include "engine/Box.hpp"

using namespace glPortal::engine;

/**
 * Class representing a shot from the portal gun.
 */
class Shot {
public:
	Shot() : active(false), x(0), y(0), z(0), xspeed(0), yspeed(0), zspeed(0) { }

	void shoot(int _id, float _x, float _y, float _z, float xrot, float yrot);
	void update(float dt);
	void draw(float xrot, float yrot);

	bool active; /**< True if the portal is currently active */
	int id;		 /**< ID of the portal. Either 0 or 1. */
	float x,y,z;
	float xspeed, yspeed, zspeed;
};
#endif
