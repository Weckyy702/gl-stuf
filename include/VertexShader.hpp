#pragma once

#include "Shader.hpp"
class VertexShader final : public Shader {
  VertexShader(std::string_view src);
  ~VertexShader() noexcept override;
};
