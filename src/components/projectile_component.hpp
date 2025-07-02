#ifndef PROJECTILE_COMPONENT_HPP
#define PROJECTILE_COMPONENT_HPP

struct ProjectileComponent {
	int damage{};
	double duration{};
	double elapsed_seconds{ 0 };
	bool is_friendly{};


	ProjectileComponent(int damage = 10, double duration = 1, bool is_friendly = false)
		: damage{ damage }, duration{ duration },
		is_friendly{ is_friendly } {
	}
};

#endif //PROJECTILE_COMPONENT_HPP
