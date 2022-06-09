#include <stdio.h>
#include <string.h>
#include "driver.h"
#include "g.h"

int main() {
  uint8_t bytes[2] = {0, 1};
  uint16_t big;

  memcpy(&big, bytes, 2);
  printf("big: %hu\n", big);
  return 0;
}
