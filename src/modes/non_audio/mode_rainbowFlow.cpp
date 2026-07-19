// /src/modes/non_audio/mode_rainbowFlow.cpp

#include "modes/non_audio/mode_rainbowFlow.h"
#include "input.h"
#include "utils.h"
#include "palette.h"
#include "hex_geometry.h"
#include "path_math.h"
#include <Arduino.h>
#include <math.h>

// =========================================================
// RAINBOW FLOW MODE
// One full hue cycle spread per-LED along the chain walk
// (HEX_PATH_PERIM, 3 → 2 → 1 → 4 → … → 9), scrolling
// continuously so colours travel through the sculpture.
// LEDs inside each hex follow the wiring-corrected logical
// order via pathPosToLED. Left pot = scroll speed.
// =========================================================

static const int TOTAL = PATH_LEN_PERIM * HEX_LED_COUNT;

void mode_rainbowFlow() {
    // Time-based offset (frame-rate independent): left pot maps to
    // 10–130 hue units/s, i.e. a full cycle every ~26 s down to ~2 s.
    static float offset = 0.0f;
    static unsigned long lastMs = 0;

    unsigned long now = millis();
    if (lastMs == 0) lastMs = now;
    float speed = 10.0f + modeEngine.leftPot() * 120.0f;
    offset = fmodf(offset + (now - lastMs) * speed / 1000.0f, 256.0f);
    lastMs = now;

    for (int p = 0; p < TOTAL; p++) {
        uint8_t hue = (uint8_t)(p * 256 / TOTAL + (int)offset);
        int led = pathPosToLED(p, HEX_PATH_PERIM, PATH_LEN_PERIM,
                               HEXES, HEX_LED_COUNT);
        setLED(led, colourFromHue(hue));
    }
}
