#ifndef COMPONENT_SOUNDSOURCE_HPP
#define COMPONENT_SOUNDSOURCE_HPP

#include "Component.hpp"
#include <engine/core/math/Vector3f.hpp>

namespace glPortal {

class SoundSource : public Component {
public:
  SoundSource(Entity &ent) :
    Component(ent) {}

  // TODO

  void playSound(const std::string &path);
};

} /* namespace glPortal */

#endif /* COMPONENT_SOUNDSOURCE_HPP */
