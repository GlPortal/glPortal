#include "StatisticsController.hpp"
#include <fstream>
#include <boost/filesystem.hpp>
#include "../../../../util/sheduler/Sheduler.hpp"

namespace glPortal {
  StatisticsController::StatisticsController(){
    sheduler = new Sheduler(1000);
    sheduler->add(this);
    sheduler->start();
  }
  
  std::string StatisticsController::setMessage(std::string message){
          
      log.push_back(message);

      if(log.size() > 10){
        log.erase(log.begin());
      }
    return std::string("message The statistics controller logged your message\n");
  }

  void StatisticsController::run(){
    std::ofstream myfile;
    const char dir_path[] = "html";

    boost::filesystem::path dir(dir_path);
    if(boost::filesystem::create_directory(dir)) {
      std::cout << "HTML directory created." << "\n";
    }
    myfile.open ("html/index.html");
    
    for (std::vector<std::string>::iterator it = log.begin() ; it != log.end(); ++it){
        myfile << *it;
    }
    myfile.close();
      
  }
}
