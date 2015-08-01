#include "Observer.hpp"
#include <engine/env/Environment.hpp>

namespace glPortal {

void Observer::addCallback(Event event, const std::function<void()> &method) {
  observing.push_back(Environment::dispatcher.addObserver(event, method));
}

Observer::~Observer() {
  for (Dispatcher::CallbackPointer &ptr : observing) {
    Environment::dispatcher.removeObserver(ptr);
  }
}

}
