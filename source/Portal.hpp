#ifndef PORTAL_HPP
#define PORTAL_HPP

#include "engine/Entity.hpp"
#include "engine/util/Vector3f.hpp"

namespace glPortal {

class Portal: public Entity {
public:
  Portal() :
      open(false) {}
  Vector3f direction;
  bool open;
};

} /* namespace glPortal */

#endif /* PORTAL_HPP */
