#ifndef RENDER_HEALTH_SYSTEM_HPP
#define RENDER_HEALTH_SYSTEM_HPP

#include "../asset_manager/asset_manager.hpp"
#include "../ecs/ecs.hpp"
#include "../components/health_component.hpp"
#include "../components/transform_component.hpp"
#include "../components/sprite_component.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class RenderHealthSystem : public System {

public:
	RenderHealthSystem() {
		require_component<HealthComponent>();
		require_component<TransformComponent>();
		require_component<SpriteComponent>();
	}

	void update(SDL_Renderer* renderer, AssetManager& asset_manager, SDL_Rect* camera) {

		for (const Entity e : get_entities()) {
			auto& health{ e.get_component<HealthComponent>() };
			auto& transform{ e.get_component<TransformComponent>() };
			auto& sprite{ e.get_component<SpriteComponent>() };

			SDL_Color health_bar_color{ 255, 255, 255, 0 };

			int health_amount{ health.health };

			if (health_amount >= 80) {
				health_bar_color = { 0, 255, 0, 0 };
			}
			else if (health_amount >= 40) {
				health_bar_color = { 255, 255, 0, 0 };
			}
			else {
				health_bar_color = { 255, 0, 0, 0 };
			}

			int health_bar_width{ 15 };
			int health_bar_height{ 3 };
			double health_bar_pos_x{ (transform.position.x + (sprite.width / 3 * transform.scale.x)) - camera->x };
			double health_bar_pos_y{ transform.position.y - camera->y };

			SDL_Rect health_bar_rect{
				static_cast<int>(health_bar_pos_x),
				static_cast<int>(health_bar_pos_y),
				static_cast<int>(health_bar_width * (health_amount / 100.0)),
				static_cast<int>(health_bar_height)
			};

			SDL_SetRenderDrawColor(
				renderer,
				health_bar_color.r,
				health_bar_color.g,
				health_bar_color.b,
				255
			);

			SDL_RenderFillRect(renderer, &health_bar_rect);

			std::string health_text{ std::to_string(health_amount) };
			SDL_Surface* surface{
				TTF_RenderText_Blended(
					asset_manager.get_font("pico8_4"),
					health_text.c_str(),
					health_bar_color
				)
			};

			SDL_Texture* texture{ SDL_CreateTextureFromSurface(renderer, surface) };
			SDL_FreeSurface(surface);

			int label_w{ 0 };
			int label_h{ 0 };
			SDL_QueryTexture(texture, NULL, NULL, &label_w, &label_h);
			SDL_Rect health_bar_text_rect{
				static_cast<int>(health_bar_pos_x),
				static_cast<int>(health_bar_pos_y) - 6,
				label_w,
				label_h
			};

			SDL_RenderCopy(renderer, texture, NULL, &health_bar_text_rect);

			SDL_DestroyTexture(texture);
		}
	}
};

#endif //RENDER_HEALTH_SYSTEM_HPP
