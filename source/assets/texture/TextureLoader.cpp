#include "TextureLoader.hpp"

#include <GL/glew.h>
#include <utility>

#include "engine/env/Environment.hpp"
#include "engine/stb_image.c"
#include <cstdio>

namespace glPortal {

std::map<std::string, Texture> TextureLoader::textureCache = {};

Texture TextureLoader::getEmptyDiffuse() {
  auto it = textureCache.find("engine@empty/diffuse");
  if (it != textureCache.end()) {
    return it->second;
  }
  int width, height, bytes;
  Texture texture = uploadTexture((const unsigned char*)"\xFF\xFF\xFF", 1, 1, 3);
  texture.width = width;
  texture.height = height;
  textureCache.insert(std::pair<std::string, Texture>("engine@empty/diffuse", texture));
  return texture;
}

Texture TextureLoader::getEmptyNormal() {
  auto it = textureCache.find("engine@empty/normal");
  if (it != textureCache.end()) {
    return it->second;
  }
  int width, height, bytes;
  Texture texture = uploadTexture((const unsigned char*)"\x7F\x7F\xFF", 1, 1, 3);
  texture.width = width;
  texture.height = height;
  textureCache.insert(std::pair<std::string, Texture>("engine@empty/normal", texture));
  return texture;
}

Texture TextureLoader::getEmptySpecular() {
  auto it = textureCache.find("engine@empty/specular");
  if (it != textureCache.end()) {
    return it->second;
  }
  int width, height, bytes;
  Texture texture = uploadTexture((const unsigned char*)"\xFF\xFF\xFF", 1, 1, 3);
  texture.width = width;
  texture.height = height;
  textureCache.insert(std::pair<std::string, Texture>("engine@empty/specular", texture));
  return texture;
}


Texture TextureLoader::getTexture(std::string path) {
  auto it = textureCache.find(path);
  if (it != textureCache.end()) {
    return it->second;
  }
  int width, height, bytes;
  unsigned char *data = stbi_load((Environment::getDataDir() + "/textures/" + path).c_str(), &width, &height, &bytes, 0);
  Texture texture = uploadTexture(data, width, height, bytes);
  stbi_image_free(data);
  texture.width = width;
  texture.height = height;
  textureCache.insert(std::pair<std::string, Texture>(path, texture));
  return texture;
}

Texture TextureLoader::uploadTexture(const unsigned char *data, int width, int height, int bytes) {
  Texture texture;
  GLuint handle;
  glGenTextures(1, &handle);
  glBindTexture(GL_TEXTURE_2D, handle);

  if(bytes == 3) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  }
  if(bytes == 4) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  }
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  texture.handle = handle;
  return texture;
}

} /* namespace glPortal */
