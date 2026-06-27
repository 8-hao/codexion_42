CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g
FILES = main.c parser.c codexion.c initializer.c cleaner.c pqueue.c coder_actions.c
OBJ_F = (FILES:.c=.o)


all: codexion

codexion: $(FILES)
	@$(CC) $(CFLAGS) $(FILES) -o $@