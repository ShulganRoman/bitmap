#ifndef BITMAP_BM_EXCEPTION_H
#define BITMAP_BM_EXCEPTION_H

#include <exception>
#include <string>

class WrongTypeOfFileException : public std::exception {
public:
  WrongTypeOfFileException(const std::string &message) : message(message) {}

  const char *what() const noexcept override { return message.c_str(); }

private:
  std::string message;
};

#endif // BITMAP_BM_EXCEPTION_H
