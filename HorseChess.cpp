
#include "Movegen.h"

int main() {
	init_bitboards();
	Position pos;
	pos.set(Start_FEN);
	//pos.move_piece(SQ_B2, SQ_B4);
	print(pos);
	cout << endl;
	print(PseudoAttacks[ROOK][SQ_H1]);
	print(pseudolegal_bishop(pos.byType[ALL_PIECES], SQ_D1));
	MoveList moves;
	legal_moves(pos, moves);
	cout << moves.size << '\n';

	int time = clock();
	int ans = 0;
for (int i = 0; i < 2e7; i++) {
	moves.size = 0;
	if (rnd() % 2) {
		pos.byColor[WHITE] ^= square_bb(SQ_B4);
	}
		legal_moves(pos, moves);
		ans += moves.size;
	}
	cout << float(clock() - time) / CLOCKS_PER_SEC << endl;
cout << ans << endl;


	return 0;
}
