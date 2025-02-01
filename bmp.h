#ifndef BITMAP_BMP_H
#define BITMAP_BMP_H

#include "color.h"
#include "defs.h"
#include <string>

struct bmp {
public:
  MAYBE_UN void print_map() const;

  MAYBE_UN std::string create_bmp();

  MAYBE_UN unsigned int size_of_file() const;

  MAYBE_UN void inversion_image();

private:
  void header(std::string &new_str) {
    new_str += "BM";
    new_str += convert_to_string<UI>(_size_of_file);
    new_str += convert_to_string<US>(0);
    new_str += convert_to_string<US>(0);
    new_str += convert_to_string<UI>(_count_bit);
    new_str += convert_to_string<UI>(56);
    new_str += convert_to_string<int>(_width);
    new_str += convert_to_string<int>(_height);
    new_str += convert_to_string<US>(1);
    new_str += convert_to_string<US>(32);
    new_str += convert_to_string<UI>(0);
    new_str += convert_to_string<UI>(49152);
    new_str += convert_to_string<int>(2834);
    new_str += convert_to_string<int>(2834);
    new_str += convert_to_string<UI>(0);
    new_str += convert_to_string<UI>(0);
    new_str += convert_to_string<UI>(16711680);
    new_str += convert_to_string<UI>(65280);
    new_str += convert_to_string<UI>(255);
    new_str += convert_to_string<UI>(4278190080);
  }

  template <typename T> T convert_to_value(int position);

  template <typename T> std::string convert_to_string(T value);

public:
  MAYBE_UN explicit bmp(std::string &s)
      : _str(s), _count_bit(convert_to_value<int>(10)),
        _width(convert_to_value<int>(18)), _height(convert_to_value<int>(22)),
        _size_of_file(_height * _width * 4 + _count_bit) {
    _bitmap = new color *[_height];
    for (int i = 0; i < _height; i++)
      *(i + _bitmap) = new color[_width];

    for (int i = _height - 1, k = _count_bit; i >= 0; i--)
      for (int j = 0; j < _width; j++) {
        _bitmap[i][j].B = convert_to_value<UC>(k++);
        _bitmap[i][j].G = convert_to_value<UC>(k++);
        _bitmap[i][j].R = convert_to_value<UC>(k++);
        _bitmap[i][j].A = convert_to_value<UC>(k++);
      }
  }

  MAYBE_UN explicit bmp(int width, int height, color **image)
      : _str(), _count_bit(70), _width(width), _height(height),
        _size_of_file(_height * _width * 4 + _count_bit) {
    _bitmap = new color *[_height];
    for (int i = 0; i < _height; i++)
      *(i + _bitmap) = new color[_width];

    for (int i = 0; i < _height; i++)
      for (int j = 0; j < _width; j++)
        _bitmap[i][j] = image[i][j];
  }

private:
  color **_bitmap;
  std::string _str, _header;
  int _width, _height, _count_bit;
  unsigned int _size_of_file;
};

template <typename T> T bmp::convert_to_value(int position) {
  std::string::iterator iter = _str.begin() + position;
  char *array = new char[4];
  for (int i = 0; i < sizeof(T); i++) {
    array[i] = *iter;
    iter++;
  }
  T *value = reinterpret_cast<T *>(array);
  return *value;
}

template <typename T> std::string bmp::convert_to_string(T value) {
  std::string s;
  char *symbol = reinterpret_cast<char *>(&value);
  for (int i = 0; i < sizeof(T); i++)
    s += symbol[i];
  return s;
}

MAYBE_UN void bmp::print_map() const {
  for (int i = 0; i < _height; i++)
    for (int j = 0; j < _width; j++)
      _bitmap[i][j]._pixel_info();
}

MAYBE_UN std::string bmp::create_bmp() {
  std::string new_str;
  header(new_str);
  for (int i = _height - 1; i >= 0; i--)
    for (int j = 0; j < _width; j++) {
      new_str += char(_bitmap[i][j].B);
      new_str += char(_bitmap[i][j].G);
      new_str += char(_bitmap[i][j].R);
      new_str += char(_bitmap[i][j].A);
    }
  return new_str;
}

MAYBE_UN unsigned int bmp::size_of_file() const { return _size_of_file; }

MAYBE_UN void bmp::inversion_image() {
  for (int i = _height - 1; i >= 0; i--)
    for (int j = 0; j < _width; j++) {
      _bitmap[i][j].R = 255 - _bitmap[i][j].R;
      _bitmap[i][j].G = 255 - _bitmap[i][j].G;
      _bitmap[i][j].B = 255 - _bitmap[i][j].B;
    }
}

#endif // BITMAP_BMP_H
