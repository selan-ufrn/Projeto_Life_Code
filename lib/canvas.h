#ifndef CANVAS_H
#define CANVAS_H

#include <vector>
using std::vector;
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <cstdint>

#include "common.h"

namespace life {

//! Provides methods for drawing on an image.
/*!
 * This is a drawing area on which we shall draw a Life representation.
 *
 * Here some information on the canvas properties:
 *
 * 1. The drawing area has the origin set to the top left corner, with
 * positive `X` axis going to the right and positive `Y` axis going to
 * the bottom of the canvas.
 * 2. A pixel on the canvas has a square shape based on the pixel size
 * set when the canvas is instantiated.
 * 3. Because of 2, we have the `virtual` dimension (set by the client),
 * and the `real` dimension, which is w * pixel_size by h * pixel_size.
 *
 * This class returns to the client an image (object) representation
 * of the canvas, which might be stored by the client as a PPM or
 * PNG image file.
 */
class Canvas {
 public:
  //== Alias
  typedef uint8_t component_t;     //!< Type of a color channel.
  typedef unsigned long coord_t; //!< The pixel coordinate type.
  //== Constants
  static constexpr uint8_t image_depth = 4;  //!< Default value is RGBA (4 channels).

 public:
  //=== Special members
  /// Constructor
  /*! Creates an empty canvas with the requested dimensions.
   * @param w The canvas width in virtual pixels.
   * @param h The canvas height in virtual pixels.
   * @param bs The canvas block size in real pixels.
   */
  Canvas(size_t w = 0, size_t h = 0, short bs = 4);
  /// Destructor.
  ~Canvas(void);
  /// Copy constructor.
  Canvas(const Canvas&);
  /// Assignment operator.
  Canvas& operator=(const Canvas&);

  //=== Members
  /// Clear the canvas with black color.
  void clear(const Color& = BLACK);
  /// Set the color of a pixel on the canvas.
  void pixel(coord_t, coord_t, const Color&);
  /// Get the pixel color from the canvas.
  Color pixel(coord_t, coord_t) const;

  //=== Attribute accessors members.
  /// Get the canvas width.
  size_t width(void) const
  { return m_width; }
  /// Get the canvas height.
  size_t height(void) const
  { return m_height; }
  /// Get the canvas pixels, as an array of `unsigned char`.
  const component_t* pixels(void) const
  { return m_pixels.data(); }

 private:
  size_t m_width;                //!< The image width in pixel units.
  size_t m_height;               //!< The image height in pixel units.
  short m_block_size;            //!< Cell size in pixels
  vector<component_t> m_pixels;  //!< The pixels, stored as 3 RGB components.
};
}  // namespace life

#endif  // CANVAS_H
