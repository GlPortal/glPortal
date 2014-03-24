#include "AudioPlayer.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <stdexcept>
namespace glPortal {
  namespace util{
    namespace audioplayer{
      void AudioPlayer::init(){
	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16; 
	int audio_channels = 2;
	int audio_buffers = 4096;

	SDL_Init(SDL_INIT_AUDIO);

	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
	  printf("Unable to open audio!\n");
	  exit(1);
	}
      }

      void AudioPlayer::playByFileName(std::string filename) {
	music = Mix_LoadMUS("data/audio/music/track1.ogg");
      }

      void AudioPlayer::addSong(std::string filename) {
	tracks.push_back(filename);
      }

      void AudioPlayer::play() {
	Mix_PlayMusic(music, 0);
      }

      void AudioPlayer::pause() {
      }

      void AudioPlayer::stop() {
      }

      void AudioPlayer::cleanUp() {
	Mix_CloseAudio()
      }
    }
  }
}
