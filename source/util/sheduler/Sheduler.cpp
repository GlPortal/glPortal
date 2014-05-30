#include "Sheduler.hpp"
#include "Job.hpp"
//#include <thread>
#include <boost/thread.hpp>
#include <chrono>

namespace glPortal {
  Sheduler::Sheduler(int milliseconds){
    this->interval = milliseconds;
  }

  void Sheduler::add(glPortal::Job *job){
    this->jobs.push_back(job);
  }

  void Sheduler::run(){
    for( auto &i : this->jobs) {
      i->run();;
    }
    boost::this_thread::sleep(boost::posix_time::milliseconds(this->interval));
  }
  
  void Sheduler::start(){
    thread = boost::thread(&Sheduler::run, this);
    thread.join();
  }
}
