#pragma once

#include "Bitboards.h"

class Position {
private:
public:
	Piece board[SQUARE_CNT];
	Bitboard byType[TYPE_CNT];
	Bitboard byColor[COLOR_CNT];
	Bitboard enPassantTarget = 0;
	Color sideToMove;

	void set(string fen) {
		memset(board, 0, sizeof(board));
		memset(byType, 0, sizeof(byType));
		memset(byColor, 0, sizeof(byColor));
		stringstream ss(fen);
		string piecePlacement, activeColor, castlingRights, enPassantTargetSquare, halfMoveClock, fullMoveNumber;
		ss >> piecePlacement >> activeColor >> castlingRights >> enPassantTargetSquare >> halfMoveClock >> fullMoveNumber;
		Square pos = SQ_A8;
		for (char c : piecePlacement) {
			if (c == '/') {
				pos = Square(int(pos) - 16);
				continue;
			}
			else if (isdigit(c)) {
				pos = Square(int(pos) + c - '0');
			}
			else {
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
		enPassantTarget = enPassantTargetSquare == "-" ? 0 :  square_bb(string_to_square(enPassantTargetSquare));
	}

	string get_fen() {
		string fen = "";
		Square pos = SQ_A8;
		while (pos != SQUARE_CNT) {
			if (pos % 8 == 0 && pos != SQ_A8) {
				fen += '/';
			}
			if (board[pos] != NO_PIECE) {
				fen += PieceNames[board[pos]];
			} else {
				if (isdigit(fen.back())) {
					fen.back()++;
				} else {
					fen += '1';
				}
			}
			pos = Square(int(pos) + 1);
		}
		fen += ' ';
		fen += sideToMove == WHITE ? 'w' : 'b';
		fen += ' ';
		fen += 'KQkq';
		fen += ' ';
		fen += '-';
		return fen;
	}

	inline void move_piece(Square from, Square to) {
		Piece pc = board[from];
		Bitboard fromTo = from | to;
		byType[ALL_PIECES] ^= fromTo;
		byType[type_of(pc)] ^= fromTo;
		byColor[color_of(pc)] ^= fromTo;
		board[from] = NO_PIECE;
		board[to] = pc;
	}

	inline void make_move(Move move) {
		move_piece(move.from, move.to);
		sideToMove = Color(!sideToMove);
	}

	bool is_legal(Move move) {
		return 1;
	}

};


void print(Position pos) {
	print(pos.byType[ALL_PIECES]);
}