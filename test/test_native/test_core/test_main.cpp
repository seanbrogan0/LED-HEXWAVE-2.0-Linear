// test/test_native/test_core/test_main.cpp
// =========================================================
// Native unit tests for the Arduino-free hexcore library.
// Run with:  pio test -e native
// =========================================================

#include <unity.h>
#include "color_math.h"
#include "mode_state.h"
#include "hex_geometry.h"
#include "path_math.h"

void setUp() {}
void tearDown() {}

// =========================================================
// color_math
// =========================================================
void test_packColour_bit_layout() {
    TEST_ASSERT_EQUAL_HEX32(0x00FF8040u, packColour(0xFF, 0x80, 0x40));
    TEST_ASSERT_EQUAL_HEX32(0x00000000u, packColour(0, 0, 0));
    TEST_ASSERT_EQUAL_HEX32(0x00FFFFFFu, packColour(255, 255, 255));
}

void test_dimColour32_factors() {
    TEST_ASSERT_EQUAL_HEX32(0u, dimColour32(packColour(255, 255, 255), 0.0f));
    TEST_ASSERT_EQUAL_HEX32(packColour(255, 100, 0),
                            dimColour32(packColour(255, 100, 0), 1.0f));
    // Channels truncate, matching the modes' float→uint8 cast semantics
    TEST_ASSERT_EQUAL_HEX32(packColour(102, 40, 0),
                            dimColour32(packColour(255, 100, 0), 0.4f));
}

void test_lerpColour32_endpoints_and_clamping() {
    uint32_t a = packColour(10, 20, 30);
    uint32_t b = packColour(200, 100, 50);
    TEST_ASSERT_EQUAL_HEX32(a, lerpColour32(a, b, 0.0f));
    TEST_ASSERT_EQUAL_HEX32(b, lerpColour32(a, b, 1.0f));
    TEST_ASSERT_EQUAL_HEX32(a, lerpColour32(a, b, -5.0f));   // t clamps low
    TEST_ASSERT_EQUAL_HEX32(b, lerpColour32(a, b, 5.0f));    // t clamps high
    TEST_ASSERT_EQUAL_HEX32(packColour(127, 127, 127),
                            lerpColour32(0, packColour(255, 255, 255), 0.5f));
}

void test_clamp_helpers() {
    TEST_ASSERT_EQUAL(5, clampInt(5, 0, 10));
    TEST_ASSERT_EQUAL(0, clampInt(-3, 0, 10));
    TEST_ASSERT_EQUAL(10, clampInt(42, 0, 10));
    TEST_ASSERT_EQUAL_FLOAT(0.5f, clampFloat(0.5f, 0.0f, 1.0f));
    TEST_ASSERT_EQUAL_FLOAT(0.0f, clampFloat(-1.0f, 0.0f, 1.0f));
    TEST_ASSERT_EQUAL_FLOAT(1.0f, clampFloat(9.0f, 0.0f, 1.0f));
}

void test_wrapIndex() {
    TEST_ASSERT_EQUAL(5, wrapIndex(-1, 6));
    TEST_ASSERT_EQUAL(0, wrapIndex(6, 6));
    TEST_ASSERT_EQUAL(3, wrapIndex(3, 6));
}

// =========================================================
// mode_state
// =========================================================
static const ModeCounts COUNTS = { 4, 4 };

void test_cycle_wraps_and_only_touches_active_group() {
    ModeState s = { true, 3, 2 };
    modeStateCycle(s, COUNTS);
    TEST_ASSERT_EQUAL(0, s.audioMode);      // wrapped at count
    TEST_ASSERT_EQUAL(2, s.nonAudioMode);   // untouched

    s = { false, 1, 3 };
    modeStateCycle(s, COUNTS);
    TEST_ASSERT_EQUAL(1, s.audioMode);      // untouched
    TEST_ASSERT_EQUAL(0, s.nonAudioMode);   // wrapped at count
}

void test_toggle_flips_flag_only() {
    ModeState s = { false, 2, 1 };
    modeStateToggleAudio(s);
    TEST_ASSERT_TRUE(s.audioEnabled);
    TEST_ASSERT_EQUAL(2, s.audioMode);
    TEST_ASSERT_EQUAL(1, s.nonAudioMode);
    modeStateToggleAudio(s);
    TEST_ASSERT_FALSE(s.audioEnabled);
}

void test_reset_preserves_audioEnabled() {
    ModeState s = { true, 3, 2 };
    modeStateReset(s);
    TEST_ASSERT_TRUE(s.audioEnabled);
    TEST_ASSERT_EQUAL(0, s.audioMode);
    TEST_ASSERT_EQUAL(0, s.nonAudioMode);
}

void test_fromBytes_validates_garbage() {
    // Garbage EEPROM (e.g. factory 0xFF) must land on safe defaults
    ModeState s = modeStateFromBytes(255, 255, 7, COUNTS);
    TEST_ASSERT_FALSE(s.audioEnabled);      // only exactly 1 means enabled
    TEST_ASSERT_EQUAL(0, s.audioMode);
    TEST_ASSERT_EQUAL(0, s.nonAudioMode);
}

void test_fromBytes_keeps_valid_state() {
    ModeState s = modeStateFromBytes(1, 3, 2, COUNTS);
    TEST_ASSERT_TRUE(s.audioEnabled);
    TEST_ASSERT_EQUAL(3, s.audioMode);
    TEST_ASSERT_EQUAL(2, s.nonAudioMode);
}

// =========================================================
// hex_geometry (data integrity for THIS fixture)
// =========================================================
void test_hexes_cover_every_led_exactly_once() {
    const int TOTAL = NUM_HEXES * HEX_LED_COUNT;
    bool seen[NUM_HEXES * HEX_LED_COUNT] = { false };

    for (int h = 0; h < NUM_HEXES; h++) {
        for (int i = 0; i < HEX_LED_COUNT; i++) {
            int idx = HEXES[h][i];
            TEST_ASSERT_TRUE_MESSAGE(idx >= 0 && idx < TOTAL, "LED index out of range");
            TEST_ASSERT_FALSE_MESSAGE(seen[idx], "LED index appears twice");
            seen[idx] = true;
        }
    }
    for (int i = 0; i < TOTAL; i++) {
        TEST_ASSERT_TRUE_MESSAGE(seen[i], "LED not covered by any hex");
    }
}

void test_path_entries_reference_real_hexes() {
    for (int i = 0; i < PATH_LEN_COMETS; i++) {
        TEST_ASSERT_TRUE(HEX_PATH_COMETS[i] >= 1 && HEX_PATH_COMETS[i] <= NUM_HEXES);
    }
    for (int i = 0; i < PATH_LEN_PERIM; i++) {
        TEST_ASSERT_TRUE(HEX_PATH_PERIM[i] >= 1 && HEX_PATH_PERIM[i] <= NUM_HEXES);
    }
}

// =========================================================
// path_math
// =========================================================
void test_pathPosToLED_hex_boundaries() {
    // Position 0 is the first LED of the path's first hex (HEX1)
    TEST_ASSERT_EQUAL(HEX1[0],
        pathPosToLED(0, HEX_PATH_COMETS, PATH_LEN_COMETS, HEXES, HEX_LED_COUNT));
    // Position 23 is the last LED of HEX1
    TEST_ASSERT_EQUAL(HEX1[HEX_LED_COUNT - 1],
        pathPosToLED(HEX_LED_COUNT - 1, HEX_PATH_COMETS, PATH_LEN_COMETS, HEXES, HEX_LED_COUNT));
    // Position 24 crosses into the path's second hex (HEX2)
    TEST_ASSERT_EQUAL(HEX2[0],
        pathPosToLED(HEX_LED_COUNT, HEX_PATH_COMETS, PATH_LEN_COMETS, HEXES, HEX_LED_COUNT));
}

void test_pathPosToLED_revisited_hex() {
    // The comets path {1,2,3,2,...} deliberately revisits HEX2 at slot 3
    TEST_ASSERT_EQUAL(HEX2[0],
        pathPosToLED(3 * HEX_LED_COUNT, HEX_PATH_COMETS, PATH_LEN_COMETS, HEXES, HEX_LED_COUNT));
}

void test_wrapPathPos_seam() {
    const int TOTAL = PATH_LEN_COMETS * HEX_LED_COUNT;
    TEST_ASSERT_EQUAL(TOTAL - 1, wrapPathPos(-1, TOTAL));
    TEST_ASSERT_EQUAL(0, wrapPathPos(TOTAL, TOTAL));
    TEST_ASSERT_EQUAL(5, wrapPathPos(5, TOTAL));
    // A comet tail 10 LEDs behind a head at 0 wraps to the far end
    TEST_ASSERT_EQUAL(TOTAL - 10, wrapPathPos(0 - 10, TOTAL));
}

// =========================================================
// RUNNER
// =========================================================
int main(int, char**) {
    UNITY_BEGIN();

    RUN_TEST(test_packColour_bit_layout);
    RUN_TEST(test_dimColour32_factors);
    RUN_TEST(test_lerpColour32_endpoints_and_clamping);
    RUN_TEST(test_clamp_helpers);
    RUN_TEST(test_wrapIndex);

    RUN_TEST(test_cycle_wraps_and_only_touches_active_group);
    RUN_TEST(test_toggle_flips_flag_only);
    RUN_TEST(test_reset_preserves_audioEnabled);
    RUN_TEST(test_fromBytes_validates_garbage);
    RUN_TEST(test_fromBytes_keeps_valid_state);

    RUN_TEST(test_hexes_cover_every_led_exactly_once);
    RUN_TEST(test_path_entries_reference_real_hexes);

    RUN_TEST(test_pathPosToLED_hex_boundaries);
    RUN_TEST(test_pathPosToLED_revisited_hex);
    RUN_TEST(test_wrapPathPos_seam);

    return UNITY_END();
}
