#ifndef __PLAYER_HPP
#define __PLAYER_HPP

#include "Map.hpp"
#include "Portal.hpp"
#include "Shot.hpp"

/**
 * Class for controlling the player and camera view
 */
class Player {
public:
	Player();
	void create(float x, float y, float z);
	void update(float dt, bool *keystates, float mousex, float mousey, Map &map);
	void mousePressed(int key);
	void setView();
	bool portalsActive();
	void drawPortalStencils();
	void drawPortalOutlines(GLuint *textures);
	void drawShots(GLuint *textures);

	float getX() { return x; }
	float getY() { return y; }
	float getZ() { return z; }
	Portal *getPortals() { return portals; }

protected:
	float x,y,z; 		// Position
	float xrot, yrot;	// Rotation in x- and y-axis
	float xspeed, yspeed, zspeed; // Velocity in x-, y- and z-axis
	bool onGround;		// True if player was standing on ground last frame
	Portal portals[2];
	Shot shots[2];
};

#endif
