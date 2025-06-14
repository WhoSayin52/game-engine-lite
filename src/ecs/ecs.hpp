#ifndef ECS_HPP
#define ECS_HPP

#include <bitset>
#include <vector>
#include <cstdint>

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
	static int next_id;
};

template <typename T>
class Component : public IComponent {
public:
	static int get_id() {
		static int id = next_id++;
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

private:
	int entity_count{};
	std::vector<IPool*> component_pools{};
};

template <typename TComponent>
void System::require_component() {
	const int component_id = Component<TComponent>::get_id();
	component_signature.set(component_id);
}

#endif //ECS_HPP
