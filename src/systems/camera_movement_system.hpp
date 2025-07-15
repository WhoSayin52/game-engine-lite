#ifndef CAMERA_MOVEMENT_SYSTEM_HPP
#define CAMERA_MOVEMENT_SYSTEM_HPP

#include "../game/game.hpp"
#include "../ecs/ecs.hpp"
#include "../components/camera_component.hpp"
#include "../components/transform_component.hpp"

#include <SDL2/SDL.h>

class CameraMovementSystem : public System {
public:
	CameraMovementSystem() {
		require_component<CameraComponent>();
		require_component<TransformComponent>();
	}

	void update(SDL_Rect* camera) {
		for (Entity& entity : get_entities()) {
			TransformComponent& transfrom{ entity.get_component<TransformComponent>() };

			double entity_x{ static_cast<double>(transfrom.position.x) };
			double entity_y{ static_cast<double>(transfrom.position.y) };

			double w{ static_cast<double>(camera->w) };
			double h{ static_cast<double>(camera->h) };

			double mid_x{ w / 2.0 };
			double mid_y{ h / 2.0 };

			double new_x{ entity_x - mid_x };
			double new_y{ entity_y - mid_y };

			if (new_x < 0.0) {
				new_x = 0.0;
			}
			else if (new_x + w > static_cast<double>(Game::map_width)) {
				new_x = static_cast<double>(Game::map_width) - w;
			}

			if (new_y < 0) {
				new_y = 0;
			}
			else if (new_y + h > static_cast<double>(Game::map_height)) {
				new_y = static_cast<double>(Game::map_height) - h;
			}

			camera->x = static_cast<int>(std::round(new_x));
			camera->y = static_cast<int>(std::round(new_y));
		}
	}
};

#endif //CAMERA_MOVEMENT_SYSTEM_HPP
