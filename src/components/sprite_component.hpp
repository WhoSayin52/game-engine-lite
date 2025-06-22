#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP

#include <SDL2/SDL_rect.h>

#include <string>

namespace sprite_config {
	constexpr int src_x{ 0 };
	constexpr int src_y{ 0 };
	constexpr int width{ 32 };
	constexpr int height{ 32 };
}

struct SpriteComponent {
	std::string asset_id{};
	int z_index{};
	int width{ sprite_config::width };
	int height{ sprite_config::height };
	SDL_Rect src_rect{
		sprite_config::src_x,
		sprite_config::src_y,
		sprite_config::width,
		sprite_config::height
	};

	SpriteComponent(
		const std::string& asset_id = "",
		int z_index = 0,
		int width = sprite_config::width,
		int height = sprite_config::height,
		int src_x = sprite_config::src_x,
		int src_y = sprite_config::src_y
	) :
		asset_id{ asset_id }, z_index{ z_index },
		width{ width }, height{ height },
		src_rect{ src_x, src_y, width, height }
	{
	}
};

#endif //SPRITE_COMPONENT_HPP
