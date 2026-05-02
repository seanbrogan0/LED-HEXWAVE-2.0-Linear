// /src/modes/non_audio/mode_hexFadeRandom.cpp

#include "modes/non_audio/mode_hexFadeRandom.h"
#include "input.h"
#include "utils.h"
#include "palette.h"
#include "hex_mapping.h"
#include "hardware.h"
#include <Arduino.h>

// =========================================================
// MODE 2 — HEX FADE RANDOM
// Picks ONE hex, fades from its current colour to a new
// random palette colour. Other hexes keep their colours.
// Left pot = fade speed. Right pot = brightness.
// =========================================================

// Per-hex current colours
static uint32_t hexColours[9];

// Active fade state
static int currentHex = -1;
static uint32_t startColor = 0;
static uint32_t targetColor = 0;
static float blend = 0.0f;
static bool firstRun = true;

// =========================================================
// COLOUR LERP (start → target, t = 0..1)
// =========================================================
static uint32_t lerpColour(uint32_t a, uint32_t b, float t) {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    uint8_t ar = (a >> 16) & 0xFF;
    uint8_t ag = (a >> 8)  & 0xFF;
    uint8_t ab =  a        & 0xFF;

    uint8_t br = (b >> 16) & 0xFF;
    uint8_t bg = (b >> 8)  & 0xFF;
    uint8_t bb =  b        & 0xFF;

    uint8_t r = ar + (int)((br - ar) * t);
    uint8_t g = ag + (int)((bg - ag) * t);
    uint8_t bch = ab + (int)((bb - ab) * t);

    return strip.Color(r, g, bch);
}

// =========================================================
// SELECT NEXT HEX (PRIORITISE BLACK HEXES)
// =========================================================
static int pickNextHex() {
    // First collect all black hexes
    int blackList[9];
    int blackCount = 0;

    for (int i = 0; i < 9; i++) {
        if (hexColours[i] == strip.Color(0, 0, 0)) {
            blackList[blackCount++] = i;
        }
    }

    // If any black hexes exist → choose one of them
    if (blackCount > 0) {
        return blackList[random(blackCount)];
    }

    // Otherwise choose any hex
    return random(0, 9);
}

// =========================================================
// HEX FADE RANDOM MODE
// =========================================================
void mode_hexFadeRandom() {

    // ---------------------------------------------------------
    // INITIALISE PER-HEX COLOURS ON FIRST RUN
    // ---------------------------------------------------------
    if (firstRun) {
        for (int i = 0; i < 9; i++) {
            hexColours[i] = strip.Color(0, 0, 0);  // start from black
        }
        currentHex = pickNextHex();
        startColor = hexColours[currentHex];
        targetColor = randomPaletteColour();
        blend = 0.0f;
        firstRun = false;
    }

    // ---------------------------------------------------------
    // LEFT POT = FADE SPEED (slow → fast)
    // ---------------------------------------------------------
    float fadeSpeed = map(leftPotValue, 0, 4095, 1, 20) / 500.0f;

    // ---------------------------------------------------------
    // ADVANCE BLEND
    // ---------------------------------------------------------
    blend += fadeSpeed;

    if (blend > 1.0f) blend = 1.0f;

    // Update the active hex colour
    uint32_t newColour = lerpColour(startColor, targetColor, blend);
    hexColours[currentHex] = newColour;

    // ---------------------------------------------------------
    // WHEN FADE COMPLETES → PICK NEW HEX + NEW TARGET COLOUR
    // ---------------------------------------------------------
    if (blend >= 1.0f) {
        // Start next fade from the colour we just reached
        startColor = hexColours[currentHex];

        // Pick next hex (prioritise black ones)
        currentHex = pickNextHex();

        // New target colour
        targetColor = randomPaletteColour();

        blend = 0.0f;
    }

    // ---------------------------------------------------------
    // DRAW ALL HEXES FROM THEIR STORED COLOURS
    // ---------------------------------------------------------
    for (int h = 0; h < 9; h++) {
        fillHex(h, hexColours[h]);
    }
}
