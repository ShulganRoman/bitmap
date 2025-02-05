#ifndef BITMAP_BMP_H
#define BITMAP_BMP_H

#include "bm_exception.h"
#include "bm_header.h"
#include "defs.h"
#include <MacTypes.h>
#include <fstream>
#include <iostream>
#include <vector>

class bmp3 {
  std::string fileName;
  Header header;
  InfoHeader infoHeader;

public:
  explicit bmp3(const std::string &path) : fileName(path) {
    std::ifstream file(path, std::ios::binary);

    if (file.is_open()) {
      file.read(reinterpret_cast<char *>(&header), sizeof(header));
      file.read(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));
      file.close();

      try {
        isValid();
      } catch (const std::exception &e) {
        std::cerr << e.what();
      }
    }
  }

  void create(const std::vector<std::vector<BYTE>> &image,
              std::vector<DWORD> palette) {
    std::ofstream file(fileName, std::ios::binary);

    if (file.is_open()) {
      std::vector<BYTE> imageWithPaddings = addPadding(image);
      infoHeader.sizeImage = imageWithPaddings.size();

      file.write(reinterpret_cast<char *>(&header), sizeof(header));
      file.write(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));
      file.write(reinterpret_cast<char *>(palette.data()), palette.size() * 4);
      file.write(reinterpret_cast<const char *>(imageWithPaddings.data()),
                 imageWithPaddings.size() * sizeof(WORD));
    }

    file.close();
  }

  void resize(size_t width, size_t height) {
    infoHeader.width = width;
    infoHeader.height = height;
  }

private:
  std::vector<BYTE> addPadding(const std::vector<std::vector<BYTE>> &image) {
    std::vector<std::vector<BYTE>> imageWithPaddings(
        infoHeader.height,
        std::vector<BYTE>(infoHeader.width % 32
                              ? (infoHeader.width / 32 + 1) * 4
                              : infoHeader.width / 8));

    for (int i = 0; i < image.size(); i++)
      std::copy(image[i].begin(), image[i].end(), imageWithPaddings[i].begin());

    std::vector<BYTE> result;

    for (std::vector<BYTE> vec : imageWithPaddings)
      result.insert(result.cbegin(), vec.begin(), vec.end());

    return result;
  }

  void isValid() {
    if (header.type != 0x4d42)
      throw WrongTypeOfFileException("type of file is not bmp");
    if (infoHeader.size != 40)
      throw WrongTypeOfFileException("bmp version is wrong, should be v3");
  }
};

#endif // BITMAP_BMP_H
