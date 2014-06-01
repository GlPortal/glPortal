#ifndef __CONTROLLER_HPP
#define __CONTROLLER_HPP

#include <string>
#include "controllers/StatisticsController.hpp"

namespace glPortal {
  class Controller {
  public:
    std::string setMessage(std::string message);
  private:
    StatisticsController statisticsController;
  };
}
#endif
