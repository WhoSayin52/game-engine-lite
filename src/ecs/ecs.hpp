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
	int get_id() const;

private:
	int id{};
};

class Component {

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
	std::vector<Entity>& get_entities() const;
	Signature& get_component_signature() const;

private:
	Signature component_signature{};
	std::vector<Entity> entities{};
};

class Registry {

};

#endif //ECS_HPP
