#include "g.h"
byte s_firing = false; // not firing
byte s_bombs = false; // not dropping bomb series

int controller;
struct libevdev *dev;
struct libevdev_uinput *udev;

// abs value params
int x_dev_min, max_x;
int y_dev_min, max_y;
int throttle_dev_min, throttle_dev_max;
int rudder_dev_min, rudder_dev_max;
