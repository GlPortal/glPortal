#ifndef PHYSICS_ENTITY_HPP
#define PHYSICS_ENTITY_HPP

#include <engine/VisualEntity.hpp>

namespace glPortal {

class PhysicsEntity : public VisualEntity {
public:
  // Will be repaced by the chosen physics engine body class
  BoxCollider physBody;
};

} /* namespace glPortal */

#endif /* PHYSICS_ENTITY_HPP */
