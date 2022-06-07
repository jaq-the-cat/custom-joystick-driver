#include <stdio.h>
#include "driver.h"
#include "g.h"

int main() {
  int axis_min = 0, axis_max = 1024;
  int evdev_min = 0, evdev_max = 100;

  printf("%f\n", convert_to_range(900,
        axis_min, axis_max,
        evdev_min, evdev_max));
  return 0;
}
