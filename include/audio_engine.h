// /include/audio_engine.h
#pragma once

#include <Arduino.h>

// =========================================================
// AUDIO ENGINE — HEADER
// Provides FFT buffers, constants, and computeAudioBands()
// =========================================================

// FFT configuration
#define SAMPLES 256
#define SAMPLING_FREQUENCY 8000

// FFT buffers (extern so only defined once)
extern double vReal[SAMPLES];
extern double vImag[SAMPLES];

// DC offset tracking
extern float micDC;
extern float micAlphaDC;

// Audio band normalization constants (from original sketch)
extern const double BASS_MAX;
extern const double MID_MAX;
extern const double TREB_MAX;

// Main audio band computation function
void computeAudioBands(double &bass, double &mid, double &treb);

// FFT update function (called once per frame)
void updateFFT();
