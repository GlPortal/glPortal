#include "StatisticsController.hpp"
#include <fstream>
#include <boost/filesystem.hpp>

namespace glPortal {     
  std::string StatisticsController::setMessage(std::string message){
          
      log.push_back(message);

      if(log.size() > 10){
        log.erase(log.begin());
      }
      std::ofstream myfile;
      const char dir_path[] = "html";

      boost::filesystem::path dir(dir_path);
      if(boost::filesystem::create_directory(dir)) {
        std::cout << "HTML directory created." << "\n";
      }
      myfile.open ("html/index.html");

      std::cout << "LOG:";
      for (std::vector<std::string>::iterator it = log.begin() ; it != log.end(); ++it){
        std::cout << "entry:";
        myfile << *it;
      }
      myfile.close();

    return std::string("statistics controller\n");
  }
}
