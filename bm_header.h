#ifndef BITMAP_Header_H
#define BITMAP_Header_H

#include "defs.h"
#include <CoreGraphics/CoreGraphics.h>
#include <cmath>
#include <sys/_endian.h>

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

struct Palette32 {
  DWORD redmask = 0x0f00;
  DWORD greenmask = 0x00f0;
  DWORD bluemask = 0x000f;
  DWORD alphamask = 0xf000;
};

#pragma pack(push, 1)
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
#pragma pack(pop)

#pragma pack(push, 1)
struct Header {
  WORD type = 0x4d42;
  DWORD size = (WIDTH * HEIGHT) / 8 + sizeof(Header) + sizeof(InfoHeader) + 8;
  WORD reserved1 = 0;
  WORD reserved2 = 0;
  DWORD offBits = sizeof(Header) + sizeof(InfoHeader) + 8;
};
#pragma pack(pop)

#endif // BITMAP_Header_H
