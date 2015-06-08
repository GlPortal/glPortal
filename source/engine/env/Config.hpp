#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

namespace glPortal {

class Config {
public:
  static const std::string SENSITIVITY;
  static const std::string ANTIALIASING;
  static const std::string FULLSCREEN;
  static const std::string HEIGHT;
  static const std::string WIDTH;
  static const std::string SOUND;
  static const std::string MAP;
  static const std::string VSYNC;

  static void load();
  static unsigned int getWidth() { return Config::width; }
  static unsigned int getHeight() { return Config::height; }
  static float getSensitivity() { return Config::sensitivity; }
  static bool isFullscreen() { return Config::fullscreen; }
  static bool hasAntialiasing() { return Config::antialiasing; }
  static bool hasSound() { return Config::sound; }
  static bool hasVsync() { return Config::vsync; }
private:
  static unsigned int width;
  static unsigned int height;
  static float sensitivity;
  static bool antialiasing;
  static bool fullscreen;
  static bool sound;
  static bool vsync;
  static std::string map;
};

} /* namespace glPortal */

#endif /* CONFIG_HPP */
