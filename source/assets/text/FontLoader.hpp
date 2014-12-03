#ifndef FONTLOADER_HPP
#define FONTLOADER_HPP

#include <map>
#include <string>

#include <assets/text/Font.hpp>

namespace glPortal {

class FontLoader {
public:
  static Font getFont(std::string path);
private:
  static Font loadFont(std::string path, std::string name);
  static std::map<std::string, Font> fontCache;
};

} /* namespace glPortal */

#endif /* FONTLOADER_HPP */
