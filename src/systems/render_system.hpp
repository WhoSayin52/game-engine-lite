#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "../ecs/ecs.hpp"
#include "../components/transform_component.hpp"
#include "../components/sprite_component.hpp"

#include <SDL2/SDL.h>

class RenderSystem : public System {
public:
	RenderSystem();

	void update(SDL_Renderer* renderer, AssetManager* asset_manager);
};

RenderSystem::RenderSystem() {
	require_component<TransformComponent>();
	require_component<SpriteComponent>();
}

void RenderSystem::update(SDL_Renderer* renderer, AssetManager* asset_manager) {
	for (const Entity entity : get_entities()) {
		const TransformComponent& transform{ entity.get_component<TransformComponent>() };
		const SpriteComponent& sprite{ entity.get_component<SpriteComponent>() };

		const SDL_Rect src_rect{
			sprite.src_x,
			sprite.src_y,
			sprite.width,
			sprite.height
		};

		const SDL_Rect dest_rect{
			static_cast<int>(transform.position.x),
			static_cast<int>(transform.position.y),
			static_cast<int>(sprite.width * transform.scale.x),
			static_cast<int>(sprite.height * transform.scale.x)
		};

		SDL_RenderCopyEx(
			renderer,
			asset_manager->get_texture(sprite.asset_id),
			&src_rect,
			&dest_rect,
			transform.rotation,
			nullptr,
			SDL_FLIP_NONE
		);
	}
}

#endif //RENDER_SYSTEM_HPP
