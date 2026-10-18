#pragma once

#include <cmath>

#include "dsp_declarations.hpp"

namespace dsp::utils {
/**
* @brief Return the value, in dB, of a sample's amplitude
*
* @param sample the sample, in amplitude scale, to get the dB value of
*/
template<typename _sample_t>
constexpr _sample_t amp_to_db(_sample_t sample) {
	return static_cast<_sample_t>(20 * std::log10(sample));
}
/**
* @brief Return the value, in amplitude scale, of a sample's dB value
*
* @param sample the sample, in dB, to get the amplitude scale value of
*/
template<typename _sample_t>
constexpr _sample_t db_to_amp(_sample_t sample) {
	return static_cast<_sample_t>(std::pow(10, sample / 20));
}

/**
 * @brief Get a signal sample index from a time in ms
 * @param sample_rate Sample rate of the signal
 * @param time Time in ms
 * @return Sample index corresponding to the time and sample rate
 */
constexpr size_t sample_index_from_time(const sample_rate_t sample_rate, const dsp::time_ms_floating_t time) {
	return static_cast<size_t>(time * (sample_rate / 1000.0));
}

constexpr time_ms_floating_t frequency_to_period(const frequency_hz_t freq) {
	return 1.0 / freq * 1000.0;
}

constexpr frequency_hz_t period_to_frequency(const dsp::time_ms_floating_t period) {
	return 1.0 / period * 1000.0;
}

constexpr size_t get_num_samples_in_frequency_cycle(const sample_rate_t sample_rate, const frequency_hz_t freq) {
	const dsp::time_ms_floating_t period = dsp::utils::frequency_to_period(freq);

	return dsp::utils::sample_index_from_time(sample_rate, period);
}

} // namespace dsp::utils
