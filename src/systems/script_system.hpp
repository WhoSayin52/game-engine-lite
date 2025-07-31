#ifndef SCRIPT_SYSTEM_HPP
#define SCRIPT_SYSTEM_HPP

#include "../ecs/ecs.hpp"
#include "../components/script_component.hpp"
#include "../components/transform_component.hpp"
#include "../components/rigidbody_component.hpp"
#include "../components/projectile_emitter_component.hpp"
#include "../components/animation_component.hpp"

#include <tuple>

std::tuple<double, double> get_entity_position(Entity entity) {
	if (entity.has_component<TransformComponent>()) {
		auto& transform{ entity.get_component<TransformComponent>() };
		return std::make_tuple(transform.position.x, transform.position.y);
	}
	else {
		Logger::err("Trying to get position to an entity that does not have a transform component id: " + entity.get_id());
		return std::make_tuple(0.0, 0.0);
	}
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

std::tuple<double, double> get_entity_velocity(Entity entity) {
	if (entity.has_component<RigidbodyComponent>()) {
		auto& rigidbody{ entity.get_component<RigidbodyComponent>() };
		return std::make_tuple(rigidbody.velocity.x, rigidbody.velocity.y);
	}
	else {
		Logger::err("Trying to get velocity to an entity that does not have a rigidbody component id: " + entity.get_id());
		return std::make_tuple(0.0, 0.0);
	}
}

void set_entity_velocity(Entity entity, double x, double y) {
	if (entity.has_component<RigidbodyComponent>()) {
		auto& rigidbody{ entity.get_component<RigidbodyComponent>() };
		rigidbody.velocity.x = x;
		rigidbody.velocity.y = y;
	}
	else {
		Logger::err("Trying to set velocity to an entity that does not have a rigidbody component id: " + entity.get_id());
	}
}

void set_entity_rotation(Entity entity, double angle) {
	if (entity.has_component<TransformComponent>()) {
		auto& transform{ entity.get_component<TransformComponent>() };
		transform.rotation = angle;
	}
	else {
		Logger::err("Trying to set rotation to an entity that does not have a transform component id: " + entity.get_id());
	}
}

void set_projectile_velocity(Entity entity, double x, double y) {
	if (entity.has_component<ProjectileEmitterComponent>()) {
		auto& emitter{ entity.get_component<ProjectileEmitterComponent>() };
		emitter.velocity.x = x;
		emitter.velocity.y = y;
	}
	else {
		Logger::err("Trying to set projectile emitter velocity to an entity that does not have a projectile emitter component id: " + entity.get_id());
	}
}

void set_animation_frame(Entity entity, int frame) {
	if (entity.has_component<AnimationComponent>()) {
		auto& animation{ entity.get_component<AnimationComponent>() };
		animation.current_frame = frame;
	}
	else {
		Logger::err("Trying to set frame to an entity that does not have a animation component id: " + entity.get_id());
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
		lua.set_function("set_velocity", set_entity_velocity);
		lua.set_function("get_position", get_entity_position);
		lua.set_function("get_velocity", get_entity_velocity);
		lua.set_function("set_rotation", set_entity_rotation);
		lua.set_function("set_projectile_velocity", set_projectile_velocity);
		lua.set_function("set_animation_frame", set_animation_frame);
	}

	void update(double delta_time, Uint32 ellapsed_time) {

		for (auto e : get_entities()) {
			const auto script{ e.get_component<ScriptComponent>() };
			script.func(e, delta_time, ellapsed_time);
		}
	}
};

#endif //SCRIPT_SYSTEM_HPP
