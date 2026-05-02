// /src/audio_engine.cpp

#include "audio_engine.h"
#include <ArduinoFFT.h>

// =========================================================
// AUDIO ENGINE — IMPLEMENTATION
// Fully restored from original Arduino IDE sketch
// =========================================================

// FFT instance (correct modern API)
ArduinoFFT<double> FFT = ArduinoFFT<double>();

// FFT buffers
double vReal[SAMPLES];
double vImag[SAMPLES];

// DC offset tracking
float micDC = -1;
float micAlphaDC = 0.01;

// Audio band normalization constants (original values)
const double BASS_MAX = 100000.0;
const double MID_MAX  = 200000.0;
const double TREB_MAX = 150000.0;

// =========================================================
// updateFFT()
// Reads microphone, removes DC, performs FFT, stores magnitudes
// =========================================================
void updateFFT() {
    // Read samples
    for (int i = 0; i < SAMPLES; i++) {
        int raw = analogRead(A0);

        // Initialise DC baseline
        if (micDC < 0) micDC = raw;

        // Smooth DC offset
        micDC = (micAlphaDC * raw) + ((1.0f - micAlphaDC) * micDC);

        // Remove DC
        double sample = raw - micDC;

        vReal[i] = sample;
        vImag[i] = 0;
    }

    // Apply window
    FFT.windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);

    // Compute FFT
    FFT.compute(vReal, vImag, SAMPLES, FFT_FORWARD);

    // Convert to magnitudes
    FFT.complexToMagnitude(vReal, vImag, SAMPLES);
}

// =========================================================
// computeAudioBands()
// Sums FFT magnitudes into bass/mid/treble buckets
// =========================================================
void computeAudioBands(double &bass, double &mid, double &treb) {
    bass = mid = treb = 0;

    for (int i = 1; i < SAMPLES / 2; i++) {
        double freq = i * (SAMPLING_FREQUENCY / SAMPLES);
        double mag  = vReal[i];

        if (freq < 300) {
            bass += mag;
        }
        else if (freq < 1100) {
            mid += mag;
        }
        else {
            treb += mag;
        }
    }
}
