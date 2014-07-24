#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "engine/Entity.hpp"
#include "engine/Trigger.hpp"
#include "engine/util/Vector3f.hpp"

namespace glPortal {

class Player: public Entity {
public:
  Player() :
      velocity(0, 0, 0) {
    scale.set(1, 2, 1);
  }
  Vector3f velocity;
};

} /* namespace glPortal */

#endif /* PLAYER_HPP */
