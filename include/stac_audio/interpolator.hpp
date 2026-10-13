#pragma once

#include <cstdint>
#include <type_traits>

namespace stac {
template<typename _T>
concept Interpolatable = requires(_T a, _T b, float f) {
	{a - b} -> std::convertible_to<_T>;
	{a + b} -> std::convertible_to<_T>;
	{f * a} -> std::convertible_to<_T>;
};

// TODO I still need to fix the glitch when pausing audio and probably playing it as well

// NOTE: "Should probably have a strategy parameter that I can set to switch from linear to logarithmic"
template<Interpolatable _T>
class Interpolator {
public:
	using value_type = _T;
	using ref_type = std::conditional_t<std::is_arithmetic_v<value_type>, value_type, value_type&>;
	using const_ref_type = std::conditional_t<std::is_arithmetic_v<value_type>, ref_type, const ref_type>;

	Interpolator() :
			initial_val(),
			target_val(),
			current_val(),
			intermediate_val()
	{}

	Interpolator(value_type initial_val, value_type target_val, uint32_t num_steps_to_reach_target) :
			initial_val(std::move(initial_val)),
			target_val(std::move(target_val)),
			current_val(initial_val),
			intermediate_val(target_val - initial_val),
			num_steps_to_reach_target(num_steps_to_reach_target)
	{}

	void reset() {
		this->current_val = this->initial_val;
		this->num_curr_steps = 0;
	}

	/**
	 * @brief Do one step in the interpolation
	 * @return True if the target has not been reached and a step was performed,
	 * false if the target has been reached and no step was performed
	 */
	bool step() {
		if (this->is_target_reached()) {
			return false;
		}

		this->num_curr_steps++;

		this->current_val = this->initial_val + ((static_cast<float>(this->num_curr_steps) / this->num_steps_to_reach_target) * this->intermediate_val);

		return true;
	}

	bool is_target_reached() const noexcept {
		return this->num_curr_steps == this->num_steps_to_reach_target;
	}

	void set_values(value_type initial_val, value_type target_val) {
		this->initial_val = std::move(initial_val);
		this->target_val = std::move(target_val);

		this->intermediate_val = (this->target_val - this->initial_val);

		this->reset();
	}

	void set_num_steps_to_reach_target(const uint32_t num_steps_to_reach_target) {
		this->num_steps_to_reach_target = num_steps_to_reach_target;
	}

	const_ref_type get_initial_val() const noexcept {
		return this->initial_val;
	}

	const_ref_type get_target_val() const noexcept {
		return this->target_val;
	}

	const_ref_type get_current_val() const noexcept {
		return this->current_val;
	}

private:
	value_type initial_val;
	value_type target_val;
	value_type current_val;
	value_type intermediate_val;
	uint32_t num_steps_to_reach_target = 0;
	uint32_t num_curr_steps = 0;
};
}
