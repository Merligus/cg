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
#include "Light.h"
#include "imgui.h"
#include <iostream>
#include <string>

namespace cg
{ // begin namespace cg

// Forward definition
	class Scene;


	/////////////////////////////////////////////////////////////////////
	//
	// SceneObject: scene object class
	// ===========
	class SceneObject : public SceneNode
	{
	public:
		bool visible{ true };
		bool myIteratorSet{ false };

		/// Constructs an empty scene object.
		SceneObject(const char* name, Scene& scene) :
			SceneNode{ name },
			_scene{ &scene },
			_parent{}
		{

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

		std::list<Reference<Component>>::iterator addComponent(Component* component)
		{
			for (auto it = _components.begin(); it != _components.end(); it++)
				if (strcmp((*it)->typeName(), component->typeName()) == 0)
					return it;

			component->_sceneObject = _myIterator;
			_components.push_back(component);

			if (strcmp(component->typeName(), "Transform") == 0)
			{
				Transform* t = (Transform*)&(**_components.begin());
				t->update();
			}
			return --_components.end();
		}

		/// Returns the transform of this scene object.
		Transform * transform()
		{
			for (auto it = _components.begin(); it != _components.end(); it++)
				if (strcmp((*it)->typeName(), "Transform") == 0)
					return (Transform*)&(**it);
			return nullptr;
		}

		Primitive * primitive()
		{
			for (auto it = _components.begin(); it != _components.end(); it++)
				if (strcmp((*it)->typeName(), "Primitive") == 0)
					return (Primitive*)&(**it);
			return nullptr;
		}

		Light * light()
		{
			for (auto it = _components.begin(); it != _components.end(); it++)
				if (strcmp((*it)->typeName(), "Light") == 0)
					return (Light*)&(**it);
			return nullptr;
		}

		/// Sets the parent of this scene object.
		void setParent(SceneObject* parent, bool flag = true); /// implementado em Scene.h
		void setMyIterator(std::list<SceneObject>::iterator it);
		SceneNode* autoDelete() const; /// implementado em Scene.h

		std::list<SceneObject>::iterator childrenBegin();
		std::list<SceneObject>::iterator childrenEnd();
		unsigned int childrenSize();
		std::list<SceneObject>::iterator appendChildren(SceneObject novo);
		std::list<SceneObject>::iterator removeChildren(std::list<SceneObject>::iterator it);

		std::list<Reference<Component>>::iterator componentsBegin();
		std::list<Reference<Component>>::iterator componentsEnd();
		unsigned int componentsSize();

		SceneNode* show(ImGuiTreeNodeFlags flag, SceneNode *current);
		void atualizaArvore(std::list<SceneObject>::iterator velho);
		void render(GLSL::Program *program, int *luzPontualIndex, int *luzDirecionalIndex, int *luzSpotIndex, int indexPrograma);

	private:
		Scene* _scene;
		SceneObject* _parent;
		std::list<SceneObject> _children;
		std::list<SceneObject>::iterator _myIterator;
		std::list<Reference<Component>> _components;

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
