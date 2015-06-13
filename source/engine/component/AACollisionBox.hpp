#ifndef COMPONENT_AACOLLISIONBOX_HPP
#define COMPONENT_AACOLLISIONBOX_HPP

#include "Component.hpp"
#include <engine/core/math/Vector3f.hpp>
#include <engine/BoxCollider.hpp>

namespace glPortal {

class AACollisionBox : public Component {
public:
  BoxCollider box;

  AACollisionBox(Entity &ent) :
    Component(ent) {}
};

} /* namespace glPortal */

#endif /* COMPONENT_AACOLLISIONBOX_HPP */
