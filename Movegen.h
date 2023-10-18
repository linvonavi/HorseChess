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
		while (to) {
			Square to_sq = get_square(to);
			if (pos.is_legal(Move(from, to_sq))) {
				// promotion
				if (square_bb(to_sq) & PawnPromotionMask[pos.sideToMove]) {
					moves.add(Move(from, to_sq, make_piece(QUEEN, pos.sideToMove)));
					moves.add(Move(from, to_sq, make_piece(ROOK, pos.sideToMove)));
					moves.add(Move(from, to_sq, make_piece(BISHOP, pos.sideToMove)));
					moves.add(Move(from, to_sq, make_piece(KNIGHT, pos.sideToMove)));
				} else {
					moves.add(Move(from, to_sq));
				}
			}
			to ^= square_bb(to_sq);
		}
		// double pawn push
		to |= (pos.sideToMove == WHITE ? shift_up(shift_up(square_bb(from) & MaskPawns) & mask_free) : shift_down(shift_down(square_bb(from) & MaskPawns) & mask_free)) & mask_free;
		if (to && pos.is_legal(Move(from, get_square(to)))) {
			moves.add(Move(from, get_square(to), PawnPromotionId));
		}
		// en passant
		if (PawnAttacks[pos.sideToMove][from] & pos.enPassantTarget && pos.is_legal_enPassant(Move(from, get_square(pos.enPassantTarget)))) {
			moves.add(Move(from, get_square(pos.enPassantTarget), EnPassantId));
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
		Bitboard to = (pseudolegal_bishop(pos.byType[ALL_PIECES], from) | pseudolegal_rook(pos.byType[ALL_PIECES], from)) & ~mask_my;
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
	// castling
	if (pos.sideToMove == WHITE) {
		if (pos.castlingK) {
			if (!pos.is_attacked_square(SQ_E1) && !pos.is_attacked_square(SQ_F1) && !pos.is_attacked_square(SQ_G1) && pos.board[SQ_F1] == NO_PIECE && pos.board[SQ_G1] == NO_PIECE) {
				moves.add(Move(SQ_E1, SQ_G1, CastlingK));
			}
		}
		if (pos.castlingQ) {
			if (!pos.is_attacked_square(SQ_E1) && !pos.is_attacked_square(SQ_D1) && !pos.is_attacked_square(SQ_C1) && pos.board[SQ_B1] == NO_PIECE && pos.board[SQ_C1] == NO_PIECE && pos.board[SQ_D1] == NO_PIECE) {
				moves.add(Move(SQ_E1, SQ_C1, CastlingQ));
			}
		}
	} else {
		if (pos.castlingk) {
			if (!pos.is_attacked_square(SQ_E8) && !pos.is_attacked_square(SQ_F8) && !pos.is_attacked_square(SQ_G8) && pos.board[SQ_F8] == NO_PIECE && pos.board[SQ_G8] == NO_PIECE) {
				moves.add(Move(SQ_E8, SQ_G8, Castlingk));
			}
		}
		if (pos.castlingq) {
			if (!pos.is_attacked_square(SQ_E8) && !pos.is_attacked_square(SQ_D8) && !pos.is_attacked_square(SQ_C8) && pos.board[SQ_B8] == NO_PIECE && pos.board[SQ_C8] == NO_PIECE && pos.board[SQ_D8] == NO_PIECE) {
				moves.add(Move(SQ_E8, SQ_C8, Castlingq));
			}
		}
	}
}