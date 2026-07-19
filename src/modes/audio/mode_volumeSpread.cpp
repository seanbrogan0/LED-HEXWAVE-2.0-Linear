// /src/modes/audio/mode_volumeSpread.cpp

#include "modes/audio/mode_volumeSpread.h"
#include "audio_engine.h"
#include "utils.h"
#include "palette.h"
#include "hex_geometry.h"
#include <Arduino.h>
#include <math.h>

// =========================================================
// MODE 3 — VOLUME SPREAD
// Radial expansion from the physical centre of the chain
// (hex 5, always lit) outward through SPREAD_RINGS:
//   {5} → {4,6} → {1,7} → {2,8} → {3,9}
// The frontier ring fades in fractionally, so the spread
// breathes smoothly instead of stepping hex-by-hex.
// Nonlinear mapping restores original “punchy” feel.
// =========================================================
void mode_volumeSpread() {
    float norm = (audioBassRaw() + audioMidRaw() + audioTrebRaw())
               / (BASS_MAX + MID_MAX + TREB_MAX);

    // Nonlinear curve to match original visual response
    float e = pow(norm, 0.6f);

    // Map energy to a ring radius (0 = centre only, 4 = all rings)
    float r = e * (NUM_RINGS - 1);

    uint32_t c = colourFromHue(e * 255);

    for (int k = 0; k < NUM_RINGS; k++) {
        // Ring 0 (hex 5) is always fully lit; each outer ring fades
        // in fractionally as the radius crosses it.
        float level = (k == 0) ? 1.0f : clampFloat(r - (k - 1), 0.0f, 1.0f);
        uint32_t ringCol = dimColour(c, 0.05f + 0.95f * level);

        fillHex(SPREAD_RINGS[k][0] - 1, ringCol);
        fillHex(SPREAD_RINGS[k][1] - 1, ringCol);
    }
}
