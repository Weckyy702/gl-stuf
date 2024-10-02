#pragma once

#include <glbinding/gl/types.h>

class GLObject {
public:
  virtual ~GLObject() = default;

  GLObject(GLObject const &) = delete;
  GLObject(GLObject &&) = delete;

  GLObject &operator=(GLObject const &) = delete;
  GLObject &operator=(GLObject &&) = delete;

protected:
  GLObject(gl::GLuint id) : id_{id} {}

  gl::GLuint id_;

private:
};
