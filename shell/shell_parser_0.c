/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_parser_0.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 14:59:35 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/20 14:35:23 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int is_valid_var_char(char c);

char* handle_string(char *str, uint32_t ssize) {
	uint32_t idx;
	uint32_t out_idx;
	uint8_t quote_mode;
	struct s_string_parser sp;

	sp.env_var = (char*)malloc(sizeof(char)*VAR_BUFFER);
	sp.arg_buffer = (char*)malloc(sizeof(char)*ARG_MAX);
	quote_mode = 0;
	idx = 0;
	out_idx = 0;
	sp.stash_idx = 0;
	while(idx < ssize) {
		if(str[idx] == '\"') {
			quote_mode ^= 1;
		}
		else if(str[idx] == '\'' && !(quote_mode&1)) {
			quote_mode ^= 2;
		}
		if(str[idx] == '$' && !(quote_mode&2)) {
			while(is_valid_var_char(str[++idx]) && idx < ssize)
				sp.env_var[sp.stash_idx++] = str[idx];
			sp.env_var[sp.stash_idx] = 0;
			if(getenv(sp.env_var)) {
				sp.tmp_var_len = ft_strlen(getenv(sp.env_var));
				ft_memcpy(&sp.arg_buffer[out_idx], getenv(sp.env_var),sp.tmp_var_len);
				out_idx += sp.tmp_var_len;
			}
			sp.stash_idx = 0;
		} else
			sp.arg_buffer[out_idx++] = str[idx++];
	}
	sp.result = ft_memcpy(ft_calloc(1,out_idx+1), sp.arg_buffer, out_idx);
	return (free(sp.env_var), free(sp.arg_buffer), sp.result);
}

uint8_t handle_quote(const char *prompt, uint64_t *idx, uint8_t *quote_mode, char *char_stash, uint32_t *stash_idx) {
	if(prompt[*idx] == '\"') {
		if(!(*quote_mode&2) && (*quote_mode&1) &&
		   *stash_idx == 0) {
			char_stash[0] = 0;
			(*stash_idx)++;
		}
		*quote_mode ^= 1;
		if(!(*quote_mode & 2)) {
			(*idx)++;
			return(1);
		}
	} else if(prompt[*idx] == '\'') {
		if((*quote_mode&2) && !(*quote_mode&1) &&
		   *stash_idx == 0) {
			char_stash[0] = 0;
			(*stash_idx)++;
		}
		*quote_mode ^= 2;
		if(!(*quote_mode & 1)) {
			(*idx)++;
			return (1);
		}
	}
	return (0);
}

void free_prompt(t_prompt *prompt) {
	t_prompt *current;
	t_prompt *tmp;

	current = prompt;
	tmp = current;
	while(current) {
		free(current->cmd);
		ft_lstclear(&current->parameter,free);
		ft_lstclear(&current->redirect_input,free);
		ft_lstclear(&current->redirect_output,free);
		ft_lstclear(&current->redirect_append,free);
		ft_lstclear(&current->redirect_delimit,free);
		current = current->pipe;
		free(tmp);
		tmp = current;
	}
}

static int is_valid_var_char(char c) {
	return ((c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			(c >= '0' && c <= '9') ||
			c == '_');
}