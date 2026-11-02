#pragma once

#include "signals.hpp"
#include "math.hpp"

namespace dsp::filters {
template<typename _Derived>
struct Filter {
	// TODO this might make more sense returning a return status
	// but not sure yet. I need to learn more about how filters
	// work before I can decide that
	template<typename _sample_t, size_t _capacity>
	void apply(dsp::Signal<_sample_t, _capacity>& signal) const {
		const _Derived& derived = static_cast<const _Derived&>(*this);

		derived._apply(signal);
	}
};

template<typename _sample_t, typename _tap_t>
struct FIRFilter {
	using tap_type = _tap_t;
	using sample_type = _sample_t;

	FIRFilter() = default;

	explicit FIRFilter(std::vector<tap_type> taps) :
			taps(std::move(taps)) {
		this->samples.reserve(this->taps.size());
		this->insert_iterator = this->samples.begin();
	}

	void set_taps(std::vector<tap_type> taps) {
		this->taps = std::move(taps);
		this->samples.reserve(this->taps.size());

		this->reset();
	}

	/**
	 * @brief Insert sample into the filter's buffer
	 * @return Whether the filter is primed after inserting the sample
	 */
	bool insert(sample_type sample) {
		if (this->insert_iterator == this->samples.end()) {
			this->insert_iterator = this->samples.begin();
		}

		if (!this->is_primed()) {
			this->samples.push_back(std::move(sample));
		} else {
			*this->insert_iterator = sample;
		}

		this->insert_iterator++;

		return this->is_primed();
	}

	void reset() {
		this->samples.clear();
		this->insert_iterator = this->samples.begin();
	}

	[[nodiscard]]
	bool is_primed() const {
		return this->samples.size() == this->taps.size();
	}

	[[nodiscard]]
	const std::vector<tap_type>& get_taps() const noexcept {
		return this->taps;
	}

	[[nodiscard]]
	const std::vector<sample_type>& get_samples() const noexcept {
		return this->samples;
	}

private:
	std::vector<tap_type> taps;
	std::vector<sample_type> samples;
	typename decltype(samples)::iterator insert_iterator;
};

// NOTE: I think that I would need a copy of the previous "wave" stored in order
// to completely fill in the audio thread's "wave." This is because there is a time-delay
// equal to the number of taps in the FIR filter. This would necessitate
// passing the previous "wave" into the filter OR having one long wave and essentially
// splitting it in half, so that FIR filters can look backwards in time
// at the start of the current buffer which we want to pass to the DAC
//
// Could use a double-buffering system
//
// NOTE: This does not account for adapter filters, unless the coefficient samples are functions I guess?
// So I guess that it does but at the time of writing this needs to still be tested and thought about.
// If this note is still here, then it means that this still needs to be tested and thought about more.
// Perhaps the concept of the BiQuad handles this? No clue. I should also look that up
// template<typename _sample_t, typename _coefficient_sample_t>
// struct FIRFilter : public Filter<FIRFilter<_sample_t, _coefficient_sample_t>> {
// 	using sample_type = _sample_t;
// 	using coefficient_sample_type = _coefficient_sample_t;
//
// 	explicit FIRFilter(std::vector<coefficient_sample_type> coefficients) :
// 			coefficients(std::move(coefficients)),
// 			averager(this->coefficients.size())
// 	{}
//
//
// 	template<size_t _capacity>
// 	void _apply(dsp::Signal<sample_type, _capacity>& signal) const {
// 		// be sure not to modify this in-place but rather have a separate
// 		// vector for storing the averages
// 	}
//
// private:
// 	std::vector<coefficient_sample_type> coefficients;
// 	stac::MovingAverage<sample_type> averager;
// };
}
