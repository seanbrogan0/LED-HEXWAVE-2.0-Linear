# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

ESP32 (Arduino Nano ESP32) firmware for a light sculpture: 9 hexagonal LED panels in a staggered chain (hex 3 is a short spur off hex 2; hexes 1 and 4 are adjacent — the continuous physical walk is 3→2→1→4→5→6→7→8→9), 24 WS2812 LEDs each (216 total), with two buttons, two pots, and a microphone for audio-reactive modes. Built with PlatformIO.

Sister repo **LED-HEXWAVE-2.0-Flower** shares this exact architecture (7-hex flower layout); the two differ only in `include/board_config.h`, `lib/hexcore/src/hex_geometry.*`, and their mode sets. Keep structural changes portable between the two.

## Commands

```bash
pio run                        # compile firmware
pio run -t upload              # flash the board
pio test -e native             # run hexcore unit tests on the host (no hardware needed)
pio test -e arduino_nano_esp32 -f test_hardware   # flash the on-device visual harness
```

The native test suite is a single Unity binary (`test/test_native/test_core/test_main.cpp`); there is no finer-grained single-test filter than `-f test_core`.

## Architecture

The codebase is split along an Arduino/no-Arduino boundary:

- **`lib/hexcore/`** — pure logic with no Arduino dependency, so it compiles and tests natively (`platform = native`, gnu++17): colour math (`color_math`), the mode-state machine incl. EEPROM-value validation (`mode_state`), physical LED index maps, chain-walk paths, and the centre-out `SPREAD_RINGS` table (`hex_geometry`), and path traversal math (`path_math`). New logic that doesn't touch hardware belongs here so it stays testable.
- **`include/` + `src/`** — Arduino-side code: `hardware` (the global `strip` NeoPixel object), `input` (the global `modeEngine`: buttons, pots, EEPROM persistence), `audio_engine` (timed 8 kHz, 256-sample FFT; `audioUpdate()` runs once per frame, modes read cached band getters), `palette`/`utils` helpers, and the modes.

**Mode registry**: modes are plain `void mode_x()` functions registered in two function-pointer tables (audio / non-audio) in `src/mode_registry.cpp`. The registry owns the mode counts — button cycling and EEPROM validation derive from table size, so adding a table entry is all that's needed to wire in a mode. Each `ModeEntry` has an `autoClear` flag controlling whether the main loop wipes the strip before dispatch; on this fixture every mode is `false` because trails depend on pixel persistence.

**Main loop** (`src/main.cpp`): pace to `FRAME_INTERVAL_MS` → `modeEngine.update()` → apply right-pot brightness (2% floor, capped at `MAX_BRIGHTNESS` 160/255 for power budget) → `audioUpdate()` if audio group active → dispatch current mode via registry → `strip.show()`. Modes keep per-mode state in function-local statics and read the left pot for their one parameter.

**Configuration**: all pins and per-fixture constants live in `include/board_config.h` — the only hardware config point. LED-index geometry lives in `hex_geometry` (hexes are 1-based in path arrays, `HEXES[]` is 0-based).

**Tests**: `test_build_src = yes` means the hardware test env links against `src/`, whose `setup()`/`loop()` are compiled out under `PIO_UNIT_TESTING` (see the guard in `main.cpp`) so the harness supplies its own. `test/test_hardware` is a manual visual check on the board, not an assertion suite.

## Adding a mode

1. `include/modes/<group>/mode_myMode.h` declaring `void mode_myMode();`
2. `src/modes/<group>/mode_myMode.cpp` implementing it
3. One `{ mode_myMode, autoClear, "My Mode" }` line in the matching table in `src/mode_registry.cpp`

## Behaviour notes

- Mode cycling fires on button **release**; a hold ≥ 600 ms is the reset-both-groups gesture (constants in `include/input.h`).
- Audio band ceilings (`BASS_MAX`/`MID_MAX`/`TREB_MAX` in `src/audio_engine.cpp`) are hardware-tuned; changing sampling timing may require retuning them on the physical fixture.
- EEPROM bytes 0–2 store audio-enabled flag and the two mode indices; values are validated against registry counts on boot.
