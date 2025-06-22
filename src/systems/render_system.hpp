#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "../ecs/ecs.hpp"
#include "../components/transform_component.hpp"
#include "../components/sprite_component.hpp"

#include <SDL2/SDL.h>

#include <vector>
#include <algorithm>

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

	std::vector<Entity> renderable_entities{};

	for (const Entity& entity : get_entities()) {
		if (entity.has_component<SpriteComponent>()) {
			renderable_entities.push_back(entity);
		}
	}

	std::sort(
		renderable_entities.begin(),
		renderable_entities.end(),
		[](const Entity& e1, const Entity& e2) -> bool {
			auto& e1_sprite{ e1.get_component<SpriteComponent>() };
			auto& e2_sprite{ e2.get_component<SpriteComponent>() };

			return e1_sprite.z_index < e2_sprite.z_index;
		}
	);

	for (const Entity entity : renderable_entities) {
		const TransformComponent& transform{ entity.get_component<TransformComponent>() };
		const SpriteComponent& sprite{ entity.get_component<SpriteComponent>() };

		const SDL_Rect dest_rect{
			static_cast<int>(transform.position.x),
			static_cast<int>(transform.position.y),
			static_cast<int>(sprite.width * transform.scale.x),
			static_cast<int>(sprite.height * transform.scale.x)
		};

		SDL_RenderCopyEx(
			renderer,
			asset_manager->get_texture(sprite.asset_id),
			&sprite.src_rect,
			&dest_rect,
			transform.rotation,
			nullptr,
			SDL_FLIP_NONE
		);
	}
}

#endif //RENDER_SYSTEM_HPP
