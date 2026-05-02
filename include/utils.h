// File: include/utils.h

#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "hardware.h"
#include "hex_mapping.h"

// =========================================================
// GENERIC HELPERS
// =========================================================

// Clamp integer
int clampInt(int v, int lo, int hi);

// Clamp float
float clampFloat(float v, float lo, float hi);

// Wrap index (positive modulo)
int wrapIndex(int i, int max);

// =========================================================
// LED HELPERS
// =========================================================

// Set LED colour safely (with bounds check)
void setLED(int idx, uint32_t colour);

// Fill entire strip
void fillAll(uint32_t colour);

// Fill a hex by index (0–8)
void fillHex(int hexIndex, uint32_t colour);

// Send colours to strip (global brightness applied)
void SendColours();
