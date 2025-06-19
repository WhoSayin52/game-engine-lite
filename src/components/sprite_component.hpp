#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP

struct SpriteComponent {
	float width{ 32.0f };
	float height{ 32.0f };

	SpriteComponent(float width = 32.0f, float height = 32.0f)
		: width{ width }, height{ height } {
	}
};

#endif //SPRITE_COMPONENT_HPP
