//[]---------------------------------------------------------------[]
//|                                                                 |
//| Copyright (C) 2014, 2018 Orthrus Group.                         |
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
//  OVERVIEW: Ray.h
//  ========
//  Class definition for ray.
//
// Author: Paulo Pagliosa
// Last revision: 15/09/2018

#ifndef __Ray_h
#define __Ray_h

#include "math/Matrix4x4.h"

namespace cg
{ // begin namespace cg


//////////////////////////////////////////////////////////
//
// Ray: ray class
// ===
struct Ray
{
  vec3f origin;
  vec3f direction;
  float tMin;
  float tMax;

  /// Constructs an empty Ray object.
  __host__ __device__
  Ray() = default;

  __host__ __device__
  Ray(const vec3f& origin,
    const vec3f& distance,
    float tMin = float(0),
    float tMax = math::Limits<float>::inf()):
    tMin{tMin},
    tMax{tMax}
  {
    set(origin, direction);
  }

  __host__ __device__
  Ray(const Ray& ray, const mat4f& m):
    tMin{ray.tMin},
    tMax{ray.tMax}
  {
    set(m.transform(ray.origin), m.transformVector(ray.direction));
  }

  __host__ __device__
  void set(const vec3f& origin, const vec3f& direction)
  {
    this->origin = origin;
    this->direction = direction.versor();
  }

  __host__ __device__
  void transform(const mat4f& m)
  {
    origin = m.transform(origin);
    direction = m.transformVector(direction).versor();
  }

  __host__ __device__
  vec3f operator ()(float t) const
  {
    return origin + direction * t;
  }

}; // Ray

} // end namespace cg

#endif // __Ray_h
