#ifndef WORLD_HPP
#define WORLD_HPP

#include <memory>

#include <radix/World.hpp>

namespace glPortal {

class Screen;

class World : public radix::World {
public:
  World(radix::BaseGame &game);
  ~World();
};

} /* namespace glPortal */

#endif /* WORLD_HPP */
