#include "Sheduler.hpp"
#include "Job.hpp"
#include <thread>

namespace glPortal {
  Sheduler::Sheduler(int miliseconds){
    this->interval = miliseconds;
  }

  void Sheduler::add(glPortal::Job *job){
    this->jobs.push_back(job);
  }

  void Sheduler::run(){
    for( auto &i : this->jobs) {
      i->run();;
    }
  }
  
  void Sheduler::start(){
    std::thread thread(&Sheduler::run); 
  }
}
