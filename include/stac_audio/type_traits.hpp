#pragma once

#include "frame.hpp"

namespace stac {
template<typename _sample_t>
struct is_frame : std::false_type {};

template<typename _sample_t>
struct is_frame<dsp::Frame<_sample_t>> : std::true_type {};

template<typename _sample_t>
constexpr bool is_frame_v = is_frame<_sample_t>::value;
}
