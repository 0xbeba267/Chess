/**
 * The role of Notation is preparing records of alebraic
 * notation for moves and send them to the output of program.
 *
 * Polish format example:
 * 1. e4 e5
 * 2. Sf3 Sf6
 * 3. S:e5 S:e4
 */

#ifndef NOTATION_H_
#define NOTATION_H_

#include "Global.h"

#include <iostream>

enum Castling {
	NO_CASTLING,	///< default state
	SHORT_CASTLING,
	LONG_CASTLING
};

enum Check {
	NO_CHECK,		///< default state
	CHECK,
	CHECKMATE
};

class Notation {
public:
	Notation();

	// clear all data before writing next move
	void prepareNextMove();

	// write current record in standard output of program
	void sendOutput();

	virtual ~Notation();
private:
	std::string figureSymbol;
	std::string move;
	std::string starting_field;
	std::string promotingTo;

	// if yes write ":" between before destination field
	bool captured;

	// if yes write "e.p." at the end
	bool en_passant;

	// if there is, replace record by "O-O" or "O-O-O"
	Castling castling;

	// if there is, write + or # at the end of record
	Check check;

	// number of current turn, which increase when both of players made move
	unsigned int turn;

	// color of current player (used to increase number of turn after black moved)
	ChessColor currPlayer;

	friend class Game;
};

#endif /* NOTATION_H_ */
