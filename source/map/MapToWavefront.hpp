#ifndef MAPTOWAVEFRONT_HPP
#define MAPTOWAVEFRONT_HPP

#include <string>

using namespace std;

namespace glPortal {
  namespace map {
    class MapToWavefront {
    public:
      MapToWavefront(const string & filename);
    private:
      static const char* DATA_DIRECTORY;
    };
  }
}

#endif
