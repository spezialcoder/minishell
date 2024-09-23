/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_launcher_0.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:30:39 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/23 19:42:24 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int obtain_redirect_descriptor(const t_redirect *redirect) {
	if(redirect->type == R_FILE_INPUT) {

	} else if(redirect->type == R_FILE_OUTPUT) {

	} else if(redirect->type == R_FILE_APPEND) {

	} else if(redirect->type == R_DELIMITER) {

	}
	return -1;
}