#ifndef __SHOT_HPP
#define __SHOT_HPP

#include <GL/glew.h>
#include "engine/Box.hpp"
#include "util/Vector3f.hpp"

using namespace glPortal::engine;
using namespace util;

/**
 * Class representing a shot from the portal gun.
 */
class Shot {
public:
	Shot() : active(false) { }

	void shoot(int _id, float x, float y, float z, float pitch, float yaw);
	void update(float dt);
	void draw(float pitch, float yaw);

	bool active; /**< True if the portal is currently active */
	int id;		 /**< ID of the portal. Either 0 or 1. */
	Vector3f position;
	Vector3f velocity;
};
#endif

