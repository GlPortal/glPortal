#include <unittest++/UnitTest++.h>
#include <engine/core/event/Observer.hpp>
#include <engine/core/event/Dispatcher.hpp>
#include <engine/core/event/observer/FlagObserver.hpp>
#include <engine/core/event/Event.hpp>
#include <stdexcept>

using namespace glPortal;

struct DispatcherFixtures {
  Dispatcher dispatcher;
  bool flag;
  DispatcherFixtures() {
    flag = false;
    Dispatcher dispatcher;
    FlagObserver observer(flag, true);

    observer.addCallback(Event::loadScene, std::bind(&FlagObserver::execute, observer));
    dispatcher.dispatch(Event::loadScene);
  }
  
  ~DispatcherFixtures() {}
};


SUITE(DispatcherTest)
{
  TEST_FIXTURE(DispatcherFixtures, dispatchEvent) {
    CHECK(flag);
  }
}

int main()
{
  return UnitTest::RunAllTests();
}
