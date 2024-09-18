/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:24:13 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/18 17:28:49 by lsorg            ###   ########.fr       */
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
    int recent_exit_code; // Most recent exit code for $?
} t_shell;

//Minishell main
int minishell_boot();

//Prompt
int show_prompt(t_shell sc);

//Parser
typedef struct s_prompt {
    char  *cmd;
	uint32_t argc;
    t_list *parameter;
    t_list *redirect_delimit;
    t_list *redirect_input;
    t_list *redirect_output;
    t_list *redirect_append;
    struct s_prompt *pipe;
} t_prompt;

struct s_string_parser {
	char *env_var;
	uint32_t stash_idx;
	char *arg_buffer;
	char *result;
	t_size_t tmp_var_len;
};

uint8_t handle_quote(const char *prompt, const uint64_t *idx, uint8_t *quote_mode, char *char_stash, uint32_t *stash_idx);
int handle_prompt(char *prompt, t_shell sc);
t_prompt* parse_prompt(char *prompt);
char* handle_string(char *str, uint32_t ssize);
void free_prompt(t_prompt *prompt);

//Misc
