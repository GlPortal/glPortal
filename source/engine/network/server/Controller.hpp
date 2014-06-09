#ifndef __CONTROLLER_HPP
#define __CONTROLLER_HPP

#include <string>
#include "controllers/StatisticsController.hpp"

namespace glPortal {
  class Controller {
  public:
    Controller();
    std::string setMessage(std::string message);
  private:
    StatisticsController statisticsController;
  };
}
#endif
