// /src/modes/non_audio/mode_breathe.cpp

#include "modes/non_audio/mode_breathe.h"
#include "input.h"
#include "utils.h"
#include "palette.h"
#include <Arduino.h>
#include <math.h>

// =========================================================
// BREATHE MODE
// The whole fixture pulses a single palette colour in and
// out on a ~4 s sine. Left pot = colour selection.
// (Ported from the Flower fixture's Breathe.)
// =========================================================
void mode_breathe() {
    float t = millis() / 1000.0f;
    float breath = (sinf(t * 1.5f) + 1.0f) * 0.5f;  // 0.0–1.0 sine, ~4 s period

    int colIndex = (int)(modeEngine.leftPot() * (PALETTE_MASTER_SIZE - 1));
    fillAll(dimColour(PALETTE_MASTER[colIndex], breath));
}
