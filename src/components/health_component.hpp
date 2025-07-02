#ifndef HEALTH_COMPONENT_HPP
#define HEALTH_COMPONENT_HPP

struct HealthComponent {
	int health_percentage{};

	HealthComponent(int health_percentage = 100)
		: health_percentage{ health_percentage } {
	}
};

#endif //HEALTH_COMPONENT_HPP
