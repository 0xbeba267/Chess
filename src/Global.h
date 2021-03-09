#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <SFML/Graphics.hpp>	//  todo przenieś to jeśli masz klasy abstrakcyjne (w sensie bez grafiki)

enum Lang {
	PL,
	EN,
};

extern Lang lang;


#define INPUT_END 0		// INPUT_END means where the pipe takes input
#define OUTPUT_END 1	// OUTPUT_END means where the pipe produces output

// number of pixels in width and height per one chessboard field
const float FIELD_SIZE = 60;

// path for graphics
#define W_PAWN_PNG "images/Chess_plt60.png"
#define B_PAWN_PNG "images/Chess_pdt60.png"
#define W_ROOK_PNG "images/Chess_rlt60.png"
#define B_ROOK_PNG "images/Chess_rdt60.png"
#define W_KNIGHT_PNG "images/Chess_nlt60.png"
#define B_KNIGHT_PNG "images/Chess_ndt60.png"
#define W_BISHOP_PNG "images/Chess_blt60.png"
#define B_BISHOP_PNG "images/Chess_bdt60.png"
#define W_QUEEN_PNG "images/Chess_qlt60.png"
#define B_QUEEN_PNG "images/Chess_qdt60.png"
#define W_KING_PNG "images/Chess_klt60.png"
#define B_KING_PNG "images/Chess_kdt60.png"

#define FONT_REGULAR "fonts/tahoma.ttf"

enum FigureType {
	PAWN,
	ROOK,
	KNIGHT,
	BISHOP,
	QUEEN,
	KING
};

enum ChessColor {
	W,	///< White color
	B,	///< Black color
	U	///< Unoccupied (used only in contex of field's occupier)
};

#endif /* GLOBAL_H_ */
