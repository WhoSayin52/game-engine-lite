#ifndef ECS_HPP
#define ECS_HPP

#include <bitset>
#include <vector>

/*
* Contains global vars for ECS configs
*/
namespace ecs_config {
	constexpr unsigned int max_components = 32;
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

class Registry {

};

template <typename TComponent>
void System::require_component() {
	const auto component_id = Component<TComponent>::get_id();
	component_signature.set(component_id);
}

#endif //ECS_HPP
