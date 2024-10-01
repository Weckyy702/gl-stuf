#include "Context.hpp"

#include <GLFW/glfw3.h>
#include <iostream>
#include <ostream>

[[gnu::weak]] void error_callback(int, char const *msg) noexcept {
  std::println(std::cerr, "[ERROR]: {}", msg);
}

namespace opengl {

Context Context::make() {
  if (!glfwInit())
    throw InitFailed{};

  glfwSetErrorCallback(error_callback);
  return {};
}

Context::~Context() noexcept { glfwTerminate(); }

} // namespace opengl
