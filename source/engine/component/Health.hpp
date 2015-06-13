#ifndef COMPONENT_HEALTH_HPP
#define COMPONENT_HEALTH_HPP

#include "Component.hpp"
#include <engine/core/math/Vector3f.hpp>

namespace glPortal {

class Health : public Component {
public:
  float maxHealth;
  float health;

  Health(Entity &ent);
  bool isAlive();
  void heal(float amount);
  void harm(float amount);
  void kill();
  void revive();
};

} /* namespace glPortal */

#endif /* COMPONENT_HEALTH_HPP */
