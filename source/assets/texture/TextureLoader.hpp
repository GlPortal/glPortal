#ifndef TEXTURELOADER_HPP
#define TEXTURELOADER_HPP

#include <string>
#include <map>

#include <assets/texture/Texture.hpp>

namespace glPortal {

class TextureLoader {
public:
  static Texture getEmptyTexture(const std::string &name,
    const char *pixel = "\xFF\xFF\xFF", int pxDepth = 3);
  static Texture getEmptyDiffuse();
  static Texture getEmptyNormal();
  static Texture getEmptySpecular();
  static Texture getTexture(const std::string &path);
private:
  static Texture uploadTexture(const unsigned char *data, int width, int height, int bytes);
  static std::map<std::string, Texture> textureCache;
};

} /* namespace glPortal */

#endif /* TEXTURELOADER_HPP */
