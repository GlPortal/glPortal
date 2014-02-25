#ifndef __MAPFILEPARSER_HPP
#define __MAPFILEPARSER_HPP

#include <vector>
#include <cstring>
#include <cstdlib> 
#include "../GameMap.hpp"

namespace glPortal {
  namespace map{
    class MapFileParser{
    private:
      GameMap gameMap;
      GLfloat lightpos[4];
      float startpos[3];
      float cakepos[3];	
      Box cakeBox;	
    public:
      GameMap getMapFromFile(const char *filename);
    };
  }
}

#endif
