#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include <glm/glm.hpp>
#include <SDL2/SDL_ttf.h>

#include <unordered_map>
#include <vector>
#include <string>

struct SDL_Texture;
struct SDL_Renderer;

class Registry;

class AssetManager {
public:
	AssetManager();
	~AssetManager();

	void clear_assets();

	//Textures
	void add_texture(SDL_Renderer* renderer, const std::string& asset_id, const std::string& path);
	SDL_Texture* get_texture(const std::string& asset_id) const;

	//Maps
	void add_map(SDL_Renderer* renderer, const std::string asset_id, const std::string& map_path, const std::string& sheet_path);
	void load_map(Registry* registry, const std::string& asset_id);

	//Fonts
	void add_font(const std::string& asset_id, const std::string& path, int font_size);
	TTF_Font* get_font(const std::string& asset_id);

private:
	//Textures
	std::unordered_map<std::string, SDL_Texture*> textures{};

	//Maps
	std::unordered_map<std::string, std::vector<std::vector<glm::ivec2>>> maps{};

	//Fonts
	std::unordered_map<std::string, TTF_Font*> fonts{};
};

#endif //ASSET_MANAGER_HPP
