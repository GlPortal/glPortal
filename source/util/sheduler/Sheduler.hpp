#ifndef __SHEDULER_HPP
#define __SHEDULER_HPP

#include <cstdlib>
#include <iostream>
#include <vector>
#include "Job.hpp"
#include <boost/thread.hpp>
namespace glPortal {
  class Sheduler {
  public:
    Sheduler(int milliseconds);
    void add(glPortal::Job *job);
    void run();
    void start();
  private:
    int interval;
    std::vector<glPortal::Job*> jobs;
    boost::thread thread;
  };
}
#endif
