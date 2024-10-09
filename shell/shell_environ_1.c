/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_environ_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 15:30:25 by lsorg             #+#    #+#             */
/*   Updated: 2024/10/09 16:30:31 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	update_shell_environ(t_shell *sc)
{
	if (sc->envp != NULL)
		free_split_array(sc->envp);
	sc->envp = environ_to_array(&sc->environ);
}

char	*minishell_getenv(t_shell *sc, char *key)
{
	t_size_t	idx;

	idx = 0;
	while (idx < sc->environ.size)
	{
		if (!ft_strncmp(sc->environ.key[idx], key, ft_strlen(key) + 1))
			return (sc->environ.value[idx]);
		idx++;
	}
	return (NULL);
}

void	add_environ(t_environment *env, char *key, char *str)
{
	t_size_t	idx;
	char		**new_key_arr;
	char		**new_val_arr;

	idx = 0;
	while (idx < env->size)
	{
		if (!ft_strncmp(env->key[idx], key, ft_strlen(key) + 1))
		{
			break ;
		}
		idx++;
	}
	if (idx != env->size)
	{
		free(env->value[idx]);
		env->value[idx] = ft_strdup(str);
	}
	else
	{
		new_key_arr = ft_calloc(env->size + 2, sizeof(char *));
		new_val_arr = ft_calloc(env->size + 2, sizeof(char *));
		ft_memcpy(new_key_arr, env->key, env->size * sizeof(char *));
		ft_memcpy(new_val_arr, env->value, env->size * sizeof(char *));
		new_key_arr[env->size] = ft_strdup(key);
		new_val_arr[env->size] = ft_strdup(str);
		free(env->key);
		free(env->value);
		env->key = new_key_arr;
		env->value = new_val_arr;
		env->size++;
	}
}

void	del_environ(t_environment *env, char *key)
{
	t_size_t	idx;
	t_size_t	new_idx;
	char		**new_key_var;
	char		**new_value_var;

	idx = 0;
	new_idx = 0;
	while (idx < env->size)
	{
		if (!ft_strncmp(env->key[idx], key, ft_strlen(key) + 1))
			break ;
		idx++;
	}
	if (idx == env->size)
		return ;
	idx = 0;
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

void export_print(t_shell *sc) {
	t_size_t idx;

	idx = 0;
	while(sc->envp[idx]) {
		printf("declare -x %s\n", sc->envp[idx++]);
	}
}