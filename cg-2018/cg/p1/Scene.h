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
// OVERVIEW: Scene.h
// ========
// Class definition for scene.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 25/08/2018

#ifndef __Scene_h
#define __Scene_h

#include "SceneObject.h"
#include "graphics/Color.h"

namespace cg
{ // begin namespace cg


/////////////////////////////////////////////////////////////////////
//
// Scene: scene class
// =====
class Scene: public SceneNode
{
public:
  Color backgroundColor{Color::gray};

  /// Constructs an empty scene.
  Scene(const char* name):
    SceneNode{name},
	_containerSize{0}
  {
    // do nothing
  }


  std::list<SceneObject>::iterator append(SceneObject novo)
  {
	  std::list<SceneObject>::iterator aux;
	  _containerSize++; // incrementa  o tamanho da lista
	  _container.push_back(novo); // insere na última posição
	  aux = --_container.end(); // pega o iterator para a última posição da lista
	  (*aux).setMyIterator(aux); // atualiza o iterator do elemento da lista
	  return aux; // retorna o iterator pro "novo" na lista
  }

  std::list<SceneObject>::iterator remove(std::list<SceneObject>::iterator it)
  {
	  _containerSize--; // decrementa o tamanho da lista
	  return _container.erase(it); // retorna o próximo iterator. Caso it seja o último, retorna o iterator para _children.end()
  }

private:
  std::list<SceneObject> _container;
  unsigned int _containerSize;

}; // Scene

inline void
SceneObject::setParent(SceneObject* parent)
{
	// atualiza a lista do antigo pai
	if (_parent == nullptr) // se pai é nulo então ele é raíz, logo precisa ser atualizado na coleção da classe SceneNodes
		_scene->remove(_myIterator);
	else // atualizar a lista do pai atual e inserir no novo pai
		_parent->remove(_myIterator); // retira do pai antigo

	// atualiza a lista do novo pai
	if (parent != nullptr) // adiciona na lista do novo pai
		_myIterator = parent->append(*this); // adiciona no novo pai e atualiza o myIterator
	else // adiciona na lista de raízes da cena
		_myIterator = _scene->append(*this); // adiciona nas raízes e atualiza o myIterator
	_parent = parent; // atualiza o novo pai
}

} // end namespace cg

#endif // __Scene_h
