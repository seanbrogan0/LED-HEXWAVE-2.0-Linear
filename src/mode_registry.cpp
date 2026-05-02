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

// Function pointer type for modes
typedef void (*ModeFunc)();

// Audio mode table
static ModeFunc audioModes[] = {
    mode_simpleAudioHex,
    mode_impactSparks,
    mode_audioHueDrift,
    mode_volumeSpread
};

// Non-audio mode table
static ModeFunc nonAudioModes[] = {
    mode_solid,
    mode_perimeterPathBounce,
    mode_hexFadeRandom,
    mode_dualCometsHexPath
};

void runAudioMode(int index) {
    if (index < 0) return;
    int count = sizeof(audioModes) / sizeof(audioModes[0]);
    if (index >= count) return;
    audioModes[index]();
}

void runNonAudioMode(int index) {
    if (index < 0) return;
    int count = sizeof(nonAudioModes) / sizeof(nonAudioModes[0]);
    if (index >= count) return;
    nonAudioModes[index]();
}
