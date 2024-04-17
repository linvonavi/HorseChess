#pragma once

#include "Movegen.h"

struct SearchConfig {
	int Movetime = 2000;
	int Depth;
	int StartTime;
	int VisitedNodes = 0;
	MoveList Stack;
	MoveList PriorityMoves[36];
};

inline char Position::move_eval(const Move& move) const {
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
	res += move.info == PawnPromotionId;
	return res * 6 + res1;
}

void MoveList::sort(const Position& pos, SearchConfig& config) {
	for (int i = 0; i < 36; i++) {
		config.PriorityMoves[i].size = 0;
	}
	for (int i = 0; i < size; i++) {
		char eval = pos.move_eval(moves[i]);
		config.PriorityMoves[eval].add(moves[i]);
	}
	int i = 0;
	for (int j = 35; j >= 0; j--) {
		for (int k = 0; k < config.PriorityMoves[j].size; k++) {
			moves[i++] = config.PriorityMoves[j].moves[k];
		}
	}
}



pair<Move, short> search(Position pos, SearchConfig& config, int depth = 0, short alpha = -10000, short beta = 10000) {
	config.VisitedNodes++;

	if (clock() - config.StartTime > config.Movetime) return make_pair(Move(), pos.eval());

	if (depth >= config.Depth + 5) return make_pair(Move(), pos.eval());

	Position pos1;
	MoveList moves;
	short best;

	if (depth >= config.Depth) {
		best = pos.eval();
		pos.legal_moves(moves);
		for (int i = 0; i < moves.size; i++) {
			if (!pos.board[moves.moves[i].to]) continue;
			pos1 = pos;
			pos1.make_move(moves.moves[i]);
			auto [next_move, score] = search(pos1, config, depth + 1, alpha, beta);
			best = pos.sideToMove == WHITE ? max(best, score) : min(best, score);
			if (pos.sideToMove == WHITE) {
				if (best >= beta) break;
				alpha = max(alpha, best);
			} else {
				if (best <= alpha) break;
				beta = min(beta, best);
			}
		}
		return make_pair(Move(), best);
	}
	pos.legal_moves(moves);

	moves.sort(pos, config);

	best = pos.sideToMove == WHITE ? -10000 : 10000;
	Move best_move;
	for (int i = 0; i < moves.size; i++) {
		pos1 = pos;
		pos1.make_move(moves.moves[i]);
		config.Stack.moves[depth] = moves.moves[i];
		auto [next_move, score] = search(pos1, config, depth + 1, alpha, beta);
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

template<typename R>
bool is_ready(future<R> const& f) {
	return f.wait_for(chrono::seconds(0)) == future_status::ready;
}


pair<Move, short> first_search(Position& pos, SearchConfig& config) {
	int count_threads = thread::hardware_concurrency();
	short alpha = -10000, beta = 10000;
	short best = pos.sideToMove == WHITE ? -10000 : 10000;
	Move best_move;
	MoveList moves;
	pos.legal_moves(moves);
	moves.sort(pos, config);
	vector<pair<Move, future<pair<SearchConfig, pair<Move, short>>>>> fut;
	int cnt = 0;
	bool finished = 0;
	while (cnt < moves.size) {
		while (fut.size() >= count_threads) {
			int it = 0;
			for (auto& [first_move, i] : fut) {
				if (is_ready(i)) {
					auto [config1, res] = i.get();
					auto [move, score] = res;
					config.VisitedNodes += config1.VisitedNodes;
					if (pos.sideToMove == WHITE) {
						if (score > best) {
							best = score;
							best_move = first_move;
						}
					} else {
						if (score < best) {
							best = score;
							best_move = first_move;
						}
					}
					if (pos.sideToMove == WHITE) {
						if (best >= beta) finished = 1;
						alpha = max(alpha, best);
					} else {
						if (best <= alpha) finished = 1;
						beta = min(beta, best);
					}
					fut.erase(fut.begin() + it);
					break;
				}
				it++;
			}
			if (config.Depth > 5) {
				this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}
		if (finished) break;

		Position pos1;
		pos1 = pos;
		pos1.make_move(moves.moves[cnt]);
		config.Stack.moves[0] = moves.moves[cnt];

		fut.emplace_back(moves.moves[cnt], async([=] {
			auto config_copy = config;
			config_copy.VisitedNodes = 0;
			return make_pair(config_copy, search(pos1, config_copy, 1, alpha, beta));
			}));

		cnt++;
	}
	for (auto& [first_move, i] : fut) {
		auto [config1, res] = i.get();
		auto [move, score] = res;
		config.VisitedNodes += config1.VisitedNodes;
		if (pos.sideToMove == WHITE) {
			if (score > best) {
				best = score;
				best_move = first_move;
			}
		} else {
			if (score < best) {
				best = score;
				best_move = first_move;
			}
		}
		if (pos.sideToMove == WHITE) {
			if (best >= beta) finished = 1;
			alpha = max(alpha, best);
		} else {
			if (best <= alpha) finished = 1;
			beta = min(beta, best);
		}
	}
	return make_pair(best_move, best);
}

void solve(Position pos, int depth, Move fm, SearchConfig config) {
	cout << move_to_string(fm) << ' ';
	pos.make_move(fm);
	for (int d1 = 1; d1 < depth; d1++) {
		config.Depth = depth - d1;
		auto [mo, sc] = first_search(pos, config);
		cout << move_to_string(mo) << ' ';
		pos.make_move(mo);
	}
}

Move start_search(Position& pos, SearchConfig config) {
	config.StartTime = clock();
	config.Depth = 1;
	Move res;
	short score;
	while (clock() - config.StartTime < config.Movetime) {
		auto r = first_search(pos, config);
		if (clock() - config.StartTime < config.Movetime) {
			res = r.first;
			score = r.second;
			cout << "info " << "depth " << config.Depth << " time " << clock() - config.StartTime << " nodes " << config.VisitedNodes << " pv ";
			solve(pos, config.Depth, res, config);
			cout << " score cp " << score << endl;
			if (score == 10000 || score == -10000) break;
		}
		config.Depth++;
	}
	//cout << "visited nodes " << config.VisitedNodes << endl;
	return res;
}