/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lmt_tokenize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeonpark <jeonpark@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/16 14:04:53 by jeonpark          #+#    #+#             */
/*   Updated: 2021/07/16 17:08:41 by jeonpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lmt_tokenize.h"
#include "minishell.h"
#include "lmt_util.h"

int	lmt_is_token_type_operator(t_token *p_token)
{
	if (p_token == NULL)
		return (0);
	return (lmt_is_type_operator(p_token->type));
}

int	lmt_is_token_type_redirection(t_token *p_token)
{
	if (p_token == NULL)
		return (0);
	return (lmt_is_type_redirection(p_token->type));
}

void	lmt_tokenize_parse(t_token *iterator)
{
	while (iterator != NULL)
	{
		//	compose tokenize code here..
		iterator = iterator->next;
	}
}
