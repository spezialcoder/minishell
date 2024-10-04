/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:24:13 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/28 15:25:02 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void disable_ctrl_echo();
static void minishell_cleanup(t_shell *sc);

int minishell_boot(char **envp) {
	t_shell sc;

    sc = (t_shell) {.processes = NULL, .recent_exit_code=0};
    init_environment(&sc.environ, envp);
    update_shell_environ(&sc);
	disable_ctrl_echo();
    setup_signal_handlers();
    show_prompt(&sc);
    minishell_shutdown(&sc, 0);
	return (0);
}

__attribute__((noreturn))
void minishell_shutdown(t_shell *sc, int code) {
    minishell_cleanup(sc);
    exit(code);
}

static void minishell_cleanup(t_shell *sc) {
    if(sc->envp) free_split_array(sc->envp);
    if(sc->environ.value) free_split_array(sc->environ.value);
    if(sc->environ.key) free_split_array(sc->environ.key);
}

static void disable_ctrl_echo() {
	struct termios tty;

	if(tcgetattr(STDIN_FILENO, &tty) != 0) return;
	tty.c_lflag &= ~ECHOCTL;
	if(tcsetattr(STDIN_FILENO, TCSANOW, &tty) != 0) return;
}