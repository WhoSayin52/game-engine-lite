#ifndef PROJECTILE_DURATION_SYSTEM_HPP
#define PROJECTILE_DURATION_SYSTEM_HPP

#include "../ecs/ecs.hpp"
#include "../components/projectile_component.hpp"

class ProjectileDurationSystem : public System {
public:
	ProjectileDurationSystem() {
		require_component<ProjectileComponent>();
	}

	void update(double delta_time) {

		for (Entity& e : get_entities()) {

			auto& projectile{ e.get_component<ProjectileComponent>() };

			if (projectile.elapsed_seconds >= projectile.duration) {
				e.free();
			}
			else {
				projectile.elapsed_seconds += delta_time;
			}
		}
	}
};

#endif //PROJECTILE_DURATION_SYSTEM_HPP
