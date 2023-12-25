#pragma once

#include "Movegen.h"

struct SearchConfig {
	int Movetime = 10000;
	int Depth;
	int StartTime;
	int VisitedNodes = 0;
	MoveList Stack;
};

inline pair<char, char> Position::move_eval(Move move) {
	char res = 0, res1 = 0;
	res += bool(byType[PAWN] & square_bb(move.to));
	res += 2 * bool(byType[KNIGHT] & square_bb(move.to));
	res += 3 * bool(byType[BISHOP] & square_bb(move.to));
	res += 4 * bool(byType[ROOK] & square_bb(move.to));
	res += 5 * bool(byType[QUEEN] & square_bb(move.to));
	res1 += bool(byType[PAWN] & square_bb(move.from));
	res1 += 2 * bool(byType[KNIGHT] & square_bb(move.from));
	res1 += 3 * bool(byType[BISHOP] & square_bb(move.from));
	res1 += 4 * bool(byType[ROOK] & square_bb(move.from));
	res1 += 5 * bool(byType[QUEEN] & square_bb(move.from));
	return { res, -res1 };
}

void MoveList::sort(Position& pos) {
	memset(temp.PriorityMoves, 0, sizeof(temp.PriorityMoves));
	for (int i = 0; i < size; i++) {
		auto [r1, r2] = pos.move_eval(moves[i]);
		temp.PriorityMoves[r1][r2].moves[temp.PriorityMoves[r1][r2].size++] = moves[i];
	}
	int i = 0;
	for (int r1 = 5; r1 >= 0; r1--) {
		for (int r2 = 5; r2 >= 0; r2--) {
			for (int j = 0; j < temp.PriorityMoves[r1][r2].size; j++) {
				moves[i++] = temp.PriorityMoves[r1][r2].moves[j];
			}
		}
	}
}

pair<Move, short> search(Position pos, SearchConfig& config, int depth = 0, short alpha = -10000, short beta = 10000) {
	if (clock() - config.StartTime > config.Movetime) return make_pair(Move(), pos.eval());
	config.VisitedNodes++;
	if (depth == config.Depth) {
		return make_pair(Move(), pos.eval());
	}
	MoveList moves;
	pos.legal_moves(moves);
	//sort moves
	//moves.sort(pos);
	for (int i = 1; i < moves.size; i++) {
		if (pos.move_eval(moves.moves[i]) > pos.move_eval(moves.moves[0])) {
			swap(moves.moves[0], moves.moves[i]);
		}
	}

	short best = pos.sideToMove == WHITE ? -10000 : 10000;
	Move best_move;
	Position pos1;
	for (int i = 0; i < moves.size; i++) {
		memcpy(&pos1, &pos, sizeof(Position));
		pos1.make_move(moves.moves[i]);
		config.Stack.moves[depth] = moves.moves[i];
		auto [next_move, score] = search(pos1, config, depth + 1, alpha, beta);
		if (depth == 0) {
			if (pos.sideToMove == WHITE) {
				if (score > best) {
					best = score;
					best_move = moves.moves[i];
				}
			} else {
				if (score < best) {
					best = score;
					best_move = moves.moves[i];
				}
			}
		}
		best = pos.sideToMove == WHITE ? max(best, score) : min(best, score);
		if (pos.sideToMove == WHITE) {
			if (best >= beta) break;
			alpha = max(alpha, best);
		} else {
			if (best <= alpha) break;
			beta = min(beta, best);
		}
	}
	return make_pair(best_move, best);
}

Move start_search(Position& pos, SearchConfig config) {
	config.StartTime = clock();
	config.Depth = 1;
	Move res;
	short score;
	while (clock() - config.StartTime < config.Movetime) {
		auto r = search(pos, config);
		if (clock() - config.StartTime < config.Movetime) {
			res = r.first;
			score = r.second;
			cout << "time " << clock() - config.StartTime << " depth " << config.Depth << " score cp " << score << " pv " << move_to_string(res) << endl;
		}
		config.Depth++;
	}
	cout << config.VisitedNodes << endl;
	return res;
}