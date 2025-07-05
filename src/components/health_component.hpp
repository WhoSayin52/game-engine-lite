#ifndef HEALTH_COMPONENT_HPP
#define HEALTH_COMPONENT_HPP

struct HealthComponent {
	int health{};

	HealthComponent(int health = 100)
		: health{ health } {
	}
};

#endif //HEALTH_COMPONENT_HPP
