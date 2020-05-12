#include "Figure.h"

Figure::Figure(ChessColor color, sf::Vector2i pos, Chessboard* xboard) {
	this->pos = pos;
	oldPos = pos;

	this->xboard = xboard;

	this->color = color;
	xboard->locateField(pos)->setStatus(color);

	setPosition(xboard->locateField(pos)->getPosition());
}

Figure::Figure(const Figure & old) {
	pos = old.pos;
	oldPos = old.oldPos;

	xboard = old.xboard;

	setPosition(xboard->locateField(pos)->getPosition());

	type = old.type;
	color = old.color;
	notationSymbol = old.notationSymbol;
}

Figure::~Figure() {
}
