#pragma once
#include <stdint.h>

#define byte uint8_t
#define true 0x01
#define false 0x00

extern byte s_firing;
extern byte s_bombs;
extern int controller;
extern struct libevdev *dev;
extern struct libevdev_uinput *udev;

extern int x_dev_min, max_x;
extern int y_dev_min, max_y;
extern int throttle_dev_min, throttle_dev_max;
extern int rudder_dev_min, rudder_dev_max;
