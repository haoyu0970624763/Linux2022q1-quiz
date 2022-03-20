#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int32_t min(int32_t a, int32_t b) { return a ^ ((a ^ b) & -(a >= b)); }

int main(int argc, char *argv[]) {

  int32_t input1 = atoi(argv[1]), input2 = atoi(argv[2]);

  printf("%d\n", min(input1, input2));
}
