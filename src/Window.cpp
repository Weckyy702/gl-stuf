#include "Window.hpp"
#include "DrawingContext.hpp"

#include <GLFW/glfw3.h>
#include <cassert>
#include <iostream>

namespace opengl {
Window::Window(InitData data) {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, data.opengl_version.major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, data.opengl_version.minor);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(640, 480, "01 Intro", nullptr, nullptr);

  if (!window_)
    throw InitFailed{};

  glfwSetKeyCallback(window_, _key_callback);
  glfwSetFramebufferSizeCallback(window_, _resize_callback);

  make_current();

  glfwSwapInterval(1);
}

void Window::make_current() noexcept {
  if (current_window_ == this)
    return;
  current_window_->unmake_current();

  glfwMakeContextCurrent(window_);
  current_window_ = this;
}

void Window::unmake_current() noexcept { glfwMakeContextCurrent(nullptr); }

bool Window::should_close() const noexcept {
  return glfwWindowShouldClose(window_);
}

DrawingContext Window::begin_draw() noexcept {
  glfwPollEvents();

  if (viewport_dirty_) {
    std::println(std::cout, "Size changed: ({}, {})", width, height);
    glViewport(0, 0, width, height);
    viewport_dirty_ = false;
  }

  return {this};
}

void Window::_resize_callback(GLFWwindow *window, int w, int h) noexcept {
  auto *win = current_window_;
  assert(win);
  assert(win->window_ == window);

  win->_resize(w, h);
}

void Window::_key_callback(GLFWwindow *window, int key, int scancode,
                           int action, int modifiers) noexcept {
  auto *win = current_window_;
  assert(win);
  assert(win->window_ == window);

  win->_handle_keypress(key, action);
}

void Window::_handle_keypress(int key, int action) noexcept {
  std::println(std::cout, "Got key {}", key);
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window_, true);
  }
}

void Window::_end_draw() noexcept { glfwSwapBuffers(window_); }

Window::~Window() { glfwDestroyWindow(window_); }

Window *Window::current_window_ = nullptr;

} // namespace opengl
