
#include "Movegen.h"
#include "test.h"


int main() {
	init_bitboards();
	Position pos;
	pos.set(StartFEN);

	string type;
	while (cin >> type) {
		if (type == "debug") {
			Debug = !Debug;
		}
		if (type == "go_perft") {
			int depth;
			cin >> depth;
			test(pos, depth);
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
		if (type == "get_fen") {
			cout << pos.get_fen() << endl;
		}
	}


	return 0;
}
