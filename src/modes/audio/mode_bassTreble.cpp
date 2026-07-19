// /src/modes/audio/mode_bassTreble.cpp

#include "modes/audio/mode_bassTreble.h"
#include "audio_engine.h"
#include "input.h"
#include "utils.h"
#include "palette.h"
#include "hex_geometry.h"
#include <Arduino.h>

// =========================================================
// BASS VS TREBLE MODE (dual band — mid is ignored)
// A bass front fills hex-by-hex from the spur end of the
// chain walk while a treble front fills from the hex-9 end;
// both fronts have a fractional frontier so they advance
// smoothly, and their colours blend where they overlap.
// Left pot = bass hue; treble gets the complementary hue.
// =========================================================
void mode_bassTreble() {
    float b = (float)audioBassNorm();
    float t = (float)audioTrebNorm();

    uint8_t bassHue = (uint8_t)(modeEngine.leftPot() * 255.0f);
    uint32_t bassCol = colourFromHue(bassHue);
    uint32_t trebCol = colourFromHue((uint8_t)(bassHue + 128));  // complement

    float bassFront = b * PATH_LEN_PERIM;   // advances from slot 0 (spur tip)
    float trebFront = t * PATH_LEN_PERIM;   // advances from slot 8 (hex 9)

    for (int i = 0; i < PATH_LEN_PERIM; i++) {
        float bl = clampFloat(bassFront - i, 0.0f, 1.0f);
        float tl = clampFloat(trebFront - (PATH_LEN_PERIM - 1 - i), 0.0f, 1.0f);

        // Blend the two colours weighted by each front's local level
        float sum = bl + tl;
        uint32_t mix = lerpColour32(bassCol, trebCol,
                                    sum > 0.0f ? tl / sum : 0.5f);

        float level = bl > tl ? bl : tl;
        fillHex(HEX_PATH_PERIM[i] - 1, dimColour(mix, 0.05f + 0.95f * level));
    }
}
