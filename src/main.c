#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libevdev/libevdev.h>
#include <linux/input-event-codes.h>
#include <libevdev/libevdev-uinput.h>

#include <string.h>

#define TRUE 0x01
#define FALSE 0x00

typedef enum {
  // EV_KEY
  K_FLAPS_DOWN     = BTN_BACK,
  K_FLAPS_UP       = BTN_FORWARD,
  K_FIRE           = BTN_TRIGGER,
  // EV_MSC
  K_BOMB           = BTN_X,
  K_BOMBS          = BTN_Y,
  K_ROCKETS        = BTN_Z,
  K_BOMB_BAY_OPEN  = KEY_B,
  K_BOMB_BAY_CLOSE = KEY_C,
} buttons;

#define KPRESS(key) //xdo_send_keysequence_window(x, CURRENTWINDOW, key, 1)
#define MPRESS(button)\
  usleep(100);
  //xdo_mouse_down(x, CURRENTWINDOW, button);
  //xdo_mouse_up(x, CURRENTWINDOW, button)

typedef unsigned char byte;

typedef enum {
  // buttons
  B_FLAPS_DOWN,
  B_FLAPS_UP,
  B_FIRE,
  B_BOMB,
  B_BOMBS,
  B_BOMB_BAY,
  B_ROCKETS,
  // axis
  A_JOY,
  A_THROTTLE,
  A_YAW,
  A_MIX,
  A_PITCH,
} c_command_types;

typedef union {
  struct {
    double x, y;
  } axis;
  byte is_down; // 1 if down, 0 if release
} c_data;

typedef struct {
  c_command_types type;
  c_data data;
} c_event;

byte s_firing = FALSE; // not firing
byte s_bombs = FALSE; // not dropping bomb series

void receive_commands(c_event ev) {
  switch (ev.type) {
    case B_FLAPS_DOWN:
      KPRESS(K_FLAPS_DOWN);
      break;
    case B_FLAPS_UP:
      KPRESS(K_FLAPS_UP);
      break;
    case B_FIRE:
      s_firing = ev.data.is_down;
      break;
    case B_BOMB:
      KPRESS(K_BOMB);
      break;
    case B_BOMBS:
      s_bombs = ev.data.is_down;
      break;
    case B_BOMB_BAY:
      if (ev.data.is_down) // toggle off
        KPRESS(K_BOMB_BAY_CLOSE);
      else
        KPRESS(K_BOMB_BAY_OPEN); // toggle on
      break;
    case B_ROCKETS:
      KPRESS(K_ROCKETS);
      break;
    // axis
    case A_JOY:
      break;
    case A_THROTTLE:
      break;
    case A_YAW:
      break;
    case A_MIX:
      break;
    case A_PITCH:
      break;
  }
}

int main(int argc, char* argv[]) {
  int err;
  struct libevdev *dev = NULL;
  struct libevdev_uinput *udev = NULL;

  dev = libevdev_new();
  libevdev_set_name(dev, "Arduino Controller");
  libevdev_enable_event_type(dev, EV_KEY);
  libevdev_enable_event_type(dev, EV_MSC);

  // make it a gamepad
  libevdev_enable_event_code(dev, EV_MSC, BTN_GAMEPAD, NULL);

  libevdev_enable_event_code(dev, EV_KEY, KEY_A, NULL);
  libevdev_enable_event_code(dev, EV_KEY, K_FLAPS_DOWN, NULL);
  libevdev_enable_event_code(dev, EV_KEY, K_FLAPS_UP, NULL);
  libevdev_enable_event_code(dev, EV_KEY, K_FIRE, NULL);

  libevdev_enable_event_code(dev, EV_MSC, K_BOMB, NULL);
  libevdev_enable_event_code(dev, EV_MSC, K_BOMBS, NULL);
  libevdev_enable_event_code(dev, EV_MSC, K_ROCKETS, NULL);
  libevdev_enable_event_code(dev, EV_MSC, K_BOMB_BAY_OPEN, NULL);
  libevdev_enable_event_code(dev, EV_MSC, K_BOMB_BAY_CLOSE, NULL);

  libevdev_enable_event_code(dev, EV_KEY, KEY_M, NULL);
  libevdev_enable_event_code(dev, EV_KEY, KEY_O, NULL);
  libevdev_enable_event_code(dev, EV_KEY, KEY_G, NULL);
  libevdev_enable_event_code(dev, EV_KEY, KEY_U, NULL);
  libevdev_enable_event_code(dev, EV_KEY, KEY_S, NULL);

  err = libevdev_uinput_create_from_device(dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &udev);
  if (err != 0)
    return err;

  usleep(100000);

  usleep(3*1000*1000); // sleep for 3s
  
  int amogus[6] = {KEY_A, KEY_M, KEY_O, KEY_G, KEY_U, KEY_S};
  for (int i=0; i<6; i++) {
    // start event
    libevdev_uinput_write_event(udev, EV_KEY, amogus[i], 1);
    // finish
    libevdev_uinput_write_event(udev, EV_SYN, SYN_REPORT, 0);

    // start event
    libevdev_uinput_write_event(udev, EV_KEY, amogus[i], 0);
    // finish
    libevdev_uinput_write_event(udev, EV_SYN, SYN_REPORT, 0);
  }


  usleep(10000);

  libevdev_uinput_destroy(udev);

  return 0;
}
