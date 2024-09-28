/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:24:13 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/23 19:42:24 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void stash_it(uint32_t *stash_idx, char *char_stash, t_prompt *prompt);
static void handle_redirect(char *prompt, uint64_t *idx, t_prompt *data, char *char_stash);

t_prompt* parse_prompt(char *prompt, t_shell *sc) {
    uint64_t idx;
    t_prompt *result;
	char *char_stash;
	uint32_t stash_idx;
	uint8_t quote_mode;

	prompt = handle_string(prompt, ft_strlen(prompt), sc);
    result = (t_prompt*)ft_calloc(1,sizeof(t_prompt));
	result->argc = 0;
	char_stash = (char*)malloc(sizeof(char)*STASH_SIZE);
	idx = 0;
	stash_idx = 0;
	quote_mode = 0;
	while(idx < ft_strlen(prompt)) {
		handle_quote(prompt,&idx,&quote_mode,char_stash,&stash_idx);
		if((!ft_memcmp(&prompt[idx],"<<",2) ||
		   !ft_memcmp(&prompt[idx],">>",2) ||
		   prompt[idx] == '<' || prompt[idx] == '>') &&
		   !quote_mode) {
			stash_it(&stash_idx, char_stash, result);
			handle_redirect(prompt, &idx, result, char_stash);
		}
		else if(prompt[idx] == ' ' && !quote_mode) {
			stash_it(&stash_idx, char_stash, result);
		} else if(prompt[idx] == '|' && !quote_mode) {
			result->pipe = parse_prompt(prompt+idx+1, sc);
			break;
		} else {
				char_stash[stash_idx++] = prompt[idx];
		}
		idx++;
	}
	stash_it(&stash_idx, char_stash, result);
    return (free(char_stash), free(prompt), result);
}

static void stash_it(uint32_t *stash_idx, char *char_stash, t_prompt *prompt) {
	char *result;

	if(prompt->argc == 0 && *stash_idx > 0) {
		result = (char*)ft_calloc(1,*stash_idx+1);
		ft_memcpy(result, char_stash, *stash_idx);
		prompt->cmd = result;
	}
	if(*stash_idx > 0) {
		result = (char*)ft_calloc(1,*stash_idx+1);
		ft_memcpy(result, char_stash, *stash_idx);
		ft_lstadd_back(&prompt->parameter, ft_lstnew((void*)result));
		*stash_idx = 0;
		prompt->argc++;
	}
}

static void handle_redirect(char *prompt, uint64_t *idx, t_prompt *data, char *char_stash) {
	uint32_t stash_idx;
	t_redirect *new_redirect;

	new_redirect = ft_calloc(1,sizeof(t_redirect));
	if(!ft_memcmp(&prompt[*idx],"<<", 2)) {
		new_redirect->type = R_DELIMITER;
		(*idx)++;
	} else if(!ft_memcmp(&prompt[*idx], ">>", 2)) {
		new_redirect->type = R_FILE_APPEND;
		(*idx)++;
	} else if(prompt[*idx] == '<') {
		new_redirect->type = R_FILE_INPUT;
	} else
		new_redirect->type = R_FILE_OUTPUT;
	(*idx)++;
	while(prompt[*idx] == ' ') (*idx)++;
	stash_idx = 0;
	while(*idx < ft_strlen(prompt)) {
		if(prompt[*idx] == ' ' || prompt[*idx] == '<' ||
		prompt[*idx] == '>' || prompt[*idx] == '|') {
			(*idx)--;
			break;
		}
		char_stash[stash_idx++] = prompt[(*idx)++];
	}
	new_redirect->prompt = (char*)ft_calloc(1,stash_idx+1);
	ft_memcpy(new_redirect->prompt, char_stash, stash_idx);
	ft_lstadd_back(&data->redirect, ft_lstnew((void*)new_redirect));
}
