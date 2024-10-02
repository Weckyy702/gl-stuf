#pragma once

#include <span>

#include "Bindable.hpp"
#include "Object.hpp"

class VertexBufferObject final : public GLObject, Bindable {
public:
  VertexBufferObject();

  VertexBufferObject(std::span<float>, gl::GLenum);

  void bind() override;
  void unbind() override;

  void set_vertex_data(std::span<float>, gl::GLenum);

  ~VertexBufferObject() noexcept override;

private:
};
