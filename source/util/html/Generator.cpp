#include "Generator.hpp"

namespace glPortal {
  std::string Generator::getTagHtml(std::string tagName, std::string content){
    return std::string("<") + tagName + std::string(">") + content + std::string("</") + tagName + std::string(">"); 
  }
  
  std::string Generator::getTagHtml(std::string tagName){
    return std::string("<") + tagName + std::string("/>"); 
  }

  std::string Generator::decorateWithAttribute(std::string html, std::string attributeName, std::string value){
    int position = html.find(' ',0);
    html.insert(position+1, attributeName + std::string("=\"") + value + std::string("\" "));

    return html; 
  }
}

