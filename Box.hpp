#ifndef __BOX_HPP
#define __BOX_HPP

struct Box {
	float x1,y1,z1;
	float x2,y2,z2;
	Box(float _x1, float _y1, float _z1, float _x2, float _y2, float _z2)
		: x1(_x1), y1(_y1), z1(_z1), x2(_x2), y2(_y2), z2(_z2) { }
};

#endif
