#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

namespace glPortal {

class Config {
public:
  static const std::string TRUE;
  static const std::string SUPERSAMPLING;
  static const std::string FULLSCREEN;
  static const std::string HEIGHT;
  static const std::string WIDTH;
  static const std::string SOUND;
};

} /* namespace glPortal */

#endif /* CONFIG_HPP */
