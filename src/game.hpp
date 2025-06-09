#ifndef GAME_HPP
#define GAME_HPP

#include <SDL2/SDL_stdinc.h>

struct SDL_Window;
struct SDL_Renderer;

constexpr int FPS{ 60 };
constexpr int MILLISECONDS_PRE_FRAME{ 1000 / FPS };

class Game {
public:
	Game();
	~Game();
	Game(const Game&) = delete;
	Game operator=(const Game&) = delete;
	void init();
	void run();
	void setup();
	void input();
	void update();
	void render();
	void destroy();

	int window_width{ 800 };
	int window_height{ 600 };

private:
	SDL_Window* window{ nullptr };
	SDL_Renderer* renderer{ nullptr };
	bool is_running{};
	Uint32 millisecs_prev_frame{ 0 };
};

#endif //GAME_HPP
