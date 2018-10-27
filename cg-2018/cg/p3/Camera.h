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
// OVERVIEW: Camera.h
// ========
// Class definition for camera.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 21/09/2018

#ifndef __Camera_h
#define __Camera_h

#include "core/Flags.h"
#include "core/NameableObject.h"
#include "math/Matrix4x4.h"

namespace cg
{ // begin namespace cg

#define MIN_HEIGHT      0.01f
#define MIN_ASPECT      0.1f
#define MIN_DISTANCE    0.01f
#define MIN_ANGLE       1.0f
#define MAX_ANGLE       179.0f
#define MIN_DEPTH       0.01f
#define MIN_FRONT_PLANE 0.01f


//////////////////////////////////////////////////////////
//
// Camera: camera class
// ======
class Camera: public NameableObject
{
public:
  enum ProjectionType
  {
    Perspective,
    Parallel
  };

  Camera();

  vec3f position() const;
  vec3f eulerAngles() const;
  quatf rotation() const;
  float viewAngle() const;
  float height() const;
  float aspectRatio() const;
  float clippingPlanes(float& F, float& B) const;
  ProjectionType projectionType() const;
  vec3f focalPoint() const;
  float distance() const;

  void setPosition(const vec3f& value);
  void setEulerAngles(const vec3f& value);
  void setRotation(const quatf& value);
  void setViewAngle(float value);
  void setHeight(float value);
  void setAspectRatio(float value);
  void setClippingPlanes(float F, float B);
  void setProjectionType(ProjectionType value);
  void setDistance(float value);

  void setDefaultView(float aspect = 1.0f);

  void rotateYX(float ay, float ax, bool orbit = false);
  void zoom(float zoom);
  void translate(float dx, float dy, float dz);
  void translate(const vec3f& d);

  mat4f worldToCameraMatrix() const;
  mat4f cameraToWorldMatrix() const;
  mat4f projectionMatrix() const;
  vec3f worldToCamera(const vec3f& p) const;
  vec3f cameraToWorld(const vec3f& p) const;

private:
  static uint32_t _nextId;

  vec3f _position;
  vec3f _eulerAngles;
  quatf _rotation;
  float _viewAngle;
  float _height;
  float _aspectRatio;
  float _F;
  float _B;
  ProjectionType _projectionType;
  vec3f _focalPoint;
  float _distance;
  mat4f _matrix; // view matrix
  mat4f _inverseMatrix;
  mat4f _projectionMatrix;

  static const char* defaultName();

  friend class Renderer;

}; // Camera

inline vec3f
Camera::position() const
{
  return _position;
}

inline vec3f
Camera::eulerAngles() const
{
  return _eulerAngles;
}

inline quatf
Camera::rotation() const
{
  return _rotation;
}

inline Camera::ProjectionType
Camera::projectionType() const
{
  return _projectionType;
}
inline vec3f
Camera::focalPoint() const
{
  return _focalPoint;
}

inline float
Camera::distance() const
{
  return _distance;
}

inline float
Camera::viewAngle() const
{
  return _viewAngle;
}

inline float
Camera::height() const
{
  return _height;
}

inline float
Camera::aspectRatio() const
{
  return _aspectRatio;
}

inline float
Camera::clippingPlanes(float& F, float& B) const
{
  F = _F;
  B = _B;
  return B - F;
}

inline void
Camera::translate(const vec3f& d)
{
  if (d.isNull())
    return;
  translate(d.x, d.y, d.z);
}

inline mat4f
Camera::worldToCameraMatrix() const
{
  return _matrix;
}

inline mat4f
Camera::cameraToWorldMatrix() const
{
  return _inverseMatrix;
}

inline mat4f
Camera::projectionMatrix() const
{
  return _projectionMatrix;
}

inline vec3f
Camera::worldToCamera(const vec3f& p) const
{
  return _matrix.transform3x4(p);
}

inline vec3f
Camera::cameraToWorld(const vec3f& p) const
{
  return _inverseMatrix.transform3x4(p);
}

//
// Auxiliary function
//
inline auto
vpMatrix(const Camera* c)
{
  return c->projectionMatrix() * c->worldToCameraMatrix();
}

} // end namespace cg

#endif // __Camera_h
