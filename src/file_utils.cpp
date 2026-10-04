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
		// TODO return value of errno
		return std::nullopt;
	}

	// path.data() does not get null-terminated by readlink, so need to pass num_bytes_read for its size
	return std::filesystem::path(std::string_view(path.data(), num_bytes_read)).parent_path();
#endif
}
