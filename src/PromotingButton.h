#ifndef PROMOTINGBUTTON_H_
#define PROMOTINGBUTTON_H_

#include "Global.h"

class PromotingButton : public sf::Drawable {
public:
	PromotingButton(FigureType figure, ChessColor color, sf::Vector2f pos);
	virtual ~PromotingButton();

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
private:
	FigureType figure;

	// background of the button, color changes with mouse events
	sf::RectangleShape bg;

	// texture for sprite which display figure to choose
	sf::Texture figureTexture;
	sf::Sprite figureImg;

	friend class Game;
};

#endif /* PROMOTINGBUTTON_H_ */
