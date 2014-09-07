#ifndef TRIGGER_HPP
#define TRIGGER_HPP

#include <string>

#include "Entity.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include <util/math/Vector3f.hpp>

namespace glPortal {

class Trigger: public Entity {
public:
  Trigger(): type("None") {

  }
  Mesh mesh;
  Texture texture;

  std::string type;
};

} /* namespace glPortal */

#endif /* TRIGGER_HPP */
