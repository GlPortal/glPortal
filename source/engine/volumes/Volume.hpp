#ifndef VOLUME_HPP
#define VOLUME_HPP

#include <engine/PhysicsEntity.hpp>

#include <string>

namespace glPortal {

class Volume: public PhysicsEntity {
public:
  Volume(): type("None") {}
  Volume(std::string type): type(type) {}

  std::string type;
};

} /* namespace glPortal */

#endif /* VOLUME_HPP */
