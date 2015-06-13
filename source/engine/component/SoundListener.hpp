#ifndef COMPONENT_SOUNDLISTENER_HPP
#define COMPONENT_SOUNDLISTENER_HPP

#include "Component.hpp"
#include <engine/core/math/Vector3f.hpp>

namespace glPortal {

class SoundListener : public Component {
public:
  SoundListener(Entity &ent) :
    Component(ent) {}

  // TODO
};

} /* namespace glPortal */

#endif /* COMPONENT_SOUNDLISTENER_HPP */
