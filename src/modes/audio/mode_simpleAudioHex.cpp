// /src/modes/audio/mode_simpleAudioHex.cpp

#include "modes/audio/mode_simpleAudioHex.h"
#include "audio_engine.h"
#include "utils.h"
#include "palette.h"
#include "hex_geometry.h"

// =========================================================
// MODE 0 — SIMPLE AUDIO HEX
// Bass → Hex 1–3, Mid → Hex 4–6, Treb → Hex 7–9
// Matches original sketch mapping and scaling.
// =========================================================
void mode_simpleAudioHex() {
    double bass, mid, treb;
    computeAudioBands(bass, mid, treb);

    float b = bass / BASS_MAX;
    float m = mid  / MID_MAX;
    float t = treb / TREB_MAX;

    // Bass hexes
    fillHex(0, dimColour(COL_RED, b));
    fillHex(1, dimColour(COL_RED, b));
    fillHex(2, dimColour(COL_RED, b));

    // Mid hexes
    fillHex(3, dimColour(COL_GREEN, m));
    fillHex(4, dimColour(COL_GREEN, m));
    fillHex(5, dimColour(COL_GREEN, m));

    // Treble hexes
    fillHex(6, dimColour(COL_BLUE, t));
    fillHex(7, dimColour(COL_BLUE, t));
    fillHex(8, dimColour(COL_BLUE, t));
}
