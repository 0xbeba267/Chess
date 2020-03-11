#include "ChessboardField.h"

ChessboardField::ChessboardField() {
}

ChessboardField::ChessboardField(int boardX, int boardY, int x, int y,
		ChessColor color, std::string indc) {
	onBoardPos = sf::Vector2i(boardX, boardY);
	setPosition(sf::Vector2f(x, y));
	setSize(sf::Vector2f(FIELD_SIZE, FIELD_SIZE));

	this->indc = indc;

	if (color == B) {
		setFillColor(sf::Color { 124, 147, 192 });
	} else if (color == W) {
		setFillColor(sf::Color { 234, 234, 234 });
	}

	status = U;	// unoccupied by default
}

ChessboardField::ChessboardField(const ChessboardField & old) {
	setPosition(old.getPosition());
	setSize(old.getSize());

	onBoardPos = old.onBoardPos;

	indc = old.indc;
	status = old.status;
}

sf::Vector2i ChessboardField::getBoardPos() {
	return onBoardPos;
}

ChessColor ChessboardField::getStatus() const {
	return status;
}

void ChessboardField::setStatus(ChessColor status) {
	this->status = status;
}

ChessboardField::~ChessboardField() {
}
