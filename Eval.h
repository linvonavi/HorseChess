#pragma once

#include "Movegen.h"

short Position::material_eval() {
	short res = 0;
	res += __popcnt64(byType[PAWN] & byColor[WHITE]);
	res += __popcnt64(byType[KNIGHT] & byColor[WHITE]) * 3;
	res += __popcnt64(byType[BISHOP] & byColor[WHITE]) * 3;
	res += __popcnt64(byType[ROOK] & byColor[WHITE]) * 5;
	res += __popcnt64(byType[QUEEN] & byColor[WHITE]) * 9;
	res -= __popcnt64(byType[PAWN] & byColor[BLACK]);
	res -= __popcnt64(byType[KNIGHT] & byColor[BLACK]) * 3;
	res -= __popcnt64(byType[BISHOP] & byColor[BLACK]) * 3;
	res -= __popcnt64(byType[ROOK] & byColor[BLACK]) * 5;
	res -= __popcnt64(byType[QUEEN] & byColor[BLACK]) * 9;
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