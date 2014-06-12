#ifndef __GENERATOR_HPP
#define __GENERATOR_HPP

#include <vector>
#include <string>
#include <cstring>
#include <cstdlib> 

using namespace std;

namespace glPortal {
  class Generator{
  public:
    static std::string getTagHtml(std::string tagName, std::string content);
    static std::string getTagHtml(std::string tagName);
    static std::string decorateWithAttribute(std::string html, std::string attributeName, std::string value);
  };
}

#endif
