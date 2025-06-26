#ifndef KEY_PRESSED_EVENT_HPP
#define KEY_PRESSED_EVENT_HPP

#include "../events/event.hpp"

#include <SDL2/SDL.h>

class KeyPressedEvent : public Event {
public:
	SDL_Keycode key{};

	KeyPressedEvent(SDL_Keycode key) : key{ key } {}
};

#endif //KEY_PRESSED_EVENT_HPP
