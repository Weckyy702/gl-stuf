#include "FragmentShader.hpp"

#include <glbinding/gl/gl.h>

using namespace gl;

FragmentShader::FragmentShader(std::string_view src)
    : Shader(Kind::Fragment, src) {}
