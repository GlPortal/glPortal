#ifndef __BOX_HPP
#define __BOX_HPP

#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define MIN(x,y) (((x) < (y)) ? (x) : (y))

struct Box {
	float x1,y1,z1;
	float x2,y2,z2;
	Box(float _x1, float _y1, float _z1, float _x2, float _y2, float _z2) {
		x1 = MIN(_x1, _x2);
		x2 = MAX(_x1, _x2);

		y1 = MIN(_y1, _y2);
		y2 = MAX(_y1, _y2);

		z1 = MIN(_z1, _z2);
		z2 = MAX(_z1, _z2);
	}
};

#endif
