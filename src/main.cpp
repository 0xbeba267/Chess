#include "Game.h"

using namespace sf;

// create window for program
RenderWindow window(VideoMode(910, 670), "Chess",
		Style::Titlebar | Style::Close);

#include <iostream>

int main(int argc, char* argv[]) {
	Game game;
	game.run();
}
