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
// OVERVIEW: Camera.cpp
// ========
// Source file for camera.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 15/09/2018

#include "Camera.h"

namespace cg
{ // begin namespace cg


//////////////////////////////////////////////////////////
//
// Camera implementation
// ======
uint32_t Camera::_nextId;

inline const char*
Camera::defaultName()
{
  static char name[16];

  snprintf(name, sizeof name, "Camera %d", ++_nextId);
  return name;
}

Camera::Camera():
  NameableObject{defaultName()}
{
  setDefaultView();
}

void
Camera::setPosition(const vec3f& value)
//[]---------------------------------------------------[]
//|  Set the camera's position                          |
//|                                                     |
//|  Setting the camera's position will not change      |
//|  neither the direction of projection nor the        |
//|  distance between the position and the focal point. |
//|  The focal point will be moved along the direction  |
//|  of projection.                                     |
//[]---------------------------------------------------[]
{
  // TODO
	_position = value;

	mat4f::vec3 front, Front, eulerAnglesInRadians;
	eulerAnglesInRadians = toRadians3(_eulerAngles);
	front.x = cos(eulerAnglesInRadians.x) * cos(eulerAnglesInRadians.y);
	front.y = sin(eulerAnglesInRadians.y);
	front.z = sin(eulerAnglesInRadians.x) * cos(eulerAnglesInRadians.y);
	Front = front.versor();

	_focalPoint = _distance * Front + _position;
	updateViewMatrix();
}

void
Camera::setEulerAngles(const vec3f& value)
{
  // TODO
	_eulerAngles = value;
	if (_eulerAngles.y > 89.0f)
		_eulerAngles.y = 89.0f;
	if (_eulerAngles.y < -89.0f)
		_eulerAngles.y = -89.0f;

	mat4f::vec3 front, Front, eulerAnglesInRadians;
	eulerAnglesInRadians = toRadians3(_eulerAngles);
	front.x = cos(eulerAnglesInRadians.x) * cos(eulerAnglesInRadians.y);
	front.y = sin(eulerAnglesInRadians.y);
	front.z = sin(eulerAnglesInRadians.x) * cos(eulerAnglesInRadians.y);
	Front = front.versor();

	_focalPoint = _distance * Front + _position;
	updateViewMatrix();
}

void
Camera::setRotation(const quatf& value)
{
  // TODO
	_rotation = value;
	mat4f::vec3 front, Front, eulerAnglesInRadians;
	eulerAnglesInRadians = toRadians3(_eulerAngles);
	front.x = cos(eulerAnglesInRadians.x) * cos(eulerAnglesInRadians.y);
	front.y = sin(eulerAnglesInRadians.y);
	front.z = sin(eulerAnglesInRadians.x) * cos(eulerAnglesInRadians.y);
	Front = front.versor();

	_focalPoint = _distance * Front + _position;
	updateViewMatrix();
}

void
Camera::setProjectionType(ProjectionType value)
//[]---------------------------------------------------[]
//|  Set the camera's projection type                   |
//[]---------------------------------------------------[]
{
  // TODO
	_projectionType = value;

	updateProjectionMatrix();
}

void
Camera::setDistance(float value)
//[]---------------------------------------------------[]
//|  Set the camera's distance                          |
//|                                                     |
//|  Setting the distance between the position and      |
//|  focal point will move the focal point along the    |
//|  direction of projection.                           |
//[]---------------------------------------------------[]
{
  // TODO
	mat4f::vec3 front, Front, eulerAnglesInRadians;
	eulerAnglesInRadians = toRadians3(_eulerAngles);
	front.x = cos(eulerAnglesInRadians.x) * cos(eulerAnglesInRadians.y);
	front.y = sin(eulerAnglesInRadians.y);
	front.z = sin(eulerAnglesInRadians.x) * cos(eulerAnglesInRadians.y);
	Front = front.versor();

	_focalPoint = value*Front + _position;
	_distance = value;
	updateViewMatrix();
}

void
Camera::setViewAngle(float value)
//[]---------------------------------------------------[]
//|  Set the camera's view angle                        |
//[]---------------------------------------------------[]
{
  // TODO
	_viewAngle = value;
	updateProjectionMatrix();
}

void
Camera::setHeight(float value)
//[]---------------------------------------------------[]
//|  Set the camera's view_height                       |
//[]---------------------------------------------------[]
{
  // TODO
	_height = value;
	_position.z = _height + _focalPoint.z;
	_distance = (_focalPoint - _position).length();
	updateProjectionMatrix();
}

void
Camera::setAspectRatio(float value)
//[]---------------------------------------------------[]
//|  Set the camera's aspect ratio                      |
//[]---------------------------------------------------[]
{
  // TODO
	_aspectRatio = value;
	updateProjectionMatrix();
}

void
Camera::setClippingPlanes(float F, float B)
//[]---------------------------------------------------[]
//|  Set the distance of the clippling planes           |
//[]---------------------------------------------------[]
{
  // TODO
	_F = F;
	_B = B;
	updateProjectionMatrix();
}

void
Camera::rotateYX(float ay, float ax, bool orbit)
//[]---------------------------------------------------[]
//|  Rotate YX                                          |
//|                                                     |
//|  If orbit is true, then it is a composition of an   |
//|  azimuth of ay with an elevation of ax, in this     |
//|  order. Otherwise, it is a composition of a yaw of  |
//|  ay with a pitch of ax, in this order.              |
//[]---------------------------------------------------[]
{
  // TODO
	if (orbit == true)
	{
		_eulerAngles.x = _eulerAngles.x + ay;
		_eulerAngles.y = _eulerAngles.y + ax;
		if (_eulerAngles.y > 89.0f)
			_eulerAngles.y = 89.0f;
		if (_eulerAngles.y < -89.0f)
			_eulerAngles.y = -89.0f;

		mat4f::vec3 front, Front, eulerAnglesInRadians;
		eulerAnglesInRadians = toRadians3(_eulerAngles);
		front.x = cos(eulerAnglesInRadians.x) * cos(eulerAnglesInRadians.y);
		front.y = sin(eulerAnglesInRadians.y);
		front.z = sin(eulerAnglesInRadians.x) * cos(eulerAnglesInRadians.y);
		Front = front.versor();

		_position = _focalPoint -_distance * Front;
	}
	else
	{
		_eulerAngles.x = _eulerAngles.x + ay;
		_eulerAngles.y = _eulerAngles.y + ax;
		if (_eulerAngles.y > 89.0f)
			_eulerAngles.y = 89.0f;
		if (_eulerAngles.y < -89.0f)
			_eulerAngles.y = -89.0f;

		mat4f::vec3 front, Front, eulerAnglesInRadians;
		eulerAnglesInRadians = toRadians3(_eulerAngles);
		front.x = cos(eulerAnglesInRadians.x) * cos(eulerAnglesInRadians.y);
		front.y = sin(eulerAnglesInRadians.y);
		front.z = sin(eulerAnglesInRadians.x) * cos(eulerAnglesInRadians.y);
		Front = front.versor();

		_focalPoint = _distance * Front + _position;
	}
	updateViewMatrix();
}

void
Camera::zoom(float zoom)
//[]---------------------------------------------------[]
//|  Zoom                                               |
//|                                                     |
//|  Change the view angle (or height) of the camera so |
//|  that more or less of a scene occupies the view     |
//|  window. A value > 1 is a zoom-in. A value < 1 is   |
//|  zoom-out.                                          |
//[]---------------------------------------------------[]
{
  // TODO
	if (_projectionType == Parallel)
		setHeight(_height / zoom);
	else
		setViewAngle(_viewAngle / zoom);

	updateProjectionMatrix();
}

void
Camera::translate(float dx, float dy, float dz)
//[]---------------------------------------------------[]
//|  Translate the camera                               |
//[]---------------------------------------------------[]
{
  // TODO
	mat4f::vec3 WorldUp(0.0f, 1.0f, 0.0f), Right, Up;
	mat4f::vec3 front, Front, eulerAnglesInRadians;
	eulerAnglesInRadians = toRadians3(_eulerAngles);
	front.x = cos(eulerAnglesInRadians.x) * cos(eulerAnglesInRadians.y);
	front.y = sin(eulerAnglesInRadians.y);
	front.z = sin(eulerAnglesInRadians.x) * cos(eulerAnglesInRadians.y);
	Front = front.versor();

	Right = (Front.cross(WorldUp)).versor();


	if (dz < 0)
	{
		_position.x -= Front.x * dz;
		_position.z -= Front.z * dz;
		_focalPoint.x -= Front.x * dz;
		_focalPoint.z -= Front.z * dz;
	}
	if (dz > 0)
	{
		_position.x -= Front.x * dz;
		_position.z -= Front.z * dz;
		_focalPoint.x -= Front.x * dz;
		_focalPoint.z -= Front.z * dz;
	}
	if (dx < 0)
	{
		_position += Right * dx;
		_focalPoint += Right * dx;
	}
	if (dx > 0)
	{
		_position += Right * dx;
		_focalPoint += Right * dx;
	}
	_position.y += dy;
	_focalPoint.y += dy * Front.y;

	updateViewMatrix();
}

void
Camera::setDefaultView(float aspect)
//[]---------------------------------------------------[]
//|  Set default view                                   |
//[]---------------------------------------------------[]
{
  _position.set(0.0f, 0.0f, 10.0f);
  _eulerAngles.set(0.0f);
  _eulerAngles.x = -90.0f;
  _rotation = quatf::identity();
  _focalPoint.set(0.0f, 0.0f, 0.0f);
  _distance = 10.0f;
  _aspectRatio = aspect;
  _projectionType = Perspective;
  _viewAngle = 60.0f;
  _height = 10.0f;
  _F = 0.01f;
  _B = 1000.0f;
  // TODO: update view and projection matrices.
  updateViewMatrix();
  updateProjectionMatrix();
}

void 
Camera::updateProjectionMatrix()
{
	if (_projectionType == Perspective)
		_projectionMatrix = mat4f::perspective(_viewAngle, _aspectRatio, _F, _B);
	else
		_projectionMatrix = mat4f::ortho(-1.0f/_height/_height, 1.0f/_height, -1.0f/_height, 1.0f/_height, _F, _B);
}

void
Camera::updateViewMatrix()
{
	mat4f::vec3 WorldUp(0.0f, 1.0f, 0.0f), Right, Up;
	mat4f::vec3 front, Front, eulerAnglesInRadians;
	eulerAnglesInRadians = toRadians3(_eulerAngles);
	front.x = cos(eulerAnglesInRadians.x) * cos(eulerAnglesInRadians.y);
	front.y = sin(eulerAnglesInRadians.y);
	front.z = sin(eulerAnglesInRadians.x) * cos(eulerAnglesInRadians.y);
	Front = front.versor();

	Right = (Front.cross(WorldUp)).versor();
	Up = (Right.cross(Front)).versor();

	/*_inverseMatrix = _matrix = mat4f::lookAt(_position, _position + Front, Up);*/
	_matrix = mat4f::lookAt(_position, _position + Front, Up);
	_inverseMatrix = mat4f::lookAt(_position, _position + Front, Up);
	_inverseMatrix.inverse(_inverseMatrix);
}

} // end namespace cg
