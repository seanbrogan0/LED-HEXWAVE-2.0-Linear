// /src/modes/non_audio/mode_chase.cpp

#include "modes/non_audio/mode_chase.h"
#include "input.h"
#include "utils.h"
#include "palette.h"
#include "hex_geometry.h"
#include <Arduino.h>

// =========================================================
// CHASE MODE
// One hex lights up at a time, running the physical chain
// walk (HEX_PATH_PERIM, 3 → 2 → 1 → 4 → … → 9) and looping.
// Palette colour advances each lap. Left pot = speed.
// (Ported from the Flower fixture's Chase.)
// =========================================================
static unsigned long lastChase = 0;
static int chaseSlot = 0;
static int chaseColor = 0;

void mode_chase() {
    float s = modeEngine.leftPot();
    unsigned long interval = (unsigned long)(600.0f - s * 550.0f);  // 50–600 ms

    if (millis() - lastChase >= interval) {
        lastChase = millis();
        chaseSlot = (chaseSlot + 1) % PATH_LEN_PERIM;
        if (chaseSlot == 0) {
            chaseColor = (chaseColor + 1) % PALETTE_MASTER_SIZE;
        }
    }

    for (int i = 0; i < PATH_LEN_PERIM; i++) {
        int hex = HEX_PATH_PERIM[i] - 1;
        if (i == chaseSlot) {
            fillHex(hex, PALETTE_MASTER[chaseColor]);
        } else {
            clearHex(hex);
        }
    }
}
