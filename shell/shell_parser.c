/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:24:13 by lsorg             #+#    #+#             */
/*   Updated: 2024/10/09 17:52:46 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void		stash_it(t_uint32_t *stash_idx, char *char_stash, t_prompt *prompt);
void		handle_redirect(char *prompt, t_uint64_t *idx, t_prompt *data,
				char *char_stash);

t_prompt	*parse_prompt(char *prompt, t_shell *sc)
{
	t_uint64_t	idx;
	t_prompt	*result;
	char		*char_stash;
	t_uint32_t	stash_idx;
	t_uint8_t	quote_mode;

	result = (t_prompt *)ft_calloc(1, sizeof(t_prompt));
	result->argc = 0;
	result->cmd = NULL;
	char_stash = (char *)malloc(sizeof(char) * STASH_SIZE);
	idx = 0;
	stash_idx = 0;
	quote_mode = 0;
	parse_prompt_norm((t_parse_prompt_norm){prompt, sc, &idx, result,
		char_stash, &stash_idx, &quote_mode});
	stash_it(&stash_idx, char_stash, result);
	if (quote_mode)
	{
		printf("ERROR: syntax error unclosed quote\n");
		return (free_prompt(result), free(char_stash), NULL);
	}
	return (free(char_stash), result);
}

void	stash_it(t_uint32_t *stash_idx, char *char_stash, t_prompt *prompt)
{
	char	*result;

	if (prompt->argc == 0 && *stash_idx > 0)
	{
		result = (char *)ft_calloc(1, *stash_idx + 1);
		ft_memcpy(result, char_stash, *stash_idx);
		prompt->cmd = result;
	}
	if (*stash_idx > 0)
	{
		result = (char *)ft_calloc(1, *stash_idx + 1);
		ft_memcpy(result, char_stash, *stash_idx);
		ft_lstadd_back(&prompt->parameter, ft_lstnew((void *)result));
		*stash_idx = 0;
		prompt->argc++;
	}
}

void	handle_redirect(char *prompt, t_uint64_t *idx, t_prompt *data,
		char *char_stash)
{
	t_uint32_t	stash_idx;
	t_redirect	*new_redirect;

	new_redirect = ft_calloc(1, sizeof(t_redirect));
	handle_redirect_norm(prompt, idx, new_redirect);
	(*idx)++;
	while (prompt[*idx] == ' ')
		(*idx)++;
	stash_idx = 0;
	while (*idx < ft_strlen(prompt))
	{
		if (prompt[*idx] == ' ' || prompt[*idx] == '<' || prompt[*idx] == '>'
			|| prompt[*idx] == '|')
		{
			(*idx)--;
			break ;
		}
		char_stash[stash_idx++] = prompt[(*idx)++];
	}
	new_redirect->prompt = (char *)ft_calloc(1, stash_idx + 1);
	ft_memcpy(new_redirect->prompt, char_stash, stash_idx);
	ft_lstadd_back(&data->redirect, ft_lstnew((void *)new_redirect));
}
