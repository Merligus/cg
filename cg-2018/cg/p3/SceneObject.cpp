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
#include "Scene.h"

namespace cg
{ // begin namespace cg


/////////////////////////////////////////////////////////////////////
//
// SceneObject implementation
// ===========

	void
		SceneObject::setParent(SceneObject* parent, bool flag)
	{
		std::cout << "Atualiza pai" << std::endl;
		// atualiza a lista do antigo pai
		if (myIteratorSet && flag)
		{
			if (parent == nullptr) // se pai é nulo então ele é raíz, logo precisa ser atualizado na coleção da classe SceneNodes
			{
				// copia transform primitive e children
				cg::SceneObject newBox(name(), *_scene);
				newBox.setParent(parent, false);
				auto it2 = _scene->append(newBox);
				SceneObject* aux = _myIterator->parent();
				_parent = parent;
				transform()->parentChanged();
				_parent = aux;

				std::list<Reference<Component>>::iterator comp;
				for (auto it = _components.begin(); it != _components.end(); it++)
				{
					comp = it2->addComponent(&(**it));
					(*comp)->setMyIterator(comp);
				}

				it2->atualizaArvore(_myIterator);
			}
			else // atualizar a lista do pai atual e inserir no novo pai
			{
				// copia transform primitive e children
				cg::SceneObject newBox(name(), *_scene);
				newBox.setParent(parent, false);
				auto it2 = parent->appendChildren(newBox);
				SceneObject* aux = _myIterator->parent();
				_parent = parent;
				transform()->parentChanged();
				_parent = aux;

				std::list<Reference<Component>>::iterator comp;
				for (auto it = _components.begin(); it != _components.end(); it++)
				{
					comp = it2->addComponent(&(**it));
					(*comp)->setMyIterator(comp);
				}

				it2->atualizaArvore(_myIterator);
			}
		}
		else
			_parent = parent; // atualiza o novo pai
	}

	SceneNode*
		SceneObject::autoDelete() const
	{
		if (_parent != nullptr)
		{
			SceneObject *aux = _parent;
			_parent->removeChildren(_myIterator);

			return aux;
		}
		else
		{
			Scene *aux = _scene;
			_scene->remove(_myIterator);
			return aux;
		}
	}

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
		SceneObject::appendChildren(SceneObject novo)
	{
		std::list<SceneObject>::iterator aux;
		std::cout << "Adiciona filho no pai" << std::endl;
		_children.push_back(novo); // insere na última posição
		aux = --_children.end(); // pega o iterator para a última posição da lista
		(*aux).setMyIterator(aux); // atualiza o iterator do elemento da lista
		return aux; // retorna o iterator pro "novo" na lista
	}

	std::list<SceneObject>::iterator
		SceneObject::removeChildren(std::list<SceneObject>::iterator it)
	{
		std::cout << "Remove filho do pai" << std::endl;
		return _children.erase(it); // retorna o próximo iterator. Caso it seja o último, retorna o iterator para _children.end()
	}

	SceneNode*
		SceneObject::show(ImGuiTreeNodeFlags flag, SceneNode* current)
	{
		if (_children.size() > 0)
		{
			auto open = ImGui::TreeNodeEx(this,
				current == this ? flag | ImGuiTreeNodeFlags_Selected : flag,
				this->name());
			//auto open = ImGui::TreeNode(this, this->name());
			if (ImGui::IsItemClicked())
				current = this;
			if (ImGui::BeginDragDropTarget())
			{
				if (auto* payload = ImGui::AcceptDragDropPayload("Objeto"))
				{
					auto mit = *(std::list<SceneObject>::iterator*)payload->Data;
					mit->setParent(this);
					auto it2 = --this->childrenEnd();
					mit->autoDelete();
					current = &(*it2);
				}
				ImGui::EndDragDropTarget();
			}
			if (ImGui::BeginDragDropSource())
			{
				auto mit = _myIterator;
				ImGui::SetDragDropPayload("Objeto", &mit, sizeof(mit));
				ImGui::Text("Move %s", _myIterator->name());
				ImGui::EndDragDropSource();
			}
			if (open)
			{
				for (std::list<SceneObject>::iterator it = _children.begin(); it != _children.end(); ++it)
					current = it->show(flag, current);

				ImGui::TreePop();
			}
		}
		else
		{
			flag |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
			ImGui::TreeNodeEx(this,
				current == this ? flag | ImGuiTreeNodeFlags_Selected : flag,
				this->name());
			if (ImGui::IsItemClicked())
				current = this;
			if (ImGui::BeginDragDropTarget())
			{
				if (auto* payload = ImGui::AcceptDragDropPayload("Objeto"))
				{
					auto mit = *(std::list<SceneObject>::iterator*)payload->Data;
					mit->setParent(this);
					auto it2 = --(this->childrenEnd());
					mit->autoDelete();
					current = &(*it2);
				}
				ImGui::EndDragDropTarget();
			}
			if (ImGui::BeginDragDropSource())
			{
				auto mit = _myIterator;
				ImGui::SetDragDropPayload("Objeto", &mit, sizeof(mit));
				ImGui::Text("Move %s", _myIterator->name());
				ImGui::EndDragDropSource();
			}
		}
		return current;
	}

	void
		SceneObject::atualizaArvore(std::list<SceneObject>::iterator velho)
	{
		_children.clear();
		for (std::list<SceneObject>::iterator it = velho->childrenBegin(); it != velho->childrenEnd(); ++it) // copia _children
		{
			std::list<cg::SceneObject>::iterator it3 = _myIterator->appendChildren(*it);
			it3->setParent(&(*_myIterator), false);

			for (auto comp = it3->componentsBegin(); comp != it3->componentsEnd(); comp++)
			{
				(*comp)->_sceneObject = it3;
				(*comp)->setMyIterator(comp);
			}
			it3->atualizaArvore(it);
		}
	}

	void
		SceneObject::render(GLSL::Program *program, int *luzPontualIndex, int *luzDirecionalIndex, int *luzSpotIndex, int indexPrograma)
	{
		if (this->visible)
		{
			if (light() != nullptr && indexPrograma != 2)
			{
				if (light()->type() == Light::Type::Directional && (*luzSpotIndex) + (*luzDirecionalIndex) + (*luzPontualIndex) < 8)
				{
					program->setUniformVec4((std::string("luzesDirecionais[") + std::to_string(*luzDirecionalIndex) + std::string("].color")).c_str(), light()->color());
					program->setUniformVec3((std::string("luzesDirecionais[") + std::to_string(*luzDirecionalIndex) + std::string("].direction")).c_str(), light()->transform()->rotation() * vec3f {0, -1.0f, 0 });
					(*luzDirecionalIndex)++;
				}
				else if (light()->type() == Light::Type::Point && (*luzSpotIndex) + (*luzDirecionalIndex) + (*luzPontualIndex) < 8)
				{
					program->setUniformVec4((std::string("luzesPontuais[") + std::to_string(*luzPontualIndex) + std::string("].color")).c_str(), light()->color());
					program->setUniform((std::string("luzesPontuais[") + std::to_string(*luzPontualIndex) + std::string("].falloff")).c_str(), (int)light()->falloff());
					program->setUniformVec3((std::string("luzesPontuais[") + std::to_string(*luzPontualIndex) + std::string("].position")).c_str(), transform()->position());
					(*luzPontualIndex)++;
				}
				else if (light()->type() == Light::Type::Spot && (*luzSpotIndex) + (*luzDirecionalIndex) + (*luzPontualIndex) < 8)
				{
					program->setUniformVec4((std::string("luzesSpots[") + std::to_string(*luzSpotIndex) + std::string("].color")).c_str(), light()->color());
					program->setUniform((std::string("luzesSpots[") + std::to_string(*luzSpotIndex) + std::string("].falloff")).c_str(), (int)light()->falloff());
					program->setUniformVec3((std::string("luzesSpots[") + std::to_string(*luzSpotIndex) + std::string("].position")).c_str(), transform()->position());
					program->setUniformVec3((std::string("luzesSpots[") + std::to_string(*luzSpotIndex) + std::string("].direction")).c_str(), light()->transform()->rotation() * vec3f { 0, -1.0f, 0 });
					program->setUniform((std::string("luzesSpots[") + std::to_string(*luzSpotIndex) + std::string("].innerCutOff")).c_str(), (float)cos(0.01745329251*light()->innerCutOff()));
					program->setUniform((std::string("luzesSpots[") + std::to_string(*luzSpotIndex) + std::string("].outerCutOff")).c_str(), (float)cos(0.01745329251*light()->outerCutOff()));
					(*luzSpotIndex)++;
				}
			}
			if (this->primitive() != nullptr)
			{
				if (this->primitive()->mesh() != nullptr)
				{
					cg::GLMesh * m = glMesh((this->primitive())->mesh());
					cg::Transform *t = transform();
					auto normalMatrix = mat3f{ t->worldToLocalMatrix() }.transposed();
					program->setUniformMat4("transform", t->localToWorldMatrix());
					if(indexPrograma < 2)
						program->setUniformMat3("normalMatrix", normalMatrix);
					if (indexPrograma != 2)
					{
						program->setUniformVec4("material.ambient", primitive()->material.ambient);
						program->setUniformVec4("material.diffuse", primitive()->material.diffuse);
						program->setUniformVec4("material.spot", primitive()->material.spot);
						program->setUniform("material.shine", (float)primitive()->material.shine);
						program->setUniform("flatMode", (int)0);
					}
					m->bind();
					if (indexPrograma == 2)
					{
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						glDrawElements(GL_TRIANGLES, m->vertexCount(), GL_UNSIGNED_INT, 0);
					}
					else
					{
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glDrawElements(GL_TRIANGLES, m->vertexCount(), GL_UNSIGNED_INT, 0);
					}
				}
			}
		}
		for (std::list<SceneObject>::iterator it = this->childrenBegin(); it != this->childrenEnd(); ++it)
			it->render(program, luzPontualIndex, luzDirecionalIndex, luzSpotIndex, indexPrograma);
	}

	std::list<Reference<Component>>::iterator
		SceneObject::componentsBegin()
	{
		return _components.begin();
	}

	std::list<Reference<Component>>::iterator
		SceneObject::componentsEnd()
	{
		return _components.end();
	}

	unsigned int
		SceneObject::componentsSize()
	{
		return _components.size();
	}

} // end namespace cg
