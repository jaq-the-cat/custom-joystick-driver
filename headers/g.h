#pragma once
#include <stdint.h>

#define byte uint8_t
#define true 0x01
#define false 0x00

typedef struct {
  // devices
  int controller;
  struct libevdev *dev;
  struct libevdev_uinput *udev;

  // abs value params
  int x_dev_min, max_x;
  int y_dev_min, max_y;
  int throttle_dev_min, throttle_dev_max;
  int yaw_dev_min, yaw_dev_max;
} global;

extern global g;
