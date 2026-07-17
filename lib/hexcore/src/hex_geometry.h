// hexcore/hex_geometry.h
// =========================================================
// Physical LED geometry for THIS fixture: 9 hex panels in a
// line, 24 LEDs each, 216 LEDs total. Pure data, no Arduino.
//
// Each HEXn array lists strip indices in the panel's logical
// rotational order (verified against the physical wiring).
// =========================================================

#pragma once
#include <stdint.h>

constexpr int NUM_HEXES     = 9;
constexpr int HEX_LED_COUNT = 24;

extern const uint16_t HEX1[];
extern const uint16_t HEX2[];
extern const uint16_t HEX3[];
extern const uint16_t HEX4[];
extern const uint16_t HEX5[];
extern const uint16_t HEX6[];
extern const uint16_t HEX7[];
extern const uint16_t HEX8[];
extern const uint16_t HEX9[];

// Master array for iteration (index 0 = HEX1)
extern const uint16_t* const HEXES[NUM_HEXES];

// =========================================================
// PATHS for path-travelling modes (1-based hex numbers)
// =========================================================

// Dual-comets custom path (revisits hex 2 by design)
extern const int HEX_PATH_COMETS[];
constexpr int PATH_LEN_COMETS = 10;

// Perimeter bounce path (straight run 1 → 9)
extern const int HEX_PATH_PERIM[];
constexpr int PATH_LEN_PERIM = 9;
