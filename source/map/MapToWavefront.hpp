#ifndef MAPTOWAVEFRONT_HPP
#define MAPTOWAVEFRONT_HPP

#include <string>

using namespace std;

namespace glPortal {
  namespace map{
    /**
     * Convert a glPortal gameMapto wavefront format
     **/
    class MapToWavefront {
    public:
      MapToWavefront(const string & filename);
    private:
      static const char* DATA_DIRECTORY;
    };
  }
}

#endif
