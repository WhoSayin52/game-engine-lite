#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include <glm/glm.hpp>

struct TransformComponent {
	glm::dvec2 position{};
	glm::dvec2 scale{ 1.0, 1.0 };
	double rotation{};

	TransformComponent(
		glm::dvec2 position = glm::dvec2(0, 0),
		glm::dvec2 scale = glm::dvec2(1.0, 1.0),
		double rotation = 0.0)
		: position{ position }, scale{ scale }, rotation{ rotation } {
	}
};

#endif //TRANSFORM_COMPONENT_HPP
