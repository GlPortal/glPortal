#ifndef __CONTROLLER_INTERFACE_HPP
#define __CONTROLLER_INTERFACE_HPP

#include <string>
#include <vector>

namespace glPortal {
  class ControllerInterface {
  public:
    virtual std::string setMessage(std::string message) = 0;
  private:
  };
}
#endif
