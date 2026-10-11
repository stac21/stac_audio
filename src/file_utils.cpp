#include "file_utils.hpp"

#include <array>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else // Linux
#include <string_view>
#include <unistd.h>
#include <limits.h>
#endif

#include <sndfile.h>

std::expected<std::filesystem::path, uint64_t> stac::file_utils::get_executable_path() {
#ifdef _WIN32
	std::array<wchar_t, MAX_PATH> path = {};

	if (GetModuleFileNameW(nullptr, path.data(), path.size()) == 0) {
		return std::unexpected(GetLastError());
	}

	// path.data() gets null-terminated by GetModuleFileNameW, so no need to pass the size
	return std::filesystem::path(path.data()).parent_path();

#elif defined(__APPLE__)
	static_assert(false, "Need an apple machine to build and test this on before I implement it");

#else // Linux
	std::array<char, PATH_MAX> path = {};

	const ssize_t num_bytes_read = readlink("/proc/self/exe", path.data(), path.size());

	if (num_bytes_read == -1) {
		return std::unexpected(errno);
	}

	// path.data() does not get null-terminated by readlink, so need to pass num_bytes_read for its size
	return std::filesystem::path(std::string_view(path.data(), num_bytes_read)).parent_path();
#endif
}

std::expected<dsp::Signal<dsp::frame_real_t>, int> stac::file_utils::read_snd_file(const std::string& file_path) {
	SF_INFO sf_info = {};
	SNDFILE* sf = sf_open(file_path.c_str(), SFM_READ, &sf_info);

	if (sf == nullptr) {
		return std::unexpected(sf_error(nullptr));
	} else if (sf_info.samplerate <= 0) {
		return std::unexpected(-1);
	}

	dsp::Signal<dsp::frame_real_t> signal = {
		.sample_rate = static_cast<dsp::sample_rate_t>(sf_info.samplerate),
		.samples = {}
	};

	sf_count_t curr_frames_read = 0;
	static constexpr sf_count_t num_frames_to_read = 256;
	std::array<dsp::sample_real_t, num_frames_to_read> in_buffer = {};
	dsp::frame_real_t curr_frame = {};

	do {
		curr_frames_read = sf_readf_float(sf, in_buffer.data(), in_buffer.size());

		// insert the read frames into the signal
		for (size_t i = 0; i < curr_frames_read; i += sf_info.channels) {
			curr_frame.left_sample = in_buffer.at(i);

			if (sf_info.channels == 1) {
				curr_frame.right_sample = curr_frame.left_sample;
			} else {
				curr_frame.right_sample = in_buffer.at(i + 1);
			}

			signal.samples.push_back(curr_frame);
		}
	} while (curr_frames_read == num_frames_to_read);

	return signal;
}
