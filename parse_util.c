/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonkim <seonkim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 18:26:51 by seonkim           #+#    #+#             */
/*   Updated: 2021/10/20 19:07:57 by jeonpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int chk_quotes(char *str, char quotes)
{
    int cnt;
    int quote_cnt;

    cnt = 0;
    quote_cnt = 0;
    while (quote_cnt != 2 && *str)
    {
        if (*str == quotes)
            quote_cnt++;
        cnt++;
        str++;
    }
    return (cnt);
}

int     token_len(char  *str, char quotes)
{
    int i;

    i = 0;
	if (!str)
		return (0);
    if (chk_symbol(str))
        return (chk_symbol(str));
    if (count_fd(str))
        return (count_fd(str));
    if (chk_redirect(str))
        return (chk_redirect(str));
    if (quotes == 34 || quotes == 39)
        return(chk_quotes(str, quotes));
    while (*str && !(*str == 32 || *str == 9) && !chk_symbol(str) &&
            !chk_redirect(str) && !count_fd(str))
	{
        if (*str == '\'' || *str == '\"')
            break ;
		str++;
        i++;
	}
    return (i);
}

int     check_dollar(char *line, int size)
{
    int i;

    i = -1;
    while (++i < size)
        if (line[i] == '$' && (line[i + 1] != 32 || line[i + 1] != 9))
            return (1);
    return (0);
}

int     get_arr_size(char *line, int size, int quotes)
{
    int i;
    int token;

    i = 0;
    token = 0;
    while (i < size)
    {
        if (line[i] == '$')
        {
			i++;
            while (line[i] && line[i] != quotes && line[i] != 32 && line[i] != '$')
                i++;
            token++;
        }
        else
        {
            while (line[i] && line[i] != '$')
                i++;
            token++;
        }
    }
    return (token);
}

char    *get_line(char *line, int size)
{
    int i;
    char *ret;

    i = -1;
    ret = malloc(size + 1);
    while (++i < size)
        ret[i] = line[i];
    ret[i] = 0;
    return (ret);
}

char    **split_line(char *line, int size, int quotes)
{
    char **arr;
    int i;
    int j;
    int token;

    i = 0;
    j = 0;
    arr = (char **)malloc(sizeof(char *) * get_arr_size(line, size, quotes) + 1);
    while (i < size)
    {
        token = 0;
        if (line[i] == '$')
        {
			token++;
			i++;
            while (line[i] && line[i] != quotes && line[i] != 32 && line[i] != '$')
			{
				i++;
                token++;
			}
            arr[j++] = get_line(&line[i - token], token);
        }
        else
        {
            while (line[i] && line[i] != '$')
			{
				i++;
                token++;
			}
            arr[j++] = get_line(&line[i - token], token);
        }
    }
    arr[j] = NULL;
    return (arr);
}

char    *line_to_environ(char *key, char **env)
{
	key++;
    while (*env)
    {
		if (ft_strcmp(key, *env))
        {
            *env += cmd_len(key) + 1;
			key--;
			free(key);
            return (ft_strdup(*env));
        }
        env++;
    }
    return (ft_strdup(""));
}

char    *ft_strjoin(char *s1, char *s2)
{
    int		s1_len;
	int		s2_len;
	char	*ret;
	char	*p;

	if (!s1 && !s2)
		return (0);
	s1_len = cmd_len(s1);
	s2_len = cmd_len(s2);
	if (!(ret = (char *)malloc(s1_len + s2_len + 1)))
		return (0);
	p = ret;
	while (s1 && *s1)
		*(p++) = *(s1++);
	while (*s2)
		*(p++) = *(s2++);
	*p = 0;
	if (s1)
    	free(s1 - s1_len);
	return (ret);
}

char    *split_to_join(char **split)
{
    char *ptr;
    int len;

    len = 0;
	ptr = NULL;
    while (*split)
    {
        ptr = ft_strjoin(ptr, *split);
        free(*split);
        split++;
    }
    return (ptr);
}

char    *switch_line_to_environ(char *line, int size, char **env, int quotes)
{
    char **split;
    char **arr;
    char *ret;

    split = split_line(line, size, quotes);
    arr = split;
    while (*arr)
    {
        if (**arr == '$' && !ft_strcmp(*arr, "$?"))
            *arr = line_to_environ(*arr, env);
        arr++;
    }
    ret = split_to_join(split);
    free(split);
    return (ret);
}

char    *dup_line(char *line, int size, int quotes, char **env)
{
    char *ret;
    int i;

    if (check_dollar(line, size) && quotes != '\'')
        return (switch_line_to_environ(line, size, env, quotes));
    ret = malloc(size + 1);
    i = -1;
    while (++i < size)
        ret[i] = line[i];
    ret[i] = 0;
    return(ret);
}

int    line_cpy(t_token *ptr, char *line, char **env)
{
    int i;
    int size;
    char quotes;

    i = 0;
    quotes = 0;
    while (ptr->token[i])
        i++;
    if (*line == '\'' || *line == '\"')
        quotes = *line;
    size = token_len(line, quotes);
    ptr->token[i] = dup_line(line, size, quotes, env);
    ptr->size++;
    ptr->token[++i] = 0;
    return (size);
}

int     ft_strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    while (*s1 && *s2)
    {
        if (*s1++ != *s2++)
            return (0);
        i++;
    }
    if (!i)
        return (0);
    return (1);
}

int     ft_strcmp2(char *s1, char *s2)
{
    while (*s1 || *s2)
        if (*s1++ != *s2++)
            return (0);
    if (*s1 || *s2)
        return (0);
    return (1);
}

int     ft_strcmp3(char *s1, char *s2, int size)
{
    int i;

    i = 0;
    if (!size)
        return (0);
    while (*s1 && *s2 && i < size)
    {
        if (*s1++ != *s2++)
            return (0);
        i++;
    }
    return (1);
}
