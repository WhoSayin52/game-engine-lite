#include "game/game.hpp"

int main() {
	Game game;

	game.init();
	game.run();
	game.destroy();

	return 0;
}
