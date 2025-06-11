#include "game.hpp"

#include "../logger/logger.hpp"
#include "../ecs/ecs.hpp"

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <cstdio>

Game::Game() :is_running{ false }
{
	Logger::log("Game constructor called!");
}

Game::~Game() {
	Logger::log("Game destructor called!");
}

void Game::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::string err(SDL_GetError());
		Logger::err("Failed to initialize SDL: SDL Error: " + err);
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
		std::string err{ SDL_GetError() };
		Logger::err("Failed to create window: SDL Error: " + err);
		return;
	}

	renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (renderer == nullptr) {
		std::string err{ SDL_GetError() };
		Logger::err("Failed to create renderer: SDL Error: " + err);
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

void Game::setup() {

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

	[[maybe_unused]] double delta_time = (SDL_GetTicks() - millisecs_prev_frame) / 1000.0;

	millisecs_prev_frame = SDL_GetTicks();
}

void Game::render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);



	SDL_RenderPresent(renderer);
}

void Game::destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

