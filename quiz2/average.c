#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
uint32_t average(uint32_t a, uint32_t b) {
  // input scan order doesn't limit
  return (a >> 1) + (b >> 1) + (a & b & 0x01);
}

int main(int argc, char *argv[]) {

  uint32_t input1 = strtoul(argv[1], NULL, 10),
           input2 = strtoul(argv[2], NULL, 10);

  printf("%u\n", average(input1, input2));
}