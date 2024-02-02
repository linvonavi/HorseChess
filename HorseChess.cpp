
#include "Movegen.h"
#include "Search.h"
#include "Eval.h"


int main() {
	init_bitboards();
	Position pos;
	pos.set(StartFEN);

	string type;
	while (cin >> type) {
		if (type == "uci") {
			cout << "id name HorseChess" << endl;
			cout << "id author HorseChess" << endl;
			cout << "uciok" << endl;
		}
		if (type == "isready") {
			cout << "readyok" << endl;
		}
		if (type == "ucinewgame") {
			pos.set(StartFEN);
		}

		if (type == "go") {
			SearchConfig config;
			string s;
			getline(cin, s);
			stringstream ss(s);
			while (ss >> s) {
				if (s == "movetime") {
					ss >> config.Movetime;
				}
			}
			string move = move_to_string(start_search(pos, config));
			cout << "bestmove " << move << endl;
		}

		if (type == "debug") {
			Debug = !Debug;
		}
		if (type == "go_perft") {
			int depth;
			cin >> depth;
			// TODO
		}
		if (type == "position") {
			string fen;
			getline(cin, fen);
			pos.set(fen);
			if (Debug) {
				print(pos);
			}
		}
		if (type == "get_moves") {
			MoveList moves;
			pos.legal_moves(moves);
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
			pos.legal_moves(moves);
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
