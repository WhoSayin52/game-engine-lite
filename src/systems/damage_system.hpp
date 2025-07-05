#ifndef DAMAGE_SYSTEM_HPP
#define DAMAGE_SYSTEM_HPP

#include "../ecs/ecs.hpp"
#include "../components/box_collider_component.hpp"
#include "../components/projectile_component.hpp"
#include "../components/health_component.hpp"
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
		Logger::log("Collision " + std::to_string(event.a.get_id()) + " and " + std::to_string(event.b.get_id()));

		Entity& a{ event.a };
		Entity& b{ event.b };

		if (a.belong_to_group("projectiles") && b.has_tag("player")) {
			projectile_damage(a, b);
		}
		else if (b.belong_to_group("projectiles") && a.has_tag("player")) {
			projectile_damage(b, a);
		}
		else if (a.belong_to_group("projectiles") && b.belong_to_group("enemies")) {
			projectile_damage(a, b);
		}
		else if (b.belong_to_group("projectiles") && a.belong_to_group("enemies")) {
			projectile_damage(b, a);
		}
	}

	void update() {}

private:
	void projectile_damage(Entity& projectile, Entity entity) {
		auto& p{ projectile.get_component<ProjectileComponent>() };
		auto& h{ entity.get_component<HealthComponent>() };

		if (!p.is_friendly && entity.has_tag("player")) {
			h.health -= p.damage;
			projectile.free();
		}
		else if (p.is_friendly && entity.belong_to_group("enemies")) {
			h.health -= p.damage;
			projectile.free();
		}

		if (h.health <= 0) {
			entity.free();
		}
	}
};

#endif //DAMAGE_SYSTEM_HPP
