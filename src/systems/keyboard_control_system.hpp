#ifndef KEYBOARD_CONTROL_SYSTEM_HPP
#define KEYBOARD_CONTROL_SYSTEM_HPP

#include "../ecs/ecs.hpp"
#include "../event_manager/event_manager.hpp"
#include "../events/key_pressed_event.hpp"
#include "../components/keyboard_control_component.hpp"
#include "../components/sprite_component.hpp"
#include "../components/rigidbody_component.hpp"
#include "../components/projectile_emitter_component.hpp"
#include "../components/transform_component.hpp"
#include "../components/projectile_component.hpp"

#include <SDL2/SDL_events.h>
#include <glm/glm.hpp>

class KeyboarControlSystem : public System {
public:
	KeyboarControlSystem() {
		require_component<KeyboardControlComponent>();
		require_component<SpriteComponent>();
		require_component<RigidbodyComponent>();
		require_component<ProjectileEmitterComponent>();
		require_component<TransformComponent>();
	}

	void listen_to_event(EventManager& event_manager) {
		event_manager.listen<KeyboarControlSystem, KeyPressedEvent>(
			this,
			&KeyboarControlSystem::on_key_press
		);
	}

	void on_key_press(KeyPressedEvent& event) {
		player_movement(event);
		player_fire(event);
	}

	void update() {}

private:
	void player_movement(KeyPressedEvent& event);
	void player_fire(KeyPressedEvent& event);
};

inline void KeyboarControlSystem::player_movement(KeyPressedEvent& event) {
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

inline void KeyboarControlSystem::player_fire(KeyPressedEvent& event) {
	if (!(event.key == SDLK_SPACE)) {
		return;
	}

	for (Entity& entity : get_entities()) {

		auto& emitter{ entity.get_component<ProjectileEmitterComponent>() };
		auto& transform{ entity.get_component<TransformComponent>() };
		auto& rigidbody{ entity.get_component<RigidbodyComponent>() };

		glm::dvec2 projectile_pos{ transform.position };

		if (entity.has_component<SpriteComponent>()) {
			auto& sprite{ entity.get_component<SpriteComponent>() };
			projectile_pos.x += transform.scale.x * sprite.width / 2;
			projectile_pos.y += transform.scale.y * sprite.height / 2;
		}

		glm::dvec2 player_velocity{ rigidbody.velocity };
		glm::dvec2 direction{ 0.0, 0.0 };

		if (player_velocity.x > 0) {
			direction.x = 1;
		}
		else if (player_velocity.x < 0) {
			direction.x = -1;
		}

		if (player_velocity.y > 0) {
			direction.y = 1;
		}
		else if (player_velocity.y < 0) {
			direction.y = -1;
		}

		Entity projectile{ entity.registry->create_entity() };
		projectile.add_group("projectiles");
		projectile.add_component<TransformComponent>(projectile_pos);
		projectile.add_component<RigidbodyComponent>(emitter.velocity * direction);
		projectile.add_component<SpriteComponent>("bullet", 3, false, 4, 4);
		projectile.add_component<BoxColliderComponent>(4, 4);
		projectile.add_component<ProjectileComponent>(
			emitter.damage,
			emitter.duration,
			emitter.is_friendly
		);
	}
}


#endif //KEYBOARD_CONTROL_SYSTEM_HPP
