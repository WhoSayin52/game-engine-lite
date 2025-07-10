#ifndef TEXT_LABEL_COMPONENT_HPP
#define TEXT_LABEL_COMPONENT_HPP

#include <glm/glm.hpp>	
#include <SDL2/SDL.h>

#include<string>

struct TextLabelComponent {

	glm::ivec2 position{};
	std::string text{};
	std::string asset_id{};
	SDL_Color color{};
	bool is_fixed{};

	TextLabelComponent(
		glm::ivec2 position = { 0, 0 },
		std::string text = "Insert Text",
		std::string asset_id = "arial",
		SDL_Color color = { 255, 255, 255, 255 },
		bool is_fixed = true
	) :
		position{ position },
		text{ text },
		asset_id{ asset_id },
		color{ color },
		is_fixed{ is_fixed }
	{
	}

};

#endif //TEXT_LABEL_COMPONENT_HPP
