#ifndef QUEEN_H_
#define QUEEN_H_

#include "Figure.h"

class Queen: public Figure {
public:
	Queen(ChessColor color, sf::Vector2i pos, Chessboard* xboard);
	Figure* clone() const override;

	bool isMoveLegal(sf::Vector2i newPos, SpecialMove& special);

	void setGraphics();

	virtual ~Queen();
};

#endif /* QUEEN_H_ */
