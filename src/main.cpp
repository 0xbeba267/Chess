#include "Game.h"

Lang lang;

int main(int argc, char* argv[]) {
	lang = EN;

	Game game;
	game.run();

	return 0;
}
