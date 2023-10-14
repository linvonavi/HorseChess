#pragma once

#include "Moves.h"

Bitboard PseudoAttacks[TYPE_CNT][SQUARE_CNT];
Bitboard PawnAttacks[COLOR_CNT][SQUARE_CNT];
Bitboard AttackersPawn[COLOR_CNT][SQUARE_CNT];
const int MaxRookBlockersCnt = 12;
const int MaxRookMagicSize = 13;
const int MagicRookShift[SQUARE_CNT] = {
	51, 52, 52, 52, 52, 52, 52, 51,
	52, 53, 53, 53, 53, 53, 53, 52,
	52, 53, 53, 53, 53, 53, 53, 52,
	52, 53, 53, 53, 53, 53, 53, 52,
	52, 53, 53, 53, 53, 53, 53, 52,
	52, 53, 53, 53, 53, 53, 53, 52,
	52, 53, 53, 53, 53, 53, 53, 52,
	51, 52, 52, 52, 52, 52, 52, 51
};
const int MaxBishopBlockersCnt = 10;
const int MaxBishopMagicSize = 10;
const int MagicBishopShift = 54;

Bitboard MagicRook[SQUARE_CNT][1 << MaxRookMagicSize];
uint64_t MagicRookNumbers[SQUARE_CNT] = { 6989571778334562553, 12278612541885518309, 15769845845498096959, 5270045060815576157, 12682294085293009849, 17437953358801136128, 18190492139323589034, 4192557884958109069, 1139388734268881089, 15737266025260985388, 11097733302192489233, 12325342361124402071, 11338889459435752404, 15398601878792878478, 13986284014408640616, 868631804116551028, 15436275126459135808, 6955781222515119606, 7087400616587563988, 16364837921519820051, 5266388957837974313, 13578166303025680257, 683630930970509776, 12979477480338256132, 10630076541716926230, 87049508795548227, 13537871840970625618, 16301107273116329452, 14711342748678871480, 13121457455156297482, 6817137494902412647, 4126819315624357057, 15255083412449390006, 2607832365776351571, 8401090495822515157, 16286670944655634801, 10877027521666619708, 16274931096294172917, 2151856734165295717, 15785514385025123290, 9332951596577794072, 15323361669857248896, 12955161860172151742, 18198854781074276364, 16213749747756884193, 7251955673667731279, 8676005606674071581, 2310833248942531742, 7960810820459438707, 18122706916684489656, 14427447245910605477, 4993134309641536000, 15384070257779237923, 12679297981775611152, 17243886467032846008, 5661211745398349744, 3180078109447419394, 10966091829252263990, 2007339868060606834, 9023225277379158230, 9269639723884750858, 12563505914143704838, 9809400351453112068, 3988784719170278326, };

Bitboard MagicBishop[SQUARE_CNT][1 << MaxBishopMagicSize];
uint64_t MagicBishopNumbers[SQUARE_CNT] = { 2947667278772165694, 18301848765998365067, 729919693006235833, 11021831128136023278, 1054412044467431918, 11649642299870863663, 7813497161378842344, 15536964167022953318, 7805705913528825107, 12092317580524320504, 17163424360305231502, 6352792256529822470, 4696818759170745400, 8202730408965517889, 14576421520683731187, 12828242264541034313, 10287390044869019765, 5302155820127968924, 10414102509512663394, 2770061463645390892, 367716015581738287, 12679040793862387242, 7160159996042967007, 17133129490705945961, 3316611681754028984, 3503182316394155159, 8783759536627765131, 7729866095179941376, 2906445659263621266, 16376648620835362359, 18066205610966908521, 10251757806493613677, 11599188916974793261, 4189608821147523558, 2577948483864307670, 1729100296606974407, 8039298565172771037, 61381314107426918, 7963572367917473857, 4551779573414506789, 8333058894971991481, 18010052344806911799, 9701313016554700209, 8645592252479703413, 5659164746574131732, 12626686229290098543, 13401947267987623155, 14665178712590308582, 10695918099189454517, 12351404115050108134, 14229965881012268356, 3708239919564090150, 538147598524654096, 17044714727159279230, 7676214018100180496, 1013400549632481588, 3752107411498640254, 10716927202330031368, 4196181082363938335, 16869858767904939440, 17870067265593080748, 13789005346174078469, 14019535255421959407, 2150539971632875369, };

const Bitboard MaskLeft = 0x0101010101010101;
const Bitboard MaskRight = 0x8080808080808080;
const Bitboard MaskUp = 0xff00000000000000;
const Bitboard MaskDown = 0x00000000000000ff;
const Bitboard MaskPawns = 0x00ff00000000ff00;

const Bitboard PawnPromotionMask[COLOR_CNT] = { MaskUp, MaskDown };

const Bitboard MaskCenter = 0x007e7e7e7e7e7e00;

inline Bitboard shift_up(Bitboard bb) {
	return bb << 8;
}

inline Square shift_up(Square sq) {
	return Square(int(sq) + 8);
}

inline Bitboard shift_down(Bitboard bb) {
	return bb >> 8;
}

inline Square shift_down(Square sq) {
	return Square(int(sq) - 8);
}

inline Bitboard shift_left(Bitboard bb) {
	return (bb & ~MaskLeft) >> 1;
}

inline Square shift_left(Square sq) {
	return Square(int(sq) - 1);
}

inline Bitboard shift_right(Bitboard bb) {
	return (bb & ~MaskRight) << 1;
}

inline Square shift_right(Square sq) {
	return Square(int(sq) + 1);
}

Bitboard rook_attacks(Square s, Bitboard blockers) {
	Bitboard attacks = 0;
	Bitboard bb = square_bb(s);
	while (bb = shift_left(bb)) {
		attacks |= bb;
		if (bb & blockers) {
			break;
		}
	}
	bb = square_bb(s);
	while (bb = shift_right(bb)) {
		attacks |= bb;
		if (bb & blockers) {
			break;
		}
	}
	bb = square_bb(s);
	while (bb = shift_down(bb)) {
		attacks |= bb;
		if (bb & blockers) {
			break;
		}
	}
	bb = square_bb(s);
	while (bb = shift_up(bb)) {
		attacks |= bb;
		if (bb & blockers) {
			break;
		}
	}
	return attacks;
}

Bitboard bishop_attacks(Square s, Bitboard blockers) {
	Bitboard attacks = 0;
	Bitboard bb = square_bb(s);
	while (bb = shift_up(shift_left(bb))) {
		attacks |= bb;
		if (bb & blockers) {
			break;
		}
	}
	bb = square_bb(s);
	while (bb = shift_up(shift_right(bb))) {
		attacks |= bb;
		if (bb & blockers) {
			break;
		}
	}
	bb = square_bb(s);
	while (bb = shift_down(shift_left(bb))) {
		attacks |= bb;
		if (bb & blockers) {
			break;
		}
	}
	bb = square_bb(s);
	while (bb = shift_down(shift_right(bb))) {
		attacks |= bb;
		if (bb & blockers) {
			break;
		}
	}
	return attacks;
}

Bitboard pseudolegal_rook(Bitboard board, Square s) {
	return MagicRook[s][((board & PseudoAttacks[ROOK][s]) * MagicRookNumbers[s]) >> MagicRookShift[s]];
}

Bitboard pseudolegal_bishop(Bitboard board, Square s) {
	return MagicBishop[s][((board & PseudoAttacks[BISHOP][s]) * MagicBishopNumbers[s]) >> MagicBishopShift];
}

void init_bitboards() {
	// init pawns
	for (Square s = SQ_A1; s < SQUARE_CNT; s = Square(int(s) + 1)) {
		PawnAttacks[WHITE][s] = shift_left(shift_up(square_bb(s))) | shift_right(shift_up(square_bb(s)));
		PawnAttacks[BLACK][s] = shift_left(shift_down(square_bb(s))) | shift_right(shift_down(square_bb(s)));
		AttackersPawn[BLACK][s] = shift_left(shift_up(square_bb(s))) | shift_right(shift_up(square_bb(s)));
		AttackersPawn[WHITE][s] = shift_left(shift_down(square_bb(s))) | shift_right(shift_down(square_bb(s)));
	}
	// init knights
	for (Square s = SQ_A1; s < SQUARE_CNT; s = Square(int(s) + 1)) {
		PseudoAttacks[KNIGHT][s] |= shift_left(shift_up(shift_up(square_bb(s))));
		PseudoAttacks[KNIGHT][s] |= shift_right(shift_up(shift_up(square_bb(s))));
		PseudoAttacks[KNIGHT][s] |= shift_left(shift_down(shift_down(square_bb(s))));
		PseudoAttacks[KNIGHT][s] |= shift_right(shift_down(shift_down(square_bb(s))));
		PseudoAttacks[KNIGHT][s] |= shift_up(shift_left(shift_left(square_bb(s))));
		PseudoAttacks[KNIGHT][s] |= shift_down(shift_left(shift_left(square_bb(s))));
		PseudoAttacks[KNIGHT][s] |= shift_up(shift_right(shift_right(square_bb(s))));
		PseudoAttacks[KNIGHT][s] |= shift_down(shift_right(shift_right(square_bb(s))));
	}
	// init bishops
	for (Square s = SQ_A1; s < SQUARE_CNT; s = Square(int(s) + 1)) {
		Bitboard bb = square_bb(s);
		Square blockers[MaxBishopBlockersCnt];
		int cnt = 0;
		Square s1 = s;
		while (bb = shift_up(shift_left(bb))) {
			if (bb & ~MaskLeft & ~MaskUp) {
				PseudoAttacks[BISHOP][s] |= bb;
				s1 = shift_up(shift_left(s1));
				blockers[cnt++] = s1;
			}
		}
		s1 = s;
		bb = square_bb(s);
		while (bb = shift_up(shift_right(bb))) {
			if (bb & ~MaskRight & ~MaskUp) {
				PseudoAttacks[BISHOP][s] |= bb;
				s1 = shift_up(shift_right(s1));
				blockers[cnt++] = s1;
			}
		}
		s1 = s;
		bb = square_bb(s);
		while (bb = shift_down(shift_left(bb))) {
			if (bb & ~MaskLeft & ~MaskDown) {
				PseudoAttacks[BISHOP][s] |= bb;
				s1 = shift_down(shift_left(s1));
				blockers[cnt++] = s1;
			}
		}
		s1 = s;
		bb = square_bb(s);
		while (bb = shift_down(shift_right(bb))) {
			if (bb & ~MaskRight & ~MaskDown) {
				PseudoAttacks[BISHOP][s] |= bb;
				s1 = shift_down(shift_right(s1));
				blockers[cnt++] = s1;
			}
		}
		Bitboard blockers_boards[1 << MaxBishopBlockersCnt];
		int blockers_boards_cnt = 0;
		for (int mask = 0; mask < (1 << cnt); mask++) {
			Bitboard blockers_board = 0;
			for (int i = 0; i < cnt; i++) {
				if (mask & (1 << i)) {
					blockers_board |= square_bb(blockers[i]);
				}
			}
			blockers_boards[blockers_boards_cnt++] = blockers_board;
		}
		Bitboard magic_number = MagicBishopNumbers[s];
		while (true) {
			bool used[1 << MaxBishopMagicSize];
			memset(used, 0, sizeof(used));
			bool ok = true;
			for (int i = 0; i < blockers_boards_cnt; i++) {
				int idx = (blockers_boards[i] * magic_number) >> MagicBishopShift;
				if (used[idx]) {
					ok = false;
					break;
				}
				used[idx] = true;
			}
			if (ok) {
				for (int i = 0; i < blockers_boards_cnt; i++) {
					int idx = (blockers_boards[i] * magic_number) >> MagicBishopShift;
					MagicBishop[s][idx] = bishop_attacks(s, blockers_boards[i]);
				}
				MagicBishopNumbers[s] = magic_number;
				break;
			}
			magic_number = rnd64();
		}
	}
	// init rooks
	for (Square s = SQ_A1; s < SQUARE_CNT; s = Square(int(s) + 1)) {
		Bitboard bb = square_bb(s);
		Square blockers[MaxRookBlockersCnt];
		int cnt = 0;
		Square s1 = s;
		while (bb = shift_left(bb)) {
			if (bb & ~MaskLeft) {
				PseudoAttacks[ROOK][s] |= bb;
				s1 = shift_left(s1);
				blockers[cnt++] = s1;
			}
		}
		s1 = s;
		bb = square_bb(s);
		while (bb = shift_right(bb)) {
			if (bb & ~MaskRight) {
				PseudoAttacks[ROOK][s] |= bb;
				s1 = shift_right(s1);
				blockers[cnt++] = s1;
			}
		}
		s1 = s;
		bb = square_bb(s);
		while (bb = shift_down(bb)) {
			if (bb & ~MaskDown) {
				PseudoAttacks[ROOK][s] |= bb;
				s1 = shift_down(s1);
				blockers[cnt++] = s1;
			}
		}
		s1 = s;
		bb = square_bb(s);
		while (bb = shift_up(bb)) {
			if (bb & ~MaskUp) {
				PseudoAttacks[ROOK][s] |= bb;
				s1 = shift_up(s1);
				blockers[cnt++] = s1;
			}
		}
		Bitboard blockers_boards[1 << MaxRookBlockersCnt];
		int blockers_boards_cnt = 0;
		for (int mask = 0; mask < (1 << cnt); mask++) {
			Bitboard blockers_board = 0;
			for (int i = 0; i < cnt; i++) {
				if (mask & (1 << i)) {
					blockers_board |= square_bb(blockers[i]);
				}
			}
			blockers_boards[blockers_boards_cnt++] = blockers_board;
		}
		Bitboard magic_number = MagicRookNumbers[s];
		while (true) {
			bool used[1 << MaxRookMagicSize];
			memset(used, 0, sizeof(used));
			bool ok = true;
			for (int i = 0; i < blockers_boards_cnt; i++) {
				int idx = (blockers_boards[i] * magic_number) >> MagicRookShift[s];
				if (used[idx]) {
					ok = false;
					break;
				}
				used[idx] = true;
			}
			if (ok) {
				for (int i = 0; i < blockers_boards_cnt; i++) {
					int idx = (blockers_boards[i] * magic_number) >> MagicRookShift[s];
					MagicRook[s][idx] = rook_attacks(s, blockers_boards[i]);
				}
				MagicRookNumbers[s] = magic_number;
				break;
			}
			magic_number = rnd64();
		}
	}
	// init queens
	for (Square s = SQ_A1; s < SQUARE_CNT; s = Square(int(s) + 1)) {
		PseudoAttacks[QUEEN][s] = PseudoAttacks[BISHOP][s] | PseudoAttacks[ROOK][s];
	}
	// init kings
	for (Square s = SQ_A1; s < SQUARE_CNT; s = Square(int(s) + 1)) {
		PseudoAttacks[KING][s] |= shift_left(shift_up(square_bb(s)));
		PseudoAttacks[KING][s] |= shift_up(square_bb(s));
		PseudoAttacks[KING][s] |= shift_right(shift_up(square_bb(s)));
		PseudoAttacks[KING][s] |= shift_right(square_bb(s));
		PseudoAttacks[KING][s] |= shift_right(shift_down(square_bb(s)));
		PseudoAttacks[KING][s] |= shift_down(square_bb(s));
		PseudoAttacks[KING][s] |= shift_left(shift_down(square_bb(s)));
		PseudoAttacks[KING][s] |= shift_left(square_bb(s));
	}
}