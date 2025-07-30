#ifndef SCRIPT_SYSTEM_HPP
#define SCRIPT_SYSTEM_HPP

#include "../ecs/ecs.hpp"
#include "../components/script_component.hpp"
#include "../components/transform_component.hpp"

double get_entity_position(Entity entity) {
	(void)entity;
	return 0.0;
}

void set_entity_position(Entity entity, double x, double y) {
	if (entity.has_component<TransformComponent>()) {
		auto& transform{ entity.get_component<TransformComponent>() };
		transform.position.x = x;
		transform.position.y = y;
	}
	else {
		Logger::err("Trying to set position to an entity that does not have a transform component id: " + entity.get_id());
	}
}

class ScriptSystem : public System {
public:
	ScriptSystem() {
		require_component<ScriptComponent>();
	}

	void create_lua_bindings(sol::state& lua) {

		lua.new_usertype<Entity>(
			"entity",
			"get_id", &Entity::get_id,
			"destroy", &Entity::free,
			"has_tag", &Entity::has_tag,
			"belongs_to_group", &Entity::belong_to_group
		);

		lua.set_function("set_position", set_entity_position);
	}

	void update(double delta_time, Uint32 ellapsed_time) {

		for (auto e : get_entities()) {
			const auto script{ e.get_component<ScriptComponent>() };
			script.func(e, delta_time, ellapsed_time);
		}
	}
};

#endif //SCRIPT_SYSTEM_HPP
