/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:24:13 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/17 18:08:01 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void parse_command(t_prompt *data, char *prompt);
static int is_valid_cmd_char(char c);
static void lst_append(char *str, t_list **lst, uint32_t ssize);
static void handle_redirect(char *prompt, uint64_t *idx, t_prompt *data, char *char_stash);

static void debug_print(t_prompt *prompt_data) {
	t_list *tmp;
	tmp = prompt_data->parameter;
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
}

int handle_prompt(char *prompt, t_shell sc) {
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
    }
    return 0;
}

t_prompt* parse_prompt(char *prompt) {
    uint64_t idx;
    t_prompt *result;
	char *char_stash;
	uint32_t stash_idx;

	//TODO: Handle command parse
    result = (t_prompt*)ft_calloc(1,sizeof(t_prompt));
	char_stash = (char*)malloc(sizeof(char)*STASH_SIZE);
	idx = 0;
	stash_idx = 0;
	while(idx < ft_strlen(prompt)) {
		if(!ft_memcmp(&prompt[idx],"<<",2) ||
		   !ft_memcmp(&prompt[idx],">>",2) ||
		   prompt[idx] == '<' || prompt[idx] == '>') {
			handle_redirect(prompt, &idx, result, char_stash);
		}
		else if(prompt[idx] == ' ') {
			if(stash_idx > 0)
				lst_append(char_stash,&result->parameter, stash_idx);
			stash_idx = 0;
		} else {
			char_stash[stash_idx++] = prompt[idx];
		}
		idx++;
	}
	if(stash_idx > 0)
		lst_append(char_stash,&result->parameter, stash_idx);
	free(char_stash);
    return (result);
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
		prompt[*idx] == '>' || prompt[*idx] == '|') break;
		char_stash[stash_idx++] = prompt[(*idx)++];
	}
	lst_append(char_stash, output, stash_idx);
}

static void lst_append(char *str, t_list **lst, uint32_t ssize) {
	ft_lstadd_back(lst, ft_lstnew(ft_calloc(1,ssize+1)));
	ft_memcpy(ft_lstlast(*lst)->content, str, ssize);
}

static int is_valid_cmd_char(char c) {
    return ((c > 'a' && c < 'z') ||
            (c > 'A' && c < 'Z') ||
            (c > '0' && c < '9') ||
            c == '-' || c == '_' ||
            c == '.');
}