#ifndef WORLD_HPP
#define WORLD_HPP

#include <radix/World.hpp>
#include <fstream>

namespace glPortal {

class Screen;

class World : public radix::World {
public:
  World(radix::BaseGame &game, radix::InputSource &input);
  ~World();
};

} /* namespace glPortal */

#endif /* WORLD_HPP */
