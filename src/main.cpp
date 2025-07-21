#include "game/game.hpp"

#include <sol/sol.hpp>

#include <iostream>

void test_lua() {
	sol::state lua;

	lua.open_libraries(sol::lib::base);

	lua.script_file("../assets/scripts/my_script.lua");

	//This is how we get vars from lua scripts
	int some_var_from_lua{ lua["some_var"] };
	std::cout << "The some var from lua " << some_var_from_lua << '\n';

	//This is how we get values from lua table
	bool is_full_screen = lua["config"]["fullscreen"];

	std::cout << "We read full screen value is: " << is_full_screen << '\n';

	//Or do this
	sol::table config = lua["config"];
	int width = config["resolution"]["width"];
	int height = config["resolution"]["height"];

	std::cout << "H: " << height << " | W: " << width << '\n';
}

int main() {
	//Game game{};

	//game.init();
	//game.run();
	//game.destroy();

	test_lua();

	return 0;
}
