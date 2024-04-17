#pragma once

#include "Header.h"

const int MAX_MOVES = 256;

const int PawnPromotionId = 19;
const int EnPassantId = 1;
const int CastlingK = 20, CastlingQ = 21, Castlingk = 22, Castlingq = 23;

struct Move {
	Square from, to;
	char info = 0;
	Move() : from(SQ_A1), to(SQ_A1) {}

	Move(Square from, Square to, char info = 0) : from(from), to(to), info(info) {};

	operator bool() const {
		return from != to;
	}
};


inline Move string_to_move(Piece* board, Bitboard enPassantTarget, string s) {
	Move mov(string_to_square(s.substr(0, 2)), string_to_square(s.substr(2, 2)));
	if (s.size() > 4) {
		if (s[4] == 'q' || s[4] == 'Q') {
			mov.info = QUEEN;
			return mov;
		}
		if (s[4] == 'k' || s[4] == 'K') {
			mov.info = KING;
			return mov;
		}
		if (s[4] == 'b' || s[4] == 'B') {
			mov.info = BISHOP;
			return mov;
		}
		if (s[4] == 'n' || s[4] == 'N') {
			mov.info = KNIGHT;
			return mov;
		}
		
		//mov.info = stoi(s.substr(4, s.size() - 4));
	}
	if (type_of(board[mov.from]) == KING) {
		if (mov.to == SQ_G1 && mov.from == SQ_E1) {
			mov.info = CastlingK;
		}
		if (mov.to == SQ_C1 && mov.from == SQ_E1) {
			mov.info = CastlingQ;
		}
		if (mov.to == SQ_G8 && mov.from == SQ_E8) {
			mov.info = Castlingk;
		}
		if (mov.to == SQ_C8 && mov.from == SQ_E8) {
			mov.info = Castlingq;
		}
		return mov;
	}
	if (type_of(board[mov.from]) == PAWN && (
		(mov.from >= SQ_A2 && mov.from <= SQ_H2 && mov.to >= SQ_A4) ||
		(mov.from >= SQ_A7 && mov.from <= SQ_H7 && mov.to <= SQ_H5))) {
		mov.info = PawnPromotionId;
		return mov;
	}
	if (type_of(board[mov.from]) == PAWN && column(mov.from) != column(mov.to) && board[mov.to] == NO_PIECE) {
		mov.info = EnPassantId;
	}
	if (type_of(board[mov.from]) == PAWN && (mov.to <= SQ_H1 || mov.to >= SQ_A8)) {
		mov.info = QUEEN;
	}
	return mov;
}

string move_to_string(Move move) {
	string res = square_to_string(move.from) + square_to_string(move.to);
	if (move.info) {
		if (move.info == KNIGHT) {
			res += 'n';
			return res;
		}
		if (move.info == BISHOP) {
			res += 'b';
			return res;
		}
		if (move.info == ROOK) {
			res += 'r';
			return res;
		}
		if (move.info == QUEEN) {
			res += 'q';
			return res;
		}
		//res += to_string(move.info);
	}
	return res;
}

struct Position;
struct SearchConfig;

struct MoveList {
	Move moves[MAX_MOVES];
	size_t size = 0;

	void Movelist() {
		memset(moves, 0, sizeof(moves));
	}

	void add(Move move) {
		moves[size++] = move;
	}

	bool contains(Move move) {
		for (size_t i = 0; i < size; i++) {
			if (moves[i] == move) {
				return true;
			}
		}
		return false;
	}

	void sort(const Position& pos, SearchConfig& config);
};


struct Temp {
	MoveList Moves;
	MoveList PriorityMoves[36];
};

Temp temp;