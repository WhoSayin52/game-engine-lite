#ifndef RENDER_TEXT_SYSTEM_HPP
#define RENDER_TEXT_SYSTEM_HPP

#include "../asset_manager/asset_manager.hpp"
#include "../ecs/ecs.hpp"
#include "../components/text_label_component.hpp"

#include <SDL2/SDL.h>

class RenderTextSystem : public System {
public:
	RenderTextSystem() {
		require_component<TextLabelComponent>();
	}

	void update(SDL_Renderer* renderer, AssetManager& asset_manager, SDL_Rect* camera) {

		for (const Entity& e : get_entities()) {
			auto& text_label{ e.get_component<TextLabelComponent>() };

			TTF_Font* font{ asset_manager.get_font(text_label.asset_id) };
			std::string& text{ text_label.text };
			SDL_Color color{ text_label.color };

			SDL_Surface* surface{
				TTF_RenderText_Blended(
					font,
					text.c_str(),
					color
				)
			};

			SDL_Texture* texture{
				SDL_CreateTextureFromSurface(
					renderer,
					surface
				)
			};

			SDL_FreeSurface(surface);

			int camera_x = camera->x * (1 - text_label.is_fixed);
			int camera_y = camera->y * (1 - text_label.is_fixed);

			int label_width{ 0 };
			int label_height{ 0 };

			SDL_QueryTexture(texture, nullptr, nullptr, &label_width, &label_height);

			SDL_Rect dest_rect{
				text_label.position.x - static_cast<int>(camera_x),
				text_label.position.y - static_cast<int>(camera_y),
				label_width,
				label_height
			};

			SDL_RenderCopy(renderer, texture, nullptr, &dest_rect);
			SDL_DestroyTexture(texture);
		}
	}
};

#endif //RENDER_TEXT_SYSTEM_HPP
