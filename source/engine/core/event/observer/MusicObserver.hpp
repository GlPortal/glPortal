#ifndef MUSIC_OBSERVER_HPP
#define MUSIC_OBSERVER_HPP

#include <engine/core/event/Observer.hpp>

namespace glPortal {

class MusicObserver : public Observer{
public:
  void execute();
};

}

#endif /* MUSIC_OBSERVER_HPP */
