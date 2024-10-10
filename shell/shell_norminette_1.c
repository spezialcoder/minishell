/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_norminette_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:14:00 by lsorg             #+#    #+#             */
/*   Updated: 2024/10/10 14:14:00 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int	is_valid_var_char(char c);
static void	cmd_processor(t_process *ps);

void	launch_command_norm(t_prompt *prompt, t_shell *sc, t_launch_norm *lc)
{
	ft_lstadd_back(&sc->processes, ft_lstnew((void *)(*lc).command));
	(*lc).builtin_ptr = get_builtin(prompt);
	if ((*lc).builtin_ptr && !prompt->pipe)
		(*lc).builtin_ptr(sc, prompt, (*lc).command->io);
	else
	{
		(*lc).command->process_id = fork();
		if ((*lc).command->process_id == 0)
		{
			if ((*lc).builtin_ptr)
			{
				(*lc).builtin_ptr(sc, prompt, (*lc).command->io);
				exit(0);
			}
			else
				cmd_processor((*lc).command);
		}
	}
	if (prompt->pipe)
	{
		close(lc->pipefd[1]);
		lc->error = launch_command(prompt->pipe, sc, lc->pipe_io);
		close(lc->pipefd[0]);
	}
}

void	handle_string_norm_2(t_handle_string_norm *d)
{
	while (is_valid_var_char((*d).str[++(*(*d).idx)])
		&& (*(*d).idx) < (*d).ssize)
	{
		if ((*d).str[(*(*d).idx)] >= '0' && (*d).str[(*(*d).idx)] <= '9'
			&& (*(*d).idx) == 0)
		{
			(*(*d).idx)++;
			break ;
		}
		(*(*d).sp).env_var[(*(*d).sp).stash_idx++] = (*d).str[(*(*d).idx)];
	}
	(*(*d).sp).env_var[(*(*d).sp).stash_idx] = 0;
	(*d).var_content = minishell_getenv((*d).sc, (*(*d).sp).env_var);
}

t_uint8_t	resolve_process_io_norm(t_process_io *io, t_uint8_t redirect_status,
		const t_redirect *redirect)
{
	{
		if (redirect_status & 1)
			close(io->sout);
		io->sout = obtain_redirect_descriptor(redirect);
		if (!(redirect_status & 1))
			redirect_status |= 1;
	}
	return (redirect_status);
}

static int	is_valid_var_char(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0'
			&& c <= '9') || c == '_');
}

static void	cmd_processor(t_process *ps)
{
	dup2(ps->io.sout, 1);
	dup2(ps->io.sin, 0);
	if (execve(ps->cmd, ps->argv, ps->envp))
	{
		perror("ERROR: not a executable");
	}
	exit(0);
}
