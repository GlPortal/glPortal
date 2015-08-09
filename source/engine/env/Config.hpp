#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

namespace glPortal {

class Config {
public:
  void load();
  unsigned int getWidth() { return width; }
  unsigned int getHeight() { return height; }
  float getSensitivity() { return sensitivity; }
  bool isFullscreen() { return fullscreen; }
  int getAntialiasLevel() { return antialiasing; }
  bool hasSound() { return sound; }
  bool hasVsync() { return vsync; }
  bool isHidePortalsByClick() { return hide_portals_by_click; }

  std::string map;
  std::string mapPath;
private:
  unsigned int width;
  unsigned int height;
  float sensitivity;
  int antialiasing;
  bool fullscreen;
  bool sound;
  bool vsync;
  bool hide_portals_by_click;
};

} /* namespace glPortal */

#endif /* CONFIG_HPP */
