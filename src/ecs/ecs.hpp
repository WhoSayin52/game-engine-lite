#ifndef ECS_HPP
#define ECS_HPP

#include "../logger/logger.hpp"

#include <bitset>
#include <vector>
#include <unordered_map>
#include <set>
#include <deque>
#include <utility>
#include <cstdint>
#include <typeindex>
#include <memory>

/*
* Contains global vars for ECS configs
*/
namespace ecs_config {
	constexpr std::uint32_t max_components = 32;
}

/*
* Used to identify which components a entity has,
* and which componets a system is interested in.
*/
using Signature = std::bitset<ecs_config::max_components>;

class Registry;

class Entity {
public:

	Entity(int id, Registry* registry) : id{ id }, registry{ registry } {};
	Entity& operator=(const Entity& other) = default;
	int get_id() const;
	void free();

	void add_tag(const std::string& s);
	bool has_tag(const std::string& s) const;
	void add_group(const std::string& s);
	bool belong_to_group(const std::string& s) const;

	template <typename TComponent, typename ...Args>
	void add_component(Args&& ...args);

	template <typename TComponent>
	void remove_component();

	template <typename TComponent>
	bool has_component() const;

	template <typename TComponent>
	TComponent& get_component() const;

	bool operator==(const Entity& other) const { return id == other.get_id(); }
	bool operator!=(const Entity& other) const { return id != other.get_id(); }
	bool operator>(const Entity& other) const { return id > other.get_id(); }
	bool operator<(const Entity& other) const { return id < other.get_id(); }

private:
	int id{};

public:
	Registry* registry{ nullptr };
};

struct IComponent {
protected:
	inline static int next_id{};
};

template <typename T>
class Component : public IComponent {
public:
	static int get_id() {
		static const int id = next_id++;
		return id;
	}
};

/*
* The system processes entities that contain the specified signature.
*/
class System {
public:
	System() = default;
	~System() = default;

	void add_entity(const Entity& entity);
	void remove_entity(const Entity& entity);
	std::vector<Entity>& get_entities();
	const Signature& get_component_signature() const;

	template <typename TComponent>
	void require_component();

private:
	Signature component_signature{};
	std::vector<Entity> entities{};
};

class IPool {
public:
	virtual ~IPool() = 0;
	virtual void remove_entity_from_pool(int entity_id) = 0;
};

template <typename TComponent>
class Pool : public IPool {
public:
	Pool(std::size_t size = 100) {
		data.reserve(size);
	}
	~Pool() final override = default;

	bool empty() const { return data.empty(); }
	std::size_t size() const { return data.size(); }
	void resize(std::size_t size) { data.resize(size); }
	void clear() { data.clear(); }
	void add(TComponent object) { data.push_back(object); }
	void set(int entity_id, TComponent object);
	TComponent& get(int entity_id) { return static_cast<TComponent&>(data[entity_index[entity_id]]); }
	void remove(int entity_id);
	void remove_entity_from_pool(int entity_id) override;

	TComponent& operator[](std::size_t index) { return data[index]; }

private:
	std::vector<TComponent> data{};

	std::unordered_map<int, std::size_t> entity_index{};
	std::unordered_map<std::size_t, int> index_entity{};
};

class Registry {
public:
	Registry() = default;

	void update();
	void add_entity_to_systems(const Entity& entity);
	void remove_entity_from_systems(const Entity& entity);

	// Entity managment
	Entity create_entity();
	void free_entity(const Entity& entity);

	void add_tag(const Entity& e, const std::string& s);
	void remove_tag(const Entity& e);
	bool has_tag(const Entity& e, const std::string& s) const;
	Entity get_entity_by_tag(const std::string& s) const;

	void add_group(const Entity& e, const std::string& s);
	void remove_group(const Entity& e);
	bool belong_to_group(const Entity& e, const std::string& s) const;
	std::set<Entity> get_entities_by_group(const std::string& s) const;

	// Component managment
	template <typename TComponent, typename ...Args>
	void add_component(const Entity& entity, Args&& ...args);

	template <typename TComponent>
	void remove_component(const Entity& entity);

	template <typename TComponent>
	bool has_component(const Entity& entity) const;

	template <typename TComponent>
	TComponent& get_component(const Entity& entity) const;

	// System managment
	template <typename TSystem, typename ...Args>
	void add_system(Args&& ...args);

	template <typename TSystem>
	void remove_system();

	template <typename TSystem>
	bool has_system() const;

	template <typename TSystem>
	TSystem& get_system() const;

private:
	int entity_count{};
	std::deque<int> free_ids{};
	std::vector<std::shared_ptr<IPool>> component_pools{};
	std::vector<Signature> entity_component_signatures{};
	std::unordered_map<std::type_index, std::shared_ptr<System>> systems{};
	std::set<Entity> entities_to_add{};
	std::set<Entity> entities_to_free{};
	std::unordered_map<int, std::string> entity_tag{};
	std::unordered_map<std::string, Entity> tag_entity{};
	std::unordered_map<std::string, std::set<Entity>> group_entity{};
	std::unordered_map<int, std::string> entity_group{};
};

template <typename TComponent, typename ...Args>
void Entity::add_component(Args&& ...args) {
	registry->add_component<TComponent>(*this, std::forward<Args>(args)...);
}

template <typename TComponent>
void Entity::remove_component() {
	registry->remove_component<TComponent>(*this);
}

template <typename TComponent>
bool Entity::has_component() const {
	return registry->has_component<TComponent>(*this);
}

template <typename TComponent>
TComponent& Entity::get_component() const {
	return registry->get_component<TComponent>(*this);
}

template <typename TComponent>
void System::require_component() {
	const int component_id = Component<TComponent>::get_id();
	const std::size_t component_index{ static_cast<std::size_t>(component_id) };
	component_signature.set(component_index);
}

template <typename TComponent>
void Pool<TComponent>::set(int entity_id, TComponent object) {

	if (entity_index.find(entity_id) != entity_index.end()) {
		std::size_t index{ static_cast<std::size_t>(entity_id) };
		data[index] = object;
	}
	else {
		std::size_t index{ data.size() };
		entity_index.try_emplace(entity_id, index);
		index_entity.try_emplace(index, entity_id);
		data.push_back(object);
	}
}

template <typename TComponent>
void Pool<TComponent>::remove(int entity_id) {
	std::size_t index_to_remove{ entity_index[entity_id] };
	std::size_t index_of_last{ data.size() - 1 };
	data[index_to_remove] = data[index_of_last];

	int id_of_last{ index_entity[index_of_last] };
	entity_index[id_of_last] = index_to_remove;
	index_entity[index_to_remove] = id_of_last;

	entity_index.erase(entity_id);
	index_entity.erase(index_of_last);
	data.erase(data.begin() + static_cast<int>(index_of_last));
}

template <typename TComponent>
void Pool<TComponent>::remove_entity_from_pool(int entity_id) {
	if (entity_index.find(entity_id) != entity_index.end()) {
		remove(entity_id);
	}
}

template <typename TComponent, typename ...Args>
void Registry::add_component(const Entity& entity, Args&& ...args) {
	const int component_id{ Component<TComponent>::get_id() };
	const int entity_id{ entity.get_id() };

	const std::size_t component_index{ static_cast<std::size_t>(component_id) };
	const std::size_t entity_index{ static_cast<std::size_t>(entity_id) };

	if (component_index >= component_pools.size()) {
		component_pools.resize(component_index + 1, nullptr);
	}

	if (component_pools[component_index] == nullptr) {
		component_pools[component_index] = std::make_shared<Pool<TComponent>>();
	}

	std::shared_ptr<Pool<TComponent>> pool = std::static_pointer_cast<Pool<TComponent>>(component_pools[component_index]);

	TComponent new_component{ std::forward<Args>(args)... };

	pool->set(entity_id, new_component);
	entity_component_signatures[entity_index].set(component_index);

	Logger::log("Component id " + std::to_string(component_id) + " was added to entity id " + std::to_string(entity_id));
}

template <typename TComponent>
void Registry::remove_component(const Entity& entity) {
	const int component_id{ Component<TComponent>::get_id() };
	const int entity_id{ entity.get_id() };

	const std::size_t component_index{ static_cast<std::size_t>(component_id) };
	const std::size_t entity_index{ static_cast<std::size_t>(entity_id) };

	std::shared_ptr<Pool<TComponent>> pool = std::static_pointer_cast<Pool<TComponent>>(component_pools[component_index]);

	pool->remove(entity_id);

	entity_component_signatures[entity_index].set(component_index, false);

	Logger::log("Component id " + std::to_string(component_id) + " was removed from entity id " + std::to_string(entity_id));
}

template <typename TComponent>
bool Registry::has_component(const Entity& entity) const {
	const int component_id{ Component<TComponent>::get_id() };
	const int entity_id{ entity.get_id() };

	const std::size_t component_index{ static_cast<std::size_t>(component_id) };
	const std::size_t entity_index{ static_cast<std::size_t>(entity_id) };

	return entity_component_signatures[entity_index].test(component_index);
}

template <typename TComponent>
TComponent& Registry::get_component(const Entity& entity) const {
	const int component_id{ Component<TComponent>::get_id() };
	const std::size_t component_index{ static_cast<std::size_t>(component_id) };

	std::shared_ptr<Pool<TComponent>> component_pool{
		std::static_pointer_cast<Pool<TComponent>>(component_pools[component_index])
	};

	return component_pool->get(entity.get_id());
}

template <typename TSystem, typename ...Args>
void Registry::add_system(Args&& ...args) {
	std::shared_ptr<TSystem> new_system{ std::make_shared<TSystem>(std::forward<Args>(args)...) };

	systems.insert(std::make_pair(std::type_index(typeid(TSystem)), new_system));
}

template <typename TSystem>
void Registry::remove_system() {
	auto system = systems.find(std::type_index(typeid(TSystem)));
	systems.erase(system);
}

template <typename TSystem>
bool Registry::has_system() const {
	return  systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template <typename TSystem>
TSystem& Registry::get_system() const {
	auto system{ systems.find(std::type_index(typeid(TSystem))) };
	return *std::static_pointer_cast<TSystem>(system->second);
}

#endif //ECS_HPP
