#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP
#include <map>
#include <vector>
#include "Event.hpp"
#include "Observer.hpp"
#include <functional>

namespace glPortal {
  
class Dispatcher {
private:
  //  std::map<Event, std::vector<Observer*>> eventObserverMap;
  std::map<Event, std::vector<std::function<void()>>> eventObserverMap;

  
public:
  Dispatcher();
  void dispatch(Event event);
  //  void addObserver(Event event, Observer *observer);
  void addObserver(Event event, std::function<void()> &method);
  //  inline void addObserver(Event event, Observer &observer) {
  //    addObserver(event, &observer);
  //  }
  //  void removeObserver(Event event, Observer *observer);
  //    inline void removeObserver(Event event, Observer &observer) {
  //    removeObserver(event, &observer);
  //  }

  //  void removeObserver(Event event, std::function<void()> &method);
  void removeObserver(std::function<void()> &method);

  //  inline void removeObserver(Observer &observer) {
  //    removeObserver(&observer);
  //  }
};

} /* namespace glPortal */

#endif /* DISPATCHER_HPP */
