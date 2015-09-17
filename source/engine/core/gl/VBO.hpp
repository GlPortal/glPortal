#ifndef VBO_HPP
#define VBO_HPP
#include <vector>
#include <epoxy/gl.h>

namespace glPortal {

class VBO {
public:
  bool invalid;
  GLuint id;
  
  // Ctor / dtor
  VBO();
  ~VBO();
  // No copy
  VBO(const VBO&) = delete;
  VBO& operator=(const VBO&) = delete;
  // Move
  VBO(VBO&&);
  VBO& operator=(VBO&&);
  
  operator GLuint() const { return id; }
  void setSize(unsigned int size, GLenum usage = GL_STATIC_DRAW);
  template <typename T> void setData(const std::vector<T> &data, GLenum usage = GL_STATIC_DRAW) {
    GLint currentBoundArray; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentBoundArray);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(T), data.data(), usage);
    glBindBuffer(GL_ARRAY_BUFFER, currentBoundArray);
  }
  template <typename T> void setData(const T *data, unsigned int count, GLenum usage = GL_STATIC_DRAW) {
    GLint currentBoundArray; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentBoundArray);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, count*sizeof(T), data, usage);
    glBindBuffer(GL_ARRAY_BUFFER, currentBoundArray);
  }
  template <typename T> void setSubData(const T *data, unsigned int offset, unsigned int count) {
    GLint currentBoundArray; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentBoundArray);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(T), count*sizeof(T), data);
    glBindBuffer(GL_ARRAY_BUFFER, currentBoundArray);
  }
  void bind() const;
  int getSize() const;
};

} /* namespace glPortal */

#endif /* VBO_HPP */