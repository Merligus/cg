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
// OVERVIEW: Primitive.h
// ========
// Class definition for primitive.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 30/10/2018

#ifndef __Primitive_h
#define __Primitive_h

#include "Component.h"
#include "graphics/GLMesh.h"
#include "Material.h"
#include <vector>

namespace cg
{ // begin namespace cg


/////////////////////////////////////////////////////////////////////
//
// Primitive: primitive class
// =========
	class Primitive : public Component
	{
	public:
		struct Node
		{
			TriangleMesh::Data data;
			Bounds3f bound;
			int indiceFilhoEsquerdo;
		}; // Node

		Material material;

		Primitive(TriangleMesh* mesh, const std::string& meshName) :
			Component{ "Primitive" }
		{
			_limiar = 300;
			setMesh(mesh, meshName);
		}

		TriangleMesh* mesh() const
		{
			return _mesh;
		}

		auto bvhSize()
		{
			return bvh.size();
		}

		auto bvhAt(int i)
		{
			return bvh[i].bound;
		}

		const char* const meshName() const
		{
			return _meshName.c_str();
		}

		void setMesh(TriangleMesh* mesh, const std::string& meshName)
		{
			if (mesh != nullptr)
			{
				bvh.clear();
				Node N;
				auto data = mesh->data();
				N.bound = mesh->bounds();
				N.data.numberOfTriangles = data.numberOfTriangles;
				N.data.numberOfVertices = data.numberOfVertices;
				N.data.triangles = new TriangleMesh::Triangle[N.data.numberOfTriangles];
				N.data.vertices = new vec3f[N.data.numberOfVertices];
				for (int t = 0; t < N.data.numberOfTriangles; t++)
					N.data.triangles[t].setVertices(data.triangles[t].v[0], data.triangles[t].v[1], data.triangles[t].v[2]);
				for (int v = 0; v < N.data.numberOfVertices; v++)
					N.data.vertices[v] = data.vertices[v];
				N.indiceFilhoEsquerdo = -1;
				bvh.push_back(N);
				int i = 0;
				bool flag = true;
				while (i < bvh.size() && flag)
				{
					N = bvh[i];
					if (N.data.numberOfTriangles < _limiar)
					{
						i++;
						continue;
					}
					else
					{
						int k = 3;
						vec3f size = N.bound.size();
						float passoX = (float)size.x / k;
						float passoY = (float)size.y / k;
						float passoZ = (float)size.z / k;
						vec3f p1 = N.bound.min();
						vec3f p2 = N.bound.max();
						if (passoX > passoY && passoX > passoZ)
						{
							passoY = 0.0f;
							passoZ = 0.0f;
						}
						else if (passoY > passoX && passoY > passoZ)
						{
							passoX = 0.0f;
							passoZ = 0.0f;
						}
						else
						{
							passoX = 0.0f;
							passoY = 0.0f;
						}
						Node esquerdo[3 - 1], direito[3 - 1];
						int ind = -1;
						float custoMenor = 0.0, custo = 0.0;
						for (int l = 1; l < k; l++)
						{
							vec3f p12 = p1 + vec3f{ l*passoX, l*passoY, l*passoZ } - vec3f{0.05, 0.05, 0.05};
							vec3f p21 = p2 - vec3f{ (k - l) * passoX, (k - l) * passoY, (k - l) * passoZ } + vec3f{ 0.05, 0.05, 0.05 };

							esquerdo[l - 1].data.numberOfTriangles = 0;
							esquerdo[l - 1].bound.set(p1, p21);
							esquerdo[l - 1].data.triangles = new TriangleMesh::Triangle[N.data.numberOfTriangles];
							direito[l - 1].bound.set(p12, p2);
							direito[l - 1].data.numberOfTriangles = 0;
							direito[l - 1].data.triangles = new TriangleMesh::Triangle[N.data.numberOfTriangles];
							for (int t = 0; t < N.data.numberOfTriangles; t++)
							{
								vec3f centroide(0.0f, 0.0f, 0.0f);
								centroide = (data.vertices[N.data.triangles[t].v[0]] + data.vertices[N.data.triangles[t].v[1]] + data.vertices[N.data.triangles[t].v[2]]) * 0.3333333333;
								if (esquerdo[l - 1].bound.contains(centroide))
									esquerdo[l - 1].data.triangles[esquerdo[l - 1].data.numberOfTriangles++].setVertices(N.data.triangles[t].v[0], N.data.triangles[t].v[1], N.data.triangles[t].v[2]);
								else if (direito[l - 1].bound.contains(centroide))
									direito[l - 1].data.triangles[direito[l - 1].data.numberOfTriangles++].setVertices(N.data.triangles[t].v[0], N.data.triangles[t].v[1], N.data.triangles[t].v[2]);
							}
							custo = esquerdo[l - 1].bound.area() * esquerdo[l - 1].data.numberOfTriangles + direito[l - 1].bound.area() * direito[l - 1].data.numberOfTriangles;
							if (custo < custoMenor || ind == -1)
							{
								ind = l - 1;
								custoMenor = custo;
							}
						}
						if (custoMenor > N.bound.area() * N.data.numberOfTriangles)
						{
							flag = false;
						}
						else
						{
							bvh[i].indiceFilhoEsquerdo = bvh.size();
							esquerdo[ind].indiceFilhoEsquerdo = direito[ind].indiceFilhoEsquerdo = -1;
							bvh.push_back(esquerdo[ind]);
							bvh.push_back(direito[ind]);
							i++;
						}
					}
				}
			}
			_mesh = mesh;
			_meshName = meshName;
		}

		bool intersect(const Ray& ray, float& distance, vec3f& normal, vec3f& position) const;

	private:
		Reference<TriangleMesh> _mesh;
		std::vector<Node> bvh;
		std::string _meshName;
		int _limiar;

	}; // Primitive

} // end namespace cg

#endif // __Primitive_h
