#pragma once

#include "Position.h"

void legal_moves(Position& pos, MoveList& moves) {
	Bitboard mask_my = pos.byColor[pos.sideToMove];
	Bitboard mask_opp = pos.byColor[!int(pos.sideToMove)];
	Bitboard mask_free = ~(mask_my | mask_opp);
	// pawn moves
	Bitboard pawns = mask_my & pos.byType[PAWN];
	while (pawns) {
		Square from = get_square(pawns);
		Bitboard to = PawnAttacks[pos.sideToMove][from] & mask_opp;
		to |= (pos.sideToMove == WHITE ? shift_up(square_bb(from)) : shift_down(square_bb(from))) & mask_free;
		to |= (pos.sideToMove == WHITE ? shift_up(shift_up(square_bb(from) & MaskPawns)) : shift_down(shift_down(square_bb(from) & MaskPawns))) & mask_free;
		while (to) {
			Square to_sq = get_square(to);
			if (pos.is_legal(Move(from, to_sq))) {
				moves.add(Move(from, to_sq));
			}
			to ^= square_bb(to_sq);
		}
		if (PawnAttacks[pos.sideToMove][from] & pos.enPassantTarget) {
			moves.add(Move(from, get_square(pos.enPassantTarget), 1));
		}
		pawns ^= square_bb(from);
	}
	// knight moves
	Bitboard knights = mask_my & pos.byType[KNIGHT];
	while (knights) {
		Square from = get_square(knights);
		Bitboard to = PseudoAttacks[KNIGHT][from] & ~mask_my;
		while (to) {
			Square to_sq = get_square(to);
			if (pos.is_legal(Move(from, to_sq))) {
				moves.add(Move(from, to_sq));
			}
			to ^= square_bb(to_sq);
		}
		knights ^= square_bb(from);
	}
// bishop moves
	Bitboard bishops = mask_my & pos.byType[BISHOP];
	while (bishops) {
		Square from = get_square(bishops);
		Bitboard to = pseudolegal_bishop(pos.byType[ALL_PIECES], from) & ~mask_my;
		while (to) {
			Square to_sq = get_square(to);
			if (pos.is_legal(Move(from, to_sq))) {
				moves.add(Move(from, to_sq));
			}
			to ^= square_bb(to_sq);
		}
		bishops ^= square_bb(from);
	}
// rook moves
Bitboard rooks = mask_my & pos.byType[ROOK];
	while (rooks) {
		Square from = get_square(rooks);
		Bitboard to = pseudolegal_rook(pos.byType[ALL_PIECES], from) & ~mask_my;
		while (to) {
			Square to_sq = get_square(to);
			if (pos.is_legal(Move(from, to_sq))) {
				moves.add(Move(from, to_sq));
			}
			to ^= square_bb(to_sq);
		}
		rooks ^= square_bb(from);
	}
// queen moves
	Bitboard queens = mask_my & pos.byType[QUEEN];
while (queens) {
		Square from = get_square(queens);
		Bitboard to = (pseudolegal_bishop(pos.byType[ALL_PIECES], from) | pseudolegal_rook(pos.byType[ALL_PIECES], from))  & ~mask_my;
		while (to) {
			Square to_sq = get_square(to);
			if (pos.is_legal(Move(from, to_sq))) {
				moves.add(Move(from, to_sq));
			}
			to ^= square_bb(to_sq);
		}
		queens ^= square_bb(from);
	}
// king moves
	Bitboard king = mask_my & pos.byType[KING];
	while (king) {
		Square from = get_square(king);
		Bitboard to = PseudoAttacks[KING][from] & ~mask_my;
		while (to) {
			Square to_sq = get_square(to);
			if (pos.is_legal(Move(from, to_sq))) {
				moves.add(Move(from, to_sq));
			}
			to ^= square_bb(to_sq);
		}
		king ^= square_bb(from);
	}
}