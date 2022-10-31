/**
 * ChessboardField represent one of 64 fields of board.
 * It's light or dark rectangle which is drawn as a part
 * of Chessboard class which contains all chessboardFields
 * (stored there in 8x8 array).
 *
 * Every field has also indication
 * for notation, it's containted in std::string object.
 *
 * Expect it's graphical (and notation) function the most
 * important role of ChessboardField is keeping status of
 * the field's occupier ([W]hite/[B]lack/[U]nocuppied).
 */

#ifndef CHESSBOARDFIELD_H_
#define CHESSBOARDFIELD_H_

#include "Global.h"

class Figure;

class ChessboardField: public sf::RectangleShape {
public:
	ChessboardField();

	ChessboardField(int boardX, int boardY, int x, int y, ChessColor k, std::string indc);

	// get or set current occupier of the field
	ChessColor getStatus() const;
	void setStatus(ChessColor status);

	// get {x, y} position on the board
	sf::Vector2i getBoardPos();

	std::string indc;	//	"a1", "a2" etc.

	virtual ~ChessboardField();
private:
	ChessColor status; //	[W]hite/[B]lack/[U]nocuppied

	sf::Vector2i onBoardPos;
};

#endif /* CHESSBOARDFIELD_H_ */
