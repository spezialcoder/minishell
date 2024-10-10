/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_launcher.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 14:40:37 by lsorg             #+#    #+#             */
/*   Updated: 2024/10/09 20:21:42 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char		*concat_path_file(char *path, char *file);
static t_error	resolve_process_io(const t_prompt *prompt, t_process_io *io);

t_error	launch_command(t_prompt *prompt, t_shell *sc, t_process_io io)
{
	t_launch_norm	lc;

	lc.status = 0;
	lc.command = (t_process *)ft_calloc(1, sizeof(t_process));
	lc.error = setup_process(lc.command, prompt, sc, io);
	if (lc.error != E_OK)
		return (free_t_process(lc.command), lc.error);
	ft_memset(lc.pipefd, 0, sizeof(lc.pipefd));
	if (prompt->pipe)
	{
		pipe(lc.pipefd);
		lc.command->io.sout = lc.pipefd[1];
		lc.pipe_io.sin = lc.pipefd[0];
		lc.pipe_io.sout = 1;
	}
	lc.error = resolve_process_io(prompt, &lc.command->io);
	if (lc.error != E_OK)
		return (free_t_process(lc.command), lc.error);
	launch_command_norm(prompt, sc, &lc);
	waitpid(lc.command->process_id, &lc.status, 0);
	ft_lstpop(&sc->processes, free_t_process);
	if (!prompt->pipe && WIFEXITED(lc.status))
		sc->recent_exit_code = WEXITSTATUS(lc.status);
	return (lc.error);
}

static t_error	resolve_process_io(const t_prompt *prompt, t_process_io *io)
{
	t_uint8_t	redirect_status;
	t_redirect	*redirect;
	t_list		*current;

	redirect_status = 0;
	current = prompt->redirect;
	while (current)
	{
		redirect = (t_redirect *)current->content;
		if (redirect->type == R_FILE_OUTPUT || redirect->type == R_FILE_APPEND)
			redirect_status = resolve_process_io_norm(io, redirect_status,
					redirect);
		else
		{
			if (redirect_status & 2)
				close(io->sin);
			io->sin = obtain_redirect_descriptor(redirect);
			if (io->sin == -1)
				return (E_REDIRECT_FILE_NOT_FOUND);
			if (!(redirect_status & 2))
				redirect_status |= (1 << 1);
		}
		current = current->next;
	}
	return (E_OK);
}

char	*find_binary(char *cmd, t_shell *sc)
{
	char		**path;
	char		*path_str;
	char		*binary;
	t_uint32_t	path_idx;

	if (!cmd)
		return (NULL);
	path_str = minishell_getenv(sc, "PATH");
	if (access(cmd, X_OK) == 0)
		return (ft_memcpy(ft_calloc(1, ft_strlen(cmd) + 1), cmd,
				ft_strlen(cmd)));
	if (!path_str)
		path_str = "";
	path = ft_split(path_str, ':');
	path_idx = 0;
	while (path[path_idx])
	{
		binary = concat_path_file(path[path_idx], cmd);
		if (!access(binary, X_OK))
			return (free_split_array(path), binary);
		free(binary);
		path_idx++;
	}
	binary = NULL;
	return (free_split_array(path), binary);
}

static char	*concat_path_file(char *path, char *file)
{
	char	*output;
	char	*tmp;

	if (!path || !*path)
		return (ft_calloc(1, 1));
	if (path[ft_strlen(path) - 1] == '/')
	{
		output = ft_strjoin(path, file);
	}
	else
	{
		output = ft_strjoin(path, "/");
		tmp = output;
		output = ft_strjoin(output, file);
		free(tmp);
	}
	return (output);
}
