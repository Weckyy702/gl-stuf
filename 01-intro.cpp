#include "ElementBufferObject.hpp"
#include "VertexBufferObject.hpp"
#include <cmath>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <iostream>
#include <ostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

#include <glbinding-aux/debug.h>

#define CONCAT_INNER(a, b) a##b
#define CONCAT(a, b) CONCAT_INNER(a, b)

#define DEFER Defer const CONCAT(__defer, __LINE__) = [&]()

using namespace gl;

template <typename F> struct [[maybe_unused]] Defer {
  Defer(F f) : f{f} {}

  ~Defer() { f(); }

  F f;
};

static void error_callback(int error, const char *description) {
  std::println(std::cerr, "[ERROR]: {}", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static int width, height;
static bool resized = true;

static void resize_callback(GLFWwindow *, int w, int h) {
  width = w;
  height = h;
  resized = true;
}

int main() {
  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
    return 1;

  DEFER { glfwTerminate(); };

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow *window =
      glfwCreateWindow(640, 480, "OpenGL Triangle", NULL, NULL);
  if (!window) {
    return 2;
  }

  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, resize_callback);

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glbinding::initialize(glfwGetProcAddress);
  glbinding::aux::enableGetErrorCallback();
  using namespace gl;

  float b = 0.;

  /* Shader initialization */
  std::string_view vertex_shader_src = R"(
    #version 330 core

    layout(location = 0) in vec3 position;

    void main() {
      gl_Position = vec4(position, 1.0);
    }
  )";

  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  {
    char const *src = vertex_shader_src.data();
    glShaderSource(vertex_shader, 1, &src, nullptr);
    glCompileShader(vertex_shader);

    {
      GLint success;
      glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        char _error[512]{};
        GLsizei length;
        glGetShaderInfoLog(vertex_shader, sizeof(_error), &length, _error);

        std::string_view error{_error, static_cast<size_t>(length)};

        std::println(std::cerr, "Error compiling vertex shader: {}", error);
        return 3;
      }
    }
  }

  std::string_view fragment_shader_src = R"(
    #version 330 core

    out vec4 frag_color;

    void main() {
      frag_color = vec4(1., 0., 0., 1.);
    }
  )";

  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  {
    char const *src = fragment_shader_src.data();
    glShaderSource(fragment_shader, 1, &src, nullptr);
    glCompileShader(fragment_shader);

    GLint success;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      char _error[512]{};
      GLsizei length;
      glGetShaderInfoLog(vertex_shader, sizeof(_error), &length, _error);

      std::string_view error{_error, static_cast<size_t>(length)};

      std::println(std::cerr, "Error compiling fragment shader: {}", error);
      return 4;
    }
  }

  GLuint program = glCreateProgram();
  {
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);
    {
      GLint success;
      glGetProgramiv(program, GL_LINK_STATUS, &success);
      if (!success) {
        char _error[512]{};
        GLsizei length;
        glGetProgramInfoLog(program, sizeof(_error), &length, _error);

        std::string_view error{_error, static_cast<size_t>(length)};

        std::println(std::cerr, "Error linkin shader program: {}", error);
        return 5;
      }
    }
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  DEFER { glDeleteProgram(program); };

  /* Vertex data initialization */
  std::array vertices{
      0.5f,  0.5f,  0.0f, // top right
      0.5f,  -0.5f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f  // top left
  };
  std::array indices{0u, 1u, 3u, 1u, 2u, 3u};

  GLuint vao;
  // Generate a new Vertex Array Object to hold our vertex attributes (defined
  // later)
  glGenVertexArrays(1, &vao);
  // Use the VAO for all future vertex array operations
  glBindVertexArray(vao);
  DEFER { glDeleteVertexArrays(1, &vao); };

  VertexBufferObject vbo(vertices, GL_STATIC_DRAW);
  ElementBufferObject ebo(indices, GL_STATIC_DRAW);

  // Declare what the vertex data looks like
  // index: Index of the shader input (see shader souce location)
  // size: Number of values in a vertex
  // type: Type of data stored in the buffer
  // nomralized: Translate integer values into [0, 1] floats
  // stride: Stride of the vertex data
  // pointer: Here: offset of the data in the buffer
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

  // Draw as wireframe
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  while (!glfwWindowShouldClose(window)) {
    if (resized) {
      glViewport(0, 0, width, height);
      resized = false;
    }
    b = static_cast<float>(sin(glfwGetTime()) * .5 + .5);
    glClearColor(0.5, 0., b, 1.);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
