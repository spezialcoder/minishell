/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:24:13 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/28 17:48:54 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int show_prompt(t_shell *sc) {
    char *prompt;
    char *altered_prompt;

    prompt = "";
    altered_prompt = NULL;
    prompt = readline("minishell> ");
    while(prompt) {
        altered_prompt = handle_string(prompt, ft_strlen(prompt), sc);
        if(*prompt) {
            add_history(prompt);
            if(altered_prompt && *altered_prompt)  {
                handle_prompt(altered_prompt,sc);
                free(altered_prompt);
            }
        }
		free(prompt);
        prompt = readline("minishell> ");
    }
    rl_clear_history();
    return 0;
}

int handle_prompt(char *prompt, t_shell *sc) {
	t_prompt *prompt_data;
    t_error status;

    status = E_OK;
	prompt_data = parse_prompt(prompt, sc);
	status = launch_command(prompt_data, sc, (t_process_io) {0, 1});
    handle_error(status);
    free_prompt(prompt_data);
	return 0;
}

void handle_error(t_error error) {
    if(error == E_CMD_NOT_FOUND) {
        printf("ERROR: command not found.\n");
    } else if(error == E_REDIRECT_FILE_NOT_FOUND) {
        printf("ERROR: file not found.\n");
    }
}