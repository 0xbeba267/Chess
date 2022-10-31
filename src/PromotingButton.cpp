#include "PromotingButton.h"

PromotingButton::PromotingButton(FigureType figure, ChessColor color,
		sf::Vector2f pos) {
	this->figure = figure;

	// set background of promoting button
	bg.setFillColor(sf::Color{0, 127, 255});
	bg.setPosition(pos);
	bg.setSize(sf::Vector2f(60, 60));

	if (color == W)
		switch (figure) {
		case ROOK:
			figureTexture.loadFromFile(W_ROOK_PNG);
			break;
		case KNIGHT:
			figureTexture.loadFromFile(W_KNIGHT_PNG);
			break;
		case BISHOP:
			figureTexture.loadFromFile(W_BISHOP_PNG);
			break;
		case QUEEN:
			figureTexture.loadFromFile(W_QUEEN_PNG);
			break;
		}
	else if (color == B)
		switch (figure) {
		case ROOK:
			figureTexture.loadFromFile(B_ROOK_PNG);
			break;
		case KNIGHT:
			figureTexture.loadFromFile(B_KNIGHT_PNG);
			break;
		case BISHOP:
			figureTexture.loadFromFile(B_BISHOP_PNG);
			break;
		case QUEEN:
			figureTexture.loadFromFile(B_QUEEN_PNG);
			break;
		}
	// set loaded texture
	figureImg.setTexture(figureTexture);
	figureImg.setPosition(pos);


}

void PromotingButton::draw(sf::RenderTarget &target,
		sf::RenderStates states) const {
	target.draw(bg);
	target.draw(figureImg);
}

PromotingButton::~PromotingButton() {
}

