#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "../ecs/ecs.hpp"
#include "../components/transform_component.hpp"
#include "../components/sprite_component.hpp"

#include <SDL2/SDL.h>

struct SDL_Renderer;

class RenderSystem : public System {
public:
	RenderSystem();

	void update(SDL_Renderer* renderer);
};

RenderSystem::RenderSystem() {
	require_component<TransformComponent>();
	require_component<SpriteComponent>();
}

void RenderSystem::update(SDL_Renderer* renderer) {
	for (const Entity entity : get_entities()) {
		const TransformComponent& transform{ entity.get_component<TransformComponent>() };
		const SpriteComponent& sprite{ entity.get_component<SpriteComponent>() };

		SDL_FRect rect{
			static_cast<float>(transform.position.x),
			static_cast<float>(transform.position.y),
			sprite.width,
			sprite.height
		};
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRectF(renderer, &rect);
	}
}

#endif //RENDER_SYSTEM_HPP
