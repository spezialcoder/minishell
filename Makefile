NAME = minishell
CC = cc
LIBFT_DIR = $(PWD)/lib/libft
LIBS = -L$(LIBFT_DIR) -lreadline -lncurses -lft
CFLAGS = -g -O3 -I$(LIBFT_DIR) -Wall -Wextra # -Werror
SHELL_DIR = shell
SRC = main.c $(wildcard $(SHELL_DIR)/shell*.c)
OBJ = $(SRC:.c=.o)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)


$(NAME): $(OBJ) LIBFT
	$(CC) $(OBJ) $(CFLAGS) $(LIBS) -o $@

LIBFT:
	@echo "Compiling libft"
	$(MAKE) bonus -C $(LIBFT_DIR)

clean:
	$(MAKE) clean -C $(LIBFT_DIR)
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re LIBFT
