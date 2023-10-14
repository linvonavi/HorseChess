
#include "Movegen.h"

int main() {
	init_bitboards();
	Position pos;

	string type;
	while (cin >> type) {
		if (type == "set") {
			string fen = "";
			string s;
			for (int i = 0; i < 6; i++) {
				cin >> s;
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
	}


	return 0;
}
