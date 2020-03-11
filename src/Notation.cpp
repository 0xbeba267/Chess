#include "Notation.h"

using namespace std;

Notation::Notation() {
	// set stating player white and number of turn as 1.
	currPlayer = W;
	turn = 1;
	prepareNextMove();
}

void Notation::prepareNextMove() {
	// set all variables as default and strings as empty
	captured = false;
	move = "";
	figureSymbol = "";
	starting_field = "";
	castling = NO_CASTLING;
	check = NO_CHECK;
	promotingTo = "";
}

void Notation::sendOutput() {
	string record;

	// add symbol of figure (always used)
	record += figureSymbol;

	// add starting field (when needed)
	record += starting_field;

	// add symbol of capturing (when needed)
	if (captured)
		record += ":";

	// add destination field (always used)
	record += move;

	// add promoted figure symbol (when needed)
	record += promotingTo;

	// castling replace whole record about move
	if (castling == SHORT_CASTLING)
		record = "O-O";
	else if (castling == LONG_CASTLING)
		record = "O-O-O";

	// add checking postfix if check is detected
	if (check == CHECK)
		record += "+";
	else if (check == CHECKMATE)
		record += "#";

	// write number of current move with complete record of it
	// also change the player
	if (currPlayer == W) {
		cout << turn << ". " << record << " ";
		currPlayer = B;
		if (check == CHECKMATE)
			cout << endl;

	} else {
		cout << record << endl;
		currPlayer = W;
		turn++;
	}

	// after writing output, clear all data for next move
	prepareNextMove();

}

Notation::~Notation() {
}

