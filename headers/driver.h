#pragma once

#include <stdint.h>
#include <unistd.h>
#include <libevdev/libevdev.h>
#include <linux/input-event-codes.h>
#include <libevdev/libevdev-uinput.h>
#include <math.h>
#include "g.h"

#define true 0x01
#define false 0x00
#define byte uint8_t

#define EVENT(type, data) ((c_event) {type, data})
#define AXIS(x, y) ((c_data) {.axis={x, y}})
#define UP         ((c_data) {.is_down=false})
#define DOWN       ((c_data) {.is_down=true})

// hardcoded raw controller output values
#define X_RAW_MIN 0
#define X_RAW_MAX 1023

#define Y_RAW_MIN 0
#define Y_RAW_MAX 1023

#define THROTTLE_RAW_MIN 0
#define THROTTLE_RAW_MAX 1023

#define RUDDER_RAW_MIN 0
#define RUDDER_RAW_MAX 1023

typedef enum {
  // EV_KEY
  KEY_FLAPS_DOWN     = BTN_BACK,
  KEY_FLAPS_UP       = BTN_FORWARD,
  KEY_FIRE           = BTN_TRIGGER,
  KEY_BOMB_BAY_OPEN  = KEY_B,
  KEY_BOMB_BAY_CLOSE = KEY_C,
  // EV_MSC
  MSC_BOMB           = BTN_X,
  MSC_BOMBS          = BTN_Y,
  MSC_ROCKET         = BTN_Z,
} buttons;

typedef enum {
  // buttons
  BTN_FLAPS_DOWN,
  BTN_FLAPS_UP,
  BTN_FIRE,
  BTN_BOMB,
  BTN_BOMBS,
  BTN_BOMB_BAY,
  BTN_ROCKET,
  // axis
  AXS_JOY,
  AXS_THROTTLE,
  AXS_YAW,
} c_command_types;

typedef union {
  struct {
    int x, y;
  } axis;
  byte is_down; // 1 if down, 0 if release
} c_data;

typedef struct {
  c_command_types type;
  c_data data;
} c_event;

double convert_to_range(double n, double in_min, double in_max, double out_min, double out_max);
int setup_evdev();
int setup_read_commands();
