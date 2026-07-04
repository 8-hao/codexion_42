CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g
FILES = codexion.c parser.c initializer.c setters.c helpers.c time.c coder_actions.c pqueue.c
OBJ_F = $(FILES:.c=.o)


all: codexion

codexion: $(OBJ_F)
	@$(CC) $(CFLAGS) $(OBJ_F) -o $@


clean:
	@rm -f *.o

fclean: clean
	rm -f codexion