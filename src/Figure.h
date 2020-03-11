/**
 * Abstract class for each figure.
 *
 * Contains everything what inherited figure use:
 * current and last position on board, color, graphical
 * texture and information about type.
 *
 * Every kind of figure contains has also pointer to Chessboard.
 * It's named xboard. Using name 'xboard' instead 'chessboard'
 * means, that this pointer is used only to get information
 * about statues of chessboardField[8][8] (white/black/empty
 * figure occupies this field). There is no other use for this
 * pointer. Above there is illustration what kind of information
 * figures are able to get:
 * {{B, U, B, B, B, B, U, B},
 *  {B, B, B, B, U, B, B, B},
 *  {U, U, B, U, U, U, U, U},
 *  {U, U, U, U, B, U, U, U},
 *  {U, U, U, U, W, U, U, U},
 *  {U, U, W, U, U, U, U, U},
 *  {W, W, W, W, U, W, W, W},
 *  {W, U, W, W, W, W, W, W}}
 *  Let make a simple example to see usefulness of this array.
 *  White rook on field "a1" can see that "b1" is
 *  an empty field next to it, so move here is legal,
 *  but if player try to move that rook  on other field, for
 *  ex. on "a5" it's not allowed, cause field between ("a2")
 *  is occupied.
 */

#ifndef FIGURE_H_
#define FIGURE_H_

#include "Global.h"

#include "Chessboard.h"

#include <iostream>
#include <cmath>

enum SpecialMove {
	NONE, EN_PASSANT, PROMOTE, CASTLING
};

// it allows default value sent by reference in isMoveLegal function
static SpecialMove notCheckingSpecial = NONE;

class Figure: public sf::Sprite {
public:
	Figure(ChessColor color, sf::Vector2i pos, Chessboard* xboard);
	Figure(const Figure & old);

	/**
	 * Pure virtual cloning objects method
	 * overrided in inherited figures.
	 *
	 * It allows program to copy vector
	 * of dynamically created figures of various types.
	 */
	virtual Figure* clone() const = 0;

	/**
	 * Pure virtual function checking correctness of move.
	 * Theoretically.
	 *
	 * It checks:
	 * -if movement is compatible with movement of this
	 *  kind of figure,
	 * -all fields between starting and final:
	 * 		cannot be occupied,
	 * -final field:
	 * 		can't be owned by other figure of player.
	 *
	 * However it not provide if own king is checked after move -
	 * it's role of Game class to confirm any move
	 * (figure don't know anything about other figures,
	 * only color of occupier for any field of chessboard).
	 *
	 * First @param is position of fields where figure
	 * tries to move on and this is what this function checks -
	 * @returns if figure can move here or not.
	 *
	 * There is also second @param for detect special
	 * moves, sent by reference. It's used to get know
	 * if any special move was done and which one if there was.
	 * This parameter is optional.
	 */
	virtual bool isMoveLegal(sf::Vector2i pos, SpecialMove & special =
			notCheckingSpecial) = 0;

	virtual ~Figure();
protected:
	// pointer to the chessboard used for information about fields' occupiers
	Chessboard* xboard;

	// {x,y} coords of current and last position of figure on board
	sf::Vector2i pos;
	sf::Vector2i oldPos;

	// color of the figure, so the color of it's owner
	ChessColor color;

	// what figure it is (PAWN, KING, ROOK etc.)
	FigureType type;

	// used by inherited figures to draw them
	sf::Texture texture;

	// symbol of figure (for pawn its "")
	std::string notationSymbol;

	friend class Game;
	friend class ChessboardWithPieces;
};

#endif /* FIGURE_H_ */
