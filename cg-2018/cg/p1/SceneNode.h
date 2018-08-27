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
// OVERVIEW: SceneNode.h
// ========
// Class definition for generic scene node.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 25/08/2018

#ifndef __SceneNode_h
#define __SceneNode_h

#include "core/SharedObject.h"
#include <string>
#include <list>

namespace cg
{ // begin namespace cg
	// Forward definition
	/*class SceneObject;*/

/////////////////////////////////////////////////////////////////////
//
// SceneNode: generic scene node class
// =========
class SceneNode: public SharedObject
{
public:
  /// Returns the name of this scene node.
  auto name() const
  {
    return _name.c_str();
  }

  /// Sets the name of this scene node.
  void setName(const char* format, ...);
  //std::list<SceneObject>::iterator append(SceneObject novo)
  //{
	 // _containerSize++; // incrementa  o tamanho da lista
	 // _container.push_back(novo); // insere na �ltima posi��o
	 // return _container.end(); // retorna o iterator pro "novo" na lista
  //}
  //std::list<SceneObject>::iterator remove(std::list<SceneObject>::iterator it)
  //{
	 // _containerSize--; // decrementa o tamanho da lista
	 // return _container.erase(it); // retorna o pr�ximo iterator. Caso it seja o �ltimo, retorna o in�cio
  //}


protected:
	SceneNode() = default;
	//SceneNode():
	//	_containerSize{0}
	//{
	//	// do nothing
	//}

  SceneNode(const char* name):
	  _name{ name }
	  //_containerSize{0}
  {
    // do nothing
  }

private:
  std::string _name;
  /*std::list<SceneObject> _container;
  unsigned int _containerSize;*/

}; // SceneNode

} // end namespace cg

#endif // __SceneNode_h
