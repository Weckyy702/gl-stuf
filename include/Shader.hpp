#pragma once

#include "Object.hpp"

#include <glbinding/gl/enum.h>

class Shader : public GLObject {
  using E = std::underlying_type_t<gl::GLenum>;

public:
  enum class Kind : E {
    Vertex = static_cast<E>(gl::GL_VERTEX_SHADER),
    Fragment = static_cast<E>(gl::GL_FRAGMENT_SHADER),
  };

  Kind kind() const noexcept { return kind_; }

  // TODO: input variables
  // TODO: output variables

  virtual ~Shader() = default;

protected:
  explicit Shader(Kind, std::string_view);

private:
  Kind kind_;
};
