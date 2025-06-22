#ifndef ANIMATION_COMPONENT_HPP
#define ANIMATION_COMPONENT_HPP

struct AnimationComponent {
	int frames{ 1 };
	int current_frame{ 0 };
	double frame_delay{ 0.0 };
	double elapsed_seconds{ 0.0 };
	bool loop{ true };

	AnimationComponent(
		int frames = 1,
		double frame_delay = 0.0,
		bool loop = true
	) :
		frames{ frames },
		frame_delay{ frame_delay },
		loop{ loop } {
	}
};

#endif //ANIMATION_COMPONENT_HPP
