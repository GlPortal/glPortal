#include "Observer.hpp"
#include <engine/env/Environment.hpp>

namespace glPortal {

Observer::~Observer() {
  //  Environment::dispatcher.removeObserver(this);
}

}
