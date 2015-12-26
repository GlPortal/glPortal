#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <map>
#include <string>
#include <vector>

#include <SDL2/SDL_mixer.h>

#include <engine/Entity.hpp>

namespace glPortal {

class SoundManager {
public:
  struct SoundInfo {
    Mix_Chunk *chunk;
  };

  static void init();
  static void reload();
  static void destroy();
  
  static void playMusic(const std::string &filename);
  static void playSound(const std::string &filename, const Entity &source);
  
  static void update(const Entity &listener);

private:
  static bool isInitialized, isDisabled;

  static Mix_Music *music;
  static std::map<int, SoundInfo> sounds;
};

} /* namespace glPortal */

#endif /* SOUNDMANAGER_HPP */
