#ifndef PROJECTILE_EMIT_SYSTEM_HPP
#define PROJECTILE_EMIT_SYSTEM_HPP

#include "../ecs/ecs.hpp"
#include "../components/projectile_emitter_component.hpp"
#include "../components/transform_component.hpp"
#include "../components/rigidbody_component.hpp"
#include "../components/sprite_component.hpp"
#include "../components/box_collider_component.hpp"
#include "../components/projectile_component.hpp"
#include "../components/keyboard_control_component.hpp"

class Registry;

class ProjectileEmitSystem : public System {
public:
	ProjectileEmitSystem() {
		require_component<ProjectileEmitterComponent>();
		require_component<TransformComponent>();
	}



	void update(Registry& registry, double delta_time) {

		for (Entity& entity : get_entities()) {

			if (entity.has_component<KeyboardControlComponent>()) {
				continue;
			}

			auto& emitter{ entity.get_component<ProjectileEmitterComponent>() };
			auto& transform{ entity.get_component<TransformComponent>() };

			emitter.elapsed_seconds += delta_time;

			if (emitter.elapsed_seconds >= emitter.emission_delay) {
				emitter.elapsed_seconds -= emitter.emission_delay;

				glm::dvec2 projectile_pos{ transform.position };

				if (entity.has_component<SpriteComponent>()) {
					auto& sprite{ entity.get_component<SpriteComponent>() };
					projectile_pos.x += transform.scale.x * sprite.width / 2;
					projectile_pos.y += transform.scale.y * sprite.height / 2;
				}

				Entity projectile{ registry.create_entity() };
				projectile.add_component<TransformComponent>(projectile_pos);
				projectile.add_component<RigidbodyComponent>(emitter.velocity);
				projectile.add_component<SpriteComponent>("bullet", 3, false, 4, 4);
				projectile.add_component<BoxColliderComponent>(4, 4);
				projectile.add_component<ProjectileComponent>(
					emitter.damage,
					emitter.duration,
					emitter.is_friendly
				);
			}
		}
	}
};

#endif //PROJECTILE_EMIT_SYSTEM_HPP
