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

int minishell_boot(char **envp) {
	t_shell sc;

	sc = (t_shell){envp, NULL, 0};
	disable_ctrl_echo();
    setup_signal_handlers();
    show_prompt(&sc);
	return 0;
}

static void disable_ctrl_echo() {
	struct termios tty;

	if(tcgetattr(STDIN_FILENO, &tty) != 0) return;
	tty.c_lflag &= ~ECHOCTL;
	if(tcsetattr(STDIN_FILENO, TCSANOW, &tty) != 0) return;
}