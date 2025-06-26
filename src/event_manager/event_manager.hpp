#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include "../events/event.hpp"
#include "../logger/logger.hpp"

#include <unordered_map>
#include <list>
#include <typeindex>
#include <memory>
#include <functional>

class IEventCallback {
public:
	virtual ~IEventCallback() = 0;

	void execute(Event& e) { call(e); }

private:
	virtual void call(Event& e) = 0;
};

template <typename TOwner, typename TEvent>
class EventCallback : public IEventCallback {

	using CallbackFunction = void (TOwner::*)(TEvent&);

public:
	EventCallback(
		TOwner* owner_instance,
		CallbackFunction callback_function
	) :
		owner_instance{ owner_instance },
		callback_function{ callback_function } {
	}

	~EventCallback() final override = default;

	EventCallback(const EventCallback&) = delete;
	EventCallback& operator=(const EventCallback&) = delete;

private:
	TOwner* owner_instance{};
	CallbackFunction callback_function{};

	void call(Event& e) final override { std::invoke(callback_function, owner_instance, static_cast<TEvent&>(e)); }
};

using EventHandlerList = std::list<std::unique_ptr<IEventCallback>>;

class EventManager {
public:
	EventManager();
	~EventManager();

	void reset() { listeners.clear(); }

	template <typename TOwner, typename TEvent>
	void listen(TOwner* owner_instance, void (TOwner::* callback_function)(TEvent&));

	template<typename TEvent, typename ...Args>
	void emit(Args&& ...args);

private:
	std::unordered_map<std::type_index, std::unique_ptr<EventHandlerList>> listeners{};
};

template <typename TOwner, typename TEvent>
void EventManager::listen(TOwner* owner_instance, void (TOwner::* callback_function)(TEvent&)) {

	if (!listeners[typeid(TEvent)]) {
		listeners[typeid(TEvent)] = std::make_unique<EventHandlerList>();
	}

	auto listener{ std::make_unique<EventCallback<TOwner, TEvent>>(owner_instance, callback_function) };
	listeners[typeid(TEvent)]->push_back(std::move(listener));
}

template<typename TEvent, typename ...Args>
void EventManager::emit(Args&& ...args) {
	EventHandlerList* handlers = listeners[typeid(TEvent)].get();

	if (handlers) {
		for (auto itr{ handlers->begin() }; itr != handlers->end(); ++itr) {
			IEventCallback* handler{ itr->get() };
			TEvent event{ std::forward<Args>(args)... };
			handler->execute(event);
		}
	}
}

#endif //EVENT_MANAGER_HPP
