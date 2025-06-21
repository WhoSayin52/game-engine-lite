#ifndef ANIMATION_SYSTEM_HPP
#define ANIMATION_SYSTEM_HPP

#include "../ecs/ecs.hpp"
#include "../components/animation_component.hpp"
#include "../components/sprite_component.hpp"

#include "iostream"

class AnimationSystem : public System {
public:
	AnimationSystem();

	void update(double delta_time);
};

AnimationSystem::AnimationSystem() {
	require_component<AnimationComponent>();
	require_component<SpriteComponent>();
}

void AnimationSystem::update(double delta_time) {

	for (Entity entity : get_entities()) {
		SpriteComponent& sprite{ entity.get_component<SpriteComponent>() };
		AnimationComponent& animation{ entity.get_component<AnimationComponent>() };

		bool animate{ animation.loop || animation.current_frame < animation.frames - 1 };
		if (animate) {
			animation.elapsed_seconds += delta_time;

			if (animation.elapsed_seconds >= animation.frame_delay) {
				++animation.current_frame;
				animation.current_frame = animation.current_frame % animation.frames;
				animation.elapsed_seconds -= animation.frame_delay;
			}

			sprite.src_rect.x = animation.current_frame * sprite.width;
		}
	}
}

#endif //ANIMATION_SYSTEM_HPP
