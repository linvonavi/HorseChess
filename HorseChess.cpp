
#include "Movegen.h"

long long sum = 0;

void rec(Position&pos, int depth) {
	//print(pos);
	MoveList moves;
	legal_moves(pos, moves);
	if (depth == 1) {
		sum += moves.size;
		return;
	}
	for (int i = 0; i < moves.size; i++) {
		Position pos1 = pos;
		pos1.make_move(moves.moves[i]);
		rec(pos1, depth - 1);
	}
}

void test(Position pos, int d) {
	int time = clock();
	rec(pos, d);
	cout << sum << endl;
	cout << float(clock() - time) / CLOCKS_PER_SEC << endl;
}

int main() {
	init_bitboards();
	Position pos;
	pos.set(StartFEN);
	/*for (int i = 1; i <=7; i++) {
		cout << i << endl;
		test(pos, i);
		cout << endl;
	}*/

	string type;
	while (cin >> type) {
		if (type == "debug") {
			Debug = !Debug;
		}
		if (type == "set_position") {
			string fen = "";
			string s;
			for (int i = 0; i < 6; i++) {
				cin >> s;
				if (s == "start") {
					fen = StartFEN;
					break;
				}
				fen += s + " ";
			}
			pos.set(fen);
			if (Debug) {
				print(pos);
			}
		}
		if (type == "get_moves") {
			MoveList moves;
			legal_moves(pos, moves);
			cout << moves.size << ' ';
			for (int i = 0; i < moves.size; i++) {
				print(moves.moves[i]);
			}
			cout << endl;
		}
		if (type == "get_moves_from_square") {
			string square;
			cin >> square;
			Square sq = string_to_square(square);
			MoveList moves;
			legal_moves(pos, moves);
			int sz = 0;
			for (int i = 0; i < moves.size; i++) {
				sz += (moves.moves[i].from == sq);
			}
			cout << sz << ' ';
			for (int i = 0; i < moves.size; i++) {
				if (moves.moves[i].from == sq) {
					print(moves.moves[i]);
				}
			}
			cout << endl;
		}
		if (type == "make_move") {
			string move;
			cin >> move;
			Move m = string_to_move(move);
			pos.make_move(m);
			if (Debug) {
				print(pos);
			}
		}
	}


	return 0;
}
