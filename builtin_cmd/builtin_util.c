/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonkim <seonkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 18:04:21 by seonkim           #+#    #+#             */
/*   Updated: 2021/07/07 14:53:05 by seonkim          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int  cmd_len(char *str)
{
    int i;

    i = 0;
    if (!str && !*str)
        return (0);
    while (str[i])
        i++;
    return (i);
}

char    *ft_strdup(char *str)
{
    int i;
    char    *ret;

    i = 0;
    ret = malloc(cmd_len(str) + 1);
    while (*str)
        ret[i++] = *str++;
    ret[i] = 0;
    return (ret);
}