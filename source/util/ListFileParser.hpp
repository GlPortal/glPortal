#ifndef __LISTFILEPARSER_HPP
#define __LISTFILEPARSER_HPP

#include <vector>
#include <string>
#include <cstring>
#include <cstdlib> 

using namespace std;

namespace glPortal {
  namespace util{
    class ListFileParser{
    private:
      vector<string> list;
    public:
      vector<string> getListFromFile(const char *filename);
    };
  }
}

#endif
