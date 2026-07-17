// /src/mode_registry.cpp

#include "mode_registry.h"

// Audio modes
#include "modes/audio/mode_simpleAudioHex.h"
#include "modes/audio/mode_impactSparks.h"
#include "modes/audio/mode_audioHueDrift.h"
#include "modes/audio/mode_volumeSpread.h"

// Non-audio modes
#include "modes/non_audio/mode_solid.h"
#include "modes/non_audio/mode_perimeterPathBounce.h"
#include "modes/non_audio/mode_hexFadeRandom.h"
#include "modes/non_audio/mode_dualCometsHexPath.h"

// =========================================================
// MODE TABLES
// All modes on this fixture repaint or rely on persistence,
// so none use autoClear.
// =========================================================
static const ModeEntry audioModes[] = {
    { mode_simpleAudioHex,       false, "Simple Audio Hex" },
    { mode_impactSparks,         false, "Impact Sparks" },
    { mode_audioHueDrift,        false, "Audio Hue Drift" },
    { mode_volumeSpread,         false, "Volume Spread" },
};

static const ModeEntry nonAudioModes[] = {
    { mode_solid,                false, "Solid" },
    { mode_perimeterPathBounce,  false, "Perimeter Path Bounce" },
    { mode_hexFadeRandom,        false, "Hex Fade Random" },
    { mode_dualCometsHexPath,    false, "Dual Comets Hex Path" },
};

// =========================================================
// REGISTRY API
// =========================================================
int audioModeCount() {
    return sizeof(audioModes) / sizeof(audioModes[0]);
}

int nonAudioModeCount() {
    return sizeof(nonAudioModes) / sizeof(nonAudioModes[0]);
}

void runAudioMode(int index) {
    if (index < 0 || index >= audioModeCount()) return;
    audioModes[index].run();
}

void runNonAudioMode(int index) {
    if (index < 0 || index >= nonAudioModeCount()) return;
    nonAudioModes[index].run();
}

bool audioModeAutoClear(int index) {
    if (index < 0 || index >= audioModeCount()) return false;
    return audioModes[index].autoClear;
}

bool nonAudioModeAutoClear(int index) {
    if (index < 0 || index >= nonAudioModeCount()) return false;
    return nonAudioModes[index].autoClear;
}
