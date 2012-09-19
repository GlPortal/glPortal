#ifndef __BOX_HPP
#define __BOX_HPP

#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define MIN(x,y) (((x) < (y)) ? (x) : (y))

enum BOX_TYPE { BT_WALL, BT_TILES, BT_ACID, BT_NONE };

struct Box {
	float x1,y1,z1;
	float x2,y2,z2;
	BOX_TYPE type;

	Box() : x1(0), y1(0), z1(0), x2(0), y2(0), z2(0), type(BT_NONE) {}

	Box(float _x1, float _y1, float _z1, float _x2, float _y2, float _z2, BOX_TYPE _type = BT_NONE) {
		x1 = MIN(_x1, _x2);
		x2 = MAX(_x1, _x2);

		y1 = MIN(_y1, _y2);
		y2 = MAX(_y1, _y2);

		z1 = MIN(_z1, _z2);
		z2 = MAX(_z1, _z2);

		type = _type;
	}

	Box(float *val, BOX_TYPE _type = BT_NONE) {
		x1 = MIN(val[0], val[3]);
		x2 = MAX(val[0], val[3]);

		y1 = MIN(val[1], val[4]);
		y2 = MAX(val[1], val[4]);

		z1 = MIN(val[2], val[5]);
		z2 = MAX(val[2], val[5]);

		type = _type;
	}
	
	bool collide(Box &b) {
		if(b.x1 > x2 || b.x2 < x1 ||b.y1 > y2 || b.y2 < y1 ||b.z1 > z2 || b.z2 < z1)
			return false;
		else
			return true;
	}
};

#endif
