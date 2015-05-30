#ifndef VISUAL_ENTITY_HPP
#define VISUAL_ENTITY_HPP

#include <engine/Entity.hpp>

namespace glPortal {

class VisualEntity : public Entity {
public:
  Mesh mesh;
  Texture texture;
};

} /* namespace glPortal */

#endif /* VISUAL_ENTITY_HPP */
