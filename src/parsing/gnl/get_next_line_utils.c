/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: praclet <praclet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/28 04:19:28 by praclet           #+#    #+#             */
/*   Updated: 2020/12/13 16:59:38 by praclet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

// TODO replace by my gnl

#include <stdlib.h>
#include "libft.h"
#include "get_next_line.h"

static t_file	*gnllst_add(t_gnllist **list, int fd)
{
	t_gnllist *res;
	t_file *data;

	res = (t_gnllist *)malloc(sizeof(t_gnllist));
	data = (t_file *)malloc(sizeof(t_file));
	if (!res || !data)
	{
		free(res);
		free(data);
		return (NULL);
	}
	data->fd = fd;
	data->state = 1;
	data->start = -1;
	data->end = -1;
	data->pos = -1;
	res->data = data;
	res->next = *list;
	*list = res;
	return ((t_file *)res->data);
}

t_file			*gnllst_get(t_gnllist **list, int fd)
{
	t_gnllist *lst;

	lst = *list;
	while (lst)
	{
		if (((t_file *)lst->data)->fd == fd)
			return ((t_file *)lst->data);
		lst = lst->next;
	}
	return (gnllst_add(list, fd));
}

void			gnllst_remove(t_gnllist **list, t_file **file)
{
	t_gnllist *prev;
	t_gnllist *cur;

	prev = NULL;
	cur = *list;
	while (cur)
	{
		if (cur->data == *file)
		{
			if (prev)
				prev->next = cur->next;
			else
				*list = cur->next;
			free(cur->data);
			free(cur);
			*file = NULL;
			break ;
		}
		prev = cur;
		cur = cur->next;
	}
}

char			*gnl_concat(char *s1, char *s2, size_t len2)
{
	char	*res;
	char	*tmp;
	char	*tmp_s1;

	if (!s1 && !s2)
		return (NULL);
	if (!s2)
		return (s1);
	if (!(res = (char *)malloc((ft_strlen(s1) + len2 + 1) * sizeof(char))))
	{
		free(s1);
		return (NULL);
	}
	tmp = res;
	tmp_s1 = s1;
	if (s1)
		while (*s1)
			*tmp++ = *s1++;
	if (s1)
		free(tmp_s1);
	if (s2)
		while (len2--)
			*tmp++ = *s2++;
	*tmp = 0;
	return (res);
}
