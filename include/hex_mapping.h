// File: include/hex_mapping.h

#pragma once
#include <Arduino.h>

// =========================================================
// HEX LED MAPPING
// (All arrays preserved exactly from your original sketch)
// =========================================================

// Individual hex LED index lists
extern const uint16_t HEX1[];
extern const uint16_t HEX2[];
extern const uint16_t HEX3[];
extern const uint16_t HEX4[];
extern const uint16_t HEX5[];
extern const uint16_t HEX6[];
extern const uint16_t HEX7[];
extern const uint16_t HEX8[];
extern const uint16_t HEX9[];

// Sizes of each hex
extern const uint16_t HEX_SIZE;

// Master array of pointers to each hex
extern const uint16_t* HEXES[9];
