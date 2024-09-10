#include "shell.h"

int show_prompt(t_shell sc) {
    char *prompt;

    prompt = "";
    while(prompt) {
        prompt = readline("minishell> ");
        add_history(prompt);
        handle_prompt(prompt,(t_shell){});
    }
    free(prompt);
    rl_clear_history();
    return 0;
}

