#pragma once

namespace opengl {
class Window;

class DrawingContext {
public:
  DrawingContext(Window *);
  ~DrawingContext();

private:
  Window *window_;
};
} // namespace opengl
