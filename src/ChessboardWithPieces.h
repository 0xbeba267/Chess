/**
 * Completed Chessboard - contains chessboard and figures.
 *
 * Inhertied from drawable Chessboard. Draw method is
 * same as in Chessboard class cause figures are drawn
 * by the class Game.
 *
 * Despite useful functions for mananing chessboardfields which
 * already base class Chessboard has, the new features like
 * getting pointer to Figure placed on given position.
 */

#ifndef CHESSBOARDWITHPIECES_H_
#define CHESSBOARDWITHPIECES_H_

#include "Chessboard.h"
#include "Figure.h"

class ChessboardWithPieces: public Chessboard {
public:
	ChessboardWithPieces(int x, int y);
	ChessboardWithPieces(const ChessboardWithPieces & oldChessboard);
	ChessboardWithPieces & operator=(const ChessboardWithPieces & oldChessboard);

	virtual ~ChessboardWithPieces();
private:
	// get pointer to the Figure using vector of it's position on chessboard
	Figure* getFigureOnPos(sf::Vector2i pos);

	// set the new field for @param figure and update status for old and new field
	void moveFigureOnField(Figure* figure, ChessboardField* newField);

	// correctly remove figure from the chessboard
	void deleteFigure(Figure *toDelete);

	std::vector<Figure*> figures;

	friend class Game;
};

#endif /* CHESSBOARDWITHPIECES_H_ */
