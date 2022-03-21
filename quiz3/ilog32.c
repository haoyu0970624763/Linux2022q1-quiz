#include <stdint.h>
int ilog32(uint32_t v) {
  //  if v is zero , bits are all zero , there is nothing can be stored
  int ret = v > 0;
  /* if v is greater than 0xFFFFU , then is must be greater or equal than
   * 0x00010000 it must store at least sixteen number if v is not greater than
   * 0xFFFFU , then m=0 , ret doesn't change
   */
  int m = (v > 0xFFFFU) << 4;
  v >>= m;
  ret |= m;
  /* 12-14 code have same logic with code 8-10  */
  m = (v > 0xFFU) << 3;
  v >>= m;
  ret |= m;
  m = (v > 0xFU) << 2;
  v >>= m;
  ret |= m;
  m = (v > 0x3) << 1;
  v >>= m;
  ret |= m;
  ret |= v > 1;
  return ret;
}