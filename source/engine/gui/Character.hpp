#ifndef __CHARACTER_HPP
#define __CHARACTER_HPP

#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/freeglut.h>
#include <string.h>
#include "../Resources.hpp"
//#include <map>

namespace glPortal {
  namespace engine {
    namespace gui {
      class Character {
      public:
	Character(char character);
	void draw(int x, int y);
      private:
	char character;
	int row    ;
	int column ;
	//	std::map<char, std::pair<int,int> > characterMap = {{'A', std::make_pair(1,1)}, {'B',std::make_pair(1,2)}};
      };
    }
  }
}
#endif
