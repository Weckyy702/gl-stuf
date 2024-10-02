#include "VertexBufferObject.hpp"

#include <glbinding/gl/gl.h>

#include <cassert>
#include <limits>

using namespace gl;

VertexBufferObject::VertexBufferObject() : GLObject(0) {
  glGenBuffers(1, &id_);
}

VertexBufferObject::VertexBufferObject(std::span<float> vertices,
                                       gl::GLenum usage)
    : VertexBufferObject() {
  set_vertex_data(vertices, usage);
}

void VertexBufferObject::bind() { glBindBuffer(GL_ARRAY_BUFFER, id_); }

void VertexBufferObject::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VertexBufferObject::set_vertex_data(std::span<float> vertex_data,
                                         gl::GLenum usage) {
  assert(vertex_data.size_bytes() < std::numeric_limits<GLsizeiptr>::max());

  bind();
  auto const size = static_cast<GLsizeiptr>(vertex_data.size_bytes());
  glBufferData(GL_ARRAY_BUFFER, size, vertex_data.data(), usage);
}

VertexBufferObject::~VertexBufferObject() noexcept {
  unbind();
  glDeleteBuffers(1, &id_);
}
