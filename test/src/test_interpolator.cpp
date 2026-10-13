#include <iostream>

#include <stac_audio/interpolator.hpp>
#include <stac_audio/dsp_declarations.hpp>

int main() {
	{
		dsp::sample_real_t initial_val = 1.0;
		dsp::sample_real_t target_val = 0.0;
		uint32_t num_steps_to_reach_target = dsp::FRAMES_PER_BUFFER;

		stac::Interpolator<dsp::sample_real_t> interpolator(initial_val, target_val, num_steps_to_reach_target);

		for (size_t i = 0; i < num_steps_to_reach_target + 1; i++) {
			std::cout << interpolator.get_current_val() << ", ";

			if (i % 10 == 0) {
				std::cout << "\n";
			}

			interpolator.step();
		}
	}

	return 0;
}
