#include "Config.hpp"
#include <string>
#include <cstring> // std::strerror
#include <fstream>
#include <stdexcept>
#include <json11/json11.hpp>
#include <engine/env/Environment.hpp>

using namespace json11;

namespace glPortal {

void Config::load() {
  std::string err;
  std::string templateTxt;
  std::string templatePath = Environment::getDataDir() + "/config.json";
  std::ifstream in(templatePath, std::ios::in | std::ios::binary);
  if (in) {
    in.seekg(0, std::ios::end);
    templateTxt.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&templateTxt[0], templateTxt.size());
    in.close();
  } else {
    throw std::runtime_error(std::string("Can't open ") +
                             templatePath + ": " +
                             std::strerror(errno));
  }
  Json templateJson = Json::parse(templateTxt, err);
  Json videoJson = templateJson["video"];
  fullscreen   = videoJson["fullscreen"].bool_value();
  antialiasing = videoJson["antialiasing"].number_value();
  vsync        = videoJson["vsync"].bool_value();
  width        = videoJson["width"].number_value();
  height       = videoJson["height"].number_value();
  recursive_portal = videoJson["recursive_portal"].number_value();

  sound        = templateJson["sound"]["enabled"].bool_value();

  sensitivity  = templateJson["mouse"]["sensitivity"].number_value();
  hide_portals_by_click = templateJson["mouse"]["hide_portals_by_click"].bool_value();

  // TODO: get rid of this. Default map isn't part of the whole game config,
  // it's part of a mappack config.
  map = "n1";
}

} /* namespace glPortal */
