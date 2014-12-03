#ifndef MAPLOADER_HPP
#define MAPLOADER_HPP

#include <string>
#include <tinyxml.h>

namespace glPortal {
class Entity;
class Mesh;
class Scene;

class MapLoader {
public:
  static Scene* getScene(std::string path);
  static Scene* scene;
  static TiXmlHandle rootHandle;
private:
  static Mesh getBox(Entity wall);
  static void extractSpawn();
  static void extractLights();
  static void extractDoor();
  static void extractWalls();
  static void extractTriggers();
  static void extractModels();
  static void extractButtons();
};

} /* namespace glPortal */

#endif /* MAPLOADER_HPP */
