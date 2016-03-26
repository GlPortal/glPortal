#ifndef WORLD_HPP
#define WORLD_HPP

#include <radix/World.hpp>

namespace glPortal {

class Screen;

class World : public radix::World {
public:
  std::unique_ptr<Screen> activeScreen;

  World(radix::InputSource &input);
  ~World();
};

} /* namespace glPortal */

#endif /* WORLD_HPP */
