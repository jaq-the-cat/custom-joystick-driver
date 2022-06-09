#include <stdio.h>
#include "driver.h"
#include "g.h"

int main() {
  printf("c_data:   %lu\n", sizeof(c_data));
  printf("byte:     %lu\n", sizeof(byte));
  printf("c_event:  %lu\n", sizeof(c_event));
  return 0;
}
