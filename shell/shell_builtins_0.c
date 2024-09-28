/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builtins_0.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 14:30:57 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/28 18:11:01 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void builtin_echo(t_shell *sc, t_prompt *prompt, t_process_io io) {
	t_list *param;

	param = (t_list*)prompt->parameter->next;
	while(param) {
		write(io.sout,param->content,ft_strlen((char*)param->content));
		param = param->next;
		if(param) write(io.sout," ",1);
	}
	write(io.sout,"\n",1);
}

void builtin_cd(t_shell *sc, t_prompt *prompt, t_process_io io) {

}

void builtin_pwd(t_shell *sc, t_prompt *prompt, t_process_io io) {

}

void builtin_export(t_shell *sc, t_prompt *prompt, t_process_io io) {

}

void builtin_unset(t_shell *sc, t_prompt *prompt, t_process_io io) {

}
