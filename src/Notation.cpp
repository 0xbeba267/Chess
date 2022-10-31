#include "Notation.h"

using namespace std;

Notation::Notation() {
	// set stating player white and number of turn as 1.
	currPlayer = W;
	turn = 1;
	prepareNextMove();

	if (lang == EN)
		captureSymbol = 'x';
	else if (lang == PL)
		captureSymbol = ':';
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
	en_passant = false;
}

void Notation::sendOutput() {
	string record;

	// add symbol of figure
	record += figureSymbol;

	// add starting field
	record += starting_field;

	// add symbol of capturing
	if (captured)
		record += captureSymbol;

	// add destination field
	record += move;

	// add promoted figure symbol
	record += promotingTo;

	// castling replace whole record about move
	if (castling == SHORT_CASTLING)
		record = "O-O";
	else if (castling == LONG_CASTLING)
		record = "O-O-O";

	// if there was capture en passant
	if (en_passant)
		record += " e.p.";

	// write check/checkmate symbol
	if (check == CHECK)
		record += "+";
	else if (check == CHECKMATE)
		record += "#";

	// write last move on standard output then switch current player (within this class only)
	if (currPlayer == W) {
		cout << turn << ". " << record << " ";
		currPlayer = B;
	} else {
		cout << record << endl;
		currPlayer = W;
		turn++;
	}

	fflush(stdout);

	// after writing output, clear all data for next move
	prepareNextMove();

}

Notation::~Notation() {
}

