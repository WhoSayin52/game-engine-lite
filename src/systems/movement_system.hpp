#ifndef MOVEMENT_SYSTEM_HPP
#define MOVEMENT_SYSTEM_HPP

#include "../ecs/ecs.hpp"
#include "../components/transform_component.hpp"
#include "../components/rigidbody_component.hpp"

class MovementSystem : public System {
public:
	MovementSystem() {
		require_component<TransformComponent>();
		require_component<RigidBodyComponent>();
	}

	void update(double delta_time) {
		for (Entity& entity : get_entities()) {
			TransformComponent& transform{ entity.get_component<TransformComponent>() };
			RigidBodyComponent& rigidbody{ entity.get_component<RigidBodyComponent>() };

			transform.position.x += rigidbody.velocity.x * delta_time;
			transform.position.y += rigidbody.velocity.y * delta_time;
		}
	}
};

#endif //MOVEMENT_SYSTEM_HPP
