#ifndef COMPONENT_TRIGGER_HPP
#define COMPONENT_TRIGGER_HPP

#include "Component.hpp"
#include <engine/core/math/Vector3f.hpp>

namespace glPortal {

class Trigger : public Component {
public:
  // FIXME: no types, scripts
  std::string type;
  // duk_c_function script;
  // TODO: EntityFilter filter;

  Trigger(Entity &ent) :
    Component(ent) {}

  void onEnter(Entity&);
  void onMove(Entity&);
  void onExit(Entity&);
  void onTouchStart(Entity&);
  void onTouchEnd(Entity&);
  void onUseStart(Entity&);
  void onUseEnd(Entity&);
};

} /* namespace glPortal */

#endif /* COMPONENT_TRIGGER_HPP */
