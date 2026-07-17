// /src/modes/non_audio/mode_dualCometsHexPath.cpp

#include "modes/non_audio/mode_dualCometsHexPath.h"
#include "input.h"
#include "utils.h"
#include "palette.h"
#include "hex_geometry.h"
#include "path_math.h"
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

static const int TOTAL_PATH = PATH_LEN_COMETS * HEX_LED_COUNT;

// Convert path position → LED index (path data lives in hex_geometry)
static int getPathLED(int pathPos) {
    return pathPosToLED(pathPos, HEX_PATH_COMETS, PATH_LEN_COMETS,
                        HEXES, HEX_LED_COUNT);
}

// Comet positions + colours
static int pos1 = 0;
static int pos2 = TOTAL_PATH / 2;

static uint32_t col1 = 0;
static uint32_t col2 = 0;

static bool firstRun = true;

// =========================================================
// APPLY GLOW FOR A SINGLE COMET
// headPos = path position of comet head
// colour  = comet colour
// =========================================================
static void drawCometGlow(int headPos, uint32_t colour) {

    // -----------------------------------------------------
    // HEADER GLOW (in front of head)
    // -----------------------------------------------------
    int headerPos = wrapPathPos(headPos + 1, TOTAL_PATH);
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
        int tailPos = wrapPathPos(headPos - t, TOTAL_PATH);
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

        // -----------------------------------------------------
        // GLOBAL FADE (original 0.97 tail fade)
        // -----------------------------------------------------
        for (int i = 0; i < NUM_LEDS; i++) {
            strip.setPixelColor(i, dimColour(strip.getPixelColor(i), 0.97f));
        }

        // -----------------------------------------------------
        // DRAW COMET 1 WITH EXTREME EXPONENTIAL GLOW
        // -----------------------------------------------------
        drawCometGlow(pos1, col1);
        pos1++;
        if (pos1 >= TOTAL_PATH) {
            pos1 = 0;
            col1 = randomPaletteColour();   // new colour on wrap
        }

        // -----------------------------------------------------
        // DRAW COMET 2 WITH EXTREME EXPONENTIAL GLOW
        // -----------------------------------------------------
        drawCometGlow(pos2, col2);
        pos2++;
        if (pos2 >= TOTAL_PATH) {
            pos2 = 0;
            col2 = randomPaletteColour();   // new colour on wrap
        }
    }
}
