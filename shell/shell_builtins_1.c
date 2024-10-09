/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builtins_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 14:30:57 by lsorg             #+#    #+#             */
/*   Updated: 2024/10/09 20:50:17 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	builtin_env(t_shell *sc, __attribute__((unused)) t_prompt *prompt,
		__attribute__((unused)) t_process_io io)
{
	int	idx;

	idx = 0;
	while (sc->envp[idx])
	{
		printf("%s\n", sc->envp[idx]);
		idx++;
	}
	printf("\033[0m");
}

void	builtin_exit(t_shell *sc, t_prompt *prompt,
		__attribute__((unused)) t_process_io io)
{
	int	exit_code;

	exit_code = 0;
	if (prompt->argc == 2)
	{
		exit_code = ft_atoi(prompt->parameter->next->content);
	}
	minishell_shutdown(sc, exit_code);
}
