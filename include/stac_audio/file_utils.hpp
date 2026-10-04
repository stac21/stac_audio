#pragma once

#include <filesystem>
#include <expected>

#include "dsp_declarations.hpp"
#include "signals.hpp"

namespace stac::file_utils {
// TODO switch to using a Result<fs::path, uint64_t> to determine errors with more granularity with errno and the like
std::expected<std::filesystem::path, uint64_t> get_executable_path();
std::expected<dsp::Signal<dsp::frame_real_t>, int> read_snd_file(const std::string& file_path);
}
