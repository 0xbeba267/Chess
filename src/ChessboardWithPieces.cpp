#include "ChessboardWithPieces.h"

ChessboardWithPieces::ChessboardWithPieces(int x, int y) :
		Chessboard(x, y) {

}
ChessboardWithPieces::ChessboardWithPieces(
		const ChessboardWithPieces & oldChessboard) {
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) {
			field[i][j] = oldChessboard.field[i][j];
		}

	background = oldChessboard.background;
	position = oldChessboard.position;

	for (auto * p : oldChessboard.figures) {
		figures.push_back(p->clone());
		figures[figures.size() - 1]->xboard = this;
	}

	moves = oldChessboard.moves;

	for (int i = 0; i < 8; i++) {
		indicationL[i] = oldChessboard.indicationL[i];
		indicationR[i] = oldChessboard.indicationR[i];
		indicationT[i] = oldChessboard.indicationT[i];
		indicationB[i] = oldChessboard.indicationB[i];
		rectL[i] = oldChessboard.rectL[i];
	}

	font = oldChessboard.font;
}

ChessboardWithPieces & ChessboardWithPieces::operator=(
		const ChessboardWithPieces & oldChessboard) {

	// clear previous figures and moves first
	for (auto * f : figures)
		delete f;

	figures.clear();
	moves.clear();

	// rest is same as in copy constructor:

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) {
			field[i][j] = oldChessboard.field[i][j];
		}

	background = oldChessboard.background;
	position = oldChessboard.position;

	for (auto * p : oldChessboard.figures) {
		figures.push_back(p->clone());
		figures[figures.size() - 1]->xboard = this;
	}

	moves = oldChessboard.moves;

	for (int i = 0; i < 8; i++) {
		indicationL[i] = oldChessboard.indicationL[i];
		indicationR[i] = oldChessboard.indicationR[i];
		indicationT[i] = oldChessboard.indicationT[i];
		indicationB[i] = oldChessboard.indicationB[i];
		rectL[i] = oldChessboard.rectL[i];
	}

	font = oldChessboard.font;

	return *this;
}

Figure* ChessboardWithPieces::getFigureOnPos(sf::Vector2i pos) {
	for (auto f : figures)
		if (f->pos == pos)
			return f;
	return nullptr;
}

void ChessboardWithPieces::moveFigureOnField(Figure* figure,
		ChessboardField* newField) {
	figure->pos = newField->getBoardPos();
	figure->setPosition(newField->getPosition());
	locateField(figure->oldPos)->setStatus(U);
	locateField(figure->pos)->setStatus(figure->color);

}

void ChessboardWithPieces::deleteFigure(Figure *toDelete) {
	for (unsigned int i = 0; i < figures.size(); i++)
		// remove figure from the field
		if (figures[i] == toDelete) {
			delete figures[i];
			figures.erase(figures.begin() + i);
			break;
		}
}

ChessboardWithPieces::~ChessboardWithPieces() {
	for (auto * f : figures)
		delete f;
	figures.clear();
}
