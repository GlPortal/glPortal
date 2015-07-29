#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP
#include <map>
#include <vector>
#include "Event.hpp"
#include "Observer.hpp"

namespace glPortal {

class Dispatcher {
private:
  std::map<Event, std::vector<Observer> > eventObserverMap = {};
public:
  Dispatcher();
  void dispatch(Event event);
  void addObserver(Event event, Observer observer);
};

} /* namespace glPortal */

#endif /* DISPATCHER_HPP */
