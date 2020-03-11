/**
 * Game manages almost whole program. It embrace control of program
 * after program's window is created and ends with closing it.
 *
 * All actions for game are managed from main loop which is
 * part of run() procedure. Servicing some events may depend
 * on current gameState (object of enum type).
 *
 * The main components of Game are:
 * a) ChessboardWithPieces - graphical chessboard which contains
 * also moves, figures and many functions to work with the figures
 * and fields.
 * b) Notation - object of this class writes algebraic
 * notation for every move. Game has only to to set few variables
 * and necessary information like indication of field and destination
 * field in move, then output is generated in object class
 * time when notation is send to output.
 *
 * The other part of game is communication with AI.
 * Method used in the program is running external program
 * (stockfish) with input generated in form of moves sequence
 * then redirect output of program and read bestmove of stockfish
 * from it.
 *
 * If stockfish haven't started sucessfully (there is readyok test
 * runned at the beggining of program), game continues but player
 * has to move figures of both colors.
 */

#ifndef GAME_H_
#define GAME_H_

#include "ChessboardWithPieces.h"
#include "Notation.h"
#include "PromotingButton.h"

#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Pawn.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <unistd.h>

enum GameState {
	PLAYING,	///< regular play, current player can move
	PROMOTING,	///< wait until promotion is done and back to PLAYING
	FINISHED	///< game is finished, moving is blocked
};

class Game {
public:
	Game();

	void run();
	void initialize();

	// event function which role is catch mouse click on figure
	void tryClickOn(Figure &figure);

	// event functions which work on currently dragged figure
	void dragFigure();
	void placeDraggedFigure();

	void promotingSelectorEvents(sf::Event & e, PromotingButton* btn);

	// generate selector for new figure and turn on PROMOTING game state
	void popPromotingSelector(Figure* pawn, ChessColor color);

	/**
	 * Returns if move of given figure to given field is fully legal,
	 * Also executes all actions for the special moves if they are.
	 *
	 * If move is not correct by any reason, chessboard assigns
	 * board state before trying that move.
	 *
	 * Second @param is used to turn on test mode, which always
	 * backs to the original board state after try. It's used to
	 * @return if move is legal or not.
	 */
	bool tryPlayMove(Figure* figure, ChessboardField* newField, bool testMode =
				false);

	// functions which role is only @return if there is check/checkmate
	bool isKingChecked(ChessColor color);
	bool isKingCheckmated(ChessColor color);

	// store information about move and write notation record
	void acceptMove(Move moveToAccept);

	// let AI make current move on chessboard
	void letEngineMove();

	virtual ~Game();
private:
	GameState gameState;

	ChessboardWithPieces chessboard;

	// figure currently dragged by mouse (if there is)
	Figure* dragged;

	// it's used to make graphical effect of changing promoted pawn
	Figure* promotedPawn;

	// figures to choose as pawn promotion - dynamically generated buttons
	std::vector<PromotingButton*> promoBtn;

	// class which allows to write algebraic notation as output
	Notation notation;

	// player which actions are consider
	ChessColor currPlayer;

	// semi-transparent rectangles which show last move on board
	sf::RectangleShape rectForLastField_from, rectForLastField_to;

	// if it's on, computer make moves as one side
	bool vsAI;

	/****************** Communication with stockfish ******************/
	// check if connection with stockfish is fine
	bool initAI();

	// if yes, AI will make move in the next iteration of game loop
	bool waitingForStockfishAnswer;

	// file which represent input sent to stockfish
	std::fstream inputFile;

	// file which represent output get by stockfish
	std::fstream outputFile;
};

#endif /* GAME_H_ */
