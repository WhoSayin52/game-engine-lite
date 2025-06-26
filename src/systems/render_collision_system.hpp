#ifndef RENDER_COLLISION_SYSTEM_HPP
#define RENDER_COLLISION_SYSTEM_HPP

#include "../ecs/ecs.hpp"
#include "../components/transform_component.hpp"
#include "../components/box_collider_component.hpp"

#include <SDL2/SDL.h>

class RenderCollisionSystem : public System {
public:
	RenderCollisionSystem() {
		require_component<TransformComponent>();
		require_component<BoxColliderComponent>();
	}

	void update(SDL_Renderer* renderer) {

		for (const Entity& e : get_entities()) {
			const TransformComponent& transform{ e.get_component<TransformComponent>() };
			const BoxColliderComponent& collider{ e.get_component<BoxColliderComponent>() };

			SDL_Rect collider_rect{
				static_cast<int>(transform.position.x + collider.offset.x),
				static_cast<int>(transform.position.y + collider.offset.y),
				collider.width,
				collider.height
			};

			if (collider.is_colliding) {
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			}

			SDL_RenderDrawRect(renderer, &collider_rect);
		}
	}
};

#endif //RENDER_COLLISION_SYSTEM_HPP
