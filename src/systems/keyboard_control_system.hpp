#ifndef KEYBOARD_CONTROL_SYSTEM_HPP
#define KEYBOARD_CONTROL_SYSTEM_HPP

#include "../ecs/ecs.hpp"
#include "../event_manager/event_manager.hpp"
#include "../events/key_pressed_event.hpp"
#include "../components/keyboard_control_component.hpp"
#include "../components/sprite_component.hpp"
#include "../components/rigidbody_component.hpp"

#include <SDL2/SDL_events.h>

class KeyboarControlSystem : public System {
public:
	KeyboarControlSystem() {
		require_component<KeyboardControlComponent>();
		require_component<SpriteComponent>();
		require_component<RigidbodyComponent>();
	}

	void listen_to_event(EventManager& event_manager) {
		event_manager.listen<KeyboarControlSystem, KeyPressedEvent>(
			this,
			&KeyboarControlSystem::on_key_press
		);
	}

	void on_key_press(KeyPressedEvent& event) {
		for (Entity& entity : get_entities()) {
			KeyboardControlComponent& keyboard_control{ entity.get_component<KeyboardControlComponent>() };
			SpriteComponent& sprite{ entity.get_component<SpriteComponent>() };
			RigidbodyComponent& rigidbody{ entity.get_component<RigidbodyComponent>() };

			switch (event.key)
			{
			case SDLK_UP:
				rigidbody.velocity = keyboard_control.up_velocity;
				sprite.src_rect.y = 0 * sprite.height;
				break;

			case SDLK_RIGHT:
				rigidbody.velocity = keyboard_control.right_velocity;
				sprite.src_rect.y = 1 * sprite.height;
				break;

			case SDLK_DOWN:
				rigidbody.velocity = keyboard_control.down_velocity;
				sprite.src_rect.y = 2 * sprite.height;
				break;

			case SDLK_LEFT:
				rigidbody.velocity = keyboard_control.left_velocity;
				sprite.src_rect.y = 3 * sprite.height;
				break;

			default:
				return;
			}
		}
	}

	void update() {}
};

#endif //KEYBOARD_CONTROL_SYSTEM_HPP
