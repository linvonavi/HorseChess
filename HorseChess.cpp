
#include "Movegen.h"

int main() {
	init_bitboards();
	Position pos;

	string type;
	while (cin >> type) {
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
		}
		if (type == "get_moves") {
			MoveList moves;
			legal_moves(pos, moves);
			cout << moves.size << ' ';
			for (int i = 0; i < moves.size; i++) {
				cout << square_to_string(moves.moves[i].from) << square_to_string(moves.moves[i].to) << ' ';
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
					cout << square_to_string(moves.moves[i].from) << square_to_string(moves.moves[i].to) << ' ';
				}
			}
			cout << endl;
		}
		if (type == "make_move") {
			string move;
			cin >> move;
			Move m = string_to_move(move);
			pos.make_move(m);
		}
	}


	return 0;
}
