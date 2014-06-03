#ifndef __STATISTICS_CONTROLLER_HPP
#define __STATISTICS_CONTROLLER_HPP

#include <string>
#include <vector>
#include "../../../../util/sheduler/Sheduler.hpp"
#include "../../../../util/sheduler/Job.hpp"
#include "../ControllerInterface.hpp"

namespace glPortal {
  class StatisticsController : public ControllerInterface, public Job{
  public:
    StatisticsController();
    std::string setMessage(std::string message);
    std::string loggonAction(std::string message);
    void run();
  private:
    std::vector<std::string> log;
    Sheduler *sheduler;
  };
}
#endif
