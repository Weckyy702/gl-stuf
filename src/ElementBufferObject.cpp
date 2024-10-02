#include "ElementBufferObject.hpp"

#include <glbinding/gl/gl.h>

#include <cassert>
#include <limits>

using namespace gl;

ElementBufferObject::ElementBufferObject() : GLObject(0) {
  glGenBuffers(1, &id_);
}

ElementBufferObject::ElementBufferObject(std::span<GLuint> indices,
                                         GLenum usage)
    : ElementBufferObject() {
  set_indices(indices, usage);
}

void ElementBufferObject::bind() noexcept {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
}

void ElementBufferObject::unbind() noexcept {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ElementBufferObject::set_indices(std::span<GLuint> indices, GLenum usage) {
  assert(indices.size_bytes() < std::numeric_limits<GLsizeiptr>::max());

  bind();
  auto const size = static_cast<GLsizeiptr>(indices.size_bytes());
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices.data(), usage);
}

ElementBufferObject::~ElementBufferObject() noexcept {
  unbind();
  glDeleteBuffers(1, &id_);
}
