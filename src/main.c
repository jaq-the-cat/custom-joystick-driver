#include <stdio.h>
#include <stdlib.h>
#include "driver.h"
#include "g.h"

#define DO(type, code, value)\
  libevdev_uinput_write_event(g.udev, type, code, value);\
  libevdev_uinput_write_event(g.udev, EV_SYN, SYN_REPORT, 0)

// is firing
byte s_firing;
// is dropping bomb series
byte s_bombs;

c_event receive_commands() {
  byte response[sizeof(c_event)];
  read(g.controller, response, sizeof(c_event)); // messages sent by controller are sizeof(c_event) or 12 bytes
  c_event r = *((c_event*) response);
  printf("c_event type: %d\n", r.type);
  return r;
}

void process_commands(c_event ev) {
  switch (ev.type) {
    case BTN_FLAPS_DOWN:
      printf("flaps down\n");
      DO(EV_KEY, KEY_FLAPS_DOWN, true); // press
      DO(EV_KEY, KEY_FLAPS_DOWN, false); // release
      break;
    case BTN_FLAPS_UP:
      printf("flaps up\n");
      DO(EV_KEY, KEY_FLAPS_UP, true);
      DO(EV_KEY, KEY_FLAPS_UP, false);
      break;
    case BTN_FIRE:
      printf("fire pressed\n");
      s_firing = ev.data.is_down;
      if (!s_firing) { // just released
        printf("fire released\n");
        DO(EV_KEY, KEY_FIRE, false);
      }
      break;
    case BTN_BOMB:
      DO(EV_MSC, MSC_BOMB, true);
      DO(EV_MSC, MSC_BOMB, false);
      break;
    case BTN_BOMBS:
      s_bombs = ev.data.is_down;
      if (!s_bombs) { // just released
        DO(EV_MSC, MSC_BOMBS, false);
      }
      break;
    case BTN_BOMB_BAY:
      if (ev.data.is_down) {// toggle off
        DO(EV_KEY, KEY_BOMB_BAY_CLOSE, true);
        DO(EV_KEY, KEY_BOMB_BAY_CLOSE, false);
      } else {
        DO(EV_KEY, KEY_BOMB_BAY_OPEN, true);
        DO(EV_KEY, KEY_BOMB_BAY_OPEN, false);
      }
      break;
    case BTN_ROCKET:
        DO(EV_MSC, MSC_ROCKET, true);
        DO(EV_MSC, MSC_ROCKET, false);
      break;

    // axis
    case AXS_JOY:
      printf("joystick: %d %d\n", ev.data.axis.x, ev.data.axis.y);
      DO(EV_ABS, ABS_X, convert_to_range(ev.data.axis.x,
            X_RAW_MIN, X_RAW_MAX,
            g.x_dev_min, g.max_x));
      DO(EV_ABS, ABS_Y, convert_to_range(ev.data.axis.y,
            Y_RAW_MIN, Y_RAW_MAX,
            g.y_dev_min, g.max_y));
      break;
    case AXS_THROTTLE:
      printf("throttle: %d\n", ev.data.axis.y);
      DO(EV_ABS, ABS_THROTTLE, convert_to_range(ev.data.axis.y,
            THROTTLE_RAW_MIN, THROTTLE_RAW_MAX,
            g.throttle_dev_min, g.throttle_dev_max));
      break;
    case AXS_YAW:
      DO(EV_ABS, ABS_RUDDER, convert_to_range(ev.data.axis.x,
            RUDDER_RAW_MIN, RUDDER_RAW_MAX,
            g.rudder_dev_min, g.rudder_dev_max));
      break;
  }

  if (s_firing) {
    DO(EV_KEY, KEY_FIRE, true);
  }

  if (s_bombs) {
    DO(EV_MSC, MSC_BOMBS, true);
  }
}

int main(int argc, char* argv[]) {
  usleep(100*1000); // 100ms
  
  int err = setup_evdev() | setup_read_commands();
  if (err != 0)
    return err;
  
  while (true) {
    process_commands(receive_commands());
    usleep(100);
  }

  libevdev_uinput_destroy(g.udev);

  return 0;
}
