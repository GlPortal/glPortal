#ifndef PATH_HPP
#define PATH_HPP

#include <string>

using namespace std;

namespace glPortal {

class Path {
public:
  static std::string GetDirectorySeparator();

  // convert path from unix paths used throughout the code
  static std::string FromUnixPath(const std::string & unixPath);
};

} /* namespace glPortal */

#endif /* PATH_HPP */
