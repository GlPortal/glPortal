#include "ShaderLoader.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <engine/env/Environment.hpp>
#include <engine/env/System.hpp>

namespace {
  const int LOG_SIZE = 1024;
}

namespace glPortal {

std::map<std::string, Shader> ShaderLoader::shaderCache = { };

Shader& ShaderLoader::getShader(const std::string &path) {
  auto it = shaderCache.find(path);
  if (it != shaderCache.end()) {
    return it->second;
  }

  std::string fpath = Environment::getDataDir() + "/shaders/" + path;

  //Load the shaders
  int vertShader = loadShader(Environment::getDataDir() + "/shaders/diffuse.vert", GL_VERTEX_SHADER);
  int fragShader = loadShader(fpath, GL_FRAGMENT_SHADER);

  //Create a program and attach both shaders
  int shader = glCreateProgram();
  glAttachShader(shader, vertShader);
  glAttachShader(shader, fragShader);

  glLinkProgram(shader);

  //Error checking
  GLint success = 0;
  glGetProgramiv(shader, GL_LINK_STATUS, &success);
  if (success == GL_TRUE) {
    System::Log(Debug) << fpath << ": program linked";
  } else {
    GLint logSize = 0;
    glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    char* log = (char*)malloc(logSize);
    glGetProgramInfoLog(shader, logSize, NULL, log);
    System::Log(Error) << fpath << ": linking failed:\n" << log;
    free(log);
  }

  glValidateProgram(shader);

  //Error checking
  glGetProgramiv(shader, GL_VALIDATE_STATUS, &success);
  if (success == GL_TRUE) {
    System::Log(Debug) << fpath << ": progam validated";
  } else {
    GLint logSize = 0;
    glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    char* log = (char*)malloc(logSize);
    glGetProgramInfoLog(shader, logSize, NULL, log);
    System::Log(Error) << fpath << ": validation failed:\n" << log;
    free(log);
  }

  Shader s;
  s.handle = shader;
  auto inserted = shaderCache.insert(std::pair<std::string, Shader>(path, s));
  // Return reference to newly inserted Shader
  return inserted.first->second;
}

int ShaderLoader::loadShader(const std::string &path, GLenum type) {
  std::ifstream file(path);
  if (not file.is_open()) {
    System::Log(Error) << "Could not find shader file " << path;
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
      System::Log(Debug) << path << ": vertex shader compiled";
    }
    if (type == GL_FRAGMENT_SHADER) {
      System::Log(Debug) << path << ": fragment shader compiled";
    }
  } else {
    GLint logSize = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

    char* log = (char*)malloc(logSize);
    glGetShaderInfoLog(shader, logSize, &logSize, log);

    if (type == GL_VERTEX_SHADER) {
      System::Log(Error) << path << ": vertex shader compilation failed:\n" << log;
    }
    if (type == GL_FRAGMENT_SHADER) {
      System::Log(Error) << path << ": fragment shader compilation failed:\n" << log;
    }

    free(log);
    glDeleteShader(shader);
  }

  return shader;
}

} /* namespace glPortal */
