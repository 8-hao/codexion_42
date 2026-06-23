CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
FILES = src/main.c src/parser.c src/codexion.c src/pqueue.c src/initializer.c src/time.c src/coder_actions.c
OBJ_F = (FILES:.c=.o)


all: main

main: $(FILES)
	@$(CC) $(CFLAGS) $(FILES) -o $@