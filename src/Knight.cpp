#include "Knight.h"

Knight::Knight(ChessColor color, sf::Vector2i pos, Chessboard* xboard) :
		Figure(color, pos, xboard) {

	if (lang == EN)
		notationSymbol = 'N';
	else if (lang == PL)
		notationSymbol = 'S';

	type = KNIGHT;
}

Figure* Knight::clone() const {
	return new Knight(*this);
}

bool Knight::isMoveLegal(sf::Vector2i newPos, SpecialMove& special) {
	int dx = newPos.x - pos.x;
	int dy = newPos.y - pos.y;

	if (dx == 0 && dy == 0)
		return false;
	/* KNIGHT MOVE
	 * |dx| = 2 and |dy| = 1 OR |dx| = 1 and |dy| = 2,
	 * destination field is not occupied by own figure */
	else if ((abs(dx) == 2 && abs(dy) == 1) || (abs(dx) == 1 && abs(dy) == 2)) {
		if (xboard->locateField(newPos)->getStatus() == color)
			return false;
		return true;
	} else
		return false;
}

void Knight::setGraphics() {
	if (color == W)
		texture.loadFromFile(W_KNIGHT_PNG);
	else
		texture.loadFromFile(B_KNIGHT_PNG);
	setTexture(texture);
}

Knight::~Knight() {
}

