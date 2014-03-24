#ifndef __AUDIOPLAYER_HPP
#define __AUDIOPLAYER_HPP
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

namespace glPortal {
  namespace util{
    namespace audioplayer{
      class AudioPlayer {
      public:
	void init();
	void playByFileName(std::string filename);
	void addSong(std::string filename);
	void play();
	void pause();
	void stop();
	void cleanUp();
      private:
	std::vector<std::string> tracks;
	Mix_Music *music = NULL;
      };
    }
  }
}
#endif
