#ifndef __FONT_HPP
#define __FONT_HPP

#include <string>
#include "Character.hpp"

namespace glPortal {
  namespace engine {
    namespace gui {

      class Font {
      public:
	Font(int size);
	Font();
	void drawStringToPosition(std::string string, int x, int y);
      };
    }
  }
}
#endif
