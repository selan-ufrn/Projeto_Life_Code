/*!
 * Canvas class implementation.
 * @file canvas.cpp
 */

#include "canvas.h"

namespace life {

  // TODO: Adicione o resto da implementação dos métodos aqui.

  Canvas::Canvas(size_t w, size_t h, short bs){}
    /// Destructor.
  Canvas::~Canvas(void){}
    /// Copy constructor.
  Canvas::Canvas(const Canvas&){}
  /// Assignment operator.
  Canvas& Canvas::operator=(const Canvas&){}

  //=== Members
  /// Clear the canvas with black color.
  void Canvas::clear(const Color&){}
  /// Set the color of a pixel on the canvas.
  void Canvas::pixel(coord_t, coord_t, const Color&){}
  /// Get the pixel color from the canvas.
  Color Canvas::pixel(coord_t, coord_t) const{return BLACK;}

}  // namespace life

//================================[ canvas.cpp ]================================//
