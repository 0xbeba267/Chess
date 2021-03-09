#include "King.h"

King::King(ChessColor color, sf::Vector2i pos, Chessboard* xboard) :
		Figure(color, pos, xboard) {

	if (lang == EN)
		notationSymbol = 'K';
	else if (lang == PL)
		notationSymbol = 'K';

	type = KING;
}

Figure* King::clone() const {
	return new King(*this);
}

bool King::isMoveLegal(sf::Vector2i newPos, SpecialMove& special) {
	int dx = newPos.x - pos.x;
	int dy = newPos.y - pos.y;

	if (dx == 0 && dy == 0)
		return false;
	/* KINGS MOVE
	 * |dx| = 1 or |dy| =1
	 * destination field is not occupied by own figure */
	else if (abs(dx) <= 1 && abs(dy) <= 1) {
		// check if destination field is not occupied by own figure
		if (xboard->locateField(newPos)->getStatus() == color)
			return false;
		else
			return true;
		/* CASTLING
		 * |dx| = 2 and dy = 0
		 * king and rook (from castling side) haven't moved
		 * all fields between king and rook must be empty */
	} else if (abs(dx) == 2 && dy == 0) {
		if (!xboard->moves.size()) // to avoid memory error when there was no moves
			return false;
		// check if king was moved
		for (auto m : xboard->moves) {
			if (color == ChessColor::W)
				if (m.fromPos.x == 4 && m.fromPos.y == 7)
					return false;
			if (color == ChessColor::B)
				if (m.fromPos.x == 4 && m.fromPos.y == 0)
					return false;
		}
		// check if left rook was moved (a1/a8 for white/black)
		if (dx == -2) {
			if (color == ChessColor::W)
				for (auto m : xboard->moves)
					if (m.fromPos.x == 0 && m.fromPos.y == 7)
						return false;
			if (color == ChessColor::B)
				for (auto m : xboard->moves)
					if (m.fromPos.x == 0 && m.fromPos.y == 0)
						return false;
		}
		// check if right rook was moved (f1/f8 for white/black)
		else if (dx == 2) {
			if (color == ChessColor::W)
				for (auto m : xboard->moves)
					if (m.fromPos.x == 7 && m.fromPos.y == 7)
						return false;
			if (color == ChessColor::B)
				for (auto m : xboard->moves)
					if (m.fromPos.x == 7 && m.fromPos.y == 0)
						return false;
		}
		// check if least one field between king and rook is occupied
		for (int x = pos.x + dx / abs(dx); x != newPos.x; x += dx / abs(dx)) {
			if (xboard->field[x][newPos.y].getStatus() != U)
				return false;
		}
		// sucess castling
		special = CASTLING;
		return true;
	} else
		return false;
}

void King::setGraphics() {
	if (color == W)
		texture.loadFromFile(W_KING_PNG);
	else
		texture.loadFromFile(B_KING_PNG);
	setTexture(texture);
}

King::~King() {
}
