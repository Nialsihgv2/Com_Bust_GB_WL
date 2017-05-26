SRC=com_bust.c
BIN=com_bust

$(BIN): $(SRC) physique.o lis_ast.o struct.o
	gcc -Wall -g -std=c99 $(SRC) `sdl-config  --cflags --libs` -o $(BIN) physique.o lis_ast.o struct.o -lm
physique.o: physique.c physique.h lis_ast.o struct.o
	gcc -Wall -Wextra -pedantic -g -std=c99 -c `sdl-config --cflags --libs` -lm -O3 physique.c struct.o lis_ast.o
lis_ast.o: lis_ast.c lis_ast.h struct.o
	gcc -Wall -Wextra -pedantic -g -std=c99 -c -O3 lis_ast.c struct.o
struct.o: struct.h struct.c
	gcc -Wall -Wextra -pedantic -g -std=c99 -c -O3 struct.c
clean:
	rm -f $(BIN)
