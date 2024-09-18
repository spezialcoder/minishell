/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:24:13 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/18 14:52:04 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void stash_it(uint32_t *stash_idx, char *char_stash, t_prompt *prompt);
static void parse_command(t_prompt *data, char *prompt);
static int is_valid_cmd_char(char c);
static void lst_append(char *str, t_list **lst, uint32_t ssize);
static void handle_redirect(char *prompt, uint64_t *idx, t_prompt *data, char *char_stash);

static void debug_print(t_prompt *prompt_data) {
	t_list *tmp;
	tmp = prompt_data->parameter;
	printf("Prompt: %s\n",prompt_data->cmd);
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
	/*

	int pipe_token_idx;
    char **pipe_tokens;
    t_prompt *prompt_data;
    t_prompt *current;

    current = NULL;
    pipe_token_idx = 0;
    pipe_tokens = ft_split(prompt, '|');
    while(pipe_tokens[pipe_token_idx]) {
        if(current == NULL) {
            prompt_data = parse_prompt(pipe_tokens[pipe_token_idx++]);
            current = prompt_data;
        } else {
            current->pipe = parse_prompt(pipe_tokens[pipe_token_idx++]);
            current = current->pipe;
        }
		debug_print(current);
    }
    */
	debug_print(prompt_data);
    return 0;
}

t_prompt* parse_prompt(char *prompt) {
    uint64_t idx;
    t_prompt *result;
	char *char_stash;
	uint32_t stash_idx;
	uint8_t quoute_mode;

	//TODO: Handle "" as command
    result = (t_prompt*)ft_calloc(1,sizeof(t_prompt));
	result->argc = 0;
	char_stash = (char*)malloc(sizeof(char)*STASH_SIZE);
	idx = 0;
	stash_idx = 0;
	quoute_mode = 0;
	while(idx < ft_strlen(prompt)) {
		if(prompt[idx] == '\"') {
			quoute_mode ^= 1;
			if(!(quoute_mode & 2)) {
				idx++;
				continue;
			}
		} else if(prompt[idx] == '\'') {
			quoute_mode ^= 2;
			if(!(quoute_mode & 1)) {
				idx++;
				continue;
			}
		}
		if((!ft_memcmp(&prompt[idx],"<<",2) ||
		   !ft_memcmp(&prompt[idx],">>",2) ||
		   prompt[idx] == '<' || prompt[idx] == '>') &&
		   !quoute_mode) {
			stash_it(&stash_idx, char_stash, result);
			handle_redirect(prompt, &idx, result, char_stash);
		}
		else if(prompt[idx] == ' ' && !quoute_mode) {
			stash_it(&stash_idx, char_stash, result);
		} else if(prompt[idx] == '|' && !quoute_mode) {
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
	if(prompt->argc == 0 && *stash_idx > 0) {
		prompt->cmd = (char*)ft_calloc(1,(*stash_idx)+1);
		ft_memcpy(prompt->cmd, char_stash, *stash_idx);
		*stash_idx = 0;
		prompt->argc++;
		return;
	}
	if(*stash_idx > 0) {
		lst_append(char_stash,&prompt->parameter, *stash_idx);
		*stash_idx = 0;
		prompt->argc++;
	}
}

static void handle_redirect(char *prompt, uint64_t *idx, t_prompt *data, char *char_stash) {
	t_list **output;
	uint32_t stash_idx;

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
	lst_append(char_stash, output, stash_idx);
}

static void lst_append(char *str, t_list **lst, uint32_t ssize) {
	ft_lstadd_back(lst, ft_lstnew(ft_calloc(1,ssize+1)));
	ft_memcpy(ft_lstlast(*lst)->content, str, ssize);
}