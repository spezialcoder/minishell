/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:24:13 by lsorg             #+#    #+#             */
/*   Updated: 2024/10/10 19:22:20 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	show_prompt(t_shell *sc)
{
	char	*prompt;
	char	*altered_prompt;

	altered_prompt = NULL;
	prompt = readline("minishell> ");
	while (prompt)
	{
		altered_prompt = handle_string(prompt, ft_strlen(prompt), sc);
		add_history(prompt);
		if (altered_prompt)
		{
			handle_prompt(altered_prompt, sc);
			if (*prompt)
				free(altered_prompt);
		}
		free(prompt);
		prompt = readline("minishell> ");
	}
	rl_clear_history();
	return (0);
}

int	handle_prompt(char *prompt, t_shell *sc)
{
	t_prompt	*prompt_data;
	t_error		status;

	prompt_data = parse_prompt(prompt, sc);
	if (!prompt_data || !prompt_data->cmd)
		return (free_prompt(prompt_data), 0);
	status = launch_command(prompt_data, sc, (t_process_io){0, 1});
	handle_error(status, sc);
	free_prompt(prompt_data);
	return (0);
}

void	handle_error(t_error error, t_shell *sc)
{
	if (error == E_CMD_NOT_FOUND)
	{
		printf("ERROR: command not found.\n");
		sc->recent_exit_code = 127;
	}
	else if (error == E_REDIRECT_FILE_NOT_FOUND)
	{
		printf("ERROR: file not found.\n");
	}
}
