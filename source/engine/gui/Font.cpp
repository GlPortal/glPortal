#include "Font.hpp"
#include <iostream> 

namespace glPortal {
  namespace engine {
    namespace gui {
      Font::Font(int size) {
	      //do nothing
      }

      Font::Font() {
	      //do nothing
      }

      void Font::drawStringToPosition(std::string string, int x, int y) {
	    std::string::const_iterator iterator = string.begin();
	    int charNumber = 0;
	    while(iterator != string.end()) 
	      {
	        Character test(*iterator);
	        test.draw(x + (charNumber * 30), y);
	        iterator++;        
	        charNumber++;
	      }
      }
    }
  }
} 

