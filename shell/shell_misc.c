/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_misc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsorg <lsorg@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:24:13 by lsorg             #+#    #+#             */
/*   Updated: 2024/09/23 19:30:13 by lsorg            ###   ########.fr       */
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

void free_t_redirect(void *addr) {
	t_redirect *redirect;

	redirect = (t_redirect*)addr;
	free(redirect->prompt);
	free(redirect);
}

void free_t_process(void *addr) {
    t_process *ps;

    ps = (t_process*)addr;
    free(ps->cmd);
    free_split_array(ps->argv);
    free((void*)ps);
}

void ft_lstpop(t_list **lst, void (*del)(void*)) {
    t_list *current;
    t_list *prev;

    if(!lst) return;
    prev = NULL;
    current = *lst;
    if(current->next) {
        while(current->next) {
            prev = current;
            current = current->next;
        }
        if(prev) {
            prev->next = NULL;
        }
    } else {
        *lst = NULL;
    }
    ft_lstdelone(current,del);

}