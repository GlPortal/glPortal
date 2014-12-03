#ifndef TEXTURELOADER_HPP
#define TEXTURELOADER_HPP

#include <string>
#include <map>

#include <assets/texture/Texture.hpp>

namespace glPortal {

class TextureLoader {
public:
  static Texture getTexture(std::string path);
private:
  static Texture uploadTexture(unsigned char* data, int width, int height, int bytes);
  static std::map<std::string, Texture> textureCache;
};

} /* namespace glPortal */

#endif /* TEXTURELOADER_HPP */
