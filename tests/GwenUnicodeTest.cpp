#include "../external/gwen/include/Gwen/Utility.h"
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <string>

using namespace Gwen;
using namespace Gwen::Utility;

int main(int argc, char *argv[]) {
  std::srand(std::time(nullptr));

  for (int i = 0; i < 1024; ++i) {
    String str;
    for (int s = 0; s < 64; ++s) {
      str.push_back(std::rand() % ('~'-' ') + ' ');
    }
    UnicodeString ustr = StringToUnicode(str);
    for (int j = 0; j < 64; ++j) {
      if (ustr[j] != (UnicodeString::value_type)str[j]) {
        return 1;
      }
    }
  }

  for (int i = 0; i < 1024; ++i) {
    UnicodeString oustr; String str;
    for (int s = 0; s < 64; ++s) {
      oustr.push_back(std::rand() % (0x10FFFF-' ') + ' ');
    }
    UnicodeString ustr(oustr);
    for (int s = 0; s < 1; ++s) {
      str = UnicodeToString(ustr);
      ustr = StringToUnicode(str);
    }
    if (oustr != ustr) {
      for (int j = 0; j < 64; ++j) {
        printf("%X %X %X\n", oustr[j], str[j], ustr[j]);
      }
      return 2;
    }
  }

  return 0;
}
