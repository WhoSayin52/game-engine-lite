#ifndef KEYBOARD_MOVEMENT_SYSTEM_HPP
#define KEYBOARD_MOVEMENT_SYSTEM_HPP

#include "../ecs/ecs.hpp"
#include "../event_manager/event_manager.hpp"
#include "../events/key_pressed_event.hpp"

#include <SDL2/SDL_events.h>

class KeyboardMovementSystem : public System {
public:
	KeyboardMovementSystem() = default;

	void listen_to_event(EventManager& event_manager) {
		event_manager.listen<KeyboardMovementSystem, KeyPressedEvent>(
			this,
			&KeyboardMovementSystem::on_key_press
		);
	}

	void on_key_press(KeyPressedEvent& event) {
		std::string keycode{ std::to_string(event.key) };
		std::string key(1, static_cast<char>(event.key));
		Logger::log("Key pressed event: [" + keycode + "] " + key);
	}

	void update() {}
};

#endif //KEYBOARD_MOVEMENT_SYSTEM_HPP
