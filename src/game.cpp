#include "game.hpp"

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <cstdio>

Game::Game() :is_running{ false }
{

}

Game::~Game() {

}

void Game::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Failed to initialize SDL: SDL Error: %s\n", SDL_GetError());
		return;
	}

	SDL_DisplayMode display_mode{};
	SDL_GetCurrentDisplayMode(0, &display_mode);
	window_width = display_mode.w;
	window_height = display_mode.h;
	window = SDL_CreateWindow(
		nullptr,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_BORDERLESS
	);

	if (window == nullptr) {
		fprintf(stderr, "Failed to create window: SDL Error: %s\n", SDL_GetError());
		return;
	}

	renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (renderer == nullptr) {
		fprintf(stderr, "Failed to create renderer: SDL Error: %s\n", SDL_GetError());
		return;
	}
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	is_running = true;
}

void Game::run() {
	setup();
	while (is_running) {
		input();
		update();
		render();
	}
}

glm::vec2 player_pos{};
glm::vec2 player_vel{};

void Game::setup() {
	player_pos = glm::vec2(10.0, 20.0);
	player_vel = glm::vec2(100.0, 0);
}

void Game::input() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			is_running = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) is_running = false;
			break;
		}
	}

}

void Game::update() {
	Uint32 time_to_wait = MILLISECONDS_PRE_FRAME - (SDL_GetTicks() - millisecs_prev_frame);
	if (time_to_wait <= MILLISECONDS_PRE_FRAME) SDL_Delay(time_to_wait);

	double delta_time = (SDL_GetTicks() - millisecs_prev_frame) / 1000.0;

	millisecs_prev_frame = SDL_GetTicks();

	player_pos.x += static_cast<float>(player_vel.x * delta_time);
	player_pos.y += static_cast<float>(player_vel.y * delta_time);
}

void Game::render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	//Draw a PNG texture
	SDL_Surface* surface{ IMG_Load("../assets/images/tank-tiger-right.png") };
	SDL_Texture* texture{ SDL_CreateTextureFromSurface(renderer, surface) };
	SDL_FreeSurface(surface);
	surface = nullptr;

	SDL_Rect dest_rect{
		static_cast<int>(player_pos.x),
		static_cast<int>(player_pos.y),
		32,
		32 };
	SDL_RenderCopy(renderer, texture, nullptr, &dest_rect);
	SDL_DestroyTexture(texture);

	SDL_RenderPresent(renderer);
}

void Game::destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

