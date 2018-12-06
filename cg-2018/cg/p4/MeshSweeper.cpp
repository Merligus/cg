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
// OVERVIEW: MeshSweeper.h
// ========
// Source file for mesh sweeper.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 15/09/2018

#include "MeshSweeper.h"

namespace cg
{ // begin namespace cg

namespace internal
{ // begin namespace internal

inline void
setBoxVertices(vec3f* v)
{
  const vec3f p1{-1, -1, -1};
  const vec3f p2{+1, -1, -1};
  const vec3f p3{+1, +1, -1};
  const vec3f p4{-1, +1, -1};
  const vec3f p5{-1, -1, +1};
  const vec3f p6{+1, -1, +1};
  const vec3f p7{+1, +1, +1};
  const vec3f p8{-1, +1, +1};

  v[ 0] = p1; v[ 1] = p5; v[ 2] = p8; v[ 3] = p4; // x = -1
  v[ 4] = p2; v[ 5] = p3; v[ 6] = p7; v[ 7] = p6; // x = +1
  v[ 8] = p1; v[ 9] = p2; v[10] = p6; v[11] = p5; // y = -1
  v[12] = p4; v[13] = p8; v[14] = p7; v[15] = p3; // y = +1
  v[16] = p1; v[17] = p4; v[18] = p3; v[19] = p2; // z = -1
  v[20] = p5; v[21] = p6; v[22] = p7; v[23] = p8; // z = +1
}

inline void
setBoxVertexNormals(vec3f* n)
{
  const vec3f n1{-1, 0, 0};
  const vec3f n2{+1, 0, 0};
  const vec3f n3{0, -1, 0};
  const vec3f n4{0, +1, 0};
  const vec3f n5{0, 0, -1};
  const vec3f n6{0, 0, +1};

  n[ 0] = n[ 1] = n[ 2] = n[ 3] = n1; // x = -1
  n[ 4] = n[ 5] = n[ 6] = n[ 7] = n2; // x = +1
  n[ 8] = n[ 9] = n[10] = n[11] = n3; // y = -1
  n[12] = n[13] = n[14] = n[15] = n4; // y = +1
  n[16] = n[17] = n[18] = n[19] = n5; // z = -1
  n[20] = n[21] = n[22] = n[23] = n6; // z = +1
}

inline void
setBoxTriangles(TriangleMesh::Triangle* t)
{
  t[ 0].setVertices( 0,  1,  2); t[ 1].setVertices( 2,  3,  0);
  t[ 2].setVertices( 4,  5,  7); t[ 3].setVertices( 5,  6,  7);
  t[ 4].setVertices( 8,  9, 11); t[ 5].setVertices( 9, 10, 11);
  t[ 6].setVertices(12, 13, 14); t[ 7].setVertices(14, 15, 12);
  t[ 8].setVertices(16, 17, 19); t[ 9].setVertices(17, 18, 19);
  t[10].setVertices(20, 21, 22); t[11].setVertices(22, 23, 20);
}

} // end namespace internal


//////////////////////////////////////////////////////////
//
// MeshSweeper implementation
// ===========
TriangleMesh*
MeshSweeper::makeBox()
{
  const int nv = 24;
  const int nt = 12;
  TriangleMesh::Data data;

  data.numberOfVertices = nv;
  data.vertices = new vec3f[nv];
  internal::setBoxVertices(data.vertices);
  data.vertexNormals = new vec3f[nv];
  internal::setBoxVertexNormals(data.vertexNormals);
  data.numberOfTriangles = nt;
  data.triangles = new TriangleMesh::Triangle[nt];
  internal::setBoxTriangles(data.triangles);
  return new TriangleMesh{data};
}

} // end namespace cg
