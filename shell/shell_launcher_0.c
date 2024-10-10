/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_launcher_0.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:30:39 by lsorg             #+#    #+#             */
/*   Updated: 2024/10/10 19:26:48 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int		delimiter_input(char *delim);

t_builtin_ptr	get_builtin(const t_prompt *prompt)
{
	if (!ft_strncmp(prompt->cmd, "echo", 5))
		return (builtin_echo);
	else if (!ft_strncmp(prompt->cmd, "cd", 3))
		return (builtin_cd);
	else if (!ft_strncmp(prompt->cmd, "pwd", 4))
		return (builtin_pwd);
	else if (!ft_strncmp(prompt->cmd, "export", 7))
		return (builtin_export);
	else if (!ft_strncmp(prompt->cmd, "unset", 6))
		return (builtin_unset);
	else if (!ft_strncmp(prompt->cmd, "env", 4))
		return (builtin_env);
	else if (!ft_strncmp(prompt->cmd, "exit", 5))
		return (builtin_exit);
	return (NULL);
}

int	obtain_redirect_descriptor(const t_redirect *redirect)
{
	if (redirect->type == R_FILE_INPUT)
	{
		return (open(redirect->prompt, O_RDONLY));
	}
	else if (redirect->type == R_FILE_OUTPUT)
	{
		return (open(redirect->prompt, O_WRONLY | O_TRUNC | O_CREAT, 0644));
	}
	else if (redirect->type == R_FILE_APPEND)
	{
		return (open(redirect->prompt, O_WRONLY | O_APPEND | O_CREAT, 0644));
	}
	else if (redirect->type == R_DELIMITER)
	{
		return (delimiter_input(redirect->prompt));
	}
	return (-1);
}

static int	delimiter_input(char *delim)
{
	char	*input;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
	{
		return (-1);
	}
	input = readline("> ");
	while (input && ft_memcmp(input, delim, ft_strlen(input) + 1))
	{
		write(pipefd[1], input, ft_strlen(input));
		write(pipefd[1], "\n", 1);
		free(input);
		input = readline("> ");
		if (!input)
			return (-1);
	}
	if (input)
		free(input);
	else
		return (close(pipefd[1]), close(pipefd[0]), -1);
	return (close(pipefd[1]), pipefd[0]);
}
