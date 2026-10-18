#pragma once

#include <complex>
#include <functional>
#include <optional>
#include <tuple>
#include <fftw3.h>
#include "dsp_declarations.hpp"
#include "signals.hpp"

enum class AudioThreadState {
	PLAYING = 0,
	PAUSED = 1,
	IDLE = 2,
	STARTING = 3
};

struct AudioThreadData;

struct StateTransition {
	AudioThreadState next_state = AudioThreadState::IDLE;
	std::function<bool(const AudioThreadData&)> transition_condition;
};

struct AudioThreadData {
	AudioThreadState                                       state            = AudioThreadState::IDLE;
	/// The next state to transition to. If there is no next state to transition to then this will have no value
	std::optional<StateTransition>                         state_transition = std::nullopt;
	const dsp::Signal<dsp::frame_real_t>*                  signal           = nullptr;
	dsp::Signal<dsp::frame_real_t, dsp::FRAMES_PER_BUFFER> wave;
	size_t                                                 sample_index     = 0;
	/// Volume of the wave
	dsp::amplitude_t                                       amplitude_scalar = 1.0;
	dsp::pitch_t                                           pitch_shift      = 0.0;
	/// Size of the complex wave is the size of the real wave / 2 + 1
	static constexpr size_t COMPLEX_WAVE_SIZE = std::tuple_size_v<decltype(wave.samples)> / 2 + 1;
	dsp::Signal<dsp::frame_complex_t, COMPLEX_WAVE_SIZE>   complex_wave;
	/// TODO implement the dft functionality
	fftw_plan                                              r2c_plan         = nullptr;
	fftw_plan                                              c2r_plan         = nullptr;
	/**
	 * Buffer used to share samples between audio thread and controller thread
	 * for debugging purposes. Of course, make sure to reserve all needed memory
	 * in the controller thread before passing it to the audio thread since the audio
	 * thread can't make syscalls and thus won't be able to actually add samples
	 * without memory already being reserved
	 */
	dsp::Signal<dsp::frame_real_t>*                        debug_buffer     = nullptr;
	bool                                                   record_debug_samples = false;
};

/*
 * TODO need to have some sort of structure that contains the fftw_plan,
 * buffers, and functionality to create and destroy these things that are
 * real-time audio thread safe
 */
