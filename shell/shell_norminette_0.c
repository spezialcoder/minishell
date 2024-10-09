/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_norminette_0.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:29:21 by lsorg             #+#    #+#             */
/*   Updated: 2024/10/09 17:52:39 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	del_environ_norm(t_environment *env, const char *key, t_size_t idx,
		t_size_t new_idx)
{
	char **new_key_var;
	char **new_value_var;

	new_key_var = ft_calloc(env->size, sizeof(char *));
	new_value_var = ft_calloc(env->size, sizeof(char *));
	while (idx < env->size)
	{
		if (ft_strncmp(env->key[idx], key, ft_strlen(key) + 1))
		{
			new_key_var[new_idx] = env->key[idx];
			new_value_var[new_idx++] = env->value[idx];
		}
		else
		{
			free(env->key[idx]);
			free(env->value[idx]);
		}
		idx++;
	}
	free(env->key);
	free(env->value);
	env->key = new_key_var;
	env->value = new_value_var;
	env->size = env->size - 1;
}