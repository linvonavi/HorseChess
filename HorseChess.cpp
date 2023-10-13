#include "Position.h"
#include "Bitboards.h"

int main() {
	init_bitboards();
	Position pos;
	pos.set(Start_FEN);
	pos.move_piece(SQ_A2, SQ_A4);
	print(PseudoAttacks[KNIGHT][SQ_B3]);
	
	return 0;
}
