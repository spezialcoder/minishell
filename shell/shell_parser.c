#include "shell.h"

static void parse_redirect(t_prompt *data, char *prompt);
static void parse_command(t_prompt *data, char *prompt);
static int is_valid_cmd_char(char c);

int handle_prompt(char *prompt, t_shell sc) {
    int pipe_token_idx;
    char **pipe_tokens;
    t_prompt prompt_data;

    pipe_token_idx = 0;
    pipe_tokens = ft_split(prompt, '|');
    while(pipe_tokens[pipe_token_idx]) {
        parse_prompt(pipe_tokens[pipe_token_idx++]);
    }

}

t_prompt parse_prompt(char *prompt) {
    int idx;
    t_prompt result;
    char **tokenized;

    idx = 0;
    tokenized = ft_split(prompt, ' ');
    result.cmd = tokenized[0];
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