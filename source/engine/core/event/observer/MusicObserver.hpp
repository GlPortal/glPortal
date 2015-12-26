#ifndef MUSIC_OBSERVER_HPP
#define MUSIC_OBSERVER_HPP

#include <engine/core/event/Observer.hpp>
#include <random>
#include <engine/SoundManager.hpp>
#include <engine/env/Environment.hpp>

namespace glPortal {

const std::array<const std::string, 3> MUSIC_PLAYLIST = {
  "/audio/music/track1.ogg",
  "/audio/music/track2.ogg",
  "/audio/music/track3.ogg"
};

class MusicObserver : public Observer{
private:
  std::mt19937 generator;
public:
  void loadMap();
};

} /* namespace glPortal */

#endif /* MUSIC_OBSERVER_HPP */
