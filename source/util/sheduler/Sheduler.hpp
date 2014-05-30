#ifndef __SERVER_HPP
#define __SERVER_HPP

#include <cstdlib>
#include <iostream>
#include <vector>
#include "Job.hpp"

namespace glPortal {
  class Sheduler {
  public:
    Sheduler(int miliseconds);
    void add(glPortal::Job *job);
    void run();
    void start();
  private:
    int interval;
    std::vector<glPortal::Job*> jobs;
  };
}
#endif
