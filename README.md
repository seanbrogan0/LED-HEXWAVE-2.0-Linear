# LED HexWave 2.0 — Linear (9-hex fixture)

ESP32 firmware for a light sculpture of **9 hexagonal LED panels arranged in a line**
(24 WS2812 LEDs each, 216 total), with two buttons, two pots, and a microphone for
audio-reactive modes.

> Sister project: **LED-HEXWAVE-2.0-Flower** — the same architecture driving a
> 7-hex flower layout (center hex + ring of 6). The two repos are structurally
> identical except for `include/board_config.h`, `lib/hexcore/src/hex_geometry.*`,
> and their mode sets.

## Hardware

| Item | Value |
|---|---|
| Board | Arduino Nano ESP32 (`arduino_nano_esp32`) |
| LEDs | 216 × WS2812 (GRB, 800 kHz), data on pin **17** |
| Left button | pin **4** (to GND, internal pull-up) |
| Right button | pin **7** (to GND, internal pull-up) |
| Audio-mode indicator LED | pin **6** |
| Left pot | **A3** |
| Right pot | **A2** |
| Microphone | **A0** |

Brightness is capped at **160/255** at show time (power budget) — see
`MAX_BRIGHTNESS` in `include/board_config.h`.

## Controls

- **Left button** — short press: next mode in the active group; long press (≥ 600 ms):
  reset both groups to mode 0.
- **Right button** — toggle between audio and non-audio mode groups. The indicator
  LED is lit while audio mode is active.
- **Left pot** — per-mode parameter (speed, colour, density…).
- **Right pot** — global brightness, with a 2% floor so the strip never looks dead.

The active group and both mode indices persist in EEPROM across power cycles and
are validated against the mode registry on boot, so stale or garbage values can
never select a nonexistent mode.

## Modes

| # | Non-audio | Left pot |
|---|---|---|
| 0 | **Solid** — whole strip one colour; sweeps hue 0–340°, desaturates to white at the top | colour |
| 1 | **Perimeter Path Bounce** — a pixel draws through hexes 1→9 and bounces back, new colour each bounce | speed |
| 2 | **Hex Fade Random** — one hex at a time lerps to a new palette colour, unlit hexes first | fade speed |
| 3 | **Dual Comets Hex Path** — two comets with exponential tails on a custom path | speed |

| # | Audio | Behaviour |
|---|---|---|
| 0 | **Simple Audio Hex** — bass→hexes 1–3 (red), mid→4–6 (green), treble→7–9 (blue) | |
| 1 | **Impact Sparks** — spark rate follows smoothed audio energy over a dim purple bed | |
| 2 | **Audio Hue Drift** — hue rotates; brightness follows total energy | |
| 3 | **Volume Spread** — energy lights 1–9 hexes with a punchy `x^0.6` curve | |

## Architecture

```
include/
  board_config.h      all pins + per-fixture constants (the ONLY hardware config point)
  hardware.h          the NeoPixel strip + init
  input.h             ModeEngine: buttons, pots, EEPROM persistence
  audio_engine.h      timed 8 kHz FFT sampling + cached band getters
  palette.h utils.h   colours and LED helpers
  mode_registry.h     function-pointer mode tables (owns the mode counts)
  modes/**            one header per mode
lib/hexcore/          Arduino-free pure logic: colour math, mode-state machine,
                      hex geometry, path math — natively unit-testable
src/                  implementations, one .cpp per mode under src/modes/**
test/test_native/     Unity tests for hexcore (run on your computer)
test/test_hardware/   manual visual harness (flash to the board)
```

The main loop is ~60 lines: pace the frame (`FRAME_INTERVAL_MS`), update inputs,
apply brightness, run `audioUpdate()` once per frame when in audio mode, dispatch
the current mode through the registry, show. Each registry entry carries an
`autoClear` flag deciding whether the loop wipes the strip before the mode draws —
on this fixture all modes manage their own pixels (trails depend on persistence),
so all entries are `false`.

### Adding a mode

1. Create `include/modes/<group>/mode_myMode.h` declaring `void mode_myMode();`
2. Create `src/modes/<group>/mode_myMode.cpp` implementing it
3. Add one `{ mode_myMode, autoClear, "My Mode" }` line to the table in
   `src/mode_registry.cpp`

Mode counts, button cycling, and EEPROM validation pick it up automatically.

### EEPROM layout

| Byte | Meaning |
|---|---|
| 0 | audio mode enabled (1 = yes) |
| 1 | current audio mode index |
| 2 | current non-audio mode index |

## Build & test

```bash
pio run                        # compile firmware
pio run -t upload              # flash the board
pio test -e native             # run hexcore unit tests on your computer
pio test -e arduino_nano_esp32 -f test_hardware   # flash the visual harness
```

## Behaviour notes

- Audio sampling is paced to a true 8 kHz (256 samples ≈ 32 ms per frame), so
  audio modes run at ~23 fps and band energies may differ slightly from the old
  free-running sampler; retune `BASS_MAX`/`MID_MAX`/`TREB_MAX` in
  `src/audio_engine.cpp` on hardware if needed.
- Mode cycling fires on button **release**; a hold ≥ 600 ms is the reset gesture.
- Pots are IIR-smoothed (α = 0.1), adding a slight, deliberate lag to knob moves.
