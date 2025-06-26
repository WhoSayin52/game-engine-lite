#ifndef DAMAGE_SYSTEM_HPP
#define DAMAGE_SYSTEM_HPP

#include "../ecs/ecs.hpp"
#include "../components/box_collider_component.hpp"
#include "../event_manager/event_manager.hpp"
#include "../events/collision_event.hpp"
#include "../logger/logger.hpp"

class DamageSystem : public System {
public:
	DamageSystem() {
		require_component<BoxColliderComponent>();
	}

	void listen_to_event(EventManager& event_manager) {
		event_manager.listen<DamageSystem, CollisionEvent>(this, &DamageSystem::on_collision);
	}

	void on_collision(CollisionEvent& event) {
		Logger::log("DMG " + std::to_string(event.a.get_id()) + " and " + std::to_string(event.b.get_id()));

		event.a.free();
		event.b.free();
	}

	void update() {}
};

#endif //DAMAGE_SYSTEM_HPP
