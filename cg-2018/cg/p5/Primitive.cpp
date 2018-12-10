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
#include <stack>
#include "Scene.h"

namespace cg
{ // begin namespace cg


	bool
	Primitive::intersect(const Ray& ray, float& distance, vec3f& normal, vec3f& position) const
	{
		if (_mesh == nullptr)
			return false;

		auto trans = const_cast<Primitive*>(this)->transform();
		Ray localRay{ ray.origin, ray.direction }; // trans->worldToLocalMatrix()
		localRay.origin = trans->worldToLocalMatrix().transform(ray.origin);
		localRay.direction = trans->worldToLocalMatrix().transformVector(ray.direction);
		auto d = math::inverse(localRay.direction.length());
		float tMin;
		float tMax;

		localRay.direction *= d;
		distance = math::Limits<float>::inf();
		std::stack<Node> pilha;
		if (bvh[0].bound.intersect(localRay, tMin, tMax))
		{
			pilha.push(bvh[0]);
			Scene s = *(trans->sceneObject())->scene();
			if (!s.bvhMode)
			{
				Node N = pilha.top();
				pilha.pop();
				auto data = _mesh->data();
				float t = math::Limits<float>::inf(), b1 = 0, b2 = 0;
				vec3f n;
				bool inter = false;
				for (int triangleIndex = 0; triangleIndex < N.data.numberOfTriangles; triangleIndex++)
				{
					vec3f p0, p1, p2;
					p0 = data.vertices[N.data.triangles[triangleIndex].v[0]];
					p1 = data.vertices[N.data.triangles[triangleIndex].v[1]];
					p2 = data.vertices[N.data.triangles[triangleIndex].v[2]];
					vec3f e1 = p1 - p0;
					vec3f e2 = p2 - p0;
					vec3f s1 = localRay.direction.cross(e2);
					float dir = s1.dot(e1);
					if (dir == 0)
						continue;
					dir = 1 / dir;
					vec3f s = localRay.origin - data.vertices[N.data.triangles[triangleIndex].v[0]];
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
						n = data.vertexNormals[N.data.triangles[triangleIndex].v[0]];
						/*n += data.vertexNormals[N.data.triangles[triangleIndex].v[1]];
						n += data.vertexNormals[N.data.triangles[triangleIndex].v[2]];
						n *= 0.333333333333;*/
						inter = true;
					}
				}
				t = t * d;
				if (inter && t < distance)
				{
					normal = n;
					position = t * ray.direction.versor() + ray.origin;
					distance = t;
				}
				if (distance < math::Limits<float>::inf())
					return true;
				else
					return false;
			}
		}
		while (!pilha.empty())
		{
			Node N = pilha.top();
			pilha.pop();
			if (N.indiceFilhoEsquerdo == -1)
			{
				// Passo 5
				auto data = _mesh->data();
				float t = math::Limits<float>::inf(), b1 = 0, b2 = 0;
				vec3f n;
				bool inter = false;
				for (int triangleIndex = 0; triangleIndex < N.data.numberOfTriangles; triangleIndex++)
				{
					vec3f p0, p1, p2;
					p0 = data.vertices[N.data.triangles[triangleIndex].v[0]];
					p1 = data.vertices[N.data.triangles[triangleIndex].v[1]];
					p2 = data.vertices[N.data.triangles[triangleIndex].v[2]];
					vec3f e1 = p1 - p0;
					vec3f e2 = p2 - p0;
					vec3f s1 = localRay.direction.cross(e2);
					float dir = s1.dot(e1);
					if (dir == 0)
						continue;
					dir = 1 / dir;
					vec3f s = localRay.origin - data.vertices[N.data.triangles[triangleIndex].v[0]];
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
						n = data.vertexNormals[N.data.triangles[triangleIndex].v[0]];
						/*n += data.vertexNormals[N.data.triangles[triangleIndex].v[1]];
						n += data.vertexNormals[N.data.triangles[triangleIndex].v[2]];
						n *= 0.333333333333;*/
						inter = true;
					}
				}
				t = t * d;
				if (inter && t < distance)
				{
					normal = n;
					position = t * ray.direction.versor() + ray.origin;
					distance = t;
				}
			}
			else
			{
				if (bvh[N.indiceFilhoEsquerdo].bound.intersect(localRay, tMin, tMax))
					pilha.push(bvh[N.indiceFilhoEsquerdo]);
				if (bvh[N.indiceFilhoEsquerdo + 1].bound.intersect(localRay, tMin, tMax))
					pilha.push(bvh[N.indiceFilhoEsquerdo + 1]);
			}
		}
		if (distance < math::Limits<float>::inf())
			return true;
		else
			return false;
	}

} // end namespace cg
