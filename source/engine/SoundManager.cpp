#include "SoundManager.hpp"

namespace glPortal {
  Mix_Music *SoundManager::music = NULL;
  std::map<int, SoundManager::SoundInfo> SoundManager::sounds = {};

  void SoundManager::Init() {
    music = nullptr;
    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16;
    int audio_channels = 2;
    int audio_buffers = 4096;

    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) == -1) {
      printf("Unable to open audio!\n");
      exit(1);
    }
  }

  void SoundManager::PlayMusic(const std::string& filename) {
    if (music!=nullptr) {
      Mix_FreeMusic(music);
      music = nullptr;
    }


    music = Mix_LoadMUS(filename.c_str());
    if (music == NULL) {
      printf( "Failed to load Music: SDL_mixer Error: %s\n", Mix_GetError() );
      return;
    }

    Mix_VolumeMusic(14);
    Mix_Fading(MIX_FADING_IN);

    if (Mix_PlayMusic(music, 0) == -1) {
      printf("Unable to play Ogg file: %s\n", Mix_GetError());
      return;
    }
  }

  void SoundManager::PlaySound(const std::string& filename, Player* p, SoundType type) {
    if (p->getPlayingSound()==true && type == PRIMARY)
      return;

    Mix_Chunk* sound = Mix_LoadWAV(filename.c_str());
    if (sound == NULL) {
      printf( "Failed to load Sound: SDL_mixer Error: %s\n", Mix_GetError() );
      return;
    }

    Mix_VolumeMusic(14);
    Mix_Fading(MIX_FADING_IN);
    int channel = Mix_PlayChannel(-1, sound, 0);

    if (channel == -1) {
      printf("Unable to play Ogg file: %s\n", Mix_GetError());
      return;
    }

    if (type==PRIMARY)
      p->setPlayingSound(true);

    SoundInfo info;
    info.chunk = sound;
    info.type = type;
    sounds[channel] = info;
  }

  void SoundManager::Update(Player* p) {
    std::vector<int> erase_list;

    for (auto sound : sounds) {
      if (not Mix_Playing(sound.first)) {
        p->setPlayingSound(false);
        Mix_FreeChunk(sound.second.chunk);
        erase_list.push_back(sound.first);
      }
    }

    for (auto key: erase_list) {
      sounds.erase(key);
    }
  }

  void SoundManager::Destroy() {
    for (const auto sound : sounds) {
      Mix_FreeChunk(sound.second.chunk);
    }
    if (music!=nullptr)
      Mix_FreeMusic(music);
    Mix_CloseAudio();
  }
}
