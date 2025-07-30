#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "../ecs/ecs.hpp"
#include "../components/transform_component.hpp"
#include "../components/sprite_component.hpp"

#include "../components/rigidbody_component.hpp"

#include <SDL2/SDL.h>

#include <vector>
#include <algorithm>

#include <iostream>

class RenderSystem : public System {
public:
	RenderSystem() {
		require_component<TransformComponent>();
		require_component<SpriteComponent>();
	}

	void update(SDL_Renderer* renderer, AssetManager& asset_manager, SDL_Rect* camera) {

		std::vector<Entity> renderable_entities{};

		for (const Entity& entity : get_entities()) {

			auto& transform{ entity.get_component<TransformComponent>() };
			auto& sprite{ entity.get_component<SpriteComponent>() };

			bool is_outside_camera_view{
				(transform.position.x + sprite.width * transform.scale.x) < camera->x ||
				transform.position.x > (camera->x + camera->w) ||
				(transform.position.y + sprite.height * transform.scale.y) < camera->y ||
				transform.position.y > (camera->y + camera->h)
			};

			if (is_outside_camera_view && !sprite.is_fixed) {
				continue;
			}

			renderable_entities.push_back(entity);
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

			int camera_x = sprite.is_fixed ? 0 : camera->x;
			int camera_y = sprite.is_fixed ? 0 : camera->y;

			const SDL_Rect dest_rect{
				static_cast<int>(std::round(transform.position.x - camera_x)),
				static_cast<int>(std::round(transform.position.y - camera_y)),
				static_cast<int>(sprite.width * transform.scale.x),
				static_cast<int>(sprite.height * transform.scale.y)
			};

			SDL_RenderCopyEx(
				renderer,
				asset_manager.get_texture(sprite.asset_id),
				&sprite.src_rect,
				&dest_rect,
				transform.rotation,
				nullptr,
				sprite.flip
			);
		}
	}
};

#endif //RENDER_SYSTEM_HPP
