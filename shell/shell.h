#pragma once

#include <readline/readline.h>
#include <readline/history.h>
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
    char **parameter;
    uint8_t append_mode;
    char  *redirect_input;
    char  *redirect_output;
    char  *redirect_delim;
    struct s_prompt *pipe;
} t_prompt;

int handle_prompt(char *prompt, t_shell sc);
t_prompt* parse_prompt(char *prompt);

//Misc
