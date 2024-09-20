/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_misc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:24:13 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/20 15:29:14 by lsorg            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	free_split_array(char **arr)
{
	uint32_t	idx;

	idx = 0;
	while (arr[idx])
		free(arr[idx++]);
	free(arr);
}

char	**list_to_array(t_list *lst)
{
    t_list *current;
	char **array;
    int idx;

	array = (char **)ft_calloc(ft_lstsize(lst)+1 ,sizeof(char *));
    current = lst;
    idx = 0;
    while(current) {
        array[idx++] = ft_memcpy(ft_calloc(1,ft_strlen((char*)current->content)+1),
                               current->content, ft_strlen((char*)current->content));
        current = current->next;
    }
    return (array);
}
