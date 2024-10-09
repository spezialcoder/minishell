/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:24:13 by lsorg             #+#    #+#             */
/*   Updated: 2024/10/09 16:17:12 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libft.h"
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <termcap.h>
#include <termios.h>
#include <unistd.h>
#include <readline/history.h>
#include <readline/readline.h>

#define STASH_SIZE 4096
#define VAR_BUFFER 4096
#define ARG_MAX 16384
#define PATH_MAX 4096
#pragma GCC diagnostic ignored "-Wunused-variable"

typedef unsigned char		t_uint8_t;
typedef unsigned short		t_uint16_t;
typedef unsigned int		t_uint32_t;
typedef unsigned long long	t_uint64_t;

typedef struct s_environment
{
	char					**key;
	char					**value;
	t_size_t				size;
}							t_environment;

typedef struct s_shell
{
	t_environment			environ;
	char					**envp;
	t_list					*processes;
	int						recent_exit_code;
}							t_shell;

// Minishell main
int							minishell_boot(char **envp);
void						minishell_shutdown(t_shell *sc, int code);

// Prompt
typedef enum e_error
{
	E_CMD_NOT_FOUND = 0,
	E_OK = 1,
	E_REDIRECT_FILE_NOT_FOUND = 2,
	E_NOBUILTIN = 3,
	E_ABORT = 4
}							t_error;

int							show_prompt(t_shell *sc);
void						handle_error(t_error error, t_shell *sc);

// Parser

typedef enum e_redirect_type
{
	R_FILE_INPUT = 0,
	R_FILE_OUTPUT = 1,
	R_FILE_APPEND = 2,
	R_DELIMITER = 3
}							t_redirect_type;

typedef struct s_redirect
{
	enum e_redirect_type	type;
	char					*prompt;
}							t_redirect;

typedef struct s_prompt
{
	char					*cmd;
	t_uint32_t				argc;
	t_list					*parameter;
	t_list					*redirect;
	struct s_prompt			*pipe;
}							t_prompt;

struct						s_string_parser
{
	char					*env_var;
	t_uint32_t				stash_idx;
	char					*arg_buffer;
	char					*result;
	t_size_t				tmp_var_len;
};

t_uint8_t					handle_quote(const char *prompt, t_uint64_t *idx,
								t_uint8_t *quote_mode, char *char_stash,
								t_uint32_t *stash_idx);
t_prompt					*parse_prompt(char *prompt, t_shell *sc);
char						*handle_string(char *str, t_uint32_t ssize,
								t_shell *sc);
void						free_prompt(t_prompt *prompt);
int							handle_prompt(char *prompt, t_shell *sc);
void						free_t_redirect(void *addr);

// Launcher

typedef struct s_process_io
{
	int						sin;
	int						sout;
}							t_process_io;

typedef struct s_process
{
	pid_t					process_id;
	t_process_io			io;
	char					*cmd;
	char					**argv;
	char					**envp;
}							t_process;

typedef void				(*t_builtin_ptr)(t_shell *, t_prompt *,
					t_process_io);
t_error						setup_process(t_process *ps, const t_prompt *prompt,
								t_shell *sc, t_process_io io);
t_error						launch_command(t_prompt *prompt, t_shell *sc,
								t_process_io io);
int							obtain_redirect_descriptor(
								const t_redirect *redirect);
t_builtin_ptr				get_builtin(const t_prompt *prompt);
char						*find_binary(char *cmd, t_shell *sc);

// Misc
void						free_split_array(char **arr);
char						**list_to_array(t_list *lst);
void						free_t_process(void *addr);
void						ft_lstpop(t_list **lst, void (*del)(void *));

// Interface
void						setup_signal_handlers(void);

// Builtins
void						builtin_echo(t_shell *sc, t_prompt *prompt,
								t_process_io io);
void						builtin_cd(t_shell *sc, t_prompt *prompt,
								t_process_io io);
void						builtin_pwd(t_shell *sc, t_prompt *prompt,
								t_process_io io);
void						builtin_export(t_shell *sc, t_prompt *prompt,
								t_process_io io);
void						builtin_unset(t_shell *sc, t_prompt *prompt,
								t_process_io io);
void						builtin_env(t_shell *sc, t_prompt *prompt,
								t_process_io io);
void						builtin_exit(t_shell *sc, t_prompt *prompt,
								t_process_io io);

// Environ

void						init_environment(t_environment *env, char **envp);
char						**environ_to_array(t_environment *env);
void						update_shell_environ(t_shell *sc);
char						*minishell_getenv(t_shell *sc, char *key);
void						add_environ(t_environment *env, char *key,
								char *str);
void						del_environ(t_environment *env, char *key);
void 						export_print(t_shell *sc) ;