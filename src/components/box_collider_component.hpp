#ifndef BOX_COLLIDER_COMPONENT_HPP
#define BOX_COLLIDER_COMPONENT_HPP

#include <glm/glm.hpp>

struct BoxColliderComponent {
	int width{ 0 };
	int height{ 0 };
	glm::dvec2 offset{ 0.0, 0.0 };
	bool is_colliding{ false };

	BoxColliderComponent(
		int width = 0,
		int height = 0,
		glm::dvec2 offset = glm::dvec2(0.0)
	) :
		width{ width },
		height{ height },
		offset{ offset } {
	}
};


#endif //BOX_COLLIDER_COMPONENT_HPP
