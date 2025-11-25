#include "level_loader.hpp"

#include "../components/animation_component.hpp"
#include "../components/box_collider_component.hpp"
#include "../components/camera_component.hpp"
#include "../components/health_component.hpp"
#include "../components/keyboard_control_component.hpp"
#include "../components/projectile_component.hpp"
#include "../components/projectile_emitter_component.hpp"
#include "../components/rigidbody_component.hpp"
#include "../components/script_component.hpp"
#include "../components/sprite_component.hpp"
#include "../components/text_label_component.hpp"
#include "../components/transform_component.hpp"

#include <sol/sol.hpp>

#include <fstream>

static std::string get_exe_dir();
static std::string exe_dir{ get_exe_dir() };

LevelLoader::LevelLoader() {

}

LevelLoader::~LevelLoader() {

}

void LevelLoader::load_level(sol::state& lua, SDL_Renderer* renderer, Registry* registry, AssetManager* asset_manager, int level_num) {

	(void)renderer; (void)registry; (void)asset_manager;

	sol::load_result script{ lua.load_file(exe_dir + "/../assets/scripts/level" + std::to_string(level_num) + ".lua") };

	//Check syntax of script without executing
	if (!script.valid()) {
		sol::error err{ script };
		std::string err_msg{ err.what() };
		Logger::err("Lua script error detected: " + err_msg);
		exit(EXIT_FAILURE);
	}

	//Passing lua exe dir
	lua.open_libraries(sol::lib::base);
	lua["exe_dir"] = exe_dir;

	//Executes lua script
	lua.script_file(exe_dir + "/../assets/scripts/level" + std::to_string(level_num) + ".lua");

	//Read level data table from lua script
	sol::table level{ lua["Level"] };

	//Read level assets
	sol::table assets{ level["assets"] };

	int i{ 0 };
	while (true) {
		sol::optional<sol::table> has_asset{ assets[i] };

		if (has_asset == sol::nullopt) {
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

	//Reading map
	sol::table map{ level["tilemap"] };
	std::string map_path{ map["map_file"] };
	std::string map_texture_asset_id{ map["texture_asset_id"] };

	int map_rows_count{ map["num_rows"] };
	int map_cols_count{ map["num_cols"] };
	int tile_size{ map["tile_size"] };

	double map_scale{ map["scale"] };

	std::fstream map_file{};
	map_file.open(map_path);

	for (int y{}; y < map_rows_count; ++y) {
		for (int x{}; x < map_cols_count; ++x) {
			char ch;
			map_file.get(ch);
			int src_rect_y{ std::atoi(&ch) * tile_size };
			map_file.get(ch);
			int src_rect_x{ std::atoi(&ch) * tile_size };
			map_file.ignore();

			Entity tile{ registry->create_entity() };
			tile.add_component<TransformComponent>(
				glm::dvec2(
					x * (map_scale * tile_size),
					y * (map_scale * tile_size)
				),
				glm::dvec2(
					map_scale,
					map_scale
				),
				0.0
			);
			tile.add_component<SpriteComponent>(
				map_texture_asset_id,
				0,
				false,
				tile_size,
				tile_size,
				src_rect_x,
				src_rect_y
			);
		}
	}

	map_file.close();
	Game::map_width = static_cast<int>(map_cols_count * tile_size * map_scale);
	Game::map_height = static_cast<int>(map_rows_count * tile_size * map_scale);

	//Reading entities
	sol::table entities{ level["entities"] };

	i = 0;
	while (true) {
		sol::optional<sol::table> has_entities{ entities[i] };
		if (has_entities == sol::nullopt) {
			break;
		}

		sol::table entity{ entities[i] };

		Entity e{ registry->create_entity() };

		//Tag
		sol::optional<std::string> tag{ entity["tag"] };
		if (tag != sol::nullopt) {
			e.add_tag(entity["tag"]);
		}

		//Group
		sol::optional<std::string> group{ entity["group"] };
		if (group != sol::nullopt) {
			e.add_group(entity["group"]);
		}

		// Components
		sol::optional<sol::table> has_component{ entity["components"] };
		if (has_component != sol::nullopt) {

			sol::optional<sol::table> transform{ entity["components"]["transform"] };
			if (transform != sol::nullopt) {
				e.add_component<TransformComponent>(
					glm::dvec2(
						entity["components"]["transform"]["position"]["x"],
						entity["components"]["transform"]["position"]["y"]
					),
					glm::dvec2(
						entity["components"]["transform"]["scale"]["x"],
						entity["components"]["transform"]["scale"]["y"]
					),
					entity["components"]["transform"]["rotation"].get_or(0.0)
				);
			}

			sol::optional<sol::table> rigidbody{ entity["components"]["rigidbody"] };
			if (rigidbody != sol::nullopt) {
				e.add_component<RigidbodyComponent>(
					glm::dvec2(
						entity["components"]["rigidbody"]["velocity"]["x"].get_or(0.0),
						entity["components"]["rigidbody"]["velocity"]["y"].get_or(0.0)
					)
				);
			}

			sol::optional<sol::table> sprite{ entity["components"]["sprite"] };
			if (sprite != sol::nullopt) {
				e.add_component<SpriteComponent>(
					entity["components"]["sprite"]["texture_asset_id"],
					entity["components"]["sprite"]["z_index"].get_or(1),
					entity["components"]["sprite"]["fixed"].get_or(false),
					entity["components"]["sprite"]["width"],
					entity["components"]["sprite"]["height"],
					entity["components"]["sprite"]["src_rect_x"].get_or(0),
					entity["components"]["sprite"]["src_rect_y"].get_or(0)
				);
			}

			sol::optional<sol::table> animation{ entity["components"]["animation"] };
			if (animation != sol::nullopt) {
				e.add_component<AnimationComponent>(
					entity["components"]["animation"]["num_frames"],
					entity["components"]["animation"]["frame_delay"]
				);
			}

			sol::optional<sol::table> collider{ entity["components"]["boxcollider"] };
			if (collider != sol::nullopt) {
				e.add_component<BoxColliderComponent>(
					entity["components"]["boxcollider"]["width"],
					entity["components"]["boxcollider"]["height"],
					glm::dvec2(
						entity["components"]["boxcollider"]["offset"]["x"].get_or(0.0),
						entity["components"]["boxcollider"]["offset"]["y"].get_or(0.0)
					)
				);
			}

			sol::optional<sol::table> health{ entity["components"]["health"] };
			if (health != sol::nullopt) {
				e.add_component<HealthComponent>(
					static_cast<int>(entity["components"]["health"]["health_percentage"].get_or(100))
				);
			}

			sol::optional<sol::table> projectile_emitter{ entity["components"]["projectile_emitter"] };
			if (projectile_emitter != sol::nullopt) {
				e.add_component<ProjectileEmitterComponent>(
					glm::dvec2(
						entity["components"]["projectile_emitter"]["projectile_velocity"]["x"],
						entity["components"]["projectile_emitter"]["projectile_velocity"]["y"]
					),
					static_cast<int>(entity["components"]["projectile_emitter"]["hit_percentage_damage"].get_or(10)),
					static_cast<double>(entity["components"]["projectile_emitter"]["emission_delay"].get_or(2.0)),
					static_cast<double>(entity["components"]["projectile_emitter"]["projectile_duration"].get_or(10)),
					entity["components"]["projectile_emitter"]["friendly"].get_or(false)
				);
			}

			sol::optional<sol::table> camera_follow{ entity["components"]["camera_follow"] };
			if (camera_follow != sol::nullopt) {
				e.add_component<CameraComponent>();
			}

			sol::optional<sol::table> keyboard_controlled{ entity["components"]["keyboard_controller"] };
			if (keyboard_controlled != sol::nullopt) {
				e.add_component<KeyboardControlComponent>(

					glm::dvec2(
						entity["components"]["keyboard_controller"]["up_velocity"]["x"],
						entity["components"]["keyboard_controller"]["up_velocity"]["y"]
					),
					glm::dvec2(
						entity["components"]["keyboard_controller"]["right_velocity"]["x"],
						entity["components"]["keyboard_controller"]["right_velocity"]["y"]
					),
					glm::dvec2(
						entity["components"]["keyboard_controller"]["down_velocity"]["x"],
						entity["components"]["keyboard_controller"]["down_velocity"]["y"]
					),
					glm::dvec2(
						entity["components"]["keyboard_controller"]["left_velocity"]["x"],
						entity["components"]["keyboard_controller"]["left_velocity"]["y"]
					)
				);
			}

			sol::optional<sol::table> script{ entity["components"]["on_update_script"] };
			if (script != sol::nullopt) {

				sol::function func = entity["components"]["on_update_script"][0];

				e.add_component<ScriptComponent>(
					func
				);
			}
		}
		++i;
	}
}

#include <libgen.h>
#include <unistd.h>
#include <linux/limits.h> 

static std::string get_exe_dir() {
	char result[PATH_MAX];
	ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
	if (count != -1) {
		result[count] = '\0';
		return std::string(dirname(result));
	}

	Logger::err("Failed to determine exe directory!");
	exit(EXIT_FAILURE);
}
