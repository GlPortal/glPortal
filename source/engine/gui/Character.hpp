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
	  {'A',std::make_pair(1,1)}, {U'Ä',std::make_pair(5,4)}, {'B',std::make_pair(1,2)}, {'C',std::make_pair(1,3)}, 
	  {'D',std::make_pair(1,4)}, {'E',std::make_pair(1,5)}, {'F',std::make_pair(1,6)}, {'G',std::make_pair(1,7)}, 
	  {'H',std::make_pair(1,8)}, {'I',std::make_pair(1,9)}, {'J',std::make_pair(1,10)}, {'K',std::make_pair(1,11)}, 
	  {'L',std::make_pair(1,12)}, {'M',std::make_pair(1,13)}, {'N',std::make_pair(1,14)}, {'O',std::make_pair(1,15)}, 
	  {U'Ö',std::make_pair(6,8 )}, {'P',std::make_pair(1,16)}, {'Q',std::make_pair(1,17)}, {'R',std::make_pair(1,18)}, 
	  {'S',std::make_pair(1,19)}, {'T',std::make_pair(1,20)}, {'U',std::make_pair(1,21)}, {U'Ü',std::make_pair(6,18)}, 
	  {'V',std::make_pair(1,22)}, {'W',std::make_pair(1,23)}, {'X',std::make_pair(1,24)}, {'Y',std::make_pair(1,25)}, 
	  {'Z',std::make_pair(1,26)},{'a',std::make_pair(2,1)}, {U'ä',std::make_pair(5,10)}, {'b',std::make_pair(2,2)}, 
	  {'c',std::make_pair(2,3)}, {'d',std::make_pair(2,4)}, {'e',std::make_pair(2,5)}, {'f',std::make_pair(2,6)}, 
	  {'g',std::make_pair(2,7)}, {'h',std::make_pair(2,8)}, {'i',std::make_pair(2,9)}, {'j',std::make_pair(2,10)}, 
	  {'k',std::make_pair(2,11)}, {'l',std::make_pair(2,12)}, {'m',std::make_pair(2,13)}, {'n',std::make_pair(2,14)}, 
	  {'o',std::make_pair(2,15)}, {U'ö',std::make_pair(6,13)}, {'p',std::make_pair(2,16)}, {'q',std::make_pair(2,17)}, 
	  {'r',std::make_pair(2,18)}, {'s',std::make_pair(2,19)}, {'t',std::make_pair(2,20)}, {'u',std::make_pair(2,21)}, 
	  {U'ü',std::make_pair(6,22)}, {'v',std::make_pair(2,22)}, {'w',std::make_pair(2,23)}, {'x',std::make_pair(2,24)}, 
	  {'y',std::make_pair(2,25)}, {'z',std::make_pair(2,26)}, {'0',std::make_pair(4,1)},{'1',std::make_pair(4,2)},
	  {'2',std::make_pair(4,3)}, {'3',std::make_pair(4,4)}, {'4',std::make_pair(4,5)},{'5',std::make_pair(4,6)},
	  {'6',std::make_pair(4,7)}, {'7',std::make_pair(4,8)}, {'8',std::make_pair(4,9)},{'9',std::make_pair(4,10)},
	  {':',std::make_pair(4,11)}, {';',std::make_pair(4,12)}
	};
      };
    }
  }
}
#endif
