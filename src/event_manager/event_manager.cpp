#include "event_manager.hpp"

IEventCallback::~IEventCallback() {}

EventManager::EventManager() {
	Logger::log("Event manager created!");
}

EventManager::~EventManager() {
	Logger::log("Event manager destroyed!");
}

