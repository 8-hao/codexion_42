CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g
FILES = src/codexion.c src/parser.c src/time.c src/initializer.c src/helpers.c src/setters.c src/pqueue.c src/coder_actions.c src/monitor_actions.c
OBJ_F = $(FILES:.c=.o)


all: codexion

codexion: $(OBJ_F)
	@$(CC) $(CFLAGS) $(OBJ_F) -o $@


clean:
	@rm -f $(OBJ_F)

fclean: clean
	rm -f codexion

re: fclean all

.PHONY : all clean fclean re