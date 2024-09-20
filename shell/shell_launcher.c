/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_launcher.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 14:40:37 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/20 16:18:05 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char* find_binary(char *cmd);
static char* concat_path_file(char *path, char *file);
static void cmd_processor(char *cmd, int fd_in, int fd_out, char **argv, char **envp);

int launch_command(t_prompt *prompt, t_shell *sc) {
	t_prompt *current;

	current = prompt;
	printf("Binary: %s\n", find_binary(current->cmd));
	return 0;
}

static void cmd_processor(char *cmd, int fd_in, int fd_out, char **argv, char **envp) {
	dup2(fd_in, 1);
	dup2(fd_out, 0);
	execve(cmd, argv, envp);
}

static char* find_binary(char *cmd) {
	char **path;
	char *path_str;
	char *binary;
	uint32_t path_idx;

	if(!cmd) return (NULL);
	path_str = getenv("PATH");
	if(access(cmd, F_OK | X_OK) == 0)
		return (ft_memcpy(ft_calloc(1, ft_strlen(cmd)+0), cmd, ft_strlen(cmd)));
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