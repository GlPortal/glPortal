#include "Config.hpp"

#include "Environment.hpp"
#include "ConfigFileParser.hpp"

#include <iostream>
#include <stdexcept>

namespace glPortal {
  const std::string Config::SENSITIVITY = "sensitivity";
  const std::string Config::ANTIALIASING = "antialiasing";
  const std::string Config::FULLSCREEN = "fullscreen";
  const std::string Config::HEIGHT = "height";
  const std::string Config::WIDTH = "width";
  const std::string Config::SOUND = "sound";
  const std::string Config::MAP = "map";
  const std::string Config::VSYNC = "vsync";

  unsigned int Config::width = 1024;
  unsigned int Config::height = 768;
  float Config::sensitivity = 0.1f;
  bool Config::antialiasing = true;
  bool Config::fullscreen = true;
  bool Config::sound = true;
  bool Config::vsync = true;
  std::string Config::map = "n1.xml";

  void Config::load() {
    try {
      ConfigFileParser *config = Environment::getConfigPointer();
  
      Config::fullscreen = config->getBool(Config::FULLSCREEN);
      Config::antialiasing = config->getBool(Config::ANTIALIASING);
      Config::vsync = config->getBool(Config::VSYNC);
      Config::sound = config->getBool(Config::SOUND);
      Config::sensitivity = config->getFloat(Config::SENSITIVITY);

      std::string width = config->getString(Config::WIDTH);
      if (width == "auto") {
        Config::width = -1;
      } else {
        Config::width = std::stoi(width);
      }

      std::string height = config->getString(Config::HEIGHT);
      if (height == "auto") {
        Config::height = -1;
      } else {
        Config::height = std::stoi(height);
      }
    } catch (const std::out_of_range &e) {
      std::cout << "Failed to load configuration due to a missing config key" << std::endl;
    }
  }
} /* namespace glPortal */

