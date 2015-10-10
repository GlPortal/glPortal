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
  static const std::string MAP_PATH;
  static const std::string VSYNC;
  static const std::string HIDE_PORTALS_BY_CLICK;

  static void load();
  static unsigned int getWidth() { return Config::width; }
  static unsigned int getHeight() { return Config::height; }
  static float getSensitivity() { return Config::sensitivity; }
  static bool isFullscreen() { return Config::fullscreen; }
  static bool hasAntialiasing() { return Config::antialiasing; }
  static bool hasSound() { return Config::sound; }
  static bool hasVsync() { return Config::vsync; }
  static bool isHidePortalsByClick() { return Config::hide_portals_by_click; }
private:
  static unsigned int width;
  static unsigned int height;
  static float sensitivity;
  static bool antialiasing;
  static bool fullscreen;
  static bool sound;
  static bool vsync;
  static std::string map;
  static std::string mapPath;
  static bool hide_portals_by_click;
};

} /* namespace glPortal */

#endif /* CONFIG_HPP */
