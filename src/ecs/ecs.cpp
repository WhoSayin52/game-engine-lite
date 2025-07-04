#include "ecs.hpp"

#include "../logger/logger.hpp"

#include <algorithm>
#include <string>

int Entity::get_id() const {
	return id;
}

void Entity::free() {
	registry->free_entity(*this);
}

void Entity::add_tag(std::string s) {
	registry->add_tag(*this, s);
}

void Entity::add_group(std::string s) {
	registry->add_group(*this, s);
}

void System::add_entity(const Entity& entity) {
	entities.push_back(entity);
}

void System::remove_entity(const Entity& entity) {

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

std::vector<Entity>& System::get_entities() {
	return entities;
}

const Signature& System::get_component_signature() const {
	return component_signature;
}

IPool::~IPool() {}

void Registry::update() {
	for (const Entity& entity : entities_to_add) {
		add_entity_to_systems(entity);
	}
	entities_to_add.clear();

	for (const Entity& entity : entities_to_free) {
		remove_entity_from_systems(entity);
		entity_component_signatures[static_cast<std::size_t>(entity.get_id())].reset();
		free_ids.push_back(entity.get_id());
		Logger::log("Registry: Entity destroyed, id: " + std::to_string(entity.get_id()));
	}
	entities_to_free.clear();
}

void Registry::add_entity_to_systems(const Entity& entity) {
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

void Registry::remove_entity_from_systems(const Entity& entity) {
	for (auto& pair : systems) {
		System& system{ *pair.second };
		system.remove_entity(entity);
	}
}

Entity Registry::create_entity() {
	int new_id{};

	if (free_ids.empty()) {
		new_id = entity_count++;
	}
	else {
		new_id = free_ids.front();
		free_ids.pop_front();
	}

	Entity entity{ new_id, this };

	entities_to_add.insert(entity);

	std::size_t id{ static_cast<std::size_t>(entity.get_id()) };

	if (id >= entity_component_signatures.size()) {
		entity_component_signatures.resize(id + 1);
	}

	Logger::log("Registry: Entity created, id: " + std::to_string(entity.get_id()));

	return entity;
}

void Registry::free_entity(const Entity& entity) {
	entities_to_free.insert(entity);
}

void Registry::add_tag(const Entity& e, std::string s) {
	entity_tag.insert({ e.get_id(), s });
	tag_entity.insert({ s, e.get_id() });
}

void Registry::add_group(const Entity& e, std::string s) {
	group_entity[s].insert(e.get_id());
	entity_group.insert({ e.get_id(), s });
}
