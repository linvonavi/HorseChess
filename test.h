#pragma once

#include "Movegen.h"


void rec(Position pos, int depth, MoveList* moves, int it, long long& sum) {
	moves[it].size = 0;
	legal_moves(pos, moves[it]);
	if (depth == 1) {
		sum += moves[it].size;
		return;
	}
	for (int i = 0; i < moves[it].size; i++) {
		Position pos1 = pos;
		pos1.make_move(moves[it].moves[i]);
		it++;
		rec(pos1, depth - 1, moves, it, sum);
		it--;
	}
}

long long test_line(Position pos, int d) {
	MoveList moves[10];
	int it = 0;
	long long sum = 0;
	rec(pos, d, moves, it, sum);
	return sum;
}


void test(Position pos, int d) {
	int time = clock();
	long long sum = 0;
	MoveList moves;
	legal_moves(pos, moves);
	vector<future<long long>> futures;
	for (int i = 0; i < moves.size; i++) {
		Position pos1 = pos;
		pos1.make_move(moves.moves[i]);
		futures.push_back(async(launch::async,test_line, pos1, d - 1));
	}
	for (int i = 0; i < moves.size; i++) {
		long long res = futures[i].get();
		print(moves.moves[i]);
		cout << " " << res << endl;
		sum += res;
	}
	cout << sum << endl;
	cout << float(clock() - time) / CLOCKS_PER_SEC << endl;
}