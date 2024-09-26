/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_launcher_0.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:30:39 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/23 19:42:24 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int delimiter_input(char *delim);

t_error launch_builtin(t_prompt *prompt, t_shell *sc) {
    t_size_t prompt_length;

    prompt_length = ft_strlen(prompt->cmd);
    if      (!ft_strncmp(prompt->cmd,"echo", prompt_length)) builtin_echo(sc,prompt);
    else if(!ft_strncmp(prompt->cmd,"cd", prompt_length)) builtin_cd(sc,prompt);
    else if(!ft_strncmp(prompt->cmd,"pwd", prompt_length)) builtin_pwd(sc);
    else if(!ft_strncmp(prompt->cmd,"export", prompt_length)) builtin_export(sc,prompt);
    else if(!ft_strncmp(prompt->cmd,"unset", prompt_length)) builtin_unset(sc,prompt);
    else if(!ft_strncmp(prompt->cmd,"env", prompt_length)) builtin_env(sc,prompt);
    else if(!ft_strncmp(prompt->cmd,"exit", prompt_length)) builtin_exit(sc,prompt);
    else return (E_NOBUILTIN);
    return (E_OK);
}

int obtain_redirect_descriptor(const t_redirect *redirect) {
    if(redirect->type == R_FILE_INPUT) {
        return open(redirect->prompt,O_RDONLY);
	} else if(redirect->type == R_FILE_OUTPUT) {
        return open(redirect->prompt,O_WRONLY | O_CREAT);
	} else if(redirect->type == R_FILE_APPEND) {
        return open(redirect->prompt, O_WRONLY | O_APPEND, O_CREAT);
	} else if(redirect->type == R_DELIMITER) {
        return delimiter_input(redirect->prompt);
	}
	return -1;
}

static int delimiter_input(char *delim) {
    char *input;
    int pipefd[2];

    if(pipe(pipefd) == -1) {
        return (-1);
    }
    input = readline("> ");
    while(ft_memcmp(input, delim, ft_strlen(input))) {
        write(pipefd[1], input, ft_strlen(input));
        write(pipefd[1], "\n\0", 2);
        free(input);
        input = readline("> ");
        if(!input) return -1;
    }
    return (close(pipefd[1]), pipefd[0]);
}