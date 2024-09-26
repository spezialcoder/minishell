/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_interface_0.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:24:13 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/18 15:21:31 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void handle_signals(int sig) {
    if(sig == SIGINT) {
        rl_on_new_line();
        rl_replace_line("",0);
        printf("\n");
        rl_redisplay();
    }
}

void setup_signal_handlers() {
    struct sigaction sa;

    sa.sa_handler = handle_signals;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDWAIT;
    sigaction(SIGINT, &sa, NULL);
    //TODO: dunno if this could be useful: sigaction(SIGCHLD, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

t_error setup_process(t_process *ps, const t_prompt *prompt, const t_shell *s, t_process_io io) {
    ps->cmd = find_binary(prompt->cmd);
    if(!ps->cmd) return (free(ps), E_CMD_NOT_FOUND);
    ps->argv = list_to_array(prompt->parameter);
    ps->envp = sc->envp; //TODO: Envp used
    ps->io = io;
    return (E_OK);
}