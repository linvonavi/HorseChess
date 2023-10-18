import subprocess

# Запускаем шахматный движок
process = subprocess.Popen('C:\\Users\\ПК\\source\\repos\\HorseChess\\x64\\Release\\HorseChess.exe', stdin=subprocess.PIPE, stdout=subprocess.PIPE)
#process.stdin.write(b'set_position\n')

def get_moves():
    process.stdin.write(b'get_moves\n' )
    process.stdin.flush()
    output = process.stdout.readline()
    ans = []
    for move in output.decode().split(" ")[1:]:
        ans.append(move[0:2])
    return ans
# Определяем функции для взаимодействия с шахматным движком
def get_possible_moves(position):
    process.stdin.write(b'get_moves_from_square\n' )
    process.stdin.write(position.encode("utf-8"))
    process.stdin.write(b'\n' )
    process.stdin.flush()
    output = process.stdout.readline()
    ans = []
    for move in output.decode().split(" ")[1:]:
        ans.append(move[2:4])
    return ans

def make_move(move):
    process.stdin.write(b'make_move\n')
    process.stdin.write(move.encode("utf-8"))
    process.stdin.write(b'\n' )
    process.stdin.flush()

def to_alg(row, col):
    return chr(ord('a') + col) + chr(ord('8') - row)

def setpos(fen):
    process.stdin.write(fen.encode())

pos = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - - -"



# Закрываем процесс после завершения работы
process.stdin.close()
process.terminate()

pygame.quit()
