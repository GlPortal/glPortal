#include <engine/Sound.hpp>
#include <cstdio>
#include <cstdlib>

namespace glPortal {

Mix_Music *Sound::music = NULL;

void Sound::init() {
  int audio_rate = 22050;
  Uint16 audio_format = AUDIO_S16;
  int audio_channels = 2;
  int audio_buffers = 4096;

  if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) == -1) {
    printf("Unable to open audio!\n");
    exit(1);
  }
}

void Sound::load(std::string filename) {
  music = Mix_LoadMUS(filename.c_str());
  if( music == NULL ) {
    printf( "Failed to load Music: SDL_mixer Error: %s\n", Mix_GetError() );
  }
}

void Sound::play() {
  Mix_VolumeMusic(14);
  Mix_Fading(MIX_FADING_IN);
  if(Mix_PlayMusic(music, -1) == -1) {
    printf("Unable to play Ogg file: %s\n", Mix_GetError());
  }
}

void Sound::pause() {

}

void Sound::stop() {

}

void Sound::destroy() {
  Mix_FreeMusic(music);
  Mix_CloseAudio();
}

} /* namespace glPortal */
