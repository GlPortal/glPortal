#ifndef __LAMP_HPP
#define __LAMP_HPP

#include <vector>
#include <cstring>
#include <cstdlib> 

namespace glPortal {
  namespace map{
    namespace objects{
      class Lamp{
      private:
	float x, y, z, w;
      public:
	Lamp(float (&position)[4]);	     
float* getPosition();
      };
    }
  }
}

#endif
