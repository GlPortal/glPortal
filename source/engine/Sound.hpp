#ifndef SOUND_HPP
#define SOUND_HPP

#include <SDL2/SDL_mixer.h>
#include <string>

namespace glPortal {

class Sound {
private:
  static Mix_Music *music;
public:
  static void init();
  static void load(std::string filename);
  static void play();
  static void pause();
  static void stop();
  static void destroy();
};

} /* namespace glPortal */

#endif /* SOUND_HPP */
