CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
FILES = src/main.c src/parser.c src/threads.c
OBJ_F = (FILES:.c=.o)


all: main

main: $(FILES)
	@$(CC) $(FILES) -o $@