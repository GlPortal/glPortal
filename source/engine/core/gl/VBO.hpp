#ifndef VBO_HPP
#define VBO_HPP

#include <vector>

#include <epoxy/gl.h>

namespace glPortal {

class VBO {
protected:
  int size;
  GLuint id;

public:
  // Ctor / dtor
  VBO();
  VBO(unsigned int size, GLenum usage = GL_STATIC_DRAW);
  ~VBO();
  // No copy
  VBO(const VBO&) = delete;
  VBO& operator=(const VBO&) = delete;
  // Move
  VBO(VBO&&);
  VBO& operator=(VBO&&);
  
  operator GLuint() const { return id; }
  void setSize(unsigned int size, GLenum usage = GL_STATIC_DRAW);
  int getSize() const {
    return size;
  }

  template <typename T>
  void setData(const std::vector<T> &data, GLenum usage = GL_STATIC_DRAW) {
    GLint currentBoundArray; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentBoundArray);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    size = data.size()*sizeof(T);
    glBufferData(GL_ARRAY_BUFFER, size, data.data(), usage);
    glBindBuffer(GL_ARRAY_BUFFER, currentBoundArray);
  }
  template <typename T>
  void setData(const T *data, unsigned int count, GLenum usage = GL_STATIC_DRAW) {
    GLint currentBoundArray; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentBoundArray);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    size = count*sizeof(T);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    glBindBuffer(GL_ARRAY_BUFFER, currentBoundArray);
  }
  template <typename T>
  void update(const std::vector<T> &data, unsigned int count = 0, unsigned int offset = 0) {
    GLint currentBoundArray; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentBoundArray);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    if (count == 0) {
      count = data.size();
    }
    glBufferSubData(GL_ARRAY_BUFFER, offset, count*sizeof(T), data.data());
    glBindBuffer(GL_ARRAY_BUFFER, currentBoundArray);
  }
  template <typename T>
  void update(const T *data, unsigned int count, unsigned int offset = 0) {
    GLint currentBoundArray; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentBoundArray);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferSubData(GL_ARRAY_BUFFER, offset, count*sizeof(T), data);
    glBindBuffer(GL_ARRAY_BUFFER, currentBoundArray);
  }
  void bind() const;
};

} /* namespace glPortal */

#endif /* VBO_HPP */