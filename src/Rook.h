#ifndef ROOK_H_
#define ROOK_H_

#include "Figure.h"

class Rook: public Figure {
public:
	Rook(ChessColor color, sf::Vector2i pos, Chessboard* xboard);
	Figure* clone() const override;

	bool isMoveLegal(sf::Vector2i newPos, SpecialMove& special);

	virtual ~Rook();
};

#endif /* ROOK_H_ */
