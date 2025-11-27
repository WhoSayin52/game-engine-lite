#include "game/game.hpp"

#include <iostream>

int main(int argc, char* argv[]) {

	int level{ 1 };

	if (argc == 2) {
		level = std::stoi(argv[1]);
		level = (level < 1 || level > 2) ? 1 : level;
	}

	Game game{};

	game.init();
	game.run(level);
	game.destroy();

	return 0;
}

