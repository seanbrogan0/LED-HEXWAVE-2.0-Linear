// /src/modes/audio/mode_beatWave.cpp

#include "modes/audio/mode_beatWave.h"
#include "audio_engine.h"
#include "input.h"
#include "utils.h"
#include "palette.h"
#include "hex_geometry.h"
#include <Arduino.h>

// =========================================================
// BEAT WAVE MODE
// A bass hit launches a wave from the chain's physical
// centre (hex 5) expanding outward through SPREAD_RINGS:
//   {5} → {4,6} → {1,7} → {2,8} → {3,9}
// The wavefront ring is full colour, the ring behind it dims
// to 35%, older rings clear; after the outermost ring fades
// the mode returns to idle. Left pot = colour.
// (The Flower fixture's Audio Ripple generalised to 5 rings.)
// =========================================================
static int waveFront = -1;          // -1 = idle, else current front ring
static float wavePrevBass = 0.0f;
static unsigned long lastWaveStep = 0;

void mode_beatWave() {
    float bN = (float)audioBassNorm();

    // Trigger a new wave on a bass spike (only when idle)
    if (waveFront < 0 && bN > wavePrevBass + 0.15f && bN > 0.3f) {
        waveFront = 0;
        lastWaveStep = millis();
    }
    wavePrevBass = bN;

    // Advance the wavefront every 120 ms; one extra state lets the
    // outermost ring fade before going idle
    if (waveFront >= 0 && millis() - lastWaveStep >= 120) {
        lastWaveStep = millis();
        waveFront++;
        if (waveFront > NUM_RINGS) waveFront = -1;
    }

    int colIndex = (int)(modeEngine.leftPot() * (PALETTE_MASTER_SIZE - 1));
    uint32_t col = PALETTE_MASTER[colIndex];
    uint32_t dimCol = dimColour(col, 0.35f);

    for (int h = 0; h < NUM_HEXES; h++) clearHex(h);

    if (waveFront >= 0) {
        if (waveFront < NUM_RINGS) {
            fillHex(SPREAD_RINGS[waveFront][0] - 1, col);
            fillHex(SPREAD_RINGS[waveFront][1] - 1, col);
        }
        int behind = waveFront - 1;
        if (behind >= 0 && behind < NUM_RINGS) {
            fillHex(SPREAD_RINGS[behind][0] - 1, dimCol);
            fillHex(SPREAD_RINGS[behind][1] - 1, dimCol);
        }
    }
}
