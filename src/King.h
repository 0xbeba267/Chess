#ifndef KING_H_
#define KING_H_

#include "Figure.h"

class King: public Figure {
public:
	King(ChessColor color, sf::Vector2i pos, Chessboard* xboard);
	Figure* clone() const override;

	bool isMoveLegal(sf::Vector2i newPos, SpecialMove& special);

	virtual ~King();
};

#endif /* KING_H_ */
