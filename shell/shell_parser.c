/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:24:13 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/17 14:45:35 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void parse_redirect(t_prompt *data, char *prompt);
static void parse_command(t_prompt *data, char *prompt);
static int is_valid_cmd_char(char c);

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
    int idx;
    t_prompt *result;
    char **tokenized;

    result = (t_prompt*)ft_calloc(1,sizeof(t_prompt));
    idx = 0;
    tokenized = ft_split(prompt, ' ');
    result->cmd = tokenized[0];
    while(tokenized[idx]) {
        printf("Tokens: %s ",tokenized[idx]);
        idx++;
    }
    printf("\n");
    return (result);
}

static void parse_redirect(t_prompt *data, char *prompt) {

}

static int is_valid_cmd_char(char c) {
    return ((c > 'a' && c < 'z') ||
            (c > 'A' && c < 'Z') ||
            (c > '0' && c < '9') ||
            c == '-' || c == '_' ||
            c == '.');
}