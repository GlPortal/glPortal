#ifndef __CHARACTER_HPP
#define __CHARACTER_HPP

#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/freeglut.h>
#include <string.h>
#include "../Resources.hpp"
#include <map>

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
       	std::map<char, std::pair<int,int> > characterMap = {
	  {'A',std::make_pair(1,1)}, 
	  {'B',std::make_pair(1,2)}, 
	  {'C',std::make_pair(1,3)}, 
	  {'D',std::make_pair(1,4)}, 
	  {'E',std::make_pair(1,5)}, 
	  {'F',std::make_pair(1,6)}, 
	  {'G',std::make_pair(1,7)}, 
	  {'H',std::make_pair(1,8)}, 
	  {'I',std::make_pair(1,9)}, 
	  {'J',std::make_pair(1,10)}, 
	  {'K',std::make_pair(1,11)}, 
	  {'L',std::make_pair(1,12)}, 
	  {'M',std::make_pair(1,13)}, 
	  {'N',std::make_pair(1,14)}, 
	  {'O',std::make_pair(1,15)}, 
	  {'P',std::make_pair(1,16)}, 
	  {'Q',std::make_pair(1,17)}, 
	  {'R',std::make_pair(1,18)}, 
	  {'S',std::make_pair(1,19)}, 
	  {'T',std::make_pair(1,20)}, 
	  {'U',std::make_pair(1,21)}, 
	  {'V',std::make_pair(1,22)}, 
	  {'W',std::make_pair(1,23)}, 
	  {'X',std::make_pair(1,24)}, 
	  {'Y',std::make_pair(1,25)}, 
	  {'Z',std::make_pair(1,26)}
	};
      };
    }
  }
}
#endif
