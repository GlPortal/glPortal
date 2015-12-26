#ifndef FLAG_OBSERVER_HPP
#define FLAG_OBSERVER_HPP

#include <engine/core/event/Observer.hpp>

namespace glPortal {

class FlagObserver : public Observer{
private:
  bool resultingStatus;
  bool *flag;

public:
  FlagObserver(bool &flag, bool resultingStatus);
  void execute();
};

} /* namespace glPortal */

#endif /* FLAG_OBSERVER_HPP */
