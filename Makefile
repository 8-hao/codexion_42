CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
FILES = main.c parser.c codexion.c initializer.c helpers.c pqueue.c coder_actions.c time.c setters.c
OBJ_F = $(FILES:.c=.o)


all: codexion

codexion: $(OBJ_F)
	@$(CC) $(CFLAGS) $(OBJ_F) -o $@


clean:
	@rm -f *.o

fclean: clean
	rm -f codexion

re: fclean all