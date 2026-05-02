// /src/modes/audio/mode_audioHueDrift.cpp

#include "modes/audio/mode_audioHueDrift.h"
#include "audio_engine.h"
#include "utils.h"
#include "palette.h"
#include "hex_mapping.h"
#include <Arduino.h>

// =========================================================
// MODE 2 — AUDIO HUE DRIFT
// Hue rotates slowly; brightness driven by audio energy.
// Drift speed + cadence tuned to original loop timing.
// =========================================================
static uint8_t driftHue = 0;

void mode_audioHueDrift() {
    // Slow hue drift (decoupled from raw loop speed)
    static unsigned long lastHue = 0;
    unsigned long now = millis();
    if (now - lastHue > 30) {  // ~33 Hz hue update
        driftHue++;
        lastHue = now;
    }

    double bass, mid, treb;
    computeAudioBands(bass, mid, treb);

    float energy = (bass + mid + treb) / (BASS_MAX + MID_MAX + TREB_MAX);

    uint32_t c = colourFromHue(driftHue);
    fillAll(dimColour(c, energy));
}
