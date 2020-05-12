#ifndef KNIGHT_H_
#define KNIGHT_H_

#include "Figure.h"

class Knight: public Figure {
public:
	Knight(ChessColor color, sf::Vector2i pos, Chessboard* xboard);
	Figure* clone() const override;

	bool isMoveLegal(sf::Vector2i newPos, SpecialMove& special);

	void setGraphics();

	virtual ~Knight();
};

#endif /* KNIGHT_H_ */
