#pragma once

#include "lib/types.hh"

class IO {
public:
  virtual char getch() = 0;
  virtual void putch(char ch) = 0;

  void putstr(const char *str);
  uint32_t readline(char *buf);

  inline void setEcho(bool e) { echo = e; }

protected:
  bool echo{false};
};
