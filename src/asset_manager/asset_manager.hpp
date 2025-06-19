#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include <unordered_map>
#include <string>
#include <string_view>

struct SDL_Texture;
struct SDL_Renderer;

class AssetManager {
public:
	AssetManager();
	~AssetManager();

	void clear_assets();
	void add_texture(SDL_Renderer* renderer, const std::string& asset_id, const std::string& path);
	SDL_Texture* get_texture(const std::string& asset_id) const;

private:
	std::unordered_map<std::string, SDL_Texture*> textures{};
};

#endif //ASSET_MANAGER_HPP
