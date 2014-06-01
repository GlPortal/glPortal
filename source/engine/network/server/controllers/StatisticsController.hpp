#ifndef __STATISTICS_CONTROLLER_HPP
#define __STATISTICS_CONTROLLER_HPP

#include <string>
#include <vector>

namespace glPortal {
  class StatisticsController {
  public:
    std::string setMessage(std::string message);
  private:
    std::vector<std::string> log;
  };
}
#endif
