#include "asset_manager.hpp"

#include "../logger/logger.hpp"

#include <SDL2/SDL_image.h>

AssetManager::AssetManager() {
	Logger::log("Asset manager created!");
}

AssetManager::~AssetManager() {
	clear_assets();
	Logger::log("Asset manager destroyed!");
}

void AssetManager::clear_assets() {
	for (auto it{ textures.begin() }; it != textures.end(); ++it) {
		SDL_DestroyTexture(it->second);
		it->second = nullptr;
		it = textures.erase(it);
	}
}

void AssetManager::add_texture(SDL_Renderer* renderer, const std::string& asset_id, const std::string& path) {
	SDL_Texture* texture{ IMG_LoadTexture(renderer, path.c_str()) };
	textures.insert(std::make_pair(asset_id, texture));

	Logger::log("New texture added to the asset store. Id: " + asset_id);
}

SDL_Texture* AssetManager::get_texture(const std::string& asset_id) const {
	auto iterator{ textures.find(asset_id) };
	if (iterator == textures.end()) {
		return nullptr;
	}
	else {
		return iterator->second;
	}
}
