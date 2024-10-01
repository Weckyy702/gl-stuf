#include "DrawingContext.hpp"
#include "Window.hpp"
#include <cassert>

namespace opengl {
DrawingContext::DrawingContext(Window *parent) : window_{parent} {
  assert(parent == Window::current());
}

DrawingContext::~DrawingContext() { window_->_end_draw(); }
} // namespace opengl
