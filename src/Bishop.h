#ifndef BISHOP_H_
#define BISHOP_H_

#include "Figure.h"

class Bishop: public Figure {
public:
	Bishop(ChessColor color, sf::Vector2i pos, Chessboard* xboard);
	Figure* clone() const override;

	bool isMoveLegal(sf::Vector2i newPos, SpecialMove& special);

	virtual ~Bishop();
};

#endif /* BISHOP_H_ */
