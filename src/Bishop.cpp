#include "Bishop.h"

Bishop::Bishop(ChessColor color, sf::Vector2i pos, Chessboard* xboard) :
		Figure(color, pos, xboard) {
	if (color == W)
		texture.loadFromFile(W_BISHOP_PNG);
	else if (color == B)
		texture.loadFromFile(B_BISHOP_PNG);

	// set texture of sprite
	setTexture(texture);

	notationSymbol = 'G';
	type = BISHOP;
}

Figure* Bishop::clone() const {
	return new Bishop(*this);
}

bool Bishop::isMoveLegal(sf::Vector2i newPos, SpecialMove& special) {
	int dx = pos.x - oldPos.x;
	int dy = pos.y - oldPos.y;

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

Bishop::~Bishop() {
}

