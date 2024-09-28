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

static void debug_print(t_prompt *prompt_data) {
	t_list *tmp;
	tmp = prompt_data->parameter;
	printf("CMD: %s\n",prompt_data->cmd);
	printf("Argc: %d\n",prompt_data->argc);
	for(int i = 0; i < ft_lstsize(prompt_data->parameter); i++) {
		printf("ARGV %s\n", (char*)tmp->content);
		tmp = tmp->next;
	}
	tmp = prompt_data->redirect;
	for(int i = 0; i < ft_lstsize(prompt_data->redirect); i++) {
		switch(((t_redirect*)tmp->content)->type) {
			case R_FILE_INPUT:
				printf("Redirect Input ");
				break;
			case R_FILE_OUTPUT:
				printf("Redirect Output ");
				break;
			case R_FILE_APPEND:
				printf("Redirect Append ");
				break;
			case R_DELIMITER:
				printf("Redirect Delimit ");
				break;
			default:
				printf("Invalid Redirect: ");
				break;
		}
		printf("%s\n", ((t_redirect *)tmp->content)->prompt);
		tmp = tmp->next;
	}
	if(prompt_data->pipe) debug_print(prompt_data->pipe);
}

int show_prompt(t_shell *sc) {
    char *prompt;

    prompt = "";
    prompt = readline("minishell> ");
    while(prompt) {
        if(*prompt) {
            add_history(prompt);
            handle_prompt(prompt,sc);
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
	//debug_print(prompt_data);
	status = launch_command(prompt_data, sc, (t_process_io){0,1});
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