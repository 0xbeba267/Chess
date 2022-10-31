#include "Bishop.h"

Bishop::Bishop(ChessColor color, sf::Vector2i pos, Chessboard* xboard) :
		Figure(color, pos, xboard) {

	if (lang == EN)
		notationSymbol = 'B';
	else if (lang == PL)
		notationSymbol = 'G';

	type = BISHOP;
}

Figure* Bishop::clone() const {
	return new Bishop(*this);
}

bool Bishop::isMoveLegal(sf::Vector2i newPos, SpecialMove& special) {
	int dx = newPos.x - pos.x;
	int dy = newPos.y - pos.y;

	if (dx == 0 && dy == 0)
		return false;
	/* BISHOP MOVE
	 * |dy| == |dx|,
	 * each field between starting and final has to be empty,
	 * destination field is not occupied by own figure */
	else if (abs(dx) != abs(dy))
		return false;
	else {
		for (int x = pos.x + dx / abs(dx); x != newPos.x; x += dx / abs(dx)) {
			// check if least one field between is occupied
			if (xboard->field[x][pos.y + dy / abs(dy) * abs(x - pos.x)].getStatus()
					!= U && x != newPos.x)
				return false;
		}
		// check if destination field is not occupied by own figure
		if (xboard->locateField(newPos)->getStatus() == color)
			return false;
		return true;
	}
}

void Bishop::setGraphics() {
	if (color == W)
		texture.loadFromFile(W_BISHOP_PNG);
	else
		texture.loadFromFile(B_BISHOP_PNG);
	setTexture(texture);
}

Bishop::~Bishop() {
}

