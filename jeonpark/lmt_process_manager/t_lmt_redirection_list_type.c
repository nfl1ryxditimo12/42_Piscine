/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_lmt_redirection_list_type.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeonpark <jeonpark@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 15:32:18 by jeonpark          #+#    #+#             */
/*   Updated: 2021/07/16 13:24:04 by jeonpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "t_lmt_process_manager.h"

//	stdlib.h: free(), NULL

static t_lmt_redirection_list	*lmt_redirection_list_alloc(void)
{
	return (lmt_alloc(sizeof(t_lmt_redirection_list)));
}

static void	lmt_redirection_list_init(t_lmt_redirection_list *list)
{
	list->p_dummy = lmt_redirection_new(TYPE_NONE, -1, -1, NULL);
	list->last = list->p_dummy;
}

t_lmt_redirection_list	*lmt_redirection_list_new(void)
{
	t_lmt_redirection_list	*list;

	list = lmt_redirection_list_alloc();
	lmt_redirection_list_init(list);
	return (list);
}

//	- 인자:
//		- list: free 할 redirection list
//		- option:
//			- REDIRECTION_FREE_NORMAL: 각 redirection 의 fd_new 를 close 하지 않는다
//			- REDIRECTION_FREE_FD: 각 redirection 의 fd_new 를 close 한다
void	lmt_redirection_list_free(t_lmt_redirection_list *list, int option)
{
	t_lmt_redirection	*iterator;
	t_lmt_redirection	*next;

	iterator = list->p_dummy->next;
	while (iterator != NULL)
	{
		next = iterator->next;
		lmt_redirection_free(iterator, option);
		iterator = next;
	}
	free(list->p_dummy);
	free(list);
}
