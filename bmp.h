#ifndef BITMAP_BMP_H
#define BITMAP_BMP_H

#include "bm_exception.h"
#include "bm_header.h"
#include "color.h"
#include "defs.h"
#include <MacTypes.h>
#include <fstream>
#include <iostream>
#include <vector>

class bmp3 {
  std::string fileName;

  Header header;
  InfoHeader infoHeader;
  Palette1 palette;

  std::vector<std::vector<BYTE>> image;

public:
  explicit bmp3(const std::string &path) : fileName(path) {
    std::ifstream file(path, std::ios::binary);

    actualWidthInBytes = 16;

    image = std::vector<std::vector<BYTE>>(
        infoHeader.height, std::vector<BYTE>(actualWidthInBytes));

    if (!file)
      return;

    file.read(reinterpret_cast<char *>(&header), sizeof(header));
    file.read(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));
    file.close();

    try {
      isValid();
    } catch (const std::exception &e) {
      std::cerr << e.what();
    }

    actualWidthInBytes = infoHeader.width % 32 ? (infoHeader.width / 32 + 1) * 4
                                               : infoHeader.width / 8;

    readImage();
  }

  void setImage(std::vector<std::vector<BYTE>> &img, DWORD X = 0, DWORD Y = 0) {
    std::reverse(image.begin(), image.end());

    for (int i = 0; i < img.size() && i + Y < image.size(); i++)
      bit_insert(image[i + Y], X, img[i]);

    std::reverse(image.begin(), image.end());
  }

  void create() {
    std::ofstream file(fileName, std::ios::binary);
    if (!file)
      return;

    header.offBits = sizeof(header) + infoHeader.size + palette.getSize();
    infoHeader.sizeImage = infoHeader.height * actualWidthInBytes;
    infoHeader.sizeImage = infoHeader.height * actualWidthInBytes;
    header.size = actualWidthInBytes * infoHeader.height + sizeof(header) +
                  infoHeader.size + palette.getSize();

    file.write(reinterpret_cast<char *>(&header), sizeof(header));
    file.write(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));
    file.write(reinterpret_cast<char *>(palette.colors.data()),
               palette.getSize());
    for (const std::vector<BYTE> &row : image)
      file.write(reinterpret_cast<const char *>(row.data()), row.size());

    file.close();
  }

  void resize(size_t width, size_t height) {
    infoHeader.width = width;
    infoHeader.height = height;

    actualWidthInBytes = infoHeader.width % 32 ? (infoHeader.width / 32 + 1) * 4
                                               : infoHeader.width / 8;

    image = std::vector<std::vector<BYTE>>(
        infoHeader.height, std::vector<BYTE>(actualWidthInBytes));
  }

  void setColors(std::vector<color> colors) {
    for (int i = 0; i < palette.colors.size(); i++)
      palette.colors[i] = colors[i];
  }

private:
  void readImage() {
    std::ifstream file(fileName, std::ios::binary);

    if (!file)
      return;

    file.seekg(header.offBits, std::ios::beg);

    for (auto &row : image)
      file.read(reinterpret_cast<char *>(row.data()), row.size());
  }

  void isValid() {
    if (header.type != 0x4d42)
      throw WrongTypeOfFileException("type of file is not bmp");
    if (infoHeader.size != 40)
      throw WrongTypeOfFileException("bmp version is wrong, should be v3");
  }

  void bit_insert(std::vector<BYTE> &dest, size_t dest_start_bit,
                  const std::vector<BYTE> &src) {
    const size_t CHAR_BITS = CHAR_BIT;
    size_t total_bits = dest.size() * CHAR_BITS;

    if (dest_start_bit >= total_bits)
      return;

    for (size_t src_bit = 0; src_bit < src.size() * CHAR_BITS; ++src_bit) {
      size_t dest_bit = dest_start_bit + src_bit;

      if (dest_bit >= total_bits)
        break;

      size_t src_byte = src_bit / CHAR_BITS;
      size_t src_bit_in_byte = src_bit % CHAR_BITS;
      uint8_t src_mask = 1 << (CHAR_BITS - 1 - src_bit_in_byte);

      size_t dest_byte = dest_bit / CHAR_BITS;
      size_t dest_bit_in_byte = dest_bit % CHAR_BITS;
      uint8_t dest_mask = 1 << (CHAR_BITS - 1 - dest_bit_in_byte);

      bool bit_value = (src[src_byte] & src_mask) != 0;

      if (bit_value) {
        dest[dest_byte] |= dest_mask;
      } else {
        dest[dest_byte] &= ~dest_mask;
      }
    }
  }

private:
  LONG actualWidthInBytes;
};

#endif // BITMAP_BMP_H
