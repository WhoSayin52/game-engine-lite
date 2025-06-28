#ifndef CAMERA_FOLLOW_SYSTEM_HPP
#define CAMERA_FOLLOW_SYSTEM_HPP

#include "../game/game.hpp"
#include "../ecs/ecs.hpp"
#include "../components/camera_component.hpp"
#include "../components/transform_component.hpp"

#include <SDL2/SDL.h>

#include <iostream>

class CameraFollowSystem : public System {
public:
	CameraFollowSystem() {
		require_component<CameraComponent>();
		require_component<TransformComponent>();
	}

	void update(SDL_Rect* camera) {
		for (Entity& entity : get_entities()) {
			TransformComponent& transfrom{ entity.get_component<TransformComponent>() };

			int entity_x{ static_cast<int>(transfrom.position.x) };
			int entity_y{ static_cast<int>(transfrom.position.y) };

			int mid_x{ camera->w / 2 };
			int mid_y{ camera->h / 2 };

			int new_x{ entity_x - mid_x };
			int new_y{ entity_y - mid_y };

			if (new_x < 0) {
				new_x = 0;
			}
			else if (new_x + camera->w > Game::map_width) {
				new_x = Game::map_width - camera->w;
			}

			if (new_y < 0) {
				new_y = 0;
			}
			else if (new_y + camera->h > Game::map_height) {
				new_y = Game::map_height - camera->h;
			}

			camera->x = new_x;
			camera->y = new_y;
		}
	}
};

#endif //CAMERA_FOLLOW_SYSTEM_HPP
