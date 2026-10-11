#pragma once

#include <cstdint>
#include <filesystem>
#include <expected>
#include <fstream>

#include "signals.hpp"
#include "type_traits.hpp"

namespace stac::file_utils {
// TODO switch to using a Result<fs::path, uint64_t> to determine errors with more granularity with errno and the like
std::expected<std::filesystem::path, uint64_t> get_executable_path();
std::expected<dsp::Signal<dsp::frame_real_t>, int> read_snd_file(const std::string& file_path);

enum class WriteResult : uint8_t {
	SUCCESS      = 0,
	OPEN_FAILED  = 1,
	FLUSH_FAILED = 2
};

/**
 * @brief Write the specified signal's frame data to the specified file.
 * If _sample_t is of type dsp::Frame then data will be in csv format with
 * left channel data as the first element of each line and right channel data
 * as the second element of each line.
 * If _sample_t is of any other type then each element of the Signal will be written
 * to a line using the << operator
 * Will overwrite any existing data that is contained with the file.
 *
 * THROWS std::ifstream::failure if file is unable to be opened/written to
 *
 * @param signal the signal to write to a file
 * @param file_path the path of the file to write the signal to
*/
template<typename _sample_t, size_t _capacity>
WriteResult write_signal_to_file(const dsp::Signal<_sample_t, _capacity>& signal, const std::string& file_path) {
	std::fstream file(file_path, std::ios_base::out | std::ios_base::trunc);

	if (!file.is_open()) {
		return WriteResult::OPEN_FAILED;
	}

	file << signal.sample_rate << "\n";

	for (size_t i = 0; i < signal.samples.size(); i++) {
		static constexpr size_t num_frames_to_write_before_flushing = 50;

		if constexpr (is_frame_v<_sample_t>) {
			const _sample_t& frame = signal.samples.at(i);

			file << frame.left_sample << ", " << frame.right_sample << "\n";
		} else {
			file << signal.samples.at(i);
		}

		if (i % num_frames_to_write_before_flushing == 0) {
			file.flush();

			if (file.bad()) {
				return WriteResult::FLUSH_FAILED;
			}
		}
	}

	return WriteResult::SUCCESS;
}
}
