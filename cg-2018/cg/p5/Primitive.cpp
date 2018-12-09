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
// OVERVIEW: Primitive.cpp
// ========
// Source file for primitive.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 30/10/2018

#include "Primitive.h"
#include "Transform.h"

namespace cg
{ // begin namespace cg


bool
Primitive::intersect(const Ray& ray, float& distance, int& tIndex, vec3f& position) const
{
	if (_mesh == nullptr)
		return false;

	auto trans = const_cast<Primitive*>(this)->transform();
	Ray localRay{ray.origin, ray.direction}; // trans->worldToLocalMatrix()
	localRay.origin = trans->worldToLocalMatrix().transform(ray.origin);
	localRay.direction = trans->worldToLocalMatrix().transformVector(ray.direction);
	auto d = math::inverse(localRay.direction.length());
	float tMin;
	float tMax;

	localRay.direction *= d;
	if (_mesh->bounds().intersect(localRay, tMin, tMax))
	{
		// TODO: _mesh intersection
		auto data = _mesh->data();
		float t = math::Limits<float>::inf(), b1 = 0, b2 = 0;
		bool inter = false;
		for (int triangleIndex = 0; triangleIndex < data.numberOfTriangles; triangleIndex++)
		{
			vec3f e1 = data.vertices[data.triangles[triangleIndex].v[1]] - data.vertices[data.triangles[triangleIndex].v[0]];
			vec3f e2 = data.vertices[data.triangles[triangleIndex].v[2]] - data.vertices[data.triangles[triangleIndex].v[0]];
			vec3f s1 = localRay.direction.cross(e2);
			float dir = s1.dot(e1);
			if (dir == 0)
				continue;
			dir = 1 / dir;
			vec3f s = localRay.origin - data.vertices[data.triangles[triangleIndex].v[0]];
			vec3f s2 = s.cross(e1);
			float tAux = (s2.dot(e2)) * dir;
			if (tAux <= 0)
				continue;
			float b1Aux = (s1.dot(s)) * dir;
			if (b1Aux < 0)
				continue;
			float b2Aux = s2.dot(localRay.direction) * dir;
			if (b2Aux < 0 || b2Aux + b1Aux > 1)
				continue;
			if (tAux*d >= ray.tMin && tAux*d <= ray.tMax && tAux < t)
			{
				t = tAux;
				tIndex = triangleIndex;
				inter = true;
			}
		}
		if (inter)
		{
			// position = trans->localToWorldMatrix().transform((1 - b1 - b2) * data.vertices[data.triangles[tIndex].v[0]] + b1 * data.vertices[data.triangles[tIndex].v[1]] + b2 * data.vertices[data.triangles[tIndex].v[2]]);
			position = trans->localToWorldMatrix().transform(t * localRay.direction.versor() + localRay.origin);
			distance = (position - ray.origin).length();
			return true;
		}
	}
	return false;
}

} // end namespace cg
