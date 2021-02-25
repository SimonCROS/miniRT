/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 13:45:01 by scros             #+#    #+#             */
/*   Updated: 2021/02/25 16:14:49 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "get_next_line.h"

char	**ft_split_first(char *s, char c)
{
	char	**parts;
	size_t	i;
	size_t	len;

	if (!(parts = malloc(sizeof(parts) * 2)))
		return (NULL);
	len = 0;
	i = 0;
	parts[1] = NULL;
	while (s[i] && s[i++] != c)
		;
	if (i == 0)
		i++;
	if (s[i - 1] == c)
		parts[1] = s + i;
	if (!parts[1])
		len = -1;
	else
		len += parts[1] - s - 1;
	if (!(parts[0] = ft_substr(s, 0, len)))
		return (NULL);
	if (parts[1] && !(parts[1] = ft_substr(parts[1], 0, -1)))
		return (NULL);
	free(s);
	return (parts);
}

int		load_remain(t_list *remain, char **line, char ***current)
{
	if (remain->content)
	{
		if (!(*current = ft_split_first(remain->content, '\n')))
			return (-1);
		free(*line);
		*line = (*current)[0];
	}
	return (1);
}

void	free_reader(void *reader)
{
	free(((t_file_reader*) reader)->buffer);
	free(reader);
}

void	*new_reader(void *fd)
{
	t_file_reader	*reader;

	reader = malloc(sizeof(t_file_reader));
	if (!reader)
		return (NULL);
	reader->fd = *(int*)fd;
	return (reader);
}

void	is_fd(void *reader, void *fd)
{
	return (((t_file_reader*)reader)->fd == *(int*)fd);
}

int		get_next_line(int fd, char **line)
{
	static t_list	*remain_lst;
	void			*optional_current;
	t_file_reader	*current;
	ssize_t			result;

	if (BUFFER_SIZE < 1 || read(fd, NULL, 0))
		return (-1);
	if (!remain_lst)
		remain_lst = ft_lst_new(free);
	if (!remain_lst)
		return (-1);
	optional_current = lst_first(lst_filter(is_fd, &fd));
	current = (t_file_reader*)o_or_else_get(optional_current, new_reader, &fd);
	while (1)
	{
		current = malloc(BUFFER_SIZE + 1);
		result = read(fd, current, BUFFER_SIZE);
		
	}
}
