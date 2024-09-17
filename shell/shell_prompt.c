/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:24:13 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/17 14:45:23 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

