#ifndef MOVEMENT_SYSTEM_HPP
#define MOVEMENT_SYSTEM_HPP

#include "../ecs/ecs.hpp"
#include "../components/transform_component.hpp"
#include "../components/rigidbody_component.hpp"
#include "../logger/logger.hpp"

class MovementSystem : public System {
public:
	MovementSystem();
	void update(double delta_time);
};

MovementSystem::MovementSystem() {
	require_component <TransformComponent>();
	require_component <RigidBodyComponent>();
}

void MovementSystem::update(double delta_time) {
	for (const Entity entity : get_entities()) {
		TransformComponent& transform{ entity.get_component<TransformComponent>() };
		RigidBodyComponent& rigidbody{ entity.get_component<RigidBodyComponent>() };

		transform.position.x += rigidbody.velocity.x * delta_time;
		transform.position.y += rigidbody.velocity.y * delta_time;

		Logger::log(
			"Entity id "
			+ std::to_string(entity.get_id())
			+ " position is ("
			+ std::to_string(transform.position.x)
			+ " , " 
			+ std::to_string(transform.position.y)
			+ ")"
		);
	}
}

#endif //MOVEMENT_SYSTEM_HPP
