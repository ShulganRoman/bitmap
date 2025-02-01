#ifndef BITMAP_COLOR_H
#define BITMAP_COLOR_H

#include <stdio.h>

class color {
  color(int red, int green, int blue, int alpha)
      : R(red), G(green), B(blue), A(alpha) {}

public:
  unsigned char R, G, B, A;
  color() : R(255), G(255), B(255), A(255) {}
  void operator=(color &other);
  color operator+(const color &other);
  void _pixel_info() const;

public:
  color visible(int alpha) { return color(R, G, B, alpha); }

  color red(int alpha = 255) { return color{255, 0, 0, alpha}; }

  color green(int alpha = 255) { return color{0, 255, 0, alpha}; }

  color blue(int alpha = 255) { return color{0, 0, 255, alpha}; }

  color black(int alpha = 255) { return color{0, 0, 0, alpha}; }

  color white(int alpha = 255) { return color{255, 255, 255, alpha}; }

  color grey(int greyScale = 128, int alpha = 255) {
    return color{greyScale, greyScale, greyScale, alpha};
  }
};

inline void color::operator=(color &other) {
  R = other.R;
  G = other.G;
  B = other.B;
  A = other.A;
}

inline color color::operator+(const color &other) {
  if (R + other.R >= 255)
    R = 255;
  else
    R += other.R;
  if (G + other.G >= 255)
    G = 255;
  else
    G += other.G;
  if (B + other.B >= 255)
    B = 255;
  else
    B += other.B;
  return color{R, G, B, 255};
}
inline void color::_pixel_info() const { printf("%d %d %d %d", R, G, B, A); }

#endif // BITMAP_COLOR_H
