import pygame
import subprocess

# Запускаем шахматный движок
process = subprocess.Popen('C:\\Users\\ПК\\source\\repos\\HorseChess\\x64\\Release\\HorseChess.exe', stdin=subprocess.PIPE, stdout=subprocess.PIPE)
#process = subprocess.Popen('C:\\Users\\ПК\\source\\repos\\HorseChess\\HorseChess.exe', stdin=subprocess.PIPE, stdout=subprocess.PIPE)
#process.stdin.write(b'set_position\n')
def get_eval():
    process.stdin.write(b'eval\n' )
    process.stdin.flush()
    output = process.stdout.readline()
    return output

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

def best_move():
    process.stdin.write(b'go\n')
    process.stdin.flush()
    output = process.stdout.readline()
    print(output)
    while output.decode().split(" ")[0] != "bestmove":
        output = process.stdout.readline()
        print(output)
    return output.decode().split(" ")[1][:-2]



# Инициализируем pygame и создаем окно
pygame.init()
window = pygame.display.set_mode((800, 800))

# Определяем цвета клеток
light_square = (240, 218, 181)
dark_square = (181, 135, 99)

# Определяем шахматную доску
board = [["r", "n", "b", "q", "k", "b", "n", "r",],
         ["p", "p", "p", "p", "p", "p", "p", "p",],
         [".", ".", ".", ".", ".", ".", ".", ".",],
         [".", ".", ".", ".", ".", ".", ".", ".",],
         [".", ".", ".", ".", ".", ".", ".", ".",],
         [".", ".", ".", ".", ".", ".", ".", ".",],
         ["P", "P", "P", "P", "P", "P", "P", "P",],
         ["R", "N", "B", "Q", "K", "B", "N", "R"]]
sett = ""
sr=0
sc = 0
att = []
fps = 10

imgs = dict()

for fig in "pbnrqkPBNRQK":
    imgs[fig] = pygame.image.load("img/" + fig.lower() + ('b' if fig.islower() else 'w') + '.png');

    imgs[fig] = pygame.transform.scale( imgs[fig], (100, 100))

def draw():
    for row in range(8):
        for col in range(8):
            color = light_square if (row + col) % 2 == 0 else dark_square
            if to_alg(row, col) in att:
                color = "green"
            if to_alg(row, col) == sett:
                color = "gray"
            if sett == "" and to_alg(row, col) in get_moves():
                color = "gray"
            pygame.draw.rect(window, color, pygame.Rect(col * 100, row * 100, 100, 100))
            piece = board[row][col]
            font = pygame.font.Font(None, 15)
            text = font.render(to_alg(row, col), True, (0, 0, 0))
            window.blit(text, (col * 100 + 5, row * 100 + 85))
            if piece != '.':
                window.blit(imgs[piece], (col * 100 , row * 100 ))
                #font = pygame.font.Font(None, 72)
                #text = font.render(piece, True, (0, 0, 0) if piece.islower() else (255, 255, 255))
                #window.blit(text, (col * 100 + 15, row * 100 + 15))

    font = pygame.font.Font(None, 15)
    text = font.render(get_eval()[:-2], True, (0, 0, 0))
    window.blit(text, (750, 5))


clock = pygame.time.Clock()
# Главный цикл игры
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            x, y = pygame.mouse.get_pos()
            row, col = y // 100, x // 100
            if (to_alg(row, col) == sett):
                sett = ""
                att = []
                continue
            if (sett == ""):
                sett = to_alg(row, col)
                sr = row
                sc = col
                att = get_possible_moves(sett)
            else:
                if (to_alg(row, col) not in get_possible_moves(sett)):
                    #continue
                    pass
                print("make_move", sett + to_alg(row, col))
                make_move(sett + to_alg(row, col))
                board[row][col] = board[sr][sc]
                board[sr][sc] = '.'
                sett = ""
                att = []
                draw()
                pygame.display.flip()
                move_opp = best_move()
                print("make_move", move_opp)
                make_move(move_opp)
                board[ord('8') - ord(move_opp[3])][ord(move_opp[2]) - ord('a')] = board[ord('8') - ord(move_opp[1])][ord(move_opp[0]) - ord('a')]
                board[ord('8') - ord(move_opp[1])][ord(move_opp[0]) - ord('a')] = '.'

    # Рисуем шахматную доску
    draw()

    pygame.display.flip()
    clock.tick(fps)

# Закрываем процесс после завершения работы
process.stdin.close()
process.terminate()

pygame.quit()
