#include "ShaderLoader.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include "engine/env/Environment.hpp"

namespace {
  const int LOG_SIZE = 1024;
}

namespace glPortal {

std::map<std::string, Shader> ShaderLoader::shaderCache = { };

Shader& ShaderLoader::getShader(const std::string &fpath) {
  std::string path = Environment::getDataDir() + "/shaders/" + fpath;
  if(shaderCache.find(path) != shaderCache.end()) {
    return shaderCache.at(path);
  }

  //Load the shaders
  int vertShader = loadShader(Environment::getDataDir() + "/shaders/diffuse.vert", GL_VERTEX_SHADER);
  int fragShader = loadShader(path, GL_FRAGMENT_SHADER);

  //Create a program and attach both shaders
  int shader = glCreateProgram();
  glAttachShader(shader, vertShader);
  glAttachShader(shader, fragShader);

  glLinkProgram(shader);

  //Error checking
  GLint success = 0;
  glGetProgramiv(shader, GL_LINK_STATUS, &success);
  if (success == GL_TRUE) {
    std::cout << "Shader program linked successfully" << std::endl;
  } else {
    GLint logSize = 0;
    glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    char* log = (char*)malloc(logSize);
    glGetProgramInfoLog(shader, logSize, NULL, log);
    std::cout << "Failed linking " << path << ":\n" << log << std::endl;
    free(log);
  }

  glValidateProgram(shader);

  //Error checking
  glGetProgramiv(shader, GL_VALIDATE_STATUS, &success);
  if (success == GL_TRUE) {
    std::cout << "Shader program validated successfully" << std::endl;
  } else {
    GLint logSize = 0;
    glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    char* log = (char*)malloc(logSize);
    glGetProgramInfoLog(shader, logSize, NULL, log);
    std::cout << "Failed to validate " << path << ":\n" << log << std::endl;
    free(log);
  }

  Shader s;
  s.handle = shader;
  auto inserted = shaderCache.insert(std::pair<std::string, Shader>(path, s));
  // Return reference to newly inserted Shader
  return (*inserted.first).second;
}

int ShaderLoader::loadShader(const std::string &path, GLenum type) {
  std::ifstream file(path.c_str());
  if (not file.is_open()) {
    std::cout << "Could not find file" << path << std::endl;
  }
  std::string str;
  std::string file_contents;
  while (std::getline(file, str)) {
    file_contents += str;
    file_contents.push_back('\n');
  }
  const char* buffer = file_contents.c_str();

  int shader = glCreateShader(type);
  glShaderSource(shader, 1, (const char**) &buffer, NULL);
  glCompileShader(shader);

  //Error checking
  GLint success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (success == GL_TRUE) {
    if (type == GL_VERTEX_SHADER) {
      std::cout << "Vertex shader compiled successfully" << std::endl;
    }
    if (type == GL_FRAGMENT_SHADER) {
      std::cout << "Fragment shader compiled successfully" << std::endl;
    }
  } else {
    if (type == GL_VERTEX_SHADER) {
      std::cout << "Vertex shader compilation failed" << std::endl;
    }
    if (type == GL_FRAGMENT_SHADER) {
      std::cout << "Fragment shader compilation failed" << std::endl;
    }
    
    GLint logSize = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

    char* log = (char*)malloc(logSize);
    glGetShaderInfoLog(shader, logSize, &logSize, log);

    std::cout << log << std::endl;
    free(log);
    glDeleteShader(shader);
  }

  return shader;
}

} /* namespace glPortal */
