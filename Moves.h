#pragma once

#include "Header.h"

const int MAX_MOVES = 256;

struct Move {
	Square from, to;
	int info = 0; // 1 - en passant
	Move () : from(SQ_A1), to(SQ_A1) {}

	Move(Square from, Square to, int info = 0) : from(from), to(to), info(info) {};

	operator bool() const {
		return from != to;
	}
};

inline Move string_to_move(string s) {
	return Move(string_to_square(s.substr(0, 2)), string_to_square(s.substr(2, 2)));
}

struct MoveList {
	Move moves[MAX_MOVES];
	size_t size = 0;

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
};

