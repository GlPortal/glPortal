#include "Lamp.hpp"
#include <iostream>
#include <cstdlib>
namespace glPortal {
  namespace map{
    namespace objects{
      Lamp::Lamp(float (&position)[4]){
	this->x = position[0];
	this->y = position[1];
	this->z = position[2];
	this->w = position[3];
      }

      float* Lamp::getPosition(){
	float position[3];
	position[0] = this->x;
	position[1] = this->y;
	position[2] = this->z;
	position[3] = this->w;
	
	return position;
      }
    }
  }
}
