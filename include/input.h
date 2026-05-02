// File: include/input.h

#pragma once
#include <Arduino.h>

// =========================================================
// BUTTON + POT GLOBALS
// =========================================================

// Raw pot values (0–4095 on ESP32 ADC)
extern int leftPotValue;
extern int rightPotValue;

// Button states
extern bool leftBtnPressed;
extern bool rightBtnPressed;

// Debounce timing
extern unsigned long lastLeftBtnTime;
extern unsigned long lastRightBtnTime;
extern const unsigned long debounceDelay;

// =========================================================
// MODE STATE
// =========================================================

// Audio / non‑audio mode flag
extern bool audioModeEnabled;

// Current mode indices
extern int currentAudioMode;
extern int currentNonAudioMode;

// EEPROM persistence
void saveModeState();
void loadModeState();

// =========================================================
// INPUT PROCESSING
// =========================================================
void ReadPots();
void ReadButtons();
