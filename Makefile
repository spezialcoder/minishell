NAME = minishell
CC = cc
LIBFT_DIR = $(PWD)/lib/libft
LIBS = -L$(LIBFT_DIR) -lreadline -lncurses -lft
CFLAGS = -g -I$(LIBFT_DIR) -Wall -Wextra -O3 -Werror
SHELL_DIR = shell
SRC =	main.c \
		$(SHELL_DIR)/shell.c \
		$(SHELL_DIR)/shell_builtins_0.c \
		$(SHELL_DIR)/shell_builtins_1.c \
		$(SHELL_DIR)/shell_environ.c \
		$(SHELL_DIR)/shell_environ_1.c \
		$(SHELL_DIR)/shell_interface_0.c \
		$(SHELL_DIR)/shell_launcher.c \
		$(SHELL_DIR)/shell_launcher_0.c \
		$(SHELL_DIR)/shell_misc.c \
		$(SHELL_DIR)/shell_parser.c \
		$(SHELL_DIR)/shell_parser_0.c \
		$(SHELL_DIR)/shell_prompt.c \
		$(SHELL_DIR)/shell_norminette_0.c \
		$(SHELL_DIR)/shell_norminette_1.c

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
