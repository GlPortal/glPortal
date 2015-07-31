#include "Dispatcher.hpp"
#include <algorithm>

namespace glPortal {

Dispatcher::Dispatcher(){
  
}
  
void Dispatcher::dispatch(Event event) {
  std::map<Event, std::vector<Observer*>>::iterator it;
  it = eventObserverMap.find(event);
  if (it != eventObserverMap.end()){
    std::vector<Observer*> &observers = it->second;
    for (Observer *obs : observers) {
      obs->execute();
    }
  }
}

void Dispatcher::addObserver(Event event, Observer *observer){
  std::map<Event, std::vector<Observer*>>::iterator it;
  it = eventObserverMap.find(event);
  if (it != eventObserverMap.end()){
    eventObserverMap.at(event).push_back(observer); 
  } else {
    auto newIt = eventObserverMap.emplace(std::piecewise_construct, std::forward_as_tuple(event), std::forward_as_tuple()).first;
    std::vector<Observer*> &observers = newIt->second;
    observers.push_back(observer);
  }
}

void Dispatcher::removeObserver(Event event, Observer *observer) {
  auto it = eventObserverMap.find(event);
  if (it != eventObserverMap.end()) {
    std::vector<Observer*> &observers = it->second;
    std::remove(observers.begin(), observers.end(), observer);
  }
}

void Dispatcher::removeObserver(Observer *observer) {
  for (auto pair : eventObserverMap) {
    std::vector<Observer*> &observers = pair.second;
    std::remove(observers.begin(), observers.end(), observer);
  }
}

} /* namespace glPortal */
