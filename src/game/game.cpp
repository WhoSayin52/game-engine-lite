#include "game.hpp"

#include "../components/transform_component.hpp"
#include "../components/rigidbody_component.hpp"
#include "../components/sprite_component.hpp"
#include "../components/animation_component.hpp"

#include "../systems/animation_system.hpp"
#include "../systems/movement_system.hpp"
#include "../systems/render_system.hpp"

#include "../logger/logger.hpp"
#include "../ecs/ecs.hpp"

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <cstdio>

Game::Game()
{
	is_running = false;
	registry = std::make_unique<Registry>();
	asset_manager = std::make_unique<AssetManager>();
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

	//SDL_DisplayMode display_mode{};
	//SDL_GetCurrentDisplayMode(0, &display_mode);
	//window_width = display_mode.w;
	//window_height = display_mode.h;
	window = SDL_CreateWindow(
		nullptr,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		0
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
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

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

void Game::load_level([[maybe_unused]] int level) {
	registry->add_system<MovementSystem>();
	registry->add_system<RenderSystem>();
	registry->add_system<AnimationSystem>();

	asset_manager->add_texture(renderer, "chopper", "../assets/images/chopper.png");
	asset_manager->add_texture(renderer, "radar", "../assets/images/radar.png");
	asset_manager->add_texture(renderer, "tank_panther_right", "../assets/images/tank-panther-right.png");
	asset_manager->add_texture(renderer, "truck_ford_right", "../assets/images/truck-ford-right.png");

	asset_manager->add_map(renderer, "jungle_map", "../assets/tilemaps/jungle.map", "../assets/tilemaps/jungle.png");
	asset_manager->load_map(registry.get(), "jungle_map");

	Entity chopper = registry->create_entity();
	chopper.add_component<TransformComponent>(
		glm::dvec2(10.0, 10.0),
		glm::dvec2(1.0, 1.0),
		0.0
	);
	chopper.add_component<RigidBodyComponent>(glm::dvec2(0.0, 0.0));
	chopper.add_component<SpriteComponent>("chopper", 1);
	chopper.add_component<AnimationComponent>(2, 0.1, true);

	Entity radar = registry->create_entity();
	radar.add_component<TransformComponent>(
		glm::dvec2(window_width - 74, 10),
		glm::dvec2(1.0, 1.0),
		0.0
	);
	radar.add_component<SpriteComponent>("radar", 2, 64, 64);
	radar.add_component<AnimationComponent>(8, 0.15, true);


	Entity tank{ registry->create_entity() };
	tank.add_component<TransformComponent>(
		glm::dvec2(10.0, 30.0),
		glm::dvec2(1.0, 1.0),
		0.0
	);
	tank.add_component<RigidBodyComponent>(glm::dvec2(50.0, 0.0));
	tank.add_component<SpriteComponent>("tank_panther_right", 1);

	Entity truck{ registry->create_entity() };
	truck.add_component<TransformComponent>(
		glm::dvec2(300.0, 200.0),
		glm::dvec2(1.0, 1.0),
		0.0
	);
	truck.add_component<RigidBodyComponent>(glm::dvec2(-20.0, -10.0));
	truck.add_component<SpriteComponent>("truck_ford_right", 1);
}

void Game::setup() {
	load_level(0);
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

	registry->get_system<MovementSystem>().update(delta_time);
	registry->get_system<AnimationSystem>().update(delta_time);

	registry->update();
}

void Game::render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	registry->get_system<RenderSystem>().update(renderer, &(*asset_manager));

	SDL_RenderPresent(renderer);
}

void Game::destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

