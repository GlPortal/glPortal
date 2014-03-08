#include "Timer.hpp"
#include <iostream> 

namespace glPortal {
  namespace engine {
    namespace tools {
      Timer::Timer(){}

      void Timer::start(){
	      std::time_t currentTime = std::time(NULL);
	      this->startTime = currentTime;
      }
      void Timer::stop(){
	      std::time_t currentTime = std::time(NULL);
	      this->stopTime = currentTime;
      }

      std::string Timer::getTimeString() {
	      struct tm * timeinfo;
	      std::time_t currentTime = std::time(NULL);
	      std::time_t timeDifference = currentTime - this->startTime;
	      char date[20];
	      timeinfo = localtime(&timeDifference);
	      strftime(date, sizeof(date), "%M:%S", timeinfo);

	      std::string resultString = std::string(date);
	      return resultString;
      }

      std::string Timer::getStoppedTimeString(){	
	      struct tm * timeinfo;
	      std::time_t timeDifference = this->stopTime - this->startTime;
	      char date[20];
	      timeinfo = localtime(&timeDifference);
	      strftime(date, sizeof(date), "%M:%S", timeinfo);
	
	      std::string resultString = std::string(date);
	      return resultString;
      } 
    }
  }
}
