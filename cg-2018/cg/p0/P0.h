#ifndef __P0_h
#define __P0_h

#include "graphics/Application.h"
#include "math/Matrix4x4.h"

class P0: public cg::GLWindow
{
public:
  using Base = cg::GLWindow;
  using Program = cg::GLSL::Program;
  using Color = cg::Color;

  P0(int width, int height):
    Base{"cg2018 - P0", width, height},
    _program{"P0"}
  {
    // do nothing
  }

  ~P0()
  {
    glDeleteVertexArrays(1, &_vao);
  }

  /// Initialize the app.
  void initialize() override;

  /// Update the GUI.
  void gui() override;

  /// Renders the scene.
  void render() override;

private:
  Program _program;
  GLuint _vao{0};
  Color _triangleColor{Color::blue};
  mat4f _transform{mat4f::identity()};

}; // P0

#endif // __P0_h
