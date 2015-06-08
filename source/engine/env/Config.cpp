#include "Config.hpp"

#include "Environment.hpp"
#include "ConfigFileParser.hpp"

#include <iostream>
#include <stdexcept>

namespace glPortal {
  const std::string Config::TRUE = "yes";
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
  
      Config::fullscreen = config->getStringByKey(Config::FULLSCREEN) == Config::TRUE;
      Config::antialiasing = config->getStringByKey(Config::ANTIALIASING) == Config::TRUE;
      Config::vsync = config->getStringByKey(Config::VSYNC) == Config::TRUE;
      Config::sound = config->getStringByKey(Config::SOUND) == Config::TRUE;
	  Config::sensitivity = config->getFloatByKey(Config::SENSITIVITY);

      std::string width = config->getStringByKey(Config::WIDTH);
      if (width == "auto") {
        Config::width = -1;
      } else {
        Config::width = std::stoi(width);
      }

      std::string height = config->getStringByKey(Config::HEIGHT);
      if (height == "auto") {
        Config::height = -1;
      } else {
        Config::height = std::stoi(height);
      }
    } catch (const std::invalid_argument &e) {
      std::cout << "Failed to load configuration due to an invalid argument" << std::endl;
    }
  }
} /* namespace glPortal */

