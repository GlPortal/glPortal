#ifndef MAPLOADER_HPP
#define MAPLOADER_HPP

#include "../Scene.hpp"
#include <string>

namespace glPortal {

class MapLoader {
public:
  static Scene* getScene(std::string path);
};

} /* namespace glPortal */

#endif /* MAPLOADER_HPP */
