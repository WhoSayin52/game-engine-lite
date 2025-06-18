#include "ecs.hpp"

#include "../logger/logger.hpp"

#include <algorithm>
#include <string>

int Entity::get_id() const {
	return id;
}

void System::add_entity(Entity entity) {
	entities.push_back(entity);
}

void System::remove_entity(Entity entity) {

	entities.erase(
		std::remove_if(
			entities.begin(),
			entities.end(),
			[&entity](const Entity& other) -> bool {
				return  other == entity;
			}
		),
		entities.end()
	);
}

const std::vector<Entity>& System::get_entities() const {
	return entities;
}	std::set<Entity> entities_to_add{};
std::set<Entity> entities_to_kill{};

const Signature& System::get_component_signature() const {
	return component_signature;
}

void Registry::update() {
	for (Entity entity : entities_to_add) {
		add_entity_to_systems(entity);
	}
	entities_to_add.clear();
}

void Registry::add_entity_to_systems(Entity entity) {
	const int entity_id{ entity.get_id() };
	const Signature& entity_component_signature{ entity_component_signatures[static_cast<std::size_t>(entity_id)] };

	for (auto& pair : systems) {
		const Signature& system_component_signature{ pair.second->get_component_signature() };

		bool is_interested{ (entity_component_signature & system_component_signature) == system_component_signature };
		if (is_interested) {
			pair.second->add_entity(entity);
		}
	}
}

Entity Registry::create_entity() {
	Entity entity{ entity_count++, this };

	entities_to_add.insert(entity);

	std::size_t id{ static_cast<std::size_t>(entity.get_id()) };

	if (id >= entity_component_signatures.size()) {
		entity_component_signatures.resize(id + 1);
	}

	Logger::log("Registry: Entity created, id: " + std::to_string(entity.get_id()));

	return entity;
}

