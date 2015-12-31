#include "VBO.hpp"

namespace glPortal {

VBO::VBO() :
  size(0) {
  glGenBuffers(1, &id);
}

VBO::VBO(unsigned int size, GLenum usage) :
  size(0) {
  glGenBuffers(1, &id);
  setSize(size, usage);
}

VBO::VBO(VBO &&other) {
  id = other.id;
  other.size = -1;
}
VBO& VBO::operator=(VBO &&other) {
  id = other.id;
  other.size = -1;
  return *this;
}

VBO::~VBO() {
  if (size != -1) {
    glDeleteBuffers(1, &id);
  }
}

void VBO::bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::setSize(unsigned int size, GLenum usage) {
  GLint currentBoundArray; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentBoundArray);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  this->size = size;
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, usage);
  glBindBuffer(GL_ARRAY_BUFFER, currentBoundArray);
}

} /* namespace glPortal */
