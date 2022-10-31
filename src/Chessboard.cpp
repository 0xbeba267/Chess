#include "Chessboard.h"

Chessboard::Chessboard() {
}

Chessboard::Chessboard(int x, int y) {
	position = {x, y};
	background.setPosition(x - 30, y - 30);
	background.setSize(sf::Vector2f(8 * FIELD_SIZE + 60, 8 * FIELD_SIZE + 60));
	background.setFillColor(sf::Color {61, 73, 96});

	// assigment position and color for every chessboardfield
	for (int j = 0; j < 8; j++)
	for (int i = 0; i < 8; i++) {
		// create indication for fields based on x and y coords
		std::string indc;
		indc += static_cast<char>(97 + i);
		indc += static_cast<char>(49 + 7 - j);

		if ((i + j) % 2)
		field[i][j] = ChessboardField(i, j, i * FIELD_SIZE + x, j * FIELD_SIZE + y, B,
				indc);
		else
		field[i][j] = ChessboardField(i, j, i * FIELD_SIZE + x, j * FIELD_SIZE + y, W,
				indc);
	}

	font.loadFromFile(FONT_REGULAR);

	// prepare texts for indication for fields
	for (int i = 0; i < 8; i++) {
		// for int → string conversion
		std::ostringstream ssl, ssp, sst, ssb;

		// left side numbers
		indicationL[i].setCharacterSize(24);
		indicationL[i].setColor(sf::Color::White);
		indicationL[i].setFont(font);
		ssl << 8 - i;
		indicationL[i].setString(ssl.str());
		indicationL[i].setPosition(x - 20,
				y + i * FIELD_SIZE + 30 - indicationL[i].getGlobalBounds().height / 2);
		indicationL[i].move(
				indicationL[i].getPosition().x
				- indicationL[i].getGlobalBounds().left,
				indicationL[i].getPosition().y
				- indicationL[i].getGlobalBounds().top);

		// right side numbers (reversed)
		indicationR[i].setCharacterSize(34);
		indicationR[i].setColor(sf::Color::White);
		indicationR[i].setFont(font);
		indicationR[i].setRotation(180);
		ssp << 8 - i;
		indicationR[i].setString(ssp.str());
		indicationR[i].setPosition(FIELD_SIZE * 8 + x + 32,
				y + i * FIELD_SIZE
				+ (FIELD_SIZE - indicationR[i].getGlobalBounds().height / 2));

		// bottom side letters
		indicationB[i].setCharacterSize(24);
		indicationB[i].setColor(sf::Color::White);
		indicationB[i].setFont(font);
		ssb << static_cast<char>(97 + i);
		indicationB[i].setString(ssb.str());
		indicationB[i].setPosition(
				x + i * FIELD_SIZE + (FIELD_SIZE - indicationB[i].getGlobalBounds().width) / 2,
				y + FIELD_SIZE * 8 + 1);

		// upper side letters (reversed)
		indicationT[i].setCharacterSize(34);
		indicationT[i].setColor(sf::Color::White);
		indicationT[i].setFont(font);
		indicationT[i].setRotation(180);
		sst << static_cast<char>(97 + i);
		indicationT[i].setString(sst.str());
		indicationT[i].setPosition(
				x + i * FIELD_SIZE + FIELD_SIZE
				- (FIELD_SIZE - indicationT[i].getGlobalBounds().width) / 2,
				y - 5);
	}
}

ChessboardField* Chessboard::locateFieldOnScreen(sf::Vector2f pos) {
	if (getGlobalBounds().contains(pos)) {
		int fx = (pos.x - getPosition().x) / FIELD_SIZE;
		int fy = (pos.y - getPosition().y) / FIELD_SIZE;
		return &field[fx][fy];
	}
	return nullptr;
}

ChessboardField* Chessboard::locateField(sf::Vector2i pos) {
	return &field[pos.x][pos.y];
}

void Chessboard::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(background);

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) {
			target.draw(field[i][j]);
		}

	for (int i = 0; i < 8; i++) {
		target.draw(rectL[i]);
		target.draw(indicationL[i]);
		target.draw(indicationB[i]);
		/* reversed indication for fields (indicationT[8]
		 * and indicationR) is disabled by default		 */

	}
}

sf::Vector2f Chessboard::getPosition() const {
	return position;
}

sf::FloatRect Chessboard::getGlobalBounds() const {
	return sf::FloatRect(getPosition().x, getPosition().y, FIELD_SIZE * 8,
			FIELD_SIZE * 8);
}

Chessboard::~Chessboard() {
	moves.clear();
}
