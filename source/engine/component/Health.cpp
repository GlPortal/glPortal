#include "Health.hpp"
#include <engine/core/math/Math.hpp>

namespace glPortal {

Health::Health(Entity &ent) :
  Component(ent), maxHealth(1.f), health(maxHealth) {
}

bool Health::isAlive() {
  return health > 0;
}

void Health::heal(float amount) {
  health = Math::clamp(health + amount, 0.f, maxHealth);
}

void Health::harm(float amount) {
  health = Math::clamp(health - amount, 0.f, maxHealth);
}

void Health::kill() {
  health = 0;
}

void Health::revive() {
  if (not isAlive()) {
    health = 1.f;
  }
}

} /* namespace glPortal */
