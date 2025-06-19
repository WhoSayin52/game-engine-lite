#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP

#include <string>

namespace sprite_config {
	constexpr int src_x{ 0 };
	constexpr int src_y{ 0 };
	constexpr int width{ 32 };
	constexpr int height{ 32 };
}

struct SpriteComponent {
	std::string asset_id{};
	int width{ sprite_config::width };
	int height{ sprite_config::height };
	int src_x{ sprite_config::src_x };
	int src_y{ sprite_config::src_y };

	SpriteComponent(
		const std::string& asset_id = "",
		int width = sprite_config::width,
		int height = sprite_config::height,
		int src_x = sprite_config::src_x,
		int src_y = sprite_config::src_y
		)
		: asset_id{ asset_id },
		width{ width }, height{ height },
		src_x{ src_x }, src_y{ src_y }
	{
	}
};

#endif //SPRITE_COMPONENT_HPP
