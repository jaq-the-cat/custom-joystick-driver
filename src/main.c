#include <stdio.h>
#include <stdlib.h>
#include <xdo.h>
#include <unistd.h>

#define TRUE 0x01
#define FALSE 0x00

#define K_FLAPS_DOWN "Mouse5" // backwards
#define K_FLAPS_UP "Mouse4" // forwards
#define K_FIRE "Mouse1"
#define K_BOMB "3"
#define K_BOMBS "0"
#define K_BOMB_BAY_OPEN "B"
#define K_BOMB_BAY_CLOSE "b"
#define K_ROCKETS "4"
// axis

#define KEY(key) xdo_send_keysequence_window(x, CURRENTWINDOW, key, 1)
#define MOUSE(button)\
  xdo_mouse_down(x, CURRENTWINDOW, button);\
  usleep(100);\
  xdo_mouse_up(x, CURRENTWINDOW, button)

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

void receive_commands(xdo_t *x, c_event ev) {
  switch (ev.type) {
    case B_FLAPS_DOWN:
      KEY(K_FLAPS_DOWN);
      break;
    case B_FLAPS_UP:
      KEY(K_FLAPS_UP);
      break;
    case B_FIRE:
      s_firing = ev.data.is_down;
      break;
    case B_BOMB:
      KEY(K_BOMB);
      break;
    case B_BOMBS:
      s_bombs = ev.data.is_down;
      break;
    case B_BOMB_BAY:
      if (ev.data.is_down) // toggle off
        KEY(K_BOMB_BAY_CLOSE);
      else
        KEY(K_BOMB_BAY_OPEN); // toggle on
      break;
    case B_ROCKETS:
      KEY(K_ROCKETS);
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
  xdo_t *x = xdo_new(NULL);

  xdo_free(x);
  return 0;
}
