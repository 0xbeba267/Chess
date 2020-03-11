/**
 * Chessboard class - drawable object with some features.
 *
 * Despite Chessboard is sf::Drawable and contains graphical
 * content to display on screen, it's used as container
 * for 8x8 Chessboardfield array with some functions which
 * can locate them easy and vector of moves played on the board.
 * (The graphical content of this class it's used in
 * expanded, inherited class - ChessboardWithPieces).
 */

#ifndef CHESSBOARD_H_
#define CHESSBOARD_H_

#include <sstream>
#include <math.h>
#include "ChessboardField.h"

/**
 * Structure used to contain information about all shifts done
 * on chessboard. In addiction to start and end field for move,
 * the type of figure is also stored which can be very useful
 * in some situations ex. cheking en passant capture.
 * The last contained value is type of figure after move,
 * it matters when move is pawn promotion, otherwise
 * newFigure is always equals to the figure.
 */
struct Move {
	FigureType figure;		///< type of moved figure
	sf::Vector2i fromPos;	///< starting position
	sf::Vector2i toPos;		///< final position
	FigureType newFigure;	///< type of figure after move
};

class Chessboard: public sf::Drawable {
public:
	Chessboard();
	Chessboard(int x, int y);
	virtual ~Chessboard();

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	ChessboardField* locateFieldOnScreen(sf::Vector2f pos);
	ChessboardField* locateField(sf::Vector2i pos);

	sf::Vector2f getPosition() const;
	sf::FloatRect getGlobalBounds() const;

	ChessboardField field[8][8];

	std::vector<Move> moves;
protected:
	sf::Vector2f position;
	sf::RectangleShape background;

	// indication for fields: A - F and 1 - 8
	sf::Text indicationL[8], indicationR[8], indicationT[8], indicationB[8];
	sf::RectangleShape rectL[8];

	sf::Font font;
};

#endif /* CHESSBOARD_H_ */
