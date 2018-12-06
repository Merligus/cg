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
// OVERVIEW: Image.h
// ========
// Class definition for generic image.
//
// Author: Paulo Pagliosa
// Last revision: 26/11/2018

#ifndef __Image_h
#define __Image_h

#include "core/SharedObject.h"
#include "graphics/Color.h"
#include <stdexcept>

namespace cg
{ // begin namespace cg

#define MIN_IMAGE_WIDTH 4

inline int
roundupImageWidth(int w)
{
  return (w + MIN_IMAGE_WIDTH - 1) & -MIN_IMAGE_WIDTH;
}

static inline
void image_index_out_of_range()
{
  throw std::logic_error("Image: index out of range");
}

//
// Forward definition
//
class Image;


/////////////////////////////////////////////////////////////////////
//
// Pixel: pixel class
// =====
struct Pixel
{
  using byte = uint8_t;

  byte r;
  byte g;
  byte b;

  __host__ __device__
  Pixel() = default;

  __host__ __device__
  Pixel(byte r, byte g, byte b)
  {
    set(r, g, b);
  }

  __host__ __device__
  Pixel(const Color& c)
  {
    set(c);
  }

  __host__ __device__
  void set(byte r, byte g, byte b)
  {
    this->r = r;
    this->g = g;
    this->b = b;
  }

  __host__ __device__
  void set(const Color& c)
  {
    r = (byte)(255 * c.r);
    g = (byte)(255 * c.g);
    b = (byte)(255 * c.b);
  }

  __host__ __device__
  Pixel& operator +=(const Pixel& p)
  {
    r += p.r;
    g += p.g;
    b += p.b;
    return *this;
  }

  __host__ __device__
  Pixel& operator +=(const Color& c)
  {
    r += (byte)(255 * c.r);
    g += (byte)(255 * c.g);
    b += (byte)(255 * c.b);
    return *this;
  }

}; // Pixel


/////////////////////////////////////////////////////////////////////
//
// ImageBuffer: generic image buffer class
// ===========
class ImageBuffer
{
public:
  // Default constructor.
  ImageBuffer() = default;

  // Constructor.
  ImageBuffer(int width, int height);

  ImageBuffer(const ImageBuffer&) = delete;
  ImageBuffer& operator =(const ImageBuffer&) = delete;

  // Move constructor and operator.
  ImageBuffer(ImageBuffer&& other);
  ImageBuffer& operator =(ImageBuffer&& other);

  // Destructor.
  ~ImageBuffer()
  {
    delete []_buffer;
  }

  auto width() const
  {
    return _W;
  }

  auto height() const
  {
    return _H;
  }

  const Pixel& operator ()(int x, int y) const
  {
#ifdef _DEBUG
    if (x < 0 || x >= _W || y < 0 || y >= _H)
      image_index_out_of_range();
#endif // _DEBUG
    return _buffer[y * _W + x];
  }

  Pixel& operator ()(int x, int y)
  {
#ifdef _DEBUG
    if (x < 0 || x >= _W || y < 0 || y >= _H)
      image_index_out_of_range();
#endif // _DEBUG
    return _buffer[y * _W + x];
  }

  auto length() const
  {
    return _W * _H;
  }

  const Pixel& operator [](int i) const
  {
#ifdef _DEBUG
    if (i < 0 || i >= _W * _H)
      image_index_out_of_range();
#endif // _DEBUG
    return _buffer[i];
  }

  Pixel& operator [](int i)
  {
#ifdef _DEBUG
    if (i < 0 || i >= _W * _H)
      image_index_out_of_range();
#endif // _DEBUG
    return _buffer[i];
  }

private:
  int _W{};
  int _H{};
  Pixel* _buffer{};

  friend Image;

}; // ImageBuffer


/////////////////////////////////////////////////////////////////////
//
// Image: generic image class
// =====
class Image: public SharedObject
{
public:
  auto width() const
  {
    return _W;
  }

  auto height() const
  {
    return _H;
  }

  void setData(int x, int y, const ImageBuffer& data);

  void setData(const ImageBuffer& data)
  {
    setData(0, 0, data);
  }

  ImageBuffer data(int x, int y, int w, int h) const;

  ImageBuffer data() const
  {
    return data(0, 0, _W, _H);
  }

  virtual void draw() const = 0;

protected:
  int _W;
  int _H;

  // Protected constructor.
  Image(int width, int height);

  virtual void setSubImage(int, int, int, int, const Pixel*) = 0;
  virtual void getSubImage(int, int, int, int, Pixel*) const = 0;

}; // Image

} // end namespace cg

#endif // __Image_h
