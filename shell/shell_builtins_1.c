/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builtins_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 14:30:57 by lsorg             #+#    #+#             */
/*   Updated: 2024/10/09 17:20:42 by lsorg            ###   ########.fr       */
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

void	builtin_exit(t_shell *sc, __attribute__((unused)) t_prompt *prompt,
	__attribute__((unused)) t_process_io io)
{
	minishell_shutdown(sc, 0);
}
