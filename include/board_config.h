// include/board_config.h
// =========================================================
// Everything hardware-specific for THIS fixture lives here.
// The sister repo (7-hex Flower) differs only in this file,
// hex_geometry, and its mode set.
// =========================================================

#pragma once
#include <Arduino.h>

// =========================================================
// PIN DEFINITIONS
// =========================================================
#define PIN_LEDS        17
#define PIN_LEFTBTN     4
#define PIN_RIGHTBTN    7
#define PIN_LED_IND     6      // audio-mode indicator LED (omit on boards without one)
#define PIN_LEFTPOT     A3
#define PIN_RIGHTPOT    A2
#define PIN_MIC         A0

// =========================================================
// STRIP + FRAME CONFIGURATION
// =========================================================
#define NUM_LEDS            216
#define MAX_BRIGHTNESS      160     // power-budget cap applied at show time
#define BRIGHTNESS_FLOOR    0.02f   // right pot never turns the strip fully off
#define FRAME_INTERVAL_MS   10      // 0 = run unpaced
