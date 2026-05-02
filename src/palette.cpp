// /src/palette.cpp

#include "palette.h"
#include "hardware.h"

// =========================================================
// BASE COLOUR CONSTANTS
// =========================================================
const uint32_t COL_RED    = strip.Color(255,   0,   0);   // Red
const uint32_t COL_ORANGE = strip.Color(255, 120,   0);   // Orange
const uint32_t COL_YELLOW = strip.Color(255, 255,   0);   // Yellow
const uint32_t COL_GREEN  = strip.Color(  0, 255,   0);   // Green
const uint32_t COL_CYAN   = strip.Color(  0, 255, 255);   // Cyan
const uint32_t COL_BLUE   = strip.Color(  0,   0, 255);   // Blue
const uint32_t COL_PURPLE = strip.Color(180,   0, 255);   // Purple
const uint32_t COL_WHITE  = strip.Color(255, 255, 255);   // White

// =========================================================
// MASTER PALETTE (15 COLOURS FROM ORIGINAL SKETCH)
// =========================================================
const uint32_t PALETTE_MASTER[] = {
    strip.Color(255,   0,   0),   // Red
    strip.Color(255,  20, 147),   // Deep Pink
    strip.Color(  0, 255, 255),   // Cyan
    strip.Color( 57, 255,  20),   // Neon Green
    strip.Color(255, 255,   0),   // Yellow
    strip.Color(255,  80,   0),   // Orange
    strip.Color(  0, 120, 255),   // Sky Blue
    strip.Color(  0, 255, 140),   // Mint
    strip.Color(120,   0, 255),   // Purple
    strip.Color(  0,  60, 255),   // Royal Blue
    strip.Color(200, 220,  40),   // Lime Yellow
    strip.Color(255,   0, 110),   // Hot Pink
    strip.Color(180,   0, 255),   // Violet
    strip.Color(  0, 255, 200),   // Aqua
    strip.Color(255, 100,   0)    // Amber
};

const int PALETTE_MASTER_SIZE =
    sizeof(PALETTE_MASTER) / sizeof(PALETTE_MASTER[0]);

// =========================================================
// HUE → COLOUR (0–255)
// =========================================================
uint32_t colourFromHue(uint8_t hue) {
    return strip.ColorHSV((uint16_t)hue * 256, 255, 255);
}

// =========================================================
// DIM COLOUR
// =========================================================
uint32_t dimColour(uint32_t c, float factor) {
    uint8_t r = (uint8_t)(((c >> 16) & 0xFF) * factor);
    uint8_t g = (uint8_t)(((c >> 8)  & 0xFF) * factor);
    uint8_t b = (uint8_t)(( c        & 0xFF) * factor);
    return strip.Color(r, g, b);
}

// =========================================================
// RANDOM PALETTE COLOUR
// =========================================================
uint32_t randomPaletteColour() {
    return PALETTE_MASTER[random(PALETTE_MASTER_SIZE)];
}
