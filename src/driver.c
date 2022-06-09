#include <stdio.h>
#include "driver.h"
#include <fcntl.h>
#include <termios.h>
#include <string.h>

#define USB "/dev/ttyUSB0"

double convert_to_range(double n, double in_min, double in_max, double out_min, double out_max) {
  return (n - in_min) * ((out_max - out_min) / (in_max - in_min)) + out_min;
}

int setup_evdev() {
  g.dev = libevdev_new();
  libevdev_set_name(g.dev, "Arduino Controller");
  libevdev_enable_event_type(g.dev, EV_KEY);
  libevdev_enable_event_type(g.dev, EV_MSC);

  // make it a gamepad
  libevdev_enable_event_code(g.dev, EV_MSC, BTN_GAMEPAD, NULL);

  // keys
  libevdev_enable_event_code(g.dev, EV_KEY, KEY_FLAPS_DOWN, NULL);
  libevdev_enable_event_code(g.dev, EV_KEY, KEY_FLAPS_UP, NULL);
  libevdev_enable_event_code(g.dev, EV_KEY, KEY_FIRE, NULL);
  libevdev_enable_event_code(g.dev, EV_KEY, KEY_BOMB_BAY_OPEN, NULL);
  libevdev_enable_event_code(g.dev, EV_KEY, KEY_BOMB_BAY_CLOSE, NULL);

  // misc
  libevdev_enable_event_code(g.dev, EV_MSC, MSC_BOMB, NULL);
  libevdev_enable_event_code(g.dev, EV_MSC, MSC_BOMBS, NULL);
  libevdev_enable_event_code(g.dev, EV_MSC, MSC_ROCKET, NULL);

  // absolute movement
  // event codes
  libevdev_enable_event_code(g.dev, EV_ABS, ABS_X, NULL);
  g.x_dev_min = libevdev_get_abs_minimum(g.dev, ABS_X);
  g.max_x = libevdev_get_abs_maximum(g.dev, ABS_X);

  libevdev_enable_event_code(g.dev, EV_ABS, ABS_Y, NULL);
  g.y_dev_min = libevdev_get_abs_minimum(g.dev, ABS_Y);
  g.max_y = libevdev_get_abs_maximum(g.dev, ABS_Y);

  libevdev_enable_event_code(g.dev, EV_ABS, ABS_THROTTLE, NULL);
  g.throttle_dev_min = libevdev_get_abs_minimum(g.dev, ABS_THROTTLE);
  g.throttle_dev_max = libevdev_get_abs_maximum(g.dev, ABS_THROTTLE);

  libevdev_enable_event_code(g.dev, EV_ABS, ABS_RUDDER, NULL);
  g.yaw_dev_min = libevdev_get_abs_minimum(g.dev, ABS_RUDDER);
  g.yaw_dev_max = libevdev_get_abs_maximum(g.dev, ABS_RUDDER);

  return libevdev_uinput_create_from_device(g.dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &g.udev);
}

int setup_read_commands() {
  g.controller = open(USB, O_RDWR | O_NOCTTY);

  struct termios tty;
  memset(&tty, 0, sizeof tty);

  if (tcgetattr(g.controller, &tty) != 0) {
    fprintf(stderr, "Error from tcgetattr");
    return -1;
  }

  cfsetospeed(&tty, (speed_t)B9600);
  cfsetispeed(&tty, (speed_t)B9600);

  tty.c_cflag &= -PARENB;
  tty.c_cflag &= -CSTOPB;
  tty.c_cflag &= -CSIZE;
  tty.c_cflag |= CS8;

  tty.c_cflag |= -CRTSCTS;
  tty.c_cc[VMIN] = 1;
  tty.c_cc[VTIME] |= 5;
  tty.c_cflag |= CREAD | CLOCAL;

  cfmakeraw(&tty);

  tcflush(g.controller, TCIFLUSH);
  if (tcsetattr(g.controller, TCSANOW, &tty) != 0) {
    fprintf(stderr, "Error from tcsetattr");
    return -1;
  }

  return 0;
}
