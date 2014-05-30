#include "Sheduler.hpp"
#include "Job.hpp"

namespace glPortal {
  Sheduler::Sheduler(int miliseconds){
    this->interval = miliseconds;
  }

  void Sheduler::add(glPortal::Job *job){
    this->jobs.push_back(job);
  }

  void Sheduler::run(){
    /*    for( auto &i : this->jobs) {
      i.run();;
      }*/
  }
}
