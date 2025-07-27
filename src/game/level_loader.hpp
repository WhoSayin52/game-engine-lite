#ifndef LEVEL_LOADER_HPP
#define LEVEL_LOADER_HPP

#include "game.hpp"

#include "../asset_manager/asset_manager.hpp"
#include "../ecs/ecs.hpp"

#include <SDL2/SDL.h>

class LevelLoader {
public:
	LevelLoader();
	~LevelLoader();

	void load_level(SDL_Renderer* renderer, Registry* registry, AssetManager* asset_manager, int level);
};

#endif //LEVEL_LOADER_HPP
