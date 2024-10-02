#include "VertexShader.hpp"

#include <glbinding/gl/gl.h>

using namespace gl;

VertexShader::VertexShader(std::string_view src) : Shader(Kind::Vertex, src) {}
