// File: src/hardware.cpp

#include "hardware.h"

// =========================================================
// STRIP INITIALISATION
// =========================================================
Adafruit_NeoPixel strip(NUM_LEDS, PIN_LEDS, NEO_GRB + NEO_KHZ800);

// Global brightness cap
int globalMaxBright = 160;

// =========================================================
// HARDWARE INITIALISATION (OPTIONAL EXPANSION POINT)
// =========================================================
void initHardware() {
    // If you ever want to expand hardware init logic,
    // this function is here for that purpose.
    strip.begin();
    strip.show();
}
