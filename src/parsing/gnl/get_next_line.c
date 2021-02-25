/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: praclet <praclet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 09:14:07 by praclet           #+#    #+#             */
/*   Updated: 2020/12/13 09:58:31 by praclet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

// TODO replace by my gnl

#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "get_next_line.h"

static char	*gnl_new_line(t_file *file, char *s1)
{
	if (file->start > file->end || (file->state == 0 && file->start == -1))
		return (gnl_concat(s1, "", 0));
	return (gnl_concat(s1, file->buffer + file->start,
			file->pos < 0 ? file->end - file->start + 1 : file->pos));
}

static void	ft_next_stop(t_file *file)
{
	char	*cur;
	char	*min;
	char	*max;

	if (file->start < 0 || file->end < 0
		|| file->start >= BUFFER_SIZE || file->end >= BUFFER_SIZE)
	{
		file->pos = -1;
		return ;
	}
	cur = file->buffer + file->start;
	min = cur;
	max = file->buffer + file->end;
	while (cur <= max)
	{
		if (*cur == '\n')
		{
			file->pos = cur - min;
			return ;
		}
		cur++;
	}
	file->pos = -1;
}

static int	gnl_fill_buffer(t_file *file)
{
	if (file->state < 0)
		return (-1);
	if (file->state == 0
		|| (file->start >= 0 && file->start <= file->end))
	{
		ft_next_stop(file);
		return (file->state);
	}
	file->state = read(file->fd, file->buffer, BUFFER_SIZE);
	if (file->state < 0)
	{
		file->start = -1;
		file->end = -1;
		file->pos = -1;
		return (file->state);
	}
	if (file->state)
	{
		file->start = 0;
		file->end = file->state - 1;
	}
	file->state = !!file->state;
	ft_next_stop(file);
	return (file->state);
}

static int	gnl_update_file(t_gnllist **dir, t_file **file, int rc)
{
	if (rc < 0 || (*file)->state < 0
		|| ((*file)->state == 0 && (*file)->pos == -1))
		gnllst_remove(dir, file);
	else
	{
		if ((*file)->pos < 0)
		{
			(*file)->start = -1;
			(*file)->end = -1;
			(*file)->pos = -1;
		}
		else
			(*file)->start += (*file)->pos + 1;
	}
	return (rc);
}

int			get_next_line(int fd, char **line)
{
	static t_gnllist	*dir;
	t_file			*file;
	int				tmp;

	if (BUFFER_SIZE <= 0 || !(file = gnllst_get(&dir, fd)))
		return (-1);
	if ((tmp = gnl_fill_buffer(file)) < 0)
		return (gnl_update_file(&dir, &file, tmp));
	*line = gnl_new_line(file, NULL);
	if (!*line)
		return (gnl_update_file(&dir, &file, -1));
	(void)gnl_update_file(&dir, &file, 1);
	while (file && file->pos < 0 && file->state > 0)
	{
		if ((tmp = gnl_fill_buffer(file)) < 0)
			return (gnl_update_file(&dir, &file, -1));
		if (tmp)
			*line = gnl_new_line(file, *line);
		if (!*line)
			return (gnl_update_file(&dir, &file, -1));
		(void)gnl_update_file(&dir, &file, 1);
	}
	return (file ? 1 : 0);
}
