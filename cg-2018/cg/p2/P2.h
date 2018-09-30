#ifndef __P2_h
#define __P2_h

#include "Assets.h"
#include "Primitive.h"
#include "Renderer.h"
#include "core/Flags.h"
#include "graphics/Application.h"

using namespace cg;

class P2: public GLWindow
{
public:
  P2(int width, int height):
    GLWindow{"cg2018 - P2", width, height},
    _program{"P2"}
  {
    // do nothing
  }

  /// Initialize the app.
  void initialize() override;

  /// Update the GUI.
  void gui() override;

  /// Renders the scene.
  void render() override;

private:
  enum class MoveBits
  {
    Left = 1,
    Right = 2,
    Forward = 4,
    Back = 8,
    Up = 16,
    Down = 32
  };

  enum class DragBits
  {
    Rotate = 1,
    Pan = 2
  };

  using SceneRef = Reference<Scene>;
  using CameraRef = Reference<Camera>;

  GLSL::Program _program;
  SceneRef _scene;
  CameraRef _camera;
  Reference<SceneObject> _box;
  Reference<Primitive> _primitive;
  SceneNode* _current{};
  Color _selectedWireframeColor{255, 102, 0};
  Flags<MoveBits> _moveFlags{};
  Flags<DragBits> _dragFlags{};
  int _pivotX;
  int _pivotY;
  int _mouseX;
  int _mouseY;

  static MeshMap _defaultMeshes;

  void buildScene();

  void mainMenu();
  void fileMenu();
  void showOptions();

  void hierarchyWindow();
  void inspectorWindow();
  void assetsWindow();
  void rendererWindow();
  void sceneGui();
  void sceneObjectGui();
  void objectGui();
  void cameraGui();
  void inspectPrimitive(Primitive&);

  bool windowResizeEvent(int, int) override;
  bool keyInputEvent(int, int, int) override;
  bool scrollEvent(double, double) override;
  bool mouseButtonInputEvent(int, int, int) override;
  bool mouseMoveEvent(double, double) override;

  static void buildDefaultMeshes();

}; // P2

#endif // __P2_h
