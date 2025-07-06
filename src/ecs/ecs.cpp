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

void Entity::add_tag(const std::string& s) {
	registry->add_tag(*this, s);
}

bool Entity::has_tag(const std::string& s) const {
	return registry->has_tag(*this, s);
}

void Entity::add_group(const std::string& s) {
	registry->add_group(*this, s);
}

bool Entity::belong_to_group(const std::string& s) const {
	return registry->belong_to_group(*this, s);
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

		for (auto p : component_pools) {
			if (p != nullptr) {
				p->remove_entity_from_pool(entity.get_id());
			}
		}

		remove_tag(entity);
		remove_group(entity);
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

void Registry::add_tag(const Entity& e, const std::string& s) {
	entity_tag.insert({ e.get_id(), s });
	tag_entity.insert({ s, e });
}

void Registry::remove_tag(const Entity& e) {

	int id{ e.get_id() };
	if (!entity_tag.contains(id)) {
		return;
	}

	std::string tag{ entity_tag[id] };
	entity_tag.erase(id);
	tag_entity.erase(tag);
}

bool Registry::has_tag(const Entity& e, const std::string& s) const {
	auto itr{ entity_tag.find(e.get_id()) };

	if (itr != entity_tag.end()) {
		return itr->second == s;
	}
	else {
		return false;
	}
}

Entity Registry::get_entity_by_tag(const std::string& s) const {
	return tag_entity.at(s);
}

void Registry::add_group(const Entity& e, const std::string& s) {
	group_entity[s].insert(e);
	entity_group.insert({ e.get_id(), s });
}

void Registry::remove_group(const Entity& e) {
	int id{ e.get_id() };
	if (!entity_group.contains(id)) {
		return;
	}

	std::string group{ entity_group[id] };
	entity_group.erase(id);
	group_entity[group].erase(e);
}

bool Registry::belong_to_group(const Entity& e, const std::string& s) const {
	auto itr{ entity_group.find(e.get_id()) };

	if (itr != entity_group.end()) {
		return itr->second == s;
	}
	else {
		return false;
	}
}

std::set<Entity> Registry::get_entities_by_group(const std::string& s) const {
	return group_entity.at(s);
}
