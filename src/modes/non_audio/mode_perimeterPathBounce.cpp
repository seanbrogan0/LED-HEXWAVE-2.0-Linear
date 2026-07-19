// /src/modes/non_audio/mode_perimeterPathBounce.cpp

#include "modes/non_audio/mode_perimeterPathBounce.h"
#include "input.h"
#include "utils.h"
#include "hex_geometry.h"
#include "palette.h"
#include "hardware.h"
#include <Arduino.h>

// =========================================================
// PERIMETER PATH BOUNCE MODE
// Path data (HEX_PATH_PERIM, the physical chain walk
// 3 → 2 → 1 → 4 → … → 9) lives in hex_geometry; the bounce
// reverses at the chain's two true endpoints (spur tip, hex 9).
// =========================================================
void mode_perimeterPathBounce() {
    static int hexIndex = 0;
    static int edgeIndex = 0;
    static int direction = 1;

    static bool firstRun = true;
    static uint32_t currentColor;
    static int lastColorIndex = -1;

    // ---------------------------------------------------------
    // INITIAL COLOUR SELECTION (NOT SAME AS LAST)
    // ---------------------------------------------------------
    if (firstRun) {
        int idx;
        if (lastColorIndex < 0) {
            idx = random(PALETTE_MASTER_SIZE);
        } else {
            do {
                idx = random(PALETTE_MASTER_SIZE);
            } while (idx == lastColorIndex);
        }
        currentColor = PALETTE_MASTER[idx];
        lastColorIndex = idx;
        firstRun = false;
    }

    // ---------------------------------------------------------
    // LEFT POT = SPEED (same 400 → 50 ms curve as the raw-pot map)
    // ---------------------------------------------------------
    int speed = (int)(400.0f - modeEngine.leftPot() * 350.0f);
    static unsigned long lastMove = 0;

    if (millis() - lastMove >= (unsigned long)speed) {
        lastMove = millis();

        int hexNum = HEX_PATH_PERIM[hexIndex] - 1;
        int ledIndex = HEXES[hexNum][edgeIndex];

        // Draw pixel (no clearing → tail persists)
        setLED(ledIndex, currentColor);

        // Advance along edge
        edgeIndex++;

        // -----------------------------------------------------
        // END OF HEX → MOVE TO NEXT / BOUNCE
        // -----------------------------------------------------
        if (edgeIndex >= HEX_LED_COUNT) {
            edgeIndex = 0;
            hexIndex += direction;

            // Bounce at far end
            if (hexIndex >= PATH_LEN_PERIM) {
                hexIndex = PATH_LEN_PERIM - 1;
                direction = -1;

                int newIndex;
                do {
                    newIndex = random(PALETTE_MASTER_SIZE);
                } while (newIndex == lastColorIndex);

                currentColor = PALETTE_MASTER[newIndex];
                lastColorIndex = newIndex;
            }
            // Bounce at near end
            else if (hexIndex < 0) {
                hexIndex = 0;
                direction = +1;

                int newIndex;
                do {
                    newIndex = random(PALETTE_MASTER_SIZE);
                } while (newIndex == lastColorIndex);

                currentColor = PALETTE_MASTER[newIndex];
                lastColorIndex = newIndex;
            }
        }
    }
}
