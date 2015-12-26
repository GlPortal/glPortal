#include "MusicObserver.hpp"

namespace glPortal {

void MusicObserver::loadMap() {
  //play a random piece of music each time a scene is loaded
  std::uniform_int_distribution<> dis(0, MUSIC_PLAYLIST.size()-1);
  SoundManager::playMusic(Environment::getDataDir() + MUSIC_PLAYLIST[dis(generator)]);
}

} /* namespace glPortal */
