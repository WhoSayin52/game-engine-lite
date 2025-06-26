#ifndef EVENT_HPP
#define EVENT_HPP

class Event {
public:
	Event() = default;
	virtual ~Event() = 0;
};

inline Event::~Event() {}

#endif //EVENT
