#pragma once

#include <cstdint>
#include <cmath>
#include <fstream>

#include "dsp_declarations.hpp"
#include "signals.hpp"

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
constexpr size_t sample_index_from_time(const sample_rate_t sample_rate, const time_ms_t time) {
	return static_cast<size_t>(time * (sample_rate / 1000.0));
}
} // namespace dsp::utils
