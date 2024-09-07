NAME = minishell
CC = cc
LIBFT_DIR = $(PWD)/lib/libft
CFLAGS = -L($PWD)/lib/libft -lft -Wall -Wextra # -Werror
SHELL_DIR = shell
SRC = main.c $(wildcard $(SHELL_DIR)/shell_*.c)
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

LIBFT:
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re LIBFT
