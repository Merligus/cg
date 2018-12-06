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
// OVERVIEW: GLImage.cpp
// ========
// Source file for OpenGL image.
//
// Author: Paulo Pagliosa
// Last revision: 21/11/2018

#include "GLImage.h"
#include <memory>

namespace cg
{ // begin namespace cg

#define STRINGIFY(A) "#version 400\n"#A

static const char* vertexShader = STRINGIFY(
  uniform vec4 vp[] = vec4[4](
    vec4(-1.0f, -1.0f, 0, 1),
    vec4(+1.0f, -1.0f, 0, 1),
    vec4(+1.0f, +1.0f, 0, 1),
    vec4(-1.0f, +1.0f, 0, 1));
    uniform vec2 tc[] = vec2[4](
    vec2(0.0f, 0.0f),
    vec2(1.0f, 0.0f),
    vec2(1.0f, 1.0f),
    vec2(0.0f, 1.0f));
  out vec2 vs_tc;
  void main()
  {
    gl_Position = vp[gl_VertexID];
    vs_tc = tc[gl_VertexID];
  }
);

static const char* fragmentShader = STRINGIFY(
  uniform sampler2D tex;
  in vec2 vs_tc;
  out vec4 fs_color;
  void main()
  {
    fs_color = texture(tex, vs_tc);
  }
);

//
// Auxiliary functions
//
inline GLuint
createTexture(int w, int h)
{
  GLuint id;

  // Create texture
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
  // Initialize texture
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, w, h);
  // Set texture sampler parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  return id;
}

inline void
setTextureData(int x, int y, int w, int h, const Pixel* data)
{
  glTexSubImage2D(GL_TEXTURE_2D,
    0,
    x,
    y,
    w,
    h,
    GL_RGB,
    GL_UNSIGNED_BYTE,
    data);
}

inline void
getTextureData(int x, int y, int w, int h, Pixel* data)
{
  // TODO
}


/////////////////////////////////////////////////////////////////////
//
// GLImage implementation
// =======
class GLImage::Drawer: public GLSL::Program
{
public:
  // Constructor
  Drawer():
    GLSL::Program{"Image Drawer"}
  {
    setShaders(vertexShader, fragmentShader).use();
    glGenVertexArrays(1, &_vao);
  }

  ~Drawer()
  {
    glDeleteVertexArrays(1, &_vao);
  }

  // Render image
  void draw(const GLImage& image)
  {
    auto dt = glIsEnabled(GL_DEPTH_TEST);
    auto cp = current();
    GLint cv[4];

    this->use();
    glGetIntegerv(GL_VIEWPORT, cv);
    glViewport(0, 0, image.width(), image.height());
    glBindVertexArray(_vao);
    image.bind();
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glViewport(cv[0], cv[1], cv[2], cv[3]);
    if (dt)
      glEnable(GL_DEPTH_TEST);
    setCurrent(cp);
  }

private:
  GLuint _vao;

}; // Drawer

GLImage::GLImage(int w, int h):
  Image{w, h},
  _handle{createTexture(w, h)}
{
  // do nothing
}

GLImage::~GLImage()
{
  glDeleteTextures(1, &_handle);
}

inline void
GLImage::bind() const
{
  glBindTexture(GL_TEXTURE_2D, _handle);
}

void
GLImage::setSubImage(int x, int y, int w, int h, const Pixel* data)
{
  setTextureData(x, y, w, h, data);
}

void
GLImage::getSubImage(int x, int y, int w, int h, Pixel* data) const
{
  getTextureData(x, y, w, h, data);
}

GLImage::Drawer*
GLImage::drawer()
{
  static Drawer* instance;

  if (instance == nullptr)
    instance = new Drawer();
  return instance;
}

void
GLImage::draw() const
{
  drawer()->draw(*this);
}

} // end namespace cg
