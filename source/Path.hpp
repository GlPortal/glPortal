#ifndef __PATH_HPP
#define __PATH_HPP

#include <stdio.h>
#include <string>

using namespace std;

class Path {
public:

  static std::string GetDirectorySeparator();
  
  /// convert path from unix paths used throughout the code
  static std::string FromUnixPath(const std::string & unixPath);
  
private:
  Path();
};
#endif
