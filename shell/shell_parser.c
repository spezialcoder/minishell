/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:24:13 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/18 17:29:07 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void stash_it(uint32_t *stash_idx, char *char_stash, t_prompt *prompt);
static void handle_redirect(char *prompt, uint64_t *idx, t_prompt *data, char *char_stash);

static void debug_print(t_prompt *prompt_data) {
	t_list *tmp;
	tmp = prompt_data->parameter;
	printf("Prompt: %s\n",prompt_data->cmd);
	printf("Argc: %d\n",prompt_data->argc);
	for(int i = 0; i < ft_lstsize(prompt_data->parameter); i++) {
		printf("Parameter %s\n", (char*)tmp->content);
		tmp = tmp->next;
	}
	tmp = prompt_data->redirect_input;
	for(int i = 0; i < ft_lstsize(prompt_data->redirect_input); i++) {
		printf("Redirect Input %s\n", (char*)tmp->content);
		tmp = tmp->next;
	}
	tmp = prompt_data->redirect_output;
	for(int i = 0; i < ft_lstsize(prompt_data->redirect_output); i++) {
		printf("Redirect Output %s\n", (char*)tmp->content);
		tmp = tmp->next;
	}
	tmp = prompt_data->redirect_append;
	for(int i = 0; i < ft_lstsize(prompt_data->redirect_append); i++) {
		printf("Redirect Append %s\n", (char*)tmp->content);
		tmp = tmp->next;
	}
	tmp = prompt_data->redirect_delimit;
	for(int i = 0; i < ft_lstsize(prompt_data->redirect_delimit); i++) {
		printf("Redirect Delim %s\n", (char*)tmp->content);
		tmp = tmp->next;
	}
	if(prompt_data->pipe) debug_print(prompt_data->pipe);
}

int handle_prompt(char *prompt, t_shell sc) {
	t_prompt *prompt_data;

	prompt_data = parse_prompt(prompt);
	debug_print(prompt_data);
	free(prompt_data);
    return 0;
}

t_prompt* parse_prompt(char *prompt) {
    uint64_t idx;
    t_prompt *result;
	char *char_stash;
	uint32_t stash_idx;
	uint8_t quote_mode;

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
			result->pipe = parse_prompt(prompt+idx+1);
			break;
		} else {
				char_stash[stash_idx++] = prompt[idx];
		}
		idx++;
	}
	stash_it(&stash_idx, char_stash, result);
	free(char_stash);
    return (result);
}

static void stash_it(uint32_t *stash_idx, char *char_stash, t_prompt *prompt) {
	char *result;

	if(prompt->argc == 0 && *stash_idx > 0) {
		result = handle_string(char_stash, *stash_idx);
		prompt->cmd = result;
		*stash_idx = 0;
		prompt->argc++;
		return;
	}
	if(*stash_idx > 0) {
		result = handle_string(char_stash, *stash_idx);
		ft_lstadd_back(&prompt->parameter, ft_lstnew((void*)result));
		*stash_idx = 0;
		prompt->argc++;
	}
}

static void handle_redirect(char *prompt, uint64_t *idx, t_prompt *data, char *char_stash) {
	t_list **output;
	uint32_t stash_idx;
	char *result;

	if(!ft_memcmp(&prompt[*idx],"<<", 2)) {
		output = &data->redirect_delimit;
		(*idx)++;
	} else if(!ft_memcmp(&prompt[*idx], ">>", 2)) {
		output = &data->redirect_append;
		(*idx)++;
	} else if(prompt[*idx] == '<') {
		output = &data->redirect_input;
	} else
		output = &data->redirect_output;
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
	result = handle_string(char_stash, stash_idx);
	ft_lstadd_back(output, ft_lstnew((void*)result));
}
