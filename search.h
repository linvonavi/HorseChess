#pragma once

#include "Movegen.h"

struct SearchConfig {
	int Movetime = 5000;
	int Depth;
	int StartTime;
	MoveList Stack;
};


pair<Move, short> search(Position pos, SearchConfig&config, int depth = 0, short alpha = -10000, short beta = 10000) {
	if (clock() - config.StartTime > config.Movetime) return make_pair(Move(), pos.eval());
	if (depth == config.Depth) {
		return make_pair(Move(), pos.eval());
	}
	MoveList moves;
	pos.legal_moves(moves);
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
	return res;
}