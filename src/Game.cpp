#include "Game.h"

using namespace sf;
using namespace std;

std::string exec_test(const char* cmd) {
	array<char, 128> buffer;
	string result;

	FILE *pipe_fp;
	if ((pipe_fp = popen(cmd, "r")) == NULL)
		throw std::runtime_error("popen() failed!");

	int line = 0;
	while (fgets(buffer.data(), buffer.size(), pipe_fp) != nullptr) {
		result += buffer.data();
	}
	pclose(pipe_fp);

	return result;
}

Game::Game() :
		window(VideoMode(910, 670), "Chess", Style::Titlebar | Style::Close), chessboard(
				200, 80) {
	currPlayer = W;

	rectForLastField_from.setFillColor(sf::Color { 255, 255, 0, 48 });
	rectForLastField_to.setFillColor(sf::Color { 255, 255, 0, 48 });
	rectForLastField_from.setSize( { FIELD_SIZE, FIELD_SIZE });
	rectForLastField_to.setSize( { FIELD_SIZE, FIELD_SIZE });

	waitingForStockfishAnswer = false;

	dragged = nullptr;
}

void Game::run() {
	initialize();
	gameState = PLAYING;

	// turn vs computer mode if connector to stockfish works correctly
	if (testConnector()) {
		vsAI = true;
	} else {
		vsAI = false;
		cout << "Can't find stockfish. AI is off.\n";
	}

	/**************** MAIN LOOP OF THE GAME **************/
	while (window.isOpen()) {
		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed)
				window.close();

			/*============ PLAYING ============
			 * standard mode
			 * check events for moving figures */
			if (gameState == PLAYING) {
				// try to catch any figure by mouseclick (if none is dragged)
				if (dragged == nullptr) {
					if (e.type == sf::Event::MouseButtonPressed
							&& sf::Mouse::isButtonPressed(sf::Mouse::Left))
						for (auto * f : chessboard.figures)
							if (f->color == currPlayer)
								tryClickOn(*f);
				}
				// when mouse button is released, try to place dragged figure on new field
				else if (e.type == sf::Event::MouseButtonReleased) {
					placeDraggedFigure();
					dragged = nullptr;
				} else
					// drag figure with mouse until it isn't released
					dragFigure();
			}
			/*=========== PROMOTING ============
			 * moving of figures is halted
			 * player has to choose his new figure */
			else if (gameState == PROMOTING) {
				for (auto * b : promoBtn) {
					promotingSelectorEvents(e, b);
				}
			}
			/*============ FINISHED =============
			 * game is finished with some result
			 * waiting for restart signal */
			else if (gameState == FINISHED) {

			}
		}

		/* -------- DRAW EVERYTHING ON SCREEN -------- */
		window.clear();
		window.draw(chessboard);

		// draw places of last move
		if (chessboard.moves.size()) {
			window.draw(rectForLastField_from);
			window.draw(rectForLastField_to);
		}

		// draw each figure on chessboard
		for (auto * f : chessboard.figures) {
			// do not draw dragged figure now
			if (f == dragged)
				continue;
			window.draw(*f);
		}
		// draw dragged figure (to place it on the top)
		if (dragged)
			window.draw(*dragged);

		// draw buttons for pawn's promotion (if they exist)
		for (auto * b : promoBtn)
			window.draw(*b);

		window.display();

		// let stockfish play next move
		if (vsAI)
			if (waitingForStockfishAnswer)
				letEngineMove();

	}/****************************************************/
}

void Game::initialize() {
	// matrix which contains numbers representing figures and thier colors
	int board[8][8] = { -1, -2, -3, -4, -5, -3, -2, -1, -6, -6, -6, -6, -6, -6,
			-6, -6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 1, 2, 3,
			4, 5, 3, 2, 1 };
	ChessColor currentColor;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) {
			// copy color of figure first
			if (board[i][j] > 0)
				currentColor = W;
			else
				currentColor = B;

			// create all figures based on number in matrix board
			if (abs(board[i][j]) == 1)
				chessboard.figures.push_back(
						new Rook(currentColor, { j, i }, &chessboard));
			if (abs(board[i][j]) == 2)
				chessboard.figures.push_back(
						new Knight(currentColor, { j, i }, &chessboard));
			if (abs(board[i][j]) == 3)
				chessboard.figures.push_back(
						new Bishop(currentColor, { j, i }, &chessboard));
			if (abs(board[i][j]) == 4)
				chessboard.figures.push_back(
						new Queen(currentColor, { j, i }, &chessboard));
			if (abs(board[i][j]) == 5)
				chessboard.figures.push_back(
						new King(currentColor, { j, i }, &chessboard));
			if (abs(board[i][j]) == 6)
				chessboard.figures.push_back(
						new Pawn(currentColor, { j, i }, &chessboard));
		}
	// load graphics for each figure
	for (auto f : chessboard.figures)
		f->setGraphics();
}

void Game::tryClickOn(Figure &figure) {
	sf::Vector2f m_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(
			window));
	// check if there is click action on non-dragged figure
	if (figure.getGlobalBounds().contains(m_pos)) {
		dragged = &figure;
	}
}

void Game::dragFigure() {
	sf::Vector2f m_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(
			window));
	dragged->setPosition(m_pos - sf::Vector2f(30, 30));
}

void Game::placeDraggedFigure() {
	sf::Vector2f m_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(
			window));

	// locate destination field using mouse
	if (ChessboardField* newField = chessboard.locateFieldOnScreen(m_pos))
		tryPlayMove(dragged, newField);
	else
		// if it's outside board, place figure back on its pos
		dragged->setPosition(
				chessboard.locateField(dragged->pos)->getPosition());

	// make figure no longer dragged
	dragged = nullptr;

	// reload graphics for pieces (chessboard could be reconstructed)
	for (auto f : chessboard.figures)
		f->setGraphics();
}

bool Game::tryPlayMove(Figure *figure, ChessboardField* newField,
		bool testMode) {

	// save backup of the chessboard
	ChessboardWithPieces chessboardBackup = chessboard;

	figure->oldPos = figure->pos;

	// any contradiction with chess rules toggle it off
	bool legalMv = true;

	// variable which is used to receive special move signal
	SpecialMove spcMv = NONE;

	// information about castling in current move
	Castling castling = NO_CASTLING;

	// destination field on which figure tries to move
	if (figure->isMoveLegal(newField->getBoardPos(), spcMv)) {
		/* move of current figure looks correct
		 *
		 * if something went wrong (ex. player's king is checked)
		 * chessboard backs to the backup of board anyway */

		// capture old figure if field was occupied
		if (newField->getStatus() != U) {
			// look for figure on field where current figure is going...
			for (auto * f : chessboard.figures) {
				// ...then remove beated figure
				if (f->pos == newField->getBoardPos()) {
					chessboard.deleteFigure(f);
					if (!testMode)
						notation.captured = true;
					// if pawn captured figure, send his column to the notation
					if (figure->type == PAWN)
						if (!testMode)
							notation.starting_field = chessboard.locateField(
									figure->pos)->indc[0];
					break;
				}
			}
		}

		// do addicional actions when move is EN PASSANT
		if (spcMv == EN_PASSANT) {
			if (!testMode) {
				notation.captured = true;
				notation.en_passant = true;
			}
			// clear status of field and remove last moved figure
			for (auto * f : chessboard.figures)
				if (f->pos
						== chessboard.moves[chessboard.moves.size() - 1].toPos) {
					chessboard.locateField(f->pos)->setStatus(U);
					chessboard.deleteFigure(f);
					break;
				}
		}
		// do addicional actions when move is CASTLING
		else if (spcMv == CASTLING) {
			// check if king isn't atacked on any field on path
			int dx = newField->getBoardPos().x - figure->oldPos.x;
			for (int x = figure->oldPos.x;
					x != newField->getBoardPos().x + dx / abs(dx);
					x += dx / abs(dx)) {
				// TODO aktualizuj status pola po drodze, po jest to potrzebne by spr. czy pion może tu bić
				figure->pos.x = x;
				if (isKingChecked(currPlayer))
					legalMv = false;
			}
			// castle queenside -> move rook from 'a' column to king's x + 1
			if (dx == -2)
				for (auto * f : chessboard.figures)
					if (f->pos.x == 0 && f->type == ROOK
							&& f->color == figure->color) {
						sf::Vector2i rookNewPos = figure->pos
								+ sf::Vector2i(1, 0);

						chessboard.moveFigureOnField(f,
								chessboard.locateField(rookNewPos));
						castling = LONG_CASTLING;
					}
			// castle kingside ->  move rook from 'f' column to king's x - 1
			if (dx == 2)
				for (auto * f : chessboard.figures)
					if (f->pos.x == 7 && f->type == ROOK
							&& f->color == figure->color) {
						sf::Vector2i rookNewPos = figure->pos
								+ sf::Vector2i(-1, 0);

						chessboard.moveFigureOnField(f,
								chessboard.locateField(rookNewPos));
						castling = SHORT_CASTLING;
					}
		}

		// disambiguating moves for notation
		// try to find other figure of same type moving on same field
		if (!testMode)
			for (auto * f : chessboard.figures)
				if (f->type == figure->type && f != figure
						&& f->color == figure->color
						&& f->isMoveLegal(newField->getBoardPos()))
					if (f->pos.x != figure->pos.x) {
						// add column of moved figure
						notation.starting_field = chessboard.locateField(
								figure->pos)->indc[0];
					} else if (f->pos.y != figure->pos.y) {
						// add row of moved figure
						notation.starting_field = chessboard.locateField(
								figure->pos)->indc[1];
//					TODO when player have 3 same figures, need to check all of them at once
//					} else {
//						// add column and row of moved figure
//						startingFieldPrefix = chessboard.locateField(
//								figure.field)->indc;
					}

		// place figure on the new field
		chessboard.moveFigureOnField(figure, newField);

		// cancel any moves which left player's king atacked
		if (isKingChecked(currPlayer)) {
			legalMv = false;
		}
	} else {
		// this is not correct move for current figure
		legalMv = false;
	}

	if (legalMv && !testMode) {
		// Correct move

		// set notation for current move
		notation.figureSymbol = figure->notationSymbol;
		notation.move = chessboard.locateField(figure->pos)->indc;

		// color both old and new field of moved figure
		rectForLastField_from.setPosition(
				(chessboard.locateField(figure->oldPos))->getPosition());
		rectForLastField_to.setPosition(newField->getPosition());

		if (castling != NO_CASTLING)
			notation.castling = castling;

		if (spcMv == PROMOTE) {
			// confirm move after selecting new figure
			popPromotingSelector(figure, figure->color);
			gameState = PROMOTING;

		} else {
			// confirm move now
			Move move { figure->type, figure->oldPos, newField->getBoardPos(),
					figure->type };
			acceptMove(move);

		}
	} else {
		// Uncorrect move (or test mode is on)
		// back to previous chessboard setup
		chessboard = chessboardBackup;
	}

	if (legalMv)
		return true;
	else
		return false;
}

void Game::promotingSelectorEvents(Event & e, PromotingButton* btn) {
	sf::Vector2f m_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(
			window));
	if (btn->bg.getGlobalBounds().contains(m_pos)) {
		// color button on orange when hovered
		btn->bg.setFillColor(sf::Color { 255, 127, 0 });
		// change preview texture of pawn to the potential new figure
		promotedPawn->setTexture(btn->figureTexture);

		// promote pawn when mouse releases on current button
		if (e.type == sf::Event::MouseButtonReleased) {
			string promoInfo;

			// place a new figure
			switch (btn->figure) {
			case ROOK:
				chessboard.figures.push_back(
						new Rook(promotedPawn->color, promotedPawn->pos,
								&chessboard));
				break;
			case KNIGHT:
				chessboard.figures.push_back(
						new Knight(promotedPawn->color, promotedPawn->pos,
								&chessboard));
				break;
			case BISHOP:
				chessboard.figures.push_back(
						new Bishop(promotedPawn->color, promotedPawn->pos,
								&chessboard));
				break;
			case QUEEN:
				chessboard.figures.push_back(
						new Queen(promotedPawn->color, promotedPawn->pos,
								&chessboard));
				break;
			}
			// add information on which figure pawn was promoted (last added figure)
			notation.promotingTo = chessboard.figures[chessboard.figures.size()
					- 1]->notationSymbol;

			// save position before removing pawn
			sf::Vector2i fromPos = promotedPawn->oldPos;
			sf::Vector2i toPos = promotedPawn->pos;

			// remove promoted pawn
			chessboard.deleteFigure(promotedPawn);

			// add information about move
			Move move { PAWN, fromPos, toPos, btn->figure };
			acceptMove(move);

			// back to the normal game, remove promoting selector
			gameState = PLAYING;
			for (auto * b : promoBtn)
				delete b;
			promoBtn.clear();
		}
	} else {
		// color button on blue when isn't hovered
		btn->bg.setFillColor(sf::Color { 0, 127, 255 });
	}
}

void Game::popPromotingSelector(Figure* pawn, ChessColor color) {
	promotedPawn = pawn;
	promotedPawn->setColor(sf::Color { 255, 255, 255, 163 });

	sf::Vector2f calculatedPos = chessboard.getPosition();
	calculatedPos.x -= 30 + FIELD_SIZE;
	FigureType figureList[4] = { QUEEN, KNIGHT, ROOK, BISHOP };
	for (int b = 0; b < 4; b++) {
		promoBtn.push_back(
				new PromotingButton(figureList[b], color, calculatedPos));
		calculatedPos.y += FIELD_SIZE;
	}
}

bool Game::isKingChecked(ChessColor color) {
	Figure* king;

	// first find player's king
	for (auto * f : chessboard.figures)
		if (f->type == KING && f->color == color) {
			king = f;
			break;
		}

	// check all opponent figures if any is able to atack king
	for (auto * f : chessboard.figures) {
		if (f->color != king->color)
			if (f->isMoveLegal(king->pos))
				return true;
	}
	return false;
}

bool Game::isKingCheckmated(ChessColor color) {
	// see if player, which king is checked has no legal move

	// try to play move any figure on any field
	for (int ii = 0; ii < 8; ii++)
		for (int jj = 0; jj < 8; jj++) {
			for (auto * f : chessboard.figures) {
				// figure color must same as king's owner
				if (f->color == color)
					// check if move is pseudolegal first
					if (f->isMoveLegal(Vector2i { ii, jj }))
						// then check if that move is legal using test mode
						if (tryPlayMove(f, &chessboard.field[ii][jj], true))
							return false;
			}
		}
	return true;
}

void Game::acceptMove(Move moveToAccept) {
	// switch player
	if (currPlayer == W)
		currPlayer = B;
	else if (currPlayer == B)
		currPlayer = W;

	// ! currPlayer in actions below is now opponent !

	// todo wyjeb to i zrobp orzadne sprawdzanie potem, bo tu nie mozna wewnatrz accept move
	if (currPlayer == B)
		waitingForStockfishAnswer = true;

	// check if opponent's king is checked
	if (isKingChecked(currPlayer)) {
		notation.check = CHECK;
		// recognize if there is also checkmate
		if (isKingCheckmated(currPlayer)) {
			notation.check = CHECKMATE;
			gameState = FINISHED;
		}
	}
	chessboard.moves.push_back(moveToAccept);
	notation.sendOutput();

	// reload graphics for pieces (chessboard could be reconstructed)
	for (auto * f : chessboard.figures)
		f->setGraphics();
}

bool Game::testConnector() {
	// 1) Run stockfish with "isready" parameter and get line number 1 from output
	std::string output = exec_test("./connector.out isready", 1);

	// 2) If asnswer is "readyok" means chess engine is ready to work
	string expected_answer = "readyok";
	string answer = output.substr(0, expected_answer.length());
	if (answer == expected_answer)
		return true;
	else
		return false;

		cout<< "Answer: " << output;
}

void Game::letEngineMove() {
	string moves_as_string;
	for (auto m : chessboard.moves) {
		string promoInfo = "";
		if (m.figure != m.newFigure)
		switch (m.newFigure) {
			case ROOK:
			promoInfo = "r";
			break;
			case KNIGHT:
			promoInfo = "n";
			break;
			case BISHOP:
			promoInfo = "b";
			break;
			case QUEEN:
			promoInfo = "q";
			break;
		}
		moves_as_string += chessboard.locateField(m.fromPos)->indc
		+ chessboard.locateField(m.toPos)->indc + promoInfo + " ";
	}

	const char* run_cmd = "./connector.out";

	string cmd = run_cmd;
	cmd += " \"" + moves_as_string + "\"";

	FILE *pipe_fp;
	if (( pipe_fp = popen(cmd.c_str(), "r")) == NULL)
	throw std::runtime_error("popen() failed!");

	std::array<char, 80> buffer;

	std::string output;	// output line with bestmove from ProgramConnector.out

	while (fgets(buffer.data(), buffer.size(), pipe_fp) != nullptr) {
		if (strncmp("bestmove", buffer.data(), 8) == 0) {
			output += buffer.data();

			pclose(pipe_fp);
			break;
		}
	}

	//cout << "`" << output << "`" << endl;

	// TODO try decode 3 characters from second move to make promotion automatically
	// decode output to get the best move as indication
	string moveFromIndc = output.substr(9, 2);
	string moveToIndc = output.substr(11, 2);

	Figure* movingFigure = nullptr;
	ChessboardField* newField = nullptr;
	// locate figure on starting field
	for (int i = 0; i < 8; i++)
	for (int j = 0; j < 8; j++) {
		if (chessboard.field[i][j].indc == moveFromIndc)
		movingFigure = chessboard.getFigureOnPos(
				chessboard.field[i][j].getBoardPos());
	}
	// find new field on the chessboard
	for (int i = 0; i < 8; i++)
	for (int j = 0; j < 8; j++) {
		if (chessboard.field[i][j].indc == moveToIndc)
		newField = &chessboard.field[i][j];
	}

	if (movingFigure && newField)
	tryPlayMove(movingFigure, newField);

	waitingForStockfishAnswer = false;
}

Game::~Game() {

}

