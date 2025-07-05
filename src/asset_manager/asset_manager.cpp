#include "asset_manager.hpp"

#include "../game/game.hpp"
#include "../logger/logger.hpp"
#include "../ecs/ecs.hpp"
#include "../components/transform_component.hpp"
#include "../components/sprite_component.hpp"

#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>

#include <fstream>
#include <sstream>

AssetManager::AssetManager() {
	Logger::log("Asset manager created!");
}

AssetManager::~AssetManager() {
	clear_assets();
	Logger::log("Asset manager destroyed!");
}

void AssetManager::clear_assets() {
	for (auto it{ textures.begin() }; it != textures.end();) {
		SDL_DestroyTexture(it->second);
		it->second = nullptr;
		it = textures.erase(it);
	}

	maps.clear();
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

void AssetManager::add_map(SDL_Renderer* renderer, const std::string asset_id, const std::string& map_path, const std::string& sheet_path) {

	std::ifstream file{ map_path };

	if (!file) {
		Logger::err("Failed to open map at path: " + map_path);
		return;
	}

	std::vector<std::vector<glm::ivec2>> map{};
	std::string line{};
	std::string token{};

	while (std::getline(file, line)) {

		std::istringstream stream{ line };
		map.push_back({});

		while (std::getline(stream, token, ',')) {

			glm::ivec2 pos{ token[1] - '0', token[0] - '0' };

			map.back().push_back(pos);
		}
	}

	maps.insert(std::make_pair(asset_id, map));
	add_texture(renderer, asset_id, sheet_path);
}

void AssetManager::load_map(Registry* registry, const std::string& asset_id) {

	auto it{ maps.find(asset_id) };
	if (it == maps.end()) {
		Logger::err("Map " + asset_id + " not found in maps!");
		return;
	}

	std::vector<std::vector<glm::ivec2>>& map{ it->second };

	constexpr double tile_scale{ 1.0 };

	for (std::size_t row{}; row < map.size(); ++row) {
		for (std::size_t col{}; col < map[row].size(); ++col) {

			glm::ivec2 pos{ map[row][col] };

			Entity tile{ registry->create_entity() };
			tile.add_group("tiles");

			tile.add_component<TransformComponent>(
				glm::dvec2(
					static_cast<double>(col) * sprite_config::width * tile_scale,
					static_cast<double>(row) * sprite_config::height * tile_scale
				),
				glm::dvec2(tile_scale, tile_scale),
				0.0
			);

			tile.add_component<SpriteComponent>(
				asset_id,
				0,
				false,
				sprite_config::width,
				sprite_config::height,
				sprite_config::width * pos.x,
				sprite_config::height * pos.y
			);
		}
	}

	Game::map_width = static_cast<int>(map.back().size()) * static_cast<int>(tile_scale) * sprite_config::height;
	Game::map_height = static_cast<int>(map.size()) * static_cast<int>(tile_scale) * sprite_config::width;
}
