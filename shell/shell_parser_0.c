/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_parser_0.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 14:59:35 by lsorg             #+#    #+#             */
/*   Updated: 2024/10/09 20:09:30 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	handle_string_norm_3(t_uint32_t *idx, t_uint32_t *out_idx,
		struct s_string_parser *sp, char **var_content)
{
	(*idx) = 0;
	(*out_idx) = 0;
	(*sp).stash_idx = 0;
	(*var_content) = NULL;
}

char	*handle_string(char *str, t_uint32_t ssize, t_shell *sc)
{
	t_uint32_t				idx;
	t_uint32_t				out_idx;
	t_uint8_t				quote_mode;
	struct s_string_parser	sp;
	char					*var_content;

	if (!str || !*str)
		return (str);
	sp.env_var = (char *)malloc(sizeof(char) * VAR_BUFFER);
	sp.arg_buffer = (char *)malloc(sizeof(char) * ARG_MAX);
	quote_mode = 0;
	handle_string_norm_3(&idx, &out_idx, &sp, &var_content);
	while (idx < ssize)
	{
		if (str[idx] == '\"' && !(quote_mode & 2))
			quote_mode ^= 1;
		else if (str[idx] == '\'' && !(quote_mode & 1))
			quote_mode ^= 2;
		handle_string_norm((t_handle_string_norm){str, ssize, sc, quote_mode,
			var_content, &idx, &out_idx, &sp});
	}
	sp.result = ft_memcpy(ft_calloc(1, out_idx + 1), sp.arg_buffer, out_idx);
	return (free(sp.env_var), free(sp.arg_buffer), sp.result);
}

t_uint8_t	handle_quote(struct s_handle_quote q)
{
	if (q.prompt[*q.idx] == '\"' && !(*q.quote_mode & 2))
	{
		if ((*q.quote_mode & 1) && *q.stash_idx == 0)
		{
			q.char_stash[0] = 0;
			(*q.stash_idx)++;
		}
		*q.quote_mode ^= 1;
		if (q.prompt[++(*q.idx)] == '\"' || q.prompt[*q.idx] == '\'')
			return (handle_quote(q), 1);
		return (1);
	}
	else if (q.prompt[*q.idx] == '\'' && !(*q.quote_mode & 1))
	{
		if ((*q.quote_mode & 2) && *q.stash_idx == 0)
		{
			q.char_stash[0] = 0;
			(*q.stash_idx)++;
		}
		*q.quote_mode ^= 2;
		if (q.prompt[++(*q.idx)] == '\'' || q.prompt[*q.idx] == '\"')
			return (handle_quote(q), 1);
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
