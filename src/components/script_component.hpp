#ifndef SCRIPT_COMPONENT_HPP
#define SCRIPT_COMPONENT_HPP

#include <sol/sol.hpp>

struct ScriptComponent {

	sol::function func;

	ScriptComponent(sol::function func = sol::lua_nil) : func{ func } {
	}
};

#endif //SCRIPT_COMPONENT_HPP
