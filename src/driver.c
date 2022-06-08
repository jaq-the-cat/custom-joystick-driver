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
  dev = libevdev_new();
  libevdev_set_name(dev, "Arduino Controller");
  libevdev_enable_event_type(dev, EV_KEY);
  libevdev_enable_event_type(dev, EV_MSC);

  // make it a gamepad
  libevdev_enable_event_code(dev, EV_MSC, BTN_GAMEPAD, NULL);

  // keys
  libevdev_enable_event_code(dev, EV_KEY, KEY_FLAPS_DOWN, NULL);
  libevdev_enable_event_code(dev, EV_KEY, KEY_FLAPS_UP, NULL);
  libevdev_enable_event_code(dev, EV_KEY, KEY_FIRE, NULL);
  libevdev_enable_event_code(dev, EV_KEY, KEY_BOMB_BAY_OPEN, NULL);
  libevdev_enable_event_code(dev, EV_KEY, KEY_BOMB_BAY_CLOSE, NULL);

  // misc
  libevdev_enable_event_code(dev, EV_MSC, MSC_BOMB, NULL);
  libevdev_enable_event_code(dev, EV_MSC, MSC_BOMBS, NULL);
  libevdev_enable_event_code(dev, EV_MSC, MSC_ROCKET, NULL);

  // absolute movement
  // event codes
  libevdev_enable_event_code(dev, EV_ABS, ABS_X, NULL);
  x_dev_min = libevdev_get_abs_minimum(dev, ABS_X);
  max_x = libevdev_get_abs_maximum(dev, ABS_X);

  libevdev_enable_event_code(dev, EV_ABS, ABS_Y, NULL);
  y_dev_min = libevdev_get_abs_minimum(dev, ABS_Y);
  max_y = libevdev_get_abs_maximum(dev, ABS_Y);

  libevdev_enable_event_code(dev, EV_ABS, ABS_THROTTLE, NULL);
  throttle_dev_min = libevdev_get_abs_minimum(dev, ABS_THROTTLE);
  throttle_dev_max = libevdev_get_abs_maximum(dev, ABS_THROTTLE);

  libevdev_enable_event_code(dev, EV_ABS, ABS_RUDDER, NULL);
  rudder_dev_min = libevdev_get_abs_minimum(dev, ABS_RUDDER);
  rudder_dev_max = libevdev_get_abs_maximum(dev, ABS_RUDDER);

  return libevdev_uinput_create_from_device(dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &udev);
}

int setup_read_commands() {
  controller = open(USB, O_RDWR | O_NOCTTY);

  struct termios tty;
  memset(&tty, 0, sizeof tty);

  if (tcgetattr(controller, &tty) != 0) {
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

  tcflush(controller, TCIFLUSH);
  if (tcsetattr(controller, TCSANOW, &tty) != 0) {
    fprintf(stderr, "Error from tcsetattr");
    return -1;
  }

  return 0;
}
