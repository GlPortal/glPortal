#ifndef MAPLOADER_HPP
#define MAPLOADER_HPP

#include <string>

namespace glPortal {
class Entity;
class Mesh;
class Scene;

class MapLoader {
public:
  static Scene* getScene(std::string path);
private:
  static Mesh getBox(Entity wall);
};

} /* namespace glPortal */

#endif /* MAPLOADER_HPP */
