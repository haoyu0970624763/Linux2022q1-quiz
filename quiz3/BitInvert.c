#include <stdint.h>
uint8_t rev8(uint8_t x) {
  x = (x >> 4) | (x << 4);
  x = ((x & 0xCC) >> 2) | ((x & 0x33) << 2);
  x = ((x & 0xAA) >> 1) | ((x & 0x55) << 1);
  return x;
}
