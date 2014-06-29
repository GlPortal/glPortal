#include "Resource.hpp"

#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <string>

namespace glPortal {

int Resource::loadShaders() {
  //Load the shaders
  int vertexShader = loadShader("data/shaders/diffuse.vert", GL_VERTEX_SHADER);
  int fragmentShader = loadShader("data/shaders/diffuse.frag", GL_FRAGMENT_SHADER);

  //Create a program and attach both shaders
  int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  glLinkProgram(shaderProgram);
  glValidateProgram(shaderProgram);

  //Error checking
  GLint success = 0;
  glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
  if (success == GL_TRUE) {
    std::cout << "Shader program linked successfully" << std::endl;
  }

  return shaderProgram;
}

int Resource::loadShader(const char* filename, GLenum type) {
  std::ifstream file(filename);
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

    char errorLog[logSize];
    glGetShaderInfoLog(shader, logSize, NULL, &errorLog[0]);

    std::cout << errorLog << std::endl;
    glDeleteShader(shader);
  }

  return shader;
}

} /* namespace glPortal */
