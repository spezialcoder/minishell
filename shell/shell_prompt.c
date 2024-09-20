/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:24:13 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/20 15:01:11 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void debug_print(t_prompt *prompt_data) {
	t_list *tmp;
	tmp = prompt_data->parameter;
	printf("Prompt: %s\n",prompt_data->cmd);
	printf("Argc: %d\n",prompt_data->argc);
	for(int i = 0; i < ft_lstsize(prompt_data->parameter); i++) {
		printf("Parameter %s\n", (char*)tmp->content);
		tmp = tmp->next;
	}
	tmp = prompt_data->redirect_input;
	for(int i = 0; i < ft_lstsize(prompt_data->redirect_input); i++) {
		printf("Redirect Input %s\n", (char*)tmp->content);
		tmp = tmp->next;
	}
	tmp = prompt_data->redirect_output;
	for(int i = 0; i < ft_lstsize(prompt_data->redirect_output); i++) {
		printf("Redirect Output %s\n", (char*)tmp->content);
		tmp = tmp->next;
	}
	tmp = prompt_data->redirect_append;
	for(int i = 0; i < ft_lstsize(prompt_data->redirect_append); i++) {
		printf("Redirect Append %s\n", (char*)tmp->content);
		tmp = tmp->next;
	}
	tmp = prompt_data->redirect_delimit;
	for(int i = 0; i < ft_lstsize(prompt_data->redirect_delimit); i++) {
		printf("Redirect Delim %s\n", (char*)tmp->content);
		tmp = tmp->next;
	}
	if(prompt_data->pipe) debug_print(prompt_data->pipe);
}

int show_prompt(t_shell *sc) {
    char *prompt;

    prompt = "";
    while(prompt) {
        prompt = readline("minishell> ");
        add_history(prompt);
        handle_prompt(prompt,sc);
    }
    free(prompt);
    rl_clear_history();
    return 0;
}

int handle_prompt(char *prompt, t_shell *sc) {
	t_prompt *prompt_data;

	prompt_data = parse_prompt(prompt, sc);
	launch_command(prompt_data, sc);
    free_prompt(prompt_data);
	return 0;
}