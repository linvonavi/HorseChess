#pragma once

#include "Position.h"


void Position::legal_moves(MoveList& moves) {
	Bitboard mask_my = byColor[sideToMove];
	Bitboard mask_opp = byColor[!int(sideToMove)];
	Bitboard mask_free = ~(mask_my | mask_opp);
	// pawn moves
	Bitboard pawns = mask_my & byType[PAWN];
	while (pawns) {
		Square from = get_square(pawns);
		Bitboard to = PawnAttacks[sideToMove][from] & mask_opp;
		to |= (sideToMove == WHITE ? shift_up(square_bb(from)) : shift_down(square_bb(from))) & mask_free;
		while (to) {
			Square to_sq = get_square(to);
			if (is_legal(Move(from, to_sq))) {
				// promotion
				if (square_bb(to_sq) & PawnPromotionMask[sideToMove]) {
					moves.add(Move(from, to_sq, make_piece(QUEEN, sideToMove)));
					moves.add(Move(from, to_sq, make_piece(ROOK, sideToMove)));
					moves.add(Move(from, to_sq, make_piece(BISHOP, sideToMove)));
					moves.add(Move(from, to_sq, make_piece(KNIGHT, sideToMove)));
				} else {
					moves.add(Move(from, to_sq));
				}
			}
			to ^= square_bb(to_sq);
		}
		// double pawn push
		to |= (sideToMove == WHITE ? shift_up(shift_up(square_bb(from) & MaskPawns) & mask_free) : shift_down(shift_down(square_bb(from) & MaskPawns) & mask_free)) & mask_free;
		if (to && is_legal(Move(from, get_square(to)))) {
			moves.add(Move(from, get_square(to), PawnPromotionId));
		}
		// en passant
		if (PawnAttacks[sideToMove][from] & enPassantTarget && is_legal_enPassant(Move(from, get_square(enPassantTarget)))) {
			moves.add(Move(from, get_square(enPassantTarget), EnPassantId));
		}
		pawns ^= square_bb(from);
	}
	// knight moves
	Bitboard knights = mask_my & byType[KNIGHT];
	while (knights) {
		Square from = get_square(knights);
		Bitboard to = PseudoAttacks[KNIGHT][from] & ~mask_my;
		while (to) {
			Square to_sq = get_square(to);
			if (is_legal(Move(from, to_sq))) {
				moves.add(Move(from, to_sq));
			}
			to ^= square_bb(to_sq);
		}
		knights ^= square_bb(from);
	}
	// bishop moves
	Bitboard bishops = mask_my & (byType[BISHOP] | byType[QUEEN]);
	while (bishops) {
		Square from = get_square(bishops);
		Bitboard to = pseudolegal_bishop(byType[ALL_PIECES], from) & ~mask_my;
		while (to) {
			Square to_sq = get_square(to);
			if (is_legal(Move(from, to_sq))) {
				moves.add(Move(from, to_sq));
			}
			to ^= square_bb(to_sq);
		}
		bishops ^= square_bb(from);
	}
	// rook moves
	Bitboard rooks = mask_my & (byType[ROOK] | byType[QUEEN]);
	while (rooks) {
		Square from = get_square(rooks);
		Bitboard to = pseudolegal_rook(byType[ALL_PIECES], from) & ~mask_my;
		while (to) {
			Square to_sq = get_square(to);
			if (is_legal(Move(from, to_sq))) {
				moves.add(Move(from, to_sq));
			}
			to ^= square_bb(to_sq);
		}
		rooks ^= square_bb(from);
	}
	// king moves
	Bitboard king = mask_my & byType[KING];
	while (king) {
		Square from = get_square(king);
		Bitboard to = PseudoAttacks[KING][from] & ~mask_my;
		while (to) {
			Square to_sq = get_square(to);
			if (is_legal(Move(from, to_sq))) {
				moves.add(Move(from, to_sq));
			}
			to ^= square_bb(to_sq);
		}
		king ^= square_bb(from);
	}
	// castling
	if (sideToMove == WHITE) {
		if (castlingK) {
			if (!is_attacked_square(SQ_E1) && !is_attacked_square(SQ_F1) && !is_attacked_square(SQ_G1) && board[SQ_F1] == NO_PIECE && board[SQ_G1] == NO_PIECE) {
				moves.add(Move(SQ_E1, SQ_G1, CastlingK));
			}
		}
		if (castlingQ) {
			if (!is_attacked_square(SQ_E1) && !is_attacked_square(SQ_D1) && !is_attacked_square(SQ_C1) && board[SQ_B1] == NO_PIECE && board[SQ_C1] == NO_PIECE && board[SQ_D1] == NO_PIECE) {
				moves.add(Move(SQ_E1, SQ_C1, CastlingQ));
			}
		}
	} else {
		if (castlingk) {
			if (!is_attacked_square(SQ_E8) && !is_attacked_square(SQ_F8) && !is_attacked_square(SQ_G8) && board[SQ_F8] == NO_PIECE && board[SQ_G8] == NO_PIECE) {
				moves.add(Move(SQ_E8, SQ_G8, Castlingk));
			}
		}
		if (castlingq) {
			if (!is_attacked_square(SQ_E8) && !is_attacked_square(SQ_D8) && !is_attacked_square(SQ_C8) && board[SQ_B8] == NO_PIECE && board[SQ_C8] == NO_PIECE && board[SQ_D8] == NO_PIECE) {
				moves.add(Move(SQ_E8, SQ_C8, Castlingq));
			}
		}
	}
}

bool Position::is_exist_legal_move() {
	Bitboard mask_my = byColor[sideToMove];
	Bitboard mask_opp = byColor[!int(sideToMove)];
	Bitboard mask_free = ~(mask_my | mask_opp);
	// pawn moves
	Bitboard pawns = mask_my & byType[PAWN];
	while (pawns) {
		Square from = get_square(pawns);
		Bitboard to = PawnAttacks[sideToMove][from] & mask_opp;
		to |= (sideToMove == WHITE ? shift_up(square_bb(from)) : shift_down(square_bb(from))) & mask_free;
		while (to) {
			Square to_sq = get_square(to);
			if (is_legal(Move(from, to_sq))) {
				// promotion
				return 1;
			}
			to ^= square_bb(to_sq);
		}
		// double pawn push
		to |= (sideToMove == WHITE ? shift_up(shift_up(square_bb(from) & MaskPawns) & mask_free) : shift_down(shift_down(square_bb(from) & MaskPawns) & mask_free)) & mask_free;
		if (to && is_legal(Move(from, get_square(to)))) {
			return 1;
		}
		// en passant
		if (PawnAttacks[sideToMove][from] & enPassantTarget && is_legal_enPassant(Move(from, get_square(enPassantTarget)))) {
			return 1;
		}
		pawns ^= square_bb(from);
	}
	// knight moves
	Bitboard knights = mask_my & byType[KNIGHT];
	while (knights) {
		Square from = get_square(knights);
		Bitboard to = PseudoAttacks[KNIGHT][from] & ~mask_my;
		while (to) {
			Square to_sq = get_square(to);
			if (is_legal(Move(from, to_sq))) {
				return 1;
			}
			to ^= square_bb(to_sq);
		}
		knights ^= square_bb(from);
	}
	// bishop moves
	Bitboard bishops = mask_my & (byType[BISHOP] | byType[QUEEN]);
	while (bishops) {
		Square from = get_square(bishops);
		Bitboard to = pseudolegal_bishop(byType[ALL_PIECES], from) & ~mask_my;
		while (to) {
			Square to_sq = get_square(to);
			if (is_legal(Move(from, to_sq))) {
				return 1;
			}
			to ^= square_bb(to_sq);
		}
		bishops ^= square_bb(from);
	}
	// rook moves
	Bitboard rooks = mask_my & (byType[ROOK] | byType[QUEEN]);
	while (rooks) {
		Square from = get_square(rooks);
		Bitboard to = pseudolegal_rook(byType[ALL_PIECES], from) & ~mask_my;
		while (to) {
			Square to_sq = get_square(to);
			if (is_legal(Move(from, to_sq))) {
				return 1;
			}
			to ^= square_bb(to_sq);
		}
		rooks ^= square_bb(from);
	}
	// king moves
	Bitboard king = mask_my & byType[KING];
	while (king) {
		Square from = get_square(king);
		Bitboard to = PseudoAttacks[KING][from] & ~mask_my;
		while (to) {
			Square to_sq = get_square(to);
			if (is_legal(Move(from, to_sq))) {
				return 1;
			}
			to ^= square_bb(to_sq);
		}
		king ^= square_bb(from);
	}
	// castling
	if (sideToMove == WHITE) {
		if (castlingK) {
			if (!is_attacked_square(SQ_E1) && !is_attacked_square(SQ_F1) && !is_attacked_square(SQ_G1) && board[SQ_F1] == NO_PIECE && board[SQ_G1] == NO_PIECE) {
				return 1;
			}
		}
		if (castlingQ) {
			if (!is_attacked_square(SQ_E1) && !is_attacked_square(SQ_D1) && !is_attacked_square(SQ_C1) && board[SQ_B1] == NO_PIECE && board[SQ_C1] == NO_PIECE && board[SQ_D1] == NO_PIECE) {
				return 1;
			}
		}
	} else {
		if (castlingk) {
			if (!is_attacked_square(SQ_E8) && !is_attacked_square(SQ_F8) && !is_attacked_square(SQ_G8) && board[SQ_F8] == NO_PIECE && board[SQ_G8] == NO_PIECE) {
				return 1;
			}
		}
		if (castlingq) {
			if (!is_attacked_square(SQ_E8) && !is_attacked_square(SQ_D8) && !is_attacked_square(SQ_C8) && board[SQ_B8] == NO_PIECE && board[SQ_C8] == NO_PIECE && board[SQ_D8] == NO_PIECE) {
				return 1;
			}
		}
	}
	return 0;
}
