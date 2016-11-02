#ifndef COMPONENT_SCREEN_HPP
#define COMPONENT_SCREEN_HPP

#include <radix/component/Component.hpp>

#include <serine/Archiver.hpp>

#include <radix/core/math/Vector4f.hpp>

namespace glPortal {

class Screen : public radix::Component {
public:
  std::string title;
  std::string text;
  bool enabled;
  float alpha;
  Vector4f backgroundColor;
  Vector4f textColor;

  Screen(radix::Entity &ent) :
    Component(ent),
    enabled(false),
    backgroundColor(0, 0, 0, 1),
    textColor(1, 1, 1, 1) {
  }

  const char* getName() const {
    return "Screen";
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

#endif /* COMPONENT_SCREEN_HPP */
