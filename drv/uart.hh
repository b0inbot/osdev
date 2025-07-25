/* file: drv/uart.hh
 *
 * UART header implementation. Takes in the COMn port but only
 * COM1 has been tested (0x3f8)
 */

#pragma once

#define COM1_PORT 0x3f8
#define COM2_PORT 0x2f8

#include "lib/types.hh"

class Uart {
public:
  Uart(uint32_t port) : _port(port) {}
  bool init();
  void putstr(const char *str);
  void putch(const char c);

  char getch();

private:
  int serial_received();
  bool is_transmit_empty();
  uint32_t _port;
};
