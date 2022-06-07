#include "g.h"
byte s_firing = false; // not firing
byte s_bombs = false; // not dropping bomb series

int controller;
struct libevdev *dev;
struct libevdev_uinput *udev;

// abs value params
int min_x, max_x;
int min_y, max_y;
int min_throttle, max_throttle;
int min_rudder, max_rudder;
