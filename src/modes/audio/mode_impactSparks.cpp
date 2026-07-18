// /src/modes/audio/mode_impactSparks.cpp

#include "modes/audio/mode_impactSparks.h"
#include "audio_engine.h"
#include "utils.h"
#include "palette.h"
#include "hex_geometry.h"
#include <Arduino.h>

// =========================================================
// MODE 1 — IMPACT SPARKS
// Transient-driven bursts of colour across random LEDs.
// Energy smoothing + interval scaling tuned to original feel.
// =========================================================
static float impactEnergy = 0.0f;
static unsigned long lastSparkTime = 0;

void mode_impactSparks() {
    float energy = (audioBassRaw() + audioMidRaw() + audioTrebRaw())
                 / (BASS_MAX + MID_MAX + TREB_MAX);

    // Exponential smoothing of energy (matches original “laggy” feel)
    impactEnergy = impactEnergy * 0.90f + energy * 0.10f;

    // Dim purple background
    fillAll(dimColour(COL_PURPLE, 0.05f));

    // Spark rate based on energy (higher energy → more frequent sparks)
    unsigned long now = millis();
    int interval = 20 + (1 - impactEnergy) * 150;

    if (now - lastSparkTime > (unsigned long)interval) {
        lastSparkTime = now;
        int idx = random(NUM_LEDS);
        setLED(idx, randomPaletteColour());
    }
}
