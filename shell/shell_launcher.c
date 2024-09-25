/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_launcher.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 14:40:37 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/23 19:35:39 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char* find_binary(char *cmd);
static char* concat_path_file(char *path, char *file);
static void resolve_process_io(const t_prompt *prompt, t_process_io *io);
static void cmd_processor(t_process *ps);

t_error launch_command(const t_prompt *prompt, t_shell *sc, t_process_io io) {
	t_process *command;
    t_process_io pipe_io;
    int pipefd[2];
    int status;

    status = 0;
    command = (t_process*)ft_calloc(1,sizeof(t_process));
    command->cmd = find_binary(prompt->cmd);
    if(!command->cmd) return (free(command), E_CMD_NOT_FOUND);
    command->argv = list_to_array(prompt->parameter);
    command->envp = sc->envp; //TODO: Envp used
    command->io = io;
    ft_memset(pipefd, 0, sizeof(pipefd));
    if(prompt->pipe) {
        pipe(pipefd);
        command->io.sout = pipefd[1];
        pipe_io.sin = pipefd[0];
        pipe_io.sout = 1;
    }
    resolve_process_io(prompt, &command->io);
    command->process_id = fork();
    ft_lstadd_back(&sc->processes, ft_lstnew((void*)command));
    if(command->process_id == 0) {
        cmd_processor(command);
    }
    if(prompt->pipe) {
        close(pipefd[1]);
        launch_command(prompt->pipe, sc, pipe_io);
    }
    if(pipefd[0]) close(pipefd[0]);
    waitpid(command->process_id, &status, 0);
    ft_lstpop(&sc->processes, free_t_process);
    if(!prompt->pipe) {
        if(WIFEXITED(status))
            sc->recent_exit_code = WEXITSTATUS(status);
    }
	return E_OK;
}

static void cmd_processor(t_process *ps) {
    dup2(ps->io.sout, 1);
    dup2(ps->io.sin, 0);
    execve(ps->cmd, ps->argv, ps->envp);
}

static void resolve_process_io(const t_prompt *prompt, t_process_io *io) {
	uint8_t redirect_status;
	t_redirect *redirect;
	t_list *current;

	redirect_status = 0;
	current = prompt->redirect;
	while(current) {
		redirect = (t_redirect*)current->content;
        if(redirect->type == R_FILE_OUTPUT ||
                redirect->type == R_FILE_APPEND) {
            if(redirect_status&1) close(io->sout);
            io->sout = obtain_redirect_descriptor((t_redirect*)current->content);
            if(!(redirect_status&1)) redirect_status |= 1;
        } else {
            if(redirect_status&2) close(io->sin);
            io->sin = obtain_redirect_descriptor((t_redirect*)current->content);
            if(!(redirect_status&2)) redirect_status |= (1<<1);
        }
		current = current->next;
	}
}

static char* find_binary(char *cmd) {
	char **path;
	char *path_str;
	char *binary;
	uint32_t path_idx;

	if(!cmd) return (NULL);
	path_str = getenv("PATH");
	if(access(cmd, F_OK | X_OK) == 0)
		return (ft_memcpy(ft_calloc(1, ft_strlen(cmd)+1), cmd, ft_strlen(cmd)));
	if(!path_str)
		path_str = "";
	path = ft_split(path_str,':');
	path_idx = 0;
	while(path[path_idx]) {
		binary = concat_path_file(path[path_idx], cmd);
		if(!access(binary, F_OK | X_OK))
			return (free_split_array(path), binary);
		free(binary);
		path_idx++;
	}
	binary = NULL;
	return (free_split_array(path), binary);
}

static char* concat_path_file(char *path, char *file) {
	char *output;
	char *tmp;

	if(!path || !*path) return ft_calloc(1,1);
	if(path[ft_strlen(path)-1] == '/') {
		output = ft_strjoin(path, file);
	} else {
		output = ft_strjoin(path, "/");
		tmp = output;
		output = ft_strjoin(output, file);
		free(tmp);
	}
	return (output);
}