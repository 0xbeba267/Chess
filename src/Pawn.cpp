#include "Pawn.h"

Pawn::Pawn(ChessColor color, sf::Vector2i pos, Chessboard* xboard) :
		Figure(color, pos, xboard) {

	notationSymbol = "";
	type = PAWN;
}

Figure* Pawn::clone() const {
	return new Pawn(*this);
}

bool Pawn::isMoveLegal(sf::Vector2i newPos, SpecialMove& special) {
	int dx = newPos.x - pos.x;
	int dy = newPos.y - pos.y;

	int finalY;
	if (color == W)
		finalY = 0;
	else if (color == B)
		finalY = 7;

	if (dx == 0 && dy == 0)
		return false;
	/* STANDARD PAWN MOVE FORWARD
	 * y has to be -1/+1 [w/b], x same as last,
	 * destination field is not occupied by any figure */
	else if ((dx == 0)
			&& ((dy == -1 && color == W) || (dy == 1 && color == B))) {
		if (xboard->field[newPos.x][newPos.y].getStatus() != U)
			return false;
		else {
			if (newPos.y == finalY)
				special = PROMOTE;
			return true;
		}
	}
	/* DOUBLE PAWN MOVE FORWARD (for starting position only)
	 * dy has to be -2/+2 [w/b], x same as last,
	 * y has to be 6/1 [w/b] which is starting field line
	 * destination field and field between is not occupied
	 * by any figure */
	else if ((dx == 0)
			&& ((dy == -2 && color == W && pos.y == 6
					&& xboard->field[newPos.x][newPos.y + 1].getStatus() == U)
					|| (dy == 2 && color == B && pos.y == 1
							&& xboard->field[newPos.x][newPos.y - 1].getStatus()
									== U))) {
		if (xboard->field[newPos.x][newPos.y].getStatus() != U)
			return false;
		else
			return true;

	}
	/* CAPTURE ENEMY FIGURES
	 * dy = -1/+1 [w/b], |dx| = 1,
	 * destination field must be occupied by enemy figure */
	else if (abs(dx) == 1
			&& ((dy == -1 && color == W) || (dy == 1 && color == B))
			&& xboard->locateField(newPos)->getStatus() != color
			&& xboard->locateField(newPos)->getStatus() != U) {
		if (newPos.y == finalY)
			special = PROMOTE;
		return true;
	} else if (abs(dx) == 1
			&& ((dy == -1 && color == W) || (dy == 1 && color == B))) {
		/* EN PASSANT CAPTURE
		 * y has to be -1/+1 [w/b], |dx| = 1,
		 * last move of opponent was from rank 2 to 4 [white]
		 * or from rank 7 to 6 [black] */
		int e, f;
		if (color == W) {
			e = 1;
			f = 1;
		} else if (color == B) {
			e = -1;
			f = 6;
		}
		if (!xboard->moves.size()) // to avoid memory error when there was no moves
			return false;
		if (xboard->moves[xboard->moves.size() - 1].fromPos
				== xboard->field[newPos.x][f].getBoardPos()
				&& xboard->moves[xboard->moves.size() - 1].toPos
						== xboard->field[newPos.x][f + 2 * e].getBoardPos()
				&& xboard->moves[xboard->moves.size() - 1].figure == PAWN
				&& xboard->moves[xboard->moves.size() - 1].toPos
						== xboard->field[newPos.x][newPos.y + e].getBoardPos()) {

			special = EN_PASSANT;
			return true;
		}
	}
	return false;
}

void Pawn::setGraphics() {
	if (color == W)
		texture.loadFromFile(W_PAWN_PNG);
	else
		texture.loadFromFile(B_PAWN_PNG);
	setTexture(texture);
}

Pawn::~Pawn() {
}

