/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_parser_0.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 14:59:35 by lsorg             #+#    #+#             */
/*   Updated: 2024/10/08 18:28:15 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int	is_valid_var_char(char c);

char	*handle_string(char *str, t_uint32_t ssize, t_shell *sc)
{
	t_uint32_t				idx;
	t_uint32_t				out_idx;
	t_uint8_t					quote_mode;
	struct s_string_parser	sp;
	char					*var_content;

	if (!str || !*str)
		return (str);
	sp.env_var = (char *)malloc(sizeof(char) * VAR_BUFFER);
	sp.arg_buffer = (char *)malloc(sizeof(char) * ARG_MAX);
	quote_mode = 0;
	idx = 0;
	out_idx = 0;
	sp.stash_idx = 0;
	while (idx < ssize)
	{
		if (str[idx] == '\"' && !(quote_mode & 2))
			quote_mode ^= 1;
		else if (str[idx] == '\'' && !(quote_mode & 1))
			quote_mode ^= 2;
		if (str[idx] == '$' && !(quote_mode & 2))
		{
			if (str[idx + 1] == '?')
			{
				var_content = ft_itoa(sc->recent_exit_code);
				idx++;
			}
			else
			{
				while (is_valid_var_char(str[++idx]) && idx < ssize)
				{
					if (str[idx] >= '0' && str[idx] <= '9' && idx == 0)
					{
						idx++;
						break ;
					}
					sp.env_var[sp.stash_idx++] = str[idx];
				}
				sp.env_var[sp.stash_idx] = 0;
				var_content = minishell_getenv(sc, sp.env_var);
			}
			if (var_content)
			{
				sp.tmp_var_len = ft_strlen(var_content);
				ft_memcpy(&sp.arg_buffer[out_idx], var_content, sp.tmp_var_len);
				out_idx += sp.tmp_var_len;
			}
			if (str[idx] == '?')
			{
				free(var_content);
				idx++;
			}
			sp.stash_idx = 0;
		}
		else
			sp.arg_buffer[out_idx++] = str[idx++];
	}
	sp.result = ft_memcpy(ft_calloc(1, out_idx + 1), sp.arg_buffer, out_idx);
	return (free(sp.env_var), free(sp.arg_buffer), sp.result);
}

t_uint8_t	handle_quote(const char *prompt, t_uint64_t *idx, t_uint8_t *quote_mode,
						  char *char_stash, t_uint32_t *stash_idx)
{
	if (prompt[*idx] == '\"' && !(*quote_mode & 2))
	{
		if ((*quote_mode & 1) && *stash_idx == 0)
		{
			char_stash[0] = 0;
			(*stash_idx)++;
		}
		*quote_mode ^= 1;
		(*idx)++;
		if (prompt[*idx] == '\"')
			return ((*idx)++, *quote_mode ^= 1, 1);
		return (1);
	}
	else if (prompt[*idx] == '\'' && !(*quote_mode & 1))
	{
		if ((*quote_mode & 2) && *stash_idx == 0)
		{
			char_stash[0] = 0;
			(*stash_idx)++;
		}
		*quote_mode ^= 2;
		(*idx)++;
		if (prompt[*idx] == '\'')
			return ((*idx)++, *quote_mode ^= 2, 1);
		return (1);
	}
	return (0);
}

void	free_prompt(t_prompt *prompt)
{
	t_prompt	*current;
	t_prompt	*tmp;

	current = prompt;
	tmp = current;
	while (current)
	{
		if (current->cmd)
			free(current->cmd);
		if (current->parameter)
			ft_lstclear(&current->parameter, free);
		if (current->redirect)
			ft_lstclear(&current->redirect, free_t_redirect);
		current = current->pipe;
		free(tmp);
		tmp = current;
	}
}

static int	is_valid_var_char(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0'
			&& c <= '9') || c == '_');
}