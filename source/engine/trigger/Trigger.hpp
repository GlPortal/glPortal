#ifndef TRIGGER_HPP
#define TRIGGER_HPP

#include <string>

#include <assets/model/Mesh.hpp>
#include <assets/texture/Texture.hpp>

#include <engine/Entity.hpp>

#include <engine/core/math/Vector3f.hpp>

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
