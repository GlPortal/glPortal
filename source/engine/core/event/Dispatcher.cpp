#include "Dispatcher.hpp"
#include <algorithm>


namespace glPortal {

Dispatcher::Dispatcher(){

}

void Dispatcher::dispatch(Event event) {
  std::map<Event, std::vector<std::function<void()>>>::iterator it;
  it = eventObserverMap.find(event);
  if (it != eventObserverMap.end()){
    std::vector<std::function<void()>> &observers = it->second;
    for (auto& f : observers) {
      f();
    }
  }
}

void Dispatcher::addObserver(Event event, std::function<void()> &method){
  std::map<Event, std::vector<std::function<void()>>>::iterator it;
  it = eventObserverMap.find(event);
  if (it != eventObserverMap.end()){
    eventObserverMap.at(event).push_back(method);
  } else {
    auto newIt = eventObserverMap.emplace(std::piecewise_construct,
                                          std::forward_as_tuple(event),
                                          std::forward_as_tuple()).first;
    std::vector<std::function<void()>> &observers = newIt->second;
    observers.push_back(method);
  }
}

} /* namespace glPortal */
