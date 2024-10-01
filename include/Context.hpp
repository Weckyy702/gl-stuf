#include <exception>
namespace opengl {
class Window;

class Context {
public:
  struct InitFailed : public std::exception {
    char const *what() const noexcept override {
      return "Initializing GLFW failed!";
    }
  };

  static Context make();

  ~Context() noexcept;

private:
  Context() = default;
};

} // namespace opengl
