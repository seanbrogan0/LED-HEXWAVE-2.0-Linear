// /src/modes/non_audio/mode_perimeterPathBounce.cpp

#include "modes/non_audio/mode_perimeterPathBounce.h"
#include "input.h"
#include "utils.h"
#include "hex_mapping.h"
#include "palette.h"
#include "hardware.h"
#include <Arduino.h>

// =========================================================
// PERIMETER PATH (ORIGINAL ORDER 1→9)
// =========================================================
static int HEX_PATH_PERIM[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
static const int PATH_LEN_PERIM = 9;

// =========================================================
// PERIMETER PATH BOUNCE MODE
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
    // LEFT POT = SPEED CONTROL
    // ---------------------------------------------------------
    int speed = map(leftPotValue, 0, 4095, 400, 50);
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
        if (edgeIndex >= HEX_SIZE) {
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
