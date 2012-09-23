#ifndef __PORTAL_HPP
#define __PORTAL_HPP

#include "Box.hpp"

enum PORTAL_DIR { PD_RIGHT, PD_FRONT, PD_LEFT, PD_BACK, PD_NONE }; /**< Possible direction a portal can face */
enum PORTAL_COLOR { PC_BLUE, PC_ORANGE }; /**< Colors a portal can have */

/**
 * Class for controlling and drawing portals
 */
class Portal {
public:
	Portal() : x(0), y(0), z(0), dir(PD_NONE), active(false) { }

	void place(float x, float y, float z, PORTAL_DIR dir);
	void placeOnBox(Box &box, float hitx, float hity, float hitz);
	void disable();
	void rotateToDir();
	void rotateFromDir();
	void drawStencil();
	void drawOutline(PORTAL_COLOR color);

	float x,y,z;
	PORTAL_DIR dir; /**< Direction portal is facing */
	bool active;	/**< True if the portal is currently active */
};

#endif
