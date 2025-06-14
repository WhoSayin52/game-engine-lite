#include "ecs.hpp"

#include <algorithm>

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
}

const Signature& System::get_component_signature() const {
	return component_signature;
}
