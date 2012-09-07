#ifndef __MAP_HPP
#define __MAP_HPP

#include <vector>
#include "Box.hpp"

class Map {
public:
	Map();
	void draw();

protected:
	std::vector<Box> walls;
};

#endif
