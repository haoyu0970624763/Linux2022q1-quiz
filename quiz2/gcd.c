#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint64_t min(int64_t a, int64_t b) { return a ^ ((a ^ b) & -(a >= b)); }

uint64_t gcd64(uint64_t u, uint64_t v) {
  /* if u or v equal zero , then return the number  which is not equal to zero
   */
  if (!u || !v)
    return u | v;

  /*
    __builtin_ctz is used to count the rightmost zero number
    shift is used to record common factor of 2^k which means the smaller
    __builtin_ctz
  */
  int shift_u = __builtin_ctz(u);
  int shift_v = __builtin_ctz(v);
  int shift = min(shift_u, shift_v);

  u >> shift;
  v >> shift;

  // 更相減損法
  if (!(u & 1))
    u >>= __builtin_ctz(u);
  do {
    if (!(v & 1))
      v >>= __builtin_ctz(v);
    if (u < v) {
      v -= u;
    } else {
      uint32_t t = u - v;
      u = v;
      v = t;
    }
  } while (v);
  return u << shift;
}

int main(int argc, char *argv[]) {

  uint64_t input1 = strtoul(argv[1], NULL, 10),
           input2 = strtoul(argv[2], NULL, 10);

  printf("%lu\n", gcd64(input1, input2));
}