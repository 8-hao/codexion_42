CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
FILES = main.c parser.c codexion.c initializer.c cleaner.c coder_actions.c
OBJ_F = (FILES:.c=.o)


all: codexion

codexion: $(FILES)
	@$(CC) $(CFLAGS) $(FILES) -o $@