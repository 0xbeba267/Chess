#ifndef PAWN_H_
#define PAWN_H_

#include "Figure.h"

class Pawn: public Figure {
public:
	Pawn(ChessColor color, sf::Vector2i pos, Chessboard* xboard);
	Figure* clone() const override;

	bool isMoveLegal(sf::Vector2i newPos, SpecialMove& special);

	void setGraphics();

	virtual ~Pawn();
};

#endif /* PAWN_H_ */
