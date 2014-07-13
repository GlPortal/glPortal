#ifndef XMLMAPLOADER_HPP
#define XMLMAPLOADER_HPP

#include <string>

namespace glPortal {
class Entity;
class Mesh;
class Scene;

class XmlMapLoader {
public:
  static Scene* getScene(std::string path);
private:
  static Mesh getBox(Entity wall);
};

} 

#endif 
