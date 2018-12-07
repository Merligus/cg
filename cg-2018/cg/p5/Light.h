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
// OVERVIEW: Light.h
// ========
// Class definition for light.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 05/10/2018

#ifndef __Light_h
#define __Light_h

#include "Component.h"
#include "graphics/Color.h"

namespace cg
{ // begin namespace cg


/////////////////////////////////////////////////////////////////////
//
// Light: light class
// =====
	class Light : public Component
	{
	public:
		enum Type
		{
			Directional,
			Point,
			Spot
		};

		Light() :
			Component{ "Light" },
			_type{ Point },
			_color{ Color::gray },
			_falloff{ 1 },
			_innerCutOff{ 10.0f },
			_outerCutOff{ 14.0f }
		{
			// do nothing 
		}

		auto type() const
		{
			return _type;
		}

		void setType(Type type)
		{
			_type = type;
		}

		Color color()
		{
			return _color;
		}

		void setColor(Color a)
		{
			_color = a;
		}

		int falloff()
		{
			return _falloff;
		}

		void setFalloff(int f)
		{
			_falloff = f;
		}

		float innerCutOff()
		{
			return _innerCutOff;
		}

		void setInnerCutOff(float i)
		{
			_innerCutOff = i;
		}

		float outerCutOff()
		{
			return _outerCutOff;
		}

		void setOuterCutOff(float o)
		{
			_outerCutOff = o;
		}

	private:
		Type _type;
		Color _color;
		int _falloff;
		float _innerCutOff;
		float _outerCutOff;
	}; // Light

} // end namespace cg

#endif // __Light_h
