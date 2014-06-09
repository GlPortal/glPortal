#include "Controller.hpp"

namespace glPortal {
  Controller::Controller(): statisticsController(){
  }
  
  std::string Controller::setMessage(std::string message){
    std::string controllerName;
    std::string::size_type pos;
    pos = message.find(' ',0);
    controllerName = message.substr(0, pos);
    message = message.substr(pos+1, message.size());
    
    if(controllerName == "statistics"){
      return statisticsController.setMessage(message);
    } else {
      return std::string("error unknown controller\n");
    }
  }
}
