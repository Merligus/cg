//[]---------------------------------------------------------------[]
//|                                                                 |
//| Copyright (C) 2018 Orthrus Group.                               |
//|                                                                 |
//| This software is provided 'as-is', without any express or       |
//| implied warranty. In no event will the authors be held liable   |
//| for any damages arising from the use of this software.          |
//|                                                                 |
//| Permission is granted to anyone to use this software for any    |
//| purpose, including commercial applications, and to alter it and |
//| redistribute it freely, subject to the following restrictions:  |
//|                                                                 |
//| 1. The origin of this software must not be misrepresented; you  |
//| must not claim that you wrote the original software. If you use |
//| this software in a product, an acknowledgment in the product    |
//| documentation would be appreciated but is not required.         |
//|                                                                 |
//| 2. Altered source versions must be plainly marked as such, and  |
//| must not be misrepresented as being the original software.      |
//|                                                                 |
//| 3. This notice may not be removed or altered from any source    |
//| distribution.                                                   |
//|                                                                 |
//[]---------------------------------------------------------------[]
//
// OVERVIEW: SceneObject.h
// ========
// Class definition for scene object.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 25/08/2018

#ifndef __SceneObject_h
#define __SceneObject_h

#include "SceneNode.h"
#include "Transform.h"
#include "Primitive.h"
#include "imgui.h"
#include <iostream>

namespace cg
{ // begin namespace cg

// Forward definition
class Scene;


/////////////////////////////////////////////////////////////////////
//
// SceneObject: scene object class
// ===========
class SceneObject: public SceneNode
{
public:
  bool visible{true};
  bool myIteratorSet{false};

  /// Constructs an empty scene object.
  SceneObject(const char* name, Scene* scene) :
	  SceneNode{ name },
	  _scene{ scene },
	  _parent{}
  {
	  _components.push_back(new Transform());
	  std::list<Component*>::iterator begin = _components.begin();
	  makeUse((Transform*)(*begin)->mySelf());
  }

  /// Constructs defined scene object.
  SceneObject(const char* name, Scene* scene, Primitive * p):
    SceneNode{name},
    _scene{scene},
    _parent{}
  {
	_components.push_back(new Transform());
	std::list<Component*>::iterator begin = _components.begin();
    makeUse((Transform*)(*begin)->mySelf());

	_components.push_back(p);
	_primitiveInUse = --_components.end();
	(*_primitiveInUse)->setMyIterator(_primitiveInUse);
  }

  /// Returns the scene which this scene object belong to.
  auto scene() const
  {
    return _scene;
  }

  /// Returns the parent of this scene object.
  auto parent() const
  {
    return _parent;
  }

  /// Returns the transform of this scene object.
  auto transform()
  {
	  return (Transform*)((*_components.begin())->mySelf());
  }

  Primitive * primitive()
  {
	  if (_components.size() > 1)
		  return (Primitive*)((*_primitiveInUse)->mySelf());
	  else
		  return nullptr;
  }

  /// Sets the parent of this scene object.
  void setParent(SceneObject* parent); /// implementado em Scene.h
  void setMyIterator(std::list<SceneObject>::iterator it);
  SceneObject* mySelf();
  std::list<SceneObject>::iterator childrenBegin();
  std::list<SceneObject>::iterator childrenEnd();
  auto childrenSize();
  std::list<SceneObject>::iterator appendChildren(SceneObject novo);
  std::list<SceneObject>::iterator removeChildren(std::list<SceneObject>::iterator it);

  SceneNode* show(ImGuiTreeNodeFlags flag, SceneNode *current);
  void render(GLSL::Program *program);

  void setPrimitiveInUse(std::list<Component*>::iterator it);
  std::list<Component*>::iterator componentsBegin();
  std::list<Component*>::iterator componentsEnd();
  auto componentsSize();
  std::list<Component*>::iterator appendComponents(Component* novo);
  std::list<Component*>::iterator removeComponents(std::list<Component*>::iterator it);

private:
  Scene* _scene;
  SceneObject* _parent;
  std::list<SceneObject> _children;
  std::list<SceneObject>::iterator _myIterator;
  std::list<Component*> _components;
  std::list<Component*>::iterator _primitiveInUse;

  friend class Scene;

}; // SceneObject

/// Returns the transform of a component.
inline Transform*
Component::transform() // declared in Component.h
{
  return sceneObject()->transform();
}

/// Returns the parent of a transform.
inline Transform*
Transform::parent() const // declared in Transform.h
{
  if (auto p = sceneObject()->parent())
    return p->transform();
   return nullptr;
}

} // end namespace cg

#endif // __SceneObject_h
