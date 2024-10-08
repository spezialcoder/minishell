/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_environ.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 15:30:25 by lsorg             #+#    #+#             */
/*   Updated: 2024/10/08 18:44:04 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char		*extract_key(char *entry);
char		*extract_value(char *entry);
static char	*make_str(char *key, char *value);

void	init_environment(t_environment *env, char **envp)
{
	int	size;
	int	idx;

	size = 0;
	idx = 0;
	while (envp[size])
		size++;
	env->key = ft_calloc(size + 1, sizeof(char *));
	env->value = ft_calloc(size + 1, sizeof(char *));
	env->size = size;
	while (idx < size)
	{
		env->key[idx] = extract_key(envp[idx]);
		env->value[idx] = extract_value(envp[idx]);
		idx++;
	}
}

char	**environ_to_array(t_environment *env)
{
	char		**envp;
	t_size_t	idx;

	idx = 0;
	envp = ft_calloc(env->size + 1, sizeof(char *));
	while (idx < env->size)
	{
		envp[idx] = make_str(env->key[idx], env->value[idx]);
		idx++;
	}
	return (envp);
}

static char	*make_str(char *key, char *value)
{
	char	*string;
	int		value_len;
	int		key_len;

	value_len = ft_strlen(value);
	key_len = ft_strlen(key);
	string = ft_calloc(key_len + value_len + 2, sizeof(char));
	ft_memcpy(string, key, key_len);
	string[key_len] = '=';
	ft_memcpy(string + key_len + 1, value, value_len);
	return (string);
}

char	*extract_key(char *entry)
{
	char		*key;
	char		*end;
	t_size_t	len;

	end = ft_strchr(entry, '=');
	if (!end)
		return (NULL);
	len = end - entry;
	key = ft_calloc(len + 1, sizeof(char));
	ft_memcpy(key, entry, len);
	return (key);
}

char	*extract_value(char *entry)
{
	char	*value;
	char	*start;

	start = ft_strchr(entry, '=');
	if (!start)
		return (NULL);
	value = ft_strdup(start + 1);
	return (value);
}
