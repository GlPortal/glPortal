#include "Path.hpp"

#include <string>

using namespace std;

namespace glPortal {

string Path::GetDirectorySeparator() {
#ifndef _WIN32
  return "/";
#else
  return "\\";
#endif
}

string Path::FromUnixPath(const string& unixPath) {
  string path(unixPath);
#ifdef _WIN32
  for (unsigned int i = 0; i < path.size(); ++i)
    if (path.at(i) == '/')
      path.at(i) = '\\';
#endif
  return path;
}

} /* namespace glPortal */
