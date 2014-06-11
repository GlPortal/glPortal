#ifndef MAPLOADER_HPP
#define MAPLOADER_HPP

#include "../Scene.hpp"
#include <string>

namespace glPortal {

class MapLoader {
public:
  static Scene* getScene(std::string path);
private:
  static Mesh getBox(Entity wall);
};

} /* namespace glPortal */

#endif /* MAPLOADER_HPP */
