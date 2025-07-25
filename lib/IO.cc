#include "lib/IO.hh"

void IO::putstr(const char *str) {
  while (*str != 0) {
    putch(*str++);
  }
}

uint32_t IO::readline(char *buf) {
  uint32_t x = 0;
  char c;
  buf[x] = 0;
  while (1) {
    c = getch();

    if (echo) {
      putch(c);
    }
    if (c == '\n' || c == '\r') {
      if (echo) {
        putch('\n');
      }
      return x;
    }
    buf[x++] = c;
    buf[x] = 0;
  }
}
