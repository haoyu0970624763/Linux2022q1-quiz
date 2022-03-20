#include <limits.h>
int divide(int dividend, int divisor) {

  /* deal with the only case may overflow */
  if (dividend == INT_MIN && divisor == -1) {
    return INT_MAX;
  }
  /* signal variable  is to check the operation is plus or minus sign*/
  int signal = 1;
  unsigned int dvd = dividend;
  /* if dvd is negative , then deal with signal first
   * then correct the unsigned variable dvd to right value
   * the way to deal with divisor is same as the dividend
   * Use ~signal+1 to replace signal *(-1)
   */
  if (dividend < 0) {
    signal = ~signal + 1;
    dvd = ~dvd + 1;
  }
  unsigned int dvs = divisor;
  if (divisor < 0) {
    signal = ~signal + 1;
    dvs = ~dvs + 1;
  }

  if (dvd < dvs) {
    return 0;
  }
  /* Use faster way to deal with all of case ,
   * which divisor is the pattern of 2^k  */
  unsigned int special_case = divisor;
  int power_of_two = __builtin_ctz(special_case);
  special_case = special_case >> power_of_two;
  if (special_case == 1) {
    return signal * (dvd >> power_of_two);
  }
  /* find the maximum value of shift*/
  int shift = 0;
  while (dvd > (dvs << shift))
    shift++;

  unsigned int res = 0;
  while (dvd >= dvs) {
    while (dvd < (dvs << shift))
      shift--;
    res |= (unsigned int)1 << shift;
    dvd -= dvs << shift;
  }

  return res * signal;
}
