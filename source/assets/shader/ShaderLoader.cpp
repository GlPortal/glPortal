#include "ShaderLoader.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <engine/env/Environment.hpp>
#include <engine/env/System.hpp>

namespace {
  const int LOG_SIZE = 1024;
}

namespace glPortal {

std::map<std::pair<std::string, std::string>, Shader> ShaderLoader::shaderCache = { };

Shader& ShaderLoader::getShader(const std::string &fragpath, const std::string &vertpath) {
  std::string fpath = Environment::getDataDir() + "/shaders/" + fragpath;
  std::string vpath;
  if (vertpath.empty()) {
    vpath = Environment::getDataDir() + "/shaders/diffuse.vert";
  } else {
    vpath = Environment::getDataDir() + "/shaders/" + vertpath;
  }
  auto it = shaderCache.find(std::pair<std::string, std::string>(fragpath, vertpath));
  if (it != shaderCache.end()) {
    return it->second;
  }

  // Load the shaders
  int vertShader = loadShader(vpath, GL_VERTEX_SHADER);
  int fragShader = loadShader(fpath, GL_FRAGMENT_SHADER);

  // Create a program and attach both shaders
  int shader = glCreateProgram();
  glAttachShader(shader, vertShader);
  glAttachShader(shader, fragShader);

  glLinkProgram(shader);

  // Error checking
  GLint success = 0;
  glGetProgramiv(shader, GL_LINK_STATUS, &success);
  if (success == GL_TRUE) {
    System::Log(Debug, "ShaderLoader") << fpath << ": program linked";
  } else {
    GLint logSize = 0;
    glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    std::unique_ptr<char> log(new char[logSize]);
    glGetProgramInfoLog(shader, logSize, NULL, log.get());
    System::Log(Error, "ShaderLoader") << fpath << ": linking failed:\n" << log.get();
  }

  glValidateProgram(shader);

  // Error checking
  glGetProgramiv(shader, GL_VALIDATE_STATUS, &success);
  if (success == GL_TRUE) {
    System::Log(Debug, "ShaderLoader") << fpath << ": progam validated";
  } else {
    GLint logSize = 0;
    glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    std::unique_ptr<char> log(new char[logSize]);
    glGetProgramInfoLog(shader, logSize, NULL, log.get());
    System::Log(Error, "ShaderLoader") << fpath << ": validation failed:\n" << log.get();
  }

  Shader s;
  s.handle = shader;
  auto inserted = shaderCache.insert(std::pair<std::pair<std::string, std::string>, Shader>(
    std::pair<std::string, std::string>(fragpath, vertpath), s));
  // Return reference to newly inserted Shader
  return inserted.first->second;
}

int ShaderLoader::loadShader(const std::string &path, GLenum type) {
  std::ifstream file(path);
  if (not file.is_open()) {
    System::Log(Error, "ShaderLoader") << "Could not find shader file " << path;
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
      System::Log(Debug, "ShaderLoader") << path << ": vertex shader compiled";
    }
    if (type == GL_FRAGMENT_SHADER) {
      System::Log(Debug, "ShaderLoader") << path << ": fragment shader compiled";
    }
  } else {
    GLint logSize = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

    std::unique_ptr<char> log(new char[logSize]);
    glGetShaderInfoLog(shader, logSize, &logSize, log.get());

    if (type == GL_VERTEX_SHADER) {
      System::Log(Error, "ShaderLoader") << path << ": vertex shader compilation failed:\n" <<
        log.get();
    }
    if (type == GL_FRAGMENT_SHADER) {
      System::Log(Error, "ShaderLoader") << path << ": fragment shader compilation failed:\n" <<
        log.get();
    }

    glDeleteShader(shader);
  }

  return shader;
}

} /* namespace glPortal */
