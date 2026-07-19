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
//
// On the wall the panels form a staggered chain, and hex 3
// is a short spur off hex 2 (hexes 1 and 4 touch). Starting
// at the spur tip gives a walk that visits every hex exactly
// once with each consecutive pair physically adjacent:
//   3 → 2 → 1 → 4 → 5 → 6 → 7 → 8 → 9
// =========================================================

// Dual-comets path: the chain walk, looped one-way ad
// infinitum (comets wrap 9 → 3; no return leg)
extern const int HEX_PATH_COMETS[];
constexpr int PATH_LEN_COMETS = 9;

// Perimeter bounce path (same walk; both ends are true
// endpoints of the physical chain — spur tip and hex 9)
extern const int HEX_PATH_PERIM[];
constexpr int PATH_LEN_PERIM = 9;

// =========================================================
// CENTRE-OUT RINGS for spread/wave modes (1-based hex numbers)
// Hex 5 is the middle of the chain walk; each ring k holds the
// two hexes k steps away along the walk (ring 0 duplicates the
// centre so every ring iterates uniformly as a pair):
//   {5,5} {4,6} {1,7} {2,8} {3,9}
// The spur tip (3) and hex 9 — the chain's two true endpoints —
// sit in the outermost ring.
// =========================================================
constexpr int NUM_RINGS = 5;
extern const int SPREAD_RINGS[NUM_RINGS][2];
