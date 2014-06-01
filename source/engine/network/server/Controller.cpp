#include "Controller.hpp"

namespace glPortal {     
  std::string Controller::setMessage(std::string message){
    std::string controllerName;
    std::string::size_type pos;
    pos = message.find(' ',0);
    controllerName = message.substr(0, pos);
    
    if(controllerName == "statistics"){
      return statisticsController.setMessage(message);
    } else {
      return std::string("error unknown controller\n");
    }
  }
}
