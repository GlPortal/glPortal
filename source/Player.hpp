#ifndef __PLAYER_HPP
#define __PLAYER_HPP

#include "GameMap.hpp"
#include "Portal.hpp"
#include "Shot.hpp"

enum PLAYER_STATE { PS_ALIVE, PS_DYING, PS_WON };

/**
 * Class for controlling the player and camera view
 */
class Player {
public:
	Player();
	void create(float x, float y, float z);
        void update(float dt, bool *keystates, float mousex, float mousey, GameMap &gameMap);
	void mousePressed(int key);
	void setView();
	bool portalsActive();
	void drawPortalStencils();
	void drawPortalOutlines();
	void drawShots();

	float getX() { return x; } /**< Get the X-coordinate of the Player */
	float getY() { return y; } /**< Get the Y-coordinate of the Player */
	float getZ() { return z; } /**< Get the Z-coordinate of the Player */
	Portal *getPortals() { return portals; } /**< Gets the list of portals */
	Shot *getShots() { return shots; } /**< Gets the list of shots */
	PLAYER_STATE getState() { return state; } /**< Gets the player's current state */

protected:
	float x,y,z;  // Position
	float xrot; /**< View rotation in X-axis */
	float yrot;	/**< View rotation in Y-axis */
	float xspeed, yspeed, zspeed; // Velocity in x-, y- and z-axis
	bool onGround;	/**< True if player was standing on ground last frame */
	PLAYER_STATE state; /**< Players current state e.g. alive, dead etc. */
	Portal portals[2];  /**< Array of player's portal */
	Shot shots[2];		/**< Array of shots */
};

#endif
