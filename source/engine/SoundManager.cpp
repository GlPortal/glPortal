#include "SoundManager.hpp"

#include <cstdio>
#include <SDL2/SDL.h>

namespace glPortal {

bool SoundManager::isInitialized = false;
bool SoundManager::isDisabled = true;

Mix_Music *SoundManager::music = nullptr;
std::map<int, SoundManager::SoundInfo> SoundManager::sounds = {};

void SoundManager::init() {
  if (isInitialized) {
    System::Log(Info, "SoundManager") << "init() called when already initialized";
    return;
  }

  isDisabled = false;

  if (SDL_WasInit(SDL_INIT_AUDIO)) {
    System::Log(Verbose, "SoundManager") << "SDL Audio system already initialized";
  } else {
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
      System::Log(Warning, "SoundManager") << "SDL Audio system init failed: "
                                           << SDL_GetError() << ", sound disabled";
      isDisabled = true;
    } else {
      System::Log(Info, "SoundManager") << "SDL Audio system initialized";
    }
  }

  music = nullptr;
  sounds.clear();

  if (not isDisabled) {
    int flags = MIX_INIT_OGG;

    if ((Mix_Init(flags) & flags) != flags) {
      System::Log(Warning, "SoundManager") << "SDL_mixer Init failed: "
                                           << Mix_GetError() << ", sound disabled";
      isDisabled = true;
    }
  }

  if (not isDisabled) {
    int audioRate = 22050;
    Uint16 audioFormat = AUDIO_S16;
    int audioChannels = 2;
    int audioBuffers = 4096;

    if (Mix_OpenAudio(audioRate, audioFormat, audioChannels, audioBuffers) == -1) {
      System::Log(Warning, "SoundManager") << "SDL_mixer OpenAudio failed: "
                                           << Mix_GetError() << ", sound disabled";
      isDisabled = true;
    }
  }

  System::Log(Verbose, "SoundManager") << "fully initialized";
  isInitialized = true;
}

void SoundManager::playMusic(const std::string &filename) {
  if (isDisabled) {
    return;
  }

  if (music != nullptr) {
    Mix_FreeMusic(music);
    music = nullptr;
  }

  music = Mix_LoadMUS(filename.c_str());
  if (music == NULL) {
    System::Log(Warning, "SoundManager") << "music load failed: " << Mix_GetError();
    return;
  }

  Mix_VolumeMusic(14);

  if (Mix_PlayMusic(music, 0) == -1) {
    System::Log(Warning, "SoundManager") << "music play failed: " << Mix_GetError();
    return;
  }
}

void SoundManager::playSound(const std::string &filename, const Entity &source) {
  if (isDisabled) {
    return;
  }

  Mix_Chunk *sound = Mix_LoadWAV(filename.c_str());
  if (sound == nullptr) {
    System::Log(Warning, "SoundManager") << "sound load failed: " << Mix_GetError();
    return;
  }

  Mix_VolumeMusic(14);
  int channel = Mix_PlayChannel(-1, sound, 0);

  if (channel == -1) {
    System::Log(Warning, "SoundManager") << "sound play failed: " << Mix_GetError();
    return;
  }

  SoundInfo &info = sounds[channel];
  info.chunk = sound;
}

void SoundManager::update(const Entity &listener) {
  if (isDisabled) {
    return;
  }

  std::vector<int> erase_list;

  for (auto sound : sounds) {
    if (not Mix_Playing(sound.first)) {
      Mix_FreeChunk(sound.second.chunk);
      erase_list.push_back(sound.first);
    }
  }

  for (auto key : erase_list) {
    sounds.erase(key);
  }
}

void SoundManager::destroy() {
  if (isInitialized) {
    System::Log(Info, "SoundManager") << "destroy() called when uninitialized";
    return;
  }

  for (const auto sound : sounds) {
    Mix_FreeChunk(sound.second.chunk);
  }
  if (music != nullptr) {
    Mix_FreeMusic(music);
  }
  Mix_CloseAudio();
  SDL_QuitSubSystem(SDL_INIT_AUDIO);

  isInitialized = false;
}

void SoundManager::reload() {
  SoundManager::destroy();
  SoundManager::init();
}

}
