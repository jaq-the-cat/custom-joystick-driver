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

extern int min_x, max_x;
extern int min_y, max_y;
extern int min_throttle, max_throttle;
extern int min_rudder, max_rudder;
