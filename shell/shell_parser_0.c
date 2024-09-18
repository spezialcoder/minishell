/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_parser_0.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 14:59:35 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/18 16:51:08 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int is_valid_var_char(char c);

char* handle_string(char *str, uint32_t ssize) {
	uint32_t idx;
	uint32_t out_idx;
	uint8_t quote_mode;
	char *env_var;
	uint32_t stash_idx;
	char *arg_buffer;
	char *result;

	env_var = (char*)malloc(sizeof(char)*VAR_BUFFER);
	arg_buffer = (char*)malloc(sizeof(char)*ARG_MAX);
	quote_mode = 0;
	idx = 0;
	out_idx = 0;
	stash_idx = 0;
	while(idx < ssize) {
		if(str[idx] == '\"') quote_mode ^= 1;
		else if(str[idx] == '\'' && !(quote_mode&1)) quote_mode ^= 2;
		if(str[idx] == '$' && !(quote_mode&2)) {
			while(is_valid_var_char(str[++idx]))
				env_var[stash_idx++] = str[idx];
			env_var[stash_idx] = 0;
			if(getenv(env_var)) {
				ft_memcpy(&arg_buffer[out_idx], getenv(env_var), ft_strlen(getenv(env_var)));
				out_idx += ft_strlen(getenv(env_var));
			}
			printf("Var detected %s\n",env_var);
			stash_idx = 0;
		} else {
			arg_buffer[out_idx++] = str[idx++];
		}

	}
	result = ft_memcpy(ft_calloc(1,out_idx+1), arg_buffer, out_idx);
	return (free(env_var), free(arg_buffer), result);
}

uint8_t handle_quote(char *prompt, uint64_t *idx, uint8_t *quote_mode, char *char_stash, uint32_t *stash_idx) {
	if(prompt[*idx] == '\"') {
		if(!(*quote_mode&2) && (*quote_mode&1) &&
		   *stash_idx == 0) {
			char_stash[0] = 0;
			(*stash_idx)++;
		}
		*quote_mode ^= 1;
	} else if(prompt[*idx] == '\'') {
		if((*quote_mode&2) && !(*quote_mode&1) &&
		   *stash_idx == 0) {
			char_stash[0] = 0;
			(*stash_idx)++;
		}
		*quote_mode ^= 2;
	}
	return (0);
}

static int is_valid_var_char(char c) {
	return ((c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			(c >= '0' && c <= '9') ||
			c == '_');
}