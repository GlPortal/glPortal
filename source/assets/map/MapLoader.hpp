#ifndef MAPLOADER_HPP
#define MAPLOADER_HPP

#include <string>
#include <tinyxml2.h>

using namespace tinyxml2;

namespace glPortal {
class Entity;
class Mesh;
class Scene;

class MapLoader {
public:
  static Scene* getSceneFromPath(const std::string &path);
  static Scene* getScene(const std::string &path);
  static Scene* scene;
  static XMLHandle rootHandle;
private:
  static Mesh getBox(const Entity &wall);
  static void extractMaterials();
  static void extractSpawn();
  static void extractLights();
  static void extractDoor();
  static void extractWalls();
  static void extractAcids();
  static void extractTriggers();
  static void extractModels();
};

} /* namespace glPortal */

#endif /* MAPLOADER_HPP */
