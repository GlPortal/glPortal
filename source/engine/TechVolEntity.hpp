#ifndef TECH_VOL_ENTITY_HPP
#define TECH_VOL_ENTITY_HPP

#include <engine/Entity.hpp>

namespace glPortal {

class TechVolEntity : public Entity {
public:
  BoxCollider collider;
};

} /* namespace glPortal */

#endif /* TECH_VOL_ENTITY_HPP */
