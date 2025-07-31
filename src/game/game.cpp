#include "game.hpp"

#include "level_loader.hpp"

#include "../logger/logger.hpp"

#include "../systems/animation_system.hpp"
#include "../systems/camera_movement_system.hpp"
#include "../systems/collision_system.hpp"
#include "../systems/damage_system.hpp"
#include "../systems/keyboard_control_system.hpp"
#include "../systems/movement_system.hpp"
#include "../systems/projectile_duration_system.hpp"
#include "../systems/projectile_emit_system.hpp"
#include "../systems/render_collision_system.hpp"
#include "../systems/render_gui_system.hpp"
#include "../systems/render_health_system.hpp"
#include "../systems/render_system.hpp"
#include "../systems/render_text_system.hpp"
#include "../systems/script_system.hpp"

#include <SDL2/SDL_image.h>

#include <glm/glm.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_sdlrenderer2.h>

int Game::window_width{};
int Game::window_height{};
int Game::map_width{};
int Game::map_height{};

Game::Game()
{
	is_running = false;
	registry = std::make_unique<Registry>();
	asset_manager = std::make_unique<AssetManager>();
	event_manager = std::make_unique<EventManager>();
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

	if (TTF_Init() != 0) {
		std::string err(SDL_GetError());
		Logger::err("Failed to initialize SDL_TTF: SDL Error: " + err);
		return;
	}

	//SDL_DisplayMode display_mode{};
	//SDL_GetCurrentDisplayMode(0, &display_mode);
	//window_width = display_mode.w;
	//window_height = display_mode.h;
	window_width = 800;
	window_height = 600;

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
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	//SDL_RenderSetScale(renderer, 2.0f, 2.0f);

	//Dear ImGui init
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);

	// Initialize the camera view with the entire screen area.
	// Division by 2 is if SDL_RenderSetScale() above.
	camera.x = 0;
	camera.y = 0;
	camera.w = window_width;
	camera.h = window_height;

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
	//Add systems
	registry->add_system<AnimationSystem>();
	registry->add_system<CameraMovementSystem>();
	registry->add_system<CollisionSystem>();
	registry->add_system<DamageSystem>();
	registry->add_system<KeyboarControlSystem>();
	registry->add_system<MovementSystem>();
	registry->add_system<RenderSystem>();
	registry->add_system<RenderHealthSystem>();
	registry->add_system<RenderTextSystem>();
	registry->add_system<RenderCollisionSystem>();
	registry->add_system<RenderGuiSystem>();
	registry->add_system<ProjectileDurationSystem>();
	registry->add_system<ProjectileEmitSystem>();
	registry->add_system<ScriptSystem>();

	// Creating Lua bindings
	registry->get_system<ScriptSystem>().create_lua_bindings(lua);

	LevelLoader loader{};
	lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os);
	loader.load_level(lua, renderer, registry.get(), asset_manager.get(), 2);
}

void Game::input() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		//ImGui SDL input
		ImGui_ImplSDL2_ProcessEvent(&event);
		ImGuiIO& io{ ImGui::GetIO() };

		int mouse_x, mouse_y;
		const Uint32 buttons{ SDL_GetMouseState(&mouse_x, &mouse_y) };
		io.MousePos = ImVec2(static_cast<float>(mouse_x), static_cast<float>(mouse_y));
		io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
		io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

		//Handle core SDL event
		switch (event.type) {
		case SDL_QUIT:
			is_running = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) is_running = false;
			if (event.key.keysym.sym == SDLK_F1) is_debugging = !is_debugging;

			event_manager->emit<KeyPressedEvent>(event.key.keysym.sym);

			break;
		}
	}

}

void Game::update() {
	Uint32 time_to_wait = MILLISECONDS_PRE_FRAME - (SDL_GetTicks() - millisecs_prev_frame);
	if (time_to_wait <= MILLISECONDS_PRE_FRAME) SDL_Delay(time_to_wait);

	[[maybe_unused]] double delta_time = (SDL_GetTicks() - millisecs_prev_frame) / 1000.0;

	millisecs_prev_frame = SDL_GetTicks();

	event_manager->reset();

	registry->get_system<MovementSystem>().listen_to_event(*event_manager);
	registry->get_system<DamageSystem>().listen_to_event(*event_manager);
	registry->get_system<KeyboarControlSystem>().listen_to_event(*event_manager);

	registry->get_system<AnimationSystem>().update(delta_time);
	registry->get_system<CollisionSystem>().update(*event_manager);
	registry->get_system<MovementSystem>().update(delta_time);
	registry->get_system<ScriptSystem>().update(delta_time, SDL_GetTicks());
	registry->get_system<CameraMovementSystem>().update(&camera);
	registry->get_system<ProjectileDurationSystem>().update(delta_time);
	registry->get_system<ProjectileEmitSystem>().update(*registry, delta_time);

	registry->update();
}

void Game::render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	registry->get_system<RenderSystem>().update(renderer, *asset_manager, &camera);
	registry->get_system<RenderHealthSystem>().update(renderer, *asset_manager, &camera);
	registry->get_system<RenderTextSystem>().update(renderer, *asset_manager, &camera);

	if (is_debugging) {
		registry->get_system<RenderCollisionSystem>().update(renderer, &camera);
		registry->get_system<RenderGuiSystem>().update(renderer, *registry, camera);
	}

	SDL_RenderPresent(renderer);
}

void Game::destroy() {
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	//TTF_Quit();
	SDL_Quit();
}

