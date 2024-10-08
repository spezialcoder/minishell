/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_interface_0.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:24:13 by lsorg             #+#    #+#             */
/*   Updated: 2024/10/08 18:43:45 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

// TODO: fix double prompt when ctrl-c running command like cat
static void	handle_signals(int sig)
{
	if (sig == SIGINT)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		printf("\n");
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		rl_redisplay();
	}
}

void	setup_signal_handlers(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_signals;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART | SA_NOCLDWAIT;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

t_error	setup_process(t_process *ps, const t_prompt *prompt, t_shell *sc,
		t_process_io io)
{
	if (get_builtin(prompt))
	{
		ps->cmd = ft_strdup("builtin");
	}
	else
	{
		ps->cmd = find_binary(prompt->cmd, sc);
	}
	if (!ps->cmd)
		return (free(ps), E_CMD_NOT_FOUND);
	ps->argv = list_to_array(prompt->parameter);
	ps->envp = sc->envp;
	ps->io = io;
	return (E_OK);
}
