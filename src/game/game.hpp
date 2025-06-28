#ifndef GAME_HPP
#define GAME_HPP

#include "../asset_manager/asset_manager.hpp"
#include "../ecs/ecs.hpp"
#include "../event_manager/event_manager.hpp"

#include <SDL2/SDL.h>

#include <memory>

struct SDL_Window;
struct SDL_Renderer;

constexpr int FPS{ 60 };
constexpr int MILLISECONDS_PRE_FRAME{ 1000 / FPS };

class Game {
public:
	static int window_width;
	static int window_height;
	static int map_width;
	static int map_height;

	Game();
	~Game();
	Game(const Game&) = delete;
	Game operator=(const Game&) = delete;
	void init();
	void run();
	void load_level(int level);
	void setup();
	void input();
	void update();
	void render();
	void destroy();

private:
	bool is_running{};
	bool is_debugging{};
	Uint32 millisecs_prev_frame{ 0 };
	SDL_Window* window{ nullptr };
	SDL_Renderer* renderer{ nullptr };
	SDL_Rect camera{};

	std::unique_ptr<Registry> registry{ nullptr };
	std::unique_ptr<AssetManager> asset_manager{ nullptr };
	std::unique_ptr<EventManager> event_manager{ nullptr };
};

#endif //GAME_HPP
