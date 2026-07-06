CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g
FILES = codexion.c parser.c time.c initializer.c helpers.c setters.c pqueue.c coder_actions.c monitor_actions.c
OBJ_F = $(FILES:.c=.o)


all: codexion

codexion: $(OBJ_F)
	@$(CC) $(CFLAGS) $(OBJ_F) -o $@


clean:
	@rm -f *.o

fclean: clean
	rm -f codexion

re: fclean all