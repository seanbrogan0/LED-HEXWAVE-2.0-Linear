// File: src/utils.cpp

#include "utils.h"

// =========================================================
// GENERIC HELPERS
// =========================================================
int clampInt(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

float clampFloat(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

int wrapIndex(int i, int max) {
    if (i < 0) return max - 1;
    if (i >= max) return 0;
    return i;
}

// =========================================================
// LED HELPERS
// =========================================================
void setLED(int idx, uint32_t colour) {
    if (idx < 0 || idx >= NUM_LEDS) return;
    strip.setPixelColor(idx, colour);
}

void fillAll(uint32_t colour) {
    for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, colour);
    }
}

void fillHex(int hexIndex, uint32_t colour) {
    if (hexIndex < 0 || hexIndex >= 9) return;

    const uint16_t* hex = HEXES[hexIndex];
    for (int i = 0; i < HEX_SIZE; i++) {
        setLED(hex[i], colour);
    }
}

// =========================================================
// SEND COLOURS (APPLY GLOBAL BRIGHTNESS)
// =========================================================
void SendColours() {
    strip.setBrightness(globalMaxBright);  
    strip.show();
}

