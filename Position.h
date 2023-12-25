#pragma once

#include "Bitboards.h"

class Position {
private:
public:
	Piece board[SQUARE_CNT];
	Bitboard byType[TYPE_CNT];
	Bitboard byColor[COLOR_CNT];
	Bitboard enPassantTarget = 0;
	bool castlingK = 0, castlingQ = 0, castlingk = 0, castlingq = 0;

	Color sideToMove;

	void legal_moves(MoveList& moves);

	bool is_exist_legal_move();

	short eval();

	short material_eval();

	void set(string fen) {
		memset(board, 0, sizeof(board));
		memset(byType, 0, sizeof(byType));
		memset(byColor, 0, sizeof(byColor));
		castlingK = 0; castlingQ = 0; castlingk = 0; castlingq = 0;
		stringstream ss(fen);
		string piecePlacement, activeColor, castlingRights, enPassantTargetSquare, halfMoveClock, fullMoveNumber;
		ss >> piecePlacement;
		if (piecePlacement == "startpos") {
			stringstream ss1(StartFEN);
			ss1 >> piecePlacement >> activeColor >> castlingRights >> enPassantTargetSquare >> halfMoveClock >> fullMoveNumber;
		} else {
			ss >> activeColor >> castlingRights >> enPassantTargetSquare >> halfMoveClock >> fullMoveNumber;
		}
		Square pos = SQ_A8;
		for (char c : piecePlacement) {
			if (c == '/') {
				pos = Square(int(pos) - 16);
				continue;
			} else if (isdigit(c)) {
				pos = Square(int(pos) + c - '0');
			} else {
				Piece pc;
				if (c == 'p') pc = B_PAWN;
				if (c == 'n') pc = B_KNIGHT;
				if (c == 'b') pc = B_BISHOP;
				if (c == 'r') pc = B_ROOK;
				if (c == 'q') pc = B_QUEEN;
				if (c == 'k') pc = B_KING;
				if (c == 'P') pc = W_PAWN;
				if (c == 'N') pc = W_KNIGHT;
				if (c == 'B') pc = W_BISHOP;
				if (c == 'R') pc = W_ROOK;
				if (c == 'Q') pc = W_QUEEN;
				if (c == 'K') pc = W_KING;
				board[pos] = pc;
				byType[ALL_PIECES] |= square_bb(pos);
				byType[type_of(pc)] |= square_bb(pos);
				byColor[color_of(pc)] |= square_bb(pos);
				pos = Square(int(pos) + 1);
			}
		}
		sideToMove = activeColor == "w" ? WHITE : BLACK;
		for (auto i : castlingRights) {
			if (i == 'K') castlingK = 1;
			if (i == 'Q') castlingQ = 1;
			if (i == 'k') castlingk = 1;
			if (i == 'q') castlingq = 1;
		}
		enPassantTarget = enPassantTargetSquare == "-" ? 0 : square_bb(string_to_square(enPassantTargetSquare));
		string move;
		if (ss >> move) {
			while (ss >> move) {
				make_move(string_to_move(move));
			}
		}
	}

	string get_fen() {
		string fen = "";
		Square pos = SQ_A8;
		while (pos != SQUARE_CNT) {
			if (board[pos] != NO_PIECE) {
				fen += PieceNames[board[pos]];
			} else {
				if (fen.size() && isdigit(fen.back())) {
					fen.back()++;
				} else {
					fen += '1';
				}
			}
			pos = Square(int(pos) + 1);
			if (pos % 8 == 0) {
				if (pos == SQ_A2) {
					break;
				}
				fen += '/';
				pos = Square(int(pos) - 16);
			}
		}
		fen += ' ';
		fen += sideToMove == WHITE ? 'w' : 'b';
		fen += ' ';
		string castling = "";
		if (castlingK) castling += 'K';
		if (castlingQ) castling += 'Q';
		if (castlingk) castling += 'k';
		if (castlingq) castling += 'q';
		if (castling == "") castling = "-";
		fen += castling;
		fen += ' ';
		fen += '-';
		return fen;
	}

	void move_piece(Square from, Square to) {
		Piece from_pc = board[from];
		Piece to_pc = board[to];
		Bitboard from_bb = square_bb(from);
		Bitboard to_bb = square_bb(to);
		byType[ALL_PIECES] &= ~from_bb;
		byType[type_of(from_pc)] &= ~from_bb;
		byColor[color_of(from_pc)] &= ~from_bb;
		byType[ALL_PIECES] &= ~to_bb;
		byType[type_of(to_pc)] &= ~to_bb;
		byColor[color_of(to_pc)] &= ~to_bb;
		byType[ALL_PIECES] |= to_bb;
		byType[type_of(from_pc)] |= to_bb;
		byColor[color_of(from_pc)] |= to_bb;
		board[from] = NO_PIECE;
		board[to] = from_pc;
	}

	void make_move(Move move) {
		if (sideToMove == WHITE) {
			if (board[move.from] == W_KING) {
				castlingK = castlingQ = 0;
			}
			if (board[move.from] == W_ROOK) {
				if (move.from == SQ_A1) castlingQ = 0;
				if (move.from == SQ_H1) castlingK = 0;
			}
			if (board[move.to] == B_ROOK) {
				if (move.to == SQ_A8) castlingq = 0;
				if (move.to == SQ_H8) castlingk = 0;
			}
		} else {
			if (board[move.from] == B_KING) {
				castlingk = castlingq = 0;
			}
			if (board[move.from] == B_ROOK) {
				if (move.from == SQ_A8) castlingq = 0;
				if (move.from == SQ_H8) castlingk = 0;
			}
			if (board[move.to] == W_ROOK) {
				if (move.to == SQ_A1) castlingQ = 0;
				if (move.to == SQ_H1) castlingK = 0;
			}
		}
		move_piece(move.from, move.to);
		enPassantTarget = 0;
		sideToMove = sideToMove == WHITE ? BLACK : WHITE;
		if (move.info) {
			if (move.info == PawnPromotionId) {
				enPassantTarget = square_bb(sideToMove == WHITE ? shift_up(move.to) : shift_down(move.to));
			}
			if (move.info == EnPassantId) {
				Square opp_sq = sideToMove == BLACK ? shift_down(move.to) : shift_up(move.to);
				Bitboard opp = square_bb(opp_sq);
				byType[ALL_PIECES] ^= opp;
				byType[type_of(board[opp_sq])] ^= opp;
				byColor[color_of(board[opp_sq])] ^= opp;
				board[opp_sq] = NO_PIECE;
				return;
			}
			if (move.info < PIECE_CNT) {
				byType[type_of(board[move.to])] ^= square_bb(move.to);
				board[move.to] = Piece(move.info);
				byType[type_of(board[move.to])] ^= square_bb(move.to);
				return;
			}
			if (move.info == CastlingK) {
				byType[ALL_PIECES] ^= (square_bb(SQ_F1) | square_bb(SQ_H1));
				byType[ROOK] ^= (square_bb(SQ_H1) | square_bb(SQ_F1));
				byColor[WHITE] ^= (square_bb(SQ_F1) | square_bb(SQ_H1));
				board[SQ_H1] = NO_PIECE;
				board[SQ_F1] = W_ROOK;
				castlingK = castlingQ = 0;
			}
			if (move.info == CastlingQ) {
				byType[ALL_PIECES] ^= (square_bb(SQ_D1) | square_bb(SQ_A1));
				byType[ROOK] ^= (square_bb(SQ_A1) | square_bb(SQ_D1));
				byColor[WHITE] ^= (square_bb(SQ_D1) | square_bb(SQ_A1));
				board[SQ_A1] = NO_PIECE;
				board[SQ_D1] = W_ROOK;
				castlingK = castlingQ = 0;
			}
			if (move.info == Castlingk) {
				byType[ALL_PIECES] ^= (square_bb(SQ_F8) | square_bb(SQ_H8));
				byType[ROOK] ^= (square_bb(SQ_H8) | square_bb(SQ_F8));
				byColor[BLACK] ^= (square_bb(SQ_F8) | square_bb(SQ_H8));
				board[SQ_H8] = NO_PIECE;
				board[SQ_F8] = B_ROOK;
				castlingk = castlingq = 0;
			}
			if (move.info == Castlingq) {
				byType[ALL_PIECES] ^= (square_bb(SQ_D8) | square_bb(SQ_A8));
				byType[ROOK] ^= (square_bb(SQ_A8) | square_bb(SQ_D8));
				byColor[BLACK] ^= (square_bb(SQ_D8) | square_bb(SQ_A8));
				board[SQ_A8] = NO_PIECE;
				board[SQ_D8] = B_ROOK;
				castlingk = castlingq = 0;
			}
		}
	}

	inline bool is_attacked_square(Square s) {
		return (AttackersPawn[!sideToMove][s] & byType[PAWN] & byColor[!sideToMove]) ||
			(PseudoAttacks[KNIGHT][s] & byType[KNIGHT] & byColor[!sideToMove]) ||
			(PseudoAttacks[KING][s] & byType[KING] & byColor[!sideToMove]) ||
			(pseudolegal_bishop(byType[ALL_PIECES], s) & (byType[BISHOP] | byType[QUEEN]) & byColor[!sideToMove]) ||
			(pseudolegal_rook(byType[ALL_PIECES], s) & (byType[ROOK] | byType[QUEEN]) & byColor[!sideToMove]);
	}

	inline bool is_legal(Move move) {
		Bitboard BCNSTM = byColor[!sideToMove];
		Bitboard BTAP = byType[ALL_PIECES];
		byColor[another_color(sideToMove)] &= ~square_bb(move.to);
		byType[ALL_PIECES] &= ~square_bb(move.from);
		byType[ALL_PIECES] |= square_bb(move.to);
		bool ans = !is_attacked_square(type_of(board[move.from]) != KING ? get_square(byType[KING] & byColor[sideToMove]) : move.to);
		byColor[!sideToMove] = BCNSTM;
		byType[ALL_PIECES] = BTAP;
		return ans;
	}


	inline bool is_legal_enPassant(Move move) {
		Square target = sideToMove == BLACK ? shift_up(move.to) : shift_down(move.to);
		Bitboard BCNSTM = byColor[!sideToMove];
		Bitboard BTAP = byType[ALL_PIECES];
		byColor[another_color(sideToMove)] &= ~square_bb(target);
		byType[ALL_PIECES] &= ~square_bb(target);
		byType[ALL_PIECES] &= ~square_bb(move.from);
		byType[ALL_PIECES] |= square_bb(move.to);
		bool ans = !is_attacked_square(type_of(board[move.from]) != KING ? get_square(byType[KING] & byColor[sideToMove]) : move.to);
		byColor[!sideToMove] = BCNSTM;
		byType[ALL_PIECES] = BTAP;
		return ans;
	}
};


void print(Position pos) {
	for (Square s = SQ_A8; s >= 0; s = Square(int(s) + 1 - 16 * ((s + 1) % 8 == 0))) {
		cout << PieceNames[pos.board[s]] << ' ';
		if (s % 8 == 7) cout << '\n';
	}
	cout << '\n';
}