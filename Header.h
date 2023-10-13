#pragma once

#include <bits/stdc++.h>
#include <random>
using namespace std;

mt19937 rnd(0);
mt19937_64 rnd64(0);

enum Piece {
	NO_PIECE,
	W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
	B_PAWN = W_PAWN + 8, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING,
	PIECE_CNT = 16
};

char Piece_names[] = ".PNBRQK..pnbrqk";

enum Color {
	WHITE, BLACK, COLOR_CNT
};

enum Type {
	NO_PIECE_TYPE, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING,
	ALL_PIECES = 0,
	TYPE_CNT = 7
};

inline Type type_of(Piece pc) {
	return Type(pc & 7);
}

inline Color color_of(Piece pc) {
	return Color(pc >> 3);
}

enum Square : int {
	SQ_A1, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1,
	SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2,
	SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3,
	SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4,
	SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5,
	SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6,
	SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7,
	SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8,
	SQUARE_CNT
};

using Bitboard = uint64_t;

inline Bitboard square_bb(Square s) {
	return Bitboard(1) << s;
}

// bitboard.print()
inline void print(Bitboard bb) {
	for (Square s = SQ_A8; s >= 0; s = Square(int(s) + 1 - 16 * ((s + 1) % 8 == 0))) {
		cout << ((bb >> s) & 1) << ' ';
		if (s % 8 == 7) cout << '\n';
	}
}

inline Bitboard operator|(Square s1, Square s2) { return square_bb(s1) | square_bb(s2); }

const string Start_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -";