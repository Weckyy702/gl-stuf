#pragma once

#include "Shader.hpp"
class FragmentShader final : public Shader {
  FragmentShader(std::string_view src);
  ~FragmentShader() noexcept override;
};
