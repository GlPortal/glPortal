#ifndef OBSERVER_HPP
#define OBSERVER_HPP
#include <functional>
#include <vector>
#include "Dispatcher.hpp"

namespace glPortal {

class Observer {
private:
  std::vector<Dispatcher::CallbackPointer> observing;

public:
  void addCallback(Event event, const std::function<void()> &method);
  ~Observer();
};

}

#endif /* OBSERVER_HPP */
