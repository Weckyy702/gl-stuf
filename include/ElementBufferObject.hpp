#pragma once
#include <span>

#include "Bindable.hpp"
#include "Object.hpp"

class ElementBufferObject final : public GLObject, Bindable {
public:
  ElementBufferObject();

  ElementBufferObject(std::span<gl::GLuint> indices, gl::GLenum usage);

  virtual void bind() noexcept override;
  virtual void unbind() noexcept override;

  void set_indices(std::span<gl::GLuint> indices, gl::GLenum usage);

  virtual ~ElementBufferObject() noexcept override;
};
