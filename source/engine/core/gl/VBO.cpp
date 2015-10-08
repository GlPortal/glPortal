#include "VBO.hpp"

namespace glPortal {

VBO::VBO() :
  invalid(false) {
  glGenBuffers(1, &id);
}

VBO::VBO(VBO &&other) {
  id = other.id;
  other.invalid = true;
}
VBO& VBO::operator=(VBO &&other) {
  id = other.id;
  other.invalid = true;
  return *this;
}

VBO::~VBO() {
  if (not invalid) {
    glDeleteBuffers(1, &id);
  }
}

void VBO::bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, id);
}

int VBO::getSize() const {
  GLint currentBoundArray; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentBoundArray);
  GLint data;
  bind();
  glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &data);
  glBindBuffer(GL_ARRAY_BUFFER, currentBoundArray);
  return data;
}

void VBO::setSize(unsigned int size, GLenum usage) {
  GLint currentBoundArray; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentBoundArray);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, usage);
  glBindBuffer(GL_ARRAY_BUFFER, currentBoundArray);
}

} /* namespace glPortal */
