#include "Dispatcher.hpp"

namespace glPortal {

Dispatcher::Dispatcher(){
  
}
  
void Dispatcher::dispatch(Event event) {
  std::vector<Observer> observers = eventObserverMap.at(event);
  for (std::vector<Observer>::iterator it = observers.begin() ; it != observers.end(); ++it){
    Observer observer = (Observer)*it;
    observer.execute();
  }
  
}

void Dispatcher::addObserver(Event event, Observer observer){
  std::map<Event, std::vector<Observer> >::iterator it;
  it = eventObserverMap.find(event);
  if (it != eventObserverMap.end()){
    eventObserverMap.at(event).push_back(observer); 
  } else {
    std::vector<Observer> observers;
    observers.push_back(observer);
    eventObserverMap.insert(std::pair<Event,std::vector<Observer> >(event,observers));
  }


}
} /* namespace glPortal */
