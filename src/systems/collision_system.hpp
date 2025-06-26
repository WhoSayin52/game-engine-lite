#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include "../ecs/ecs.hpp"
#include "../components/box_collider_component.hpp"
#include "../components/transform_component.hpp"
#include "../event_manager/event_manager.hpp"
#include "../events/collision_event.hpp"

class CollisionSystem : public System {
public:
	CollisionSystem() {
		require_component<BoxColliderComponent>();
		require_component<TransformComponent>();
	}

	void update(EventManager& event_manager) {
		auto& entities{ get_entities() };

		for (auto i{ entities.begin() }; i != entities.end(); ++i) {
			Entity& e{ *i };

			const TransformComponent& transform{ e.get_component<TransformComponent>() };
			BoxColliderComponent& collider{ e.get_component<BoxColliderComponent>() };

			for (auto j{ i + 1 }; j != entities.end(); ++j) {
				Entity& other{ *j };

				const TransformComponent& other_transform{ other.get_component<TransformComponent>() };
				BoxColliderComponent& other_collider{ other.get_component<BoxColliderComponent>() };

				bool is_colliding{ check_collision(transform, collider, other_transform, other_collider) };

				if (is_colliding) {
					event_manager.emit<CollisionEvent>(e, other);

					collider.is_colliding = true;
					other_collider.is_colliding = true;
				}
				else {
					collider.is_colliding = false;
					other_collider.is_colliding = false;
				}
			}
		}
	}

private:
	bool check_collision(
		const TransformComponent& a_transform,
		const BoxColliderComponent& a_collider,
		const TransformComponent& b_transform,
		const BoxColliderComponent& b_collider
	) {

		int a_x1{ static_cast<int>(a_transform.position.x + a_collider.offset.x) };
		int a_x2{ static_cast<int>(a_transform.position.x + a_collider.offset.x + a_collider.width) };
		int a_y1{ static_cast<int>(a_transform.position.y + a_collider.offset.y) };
		int a_y2{ static_cast<int>(a_transform.position.y + a_collider.offset.y + a_collider.height) };

		int b_x1{ static_cast<int>(b_transform.position.x + b_collider.offset.x) };
		int b_x2{ static_cast<int>(b_transform.position.x + b_collider.offset.x + b_collider.width) };
		int b_y1{ static_cast<int>(b_transform.position.y + b_collider.offset.y) };
		int b_y2{ static_cast<int>(b_transform.position.y + b_collider.offset.y + b_collider.height) };

		bool x_collision{
			(a_x1 <= b_x1 && b_x1 <= a_x2) ||
			(a_x1 <= b_x2 && b_x2 <= a_x2)
		};

		bool y_collision{
			(a_y1 <= b_y1 && b_y1 <= a_y2) ||
			(a_y1 <= b_y2 && b_y2 <= a_y2)
		};

		return x_collision && y_collision;
	}
};

#endif //COLLISION_SYSTEM_HPP
