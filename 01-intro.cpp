#include "Context.hpp"
#include "Window.hpp"
#include <cmath>

#include "glad/gl.h"
#include <GLFW/glfw3.h>

int main() {
  auto ctx = opengl::Context::make();

  auto window = opengl::WindowBuilder(ctx).build();
  window.make_current();

  float b = 0.;
  while (!window.should_close()) {
    auto const ctx = window.begin_draw();

    b = static_cast<float>(sin(glfwGetTime()) * .5 + .5);

    glClearColor(.5, 0., b, 1.);
    glClear(GL_COLOR_BUFFER_BIT);
  }
}
