#ifndef __TIMER_HPP
#define __TIMER_HPP

#include <string>
#include <ctime>

namespace glPortal {
  namespace engine {
    namespace tools {
      class Timer {
      public:
	Timer();
	void start();
	void stop();
	std::string getTimeString();
	std::string getStoppedTimeString();
      private:
	std::time_t startTime;
	std::time_t stopTime;
      };
    }
  }
}
#endif
