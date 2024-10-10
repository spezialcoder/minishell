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

extern void	stash_it(t_uint32_t *stash_idx, char *char_stash, t_prompt *prompt);
extern void	handle_redirect(char *prompt, t_uint64_t *idx, t_prompt *data,
				char *char_stash);
extern void	handle_string_norm_2(t_handle_string_norm *d);

void	del_environ_norm(t_environment *env, const char *key, t_size_t idx,
		t_size_t new_idx)
{
	char	**new_key_var;
	char	**new_value_var;

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

void	handle_redirect_norm(const char *prompt, t_uint64_t *idx,
		t_redirect *new_redirect)
{
	if (!ft_memcmp(&prompt[*idx], "<<", 2))
	{
		new_redirect->type = R_DELIMITER;
		(*idx)++;
	}
	else if (!ft_memcmp(&prompt[*idx], ">>", 2))
	{
		new_redirect->type = R_FILE_APPEND;
		(*idx)++;
	}
	else if (prompt[*idx] == '<')
	{
		new_redirect->type = R_FILE_INPUT;
	}
	else
		new_redirect->type = R_FILE_OUTPUT;
}

void	parse_prompt_norm(t_parse_prompt_norm d)
{
	while ((*d.idx) < ft_strlen(d.prompt))
	{
		handle_quote((struct s_handle_quote){d.prompt, d.idx, d.quote_mode,
			d.char_stash, d.stash_idx});
		if ((!ft_memcmp(&d.prompt[(*d.idx)], "<<", 2)
				|| !ft_memcmp(&d.prompt[(*d.idx)], ">>", 2)
				|| d.prompt[(*d.idx)] == '<' || d.prompt[(*d.idx)] == '>')
			&& !(*d.quote_mode))
		{
			stash_it(d.stash_idx, d.char_stash, d.result);
			handle_redirect(d.prompt, d.idx, d.result, d.char_stash);
		}
		else if (d.prompt[(*d.idx)] == ' ' && !(*d.quote_mode))
			stash_it(d.stash_idx, d.char_stash, d.result);
		else if (d.prompt[(*d.idx)] == '|' && !(*d.quote_mode))
		{
			d.result->pipe = parse_prompt(d.prompt + (*d.idx) + 1, d.sc);
			break ;
		}
		else
		{
			d.char_stash[(*d.stash_idx)++] = d.prompt[(*d.idx)];
		}
		(*d.idx)++;
	}
}

void	handle_string_norm(t_handle_string_norm d)
{
	if (d.str[(*d.idx)] == '$' && !(d.quote_mode & 2))
	{
		if (d.str[(*d.idx) + 1] == '?')
		{
			d.var_content = ft_itoa(d.sc->recent_exit_code);
			(*d.idx)++;
		}
		else
			handle_string_norm_2(&d);
		if (d.var_content)
		{
			(*d.sp).tmp_var_len = ft_strlen(d.var_content);
			ft_memcpy(&(*d.sp).arg_buffer[(*d.out_idx)], d.var_content,
				(*d.sp).tmp_var_len);
			(*d.out_idx) += (*d.sp).tmp_var_len;
		}
		if (d.str[(*d.idx)] == '?')
		{
			free(d.var_content);
			(*d.idx)++;
		}
		(*d.sp).stash_idx = 0;
	}
	else
		(*d.sp).arg_buffer[(*d.out_idx)++] = d.str[(*d.idx)++];
}
