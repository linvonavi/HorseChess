#pragma once

#include "Movegen.h"

short Position::material_eval() {
	short res = 0;
	res += popcount(byType[PAWN] & byColor[WHITE]);
	res += popcount(byType[KNIGHT] & byColor[WHITE]) * 3;
	res += popcount(byType[BISHOP] & byColor[WHITE]) * 3;
	res += popcount(byType[ROOK] & byColor[WHITE]) * 5;
	res += popcount(byType[QUEEN] & byColor[WHITE]) * 9;
	res -= popcount(byType[PAWN] & byColor[BLACK]);
	res -= popcount(byType[KNIGHT] & byColor[BLACK]) * 3;
	res -= popcount(byType[BISHOP] & byColor[BLACK]) * 3;
	res -= popcount(byType[ROOK] & byColor[BLACK]) * 5;
	res -= popcount(byType[QUEEN] & byColor[BLACK]) * 9;
	return res;
}

short Position::eval() {
	bool poss_move = is_exist_legal_move();
	if (!poss_move) {
		if (is_attacked_square(get_square(byType[KING] & byColor[sideToMove]))) {
			return sideToMove == WHITE ? -10000 : 10000;
		}
		return 0;
	}
	return material_eval();
}