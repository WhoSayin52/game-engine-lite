#include "game/game.hpp"

#include <sol/sol.hpp>

#include <iostream>

void test_lua(){
	sol::state lua;
	
	lua.open_libraries(sol::lib::base);

	lua.script_file("../assets/scripts/my_script.lua");

	int some_var_from_lua{lua["some_var"]};

	std::cout << "The some var from lua " << some_var_from_lua << '\n';
}

int main() {
	//Game game{};

	//game.init();
	//game.run();
	//game.destroy();

	test_lua();

	return 0;
}
