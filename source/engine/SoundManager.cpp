#include "SoundManager.hpp"

namespace glPortal
{
	
SoundManager::SoundManager() : music(nullptr)
{
	  int audio_rate = 22050;
	  Uint16 audio_format = AUDIO_S16;
	  int audio_channels = 2;
	  int audio_buffers = 4096;

	  if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) == -1) {
		printf("Unable to open audio!\n");
		exit(1);
	  }
}

void SoundManager::PlayMusic(const std::string& filename)
{
	if(music!=nullptr)
	{
		Mix_FreeMusic(music);
		music = nullptr;
	}
		
		
	music = Mix_LoadMUS(filename.c_str());
	if( music == NULL ) 
	{
	printf( "Failed to load Music: SDL_mixer Error: %s\n", Mix_GetError() );
	}
	
	Mix_VolumeMusic(14);
	Mix_Fading(MIX_FADING_IN);
	if(Mix_PlayMusic(music, 0) == -1) 
	{
	printf("Unable to play Ogg file: %s\n", Mix_GetError());
	}
}
	
void SoundManager::PlaySound(const std::string& filename)
{
	Mix_Music* sound = Mix_LoadMUS(filename.c_str());
	if( sound == NULL ) 
	{
	printf( "Failed to load Music: SDL_mixer Error: %s\n", Mix_GetError() );
	}
	
	Mix_VolumeMusic(14);
	Mix_Fading(MIX_FADING_IN);
	if(Mix_PlayMusic(sound, 0) == -1) 
	{
	printf("Unable to play Ogg file: %s\n", Mix_GetError());
	}
	
	sounds.push_back(sound);
}
	
SoundManager::~SoundManager()
{
	for(const auto sound : sounds)
	{
		Mix_FreeMusic(sound);
	}
	if(music!=nullptr)
		Mix_FreeMusic(music);
	Mix_CloseAudio();
}
}
