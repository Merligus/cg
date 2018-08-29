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
// OVERVIEW: SceneObject.cpp
// ========
// Source file for scene object.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 25/08/2018

#include "SceneObject.h"

namespace cg
{ // begin namespace cg


/////////////////////////////////////////////////////////////////////
//
// SceneObject implementation
// ===========

void
SceneObject::setMyIterator(std::list<SceneObject>::iterator it)
{
	_myIterator = it;
	myIteratorSet = true;
}

std::list<SceneObject>::iterator
SceneObject::childrenBegin()
{
	return _children.begin();
}

std::list<SceneObject>::iterator
SceneObject::childrenEnd()
{
	return _children.end();
}

unsigned int
SceneObject::childrenSize()
{
	return _children.size();
}

std::list<SceneObject>::iterator
SceneObject::append(SceneObject novo)
{
	std::list<SceneObject>::iterator aux;
	std::cout << "Adiciona no pai" << std::endl;
	_children.push_back(novo); // insere na �ltima posi��o
	aux = --_children.end(); // pega o iterator para a �ltima posi��o da lista
	(*aux).setMyIterator(aux); // atualiza o iterator do elemento da lista
	return aux; // retorna o iterator pro "novo" na lista
}

std::list<SceneObject>::iterator
SceneObject::remove(std::list<SceneObject>::iterator it)
{
	std::cout << "Remove do pai" << std::endl;
	return _children.erase(it); // retorna o pr�ximo iterator. Caso it seja o �ltimo, retorna o iterator para _children.end()
}

} // end namespace cg
