#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP
 
#include <SDL2/SDL_mixer.h>
#include <string>
#include <map>
#include <vector>
#include <engine/Entity.hpp>

namespace glPortal {

class SoundManager {
public:
  struct SoundInfo {
    Mix_Chunk *chunk;
  };

  static void Init();
  static void Destroy();
  
  static void PlayMusic(const std::string &filename);
  static void PlaySound(const std::string &filename, const Entity &source);
  
  static void Update(const Entity &listener);
private:
  static Mix_Music *music;
  static std::map<int ,SoundInfo> sounds;
};

} /* namespace glPortal */

#endif /* SOUNDMANAGER_HPP */
