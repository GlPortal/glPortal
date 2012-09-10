#ifndef __PORTAL_HPP
#define __PORTAL_HPP

enum PORTAL_DIR { RIGHT, FRONT, LEFT, BACK };

struct Portal {
	float x,y,z;
	PORTAL_DIR dir;
};

#endif
