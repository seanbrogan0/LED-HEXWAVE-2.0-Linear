// File: src/input.cpp

#include "input.h"
#include "hardware.h"
#include <EEPROM.h>

// =========================================================
// GLOBAL INPUT STATE
// =========================================================
int leftPotValue  = 0;
int rightPotValue = 0;

bool leftBtnPressed  = false;
bool rightBtnPressed = false;

unsigned long lastLeftBtnTime  = 0;
unsigned long lastRightBtnTime = 0;

// Debounce delay (matches original sketch)
const unsigned long debounceDelay = 200;

// Mode state (persisted via EEPROM)
bool audioModeEnabled   = false;
int  currentAudioMode   = 0;
int  currentNonAudioMode = 0;

// =========================================================
// EEPROM SAVE / LOAD
// =========================================================
void saveModeState() {
    EEPROM.write(0, audioModeEnabled ? 1 : 0);
    EEPROM.write(1, currentAudioMode);
    EEPROM.write(2, currentNonAudioMode);
    EEPROM.commit();
}

void loadModeState() {
    audioModeEnabled   = EEPROM.read(0) == 1;
    currentAudioMode   = EEPROM.read(1);
    currentNonAudioMode = EEPROM.read(2);
}

// =========================================================
// READ POTS
// =========================================================
void ReadPots() {
    leftPotValue  = analogRead(PIN_LEFTPOT);
    rightPotValue = analogRead(PIN_RIGHTPOT);
}

// =========================================================
// READ BUTTONS
// Original behaviour:
//  - LEFT button: cycle modes (audio or non-audio)
//  - RIGHT button: toggle audio mode on/off + update indicator LED
// =========================================================
void ReadButtons() {
    unsigned long now = millis();

    // LEFT BUTTON — cycle modes
    if (!digitalRead(PIN_LEFTBTN)) {  // active LOW
        if (!leftBtnPressed && (now - lastLeftBtnTime > debounceDelay)) {
            leftBtnPressed = true;
            lastLeftBtnTime = now;

            if (audioModeEnabled) {
                currentAudioMode = (currentAudioMode + 1) % 4;
            } else {
                currentNonAudioMode = (currentNonAudioMode + 1) % 4;
            }

            saveModeState();
        }
    } else {
        leftBtnPressed = false;
    }

    // RIGHT BUTTON — toggle audio mode + indicator LED
    if (!digitalRead(PIN_RIGHTBTN)) {  // active LOW
        if (!rightBtnPressed && (now - lastRightBtnTime > debounceDelay)) {
            rightBtnPressed = true;
            lastRightBtnTime = now;

            // Toggle audio / non-audio mode
            audioModeEnabled = !audioModeEnabled;

            // Indicator LED reflects audio mode state (ON = audio)
            digitalWrite(PIN_LED_IND, audioModeEnabled ? HIGH : LOW);

            saveModeState();
        }
    } else {
        rightBtnPressed = false;
    }
}
