#include "level_loader.hpp"

#include "../components/animation_component.hpp"
#include "../components/box_collider_component.hpp"
#include "../components/camera_component.hpp"
#include "../components/health_component.hpp"
#include "../components/keyboard_control_component.hpp"
#include "../components/projectile_component.hpp"
#include "../components/projectile_emitter_component.hpp"
#include "../components/rigidbody_component.hpp"
#include "../components/sprite_component.hpp"
#include "../components/text_label_component.hpp"
#include "../components/transform_component.hpp"

LevelLoader::LevelLoader() {

}

LevelLoader::~LevelLoader() {

}

void LevelLoader::load_level(sol::state& lua, SDL_Renderer* renderer, Registry* registry, AssetManager* asset_manager, int level_num) {

	(void)renderer; (void)registry; (void)asset_manager;

	sol::load_result script{ lua.load_file("../assets/scripts/level" + std::to_string(level_num) + ".lua") };

	//Check syntax of script without executing
	if (!script.valid()) {
		sol::error err{ script };
		std::string err_msg{ err.what() };
		Logger::err("Lua script error detected: " + err_msg);
		exit(EXIT_FAILURE);
	}

	//Executes lua script
	lua.script_file("../assets/scripts/level" + std::to_string(level_num) + ".lua");

	//Read level data table from lua script
	sol::table level{ lua["Level"] };

	//Read level assets
	sol::table assets{ level["assets"] };

	int i{ 0 };
	while (true) {
		sol::optional<sol::table> has_asset{ assets[i] };

		if (has_asset != sol::nullopt) {
			break;
		}

		sol::table asset{ assets[i] };

		std::string asset_type{ asset["type"] };
		std::string asset_id{ asset["id"] };

		if (asset_type == "texture") {
			asset_manager->add_texture(renderer, asset_id, asset["file"]);

			Logger::log("New texture loaded to asset manager id: " + asset_id);
		}
		else if (asset_type == "font") {
			asset_manager->add_font(asset_id, asset["file"], asset["font_size"]);

			Logger::log("New font loaded to asset manager id: " + asset_id);
		}
		++i;
	}

	/*
	asset_manager->add_font("arial", "../assets/fonts/arial.ttf", 12);
	asset_manager->add_font("charriot", "../assets/fonts/charriot.ttf", 12);
	asset_manager->add_font("pico8_4", "../assets/fonts/pico8.ttf", 4);
	asset_manager->add_font("pico8_10", "../assets/fonts/pico8.ttf", 10);

	asset_manager->add_texture(renderer, "chopper", "../assets/images/chopper-spritesheet.png");
	asset_manager->add_texture(renderer, "radar", "../assets/images/radar.png");
	asset_manager->add_texture(renderer, "tank_panther_right", "../assets/images/tank-panther-right.png");
	asset_manager->add_texture(renderer, "truck_ford_right", "../assets/images/truck-ford-right.png");
	asset_manager->add_texture(renderer, "bullet", "../assets/images/bullet.png");
	asset_manager->add_texture(renderer, "tree", "../assets/images/tree.png");

	asset_manager->add_map(renderer, "jungle_map", "../assets/tilemaps/jungle.map", "../assets/tilemaps/jungle.png");
	asset_manager->load_map(registry, "jungle_map");

	// Radar
	Entity radar = registry->create_entity();
	radar.add_component<TransformComponent>(
		glm::dvec2(Game::window_width - 74, 10),
		glm::dvec2(1.0, 1.0),
		0.0
	);
	radar.add_component<SpriteComponent>("radar", 2, true, 64, 64);
	radar.add_component<AnimationComponent>(8, 0.15, true);

	// Chopper
	Entity chopper = registry->create_entity();
	chopper.add_tag("player");
	chopper.add_component<TransformComponent>(
		glm::dvec2(50.0, 50.0),
		glm::dvec2(1.0, 1.0),
		0.0
	);
	chopper.add_component<RigidbodyComponent>(glm::dvec2(0.0, 0.0));
	chopper.add_component<SpriteComponent>("chopper", 1);
	chopper.add_component<BoxColliderComponent>(32, 32);
	chopper.add_component<AnimationComponent>(2, 0.1, true);
	chopper.add_component<CameraComponent>();
	chopper.add_component<KeyboardControlComponent>(
		glm::dvec2(0.0, -80.0),
		glm::dvec2(80.0, 0.0),
		glm::dvec2(0.0, 80.0),
		glm::dvec2(-80.0, 0.0)
	);
	chopper.add_component<HealthComponent>(100);
	chopper.add_component<ProjectileEmitterComponent>(
		glm::dvec2{ 150.0, 150.0 },
		10,
		0.0,
		10.0,
		true
	);

	// Tank
	Entity tank{ registry->create_entity() };
	tank.add_group("enemies");
	tank.add_component<TransformComponent>(
		glm::dvec2(235.0, 235.0),
		glm::dvec2(1.0, 1.0),
		0.0
	);
	tank.add_component<RigidbodyComponent>(glm::dvec2(20.0, 0.0));
	tank.add_component<SpriteComponent>("tank_panther_right", 1);
	tank.add_component<BoxColliderComponent>(25, 20, glm::dvec2(5.0, 5.0));
	tank.add_component<ProjectileEmitterComponent>(
		glm::dvec2{ -50.0, 0 },
		10,
		2.0,
		5.0
	);
	tank.add_component<HealthComponent>(100);

	// Truck
	Entity truck{ registry->create_entity() };
	truck.add_group("enemies");
	truck.add_component<TransformComponent>(
		glm::dvec2(10.0, 110.0),
		glm::dvec2(1.0, 1.0),
		0.0
	);
	truck.add_component<RigidbodyComponent>(glm::dvec2(0.0, 0.0));
	truck.add_component<SpriteComponent>("truck_ford_right", 1);
	truck.add_component<BoxColliderComponent>(25, 20, glm::dvec2(5.0, 5.0));
	truck.add_component<ProjectileEmitterComponent>(
		glm::dvec2{ 50.0, 0 },
		10,
		2.0,
		5.0
	);
	truck.add_component<HealthComponent>(100);

	//Obstacle
	Entity tree_a{ registry->create_entity() };
	tree_a.add_group("obstacles");
	tree_a.add_component<TransformComponent>(
		glm::dvec2(335.0, 235.0),
		glm::dvec2(1.0, 1.0),
		0.0
	);
	tree_a.add_component<SpriteComponent>("tree", 1, false, 16, 32);
	tree_a.add_component<BoxColliderComponent>(16, 32);

	Entity tree_b{ registry->create_entity() };
	tree_b.add_group("obstacles");
	tree_b.add_component<TransformComponent>(
		glm::dvec2(135.0, 235.0),
		glm::dvec2(1.0, 1.0),
		0.0
	);
	tree_b.add_component<SpriteComponent>("tree", 1, false, 16, 32);
	tree_b.add_component<BoxColliderComponent>(16, 32);

	//Title
	Entity label{ registry->create_entity() };
	label.add_component<TextLabelComponent>(
		glm::dvec2{ Game::window_width / 2, 10 },
		"Chopper 1.0",
		"arial",
		SDL_Color{ 0, 255, 0, 255 },
		true
	);
	*/
}
