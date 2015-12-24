#ifndef PATH_HPP
#define PATH_HPP

#include <string>

namespace glPortal {

class Path {
public:
  static std::string GetDirectorySeparator();

  // convert path from unix paths used throughout the code
  static std::string FromUnixPath(const std::string & unixPath);
};

} /* namespace glPortal */

#endif /* PATH_HPP */
