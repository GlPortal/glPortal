#ifndef __PLAYER_HPP
#define __PLAYER_HPP

class Player {
public:
	Player();
	void create(float x, float y, float z);
	void update(float dt, bool *keystates, float mousex, float mousey);
	void setView();

	float getX() { return x; }
	float getY() { return y; }
	float getZ() { return z; }

protected:
	float x,y,z; 		// Position
	float xrot, yrot;	// Rotation in x- and y-axis
};

#endif
