// /src/main.cpp

#include <Arduino.h>
#include <EEPROM.h>
#include "hardware.h"
#include "input.h"
#include "mode_registry.h"
#include "utils.h"
#include "audio_engine.h"   // <-- REQUIRED for updateFFT()

// =========================================================
// SETUP
// =========================================================
void setup() {
    Serial.begin(115200);

    // Button + indicator LED pins
    pinMode(PIN_LEFTBTN, INPUT_PULLUP);
    pinMode(PIN_RIGHTBTN, INPUT_PULLUP);
    pinMode(PIN_LED_IND, OUTPUT);

    // EEPROM for mode persistence
    EEPROM.begin(10);
    loadModeState();

    // Indicator LED reflects persisted audio mode state
    digitalWrite(PIN_LED_IND, audioModeEnabled ? HIGH : LOW);

    // NeoPixel strip init
    strip.begin();
    strip.show();
}

// =========================================================
// MAIN LOOP
// =========================================================
void loop() {
    // Read pots + buttons every frame
    ReadPots();
    ReadButtons();

    // Restore original brightness behaviour:
    // globalMaxBright is set BEFORE SendColours() and applied via strip.setBrightness()
    globalMaxBright = map(rightPotValue, 0, 4095, 0, 160);

    // AUDIO MODE
    if (audioModeEnabled) {

        // -------------------------------------------------
        // NEW: Update FFT BEFORE running the audio mode
        // -------------------------------------------------
        updateFFT();

        runAudioMode(currentAudioMode);
    }

    // NON-AUDIO MODE
    else {
        runNonAudioMode(currentNonAudioMode);
    }
// Debug: print current mode once per second
static unsigned long lastPrint = 0;
if (millis() - lastPrint > 1000) {
    lastPrint = millis();
    if (audioModeEnabled) {
        Serial.printf("Audio Mode: %d\n", currentAudioMode);
    } else {
        Serial.printf("Non-Audio Mode: %d\n", currentNonAudioMode);
    }
}
    // Push current frame to LEDs (uses globalMaxBright)
    SendColours();

    // Restore original Arduino loop pacing (~100 FPS, matches original sketch feel)
    delay(10);
}
