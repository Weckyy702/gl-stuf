#pragma once

#include "DrawingContext.hpp"
#include <exception>

struct GLFWwindow;

namespace opengl {

class Context;
class DrawingContext;

class Window {
  static Window *current_window_;

  friend class WindowBuilder;
  friend DrawingContext;

public:
  struct InitFailed : public std::exception {

    char const *what() const noexcept override {
      return "Initializing the window failed";
    }
  };

  struct InitData {
    int width, height;
    struct {
      int major, minor;
    } opengl_version;
  };

  static Window const *current() noexcept { return current_window_; }

  void make_current() noexcept;

  void unmake_current() noexcept;

  bool should_close() const noexcept;

  DrawingContext begin_draw() noexcept;

  ~Window();

private:
  explicit Window(InitData);

  static void _resize_callback(GLFWwindow *, int, int) noexcept;
  static void _key_callback(GLFWwindow *, int, int, int, int) noexcept;

  void _resize(int w, int h) noexcept {
    width = w;
    height = h;
    viewport_dirty_ = true;
  }

  void _handle_keypress(int key, int action) noexcept;

  void _end_draw() noexcept;

  GLFWwindow *window_;
  int width, height;
  bool viewport_dirty_;
};

class WindowBuilder {
public:
  WindowBuilder(Context const &)
      : data_{
            .width = 640,
            .height = 480,
            .opengl_version = {4, 3},
        } {}

  Window build() { return Window{data_}; }

private:
  Window::InitData data_;
};

} // namespace opengl
