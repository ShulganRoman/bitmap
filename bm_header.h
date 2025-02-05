#ifndef BITMAP_Header_H
#define BITMAP_Header_H

#include "color.h"
#include "defs.h"
#include <CoreGraphics/CoreGraphics.h>
#include <cmath>
#include <sys/_endian.h>
#include <vector>

#define WIDTH 100
#define HEIGHT 100
#define COLORS_USED(x) (x <= 8 ? std::pow(2, x) : std::pow(2, 15))
#define OFFBITS (HEADER_SIZE + INFO_HEADER_SIZE)
#define XPPM                                                                   \
  (CGDisplayPixelsWide(CGMainDisplayID()) /                                    \
   CGDisplayScreenSize(CGMainDisplayID()).width * 1000)
#define YPPM                                                                   \
  (CGDisplayPixelsHigh(CGMainDisplayID()) /                                    \
   CGDisplayScreenSize(CGMainDisplayID()).height * 1000)

#pragma pack(push, 1)

struct Palette1 { // for bitCount = 1
  std::vector<DWORD> colors = {color::black, color::white};

  size_t getSize() { return colors.size() * sizeof(color); }
};

struct InfoHeader {
  DWORD size = sizeof(InfoHeader);
  LONG width = WIDTH;
  LONG height = HEIGHT;
  WORD planes = 1;
  WORD bitCount = 1;
  DWORD compression = 0;
  DWORD sizeImage = 8;
  LONG xPelsPerMeter = XPPM;
  LONG yPelsPerMeter = YPPM;
  DWORD clrUsed = COLORS_USED(bitCount);
  DWORD clrImportant = 0;
};

struct Header {
  WORD type = 0x4d42;
  DWORD size;
  WORD reserved1 = 0;
  WORD reserved2 = 0;
  DWORD offBits;
};

#pragma pack(pop)

#endif // BITMAP_Header_H
