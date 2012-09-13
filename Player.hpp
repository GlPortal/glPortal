#ifndef __PLAYER_HPP
#define __PLAYER_HPP

class Map; // Forward declare Map due to circular dependency
#include "Map.hpp"
#include "Portal.hpp"

class Player {
public:
	Player();
	void create(float x, float y, float z);
	void update(float dt, bool *keystates, float mousex, float mousey, Map &map);
	void setView();
	bool portalsActive();

	float getX() { return x; }
	float getY() { return y; }
	float getZ() { return z; }
	Portal *getPortals() { return portals; }

protected:
	float x,y,z; 		// Position
	float xrot, yrot;	// Rotation in x- and y-axis
	float xspeed, yspeed, zspeed;
	bool onGround;
	Portal portals[2];
};

#endif
