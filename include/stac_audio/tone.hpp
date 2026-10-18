#pragma once

#include <cstdint>

#include "dsp_declarations.hpp"

namespace stac {
enum class Note : uint8_t {
	C           = 0,
	C_SHARP     = 1,
	D_FLAT      = C_SHARP,
	D           = 2,
	D_SHARP     = 3,
	E_FLAT      = D_SHARP,
	E           = 4,
	F           = 5,
	F_SHARP     = 6,
	G_FLAT      = F_SHARP,
	G           = 7,
	G_SHARP     = 8,
	A_FLAT      = G_SHARP,
	A           = 9,
	A_SHARP     = 10,
	B_FLAT      = A_SHARP,
	B           = 11,
	NUM_ENTRIES = B + 1
};

enum class Octave : uint8_t {
	ZERO        = 0,
	ONE         = 1,
	TWO         = 2,
	THREE       = 3,
	FOUR        = 4,
	FIVE        = 5,
	SIX         = 6,
	SEVEN       = 7,
	EIGHT       = 8,
	NUM_ENTRIES = EIGHT + 1
};

constexpr uint8_t NUM_SEMITONES = static_cast<uint8_t>(Note::NUM_ENTRIES) * static_cast<uint8_t>(Octave::NUM_ENTRIES);

struct Tone {
	Note   note   = Note::C;
	Octave octave = Octave::ZERO;

	/**
	 * @brief  Return the frequency corresponding to the note and octave of the tone
	 * @return The frequency corresponding to the note and octave of the tone
	 */
	constexpr dsp::frequency_hz_t frequency() const {
		return Tone::frequencies[static_cast<uint8_t>(this->note)][static_cast<uint8_t>(this->octave)];
	}

private:
	/*
	 * Note: keeping this a C-style array to ensure contiguous memory
	 * while still maintaining the simple [i][j] notation
	 */
	static constexpr dsp::frequency_hz_t frequencies[static_cast<uint8_t>(Note::NUM_ENTRIES)][static_cast<uint8_t>(Octave::NUM_ENTRIES)] = {
		// Octave  0      1      2       3       4       5       6        7        8             Note
		         { 16.35, 32.70, 65.41,  130.81, 261.63, 523.25, 1046.50, 2093.00, 4186.01 }, // C
		         { 17.32, 34.65, 69.30,  138.59, 277.18, 554.37, 1108.73, 2217.46, 4434.92 }, // C#/Db
		         { 18.35, 36.71, 73.42,  146.83, 293.66, 587.33, 1174.66, 2349.32, 4698.63 }, // D
		         { 19.45, 38.89, 77.78,  155.56, 311.13, 622.25, 1244.51, 2489.02, 4978.03 }, // D#/Eb
		         { 20.60, 41.20, 82.41,  164.81, 329.63, 659.25, 1318.51, 2637.02, 5274.04 }, // E
		         { 21.83, 43.65, 87.31,  174.61, 349.23, 698.46, 1396.91, 2793.83, 5587.65 }, // F
		         { 23.12, 46.25, 92.50,  185.00, 369.99, 739.99, 1479.98, 2959.96, 5919.91 }, // F#/Gb
		         { 24.50, 49.00, 98.00,  196.00, 392.00, 783.99, 1597.98, 3135.96, 6271.93 }, // G
		         { 25.96, 51.91, 103.83, 207.65, 415.30, 830.61, 1661.22, 3322.44, 6644.88 }, // G#/Ab
		         { 27.50, 55.00, 110.00, 220.00, 440.00, 880.00, 1760.00, 3520.00, 7040.00 }, // A
		         { 29.14, 58.27, 116.54, 233.08, 466.16, 923.33, 1864.66, 3729.31, 7458.62 }, // A#/Bb
		         { 30.87, 61.74, 123.47, 246.94, 493.88, 987.77, 1975.53, 3951.07, 7902.13 }  // B
	};

	static_assert(sizeof(Tone::frequencies) / sizeof(Tone::frequencies[0]) == static_cast<uint8_t>(Note::NUM_ENTRIES), "Number of rows in frequency matrix must == Note::NUM_ENTRIES");
	static_assert(sizeof(Tone::frequencies[0]) / sizeof(dsp::frequency_hz_t) == static_cast<uint8_t>(Octave::NUM_ENTRIES), "Number of columns in frequency matrix must == Octave::NUM_ENTRIES");
};
}
