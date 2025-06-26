#ifndef COLLISION_EVENT_HPP
#define COLLISION_EVENT_HPP

#include "event.hpp"
#include "../ecs/ecs.hpp"

class CollisionEvent : public Event {
public:
	Entity& a;
	Entity& b;

	CollisionEvent(Entity& a, Entity& b) : a{ a }, b{ b } {}
	~CollisionEvent() final override = default;
};

#endif //COLLISION_EVENT_HPP
