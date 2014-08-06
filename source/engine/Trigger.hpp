#ifndef TRIGGER_HPP
#define TRIGGER_HPP

#include "Entity.hpp"
#include <string>
namespace glPortal {

class Trigger: public Entity {
public:
  Trigger():
    position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1), type("None"){
  }
  Mesh mesh;
  Texture texture;

  std::string type;
  Vector3f position;
  Vector3f rotation;
  Vector3f scale;
};

} /* namespace glPortal */

#endif /* ENTITY_HPP */
