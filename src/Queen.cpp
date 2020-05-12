#include "Queen.h"

Queen::Queen(ChessColor color, sf::Vector2i pos, Chessboard* xboard) :
		Figure(color, pos, xboard) {

	notationSymbol = 'H';
	type = QUEEN;
}

Figure* Queen::clone() const {
	return new Queen(*this);
}

bool Queen::isMoveLegal(sf::Vector2i newPos, SpecialMove& special) {
	int dx = newPos.x - pos.x;
	int dy = newPos.y - pos.y;

	/* QUEEN MOVE
	 * |dx| == |0| or |dy| == |0| or |dy| == |dx|,
	 * each field between starting and final has to be empty,
	 * destination field is not occupied by own figure */

	if (dx == 0 && dy == 0)
		return false;
	/* move in X axis
	 * |dx| == |0| */
	else if (dx == 0) {
		for (int y = pos.y + dy / abs(dy); y != newPos.y; y += dy / abs(dy)) {
			// check if least one field between is occupied
			if (xboard->field[newPos.x][y].getStatus() != U && y != newPos.y)
				return false;
		}
		// check if destination field is not occupied by own figure
		if (xboard->field[newPos.x][newPos.y].getStatus() == color)
			return false;
		return true;
	}
	/* move in Y axis
	 * |dy| == |0| */
	else if (dy == 0) {
		for (int x = pos.x + dx / abs(dx); x != newPos.x; x += dx / abs(dx)) {
			// check if least one field between is occupied
			if (xboard->field[x][newPos.y].getStatus() != U && x != newPos.x)
				return false;
		}
		// check if destination field is not occupied by own figure
		if (xboard->locateField(newPos)->getStatus() == color)
			return false;
		return true;
	}
	/* move in diagonal
	 * |dx| == |dy| */
	else if (abs(dx) == abs(dy)) {
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
	} else
		return false;
}

void Queen::setGraphics() {
	if (color == W)
		texture.loadFromFile(W_QUEEN_PNG);
	else
		texture.loadFromFile(B_QUEEN_PNG);
	setTexture(texture);
}

Queen::~Queen() {
}

