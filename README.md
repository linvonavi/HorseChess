# HorseChess
* set board:
  ```position <fen>``` or  ```position startpos```
* get possible moves:
```get_moves```
* get possible moves from position in algebraic notation:
```get_moves_from_position <position>```
example: ```get_moves_from_position a2```
* make move
```make_move <move>```
example: ```make_move a2a4```
* start search:
```go``` or ```go movetime <ms>```
* turn on/off debug
```debug```
example:
```
debug
make_move e2e4
r n b q k b n r
p p p p p p p p
. . . . . . . .
. . . . . . . .
. . . . P . . .
. . . . . . . .
P P P P . P P P
R N B Q K B N R
```
* perft ```go_perft <n>```
