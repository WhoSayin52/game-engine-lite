#ifndef ECS_HPP
#define ECS_HPP

#include <bitset>
#include <vector>
#include <unordered_map>
#include <set>
#include <utility>
#include <cstdint>
#include <typeindex>

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

class Entity {
public:
	Entity(int id) : id{ id } {};
	Entity& operator=(const Entity& other) = default;
	int get_id() const;

	bool operator==(const Entity& other) const { return id == other.get_id(); }
	bool operator!=(const Entity& other) const { return id != other.get_id(); }
	bool operator>(const Entity& other) const { return id > other.get_id(); }
	bool operator<(const Entity& other) const { return id < other.get_id(); }

private:
	int id{};
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
	virtual ~IPool() {}
};

template <typename T>
class Pool : public IPool {
public:
	Pool(std::size_t = 100) : data(size) {}
	~Pool() final override = default;

	bool empty() const { return data.empty(); }
	std::size_t size() const { return data.size(); }
	void resize(std::size_t size) { data.resize(size); }
	void clear() { data.clear(); }
	void add(T object) { data.push_back(object); }
	void set(std::size_t index, T object) { data[index] = object; }
	T& get(int index) { return static_cast<T&>(data[index]); }

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

	// System managment
	template <typename TSystem, typename ...Args>
	void add_system(TSystem system, Args&& ...args);

	template <typename TSystem>
	void remove_system();

	template <typename TSystem>
	bool has_system() const;

	template <typename TSystem>
	TSystem& get_system() const;

private:
	int entity_count{};
	std::vector<IPool*> component_pools{};
	std::vector<Signature> entity_component_signatures{};
	std::unordered_map <std::type_index, System*> systems{};
	std::set<Entity> entities_to_add{};
	std::set<Entity> entities_to_kill{};
};

template <typename TComponent>
void System::require_component() {
	const int component_id = Component<TComponent>::get_id();
	component_signature.set(component_id);
}

template <typename TComponent, typename ...Args>
void Registry::add_component(Entity entity, Args&& ...args) {
	const int component_id{ Component<TComponent>::get_id() };
	const int entity_id{ entity.get_id() };

	if (component_id >= component_pools.size()) {
		component_pools.resize(component_id + 1, nullptr);
	}

	if (component_pools[component_id] == nullptr) {
		component_pools[component_id] = new Pool<TComponent>;
	}

	Pool<TComponent>* pool = component_pools[component_id];

	if (entity_id >= pool->size()) {
		pool->resize(entity_count);
	}

	TComponent new_component{ std::forward<Args>(args)... };

	pool->set(entity_id, new_component);
	entity_component_signatures[entity_id].set(component_id);
}

template <typename TComponent>
void Registry::remove_component(Entity entity) {
	const int component_id{ TComponent::get_id() };
	const int entity_id{ entity.get_id() };

	entity_component_signatures[entity_id].set(component_id, false);
}

template <typename TComponent>
bool Registry::has_component(Entity entity) const {
	const int component_id{ TComponent::get_id() };
	const int entity_id{ entity.get_id() };

	return entity_component_signatures[entity_id].test(component_id);
}

template <typename TSystem, typename ...Args>
void Registry::add_system(TSystem system, Args&& ...args) {
	TSystem* new_system{ new TSystem(std::forward<Args>(args)...) };

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
	return *static_cast<TSystem*>(system->second);
}

#endif //ECS_HPP
