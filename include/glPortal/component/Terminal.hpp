#ifndef COMPONENT_LIGHTSOURCE_HPP
#define COMPONENT_LIGHTSOURCE_HPP

#include <radix/component/Component.hpp>

#include <serine/Archiver.hpp>

#include <radix/core/math/Vector4f.hpp>

namespace glPortal {

class Terminal : public radix::Component {
public:
  bool enabled;
  radix::Vector4f backgroundColor;
  radix::Vector4f textColor;

  Terminal(radix::Entity &ent) :
    Component(ent),
    enabled(false),
    backgroundColor(0, 0, 0, 1),
    textColor(1, 1, 1, 1) {
  }

  const char* getName() const {
    return "Terminal";
  }

  TypeId getTypeId() const {
    return Component::getTypeId<std::remove_reference<decltype(*this)>::type>();
  }

  void serialize(serine::Archiver &ar) {
    ar("enabled", enabled);
    ar("backgroundColor", backgroundColor);
    ar("textColor", textColor);
  }
};

} /* namespace radix */

#endif /* COMPONENT_LIGHTSOURCE_HPP */
