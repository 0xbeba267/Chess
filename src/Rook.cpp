#include "Rook.h"

Rook::Rook(ChessColor color, sf::Vector2i pos, Chessboard* xboard) :
		Figure(color, pos, xboard) {

	if (lang == EN)
		notationSymbol = 'R';
	else if (lang == PL)
		notationSymbol = 'W';

	type = ROOK;
}

Figure* Rook::clone() const {
	return new Rook(*this);
}

bool Rook::isMoveLegal(sf::Vector2i newPos, SpecialMove& special) {
	int dx = newPos.x - pos.x;
	int dy = newPos.y - pos.y;

	/* ROOK MOVE
	 * |dy| == 0 || |dx| == 0,
	 * each field between starting and final has to be empty,
	 * destination field is not occupied by own figure */

	if (dx == 0 && dy == 0)
		return false;
	/* move in X axis
	 * |dx| == |0| */
	else if (dx == 0) {
		for (int y = pos.y + dy / abs(dy); y != newPos.y;
				y += dy / abs(dy)) {
			// check if least one field between is occupied
			if (xboard->field[newPos.x][y].getStatus() != U)
				return false;
		}
		// check if destination field is not occupied by own figure
		if (xboard->locateField(newPos)->getStatus() == color)
			return false;
		return true;
	}
	/* move in Y axis
	 * |dy| == |0| */
	else if (dy == 0) {
		for (int x = pos.x + dx / abs(dx); x != newPos.x;
				x += dx / abs(dx)) {
			// check if least one field between is occupied
			if (xboard->field[x][newPos.y].getStatus() != U)
				return false;
		}
		// check if destination field is not occupied by own figure
		if (xboard->locateField(newPos)->getStatus() == color)
			return false;
		return true;
	} else
		return false;
}

void Rook::setGraphics() {
	if (color == W)
		texture.loadFromFile(W_ROOK_PNG);
	else
		texture.loadFromFile(B_ROOK_PNG);
	setTexture(texture);
}

Rook::~Rook() {
}

