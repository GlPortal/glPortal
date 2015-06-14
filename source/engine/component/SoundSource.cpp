#include "SoundSource.hpp"
#include <engine/SoundManager.hpp>

namespace glPortal {

void SoundSource::playSound(const std::string &path) {
  SoundManager::PlaySound(path, entity);
}

} /* namespace glPortal */