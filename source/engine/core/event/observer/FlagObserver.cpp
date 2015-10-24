#include "FlagObserver.hpp"
#include <iostream> 
namespace glPortal {

FlagObserver::FlagObserver(bool &flag, bool resultingStatus) {
  this->flag = &flag;
  this->resultingStatus = resultingStatus;
}

void FlagObserver::execute() {
  std::cout << "Dicksnnnnnnnrrrrrrrrrrrrrrrrrrn";
  *flag = resultingStatus;
}

}
