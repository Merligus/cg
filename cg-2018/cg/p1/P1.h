#ifndef __P1_h
#define __P1_h

#include "Primitive.h"
#include "Scene.h"
#include "graphics/Application.h"
#include <iostream>

class P1: public cg::GLWindow
{
public:
  using Base = cg::GLWindow;
  using Program = cg::GLSL::Program;
  using Color = cg::Color;

  P1(int width, int height):
    Base{"cg2018 - P1", width, height},
    _program{"P1"}
  {
    // do nothing
  }

  /// Initialize the app.
  void initialize() override;

  /// Update the GUI.
  void gui() override;

  /// Renders the scene.
  void render() override;

  /// Capta entradas do teclado
  bool keyInputEvent(int key, int action, int mods) override;


private:
  Program _program;
  Reference<cg::Scene> _scene;
  cg::SceneNode* _current{};
  Color selectedWireframeColor{255, 102, 0};


  void buildScene();

  void hierarchyWindow();
  void inspectorWindow();
  void sceneGui();
  void sceneObjectGui();
  void objectGui();

}; // P1

#endif // __P1_h
