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

extern char* extract_key(char *entry);
extern char* extract_value(char *entry);

void builtin_echo(t_shell *sc, t_prompt *prompt, t_process_io io) {
	t_list *param;
    int nl;

    nl = -1;
    param = (t_list*)prompt->parameter->next;
    if(param)
        nl = ft_strncmp((char *) param->content, "-n",2);
    if (!nl) {
        param = param->next;
    }
    while(param) {
		write(io.sout,param->content,ft_strlen((char*)param->content));
		param = param->next;
		if(param) write(io.sout," ",1);
	}
	if(nl)
        write(io.sout,"\n",1);
}

void builtin_cd(t_shell *sc, t_prompt *prompt, t_process_io io) {
    char *new_path;
    t_list *param;

    param = prompt->parameter->next;
    if(param)
    {
        new_path = (char*)param->content;
    }
    else {
        printf("ERROR: cmd requires arguments.\n");
        return;
    }
    if(new_path) {
        if(chdir(new_path) == -1) {
            printf("ERROR: cmd unable to change directory.\n");
        }
    }
}

void builtin_pwd(t_shell *sc, t_prompt *prompt, t_process_io io) {
    char pwd[PATH_MAX];

    getcwd(pwd, PATH_MAX);
    write(io.sout, pwd, ft_strlen(pwd));
    write(io.sout, "\n", 1);
}

void builtin_export(t_shell *sc, t_prompt *prompt, t_process_io io) {
    char *key;
    char *value;
    char *str;
    if(prompt->argc != 2) {
        printf("ERROR: export KEY=VALUE\n");
        return;
    }
    str = (char*)prompt->parameter->next->content;
    key = extract_key(str);
    value = extract_value(str);
    if(!key || !value) {
        printf("ERROR: export syntax error.\n");
        return;
    }
    add_environ(&sc->environ, key, value);
    update_shell_environ(sc);
}

void builtin_unset(t_shell *sc, t_prompt *prompt, t_process_io io) {

}
