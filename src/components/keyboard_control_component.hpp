#ifndef KEYBOARD_CONTROL_COMPONENT_HPP
#define KEYBOARD_CONTROL_COMPONENT_HPP

#include <glm/glm.hpp>	

struct KeyboardControlComponent {

	glm::dvec2 up_velocity{};
	glm::dvec2 right_velocity{};
	glm::dvec2 down_velocity{};
	glm::dvec2 left_velocity{};

	KeyboardControlComponent(
		glm::dvec2 up_velocity = { 0.0, 0.0 },
		glm::dvec2 right_velocity = { 0.0, 0.0 },
		glm::dvec2 down_velocity = { 0.0, 0.0 },
		glm::dvec2 left_velocity = { 0.0, 0.0 }
	) :
		up_velocity{ up_velocity },
		right_velocity{ right_velocity },
		down_velocity{ down_velocity },
		left_velocity{ left_velocity }
	{
	}
};

#endif //KEYBOARD_CONTROL_COMPONENT_HPP
