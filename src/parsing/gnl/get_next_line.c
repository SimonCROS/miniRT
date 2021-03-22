#include "minirt.h"

#include <stdio.h>
static int	delete(int ret, t_gnl_entry **remain, t_gnl_entry *element, void *p)
{
	t_gnl_entry	*elem;

	if (remain && *remain && element)
	{
		if (element == *remain)
			*remain = element->next;
		else if (*remain)
		{
			elem = *remain;
			while (elem)
			{
				if (elem->next == element)
				{
					elem->next = element->next;
					break ;
				}
				elem = elem->next;
			}
		}
		free(element->content);
		free(element);
	}
	if (p)
		free(p);
	return (ret);
}

static int	get_or_create_remain(t_gnl_entry **remain, int fd,
	t_gnl_entry **buf)
{
	t_gnl_entry	*new_element;
	t_gnl_entry	*elem;

	elem = *remain;
	while (elem)
	{
		if (elem->fd == fd)
		{
			*buf = elem;
			return (TRUE);
		}
		elem = elem->next;
	}
	new_element = malloc(sizeof(*new_element));
	if (!new_element)
		return (FALSE);
	new_element->content = NULL;
	new_element->fd = fd;
	new_element->next = *remain;
	*remain = new_element;
	*buf = new_element;
	return (TRUE);
}

static ssize_t	read_line(int fd, char **line, char ***current,
	t_gnl_entry *remain)
{
	char	*tmp;
	ssize_t	result;

	remain->content = malloc(BUFF_SIZE + 1);
	if (!remain->content)
		return (-1);
	result = read(fd, remain->content, BUFF_SIZE);
	if (result < 0)
		return (-1);
	free(*current);
	remain->content[result] = 0;
	*current = ft_split_first(remain->content, '\n');
	if (!current)
		return (-1);
	if (result)
	{
		tmp = *line;
		*line = ft_strjoin(tmp, (*current)[0]);
		if (!line)
			return (-1);
		free(tmp);
	}
	free((*current)[0]);
	return (result);
}

static int	load_remain(t_gnl_entry *remain, char **line, char ***current)
{
	if (remain->content)
	{
		*current = ft_split_first(remain->content, '\n');
		if (!*current)
			return (-1);
		free(*line);
		*line = (*current)[0];
	}
	return (1);
}

int	get_next_line(int fd, char **line)
{
	static t_gnl_entry	*remain_lst;
	char				**current;
	char				*tmp_line;
	t_gnl_entry			*remain;
	ssize_t				result;

	if (!gnl_init(&current, &tmp_line, &result))
		return (-1);
	if (!get_or_create_remain(&remain_lst, fd, &remain))
		return (delete(-1, NULL, NULL, tmp_line));
	if (load_remain(remain, &tmp_line, &current) == -1)
		return (delete(-1, &remain_lst, remain, tmp_line));
	while (result == BUFF_SIZE && (!current || !current[1]))
	{
		result = read_line(fd, &tmp_line, &current, remain);
		if (result == -1)
			return (delete(-1, &remain_lst, remain, tmp_line));
	}
	remain->content = current[1];
	free(current);
	*line = tmp_line;
	if (result == BUFF_SIZE || (result && remain->content))
		return (1);
	return (delete(0, &remain_lst, remain, NULL));
}
