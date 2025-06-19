#ifndef ECS_HPP
#define ECS_HPP

#include "../logger/logger.hpp"

#include <bitset>
#include <vector>
#include <unordered_map>
#include <set>
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

private:
};

/*
* The system processes entities that contain the specified signature.
*/
class System {
public:
	System() = default;
	~System() = default;

	void add_entity(Entity entity);
	void remove_entity(Entity entity);
	const std::vector<Entity>& get_entities() const;
	const Signature& get_component_signature() const;

	template <typename TComponent>
	void require_component();

private:
	Signature component_signature{};
	std::vector<Entity> entities{};
};

class IPool {
public:
	virtual ~IPool() {}
};

template <typename T>
class Pool : public IPool {
public:
	Pool(std::size_t size = 100) : data(size) {}
	~Pool() final override = default;

	bool empty() const { return data.empty(); }
	std::size_t size() const { return data.size(); }
	void resize(std::size_t size) { data.resize(size); }
	void clear() { data.clear(); }
	void add(T object) { data.push_back(object); }
	void set(std::size_t index, T object) { data[index] = object; }
	T& get(std::size_t index) { return static_cast<T&>(data[index]); }

	T& operator[](std::size_t index) { return data[index]; }

private:
	std::vector<T> data{};
};

class Registry {
public:
	Registry() = default;

	void update();
	void add_entity_to_systems(Entity entity);

	// Entity managment
	Entity create_entity();

	// Component managment
	template <typename TComponent, typename ...Args>
	void add_component(Entity entity, Args&& ...args);

	template <typename TComponent>
	void remove_component(Entity entity);

	template <typename TComponent>
	bool has_component(Entity entity) const;

	template <typename TComponent>
	TComponent& get_component(Entity entity) const;

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
	std::vector<std::shared_ptr<IPool>> component_pools{};
	std::vector<Signature> entity_component_signatures{};
	std::unordered_map <std::type_index, std::shared_ptr<System>> systems{};
	std::set<Entity> entities_to_add{};
	std::set<Entity> entities_to_kill{};
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

template <typename TComponent, typename ...Args>
void Registry::add_component(Entity entity, Args&& ...args) {
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

	if (entity_index >= pool->size()) {
		pool->resize(static_cast<std::size_t>(entity_count));
	}

	TComponent new_component{ std::forward<Args>(args)... };

	pool->set(entity_index, new_component);
	entity_component_signatures[entity_index].set(component_index);

	Logger::log("Component id " + std::to_string(component_id) + " was added to entity id " + std::to_string(entity_id));
}

template <typename TComponent>
void Registry::remove_component(Entity entity) {
	const int component_id{ Component<TComponent>::get_id() };
	const int entity_id{ entity.get_id() };

	const std::size_t component_index{ static_cast<std::size_t>(component_id) };
	const std::size_t entity_index{ static_cast<std::size_t>(entity_id) };

	entity_component_signatures[entity_index].set(component_index, false);

	Logger::log("Component id " + std::to_string(component_id) + " was removed from entity id " + std::to_string(entity_id));
}

template <typename TComponent>
bool Registry::has_component(Entity entity) const {
	const int component_id{ TComponent::get_id() };
	const int entity_id{ entity.get_id() };

	const std::size_t component_index{ static_cast<std::size_t>(component_id) };
	const std::size_t entity_index{ static_cast<std::size_t>(entity_id) };

	return entity_component_signatures[entity_index].test(component_index);
}

template <typename TComponent>
TComponent& Registry::get_component(Entity entity) const {
	const int component_id{ Component<TComponent>::get_id() };
	const std::size_t component_index{ static_cast<std::size_t>(component_id) };

	std::shared_ptr<Pool<TComponent>> component_pool{
		std::static_pointer_cast<Pool<TComponent>>(component_pools[component_index])
	};

	const std::size_t entity_index{ static_cast<std::size_t>(entity.get_id()) };

	return component_pool->get(entity_index);
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
