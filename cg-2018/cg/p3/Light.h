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
class Light: public Component
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
	_direction{ vec3f(0, 0, 0) },
	_ambient{ Color::white },
	_diffuse{ Color::white },
	_specular{ Color::white },
	_falloff{ 2 }
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

  vec3f direction()
  {
	  return _direction;
  }

  void setDirection(vec3f d)
  {
	  _direction = d;
  }

  Color ambient()
  {
	  return _ambient;
  }

  void setAmbient(Color a)
  {
	  _ambient = a;
  }

  Color diffuse()
  {
	  return _diffuse;
  }

  void setDiffuse(Color d)
  {
	  _diffuse = d;
  }

  Color specular()
  {
	  return _specular;
  }

  void setSpecular(Color s)
  {
	  _specular = s;
  }

  int falloff()
  {
	  return _falloff;
  }

  void setFalloff(int f)
  {
	  _falloff = f;
  }

private:
	Type _type;
	vec3f _direction;
	Color _ambient;
	Color _diffuse;
	Color _specular;
	int _falloff;

}; // Light

} // end namespace cg

#endif // __Light_h
