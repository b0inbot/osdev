/* file: drv/uart.cc
 *
 * A UART implementation based on lots of stuff taken from the osdev wiki.
 * It wraps it in an OO object to match the rest of the codebase.
 *
 */
#include "drv/uart.hh"
#include "sys/syscalls.hh"

//
// a lot of this from the osdev wiki
// https://wiki.osdev.org/Serial_Ports#Port_Addresses
//
extern "C" {
static inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
  return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
  __asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}
}

#define PORT _port

bool Uart::init() {
  outb(PORT + 1, 0x00); // Disable all interrupts
  outb(PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
  outb(PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
  outb(PORT + 1, 0x00); //                  (hi byte)
  outb(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
  outb(PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
  outb(PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
  outb(PORT + 4, 0x1E); // Set in loopback mode, test the serial chip
  outb(PORT + 0, 0xAE); // Test serial chip (send byte 0xAE and check if serial
                        // returns same byte)

  // Check if serial is faulty (i.e: not same byte as sent)
  if (inb(PORT + 0) != 0xAE) {
    return false;
  }

  // If serial is not faulty set it in normal operation mode
  // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
  outb(PORT + 4, 0x0F);
  return true;
}

void Uart::putstr(const char *str) {
  while (*str != 0) {
    putch(*str++);
  }
}

int Uart::serial_received() { return inb(PORT + 5) & 1; }

char Uart::getch() {
  while (serial_received() == 0)
    ;

  return inb(PORT);
}

bool Uart::is_transmit_empty() { return (inb(PORT + 5) & 0x20) == 0; }

void Uart::putch(const char a) {
  while (is_transmit_empty())
    ;
  outb(PORT, a);
}
