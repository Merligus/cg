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
// OVERVIEW: RayTracer.cpp
// ========
// Source file for simple ray tracer.
//
// Author(s): Paulo Pagliosa (and your name)
// Last revision: 21/11/2018

#include "Camera.h"
#include "RayTracer.h"
#include <time.h>
#include <stack>

using namespace std;

namespace cg
{ // begin namespace cg

void
printElapsedTime(const char* s, clock_t time)
{
  printf("%sElapsed time: %.4f s\n", s, (float)time / CLOCKS_PER_SEC);
}


/////////////////////////////////////////////////////////////////////
//
// RayTracer implementation
// =========
RayTracer::RayTracer(Scene& scene, Camera* camera):
  Renderer{scene, camera},
  _maxRecursionLevel{6},
  _minWeight{MIN_WEIGHT}
{
  // TODO: BVH
}

void
RayTracer::render()
{
  throw std::runtime_error("RayTracer::render() invoked");
}

inline float
windowHeight(Camera* c)
{
  if (c->projectionType() == Camera::Parallel)
    return c->height();
  return c->nearPlane() * tan(math::toRadians(c->viewAngle() * 0.5f)) * 2;

}
void
RayTracer::renderImage(Image& image)
{
  auto t = clock();
  const auto& m = _camera->cameraToWorldMatrix();

  // VRC axes
  _vrc.u = m[0];
  _vrc.v = m[1];
  _vrc.n = m[2];
  // init auxiliary mapping variables
  _W = image.width();
  _H = image.height();
  _Iw = math::inverse(float(_W));
  _Ih = math::inverse(float(_H));

  auto height = windowHeight(_camera);

  _W >= _H ? _Vw = (_Vh = height) * _W * _Ih : _Vh = (_Vw = height) * _H * _Iw;
  // init pixel ray
  _pixelRay.origin = _camera->position();
  _pixelRay.direction = -_vrc.n;
  _camera->clippingPlanes(_pixelRay.tMin, _pixelRay.tMax);
  _numberOfRays = _numberOfHits = 0;
  scan(image);
  printf("\nNumber of rays: %llu", _numberOfRays);
  printf("\nNumber of hits: %llu", _numberOfHits);
  printElapsedTime("\nDONE! ", clock() - t);
}

void
RayTracer::setPixelRay(float x, float y)
//[]---------------------------------------------------[]
//|  Set pixel ray                                      |
//|  @param x coordinate of the pixel                   |
//|  @param y cordinates of the pixel                   |
//[]---------------------------------------------------[]
{
  auto p = imageToWindow(x, y);

  switch (_camera->projectionType())
  {
    case Camera::Perspective:
      _pixelRay.direction = (p - _camera->nearPlane() * _vrc.n).versor();
      break;

    case Camera::Parallel:
      _pixelRay.origin = _camera->position() + p;
      break;
  }
}

void
RayTracer::scan(Image& image)
{
  ImageBuffer scanLine{_W, 1};

  for (int j = 0; j < _H; j++)
  {
    auto y = (float)j + 0.5f;

    printf("Scanning line %d of %d\r", j + 1, _H);
    for (int i = 0; i < _W; i++)
      scanLine[i] = shoot((float)i + 0.5f, y);
    image.setData(0, j, scanLine);
  }
}

Color
RayTracer::shoot(float x, float y)
//[]---------------------------------------------------[]
//|  Shoot a pixel ray                                  |
//|  @param x coordinate of the pixel                   |
//|  @param y cordinates of the pixel                   |
//|  @return RGB color of the pixel                     |
//[]---------------------------------------------------[]
{
  // set pixel ray
  setPixelRay(x, y);

  // trace pixel ray
  Color color = trace(_pixelRay, 0, 1.0f);

  // adjust RGB color
  if (color.r > 1.0f)
    color.r = 1.0f;
  if (color.g > 1.0f)
    color.g = 1.0f;
  if (color.b > 1.0f)
    color.b = 1.0f;
  // return pixel color
  return color;
}

Color
RayTracer::trace(const Ray& ray, uint32_t level, float weight)
//[]---------------------------------------------------[]
//|  Trace a ray                                        |
//|  @param the ray                                     |
//|  @param recursion level                             |
//|  @param ray weight                                  |
//|  @return color of the ray                           |
//[]---------------------------------------------------[]
{
  if (level > _maxRecursionLevel)
    return Color::black;
  _numberOfRays++;

  Intersection hit;

  return intersect(ray, hit) ? shade(ray, hit, level, weight) : background();
}

inline constexpr auto
rt_eps()
{
  return 1e-4f;
}

bool
RayTracer::intersect(const Ray& ray, Intersection& hit)
//[]---------------------------------------------------[]
//|  Ray/object intersection                            |
//|  @param the ray (input)                             |
//|  @param information on intersection (output)        |
//|  @return true if the ray intersects an object       |
//[]---------------------------------------------------[]
{
	hit.object = nullptr;
	hit.distance = ray.tMax;
	// TODO: insert your code here
	float distance = ray.tMax;
	int triangleIndex = -1;
	vec3f position;
	auto scene = (Scene*)_scene;
	std::stack<std::list<cg::SceneObject>::iterator> pilhaDeObjetos;
	for (std::list<cg::SceneObject>::iterator object = scene->containerBegin(); object != scene->containerEnd(); ++object)
		pilhaDeObjetos.push(object);
	while (!pilhaDeObjetos.empty())
	{
		auto object = pilhaDeObjetos.top();
		if (object->primitive() != nullptr)
		{
			bool inter = object->primitive()->intersect(ray, distance, triangleIndex, position);
			if (hit.distance > distance && inter)
			{
				hit.p = position;
				hit.triangleIndex = triangleIndex;
				hit.distance = distance;
				hit.object = object->primitive();
			}
		}
		pilhaDeObjetos.pop();
		for (std::list<cg::SceneObject>::iterator filho = object->childrenBegin(); filho != object->childrenEnd(); ++filho)
			pilhaDeObjetos.push(filho);
	}
	return hit.object != nullptr;
}

Color
RayTracer::shade(const Ray& ray, Intersection& hit, int level, float weight)
//[]---------------------------------------------------[]
//|  Shade a point P                                    |
//|  @param the ray (input)                             |
//|  @param information on intersection (input)         |
//|  @param recursion level                             |
//|  @param ray weight                                  |
//|  @return color at point P                           |
//[]---------------------------------------------------[]
{
	// TODO: insert your code here
	_numberOfHits++;
	auto scene = (Scene*)_scene;
	Color c = hit.object->material.ambient * scene->ambientLight;
	if (hit.object->material.specular != Color::black)
	{
		vec3f Or{ hit.object->material.specular.r, hit.object->material.specular.g, hit.object->material.specular.b };
		weight *= Or.max();
		if (weight > _minWeight)
		{
			auto data = hit.object->mesh()->data();
			vec3f N = data.vertexNormals[hit.triangleIndex].versor();
			Ray reflectRay{ hit.p - rt_eps() * ray.direction, ray.direction.versor() - 2 * N.dot(ray.direction.versor()) * N };
			c += hit.object->material.specular * trace(reflectRay, level + 1, weight);
		}
	}

	float distance = ray.tMax;

	std::stack<std::list<cg::SceneObject>::iterator> pilhaDeObjetos;
	for (std::list<cg::SceneObject>::iterator object = scene->containerBegin(); object != scene->containerEnd(); ++object)
		pilhaDeObjetos.push(object);
	while (!pilhaDeObjetos.empty())
	{
		auto object = pilhaDeObjetos.top();
		if (object->light() != nullptr)
		{
			// vec3f O = ray.origin + (hit.distance - rt_eps()) * ray.direction.versor();
			vec3f O = hit.p - rt_eps() * ray.direction;
			vec3f direcao = object->transform()->position() - O;
			Ray lightRay{ O, direcao, 0, (direcao).length() - rt_eps() };
			if (!shadow(lightRay))
			{
				auto t = object->light()->transform();
				auto data = hit.object->mesh()->data();
				vec3f N = mat3f{ t->worldToLocalMatrix() }.transposed() * data.vertexNormals[hit.triangleIndex];
				N = N.versor();
				vec3f V = camera()->position() - hit.p;
				if (object->light()->type() == Light::Type::Directional)
				{
					vec3f direction = t->rotation() * vec3f { 0, -1.0f, 0 };
					Color color = object->light()->color();
					
					vec3f L = (-direction).versor();
					vec3f R = -L - 2 * N.dot(-L) * N;

					Color A = color * hit.object->material.ambient;
					Color D = hit.object->material.diffuse * color * std::max(N.dot(L), 0.0f);
					Color S = hit.object->material.spot * color * pow(std::max(R.dot(V), 0.0f), hit.object->material.shine);

					c += (A + D + S);
				}
				else if (object->light()->type() == Light::Type::Point)
				{
					Color color = object->light()->color();
					int falloff = object->light()->falloff();
					vec3f position = object->light()->transform()->position();

					vec3f L = (position - hit.p).versor();
					vec3f R = -L - 2 * N.dot(-L) * N;

					Color A = color * hit.object->material.ambient;
					Color D = hit.object->material.diffuse * color * std::max(N.dot(L), 0.0f);
					Color S = hit.object->material.spot * color * pow(std::max(R.dot(V), 0.0f), hit.object->material.shine);

					c = c + (A + D + S);
					/*float dl = (position - hit.p).length();
					c.r = c.r + (A.r + D.r + S.r) / pow(dl, falloff);
					c.g = c.g + (A.g + D.g + S.g) / pow(dl, falloff);
					c.b = c.b + (A.b + D.b + S.b) / pow(dl, falloff);
					c.a = c.a + (A.a + D.a + S.a) / pow(dl, falloff);*/
				}
				else if (object->light()->type() == Light::Type::Spot)
				{
					Color color = object->light()->color();
					int falloff = object->light()->falloff();
					vec3f position = object->light()->transform()->position();
					vec3f direction = t->rotation() * vec3f { 0, -1.0f, 0 };
					float innerCutOff = cos(0.01745329251*object->light()->innerCutOff());
					float outerCutOff = cos(0.01745329251*object->light()->outerCutOff());

					vec3f L = (position - hit.p).versor();
					float theta = L.dot((-direction).versor());
					float epsilon = innerCutOff - outerCutOff;
					float intensity = (theta - outerCutOff) / epsilon;
					if (intensity < 0.0f)
						intensity = 0.0f;
					else if (intensity > 1.0f)
						intensity = 1.0f;
					vec3f R = -L - 2 * N.dot(-L) * N;

					Color A = color * hit.object->material.ambient;
					Color D = hit.object->material.diffuse * color * std::max(N.dot(L), 0.0f);
					Color S = hit.object->material.spot * color * pow(std::max(R.dot(V), 0.0f), hit.object->material.shine);

					float dl = (position - hit.p).length();

					c.r = c.r + intensity * (A.r + D.r + S.r) / pow(dl, falloff);
					c.g = c.g + intensity * (A.g + D.g + S.g) / pow(dl, falloff);
					c.b = c.b + intensity * (A.b + D.b + S.b) / pow(dl, falloff);
					c.a = c.a + intensity * (A.a + D.a + S.a) / pow(dl, falloff);
				}
			}
		}
		pilhaDeObjetos.pop();
		for (std::list<cg::SceneObject>::iterator filho = object->childrenBegin(); filho != object->childrenEnd(); ++filho)
			pilhaDeObjetos.push(filho);		
	}
	return c;
}

Color
RayTracer::background() const
//[]---------------------------------------------------[]
//|  Background                                         |
//|  @return background color                           |
//[]---------------------------------------------------[]
{
  return _scene->backgroundColor;
}

bool
RayTracer::shadow(const Ray& ray)
//[]---------------------------------------------------[]
//|  Verifiy if ray is a shadow ray                     |
//|  @param the ray (input)                             |
//|  @return true if the ray intersects an object       |
//[]---------------------------------------------------[]
{
  Intersection hit;
  return intersect(ray, hit);
}

} // end namespace cg
