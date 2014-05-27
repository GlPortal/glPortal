#include "AudioPlayer.hpp"
#include "Environment.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <stdexcept>


using namespace glPortal::engine;


namespace glPortal {
  namespace util{
    namespace audioplayer{
      void AudioPlayer::init(){
	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16; 
	int audio_channels = 2;
	int audio_buffers = 4096;

	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) == -1) {
	  printf("Unable to open audio!\n");
	}
      }

      void AudioPlayer::playByFileName(std::string filename) {
	music = Mix_LoadMUS(filename.c_str());
	if( music == NULL )
	  {
	    printf( "Failed to load Music: SDL_mixer Error: %s\n", Mix_GetError() );
	  }
      }

      void AudioPlayer::addSong(std::string filename) {
	tracks.push_back(filename);
      }

      void AudioPlayer::play() {
	Mix_VolumeMusic(14);
	Mix_Fading(MIX_FADING_IN);
	if(Mix_PlayMusic(music, 0) == -1) { printf("Unable to play Ogg file: %s\n", Mix_GetError()); } 
      }

      void AudioPlayer::pause() {
      }

      void AudioPlayer::stop() {
      }

      void AudioPlayer::cleanUp() {
	Mix_FreeMusic(music);
	Mix_CloseAudio();
      }
    }
  }
}
