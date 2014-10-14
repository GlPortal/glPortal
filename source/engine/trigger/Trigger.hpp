#ifndef TRIGGER_HPP
#define TRIGGER_HPP

#include <string>

#include <engine/Entity.hpp>
#include <engine/Mesh.hpp>
#include <engine/Texture.hpp>
#include <util/math/Vector3f.hpp>

namespace glPortal {

class Trigger: public Entity {
public:
  Trigger();
  Mesh mesh;
  Texture texture;

  std::string type;
};

} /* namespace glPortal */

#endif /* TRIGGER_HPP */
