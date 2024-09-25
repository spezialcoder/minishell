/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:24:13 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/23 19:31:14 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <termios.h>
#include <termcap.h>
#include "libft.h"
#include <readline/readline.h>
#include <readline/history.h>

#define STASH_SIZE 4096
#define VAR_BUFFER 4096
#define ARG_MAX 16384

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef struct s_shell {
	char **envp;
    t_list *processes;
    int recent_exit_code; // Most recent exit code for $?
} t_shell;

//Minishell main
int minishell_boot(char **envp);

//Prompt
typedef enum e_error {
    E_CMD_NOT_FOUND=0,
    E_OK=1,
    E_REDIRECT_FILE_NOT_FOUND=2
} t_error;

int show_prompt(t_shell *sc);
void handle_error(t_error error);

//Parser

typedef enum e_redirect_type {
	R_FILE_INPUT=0,
	R_FILE_OUTPUT=1,
	R_FILE_APPEND=2,
	R_DELIMITER=3
} t_redirect_type;

typedef struct s_redirect {
	enum e_redirect_type type;
	char *prompt;
} t_redirect;

typedef struct s_prompt {
    char  *cmd;
	uint32_t argc;
    t_list *parameter;
    t_list *redirect;
    struct s_prompt *pipe;
} t_prompt;

struct s_string_parser {
	char *env_var;
	uint32_t stash_idx;
	char *arg_buffer;
	char *result;
	t_size_t tmp_var_len;
};

uint8_t handle_quote(const char *prompt, uint64_t *idx, uint8_t *quote_mode, char *char_stash, uint32_t *stash_idx);
t_prompt* parse_prompt(char *prompt, t_shell *sc);
char* handle_string(char *str, uint32_t ssize, t_shell *sc);
void free_prompt(t_prompt *prompt);
int handle_prompt(char *prompt, t_shell *sc);
void free_t_redirect(void *addr);

//Launcher

typedef struct s_process_io {
	int sin;
	int sout;
} t_process_io;

typedef struct s_process {
	pid_t process_id;
	t_process_io io;
	char *cmd;
	char **argv;
	char **envp;
} t_process;

t_error launch_command(const t_prompt *prompt, t_shell *sc, t_process_io io);
int obtain_redirect_descriptor(const t_redirect *redirect);

//Misc
void free_split_array(char **arr);
char	**list_to_array(t_list *lst);
void free_t_process(void *addr);
void ft_lstpop(t_list **lst, void (*del)(void*));