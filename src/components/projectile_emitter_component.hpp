#ifndef PROJECTILE_EMITTER_COMPONENT_HPP
#define PROJECTILE_EMITTER_COMPONENT_HPP

#include <glm/glm.hpp>

struct ProjectileEmitterComponent {

	glm::dvec2 velocity{};
	int damage{};
	double emission_delay{};
	double duration{};
	double elapsed_seconds{};
	bool is_friendly{};

	ProjectileEmitterComponent(
		glm::dvec2 velocity = { 50.0, 50.0 },
		int damage = 1,
		double emission_delay = 2.0,
		double duration = 10.0,
		bool is_friendly = false
	) :
		velocity{ velocity },
		damage{ damage },
		emission_delay{ emission_delay },
		duration{ duration },
		is_friendly{ is_friendly } {
	}
};

#endif //PROJECTILE_EMITTER_COMPONENT_HPP
