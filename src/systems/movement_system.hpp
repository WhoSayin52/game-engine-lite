#ifndef MOVEMENT_SYSTEM_HPP
#define MOVEMENT_SYSTEM_HPP

#include "../game/game.hpp"
#include "../ecs/ecs.hpp"
#include "../event_manager/event_manager.hpp"
#include "../events/collision_event.hpp"
#include "../components/transform_component.hpp"
#include "../components/rigidbody_component.hpp"
#include "../components/sprite_component.hpp"

class MovementSystem : public System {
public:
	MovementSystem() {
		require_component<TransformComponent>();
		require_component<RigidbodyComponent>();
	}

	void listen_to_event(EventManager& event_manager) {
		event_manager.listen<MovementSystem, CollisionEvent>(this, &MovementSystem::on_collision);
	}

	void on_collision(CollisionEvent& event) {
		Logger::log("Collision " + std::to_string(event.a.get_id()) + " and " + std::to_string(event.b.get_id()));

		Entity& a{ event.a };
		Entity& b{ event.b };

		if (a.belong_to_group("enemies") && b.belong_to_group("obstacles")) {
			move_opposite_direction(a, b);
		}
		else if (b.belong_to_group("enemies") && a.belong_to_group("obstacles")) {
			move_opposite_direction(b, a);
		}
	}

	void update(double delta_time) {
		for (Entity& entity : get_entities()) {
			TransformComponent& transform{ entity.get_component<TransformComponent>() };
			RigidbodyComponent& rigidbody{ entity.get_component<RigidbodyComponent>() };

			transform.position.x += rigidbody.velocity.x * delta_time;
			transform.position.y += rigidbody.velocity.y * delta_time;

			bool is_outside_map{
				transform.position.x < 0 ||
				transform.position.x > Game::map_width ||
				transform.position.y < 0 ||
				transform.position.y > Game::map_height
			};
			if (is_outside_map && !entity.has_tag("player")) {
				entity.free();
			}
		}
	}

private:
	void move_opposite_direction(Entity& enemy, Entity& obstacle) {
		(void)obstacle;
		if (!enemy.has_component<RigidbodyComponent>() ||
			!enemy.has_component<SpriteComponent>()) {
			return;
		}
		auto& rigidbody{ enemy.get_component<RigidbodyComponent>() };
		auto& sprite{ enemy.get_component<SpriteComponent>() };

		if (rigidbody.velocity.x != 0) {
			rigidbody.velocity.x *= -1;
			sprite.flip = (sprite.flip == SDL_FLIP_NONE) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
		}
		else if (rigidbody.velocity.y != 0) {
			rigidbody.velocity.y *= -1;
			sprite.flip = (sprite.flip == SDL_FLIP_NONE) ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
		}
	}
};

#endif //MOVEMENT_SYSTEM_HPP
