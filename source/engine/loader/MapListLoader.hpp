#ifndef MAPLISTLOADER_HPP
#define MAPLISTLOADER_HPP

#include <string>
#include <vector>

using namespace std;

namespace glPortal {

class MapListLoader {
public:
  static vector<string> getMapList();
};

} /* namespace glPortal */

#endif /* MAPLISTLOADER_HPP */
