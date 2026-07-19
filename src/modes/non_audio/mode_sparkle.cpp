// /src/modes/non_audio/mode_sparkle.cpp

#include "modes/non_audio/mode_sparkle.h"
#include "board_config.h"
#include "input.h"
#include "utils.h"
#include "palette.h"
#include "hardware.h"
#include <Arduino.h>

// =========================================================
// SPARKLE MODE
// Random LEDs light up in random palette colours and fade
// out over a few hundred ms. Left pot = spark density.
// This fixture never auto-clears, so the fade works directly
// on the live strip — no private buffer needed.
// (Ported from the Flower fixture's Sparkle.)
// =========================================================

// Hardware-tunable: per-frame fade at the 10 ms pace (~200 ms tail)
static const float SPARKLE_FADE = 0.95f;
// Hardware-tunable: spark spawn rate range in sparks/second
static const float SPARK_RATE_MIN = 10.0f;
static const float SPARK_RATE_MAX = 200.0f;

static float sparkAcc = 0.0f;

void mode_sparkle() {
    // Fade all existing sparks
    for (int i = 0; i < NUM_LEDS; i++) {
        uint32_t c = strip.getPixelColor(i);
        if (c) {
            strip.setPixelColor(i, dimColour(c, SPARKLE_FADE));
        }
    }

    // Spawn new sparks at a pot-scaled rate (accumulator keeps the
    // fractional remainder so low rates still fire)
    float rate = SPARK_RATE_MIN
               + modeEngine.leftPot() * (SPARK_RATE_MAX - SPARK_RATE_MIN);
    sparkAcc += rate * (FRAME_INTERVAL_MS / 1000.0f);
    while (sparkAcc >= 1.0f) {
        sparkAcc -= 1.0f;
        setLED(random(NUM_LEDS), randomPaletteColour());
    }
}
