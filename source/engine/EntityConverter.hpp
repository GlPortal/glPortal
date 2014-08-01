#ifndef ENTITYCONVERTER_HPP
#define ENTITYCONVERTER_HPP

#include <string>

namespace glPortal {
class Entity;
class Mesh;
class Scene;

class EntityConverter {
public:
  static Mesh getMeshForBox(Entity wall);
};

} 

#endif 
