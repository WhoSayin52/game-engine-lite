#ifndef GAME_HPP
#define GAME_HPP

struct SDL_Window;
struct SDL_Renderer;

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
};

#endif //GAME_HPP
