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
// Radial expansion from center based on amplitude.
// Nonlinear mapping restores original “punchy” feel.
// =========================================================
void mode_volumeSpread() {
    double bass, mid, treb;
    computeAudioBands(bass, mid, treb);

    float norm = (bass + mid + treb) / (BASS_MAX + MID_MAX + TREB_MAX);

    // Nonlinear curve to match original visual response
    float e = pow(norm, 0.6f);

    // Map energy to number of hexes lit
    int litHexes = e * 9;
    if (litHexes < 1) litHexes = 1;
    if (litHexes > 9) litHexes = 9;

    uint32_t c = colourFromHue(e * 255);

    for (int i = 0; i < 9; i++) {
        if (i < litHexes) {
            fillHex(i, c);
        } else {
            fillHex(i, dimColour(c, 0.05f));
        }
    }
}
