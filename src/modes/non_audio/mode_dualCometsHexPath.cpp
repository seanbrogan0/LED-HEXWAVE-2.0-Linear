// /src/modes/non_audio/mode_dualCometsHexPath.cpp

#include "modes/non_audio/mode_dualCometsHexPath.h"
#include "input.h"
#include "utils.h"
#include "palette.h"
#include "hex_mapping.h"
#include "hardware.h"
#include <Arduino.h>

// =========================================================
// MODE 3 — DUAL COMETS WITH EXTREME 80‑LED EXPONENTIAL GLOW
// Two comets follow a custom hex path with:
// - 15% header glow
// - 80‑LED exponential tail (k = 0.04)
// - Only first 10 LEDs freshly drawn (prevents colour pop)
// - Original 0.97 global fade
// Left pot = speed. Right pot = brightness.
// =========================================================

// Original custom path (not perimeter)
static int HEX_PATH_COMETS[] = {1,2,3,2,4,5,6,7,8,9};
static const int PATH_LEN_COMETS = 10;

// Convert path position → LED index
static int getPathLED(int pathPos) {
    int hexIndex = HEX_PATH_COMETS[pathPos / HEX_SIZE] - 1;
    int ledIndex = pathPos % HEX_SIZE;
    return HEXES[hexIndex][ledIndex];
}

// Comet positions + colours
static int pos1 = 0;
static int pos2 = (PATH_LEN_COMETS * HEX_SIZE) / 2;

static uint32_t col1 = 0;
static uint32_t col2 = 0;

static bool firstRun = true;

// =========================================================
// APPLY GLOW FOR A SINGLE COMET
// headPos = path position of comet head
// colour  = comet colour
// =========================================================
static void drawCometGlow(int headPos, uint32_t colour) {

    int totalPath = PATH_LEN_COMETS * HEX_SIZE;

    // Wrap helper
    auto wrap = [&](int p) {
        if (p < 0) return p + totalPath;
        if (p >= totalPath) return p - totalPath;
        return p;
    };

    // -----------------------------------------------------
    // HEADER GLOW (in front of head)
    // -----------------------------------------------------
    int headerPos = wrap(headPos + 1);
    strip.setPixelColor(getPathLED(headerPos), dimColour(colour, 0.15f));  // 15%

    // -----------------------------------------------------
    // HEAD
    // -----------------------------------------------------
    strip.setPixelColor(getPathLED(headPos), colour);  // 100%

    // -----------------------------------------------------
    // EXPONENTIAL TAIL (80 LEDs total)
    // Only first 10 LEDs are freshly drawn.
    // LEDs 11–80 fade naturally (prevents colour pop).
    // -----------------------------------------------------
    const float k = 0.04f;

    for (int t = 1; t <= 10; t++) {   // draw only first 10
        float level = expf(-k * t);
        int tailPos = wrap(headPos - t);
        strip.setPixelColor(getPathLED(tailPos), dimColour(colour, level));
    }

    // LEDs 11–80 are intentionally NOT redrawn.
    // They retain their previous colour and fade out via the global 0.97 fade.
}

// =========================================================
// DUAL COMETS MODE
// =========================================================
void mode_dualCometsHexPath() {

    // ---------------------------------------------------------
    // INITIALISE COLOURS ON FIRST RUN
    // ---------------------------------------------------------
    if (firstRun) {
        col1 = randomPaletteColour();
        col2 = randomPaletteColour();
        firstRun = false;
    }

    // ---------------------------------------------------------
    // LEFT POT = SPEED (matches original curve)
    // ---------------------------------------------------------
    int speed = map(leftPotValue, 0, 4095, 40, 5);

    static unsigned long lastMove = 0;
    unsigned long now = millis();

    if (now - lastMove >= (unsigned long)speed) {
        lastMove = now;

        int totalPath = PATH_LEN_COMETS * HEX_SIZE;

        // -----------------------------------------------------
        // GLOBAL FADE (original 0.97 tail fade)
        // -----------------------------------------------------
        for (int i = 0; i < NUM_LEDS; i++) {
            uint32_t c = strip.getPixelColor(i);

            uint8_t r = (c >> 16) & 0xFF;
            uint8_t g = (c >> 8)  & 0xFF;
            uint8_t b =  c        & 0xFF;

            r = (uint8_t)(r * 0.97f);
            g = (uint8_t)(g * 0.97f);
            b = (uint8_t)(b * 0.97f);

            strip.setPixelColor(i, r, g, b);
        }

        // -----------------------------------------------------
        // DRAW COMET 1 WITH EXTREME EXPONENTIAL GLOW
        // -----------------------------------------------------
        drawCometGlow(pos1, col1);
        pos1++;
        if (pos1 >= totalPath) {
            pos1 = 0;
            col1 = randomPaletteColour();   // new colour on wrap
        }

        // -----------------------------------------------------
        // DRAW COMET 2 WITH EXTREME EXPONENTIAL GLOW
        // -----------------------------------------------------
        drawCometGlow(pos2, col2);
        pos2++;
        if (pos2 >= totalPath) {
            pos2 = 0;
            col2 = randomPaletteColour();   // new colour on wrap
        }
    }
}
