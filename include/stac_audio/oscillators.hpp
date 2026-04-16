#pragma once

#include <vector>
#include <iostream>
#include <numbers>
#include <cmath>

#include "dsp_declarations.hpp"
#include "dsp_utils.hpp"
#include "type_traits.hpp"

namespace stac {
template<typename _frame_t>
std::vector<_frame_t> generate_sin_signal(const dsp::frequency_hz_t frequency, const dsp::sample_rate_t sample_rate) {
	using sample_type = std::conditional_t<is_frame_v<_frame_t>, typename _frame_t::sample_type, _frame_t>;

	const dsp::time_ms_floating_t period = dsp::utils::frequency_to_period(frequency);
	const size_t num_samples = dsp::utils::sample_index_from_time(sample_rate, period);
	std::cout << "period: " << period << ", num_samples: " << num_samples << "\n";

	std::vector<_frame_t> samples;
	samples.reserve(num_samples);

	sample_type curr_sample;

	for (size_t sample_index = 0; sample_index < num_samples; sample_index++) {
		curr_sample = static_cast<sample_type>(std::sin(frequency * 2 * std::numbers::pi * (static_cast<double>(sample_index) / sample_rate)));

		if constexpr (is_frame_v<_frame_t>) {
			samples.emplace_back(curr_sample, curr_sample);
		} else {
			samples.emplace_back(curr_sample);
		}
	}

	return samples;
}

// I don't think that this is actually the right way to do it,
// but check the google thing for descriptions of ways to do sawtooth
// properly (algorithms). There is an example in the audio_sandbox directory
enum class SawtoothGenerationStrategy {
	ADDITIVE = 0,
	PHASE_ACCUMULATOR = 1,
};

// this clearly ain't finished so don't use it
template<typename _frame_t>
std::vector<_frame_t> generate_sawtooth_signal(const dsp::frequency_hz_t fundamental_frequency, const uint16_t num_harmonics, const dsp::sample_rate_t sample_rate) {
	std::vector<_frame_t> samples;

	// TODO reserve proper num frames and calculate proper num samples
	samples.reserve(1);
	size_t num_samples = 1;

	_frame_t curr_sample;

	for (size_t sample_index = 0; sample_index < num_samples; sample_index++) {
		for (size_t harmonic_num = 1; harmonic_num <= num_harmonics; harmonic_num++) {
			// curr_sample = (1 / static_cast<float>(harmonic_num)) * static_cast<_sample_t>(std::)

			// signal.frames.emplace_back(curr_sample);
		}
	}

	return samples;
}
}
