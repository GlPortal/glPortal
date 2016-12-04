#ifndef WORLD_HELPER_HPP
#define WORLD_HELPER_HPP

#include <utility>

#include <radix/World.hpp>

namespace glPortal {

class WorldHelper {
public:
  static void shootPortal(int button, radix::World &world);
  static radix::EntityPair& getPortalPair(int pair, radix::World &world);
  static void closePortals(radix::World &world);
};

} /* namespace glPortal */

#endif /* WORLD_HELPER_HPP */
