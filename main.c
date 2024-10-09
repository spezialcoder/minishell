/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 15:30:25 by lsorg             #+#    #+#             */
/*   Updated: 2024/10/09 20:42:52 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell/shell.h"

int	main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv,
		char **envp)
{
	minishell_boot(envp);
	return (0);
}
