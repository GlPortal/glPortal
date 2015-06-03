#ifndef VISUAL_ENTITY_HPP
#define VISUAL_ENTITY_HPP

#include <engine/Entity.hpp>
#include <assets/material/Material.hpp>

namespace glPortal {

class VisualEntity : public Entity {
public:
  Mesh mesh;
  // FIXME: remove? Other class?
  Texture texture;
  Material material;
};

} /* namespace glPortal */

#endif /* VISUAL_ENTITY_HPP */
