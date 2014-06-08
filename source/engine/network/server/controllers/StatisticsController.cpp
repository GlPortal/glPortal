#include "StatisticsController.hpp"
#include <fstream>
#include <boost/filesystem.hpp>
#include "../../../../util/sheduler/Sheduler.hpp"
#include "../../../../util/html/Generator.hpp"
#include <stdexcept>
#include <mutex>

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
    
    std::string actionName;
    std::string::size_type pos;
    pos = message.find(' ',0);
    actionName = message.substr(0, pos);
    message = message.substr(pos+1, message.size());

    if(actionName == "loggon"){
      return this->loggonAction(message);
    }
    return std::string("message The statistics controller logged your message\n");
  }

  std::string StatisticsController::loggonAction(std::string message){
    std::map<std::string,int>::iterator it = userPoints.find(message);
    try{
      int points = userPoints.at(message);
      points++;
        userPoints.at(message) = points;
      } catch (const std::exception& ex) {
        userPoints.insert ( std::pair<std::string,int>(message,1) );
      }

    return std::string("user data changed\n");
  }
  
  void StatisticsController::run(){
    std::ofstream htmlFile;
    const char dir_path[] = "html";

    boost::filesystem::path dir(dir_path);
    if(boost::filesystem::create_directory(dir)) {
      std::cout << "HTML directory created." << "\n";
    }
    htmlFile.open ("html/index.html");
    htmlFile << "<!DOCTYPE html>";
    htmlFile << "<html>";
    htmlFile << Generator::getTagHtml("head", Generator::getTagHtml("title", "GlPortal Stats"));
    htmlFile << "<body>";
    htmlFile <<  Generator::getTagHtml("h1", "GlPortal Stats");
    htmlFile << "<table>";
    htmlFile << "<tr>";
    htmlFile << Generator::getTagHtml("th", "User");
    htmlFile << Generator::getTagHtml("th", "Points");

    htmlFile << "</tr>";
    for (auto& userPoint : userPoints) {
      htmlFile << "<tr>";
      htmlFile << "<td>" << userPoint.first << "</td><td>" << userPoint.second << "</td>" << std::endl;
      htmlFile << "</tr>";
    }
    htmlFile << "</table>";
    htmlFile << Generator::getTagHtml("h2", "Log");
    htmlFile << "<ul>";

    for (std::vector<std::string>::iterator it = log.begin() ; it != log.end(); ++it){
      htmlFile << Generator::getTagHtml("li", std::string(*it));
    }    
    
    htmlFile << "</ul>";
    htmlFile << "</body></html>";
    htmlFile.close();
      
  }
}
