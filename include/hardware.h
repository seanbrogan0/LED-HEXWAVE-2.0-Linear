// File: include/hardware.h

#pragma once
#include <Adafruit_NeoPixel.h>

// =========================================================
// PIN DEFINITIONS
// =========================================================
#define PIN_LEDS        17
#define PIN_LEFTBTN     4
#define PIN_RIGHTBTN    7
#define PIN_LED_IND     6
#define PIN_RIGHTPOT    A2
#define PIN_LEFTPOT     A3
#define PIN_MIC         A0

// =========================================================
// LED STRIP + GLOBAL BRIGHTNESS
// =========================================================
extern Adafruit_NeoPixel strip;
extern int globalMaxBright;

// =========================================================
// LED COUNT
// =========================================================
#define NUM_LEDS 216

// =========================================================
// INITIALISATION
// =========================================================
void initHardware();
