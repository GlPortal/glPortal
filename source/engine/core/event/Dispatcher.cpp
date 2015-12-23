#include "Dispatcher.hpp"

namespace glPortal {

Dispatcher::Dispatcher() {
}

void Dispatcher::dispatch(Event event) {
  ObserverMap::iterator it;
  it = eventObserverMap.find(event);
  if (it != eventObserverMap.end()){
    CallbackList &observers = it->second;
    for (auto &f : observers) {
      f();
    }
  }
}

Dispatcher::CallbackPointer Dispatcher::addObserver(Event event,
                                                    const std::function<void()> &method) {
  ObserverMap::iterator it;
  it = eventObserverMap.find(event);
  if (it != eventObserverMap.end()){
    it->second.push_back(method);
    return std::make_pair(event, --it->second.end());
  } else {
    auto newIt = eventObserverMap.emplace(std::piecewise_construct,
                                          std::forward_as_tuple(event),
                                          std::forward_as_tuple()).first;
    CallbackList &observers = newIt->second;
    observers.push_back(method);
    return std::make_pair(event, --observers.end());
  }
}

void Dispatcher::removeObserver(CallbackPointer &ptr) {
  ObserverMap::iterator it;
  it = eventObserverMap.find(ptr.first);
  if (it != eventObserverMap.end()) {
    CallbackList &observers = it->second;
    observers.erase(ptr.second);
  }
}

} /* namespace glPortal */
