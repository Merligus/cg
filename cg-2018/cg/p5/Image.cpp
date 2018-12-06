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
// OVERVIEW: Image.cpp
// ========
// Source file for generic image.
//
// Author: Paulo Pagliosa
// Last revision: 26/11/2018

#include "Image.h"

namespace cg
{ // begin namespace cg

static inline void
image_bad_size()
{
  throw std::logic_error("Image: bad size");
}


/////////////////////////////////////////////////////////////////////
//
// ImageBuffer implementation
// ===========
ImageBuffer::ImageBuffer(int w, int h)
{
#ifdef _DEBUG
  if (w < 1 || h < 1)
    image_bad_size();
#endif // _DEBUG
  _W = w;
  _H = h;
  _buffer = new Pixel[w * h];
}

ImageBuffer::ImageBuffer(ImageBuffer&& other):
  _W{other._W},
  _H{other._H},
  _buffer{other._buffer}
{
  other._W = other._H = 0;
  other._buffer = nullptr;
}

ImageBuffer&
ImageBuffer::operator =(ImageBuffer&& other)
{
  delete[]_buffer;
  _W = other._W;
  _H = other._H;
  _buffer = other._buffer;
  other._W = other._H = 0;
  other._buffer = nullptr;
  return *this;
}


/////////////////////////////////////////////////////////////////////
//
// Image implementation
// =====
Image::Image(int w, int h):
  _W{w},
  _H{h}
{
#ifdef _DEBUG
  if (w < 1 || h < 1)
    image_bad_size();
#endif // _DEBUG
}

void
Image::setData(int x, int y, const ImageBuffer& data)
{
#ifdef _DEBUG
  if (x < 0 || x >= _W || y < 0 || y >= _H)
    image_index_out_of_range();
#endif // _DEBUG

  auto w = std::min(_W, data._W);
  auto h = std::min(_H, data._H);

  if (x + w > _W)
    w = _W - x;
  if (y + h > _H)
    h = _W - y;
  setSubImage(x, y, w, h, data._buffer);
}

ImageBuffer
Image::data(int x, int y, int w, int h) const
{
#ifdef _DEBUG
  if (x < 0 || x >= _W || y < 0 || y >= _H)
    image_index_out_of_range();
#endif // _DEBUG
  w = std::min(_W, w);
  h = std::min(_H, h);
  if (x + w > _W)
    w = _W - x;
  if (y + h > _H)
    h = _W - y;

  ImageBuffer data{w, h};

  getSubImage(x, y, w, h, data._buffer);
  return data;
}

} // end namespace cg
