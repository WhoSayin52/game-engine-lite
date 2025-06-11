#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include <glm/glm.hpp>

struct TransformComponent {
	glm::vec2 position{};
	glm::vec2 scale{};
	double rotation{};
};

#endif //TRANSFORM_COMPONENT_HPP
