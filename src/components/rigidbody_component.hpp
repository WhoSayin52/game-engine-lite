#ifndef RIGIDBODY_COMPONENT_HPP
#define RIGIDBODY_COMPONENT_HPP

#include <glm/glm.hpp>

struct RigidBodyComponent {
	glm::dvec2 velocity{};

	RigidBodyComponent(glm::dvec2 velocity = glm::dvec2(0.0, 0.0))
		: velocity{ velocity } {
	}
};

#endif //RIGIDBODY_COMPONENT_HPP
