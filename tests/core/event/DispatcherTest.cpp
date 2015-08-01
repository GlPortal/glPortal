#include <unittest++/UnitTest++.h>
#include <engine/core/event/Observer.hpp>
#include <engine/core/event/Dispatcher.hpp>
#include <engine/core/event/observer/FlagObserver.hpp>
#include <engine/core/event/Event.hpp>
#include <stdexcept>
#include <functional>

using namespace glPortal;

Dispatcher dispatcher;

// Fix, since we 1. don't compile Observer.cpp and 2. don't want to go into the C++ dependency
// hell it implies (references Environment).
void Observer::observe(Event event, const std::function<void()> &method) {
  observing.push_back(dispatcher.addObserver(event, method));
}
Observer::~Observer() {
  for (Dispatcher::CallbackPointer &ptr : observing) {
    dispatcher.removeObserver(ptr);
  }
}

struct DispatcherFixtures {
  bool flag;
  DispatcherFixtures() {
    flag = false;
    FlagObserver observer(flag, true);
    observer.observe(Event::loadScene, std::bind(&FlagObserver::execute, observer));
    dispatcher.dispatch(Event::loadScene);
  }
  
  ~DispatcherFixtures() {}
};

SUITE(DispatcherTest)
{
  TEST_FIXTURE(DispatcherFixtures, dispatchEvent){

    CHECK(flag);
  }
}

int main()
{
  return UnitTest::RunAllTests();
}
