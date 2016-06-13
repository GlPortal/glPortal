#ifndef WORLD_HPP
#define WORLD_HPP

#include <radix/World.hpp>
#include <fstream>

namespace glPortal {

class Screen;

class World : public radix::World {
public:
  std::unique_ptr<Screen> activeScreen;
  std::string gameVersion = getGameVersion();

  std::string getGameVersion(void){
    std::ifstream input("../version.txt");
    std::string gameVersion;
    getline(input,gameVersion);
    return gameVersion;
  }

  World(radix::InputSource &input);
  ~World();
};

} /* namespace glPortal */

#endif /* WORLD_HPP */
