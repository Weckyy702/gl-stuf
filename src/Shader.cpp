#include "Shader.hpp"

#include <glbinding/gl/gl.h>

#include <cassert>
#include <limits>
#include <stdexcept>
#include <utility>

using namespace gl;

struct CompileError : std::runtime_error {
  CompileError(std::string error)
      : std::runtime_error{""}, error{std::move(error)} {}

  char const *what() const noexcept override { return error.c_str(); }

  virtual ~CompileError() = default;

  std::string error;
};

static void compile(GLuint shader, std::string_view src) {
  if (src.size() > std::numeric_limits<GLsizei>::max())
    throw std::length_error{"Shader source code is too long!"};

  char const *srcp = src.data();
  GLint src_len = static_cast<GLint>(src.size());
  glShaderSource(shader, 1, &srcp, &src_len);
  assert(std::cmp_equal(src_len, src.size()));

  glCompileShader(shader);
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success)
    return;

  std::string error;
  error.resize(512);

  GLsizei length = static_cast<GLsizei>(error.size());
  glGetShaderInfoLog(shader, length, &length, error.data());

  assert(length > 0);
  error.resize(static_cast<size_t>(length));

  throw CompileError{error};
}

Shader::Shader(Shader::Kind kind, std::string_view src)
    : GLObject(glCreateShader(static_cast<GLenum>(kind))) {
  compile(id_, src);
}
